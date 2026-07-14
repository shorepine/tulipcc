// MicroPython runtime host for AMYboard VCV — the headless analogue of
// tulip/macos/main.c's main_() thread. The Rack module (C++) calls
// amyboard_vcv_mp_start()/stop() and queues Python via amyboard_vcv_exec().

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "py/compile.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "py/gc.h"
#include "py/objstr.h"
#include "py/cstack.h"
#include "py/mphal.h"
#include "py/mpthread.h"
#include "extmod/vfs.h"
#include "extmod/vfs_posix.h"
#include "genhdr/mpversion.h"
#include "shared/runtime/pyexec.h"

#include "tsequencer.h"
#include "amy.h"

// Port symbols normally defined by the unix port's main.c:
static void stderr_print_strn(void *env, const char *str, size_t len) {
    (void)env;
    fwrite(str, 1, len, stderr);
}
const mp_print_t mp_stderr_print = {NULL, stderr_print_strn};

NORETURN void nlr_jump_fail(void *val) {
    (void)val;
    fprintf(stderr, "AMYboard VCV: FATAL: uncaught NLR\n");
    abort();
}

#ifdef _WIN32
// The windows port has no mpthreadport, so MICROPY_PY_THREAD is off there —
// but mp_sched_schedule is still called from the audio/MIDI threads while
// the VM runs on this one. win/mpconfigport.h routes the VM's atomic
// section through this mutex so the scheduler ring stays consistent.
static pthread_mutex_t vcv_atomic_mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int amyboard_vcv_atomic_begin(void) {
    pthread_mutex_lock(&vcv_atomic_mutex);
    return 0;
}
void amyboard_vcv_atomic_end(unsigned int state) {
    (void)state;
    pthread_mutex_unlock(&vcv_atomic_mutex);
}
// ports/windows/init.c: console/fmode setup.
extern void init(void);
#endif

// GC heap: 8MB on 64-bit, same as Tulip Desktop.
static long heap_size = 4 * 1024 * 1024 * (sizeof(mp_uint_t) / 4);
static char *heap = NULL;

static pthread_t mp_thread_id;
static volatile int vcv_mp_running = 0;
static volatile int vcv_mp_booted = 0;

// Simple mutex-guarded queue of Python source strings to run on the MP thread.
#define EXEC_QUEUE_SLOTS 16
static char *exec_queue[EXEC_QUEUE_SLOTS];
static int exec_head = 0, exec_tail = 0;
static pthread_mutex_t exec_lock = PTHREAD_MUTEX_INITIALIZER;

void amyboard_vcv_exec(const char *code) {
    pthread_mutex_lock(&exec_lock);
    int next = (exec_tail + 1) % EXEC_QUEUE_SLOTS;
    if (next != exec_head) {
        exec_queue[exec_tail] = strdup(code);
        exec_tail = next;
    }
    pthread_mutex_unlock(&exec_lock);
}

static char *exec_pop(void) {
    char *code = NULL;
    pthread_mutex_lock(&exec_lock);
    if (exec_head != exec_tail) {
        code = exec_queue[exec_head];
        exec_head = (exec_head + 1) % EXEC_QUEUE_SLOTS;
    }
    pthread_mutex_unlock(&exec_lock);
    return code;
}

// Shorepine control-API sysex payloads (zT/zD/zP/zA/zY + base64 transfer
// chunks) queued from the CoreMIDI thread (vcv_midi.c). The MP thread
// processes each with amy_add_message_from_sysex() and then emits the AK —
// same process-then-ack contract as hardware's scheduled
// tulip_amy_send_sysex. The editor keeps one frame in flight awaiting each
// AK, so a modest queue suffices.
#define SYSEX_QUEUE_SLOTS 32
static char *sysex_queue[SYSEX_QUEUE_SLOTS];
static int sysex_head = 0, sysex_tail = 0;
static pthread_mutex_t sysex_lock = PTHREAD_MUTEX_INITIALIZER;

void amyboard_vcv_sysex_push(const uint8_t *payload, int len) {
    if (len <= 0)
        return;
    char *copy = malloc(len + 1);
    if (!copy)
        return;
    memcpy(copy, payload, len);
    copy[len] = 0;
    pthread_mutex_lock(&sysex_lock);
    int next = (sysex_tail + 1) % SYSEX_QUEUE_SLOTS;
    if (next != sysex_head) {
        sysex_queue[sysex_tail] = copy;
        sysex_tail = next;
    } else {
        free(copy);  // full — drop (editor will time out and retry)
    }
    pthread_mutex_unlock(&sysex_lock);
}

static char *sysex_pop(void) {
    char *msg = NULL;
    pthread_mutex_lock(&sysex_lock);
    if (sysex_head != sysex_tail) {
        msg = sysex_queue[sysex_head];
        sysex_head = (sysex_head + 1) % SYSEX_QUEUE_SLOTS;
    }
    pthread_mutex_unlock(&sysex_lock);
    return msg;
}

// Process one control frame on the MP thread. amy_add_message_from_sysex
// routes transfer chunks correctly and runs the zP/zA hooks, which call
// into MicroPython — hence the NLR guard.
static void process_sysex_msg(char *msg) {
    extern void midi_out(uint8_t *bytes, uint16_t len);
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        amy_add_message_from_sysex(msg);
        nlr_pop();
    } else {
        fprintf(stderr, "AMYboard VCV: control frame raised, ignoring\n");
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
    }
    static uint8_t ack[] = {0xF0, 0x00, 0x03, 0x45, 'A', 'K', 0xF7};
    midi_out(ack, sizeof(ack));
}

static void exec_str(const char *src) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, MP_PARSE_FILE_INPUT);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, false);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
    }
}

static MP_NOINLINE void *mp_thread_body(void *vargs) {
    (void)vargs;
    #if MICROPY_PY_THREAD
    mp_thread_init();
    #endif

    mp_uint_t stack_size = 40000 * (sizeof(void *) / 4);
    mp_cstack_init_with_sp_here(stack_size);

    // The windows port enables MICROPY_ENABLE_PYSTACK; without this pool the
    // VM's first bytecode frame raises "pystack exhausted" (unix main.c:524
    // does the same init).
    #if MICROPY_ENABLE_PYSTACK
    static mp_obj_t pystack[1024];
    mp_pystack_init(pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
    #endif

    #ifdef SIGPIPE
    signal(SIGPIPE, SIG_IGN);
    #endif
    #ifdef _WIN32
    init();
    #endif

    heap = malloc(heap_size);
    gc_init(heap, heap + heap_size);

    mp_init();

    #if MICROPY_VFS_POSIX
    {
        // Mount the host FS at the root of the internal VFS.
        mp_obj_t args[2] = {
            MP_OBJ_TYPE_GET_SLOT(&mp_type_vfs_posix, make_new)(&mp_type_vfs_posix, 0, 0, NULL),
            MP_OBJ_NEW_QSTR(MP_QSTR__slash_),
        };
        mp_vfs_mount(2, args, (mp_map_t *)&mp_const_empty_map);
        MP_STATE_VM(vfs_cur) = MP_STATE_VM(vfs_mount_table);
    }
    #endif

    // sys.path = [".frozen", ""] — frozen firmware first so nothing in the
    // host's unpredictable cwd can shadow tulip/amy/midi; "" stays so
    // run_sketch()'s `import sketch` works after cd'ing to user/current.
    mp_sys_path = mp_obj_new_list(0, NULL);
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__dot_frozen));
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
    mp_obj_list_init(MP_OBJ_TO_PTR(mp_sys_argv), 0);

    pyexec_frozen_module("_boot.py", false);
    vcv_mp_booted = 1;

    // Headless pump loop instead of a REPL: drain scheduled callbacks
    // (sequencer/defer/MIDI, posted from the audio thread) and queued exec
    // strings from the Rack module.
    while (vcv_mp_running) {
        mp_handle_pending(true);
        char *msg = sysex_pop();
        if (msg) {
            process_sysex_msg(msg);
            free(msg);
        }
        char *code = exec_pop();
        if (code) {
            exec_str(code);
            free(code);
        }
        usleep(2000);
    }

    #if MICROPY_PY_THREAD
    mp_thread_deinit();
    #endif
    mp_deinit();
    free(heap);
    heap = NULL;
    return NULL;
}

int amyboard_vcv_mp_start(void) {
    if (vcv_mp_running)
        return 0;
    vcv_mp_running = 1;
    vcv_mp_booted = 0;
    if (pthread_create(&mp_thread_id, NULL, mp_thread_body, NULL)) {
        vcv_mp_running = 0;
        return -1;
    }
    return 0;
}

int amyboard_vcv_mp_booted(void) {
    return vcv_mp_booted;
}

void amyboard_vcv_mp_stop(void) {
    if (!vcv_mp_running)
        return;
    vcv_mp_running = 0;
    pthread_join(mp_thread_id, NULL);
    vcv_mp_booted = 0;
}
