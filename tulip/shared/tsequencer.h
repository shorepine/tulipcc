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
// sequencer_callbacks, defer_callbacks and defer_args are GC root pointers,
// aliased to MP_STATE_VM slots in tulip_helpers.h
#include "tulip_helpers.h"
extern uint32_t sequencer_period[SEQUENCER_SLOTS];
extern uint32_t sequencer_tick[SEQUENCER_SLOTS];
extern uint32_t defer_sysclock[DEFER_SLOTS];


void tsequencer_init();
void tulip_amy_sequencer_hook(uint32_t tick_count);

#endif