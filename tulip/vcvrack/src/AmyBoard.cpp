// AMY synthesizer engine as a VCV Rack module — phase 1 of the AMYboard
// Rack port. Embeds the AMY core (in-process, like the TULIP_DESKTOP build),
// with Rack MIDI in driving AMY's own MIDI handling (default synths: Juno-6
// ch1, DX7 ch2, GM drums ch10) and the two CV inputs exposed to AMY as
// ext0/ext1 CtrlCoefs + cv_trigger sources, matching AMYboard hardware.

#include "plugin.hpp"

extern "C" {
#include "amy.h"
// In api.c but not exported via amy.h.
void amy_default_synths(void);
}
// amy.h defines true/false macros; keep them out of C++ code below.
#undef true
#undef false

struct AmyModule;
static AmyModule* s_owner = NULL;
static float s_cvVolts[AMY_MAX_CV_IN] = {};

// AMY's CV-input hook (audio-thread): feeds ext0/ext1 CtrlCoefs and
// cv_trigger, same seam the hardware's ADS1015 reader uses.
static float cvCoefHook(uint16_t channel) {
    if (channel >= AMY_MAX_CV_IN)
        return 0.f;
    return s_cvVolts[channel];
}

struct AmyModule : Module {
    enum ParamId {
        LEVEL_PARAM,
        PARAMS_LEN
    };
    enum InputId {
        CV1_INPUT,
        CV2_INPUT,
        INPUTS_LEN
    };
    enum OutputId {
        LEFT_OUTPUT,
        RIGHT_OUTPUT,
        OUTPUTS_LEN
    };
    enum LightId {
        MIDI_LIGHT,
        LIGHTS_LEN
    };

    midi::InputQueue midiInput;
    bool owner = false;

    dsp::SampleRateConverter<2> outputSrc;
    dsp::DoubleRingBuffer<dsp::Frame<2>, 2048> outputBuffer;

    AmyModule() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configParam(LEVEL_PARAM, 0.f, 2.f, 1.f, "Level", "%", 0.f, 100.f);
        configInput(CV1_INPUT, "CV 1 (AMY ext0 / cv_trigger 0)");
        configInput(CV2_INPUT, "CV 2 (AMY ext1 / cv_trigger 1)");
        configOutput(LEFT_OUTPUT, "Left");
        configOutput(RIGHT_OUTPUT, "Right");

        // AMY (and its default synth setup) is global state: exactly one
        // module instance owns the engine. Extra instances stay silent.
        if (!s_owner) {
            amy_config_t cfg = amy_default_config();
            cfg.audio = AMY_AUDIO_IS_NONE;   // we pull blocks ourselves
            cfg.midi = AMY_MIDI_IS_NONE;     // Rack owns MIDI devices
            cfg.platform.multicore = 0;
            cfg.platform.multithread = 0;
            cfg.features.chorus = 1;
            cfg.features.reverb = 1;
            cfg.features.echo = 1;
            cfg.features.audio_in = 0;
            cfg.features.default_synths = 1;
            cfg.features.startup_bleep = 1;
            cfg.amy_external_coef_hook = cvCoefHook;
            amy_start(cfg);
            s_owner = this;
            owner = true;
        }
    }

    ~AmyModule() override {
        if (owner) {
            amy_stop();
            s_owner = NULL;
        }
    }

    void onReset() override {
        if (owner) {
            amy_add_message((char*)"S");  // AMY full reset
            amy_default_synths();
        }
        midiInput.reset();
    }

    void process(const ProcessArgs& args) override {
        if (!owner) {
            outputs[LEFT_OUTPUT].setVoltage(0.f);
            outputs[RIGHT_OUTPUT].setVoltage(0.f);
            return;
        }

        s_cvVolts[0] = inputs[CV1_INPUT].getVoltage();
        s_cvVolts[1] = inputs[CV2_INPUT].getVoltage();

        // Drain Rack MIDI into AMY's MIDI engine (drives the default synths,
        // program changes, CCs; sysex passes through for future editor use).
        midi::Message msg;
        while (midiInput.tryPop(&msg, args.frame)) {
            size_t len = msg.bytes.size();
            if (!len)
                continue;
            uint8_t sysex = (msg.bytes[0] == 0xF0);
            amy_event_midi_message_received(msg.bytes.data(), len, sysex, amy_sysclock());
            lights[MIDI_LIGHT].setBrightness(1.f);
        }
        lights[MIDI_LIGHT].setBrightnessSmooth(0.f, args.sampleTime);

        // AMY renders fixed 256-frame stereo int16 blocks at AMY_SAMPLE_RATE;
        // refill the ring (resampling if the engine rate differs) and hand
        // out one frame per process() call.
        if (outputBuffer.empty())
            renderBlock((int)args.sampleRate);

        dsp::Frame<2> frame = {};
        if (!outputBuffer.empty())
            frame = outputBuffer.shift();

        float gain = 5.f * params[LEVEL_PARAM].getValue();
        outputs[LEFT_OUTPUT].setVoltage(gain * frame.samples[0]);
        outputs[RIGHT_OUTPUT].setVoltage(gain * frame.samples[1]);
    }

    void renderBlock(int engineRate) {
        int16_t* block = amy_simple_fill_buffer();
        dsp::Frame<2> in[AMY_BLOCK_SIZE];
        for (int i = 0; i < AMY_BLOCK_SIZE; i++) {
            in[i].samples[0] = block[2 * i] / 32768.f;
            in[i].samples[1] = block[2 * i + 1] / 32768.f;
        }

        if (engineRate == AMY_SAMPLE_RATE) {
            for (int i = 0; i < AMY_BLOCK_SIZE; i++) {
                if (outputBuffer.capacity() == 0)
                    break;
                outputBuffer.push(in[i]);
            }
        }
        else {
            outputSrc.setRates(AMY_SAMPLE_RATE, engineRate);
            int inLen = AMY_BLOCK_SIZE;
            int outLen = outputBuffer.capacity();
            outputSrc.process(in, &inLen, outputBuffer.endData(), &outLen);
            outputBuffer.endIncr(outLen);
        }
    }

    json_t* dataToJson() override {
        json_t* rootJ = json_object();
        json_object_set_new(rootJ, "midi", midiInput.toJson());
        return rootJ;
    }

    void dataFromJson(json_t* rootJ) override {
        json_t* midiJ = json_object_get(rootJ, "midi");
        if (midiJ)
            midiInput.fromJson(midiJ);
    }
};

// Panel labels drawn in code (Rack's SVG renderer ignores <text> elements).
struct AmyLabels : TransparentWidget {
    struct Label {
        float x, y, size;
        const char* text;
    };
    std::vector<Label> labels;
    NVGcolor color = nvgRGB(0xe8, 0xea, 0xf0);

    void draw(const DrawArgs& args) override {
        std::shared_ptr<window::Font> font =
            APP->window->loadFont(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
        if (!font)
            return;
        nvgFontFaceId(args.vg, font->handle);
        nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgFillColor(args.vg, color);
        for (const Label& l : labels) {
            nvgFontSize(args.vg, l.size);
            nvgText(args.vg, mm2px(l.x), mm2px(l.y), l.text, NULL);
        }
    }
};

struct AmyWidget : ModuleWidget {
    AmyWidget(AmyModule* module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/AMYboard.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.32, 40.0)), module, AmyModule::LEVEL_PARAM));
        addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(20.32, 56.0)), module, AmyModule::MIDI_LIGHT));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.5, 78.0)), module, AmyModule::CV1_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(29.14, 78.0)), module, AmyModule::CV2_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(11.5, 103.0)), module, AmyModule::LEFT_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(29.14, 103.0)), module, AmyModule::RIGHT_OUTPUT));

        AmyLabels* labels = new AmyLabels;
        labels->box.size = box.size;
        labels->labels = {
            {20.32, 10.0, 22.f, "AMY"},
            {20.32, 16.5, 8.f, "SHORE PINE"},
            {20.32, 48.5, 9.f, "LEVEL"},
            {20.32, 60.5, 8.f, "MIDI"},
            {11.5, 85.5, 9.f, "CV1"},
            {29.14, 85.5, 9.f, "CV2"},
            {11.5, 110.5, 9.f, "OUT L"},
            {29.14, 110.5, 9.f, "OUT R"},
            {20.32, 122.0, 8.f, "AMYBOARD"},
        };
        addChild(labels);
    }

    void appendContextMenu(Menu* menu) override {
        AmyModule* m = dynamic_cast<AmyModule*>(module);
        if (!m)
            return;
        menu->addChild(new MenuSeparator);
        menu->addChild(createMenuLabel("MIDI input"));
        appendMidiMenu(menu, &m->midiInput);
        if (!m->owner) {
            menu->addChild(new MenuSeparator);
            menu->addChild(createMenuLabel("Inactive: another AMY module owns the engine"));
        }
    }
};

Model* modelAmy = createModel<AmyModule, AmyWidget>("AMY");
