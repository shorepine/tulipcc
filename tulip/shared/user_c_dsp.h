// user_c_dsp.h
// Runtime-installed user C DSP for Tulip/AMYboard. Works directly on AMY's
// S8.23 fixed-point buffers (int32, 1.0 == 1<<23), zero-copy. Two kinds:
//   effect — void process(int *buf, int frames, int chans)
//            runs at the end of a bus' FX chain; modify buf in place
//            (chans sequential non-interleaved channel blocks)
//   osc    — void render(int *buf, int frames, int osc, int phase_inc_q16, int amp)
//            replaces the waveform of a bound AMY osc (mono; AMY pans/mixes).
//            phase_inc_q16: per-sample phase step, 65536 == one cycle.
//            amp: current envelope level in S8.23.
// Helpers exported into user code on all backends: cos_lut(phase_q23) (AMY's
// S8.23 cosine, phase 0..1<<23 == one cycle), fxmul(a,b) (S8.23 multiply),
// to_int16(s)/from_int16(v) (S8.23 <-> plain 16-bit PCM).
// Sources may be a full program or just the function body (auto-wrapped with
// the signature + helper prototypes). State between calls = statics in the
// body; per-osc polyphonic state = static arrays indexed by osc.
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
