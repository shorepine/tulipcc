// AMYboard as a VCV Rack module — phase 2: the full firmware.
//
// Embeds MicroPython running the real frozen AMYboard firmware (_boot.py,
// amyboard.py, midi.py, the amy python package) alongside in-process AMY.
// The Rack audio thread pulls AMY blocks; the MP thread runs the sketch at
// ~/Documents/AMYboard/user/current/sketch.py, scheduled by AMY's sequencer
// exactly as on hardware. Panel: 128x128 OLED, four endless encoders with
// push buttons, CV1/CV2 in and out, stereo out, MIDI via context menu.

#include "plugin.hpp"

extern "C" {
#include "amy.h"
// tulip/shared/amy_connector.c (AMYBOARD_VCV arm)
void run_amy(void);
void tsequencer_init(void);
// tulip/vcvrack/src/mp_embed.c
int amyboard_vcv_mp_start(void);
void amyboard_vcv_mp_stop(void);
int amyboard_vcv_mp_booted(void);
void amyboard_vcv_exec(const char *code);
// Host<->firmware seams (modtulip.c / amy_connector.c, AMYBOARD_VCV arms)
extern float amyboard_vcv_cv_in[2];
extern float amyboard_vcv_cv_out[2];
extern int32_t amyboard_vcv_encoder_pos[4];
extern uint8_t amyboard_vcv_encoder_btn[4];
extern uint8_t amyboard_vcv_framebuf[128 * 64];
extern volatile uint32_t amyboard_vcv_framebuf_gen;
}
#undef true
#undef false

struct AmyModule;
static AmyModule* s_owner = NULL;

// One hardware detent per 1/24 turn of the panel encoder knobs.
static const float ENC_DETENTS_PER_TURN = 24.f;

struct AmyModule : Module {
    enum ParamId {
        LEVEL_PARAM,
        ENC1_PARAM, ENC2_PARAM, ENC3_PARAM, ENC4_PARAM,
        BTN1_PARAM, BTN2_PARAM, BTN3_PARAM, BTN4_PARAM,
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
        CV1_OUTPUT,
        CV2_OUTPUT,
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
        for (int i = 0; i < 4; i++) {
            configParam(ENC1_PARAM + i, -INFINITY, INFINITY, 0.f, string::f("Encoder %d", i + 1));
            configButton(BTN1_PARAM + i, string::f("Encoder %d button", i + 1));
        }
        configInput(CV1_INPUT, "CV 1 (ext0 / cv_trigger 0)");
        configInput(CV2_INPUT, "CV 2 (ext1 / cv_trigger 1)");
        configOutput(LEFT_OUTPUT, "Left");
        configOutput(RIGHT_OUTPUT, "Right");
        configOutput(CV1_OUTPUT, "CV 1 (set_cv_out / cv_out)");
        configOutput(CV2_OUTPUT, "CV 2 (set_cv_out / cv_out)");

        // AMY and MicroPython are global state: one module instance owns
        // the whole board. Extra instances stay silent.
        if (!s_owner) {
            run_amy();          // in-process AMY, hooks wired (amy_connector.c)
            tsequencer_init();  // AMY sequencer -> mp_sched_schedule bridge
            amyboard_vcv_mp_start();  // boot the firmware on its own thread
            s_owner = this;
            owner = true;
        }
    }

    ~AmyModule() override {
        if (owner) {
            amyboard_vcv_mp_stop();
            amy_stop();
            s_owner = NULL;
        }
    }

    void onReset() override {
        if (owner && amyboard_vcv_mp_booted())
            amyboard_vcv_exec("import amyboard; amyboard.restart_sketch()");
        midiInput.reset();
    }

    void process(const ProcessArgs& args) override {
        if (!owner) {
            outputs[LEFT_OUTPUT].setVoltage(0.f);
            outputs[RIGHT_OUTPUT].setVoltage(0.f);
            return;
        }

        amyboard_vcv_cv_in[0] = inputs[CV1_INPUT].getVoltage();
        amyboard_vcv_cv_in[1] = inputs[CV2_INPUT].getVoltage();

        // Panel encoders: endless knobs, one detent per 1/24 turn.
        for (int i = 0; i < 4; i++) {
            amyboard_vcv_encoder_pos[i] =
                (int32_t)std::round(params[ENC1_PARAM + i].getValue() * ENC_DETENTS_PER_TURN);
            amyboard_vcv_encoder_btn[i] = params[BTN1_PARAM + i].getValue() > 0.5f;
        }

        // Rack MIDI -> AMY's MIDI engine. The external midi input hook
        // forwards to Python (midi.add_callback) like every other platform.
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

        if (outputBuffer.empty())
            renderBlock((int)args.sampleRate);

        dsp::Frame<2> frame = {};
        if (!outputBuffer.empty())
            frame = outputBuffer.shift();

        float gain = 5.f * params[LEVEL_PARAM].getValue();
        outputs[LEFT_OUTPUT].setVoltage(gain * frame.samples[0]);
        outputs[RIGHT_OUTPUT].setVoltage(gain * frame.samples[1]);

        // CV outs: volts written by AMY's render hook (set_cv_out) or
        // amyboard.cv_out(); block-rate like the hardware DAC.
        outputs[CV1_OUTPUT].setVoltage(amyboard_vcv_cv_out[0]);
        outputs[CV2_OUTPUT].setVoltage(amyboard_vcv_cv_out[1]);
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

// 128x128 4-bit grayscale OLED, rendered from the firmware's framebuffer.
struct OledWidget : TransparentWidget {
    AmyModule* module = NULL;
    int img = -1;
    uint32_t drawnGen = 0;
    uint8_t rgba[128 * 128 * 4] = {};

    void drawLayer(const DrawArgs& args, int layer) override {
        if (layer != 1)
            return;
        // Expand 4bpp (two pixels/byte, 64 bytes/row) to RGBA.
        if (module && module->owner && amyboard_vcv_framebuf_gen != drawnGen) {
            drawnGen = amyboard_vcv_framebuf_gen;
            int i = 0;
            for (int y = 0; y < 128; y++) {
                for (int x = 0; x < 64; x++) {
                    uint8_t two = amyboard_vcv_framebuf[y * 64 + x];
                    uint8_t px[2] = {(uint8_t)((two >> 4) & 0xF), (uint8_t)(two & 0xF)};
                    for (int k = 0; k < 2; k++) {
                        uint8_t v = px[k] * 17;
                        rgba[i++] = v; rgba[i++] = v; rgba[i++] = v; rgba[i++] = 0xFF;
                    }
                }
            }
            if (img >= 0) {
                nvgUpdateImage(args.vg, img, rgba);
            }
        }
        if (img < 0)
            img = nvgCreateImageRGBA(args.vg, 128, 128, 0, rgba);

        NVGpaint paint = nvgImagePattern(args.vg, 0, 0, box.size.x, box.size.y, 0, img, 1.f);
        nvgBeginPath(args.vg);
        nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
        nvgFillPaint(args.vg, paint);
        nvgFill(args.vg);
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

        // OLED: 36x36mm, top center
        OledWidget* oled = new OledWidget;
        oled->module = module;
        oled->box.pos = mm2px(Vec(22.32, 7.0));
        oled->box.size = mm2px(Vec(36.0, 36.0));
        addChild(oled);

        // Encoders + buttons
        static const float encX[4] = {12.0, 31.0, 50.0, 69.0};
        for (int i = 0; i < 4; i++) {
            addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(encX[i], 56.0)), module, AmyModule::ENC1_PARAM + i));
            addParam(createParamCentered<TL1105>(mm2px(Vec(encX[i], 66.5)), module, AmyModule::BTN1_PARAM + i));
        }

        // CV jacks
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.0, 84.0)), module, AmyModule::CV1_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.0, 84.0)), module, AmyModule::CV2_INPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.0, 84.0)), module, AmyModule::CV1_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(69.0, 84.0)), module, AmyModule::CV2_OUTPUT));

        // Level, MIDI light, audio outs
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.0, 108.0)), module, AmyModule::LEVEL_PARAM));
        addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(27.0, 108.0)), module, AmyModule::MIDI_LIGHT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50.0, 108.0)), module, AmyModule::LEFT_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(69.0, 108.0)), module, AmyModule::RIGHT_OUTPUT));

        AmyLabels* labels = new AmyLabels;
        labels->box.size = box.size;
        labels->labels = {
            {11.0, 10.0, 16.f, "AMY"},
            {11.0, 15.5, 6.5f, "BOARD"},
            {12.0, 48.5, 8.f, "ENC1"},
            {31.0, 48.5, 8.f, "ENC2"},
            {50.0, 48.5, 8.f, "ENC3"},
            {69.0, 48.5, 8.f, "ENC4"},
            {12.0, 77.0, 8.f, "CV1 IN"},
            {31.0, 77.0, 8.f, "CV2 IN"},
            {50.0, 77.0, 8.f, "CV1 OUT"},
            {69.0, 77.0, 8.f, "CV2 OUT"},
            {12.0, 100.5, 8.f, "LEVEL"},
            {27.0, 100.5, 8.f, "MIDI"},
            {50.0, 100.5, 8.f, "OUT L"},
            {69.0, 100.5, 8.f, "OUT R"},
            {40.64, 122.5, 7.f, "SHORE PINE SOUND SYSTEMS"},
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
        menu->addChild(new MenuSeparator);
        menu->addChild(createMenuItem("Restart sketch", "", [m]() {
            if (m->owner)
                amyboard_vcv_exec("import amyboard; amyboard.restart_sketch()");
        }));
        menu->addChild(createMenuItem("Stop sketch", "", [m]() {
            if (m->owner)
                amyboard_vcv_exec("import amyboard; amyboard.stop_sketch()");
        }));
        menu->addChild(createMenuItem("Open sketch folder", "", []() {
            std::string home = getenv("HOME") ? getenv("HOME") : "";
            system::openDirectory(home + "/Documents/AMYboard/user/current");
        }));
        if (!m->owner) {
            menu->addChild(new MenuSeparator);
            menu->addChild(createMenuLabel("Inactive: another AMYboard module owns the engine"));
        }
    }
};

Model* modelAmy = createModel<AmyModule, AmyWidget>("AMY");
