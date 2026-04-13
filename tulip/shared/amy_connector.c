// amy_connector.c
// all the stuff that connects Tulip to AMY
// like MIDI queue -> python
// like external CV
// like t-sequencer?
// like alles / wifi stuff?


#include "polyfills.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "amy_connector.h"
#include <stdio.h>
#include <string.h>
uint8_t * external_map;

#ifdef AMY_IS_EXTERNAL
uint8_t * sysex_buffer;
uint16_t sysex_len = 0;
#endif

#ifdef __EMSCRIPTEN__

void midi_out(uint8_t * bytes, uint16_t len) {
    EM_ASM(
            if(midiOutputDevice != null) {
                midiOutputDevice.send(HEAPU8.subarray($0, $0 + $1));
            }, bytes, len
        );
}

#endif

// A queue to store the AMY midi messages coming IN
uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
extern mp_obj_t midi_callback;

int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;


#ifdef ESP_PLATFORM
#include "driver/i2c.h"

// Maps synth number -> CV channel (0 = not mapped, 1 = CV1, 2 = CV2)
// Set from Python via amyboard.set_cv_out(channel, synth)
#define MAX_CV_SYNTHS 32
uint8_t cv_synth_map[MAX_CV_SYNTHS];

// Look up which synth owns this osc, return synth number or -1
static int synth_for_osc(uint16_t osc) {
    extern uint8_t *osc_to_voice;
    if (osc_to_voice == NULL) return -1;
    uint8_t voice = osc_to_voice[osc];
    if (voice == 255) return -1;  // AMY_UNSET for uint8
    uint16_t voices[MAX_VOICES_PER_INSTRUMENT];
    for (int s = 0; s < MAX_CV_SYNTHS; s++) {
        if (cv_synth_map[s] == 0) continue;  // skip unmapped synths
        int nv = instrument_get_num_voices(s, voices);
        for (int v = 0; v < nv; v++) {
            if (voices[v] == voice) return s;
        }
    }
    return -1;
}

// AMY render hook: route osc audio to CV DAC if its synth is mapped
uint8_t external_cv_render(uint16_t osc, SAMPLE * buf, uint16_t len) {
    // First check old per-osc map for backward compat
    if(external_map[osc]>0) {
        uint8_t cv_channel = external_map[osc] - 1;
#ifdef AMYBOARD
        // AMYboard GP8413 DAC at address 88, channels 0x02/0x04
        // Sample range [-1,1] -> volts [-10,+10] -> DAC [0x0000, 0x7FFF]
        float volts = S2F(buf[0]) * 10.0f;
        uint16_t value_int = (uint16_t)(((volts + 10.0f) / 20.0f) * 0x8000);
        if (value_int > 0x7FFF) value_int = 0x7FFF;
        uint8_t reg = (cv_channel == 0) ? 0x02 : 0x04;
        uint8_t bytes[3] = { reg, value_int & 0xFF, (value_int >> 8) & 0xFF };
        i2c_master_write_to_device(I2C_NUM_0, 88, bytes, 3, pdMS_TO_TICKS(10));
#else
        // Tulip CC DAC (different address/format)
        float volts = S2F(buf[0])*2.5f + 2.5f;
        uint16_t value_int = (uint16_t)((volts/10.0) * 65535.0);
        uint8_t bytes[3];
        bytes[2] = (value_int & 0xff00) >> 8;
        bytes[1] = (value_int & 0x00ff);
        uint8_t ch = 0x02;
        uint8_t addr = 89;
        if(cv_channel == 1) ch = 0x04;
        if(cv_channel == 2) addr = 88;
        if(cv_channel == 3) {ch = 0x04; addr=88; }
        bytes[0] = ch;
        i2c_master_write_to_device(I2C_NUM_0, addr, bytes, 3, pdMS_TO_TICKS(10));
#endif
        return 1;
    }
    // Check synth-based CV map
    int s = synth_for_osc(osc);
    if (s >= 0 && s < MAX_CV_SYNTHS && cv_synth_map[s] > 0) {
        uint8_t cv_channel = cv_synth_map[s] - 1;
#ifdef AMYBOARD
        float volts = S2F(buf[0]) * 10.0f;
        uint16_t value_int = (uint16_t)(((volts + 10.0f) / 20.0f) * 0x8000);
        if (value_int > 0x7FFF) value_int = 0x7FFF;
        uint8_t reg = (cv_channel == 0) ? 0x02 : 0x04;
        uint8_t bytes[3] = { reg, value_int & 0xFF, (value_int >> 8) & 0xFF };
        i2c_master_write_to_device(I2C_NUM_0, 88, bytes, 3, pdMS_TO_TICKS(10));
#endif
        return 1;
    }
    return 0;
}
#endif

// defined in amy/src/midi_mappings.c — processes ic (MIDI CC mapping) commands
// On web, AMY runs in a separate wasm worker so midi_cc_handler is not linkable
#ifndef __EMSCRIPTEN__
extern void midi_cc_handler(uint8_t * bytes, uint16_t len, uint8_t is_sysex);
#endif

// I am called when AMY receives MIDI in, whether it has been processed (played in a instrument) or not
// In tulip i just fill up the last_midi queue so that MIDI input is accessible to Python
// I also process sysex if given, and dispatch CC mappings via midi_cc_handler
void tulip_midi_input_hook(uint8_t * data, uint16_t len, uint8_t is_sysex) {
    // Process ic (MIDI CC mapping) commands before queuing to Python
    #ifndef __EMSCRIPTEN__
    midi_cc_handler(data, len, is_sysex);
    #endif
    if(is_sysex) {
        // f0 and f7 are stripped on some platforms
        if(data[0]!=0xf0) {
            uint16_t c = 0;
            sysex_buffer[c++] = 0xf0;
            for(uint16_t i = 0; i< len; i++) {
                sysex_buffer[c++] = data[i];
            }
            sysex_buffer[c++] = 0xf7;
            sysex_len = c;
        } else {
            for(uint16_t i = 0; i< len; i++) {
                sysex_buffer[i] = data[i];
            }
            sysex_len = len;
        }
        if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_true);
    } else {
        for(uint32_t i = 0; i < (uint32_t)len; i++) {
            if(i < MAX_MIDI_BYTES_PER_MESSAGE) {
                //fprintf(stderr, "%02x ", data[i]);
                last_midi[midi_queue_tail][i] = data[i];
            }
        }
        last_midi_len[midi_queue_tail] = (uint16_t)len;
        midi_queue_tail = (midi_queue_tail + 1) % MIDI_QUEUE_DEPTH;
        if (midi_queue_tail == midi_queue_head) {
            // Queue wrap, drop oldest item.
            midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
            //fprintf(stderr, "dropped midi message\n");
        }

        // We tell Python that a MIDI message has been received
        if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_false);
    }
}

void midi_local(uint8_t * bytes, uint16_t len) {
#ifndef AMY_IS_EXTERNAL
    convert_midi_bytes_to_messages(bytes, len, 0);
#endif
#ifdef __EMSCRIPTEN__
    for(uint16_t i=0;i<len;i++) {
        EM_ASM(
            if(typeof amy_process_single_midi_byte === 'function') {
                amy_process_single_midi_byte($0, 1);
            }, bytes[i]);
    }
#endif
}

extern bool midi_has_out;
extern void send_usb_midi_out(uint8_t * data, uint16_t len);

void tulip_send_midi_out(uint8_t* buf, uint16_t len) {
    // check if we have USB HOST midi, which is handled by Tulip only - not AMYBOARD or TDECK
#ifdef ESP_PLATFORM
#ifndef TDECK
#ifndef AMYBOARD
    if(midi_has_out) {
        send_usb_midi_out(buf, len);
    }
#endif
#endif
#endif
#ifndef AMY_IS_EXTERNAL
    // Also send out via AMY
    amy_external_midi_output(buf, len);
#endif
}

#ifndef AMY_IS_EXTERNAL

#if (defined AMYBOARD) || (defined TULIP)
#include "tulip_helpers.h"
// map the mp_obj_t to a file handle


static mp_obj_t *g_files[MAX_OPEN_FILES]; // index 1..MAX_OPEN_FILES-1 used

static uint32_t alloc_handle(mp_obj_t f) {
    for (uint32_t i = 1; i < MAX_OPEN_FILES; i++) {
        if (g_files[i] == NULL) {
            g_files[i] = f;
            return i;
        }
    }
    return HANDLE_INVALID; // table full
}

static mp_obj_t lookup_handle(uint32_t h) {
    if (h == 0 || h >= MAX_OPEN_FILES) return NULL;
    return g_files[h];
}

static void free_handle(uint32_t h) {
    if (h == 0 || h >= MAX_OPEN_FILES) return;
    g_files[h] = NULL;
}


uint32_t mp_fopen_hook(char * filename, char * mode) {
    mp_obj_t f = tulip_fopen(filename, mode);
    if (!f) {
        return HANDLE_INVALID;
    }
    uint32_t h = alloc_handle(f);
    if (h == HANDLE_INVALID) {
        tulip_fclose(f);
        return HANDLE_INVALID;
    }
    return h;
}

uint32_t mp_fwrite_hook(uint32_t fptr, uint8_t * bytes, uint32_t len) {

    mp_obj_t f = lookup_handle(fptr);
    if (!f) {
        return 0;
    }
    uint32_t w = tulip_fwrite(f, bytes, len);
    return w;
}
#define MAX_MP_FREAD_SIZE 64
uint32_t mp_fread_hook(uint32_t fptr, uint8_t * bytes, uint32_t len) {
    mp_obj_t f = lookup_handle(fptr);
    if (!f) {
        return 0;
    }
    uint32_t total = 0;
    while (total < len) {
        uint32_t chunk = len - total;
        if (chunk > MAX_MP_FREAD_SIZE) {
            chunk = MAX_MP_FREAD_SIZE;
        }
        uint32_t r = tulip_fread(f, bytes + total, chunk);
        total += r;
        if (r < chunk) {
            break;
        }
    }
    return total;
}
void mp_fseek_hook(uint32_t fptr, uint32_t pos) {
    mp_obj_t f = lookup_handle(fptr);
    if (!f) {
        return;
    }
    (void)tulip_fseek(f, pos);
}

void mp_fclose_hook(uint32_t fptr) {
    mp_obj_t f = lookup_handle(fptr);
    if (f) {
        tulip_fclose(f);
        free_handle(fptr);
    }
}

STATIC mp_obj_t tulip_environment_transfer_done(size_t n_args, const mp_obj_t *args) {
    mp_obj_t mod = mp_import_name(MP_QSTR_amyboard, mp_const_none, MP_OBJ_NEW_SMALL_INT(0));
    mp_obj_t fn = mp_load_attr(mod, MP_QSTR_environment_transfer_done);
    return mp_call_function_0(fn);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_environment_transfer_done_obj, 0, 1, tulip_environment_transfer_done);

void mp_exec_hook(const char *code) {
#if defined(AMYBOARD)
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_obj_t code_str = mp_obj_new_str(code, strlen(code));
        mp_call_function_1(MP_OBJ_FROM_PTR(&mp_builtin_exec_obj), code_str);
        nlr_pop();
    } else {
        fprintf(stderr, "mp_exec_hook: exec raised, ignoring\n");
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
    }
#else
    (void)code;
#endif
}

void mp_update_file_hook(const char *filename) {
#if defined(AMYBOARD)
    // Call amyboard.update_sketch_knobs(filename) synchronously. This runs
    // from the C-side zA handler, so any unhandled Python exception would
    // NLR-longjmp out of the sysex parser mid-message and the subsequent zD
    // would never be processed (Pull would hang). Wrap the call in an NLR
    // buffer so exceptions get swallowed and the parser can continue.
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_obj_t mod = mp_import_name(MP_QSTR_amyboard, mp_const_none, MP_OBJ_NEW_SMALL_INT(0));
        mp_obj_t fn = mp_load_attr(mod, MP_QSTR_update_sketch_knobs);
        mp_obj_t path = mp_obj_new_str(filename, strlen(filename));
        mp_call_function_1(fn, path);
        nlr_pop();
    } else {
        // Python raised — swallow so the sysex parser can continue to zD.
        fprintf(stderr, "mp_update_file_hook: update_sketch_knobs raised, ignoring\n");
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
    }
#else
    (void)filename;
#endif
}

void mp_file_transfer_done_hook(const char *filename) {
#if defined(AMYBOARD)
    if (filename == NULL || filename[0] == '\0') {
        return;
    }
    const char *leaf = filename;
    const char *slash = strrchr(filename, '/');
    if (slash != NULL && slash[1] != '\0') {
        leaf = slash + 1;
    }
    if (strcmp(leaf, "sketch.py") == 0) {
        mp_sched_schedule(MP_OBJ_FROM_PTR(&tulip_environment_transfer_done_obj), mp_const_none);
    }
#else
    (void)filename;
#endif
}


void run_amy(uint8_t midi_out_pin) {
    amy_config_t amy_config = amy_default_config();
    amy_config.amy_external_midi_input_hook = tulip_midi_input_hook;
    amy_config.amy_external_render_hook = external_cv_render;
    amy_config.amy_external_fopen_hook = mp_fopen_hook;
    amy_config.amy_external_fseek_hook = mp_fseek_hook;
    amy_config.amy_external_fclose_hook = mp_fclose_hook;
    amy_config.amy_external_fread_hook = mp_fread_hook;
    amy_config.amy_external_fwrite_hook = mp_fwrite_hook;
    amy_config.amy_external_file_transfer_done_hook = mp_file_transfer_done_hook;
    amy_config.amy_external_update_file_hook = mp_update_file_hook;
    amy_config.amy_external_exec_hook = mp_exec_hook;
    extern void tulip_amy_sequencer_hook(uint32_t tick_count);
    amy_config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
    amy_config.audio = AMY_AUDIO_IS_I2S;
#if defined(AMYBOARD) || defined(AMYBOARD_WEB)
    extern float cv_input_hook(uint16_t channel);
    amy_config.amy_external_coef_hook = cv_input_hook;
#endif
#ifdef AMYBOARD
    amy_config.features.audio_in = 1;
    amy_config.midi = AMY_MIDI_IS_UART | AMY_MIDI_IS_USB_GADGET;
#else
    amy_config.features.audio_in = 0;
    amy_config.midi = AMY_MIDI_IS_UART;
#endif
    amy_config.features.default_synths = 0; // midi.py does this for us
    amy_config.i2s_lrc = CONFIG_I2S_LRCLK;
    amy_config.i2s_bclk = CONFIG_I2S_BCLK;
    amy_config.i2s_dout = CONFIG_I2S_DOUT;
    amy_config.i2s_din = CONFIG_I2S_DIN;
    amy_config.i2s_mclk = CONFIG_I2S_MCLK;
    amy_config.midi_out = midi_out_pin;
    amy_config.midi_in = MIDI_IN_PIN;
#ifndef AMYBOARD
    amy_config.features.startup_bleep = 1;
#endif
    amy_start(amy_config);
    external_map = malloc_caps(amy_config.max_oscs, MALLOC_CAP_INTERNAL);
    for(uint16_t i=0;i<amy_config.max_oscs;i++) external_map[i] = 0;
    for(uint8_t i=0;i<MAX_CV_SYNTHS;i++) cv_synth_map[i] = 0;
}

#elif defined TULIP_DESKTOP

void run_amy(uint8_t capture_device_id, uint8_t playback_device_id) {
    amy_config_t amy_config = amy_default_config();
    amy_config.amy_external_midi_input_hook = tulip_midi_input_hook;
    extern void tulip_amy_sequencer_hook(uint32_t tick_count);
    amy_config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
    amy_config.features.default_synths = 0; // midi.py does this for us
    amy_config.capture_device_id = capture_device_id;
    amy_config.playback_device_id = playback_device_id;
    amy_config.features.audio_in = 1;
    amy_config.audio = AMY_AUDIO_IS_MINIAUDIO;
    //amy_config.i2s_din = 0;  // Dummy to indicate has audio in.
    amy_config.features.startup_bleep = 1;
    amy_start(amy_config);
}

#endif

#endif
