// Tulip Desktop — Windows (MinGW) build config: MicroPython's windows port
// config with the Tulip Desktop deltas on top. Modeled on
// tulip/vcvrack/win/mpconfigport.h (the proven Windows-MicroPython recipe),
// but for the standalone tulip.exe (display + LVGL + ulab, board()=="DESKTOP").
//
// Threads stay OFF (the windows port has no mpthreadport): MicroPython runs on
// a winpthread created by main.c, and the scheduler ring — filled from the
// audio / sequencer threads via mp_sched_schedule — is protected by the
// mutex-backed atomic section below (implemented in win_mphal.c).

// Feature parity with the mac/linux desktop builds: the unix standard variant
// runs at EXTRA_FEATURES, but the windows variant never sets a level (=> CORE)
// and the firmware notices — first casualty was `import framebuf` killing
// init_display at boot. Define the level before the base config so
// mpconfig.h's defaults follow it.
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)

#include "../../micropython/ports/windows/mpconfigport.h"

// Explicit, because this was the original Windows boot failure on VCV:
// amyboard.py's Display and Tulip's framebuf paths need the framebuf module.
#undef MICROPY_PY_FRAMEBUF
#define MICROPY_PY_FRAMEBUF (1)

// The windows base enables allocated-size tracking (m_realloc/m_free take an
// extra old-size arg), but Tulip's LVGL binding (lv_mem_core_micropython.c) is
// written for the linux/macos desktop convention where they don't. Match the
// unix Tulip build.
#undef MICROPY_MALLOC_USES_ALLOCATED_SIZE
#define MICROPY_MALLOC_USES_ALLOCATED_SIZE (0)
// MEM_STATS requires the allocated-size tracking we just turned off; disable it
// (the unix Tulip build disables both together).
#undef MICROPY_MEM_STATS
#define MICROPY_MEM_STATS (0)

// The windows base wires ports/windows/init.c's init()/deinit() into
// mp_init()/mp_deinit(). For the standalone exe there is no Rack `init`
// collision, so we compile ports/windows/init.c normally and keep these hooks.

#undef MICROPY_SCHEDULER_DEPTH
#define MICROPY_SCHEDULER_DEPTH (128)
#ifndef MICROPY_ENABLE_SCHEDULER
#define MICROPY_ENABLE_SCHEDULER (1)
#endif

#undef MICROPY_PY_SYS_PATH_DEFAULT
#define MICROPY_PY_SYS_PATH_DEFAULT ".frozen"

#undef MICROPY_HW_BOARD_NAME
#define MICROPY_HW_BOARD_NAME "Tulip4"

// Tulip's own on-screen help text (help.c) instead of the stock unix one.
#undef MICROPY_PY_BUILTINS_HELP_TEXT
#define MICROPY_PY_BUILTINS_HELP_TEXT tulip_desktop_help_text

// Tulip routes stdout through display_tfb_str; it does not want MP's sys.stdin/
// sys.stdout file objects (matches the linux/macos desktop builds).
#undef MICROPY_PY_SYS_STDFILES
#define MICROPY_PY_SYS_STDFILES (0)

// Tulip uses io helpers.
#ifndef MICROPY_PY_IO
#define MICROPY_PY_IO (1)
#endif

// Cross-thread-safe scheduler: mp_sched_schedule is called from the AMY audio
// thread and the sequencer while the VM runs on its own (win)pthread.
extern unsigned int tulip_win_atomic_begin(void);
extern void tulip_win_atomic_end(unsigned int state);
#undef MICROPY_BEGIN_ATOMIC_SECTION
#undef MICROPY_END_ATOMIC_SECTION
#define MICROPY_BEGIN_ATOMIC_SECTION() tulip_win_atomic_begin()
#define MICROPY_END_ATOMIC_SECTION(state) tulip_win_atomic_end(state)

#include <unistd.h>
#ifndef MICROPY_EVENT_POLL_HOOK
#define MICROPY_EVENT_POLL_HOOK \
    do { \
        extern void mp_handle_pending(bool); \
        mp_handle_pending(true); \
        usleep(500); \
    } while (0);
#endif
