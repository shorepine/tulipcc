/*
 * Tulip Desktop — Windows (MinGW) entry point.
 *
 * Derived from tulip/linux/main.c. Differences for Windows:
 *   - SDL_MAIN_HANDLED + SDL_SetMainReady() (Tulip provides its own main()).
 *   - MicroPython runs on a winpthread; threads are OFF in the port (see
 *     mpconfigport.h + win_mphal.c's mutex-backed atomic section).
 *   - The display loop only runs if an SDL window was actually created; over a
 *     headless session (e.g. ssh) SDL video init fails, `window` stays NULL,
 *     and we simply wait on the MicroPython thread so the REPL / `-c` still runs.
 *   - A minimal `-c "<code>"` fast path so boot can be proven headlessly:
 *     `tulip.exe -c "import tulip; print(tulip.board())"`.
 *
 * This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)  Copyright (c) 2013-2017 the MicroPython authors.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/objstr.h"
#include "py/cstack.h"
#include "py/mphal.h"
#include "extmod/misc.h"
#include "extmod/modplatform.h"
#include "extmod/vfs.h"
#include "extmod/vfs_posix.h"
#include "genhdr/mpversion.h"
#include "input.h"
#include "display.h"
#include "shared/runtime/pyexec.h"

// Command line options, with their defaults
static bool compile_only = false;
static uint emit_opt = MP_EMIT_OPT_NONE;

// If a `-c "<code>"` was passed, run it after _boot and then exit.
static const char *win_exec_cmd = NULL;

#if MICROPY_ENABLE_GC
long heap_size = 4 * 1024 * 1024 * (sizeof(mp_uint_t) / 4);
#endif

extern int unix_display_draw(void);
extern void unix_display_init(void);
extern int8_t unix_display_flag;
extern SDL_Window *window; // global in unix_display.c; NULL when headless

static void stderr_print_strn(void *env, const char *str, size_t len) {
    (void)env;
    ssize_t ret;
    MP_HAL_RETRY_SYSCALL(ret, write(STDERR_FILENO, str, len), {});
    (void)ret;
    mp_os_dupterm_tx_strn(str, len);
    if (len) {
        display_tfb_str((unsigned char *)str, len, 0, tfb_fg_pal_color, tfb_bg_pal_color);
    }
}

const mp_print_t mp_stderr_print = {NULL, stderr_print_strn};

#define FORCED_EXIT (0x100)
static int handle_uncaught_exception(mp_obj_base_t *exc) {
    if (mp_obj_is_subclass_fast(MP_OBJ_FROM_PTR(exc->type), MP_OBJ_FROM_PTR(&mp_type_SystemExit))) {
        mp_obj_t exit_val = mp_obj_exception_get_value(MP_OBJ_FROM_PTR(exc));
        mp_int_t val = 0;
        if (exit_val != mp_const_none && !mp_obj_get_int_maybe(exit_val, &val)) {
            val = 1;
        }
        return FORCED_EXIT | (val & 255);
    }
    mp_obj_print_exception(&mp_stderr_print, MP_OBJ_FROM_PTR(exc));
    return 1;
}

#define LEX_SRC_STR (1)
#define LEX_SRC_VSTR (2)
#define LEX_SRC_FILENAME (3)
#define LEX_SRC_STDIN (4)

static int execute_from_lexer(int source_kind, const void *source, mp_parse_input_kind_t input_kind, bool is_repl) {
    mp_hal_set_interrupt_char(CHAR_CTRL_C);

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex;
        if (source_kind == LEX_SRC_STR) {
            const char *line = source;
            lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line, strlen(line), false);
        } else if (source_kind == LEX_SRC_VSTR) {
            const vstr_t *vstr = source;
            lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, vstr->buf, vstr->len, false);
        } else if (source_kind == LEX_SRC_FILENAME) {
            const char *filename = (const char *)source;
            lex = mp_lexer_new_from_file(qstr_from_str(filename));
        } else {
            lex = mp_lexer_new_from_fd(MP_QSTR__lt_stdin_gt_, 0, false);
        }

        qstr source_name = lex->source_name;

        #if MICROPY_PY___FILE__
        if (input_kind == MP_PARSE_FILE_INPUT) {
            mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        }
        #endif

        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, is_repl);

        if (!compile_only) {
            mp_call_function_0(module_fun);
        }

        mp_hal_set_interrupt_char(-1);
        mp_handle_pending(true);
        nlr_pop();
        return 0;
    } else {
        mp_hal_set_interrupt_char(-1);
        mp_handle_pending(false);
        return handle_uncaught_exception(nlr.ret_val);
    }
}

#if MICROPY_USE_READLINE == 1
#include "shared/readline/readline.h"
#endif

static int do_repl(void) {
    mp_hal_stdout_tx_str(MICROPY_BANNER_NAME_AND_VERSION);
    mp_hal_stdout_tx_str("; " MICROPY_BANNER_MACHINE);
    mp_hal_stdout_tx_str("\n");

    vstr_t line;
    vstr_init(&line, 16);
    for (;;) {
        mp_hal_stdio_mode_raw();

    input_restart:
        vstr_reset(&line);
        int ret = readline(&line, mp_repl_get_ps1());
        mp_parse_input_kind_t parse_input_kind = MP_PARSE_SINGLE_INPUT;

        if (ret == CHAR_CTRL_C) {
            mp_hal_stdout_tx_str("\r\n");
            goto input_restart;
        } else if (ret == CHAR_CTRL_D) {
            mp_hal_stdout_tx_str("\n");
            mp_hal_stdio_mode_orig();
            vstr_clear(&line);
            return 0;
        } else if (ret == CHAR_CTRL_E) {
            mp_hal_stdout_tx_str("\npaste mode; Ctrl-C to cancel, Ctrl-D to finish\n=== ");
            vstr_reset(&line);
            for (;;) {
                char c = mp_hal_stdin_rx_chr();
                if (c == CHAR_CTRL_C) {
                    mp_hal_stdout_tx_str("\n");
                    goto input_restart;
                } else if (c == CHAR_CTRL_D) {
                    mp_hal_stdout_tx_str("\n");
                    break;
                } else {
                    vstr_add_byte(&line, c);
                    if (c == '\r') {
                        mp_hal_stdout_tx_str("\n=== ");
                    } else {
                        mp_hal_stdout_tx_strn(&c, 1);
                    }
                }
            }
            parse_input_kind = MP_PARSE_FILE_INPUT;
        } else if (line.len == 0) {
            if (ret != 0) {
                mp_hal_stdout_tx_str("\n");
            }
            goto input_restart;
        } else {
            while (mp_repl_continue_with_input(vstr_null_terminated_str(&line))) {
                vstr_add_byte(&line, '\n');
                ret = readline(&line, mp_repl_get_ps2());
                if (ret == CHAR_CTRL_C) {
                    mp_hal_stdout_tx_str("\n");
                    goto input_restart;
                } else if (ret == CHAR_CTRL_D) {
                    break;
                }
            }
        }

        mp_hal_stdio_mode_orig();

        ret = execute_from_lexer(LEX_SRC_VSTR, &line, parse_input_kind, true);
        if (ret & FORCED_EXIT) {
            return ret;
        }
    }
}

static int do_str(const char *str) {
    return execute_from_lexer(LEX_SRC_STR, str, MP_PARSE_FILE_INPUT, false);
}

extern void setup_lvgl(void);

MP_NOINLINE void *main_(void *vargs) {
    (void)vargs;

    mp_uint_t stack_size = 40000 * (sizeof(void *) / 4);
    mp_cstack_init_with_sp_here(stack_size);

    #ifdef SIGPIPE
    signal(SIGPIPE, SIG_IGN);
    #endif

    #if MICROPY_ENABLE_GC
    char *heap = malloc(heap_size);
    gc_init(heap, heap + heap_size);
    #endif

    // The windows port enables MICROPY_ENABLE_PYSTACK; without this pool the
    // VM's first bytecode frame raises "pystack exhausted".
    #if MICROPY_ENABLE_PYSTACK
    static mp_obj_t pystack[1024];
    mp_pystack_init(pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
    #endif

    mp_init();

    #if MICROPY_EMIT_NATIVE
    MP_STATE_VM(default_emit_opt) = emit_opt;
    #else
    (void)emit_opt;
    #endif

    setup_lvgl();

    #if MICROPY_VFS_POSIX
    {
        mp_obj_t args[2] = {
            MP_OBJ_TYPE_GET_SLOT(&mp_type_vfs_posix, make_new)(&mp_type_vfs_posix, 0, 0, NULL),
            MP_OBJ_NEW_QSTR(MP_QSTR__slash_),
        };
        mp_vfs_mount(2, args, (mp_map_t *)&mp_const_empty_map);
        MP_STATE_VM(vfs_cur) = MP_STATE_VM(vfs_mount_table);
    }
    #endif

    // sys.path = [".frozen", ""] — frozen firmware first.
    mp_sys_path = mp_obj_new_list(0, NULL);
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__dot_frozen));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_argv), 0);

    pyexec_frozen_module("_boot.py", false);
    pyexec_file_if_exists("boot.py");

    if (win_exec_cmd != NULL) {
        // Headless proof / scripted run: exec the -c string and exit.
        (void)do_str(win_exec_cmd);
        unix_display_flag = -1; // ask the display loop (if any) to quit
    } else {
        if (pyexec_mode_kind == PYEXEC_MODE_FRIENDLY_REPL) {
            int r = pyexec_file_if_exists("main.py");
            if (r & PYEXEC_FORCED_EXIT) {
                goto soft_reset_exit;
            }
        }
        prompt_read_history();
        do_repl();
        prompt_write_history();
        unix_display_flag = -1; // REPL ended -> quit
    }

soft_reset_exit:
    #if MICROPY_PY_SYS_ATEXIT
    if (mp_obj_is_callable(MP_STATE_VM(sys_exitfunc))) {
        mp_call_function_0(MP_STATE_VM(sys_exitfunc));
    }
    #endif

    mp_deinit();

    #if MICROPY_ENABLE_GC && !defined(NDEBUG)
    free(heap);
    #endif
    return NULL;
}

#include "lvgl.h"
#include "tsequencer.h"
extern void run_amy(uint8_t capture_device_id, uint8_t playback_device_id);

int main(int argc, char **argv) {
    int capture_device_id = -1;
    int playback_device_id = -1;

    // Minimal arg handling (no getopt dependency).
    for (int a = 1; a < argc; a++) {
        if (strcmp(argv[a], "-c") == 0 && a + 1 < argc) {
            win_exec_cmd = argv[a + 1];
            a++;
        } else if (strcmp(argv[a], "-l") == 0) {
            amy_print_devices();
            return 0;
        } else if (strcmp(argv[a], "-d") == 0 && a + 1 < argc) {
            playback_device_id = atoi(argv[a + 1]);
            a++;
        } else if (strcmp(argv[a], "-C") == 0 && a + 1 < argc) {
            capture_device_id = atoi(argv[a + 1]);
            a++;
        } else if (strcmp(argv[a], "-h") == 0) {
            fprintf(stderr, "usage: tulip [-c \"code\"] [-d playback_id] [-C capture_id] [-l] [-h]\n");
            return 0;
        }
    }

    // Tulip owns main(), so tell SDL not to expect its own entry point.
    SDL_SetMainReady();

    unix_display_init(); // over a headless session this leaves `window` == NULL

    run_amy(capture_device_id, playback_device_id);

    pthread_t mp_thread_id;
    pthread_create(&mp_thread_id, NULL, main_, NULL);

    tsequencer_init();
    delay_ms(100);

    if (window == NULL) {
        // Headless: no display. Let MicroPython run (REPL / -c) to the end.
        pthread_join(mp_thread_id, NULL);
        return 0;
    }

display_jump:
    while (unix_display_flag >= 0) {
        int ticks = unix_display_draw();
        int sleep_ms_for_frame = (int)((1000.0 / TARGET_DESKTOP_FPS) - ticks);
        if (sleep_ms_for_frame > 0) {
            usleep(1000 * sleep_ms_for_frame);
        }
    }
    if (unix_display_flag == -2) {
        fprintf(stderr, "restarting display\n");
        unix_display_init();
        unix_display_flag = 0;
        goto display_jump;
    }

    return 0;
}

void nlr_jump_fail(void *val) {
    #if MICROPY_USE_READLINE == 1
    mp_hal_stdio_mode_orig();
    #endif
    fprintf(stderr, "FATAL: uncaught NLR %p\n", val);
    exit(1);
}
