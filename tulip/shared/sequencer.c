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
float sequencer_bpm = 108; // verified optimal BPM 
uint8_t sequencer_ppq = 48;
uint16_t sequencer_latency_ms = 50;
mp_obj_t sequencer_callbacks[SEQUENCER_SLOTS];
uint8_t sequencer_dividers[SEQUENCER_SLOTS];
uint8_t sequencer_running = 1;

// Our internal accounting
uint32_t tick_count = 0;
uint64_t next_amy_tick_us = 0;
uint32_t us_per_tick = 0;

#ifdef ESP_PLATFORM
#include "esp_timer.h"
esp_timer_handle_t periodic_timer;
#endif

void sequencer_recompute() {
    us_per_tick = (uint32_t) (1000000.0 / ((sequencer_bpm/60.0) * (float)sequencer_ppq));    
    next_amy_tick_us = amy_sysclock()*1000 + us_per_tick;
}

void sequencer_start() {
    sequencer_recompute();
    sequencer_running = 1;
    #ifdef ESP_PLATFORM
    // Restart the timer
    run_sequencer();
    #endif
}


void sequencer_stop() {
    sequencer_running = 0;
    #ifdef ESP_PLATFORM
    // Kill the timer
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
    #endif
}


void sequencer_init() {
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) { sequencer_callbacks[i] = NULL; sequencer_dividers[i] = 0; }
    sequencer_recompute();    
}

/*
#define SEQUENCER_RINGBUF_SIZE 100
mp_obj_list_t ** sequencer_tuples;
void sequencer_alloc_ringbuf() {
    sequencer_tuples = malloc_caps(sizeof(mp_obj_list_t*)*SEQUENCER_RINGBUF_SIZE, 0);
    for(uint8_t i=0;i<SEQUENCER_RINGBUF_SIZE;i++) {
        mp_obj_t list[2];
        list[0] = mp_obj_new_int(0);
        list[1] = mp_obj_new_int(0);        
        sequencer_tuples[i] = MP_OBJ_TO_PTR(mp_obj_new_list(2, list));
    }
    fprintf(stderr, "allocated\n");
    //mp_sched_schedule(sequencer_callback, mp_obj_new_tuple(2, tuple));
}
*/

static void sequencer_check_and_fill() {
    // The while is in case the timer fires later than a tick; (on esp this would be due to SPI or wifi ops), we can still use our latency to keep up
    while(amy_sysclock()  >= (next_amy_tick_us/1000)) {
        tick_count++;
        for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
            if(sequencer_dividers[i]!=0) {
                if(tick_count % sequencer_dividers[i] == 0) {
                    //mp_obj_list_t * my_preallocated_tuple = sequencer_tuples[tick_count % SEQUENCER_RINGBUF_SIZE];
                    //my_preallocated_tuple->items[0] = mp_obj_new_int((next_amy_tick_us/1000)+sequencer_latency_ms);
                    //my_preallocated_tuple->items[1] = mp_obj_new_int(tick_count);
                    //mp_sched_schedule(sequencer_callbacks[i], my_preallocated_tuple);
                    mp_sched_schedule(sequencer_callbacks[i], mp_obj_new_int((next_amy_tick_us/1000)+sequencer_latency_ms));
                }
            }
        }
        next_amy_tick_us = next_amy_tick_us + us_per_tick;
    }
}

// Desktop: called from a thread
#ifndef ESP_PLATFORM
void * run_sequencer(void *vargs) {
    // Loop forever, checking for time and sleeping
    while(1) {
        if(sequencer_running) sequencer_check_and_fill();            
        // 500000nS = 500uS = 0.5mS
        nanosleep((const struct timespec[]){{0, 500000L}}, NULL);
    }
}

// ESP: do it with hardware timer
#else
static void sequencer_timer_callback(void* arg) {
    sequencer_check_and_fill();
}

void run_sequencer() {
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &sequencer_timer_callback,
            .name = "sequencer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    // 500uS = 0.5mS
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500));
}
#endif
