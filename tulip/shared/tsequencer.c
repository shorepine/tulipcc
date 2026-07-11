
#include "tsequencer.h"
#include <inttypes.h>

// sequencer_callbacks, defer_callbacks and defer_args are GC root pointers
// (MP_REGISTER_ROOT_POINTER in modtulip.c, aliases in tulip_helpers.h), which
// registers them with literal sizes -- keep these in sync
_Static_assert(SEQUENCER_SLOTS == 8, "update cb_sequencer[] size in modtulip.c");
_Static_assert(DEFER_SLOTS == 32, "update cb_defer[]/cb_defer_args[] size in modtulip.c");

uint32_t sequencer_period[SEQUENCER_SLOTS];
uint32_t sequencer_tick[SEQUENCER_SLOTS];
uint32_t defer_sysclock[DEFER_SLOTS];

#ifdef AMY_IS_EXTERNAL
uint32_t sequencer_tick_count = 0;
#endif

void tulip_amy_sequencer_hook(uint32_t tick_count) {
    #ifdef AMY_IS_EXTERNAL
        sequencer_tick_count = tick_count;
    #endif
    for(uint8_t i=0;i<DEFER_SLOTS;i++) {
        if(defer_callbacks[i] != NULL && get_ticks_ms() > defer_sysclock[i]) {
            //fprintf(stderr, "calling defer with sysclock %" PRIu32 " and actual %" PRIu32"\n", defer_sysclock[i], get_ticks_ms() );
            mp_sched_schedule(defer_callbacks[i], defer_args[i]);
            defer_callbacks[i] = NULL; defer_sysclock[i] = 0; defer_args[i] = NULL;
        }
    }

    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
        if(sequencer_period[i]!=0) {
            uint32_t offset = tick_count % sequencer_period[i];
            if(offset == sequencer_tick[i]) {
                mp_sched_schedule(sequencer_callbacks[i], mp_obj_new_int(tick_count));
            }
        }
    }
}


void tsequencer_init() {
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) { sequencer_callbacks[i] = NULL; sequencer_period[i] = 0; sequencer_tick[i] = 0; }
    for(uint8_t i=0;i<DEFER_SLOTS;i++) { defer_callbacks[i] = NULL; defer_args[i] = NULL; defer_sysclock[i] = 0; }
    // clear the single-slot callbacks too, so a soft reboot can't leave them
    // pointing at objects from the previous session's heap
    midi_callback = NULL;
    amy_overload_callback = NULL;
    amy_block_done_callback = NULL;
    frame_callback = NULL;
    frame_arg = NULL;
    touch_callback = NULL;
    keyboard_callback = NULL;
    ui_quit_callback = NULL;
    ui_switch_callback = NULL;
    #ifndef AMY_IS_EXTERNAL
    amy_global.config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
    #endif
}
