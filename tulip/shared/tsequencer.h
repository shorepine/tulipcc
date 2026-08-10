//  tsequencer.h
#ifndef __TSEQUENCERH
#define __TSEQUENCERH

#define SEQUENCER_SLOTS 8
#define DEFER_SLOTS 32
#include "py/mphal.h"
#include "py/runtime.h"
#include <stdio.h>
#include "polyfills.h"
#ifndef AMY_IS_EXTERNAL
#include "sequencer.h" 
#else
extern uint32_t sequencer_tick_count;
#define AMY_SEQUENCER_PPQ 48
#endif
extern uint32_t sequencer_period[SEQUENCER_SLOTS];
extern uint32_t sequencer_tick[SEQUENCER_SLOTS];

// 64-bit absolute deadline (get_time_ms()). This was uint32_t off the 32-bit
// tick, so a defer straddling the 49.7-day rollover either fired instantly or
// waited another 49.7 days.
extern int64_t defer_sysclock[DEFER_SLOTS];


void tsequencer_init();
void tulip_amy_sequencer_hook(uint32_t tick_count);

#endif