// amy_connector.c
// all the stuff that connects Tulip to AMY
// like MIDI queue -> python
// like external CV
// like t-sequencer?
// like alles / wifi stuff?


#include "polyfills.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "amy_connector.h"
#include <stdio.h>
#include <string.h>
#ifdef ESP_PLATFORM
#include "esp_system.h"
#include "esp_attr.h"
#ifdef GAMMA9001
#include "esp_partition.h"
#endif
#endif
#ifdef AMYBOARD
// For amyboard_set_midi_out(): re-point the MIDI UART's TX line at runtime.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#endif
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
extern mp_obj_t amy_overload_callback;

int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;

// AMY calls this from its render task when the CPU overload failsafe trips
// (it has already reset the synth and played its bleep). Hand off to Python
// with the load as a percent -- a small int, so no cross-task heap allocation.
void tulip_amy_overload_hook(float load) {
    if (amy_overload_callback != NULL)
        mp_sched_schedule(amy_overload_callback, MP_OBJ_NEW_SMALL_INT((mp_int_t)(load * 100.0f)));
}


#ifdef ESP_PLATFORM
#include "driver/i2c.h"

// Maps synth number -> CV channel (0 = not mapped, 1 = CV1, 2 = CV2)
// Set from Python via amyboard.set_cv_out(channel, synth)
#define MAX_CV_SYNTHS 32
uint8_t cv_synth_map[MAX_CV_SYNTHS];

// Look up which synth owns this osc, return synth number or -1
static int synth_for_osc(uint16_t osc) {
    // osc_to_voice is declared in amy.h (uint16_t * as of amy#977) -- don't
    // redeclare it here, and use AMY_IS_SET so the unset sentinel follows
    // AMY's type instead of a hardcoded 255.
    if (osc_to_voice == NULL) return -1;
    uint16_t voice = osc_to_voice[osc];
    if (!AMY_IS_SET(voice)) return -1;
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

#ifdef TULIP_USER_C_DSP
// AMY's render hook has one slot; on ESP it serves both CV output and user
// C oscillators. CV wins (returns 1 = handled, osc doesn't reach the mix);
// otherwise fall through to a bound user osc, if any.
static uint8_t tulip_render_hook_chain(uint16_t osc, SAMPLE *buf, uint16_t len) {
    if (external_cv_render(osc, buf, len)) return 1;
    extern uint8_t tulip_user_render_hook(uint16_t osc, SAMPLE *buf, uint16_t len);
    return tulip_user_render_hook(osc, buf, len);
}
#endif
#endif

#ifdef AMYBOARD_VCV
// AMYboard-in-VCV-Rack: same CV-output routing as the hardware (per-osc
// external_map + synth-based cv_synth_map), but instead of an I2C DAC the
// volts land in amyboard_vcv_cv_out[] for the Rack module to put on jacks.
#define MAX_CV_SYNTHS 32
uint8_t cv_synth_map[MAX_CV_SYNTHS];
float amyboard_vcv_cv_out[2] = {0, 0};

// Look up which synth owns this osc, return synth number or -1
static int synth_for_osc(uint16_t osc) {
    // osc_to_voice is declared in amy.h (uint16_t * as of amy#977) -- don't
    // redeclare it here, and use AMY_IS_SET so the unset sentinel follows
    // AMY's type instead of a hardcoded 255.
    if (osc_to_voice == NULL) return -1;
    uint16_t voice = osc_to_voice[osc];
    if (!AMY_IS_SET(voice)) return -1;
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

uint8_t external_cv_render(uint16_t osc, SAMPLE * buf, uint16_t len) {
    uint8_t cv_channel = 255;
    if (external_map[osc] > 0) {
        cv_channel = external_map[osc] - 1;
    } else {
        int s = synth_for_osc(osc);
        if (s >= 0 && s < MAX_CV_SYNTHS && cv_synth_map[s] > 0)
            cv_channel = cv_synth_map[s] - 1;
    }
    if (cv_channel > 1) return 0;
    // Sample range [-1,1] -> volts [-10,+10], same scaling as the GP8413 path
    amyboard_vcv_cv_out[cv_channel] = S2F(buf[0]) * 10.0f;
    return 1;
}
#endif

// defined in amy/src/midi_mappings.c — processes ic (MIDI CC mapping) commands
// On web, AMY runs in a separate wasm worker so midi_msg_handler is not linkable
#ifndef __EMSCRIPTEN__
extern void midi_msg_handler(uint8_t * bytes, uint16_t len, uint8_t is_sysex, uint32_t time);
#endif

// I am called when AMY receives MIDI in, whether it has been processed (played in a instrument) or not
// In tulip i just fill up the last_midi queue so that MIDI input is accessible to Python
// I also process sysex if given, and dispatch CC mappings via midi_msg_handler
void tulip_midi_input_hook(uint8_t * data, uint16_t len, uint8_t is_sysex) {
    // Process ic (MIDI CC mapping) commands before queuing to Python
    #ifndef __EMSCRIPTEN__
    uint32_t time;
    AMY_UNSET(time);
    midi_msg_handler(data, len, is_sysex, time);
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

#if (defined AMYBOARD) || (defined TULIP) || (defined AMYBOARD_VCV)
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


#ifdef AMYBOARD_VCV
// Control-API wire paths are board-absolute (/user/current/sketch.py); the
// VCV board root is ~/Documents/AMYboard (see tulip.py root_dir()).
static const char *vcv_relocate_wire_path(const char *filename, char *buf, size_t buflen) {
    if (filename && filename[0] == '/') {
        const char *home = getenv("HOME");
        if (!home) home = getenv("USERPROFILE");  // Windows
        if (home) {
            snprintf(buf, buflen, "%s/Documents/AMYboard%s", home, filename);
            return buf;
        }
    }
    return filename;
}
#endif

uint32_t mp_fopen_hook(char * filename, const char * mode) {
#ifdef AMYBOARD_VCV
    char vcv_path[512];
    filename = (char *)vcv_relocate_wire_path(filename, vcv_path, sizeof(vcv_path));
#endif
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
#if defined(AMYBOARD) || defined(AMYBOARD_VCV)
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

void mp_file_transfer_done_hook(const char *filename) {
#if defined(AMYBOARD) || defined(AMYBOARD_VCV)
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


#ifdef ESP_PLATFORM
RTC_NOINIT_ATTR uint32_t amyboard_bootloader_flag;
#define AMYBOARD_BOOTLOADER_MAGIC 0xABCD0001
#endif

void mp_reboot_hook(uint8_t mode) {
#if defined(AMYBOARD) && defined(ESP_PLATFORM)
    if (mode == 0) {
        // Bootloader mode: skip sketch on next boot.
        amyboard_bootloader_flag = AMYBOARD_BOOTLOADER_MAGIC;
        esp_restart();
    } else if (mode == 1) {
        // Normal reboot: run sketch as usual.
        esp_restart();
    }
#endif
}

#if defined(GAMMA9001) && defined(ESP_PLATFORM)
// Map the `drums` flash partition (raw drums.bin from the amy repo, flashed by
// fs_create.py) into the data address space and hand it to AMY, which serves
// the Gamma9001 bank presets (256+) straight out of it. If the partition is
// missing or unreadable, those presets stay silent -- the baked TR-808 kit
// (patch 384) still works.
static void mount_gamma9001_drums(void) {
    const esp_partition_t *part = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "drums");
    if (part == NULL) {
        fprintf(stderr, "gamma9001: no drums partition, bank presets unavailable\n");
        return;
    }
    const void *map = NULL;
    esp_partition_mmap_handle_t handle;  // never unmapped; the samples live as long as AMY
    esp_err_t err = esp_partition_mmap(part, 0, part->size, ESP_PARTITION_MMAP_DATA, &map, &handle);
    if (err != ESP_OK || map == NULL) {
        fprintf(stderr, "gamma9001: drums partition mmap failed (%d)\n", (int)err);
        return;
    }
    amy_set_gamma9001_pcm((const int16_t *)map);
}
#endif

#endif  // hooks: AMYBOARD || TULIP || AMYBOARD_VCV

#if (defined AMYBOARD) || (defined TULIP)

void run_amy(uint8_t midi_out_pin) {
    amy_config_t amy_config = amy_default_config();
    amy_config.amy_external_midi_input_hook = tulip_midi_input_hook;
#ifdef TULIP_USER_C_DSP
    amy_config.amy_external_render_hook = tulip_render_hook_chain;
#else
    amy_config.amy_external_render_hook = external_cv_render;
#endif
    amy_config.amy_external_fopen_hook = mp_fopen_hook;
    amy_config.amy_external_fseek_hook = mp_fseek_hook;
    amy_config.amy_external_fclose_hook = mp_fclose_hook;
    amy_config.amy_external_fread_hook = mp_fread_hook;
    amy_config.amy_external_fwrite_hook = mp_fwrite_hook;
    amy_config.amy_external_file_transfer_done_hook = mp_file_transfer_done_hook;
    amy_config.amy_external_exec_hook = mp_exec_hook;
    amy_config.amy_external_reboot_hook = mp_reboot_hook;
    amy_config.amy_external_overload_hook = tulip_amy_overload_hook;
    extern void tulip_amy_sequencer_hook(uint32_t tick_count);
    amy_config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
#ifdef TULIP_USER_C_DSP
    extern void tulip_bus_postprocess_hook(uint8_t bus, SAMPLE *buf, uint16_t len);
    amy_config.amy_external_bus_postprocess_hook = tulip_bus_postprocess_hook;
#endif
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
#if defined(GAMMA9001) && defined(ESP_PLATFORM)
    mount_gamma9001_drums();
#endif
    amy_start(amy_config);
    external_map = malloc_caps(amy_config.max_oscs, MALLOC_CAP_INTERNAL);
    for(uint16_t i=0;i<amy_config.max_oscs;i++) external_map[i] = 0;
    for(uint8_t i=0;i<MAX_CV_SYNTHS;i++) cv_synth_map[i] = 0;
}

#ifdef AMYBOARD
// Set the MIDI OUT TRS standard (Type A = pin 14, Type B = pin 15). This is the single
// MIDI OUT pin-init sequence: amyboard.set_midi_type() calls it both at boot (from
// start_amy(), right after amy_start()) and at runtime, without restarting AMY. AMY
// transmits MIDI via uart_write_bytes(UART_NUM_1, ...) — keyed on the UART number, not
// the GPIO — so moving the UART's TX line to the other TRS leg is all that's needed.
// midi_uart is 1 on AMYboard (amy's esp_get_uart(1) == UART_NUM_1). Only MIDI OUT
// differs by type; MIDI IN works for both, so RX (MIDI_IN_PIN) is left unchanged.
// Requires AMY's UART driver to be installed (amy_start() does this synchronously).
void amyboard_set_midi_out(uint8_t midi_out_pin) {
    const uint8_t other_pin = (midi_out_pin == MIDI_OUT_PIN_A) ? MIDI_OUT_PIN_B : MIDI_OUT_PIN_A;
    // Let any in-flight MIDI byte finish before moving the TX line.
    uart_wait_tx_done(UART_NUM_1, pdMS_TO_TICKS(20));
    // Re-route the UART's TX to the requested TRS data leg.
    uart_set_pin(UART_NUM_1, midi_out_pin, MIDI_IN_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Disconnect the now-unused leg from the UART and hold it high (MIDI idle/source).
    // Driving it as a plain GPIO output stops it from mirroring the TX signal.
    gpio_reset_pin(other_pin);
    gpio_set_direction(other_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(other_pin, 1);
}
#endif

#elif defined TULIP_DESKTOP

void run_amy(uint8_t capture_device_id, uint8_t playback_device_id) {
    amy_config_t amy_config = amy_default_config();
    amy_config.amy_external_midi_input_hook = tulip_midi_input_hook;
    amy_config.amy_external_overload_hook = tulip_amy_overload_hook;
    extern void tulip_amy_sequencer_hook(uint32_t tick_count);
    amy_config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
#ifdef TULIP_USER_C_DSP
    extern void tulip_bus_postprocess_hook(uint8_t bus, SAMPLE *buf, uint16_t len);
    amy_config.amy_external_bus_postprocess_hook = tulip_bus_postprocess_hook;
    extern uint8_t tulip_user_render_hook(uint16_t osc, SAMPLE *buf, uint16_t len);
    amy_config.amy_external_render_hook = tulip_user_render_hook;
#endif
    amy_config.features.default_synths = 0; // midi.py does this for us
    amy_config.capture_device_id = capture_device_id;
    amy_config.playback_device_id = playback_device_id;
    amy_config.features.audio_in = 1;
    amy_config.audio = AMY_AUDIO_IS_MINIAUDIO;
    //amy_config.i2s_din = 0;  // Dummy to indicate has audio in.
    amy_config.features.startup_bleep = 1;
#ifdef GAMMA9001
    // Tulip Desktop links drums.bin straight into the binary (see tulip.mk).
    {
        extern const int16_t gamma9001_pcm_data[];
        amy_set_gamma9001_pcm(gamma9001_pcm_data);
    }
#endif
    amy_start(amy_config);
}

#elif defined AMYBOARD_VCV

// AMYboard inside a VCV Rack plugin: in-process AMY, but the host (Rack)
// owns the audio device and MIDI. Rack pulls blocks via
// amy_simple_fill_buffer() and injects MIDI via
// amy_event_midi_message_received(); AMY opens nothing itself.
void run_amy(void) {
    amy_config_t amy_config = amy_default_config();
    amy_config.amy_external_midi_input_hook = tulip_midi_input_hook;
    amy_config.amy_external_render_hook = external_cv_render;
    amy_config.amy_external_overload_hook = tulip_amy_overload_hook;
    extern void tulip_amy_sequencer_hook(uint32_t tick_count);
    amy_config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
    extern float cv_input_hook(uint16_t channel);
    amy_config.amy_external_coef_hook = cv_input_hook;
    amy_config.features.default_synths = 0; // amyboard.start_amy does this
    amy_config.features.audio_in = 0;
    amy_config.features.startup_bleep = 1;
    amy_config.audio = AMY_AUDIO_IS_NONE;
    // Not NONE: amy_dump_*_to_sysex() early-return when config.midi == 0,
    // and zD dumps must flow. The UART bit is harmless here — all UART
    // device code is ESP-only, run_midi() is a stub, and midi_out() is the
    // plugin's virtual CoreMIDI port (vcv_midi.c).
    amy_config.midi = AMY_MIDI_IS_UART;
    amy_config.platform.multicore = 0;
    amy_config.platform.multithread = 0;
    // Control-API (sysex) file transfer + exec + knobs hooks, as on hardware.
    amy_config.amy_external_fopen_hook = mp_fopen_hook;
    amy_config.amy_external_fseek_hook = mp_fseek_hook;
    amy_config.amy_external_fclose_hook = mp_fclose_hook;
    amy_config.amy_external_fread_hook = mp_fread_hook;
    amy_config.amy_external_fwrite_hook = mp_fwrite_hook;
    amy_config.amy_external_file_transfer_done_hook = mp_file_transfer_done_hook;
    amy_config.amy_external_exec_hook = mp_exec_hook;
    amy_config.amy_external_reboot_hook = mp_reboot_hook;
#ifdef GAMMA9001
    // drums_bin.c is linked straight into the plugin (see Makefile.mp).
    {
        extern const int16_t gamma9001_pcm_data[];
        amy_set_gamma9001_pcm(gamma9001_pcm_data);
    }
#endif
    amy_start(amy_config);
    // amy_start -> transfer_init() force-installs posix file hooks on any
    // _POSIX_VERSION host, clobbering the ones set above. Re-install ours so
    // wire paths ("/user/current/sketch.py") resolve through MicroPython's
    // VFS with the VCV root relocation in mp_fopen_hook.
    amy_global.config.amy_external_fopen_hook = mp_fopen_hook;
    amy_global.config.amy_external_fread_hook = mp_fread_hook;
    amy_global.config.amy_external_fwrite_hook = mp_fwrite_hook;
    amy_global.config.amy_external_fclose_hook = mp_fclose_hook;
    amy_global.config.amy_external_fseek_hook = mp_fseek_hook;
    external_map = malloc(amy_config.max_oscs);
    for (uint16_t i = 0; i < amy_config.max_oscs; i++) external_map[i] = 0;
    for (uint8_t i = 0; i < MAX_CV_SYNTHS; i++) cv_synth_map[i] = 0;
    // AMY's sysex assembly buffer -- normally allocated by the platform MIDI
    // layer, which doesn't run here (Rack owns MIDI).
    sysex_buffer = malloc(MAX_SYSEX_BYTES);
}

#endif

#endif


/* ---------------------------------------------------------------- message
 * tulip.amy_message(**kwargs) -> the wire string, or None.
 *
 * amy.message() in Python costs hundreds of microseconds per message on a
 * device — it slices the keyword map twice per argument (`map_code[:-1]` /
 * `[-1]`), calls a handler per argument, and accumulates with `+=`, and
 * every one of those allocates on the MicroPython heap. In C the same
 * message is a table walk into a stack buffer, ~6x faster measured on
 * ESP32 hardware. _boot.py installs this over amy.message for every
 * target, with the Python original kept as the fallback.
 *
 * It returns None rather than raising whenever it meets ANYTHING it does
 * not reproduce exactly — an unknown keyword, a numeric string where an
 * int was meant, an overlong result. The caller falls back to
 * amy.message(), which is the reference implementation and stays the one
 * that defines the format. That is the whole safety argument: this is an
 * accelerator for the shapes it is sure about, never a second definition
 * of the wire protocol. Bailing is always correct; being wrong is not.
 *
 * The keyword table is GENERATED from amy's own _KW_MAP_LIST (see
 * tulip/shared/gen_amy_kwmap.py) so it cannot drift from the Python, JS
 * and GDScript maps that are generated from the same list. This is pure
 * string building — it never touches AMY itself — so it compiles the same
 * on every target, including the web builds where AMY is external.
 */
#include "amy_kwmap.h"

#define AMY_MSG_CAP 512
#define AMY_MSG_MAX_KW 16

/* The table's keywords as qstrs, interned once on first use.
 * qstr_find_strn() FINDS without creating: a keyword that no frozen
 * module or caller ever mentioned has no qstr, stays 0, and so never
 * matches an incoming kwarg -- which routes that message to Python,
 * exactly as an unknown keyword should be. (In practice every keyword in
 * the table appears as a string literal in frozen amy/__init__.py, so
 * they all have compile-time qstrs.) */
static qstr amy_kw_q[AMY_KW_N];
static bool amy_kw_q_ready = false;

static qstr amy_kw_qstr(int k) {
    if (!amy_kw_q_ready) {
        for (int i = 0; i < AMY_KW_N; i++)
            amy_kw_q[i] = qstr_find_strn(AMY_KW[i].name, strlen(AMY_KW[i].name));
        amy_kw_q_ready = true;
    }
    return amy_kw_q[k];
}

/* Every formatter below calls MICROPYTHON'S OWN, not the C library's.
 * That is the whole correctness argument. snprintf("%.6f") disagrees with
 * Python as soon as a value needs more significant digits than a 24-bit
 * mantissa carries:
 *
 *     999.999      snprintf 999.999023   MicroPython 999.999
 *     123.456789   snprintf 123.456795   MicroPython 123.456792
 *
 * mp_format_float() is literally what `'%.6f' % x` runs (mpprint.c), and
 * mp_obj_print_helper(PRINT_STR) is literally what `str(x)` runs, so
 * matching is structural rather than a thing to be sampled for. */
#include "py/formatfloat.h"

/* append, refusing to overflow */
static bool amy_put(char *buf, size_t *len, const char *s, size_t n) {
    if (*len + n >= AMY_MSG_CAP)
        return false;
    memcpy(buf + *len, s, n);
    *len += n;
    buf[*len] = 0;
    return true;
}

/* str(o), MicroPython's own, appended. Used where amy's handlers call
 * str() rather than trunc() -- list elements and coef elements, whose
 * floats take the repr path and NOT %.6f. */
static bool amy_put_pystr(char *buf, size_t *len, mp_obj_t o) {
    vstr_t vs;
    vstr_init(&vs, 24);
    mp_print_t pr = {.data = &vs, .print_strn = (mp_print_strn_t)vstr_add_strn};
    mp_obj_print_helper(&pr, o, PRINT_STR);
    bool ok = amy_put(buf, len, vs.buf, vs.len);
    vstr_clear(&vs);
    return ok;
}

/* A plain decimal, for the int fast path. str(int) IS this, so it is an
 * exact shortcut around the vstr above rather than a second opinion.
 * Hand-rolled, NOT snprintf: the ESP-IDF builds use newlib-nano
 * (CONFIG_NEWLIB_NANO_FORMAT), whose printf silently mangles %lld -- on
 * hardware that turned every int argument into garbage while desktop
 * passed -- and on the VCV/linux builds snprintf is mp_vprintf-backed
 * with its own format gaps. Digits owe nothing to any libc. */
static bool amy_put_int(char *buf, size_t *len, mp_int_t i) {
    char tmp[24];   /* 64-bit mp_int_t: 20 digits + sign fits */
    char *p = tmp + sizeof tmp;
    bool neg = i < 0;
    /* unsigned copy so the most-negative value can't overflow on negate */
    unsigned long long u = neg ? -(unsigned long long)i : (unsigned long long)i;
    do { *--p = (char)('0' + (u % 10)); u /= 10; } while (u);
    if (neg) *--p = '-';
    return amy_put(buf, len, p, (size_t)(tmp + sizeof tmp - p));
}

/* amy's trunc():
 *     str -> '%'-token passthrough, blank -> '', else float(it)
 *     float -> ('%.6f' % x).rstrip('0').rstrip('.')
 *     anything else -> str(x)   (so a bool really does come out "True")
 * The '.' that %.6f always emits is what makes the rstrip safe: it stops
 * the zero-strip before it can eat an integer part like 100.0 -> "1". */
static bool amy_put_trunc(char *buf, size_t *len, mp_obj_t v) {
    if (mp_obj_is_str(v)) {
        size_t sl;
        const char *sp = mp_obj_str_get_data(v, &sl);
        if (sl && sp[0] == '%')
            return amy_put(buf, len, sp, sl);
        size_t a = 0, b = sl;
        while (a < b && (sp[a] == ' ' || sp[a] == '\t')) a++;
        while (b > a && (sp[b-1] == ' ' || sp[b-1] == '\t')) b--;
        if (a == b)
            return true;                       /* blank -> '' */
        return false;                          /* float(str): let Python */
    }
    if (mp_obj_is_float(v)) {
        char tmp[48];
        int n = mp_format_float((mp_float_t)mp_obj_get_float(v), tmp,
                                sizeof tmp, 'f', 6, '\0');
        if (n < 0 || (size_t)n >= sizeof tmp)
            return false;
        if (strchr(tmp, '.') != NULL) {        /* inf/nan have none */
            while (n > 0 && tmp[n-1] == '0') n--;
            while (n > 0 && tmp[n-1] == '.') n--;
            tmp[n] = 0;
        }
        return amy_put(buf, len, tmp, (size_t)n);
    }
    return amy_put_pystr(buf, len, v);         /* ints, bools, everything */
}

/* amy's str_of_int(): a '%'-token passes through, else str(int(arg)).
 * A numeric string would need int(str) semantics, so those bail. */
static bool amy_put_int_of(char *buf, size_t *len, mp_obj_t v) {
    if (mp_obj_is_str(v)) {
        size_t sl;
        const char *sp = mp_obj_str_get_data(v, &sl);
        if (sl && sp[0] == '%')
            return amy_put(buf, len, sp, sl);
        return false;                          /* int("12"): let Python */
    }
    if (mp_obj_is_float(v)) {
        mp_float_t f = mp_obj_get_float(v);
        if (!(f >= (mp_float_t)-1e15 && f <= (mp_float_t)1e15))
            return false;   /* int(huge/inf/nan): cast is UB, let Python */
        return amy_put_int(buf, len, (mp_int_t)f);
    }
    if (mp_obj_is_small_int(v))
        return amy_put_int(buf, len, MP_OBJ_SMALL_INT_VALUE(v));
    if (v == mp_const_true || v == mp_const_false)
        return amy_put_int(buf, len, mp_obj_get_int(v));   /* int(bool) */
    if (mp_obj_is_int(v))
        return amy_put_pystr(buf, len, v);     /* big int: str() is exact */
    return false;
}

/* Lists AND tuples: amy's parse_list_or_comma_string joins both.
 * (parse_ctrl_coefs is stricter -- it asserts on tuples -- so the coefs
 * path below does its own list-only check.) */
static bool amy_seq_items(mp_obj_t v, size_t *n, mp_obj_t **items) {
    if (mp_obj_is_type(v, &mp_type_list)) { mp_obj_list_get(v, n, items); return true; }
    if (mp_obj_is_type(v, &mp_type_tuple)) { mp_obj_tuple_get(v, n, items); return true; }
    return false;
}

/* amy's parse_list_or_comma_string(): ','.join with None as ''. */
static bool amy_put_list(char *buf, size_t *len, mp_obj_t v) {
    size_t n_items;
    mp_obj_t *items;
    if (!amy_seq_items(v, &n_items, &items))
        return amy_put_pystr(buf, len, v);
    for (size_t k = 0; k < n_items; k++) {
        if (k && !amy_put(buf, len, ",", 1))
            return false;
        mp_obj_t e = items[k];
        if (e == mp_const_none)
            continue;
        if (mp_obj_is_small_int(e)) {
            if (!amy_put_int(buf, len, MP_OBJ_SMALL_INT_VALUE(e)))
                return false;
        } else if (!amy_put_pystr(buf, len, e)) {   /* floats: str(), not %.6f */
            return false;
        }
    }
    return true;
}

/* amy's parse_ctrl_coefs(). Four accepted shapes, and note that the
 * SCALAR one truncs while the list/dict ones str() -- that asymmetry is
 * amy's, reproduced rather than tidied. */
static const char *const AMY_COEF_FIELDS[] = {
    "const", "note", "vel", "eg0", "eg1", "mod", "bend", "ext0", "ext1",
};
#define AMY_COEF_N 9

static bool amy_put_coefs(char *buf, size_t *len, mp_obj_t v) {
    if (mp_obj_is_str(v)) {
        /* ','.join(trunc(x) for x in s.split(',')) */
        size_t sl;
        const char *sp = mp_obj_str_get_data(v, &sl);
        size_t a = 0;
        bool first = true;
        while (a <= sl) {
            size_t b = a;
            while (b < sl && sp[b] != ',') b++;
            if (!first && !amy_put(buf, len, ",", 1))
                return false;
            first = false;
            mp_obj_t piece = mp_obj_new_str(sp + a, b - a);
            if (!amy_put_trunc(buf, len, piece))
                return false;
            if (b >= sl) break;
            a = b + 1;
        }
        return true;
    }
    if (mp_obj_is_int(v) || mp_obj_is_float(v))
        return amy_put_trunc(buf, len, v);

    mp_obj_t vals[AMY_COEF_N];
    size_t n_items = 0;
    mp_obj_t *items;
    if (mp_obj_is_type(v, &mp_type_dict)) {
        for (int i = 0; i < AMY_COEF_N; i++)
            vals[i] = mp_const_none;
        mp_map_t *m = mp_obj_dict_get_map(v);
        for (size_t i = 0; i < m->alloc; i++) {
            if (m->table[i].key == MP_OBJ_NULL)
                continue;
            if (!mp_obj_is_str(m->table[i].key))
                return false;
            size_t kl;
            const char *kp = mp_obj_str_get_data(m->table[i].key, &kl);
            int slot = -1;
            for (int f = 0; f < AMY_COEF_N; f++)
                if (strlen(AMY_COEF_FIELDS[f]) == kl &&
                    memcmp(AMY_COEF_FIELDS[f], kp, kl) == 0) { slot = f; break; }
            if (slot < 0)
                return false;              /* unrecognised: Python raises */
            vals[slot] = m->table[i].value;
        }
        items = vals;
        n_items = AMY_COEF_N;
    } else if (mp_obj_is_type(v, &mp_type_list)) {
        mp_obj_list_get(v, &n_items, &items);
    } else {
        return false;   /* tuples: parse_ctrl_coefs asserts, so let Python */
    }
    /* trim_trailing(coefs, x is not None) */
    while (n_items > 0 && items[n_items - 1] == mp_const_none)
        n_items--;
    for (size_t k = 0; k < n_items; k++) {
        if (k && !amy_put(buf, len, ",", 1))
            return false;
        if (items[k] == mp_const_none)
            continue;                      /* to_str(None) -> '' */
        if (!amy_put_pystr(buf, len, items[k]))
            return false;
    }
    return true;
}

static bool amy_fmt_arg(char *buf, size_t *len, char type, mp_obj_t v) {
    switch (type) {
    case 'I': return amy_put_int_of(buf, len, v);
    case 'F': return amy_put_trunc(buf, len, v);
    case 'L': return amy_put_list(buf, len, v);
    case 'S': return amy_put_pystr(buf, len, v);
    case 'C': return amy_put_coefs(buf, len, v);
    default:  return false;
    }
}

mp_obj_t tulip_amy_message(size_t n_args, const mp_obj_t *pos, mp_map_t *kwargs) {
    (void)pos;
    if (n_args != 0 || kwargs == NULL)
        return mp_const_none;

    /* Collect (table index, value), which IS priority order: the
     * generated table is _KW_MAP_LIST's order and patch_string is last. */
    uint8_t idx[AMY_MSG_MAX_KW];
    mp_obj_t val[AMY_MSG_MAX_KW];
    int n = 0;
    for (size_t i = 0; i < kwargs->alloc; i++) {
        if (kwargs->table[i].key == MP_OBJ_NULL)
            continue;
        if (n >= AMY_MSG_MAX_KW)
            return mp_const_none;
        if (!mp_obj_is_qstr(kwargs->table[i].key))
            return mp_const_none;
        qstr q = MP_OBJ_QSTR_VALUE(kwargs->table[i].key);
        mp_obj_t v = kwargs->table[i].value;
        int found = -1;
        for (int k = 0; k < AMY_KW_N; k++)
            if (amy_kw_qstr(k) == q && amy_kw_q[k] != 0) { found = k; break; }
        if (found < 0)
            return mp_const_none;                /* unknown: let Python raise */
        if (v == mp_const_none) {
            /* time=None and sequence=None are ignored; any other None is
             * an error, and Python is where that error should come from. */
            if (strcmp(AMY_KW[found].name, "time") == 0 ||
                strcmp(AMY_KW[found].name, "sequence") == 0)
                continue;
            return mp_const_none;
        }
        idx[n] = (uint8_t)found;
        val[n] = v;
        n++;
    }
    /* insertion sort by priority; n is tiny */
    for (int i = 1; i < n; i++) {
        uint8_t ki = idx[i];
        mp_obj_t vi = val[i];
        int j = i - 1;
        while (j >= 0 && idx[j] > ki) {
            idx[j + 1] = idx[j]; val[j + 1] = val[j]; j--;
        }
        idx[j + 1] = ki; val[j + 1] = vi;
    }

    char buf[AMY_MSG_CAP];
    size_t len = 0;
    buf[0] = 0;
    for (int i = 0; i < n; i++) {
        const amy_kw_t *e = &AMY_KW[idx[i]];
        if (!amy_put(buf, &len, e->wire, strlen(e->wire)))
            return mp_const_none;
        if (!amy_fmt_arg(buf, &len, e->type, val[i]))
            return mp_const_none;
    }
    if (!amy_put(buf, &len, "Z", 1))
        return mp_const_none;
    return mp_obj_new_str(buf, len);
}
MP_DEFINE_CONST_FUN_OBJ_KW(tulip_amy_message_obj, 0, tulip_amy_message);
