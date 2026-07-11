// user_c_dsp.h
// Runtime-installed user C DSP ("c_process") for Tulip/AMYboard.
// User code defines: void process(int *buf, int frames, int chans)
// where buf is AMY's per-bus SAMPLE buffer (int32 s8.23 fixed point),
// laid out as chans sequential (non-interleaved) blocks of frames samples.
// See docs/user_c_dsp_design.md.
#pragma once

#include <stdint.h>

// Registered as amy_config.amy_external_bus_dsp_hook (SAMPLE == int32_t s8.23).
void tulip_bus_dsp_hook(uint8_t bus, int32_t *buf, uint16_t len);

// Compile src and install it under name (replacing any existing install of the
// same name, keeping its bus enables). Returns slot index >= 0, or -1 with a
// message in err.
int user_c_dsp_install(const char *name, const char *src, char *err, int errlen);

// Enable (on=1) / disable (on=0) a named process on a bus. Returns 0, or
// -1 (no such name) / -2 (bad bus).
int user_c_dsp_set(const char *name, int bus, int on);

// Number of blocks processed since install. Returns -1 if no such name.
int64_t user_c_dsp_calls(const char *name);

// Remove a named process and free its compiled code. Returns 0 or -1.
int user_c_dsp_uninstall(const char *name);
