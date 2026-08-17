
#include "tsequencer.h"
#include <inttypes.h>

MP_REGISTER_ROOT_POINTER(mp_obj_t sequencer_callbacks[8]);
uint32_t sequencer_period[SEQUENCER_SLOTS];
uint32_t sequencer_tick[SEQUENCER_SLOTS];

MP_REGISTER_ROOT_POINTER(mp_obj_t defer_callbacks[32]);
MP_REGISTER_ROOT_POINTER(mp_obj_t defer_args[32]);
int64_t defer_sysclock[DEFER_SLOTS];

#ifdef AMY_IS_EXTERNAL
uint32_t sequencer_tick_count = 0;
#endif

void tulip_amy_sequencer_hook(uint32_t tick_count) {
    #ifdef AMY_IS_EXTERNAL
        sequencer_tick_count = tick_count;
    #endif
    for(uint8_t i=0;i<DEFER_SLOTS;i++) {
        if(MP_STATE_PORT(defer_callbacks)[i] != NULL && get_time_ms() > defer_sysclock[i]) {
            //fprintf(stderr, "calling defer with sysclock %" PRId64 " and actual %" PRId64"\n", defer_sysclock[i], get_time_ms() );
            mp_sched_schedule(MP_STATE_PORT(defer_callbacks)[i], MP_STATE_PORT(defer_args)[i]);
            MP_STATE_PORT(defer_callbacks)[i] = NULL; defer_sysclock[i] = 0; MP_STATE_PORT(defer_args)[i] = NULL;
        }
    }

    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
        if(sequencer_period[i]!=0) {
            uint32_t offset = tick_count % sequencer_period[i];
            if(offset == sequencer_tick[i]) {
                mp_sched_schedule(MP_STATE_PORT(sequencer_callbacks)[i], mp_obj_new_int(tick_count));
            }
        }
    }
}


// The array sizes in MP_REGISTER_ROOT_POINTER above must be literals: that
// macro is collected into genhdr/root_pointers.h, which mpstate.h includes
// long before tsequencer.h is visible. Make any drift a build error rather
// than a silently under-scanned root.
MP_STATIC_ASSERT(SEQUENCER_SLOTS == 8);
MP_STATIC_ASSERT(DEFER_SLOTS == 32);

void tsequencer_init() {
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) { MP_STATE_PORT(sequencer_callbacks)[i] = NULL; sequencer_period[i] = 0; sequencer_tick[i] = 0; }
    for(uint8_t i=0;i<DEFER_SLOTS;i++) { MP_STATE_PORT(defer_callbacks)[i] = NULL; defer_sysclock[i] = 0; }
    #ifndef AMY_IS_EXTERNAL
    amy_global.config.amy_external_sequencer_hook = tulip_amy_sequencer_hook;
    #endif
}
