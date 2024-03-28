//  sequencer.h
#ifndef __SEQUENCERH
#define __SEQUENCERH

#define SEQUENCER_SLOTS 4
#include "py/mphal.h"
#include "py/runtime.h"
#include <stdio.h>
#include "polyfills.h"

// Things that MP can change
extern float sequencer_bpm ;
extern uint8_t sequencer_ppq ;
extern uint16_t sequencer_latency_ms ;
extern uint8_t sequencer_running; 

extern mp_obj_t sequencer_callbacks[SEQUENCER_SLOTS];
extern uint8_t sequencer_dividers[SEQUENCER_SLOTS];

// Our internal accounting
extern uint32_t sequencer_tick_count ;
extern uint64_t next_amy_tick_us ;
extern uint32_t us_per_tick ;


#ifndef ESP_PLATFORM
void * run_sequencer(void *vargs);
#else
void run_sequencer();
#endif

void sequencer_init();
void sequencer_start();
void sequencer_stop();
void sequencer_recompute();

#endif
