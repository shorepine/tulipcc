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

// An AMY hook to send values out to a CV DAC over i2c, only on ESP 
uint8_t external_cv_render(uint16_t osc, SAMPLE * buf, uint16_t len) {
    if(external_map[osc]>0) {
        float volts = S2F(buf[0])*2.5f + 2.5f;
        // do the thing
        uint16_t value_int = (uint16_t)((volts/10.0) * 65535.0);
        uint8_t bytes[3];
        bytes[2] = (value_int & 0xff00) >> 8;
        bytes[1] = (value_int & 0x00ff);

        uint8_t ch = 0x02;
        uint8_t addr = 89;
        uint8_t channel = external_map[osc]-1;
        if(channel == 1) ch = 0x04;
        if(channel == 2) addr = 88;
        if(channel == 3) {ch = 0x04; addr=88; }
        bytes[0] = ch;
        i2c_master_write_to_device(I2C_NUM_0, addr, bytes, 3, pdMS_TO_TICKS(10));
        return 1;
    }
    return 0;

}
#endif

// I am called when AMY receives MIDI in, whether it has been processed (played in a instrument) or not 
// In tulip i just fill up the last_midi queue so that MIDI input is accessible to Python
// I also process sysex if given
void tulip_midi_input_hook(uint8_t * data, uint16_t len, uint8_t is_sysex) {
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
    // check if we have USB HOST midi, which is handled by Tulip
#ifdef ESP_PLATFORM
#ifndef TDECK
    if(midi_has_out) {
        send_usb_midi_out(buf, len);
    }
#endif
#endif
#ifndef AMY_IS_EXTERNAL
    // Also send out via AMY
    amy_external_midi_output(buf, len);
#endif
}

#ifndef AMY_IS_EXTERNAL

#ifdef ESP_PLATFORM
void run_amy() {
    amy_external_midi_input_hook = tulip_midi_input_hook;
    amy_external_render_hook = external_cv_render;

    amy_config_t amy_config = amy_default_config();
    amy_config.has_audio_in = 0;
    amy_config.has_midi_uart = 1;
    amy_config.set_default_synth = 0; // midi.py does this for us
    amy_config.cores = 2;
    amy_config.i2s_lrc = CONFIG_I2S_LRCLK;
    amy_config.i2s_bclk = CONFIG_I2S_BCLK;
    amy_config.i2s_dout = CONFIG_I2S_DOUT;
    if(CONFIG_I2S_DIN>=0) amy_config.i2s_din = CONFIG_I2S_DIN;
    amy_config.midi_out = MIDI_OUT_PIN;
    amy_config.midi_in = MIDI_IN_PIN;
    amy_start(amy_config);
    external_map = malloc_caps(amy_config.max_oscs, MALLOC_CAP_INTERNAL);
    for(uint16_t i=0;i<amy_config.max_oscs;i++) external_map[i] = 0;
    amy_live_start();
}

#elif defined TULIP_DESKTOP

void run_amy(uint8_t capture_device_id, uint8_t playback_device_id) {
    amy_external_midi_input_hook = tulip_midi_input_hook;
    amy_config_t amy_config = amy_default_config();
    amy_config.has_midi_mac = 1;
    amy_config.set_default_synth = 0; // midi.py does this for us
    amy_config.capture_device_id = capture_device_id;
    amy_config.playback_device_id = playback_device_id;
    amy_config.has_audio_in = 1;
    amy_start(amy_config);
    amy_live_start();
}

#endif

#endif
