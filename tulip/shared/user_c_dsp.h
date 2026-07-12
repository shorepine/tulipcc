// user_c_dsp.h
// Runtime-installed user C DSP for Tulip/AMYboard. Two kinds:
//   effect — void process(int16_t *buf, int frames, int chans)
//            runs at the end of a bus' FX chain; modify buf in place
//   osc    — void render(int16_t *buf, int frames, int osc, int phase_inc_q16, int amp_q15)
//            replaces the waveform of a bound AMY osc (mono; AMY pans/mixes)
// Samples are plain 16-bit PCM (-32767..32767, like a WAV file), channels
// sequential (non-interleaved); the dispatchers convert to/from AMY's s8.23.
// Sources may be a full program or just the function body (auto-wrapped).
// See docs/user_c_dsp_design.md.
#pragma once

#include <stdint.h>

#define USER_C_DSP_KIND_EFFECT 0
#define USER_C_DSP_KIND_OSC 1

// Registered as amy_config.amy_external_bus_postprocess_hook (SAMPLE == int32_t s8.23).
void tulip_bus_postprocess_hook(uint8_t bus, int32_t *buf, uint16_t len);

// Registered as amy_config.amy_external_render_hook (desktop; ESP chains via CV).
uint8_t tulip_user_render_hook(uint16_t osc, int32_t *buf, uint16_t len);

// Compile src (full program or bare body) and install it under name as kind
// (replacing an existing same-kind install of the same name, keeping its bus
// enables / osc bindings). Returns slot index >= 0, or -1 with a message in err.
int user_c_dsp_install(const char *name, const char *src, uint8_t kind, char *err, int errlen);

// Enable (on=1) / disable (on=0) a named effect on a bus. Returns 0, or
// -1 (no such name) / -2 (bad bus) / -3 (not an effect).
int user_c_dsp_set(const char *name, int bus, int on);

// Bind (on=1) / unbind (on=0) a named oscillator to AMY osc number osc.
// Returns 0, or -1 (no such name) / -2 (bad osc) / -3 (not an osc).
int user_c_dsp_bind_osc(const char *name, int osc, int on);

// Number of blocks processed since install. Returns -1 if no such name.
int64_t user_c_dsp_calls(const char *name);

// Remove a named process and free its compiled code. Returns 0 or -1.
int user_c_dsp_uninstall(const char *name);
