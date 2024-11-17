//  tsequencer.h
#ifndef __TSEQUENCERH
#define __TSEQUENCERH

#define SEQUENCER_SLOTS 8
#define DEFER_SLOTS 32
#include "py/mphal.h"
#include "py/runtime.h"
#include <stdio.h>
#include "polyfills.h"
#ifndef __EMSCRIPTEN__
#include "sequencer.h" 
#else
#define AMY_SEQUENCER_PPQ 48
#endif
extern mp_obj_t sequencer_callbacks[SEQUENCER_SLOTS];
extern uint8_t sequencer_dividers[SEQUENCER_SLOTS];

extern mp_obj_t defer_callbacks[DEFER_SLOTS];
extern uint32_t defer_sysclock[DEFER_SLOTS];
extern mp_obj_t defer_args[DEFER_SLOTS];


void tsequencer_init();

#endif