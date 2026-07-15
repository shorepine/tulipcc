// AMYboard VCV — Windows (MinGW) build config: MicroPython's windows port
// config with the tulip/vcvrack deltas on top.
//
// Threads stay OFF (the windows port has no mpthreadport): MicroPython runs
// on a winpthread created by mp_embed.c, and the scheduler ring — filled by
// mp_sched_schedule from the audio/MIDI threads — is protected by the
// mutex-backed atomic section below (implemented in mp_embed.c).

// Feature parity with the mac/linux builds: the unix standard variant runs
// at EXTRA_FEATURES, the windows variant never sets a level (=> CORE) and
// the firmware notices — first casualty was `import framebuf` killing
// init_display at boot. Define the level before the base config so
// mpconfig.h's defaults follow it.
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)

#include "../../../micropython/ports/windows/mpconfigport.h"

// Explicit, because this was the original Windows boot failure:
// amyboard.py's Display needs the framebuf module.
#undef MICROPY_PY_FRAMEBUF
#define MICROPY_PY_FRAMEBUF (1)

// The windows base wires ports/windows/init.c's init()/deinit() into
// mp_init()/mp_deinit(); we don't compile that file (its exported `init`
// collides with Rack's extern-C plugin entry point) — point the hooks at
// the inlined replacement in mp_embed.c instead.
#undef MICROPY_PORT_INIT_FUNC
#undef MICROPY_PORT_DEINIT_FUNC
extern void amyboard_vcv_win_init(void);
#define MICROPY_PORT_INIT_FUNC amyboard_vcv_win_init()
#define MICROPY_PORT_DEINIT_FUNC

#undef MICROPY_SCHEDULER_DEPTH
#define MICROPY_SCHEDULER_DEPTH (128)

#undef MICROPY_PY_SYS_PATH_DEFAULT
#define MICROPY_PY_SYS_PATH_DEFAULT ".frozen"

#undef MICROPY_HW_BOARD_NAME
#define MICROPY_HW_BOARD_NAME "AMYboard VCV"

#undef MICROPY_PY_BUILTINS_HELP_TEXT
#define MICROPY_PY_BUILTINS_HELP_TEXT tulip_desktop_help_text

#undef MICROPY_PY_SYS_STDFILES
#define MICROPY_PY_SYS_STDFILES (0)

// Cross-thread-safe scheduler: mp_sched_schedule is called from the Rack
// audio thread and the MIDI thread while the VM runs on its own thread.
extern unsigned int amyboard_vcv_atomic_begin(void);
extern void amyboard_vcv_atomic_end(unsigned int state);
#define MICROPY_BEGIN_ATOMIC_SECTION() amyboard_vcv_atomic_begin()
#define MICROPY_END_ATOMIC_SECTION(state) amyboard_vcv_atomic_end(state)

#include <unistd.h>
#ifndef MICROPY_EVENT_POLL_HOOK
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        usleep(500); \
    } while (0);
#endif
