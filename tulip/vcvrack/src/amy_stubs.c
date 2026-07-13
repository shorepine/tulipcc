// Platform stubs for embedding AMY in a VCV Rack plugin.
// Rack owns the audio device and MIDI I/O, so AMY's hardware layers are
// stubbed out (same approach as amy/godot/src/amy_platform_stubs.c, except
// amy_midi.c IS compiled here for its MIDI-message-to-event routing).

#include <stdint.h>
#include <stddef.h>

// --- i2s.c stubs (referenced by api.c when audio != I2S they are unused) ---
void amy_platform_init(void) {}
void amy_platform_deinit(void) {}
void amy_update_tasks(void) {}
int16_t* amy_render_audio(void) { return (int16_t*)0; }
size_t amy_i2s_write(const uint8_t* buffer, size_t nbytes) {
    (void)buffer;
    return nbytes;
}

// --- amy_midi.c device layer: no darwin/generic implementation without
// -DMACOS (CoreMIDI). Rack delivers MIDI to us via midi::InputQueue, so AMY
// never opens a device itself. ---
void run_midi(void) {}
void stop_midi(void) {}

// --- examples.c ---
void delay_ms(uint32_t ms) { (void)ms; }
