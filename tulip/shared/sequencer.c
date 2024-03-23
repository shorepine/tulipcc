// timer.c 
// handle timer-like events for music
// runs in a thread / task


//get_ticks_ms() gets amy ms 
// mp_hal_ticks_us() gets us from MP 

/*
To use the clock in your code, you should first register on the music callback with tulip.seq_add_callback(my_callback). 
We support up to 4 music callbacks running at once. 
You can remove your callback with tulip.seq_remove_callback(my_callback)

When adding a callback, the optional 2nd parameter is a divider on the system level parts-per-quarter timer. 
If you set it to 6, we would call your function my_callback every 6th "tick", so 8 times a quarter note. Set this to 48 (or, tulip.seq_ppq()) to receive a message once a beat.
The default divider is set to ppq. So if you don't set a divider in music_callback, your callback will be called once a beat. 

By default, your callback will receive a message 50 milliseconds ahead of the time of the intended tick, with 
the parameters my_callback(intended_time_ms). This is so that you can take extra CPU time to prepare to send messages at the precise time, 
using AMY scheduling commands, to keep in perfect sync. 

You can set this "lookahead" globally if you want more or less latency with tulip.seq_latency(X) or get it with tulip.seq_latency().

You can set the system-wide BPM (quarters per minute) with tulip.seq_bpm(120) or retrieve it with tulip.seq_bpm(). 

You can change the PPQ with tulip.seq_ppq(new_value) or retrieve it with tulip.seq_ppq(). 

You can pause the sequencer with tulip.seq_pause() and start it with tulip.seq_start().


So..

music_callback -- MP / tulip.py
adds your function to a list of FPs for the music callback, saves off the dividers

in C, we have a timer/thread that calls .. mp_sched_schedule(tick_guy(time)) and tick_guy calls all the callbacks



*/

#include "sequencer.h"

// Things that MP can change
float sequencer_bpm = 108;
uint8_t sequencer_ppq = 48;
uint16_t sequencer_latency_ms = 50;
mp_obj_t sequencer_callbacks[SEQUENCER_SLOTS];
uint8_t sequencer_dividers[SEQUENCER_SLOTS];
uint8_t sequencer_running = 1;

// Our internal accounting
uint32_t tick_count = 0;
uint64_t next_amy_tick_us = 0;
uint32_t us_per_tick = 0;

void sequencer_recompute() {
    us_per_tick = (uint32_t) (1000000.0 / ((sequencer_bpm/60.0) * (float)sequencer_ppq));    
    next_amy_tick_us = amy_sysclock()*1000 + us_per_tick;
}

// called from a thread/task
#ifndef ESP_PLATFORM
void * run_sequencer(void *vargs) {
#else
void run_sequencer() {
#endif
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) { sequencer_callbacks[i] = NULL; sequencer_dividers[i] = 0; }
    sequencer_recompute();
    while(1) {
        if(sequencer_running) {
            // I'm nervous here that sequencer_latency gets bigger than us_per_tick*1000. I think it's ok? 
            if(amy_sysclock()  >= (next_amy_tick_us/1000)  - (sequencer_latency_ms)) {
                tick_count++;
                for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
                    if(sequencer_dividers[i]!=0) {
                        if(tick_count % sequencer_dividers[i] == 0) {
                            mp_sched_schedule(sequencer_callbacks[i], mp_obj_new_int(next_amy_tick_us/1000));
                        }
                    }
                }
                next_amy_tick_us = next_amy_tick_us + us_per_tick;
            }
        }
        #ifndef ESP_PLATFORM
            // Sleep 500000nS / 500uS / 0.5mS
            nanosleep((const struct timespec[]){{0, 500000L}}, NULL);
        #else
            // This will eventually be a timer
            //vTaskDelay(1/portTICK_PERIOD_MS);
        #endif
    }

}

