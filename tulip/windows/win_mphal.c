/*
 * Tulip Desktop — Windows (MinGW) MicroPython HAL.
 *
 * Ported from micropython/ports/windows/windows_mphal.c, with the Tulip
 * Desktop additions from tulip/shared/desktop/unix_mphal.c:
 *   - mp_hal_stdout_tx_strn() also mirrors output to the on-screen text
 *     framebuffer via display_tfb_str() (the Tulip REPL "screen"), like the
 *     linux/macos desktop builds.
 *   - a stdin_ringbuf that tulip_helpers.c feeds (paste / injected keys).
 *   - a console-vs-pipe aware stdin so `echo ... | tulip.exe` and
 *     `tulip.exe < file` work headlessly (ReadConsoleInput only works on a
 *     real console; redirected stdin needs plain read()).
 *
 * It also provides the mutex-backed atomic section used by mpconfigport.h
 * (threads are OFF on the windows port, but the AMY audio + sequencer threads
 * post to the scheduler ring while the VM runs on its own winpthread).
 */

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mpthread.h"
#include "py/ringbuf.h"

#include <sys/time.h>
#include <windows.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <bcrypt.h>

#include "display.h"

// ---------------------------------------------------------------------------
// Cross-thread-safe scheduler atomic section (mpconfigport.h hooks).
// The windows port has no mpthreadport, so MICROPY_PY_THREAD is off — but
// mp_sched_schedule() is still called from the AMY audio thread / sequencer
// while the VM runs on this thread. Guard the scheduler ring with a mutex.
static pthread_mutex_t tulip_atomic_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int tulip_win_atomic_begin(void) {
    pthread_mutex_lock(&tulip_atomic_mutex);
    return 0;
}
void tulip_win_atomic_end(unsigned int state) {
    (void)state;
    pthread_mutex_unlock(&tulip_atomic_mutex);
}

// ---------------------------------------------------------------------------
// stdin ring buffer (fed by tulip_helpers.c for paste / injected input).
uint8_t stdin_ringbuf_array[260];
ringbuf_t stdin_ringbuf = {stdin_ringbuf_array, sizeof(stdin_ringbuf_array), 0, 0};

// ---------------------------------------------------------------------------
static HANDLE std_in = NULL;
static HANDLE con_out = NULL;
static DWORD orig_mode = 0;
static int stdin_is_console = -1; // -1 = unknown, 0 = pipe/file, 1 = console

static void assure_stdin_handle(void) {
    if (!std_in) {
        std_in = GetStdHandle(STD_INPUT_HANDLE);
    }
    if (stdin_is_console < 0) {
        DWORD mode;
        // A real console handle answers GetConsoleMode(); pipes/files don't.
        stdin_is_console = (std_in && std_in != INVALID_HANDLE_VALUE &&
                            GetConsoleMode(std_in, &mode)) ? 1 : 0;
    }
}

static void assure_conout_handle(void) {
    if (!con_out) {
        con_out = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, 0, 0);
    }
}

// Attach (or create) a console and rebind CRT stdio to it. The GUI-subsystem
// build has no console by default; main.c calls this at startup (attach to a
// parent terminal, or AllocConsole for --console) and tulip.win_console()
// calls it at runtime. Std handles that are already live pipes/files (ssh,
// shell redirection) are snapshotted BEFORE attaching — AllocConsole/
// AttachConsole overwrite the std handles — and left alone, or -c output
// would vanish into the (invisible) console.
static int std_handle_is_live(DWORD which) {
    HANDLE h = GetStdHandle(which);
    return h != NULL && h != INVALID_HANDLE_VALUE;
}

int tulip_win_console_attach(int alloc_new) {
    int in_live = std_handle_is_live(STD_INPUT_HANDLE);
    int out_live = std_handle_is_live(STD_OUTPUT_HANDLE);
    int err_live = std_handle_is_live(STD_ERROR_HANDLE);
    BOOL ok = alloc_new ? AllocConsole() : AttachConsole(ATTACH_PARENT_PROCESS);
    if (!ok && GetConsoleWindow() == NULL) {
        return 0;
    }
    if (!in_live) {
        freopen("CONIN$", "r", stdin);
        SetStdHandle(STD_INPUT_HANDLE, CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0));
        std_in = NULL;
        stdin_is_console = -1;
    }
    if (!out_live) {
        freopen("CONOUT$", "w", stdout);
        setvbuf(stdout, NULL, _IONBF, 0);
    }
    if (!err_live) {
        freopen("CONOUT$", "w", stderr);
        setvbuf(stderr, NULL, _IONBF, 0);
    }
    con_out = NULL;
    return 1;
}

// Python-facing: open a debug console window at runtime.
int tulip_win_console_open(void) {
    return tulip_win_console_attach(1);
}


void mp_hal_stdio_mode_raw(void) {
    assure_stdin_handle();
    if (!stdin_is_console) {
        return;
    }
    GetConsoleMode(std_in, &orig_mode);
    DWORD mode = orig_mode;
    mode &= ~ENABLE_ECHO_INPUT;
    mode &= ~ENABLE_LINE_INPUT;
    mode &= ~ENABLE_PROCESSED_INPUT;
    SetConsoleMode(std_in, mode);
}

void mp_hal_stdio_mode_orig(void) {
    assure_stdin_handle();
    if (!stdin_is_console) {
        return;
    }
    SetConsoleMode(std_in, orig_mode);
}

// Handler installed by SetConsoleCtrlHandler (from init.c) to handle Ctrl-C.
BOOL WINAPI console_sighandler(DWORD evt) {
    if (evt == CTRL_C_EVENT) {
        if (MP_STATE_MAIN_THREAD(mp_pending_exception) == MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_kbd_exception))) {
            exit(1);
        }
        mp_sched_keyboard_interrupt();
        return TRUE;
    }
    return FALSE;
}

// NOTE: mp_hal_set_interrupt_char() is provided by
// micropython/shared/runtime/interrupt_char.c (compiled in via SHARED_SRC_C,
// alongside the mp_interrupt_char global it also owns). We deliberately do not
// define it here to avoid a duplicate symbol; console Ctrl-C is handled by
// console_sighandler above.

void mp_hal_move_cursor_back(uint pos) {
    if (!pos) {
        return;
    }
    assure_conout_handle();
    if (!con_out || con_out == INVALID_HANDLE_VALUE) {
        return;
    }
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con_out, &info);
    info.dwCursorPosition.X -= (short)pos;
    while (info.dwCursorPosition.X < 0) {
        info.dwCursorPosition.X = info.dwSize.X + info.dwCursorPosition.X;
        info.dwCursorPosition.Y -= 1;
    }
    if (info.dwCursorPosition.Y < 0) {
        info.dwCursorPosition.X = 0;
        info.dwCursorPosition.Y = 0;
    }
    SetConsoleCursorPosition(con_out, info.dwCursorPosition);
}

void mp_hal_erase_line_from_cursor(uint n_chars_to_erase) {
    assure_conout_handle();
    if (!con_out || con_out == INVALID_HANDLE_VALUE) {
        return;
    }
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con_out, &info);
    DWORD written;
    FillConsoleOutputCharacter(con_out, ' ', n_chars_to_erase, info.dwCursorPosition, &written);
    FillConsoleOutputAttribute(con_out, info.wAttributes, n_chars_to_erase, info.dwCursorPosition, &written);
}

typedef struct item_t {
    WORD vkey;
    const char *seq;
} item_t;

static item_t keyCodeMap[] = {
    {VK_UP, "[A"},
    {VK_DOWN, "[B"},
    {VK_RIGHT, "[C"},
    {VK_LEFT, "[D"},
    {VK_HOME, "[H"},
    {VK_END, "[F"},
    {VK_DELETE, "[3~"},
    {0, ""}
};

static item_t ctrlKeyCodeMap[] = {
    {VK_LEFT, "b"},
    {VK_RIGHT, "f"},
    {VK_DELETE, "d"},
    {VK_BACK, "\x7F"},
    {0, ""}
};

static const char *cur_esc_seq = NULL;

static int esc_seq_process_vk(WORD vk, bool ctrl_key_down) {
    for (item_t *p = (ctrl_key_down ? ctrlKeyCodeMap : keyCodeMap); p->vkey != 0; ++p) {
        if (p->vkey == vk) {
            cur_esc_seq = p->seq;
            return 27;
        }
    }
    return 0;
}

static int esc_seq_chr(void) {
    if (cur_esc_seq) {
        const char c = *cur_esc_seq++;
        if (c) {
            return c;
        }
        cur_esc_seq = NULL;
    }
    return 0;
}

int mp_hal_stdin_rx_chr(void) {
    // currently processing escape seq?
    const int e = esc_seq_chr();
    if (e) {
        return e;
    }

    assure_stdin_handle();

    // Redirected stdin (pipe / file): plain byte reads; also drain the Tulip
    // injection ring so paste / scripted keys work headlessly.
    if (!stdin_is_console) {
        for (;;) {
            unsigned char c;
            ssize_t ret = read(0, &c, 1);
            if (ret == 1) {
                return c;
            }
            int rc = ringbuf_get(&stdin_ringbuf);
            if (rc != -1) {
                return rc;
            }
            if (ret == 0) {
                return CHAR_CTRL_D; // EOF
            }
            MICROPY_EVENT_POLL_HOOK
        }
    }

    // Real console: read key events. Never block indefinitely in
    // ReadConsoleInput — keys typed into the Tulip SDL window land in
    // stdin_ringbuf from the display thread, and a blocking console read
    // would sit on them until the *console* window saw an event (this was
    // the "REPL only echoes when I click the terminal" bug). Wait on the
    // console handle with a short timeout and re-poll the ringbuf.
    BOOL status;
    DWORD num_read;
    INPUT_RECORD rec;
    for (;;) {
        int rc = ringbuf_get(&stdin_ringbuf);
        if (rc != -1) {
            return rc;
        }
        MP_THREAD_GIL_EXIT();
        DWORD w = WaitForSingleObject(std_in, 20);
        if (w != WAIT_OBJECT_0) {
            MP_THREAD_GIL_ENTER();
            MICROPY_EVENT_POLL_HOOK
            continue;
        }
        status = ReadConsoleInput(std_in, &rec, 1, &num_read);
        MP_THREAD_GIL_ENTER();
        if (!status || !num_read) {
            return CHAR_CTRL_D; // EOF
        }
        if (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown) {
            continue;
        }
        const bool ctrl_key_down = (rec.Event.KeyEvent.dwControlKeyState & LEFT_CTRL_PRESSED) ||
            (rec.Event.KeyEvent.dwControlKeyState & RIGHT_CTRL_PRESSED);
        const int ret = esc_seq_process_vk(rec.Event.KeyEvent.wVirtualKeyCode, ctrl_key_down);
        if (ret) {
            return ret;
        }
        const char c = rec.Event.KeyEvent.uChar.AsciiChar;
        if (c) {
            return c;
        }
    }
}

// Route stdout to the console AND the Tulip on-screen text framebuffer, exactly
// like unix_mphal.c does for the linux/macos desktop builds.
mp_uint_t mp_hal_stdout_tx_strn(const char *str, size_t len) {
    MP_THREAD_GIL_EXIT();
    int ret = write(STDOUT_FILENO, str, len);
    MP_THREAD_GIL_ENTER();
    if (len) {
        display_tfb_str((unsigned char *)str, len, 0, tfb_fg_pal_color, tfb_bg_pal_color);
    }
    // In the console-less GUI build the console write fails; the on-screen
    // TFB consumed the output, so report it written.
    return ret < 0 ? (mp_uint_t)len : (mp_uint_t)ret;
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    mp_hal_stdout_tx_strn(str, len);
}

void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}

// NB: gettimeofday's struct timeval.tv_sec is a 32-bit long on Windows
// (LLP64), so `tv_sec * 1000` overflows 32-bit arithmetic and returns
// garbage. Use the native monotonic Windows clocks instead.
mp_uint_t mp_hal_ticks_ms(void) {
    return (mp_uint_t)GetTickCount64();
}

static LARGE_INTEGER _qpc_freq;  // ticks/sec; lazily initialized
mp_uint_t mp_hal_ticks_us(void) {
    if (_qpc_freq.QuadPart == 0)
        QueryPerformanceFrequency(&_qpc_freq);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (mp_uint_t)((now.QuadPart * 1000000ULL) / _qpc_freq.QuadPart);
}

mp_uint_t mp_hal_ticks_cpu(void) {
    LARGE_INTEGER value;
    QueryPerformanceCounter(&value);
    #ifdef _WIN64
    return value.QuadPart;
    #else
    return value.LowPart;
    #endif
}

uint64_t mp_hal_time_ns(void) {
    // Wall-clock ns. GetSystemTimePreciseAsFileTime is 100ns since 1601;
    // shift the epoch to 1970 (11644473600 s) for a sane absolute time.
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);
    uint64_t t = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    return (t - 116444736000000000ULL) * 100ULL;
}

void msec_sleep(double msec) {
    if (msec < 0.0) {
        msec = 0.0;
    }
    SleepEx((DWORD)msec, TRUE);
}

void mp_hal_delay_ms(mp_uint_t ms) {
    #if MICROPY_ENABLE_SCHEDULER
    mp_uint_t start = mp_hal_ticks_ms();
    while (mp_hal_ticks_ms() - start < ms) {
        mp_event_wait_ms(1);
    }
    #else
    msec_sleep((double)ms);
    #endif
}

void mp_hal_get_random(size_t n, void *buf) {
    NTSTATUS result = BCryptGenRandom(NULL, (unsigned char *)buf, n, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (!BCRYPT_SUCCESS(result)) {
        mp_raise_OSError(errno);
    }
}

// tulip.app_path() — on macOS this is the .app bundle path; there is no bundle
// on Windows, so return the directory the .exe lives in (with forward slashes).
// desktop_copy_sys() only acts when this ends in ".app", so it no-ops here.
char *get_tulip_home_path(void) {
    static char path[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, path, sizeof(path));
    if (len == 0 || len >= sizeof(path)) {
        path[0] = '.';
        path[1] = 0;
        return path;
    }
    // Strip the executable filename, keep the directory.
    for (int i = (int)len - 1; i >= 0; --i) {
        if (path[i] == '\\' || path[i] == '/') {
            path[i] = 0;
            break;
        }
    }
    for (char *p = path; *p; ++p) {
        if (*p == '\\') {
            *p = '/';
        }
    }
    return path;
}
