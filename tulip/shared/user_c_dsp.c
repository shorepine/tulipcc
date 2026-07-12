// user_c_dsp.c
// Runtime-installed user C DSP: compiles a C string from Python
// (tulip.install_c_process) and dispatches it from AMY's per-bus DSP hook.
// Compiler backends: libtcc (desktop). ESP32 (xcc700 + elf_loader) planned.
// See docs/user_c_dsp_design.md.

#ifdef TULIP_USER_C_DSP

#include <stdio.h>
#include <string.h>
#include "amy.h"
#include "user_c_dsp.h"

#define USER_C_DSP_SLOTS 8
#define USER_C_DSP_NAME_LEN 32

typedef void (*user_c_dsp_fn_t)(int16_t *buf, int32_t frames, int32_t chans);

typedef struct {
    char name[USER_C_DSP_NAME_LEN];
    user_c_dsp_fn_t fn;         // NULL = slot inactive; set last (release) on install
    void *compiler_state;       // TCCState* on desktop
    uint32_t bus_mask;
    int64_t calls;
} user_c_dsp_slot_t;

static user_c_dsp_slot_t slots[USER_C_DSP_SLOTS];

// User effects see familiar 16-bit PCM (-32767..32767) rather than AMY's s8.23
// fixed point; we convert at this boundary, only when an effect is enabled on
// the bus. 2x padding so an effect that mistakenly indexes 32-bit ints reads
// and writes in-bounds garbage instead of corrupting neighboring state.
static int16_t pcm_buf[AMY_BLOCK_SIZE * AMY_NCHANS * 2];

// Runs on the AMY render task, once per bus per block.
void tulip_bus_postprocess_hook(uint8_t bus, int32_t *buf, uint16_t len) {
    int converted = 0;
    for (int i = 0; i < USER_C_DSP_SLOTS; i++) {
        user_c_dsp_fn_t fn = __atomic_load_n(&slots[i].fn, __ATOMIC_ACQUIRE);
        if (fn != NULL && (slots[i].bus_mask & (1u << bus))) {
            if (!converted) {
                // s8.23 -> s.15 with clamp (the bus can exceed 1.0 pre-clip).
                for (int j = 0; j < len * AMY_NCHANS; j++) {
                    int32_t v = buf[j] >> 8;
                    if (v > 32767) v = 32767;
                    if (v < -32767) v = -32767;
                    pcm_buf[j] = (int16_t)v;
                }
                converted = 1;
            }
            fn(pcm_buf, len, AMY_NCHANS);
            slots[i].calls++;
        }
    }
    if (converted) {
        for (int j = 0; j < len * AMY_NCHANS; j++) buf[j] = ((int32_t)pcm_buf[j]) << 8;
    }
}

static int find_slot(const char *name) {
    for (int i = 0; i < USER_C_DSP_SLOTS; i++) {
        if (slots[i].name[0] && strncmp(slots[i].name, name, USER_C_DSP_NAME_LEN) == 0) return i;
    }
    return -1;
}

int user_c_dsp_set(const char *name, int bus, int on) {
    if (bus < 0 || bus >= AMY_NUM_BUSES) return -2;
    int i = find_slot(name);
    if (i < 0) return -1;
    if (on) slots[i].bus_mask |= (1u << bus);
    else slots[i].bus_mask &= ~(1u << bus);
    return 0;
}

int64_t user_c_dsp_calls(const char *name) {
    int i = find_slot(name);
    if (i < 0) return -1;
    return slots[i].calls;
}

#ifdef TULIP_DESKTOP

#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <mach-o/dyld.h>
#include <libtcc.h>

// Where tcc finds libtcc1.a and its include/ dir: env override, then the app
// bundle's Resources/tcc (staged by build.sh), then the dev-tree tinycc dir.
static const char *tcc_runtime_path(void) {
    static char path[1152];
    const char *env = getenv("TULIP_TCC_PATH");
    if (env != NULL && env[0]) {
        snprintf(path, sizeof(path), "%s", env);
        return path;
    }
    char exe[1024];
    uint32_t sz = sizeof(exe);
    if (_NSGetExecutablePath(exe, &sz) == 0) {
        snprintf(path, sizeof(path), "%s/../Resources/tcc", dirname(exe));
        char probe[1280];
        snprintf(probe, sizeof(probe), "%s/libtcc1.a", path);
        if (access(probe, R_OK) == 0) return path;
    }
#ifdef TULIP_TCC_DEV_PATH
    return TULIP_TCC_DEV_PATH;
#else
    return path;
#endif
}

typedef struct { char *buf; int len; } tcc_err_sink_t;

static void tcc_err_cb(void *opaque, const char *msg) {
    tcc_err_sink_t *sink = (tcc_err_sink_t *)opaque;
    // Keep the first error; follow-ups are usually cascades.
    if (sink->buf[0] == 0) snprintf(sink->buf, sink->len, "%s", msg);
}

// Drop a slot's compiled code, waiting out any in-flight render block.
static void free_slot_code(user_c_dsp_slot_t *slot) {
    if (slot->fn != NULL) {
        __atomic_store_n(&slot->fn, (user_c_dsp_fn_t)NULL, __ATOMIC_RELEASE);
        usleep(3 * 1000 * AMY_BLOCK_SIZE / AMY_SAMPLE_RATE * 1000);  // ~3 block times
    }
    if (slot->compiler_state != NULL) {
        tcc_delete((TCCState *)slot->compiler_state);
        slot->compiler_state = NULL;
    }
}

int user_c_dsp_install(const char *name, const char *src, char *err, int errlen) {
    err[0] = 0;
    if (name[0] == 0 || strlen(name) >= USER_C_DSP_NAME_LEN) {
        snprintf(err, errlen, "name must be 1..%d chars", USER_C_DSP_NAME_LEN - 1);
        return -1;
    }
    int i = find_slot(name);
    if (i < 0) {
        for (int j = 0; j < USER_C_DSP_SLOTS; j++) {
            if (slots[j].name[0] == 0) { i = j; break; }
        }
    }
    if (i < 0) {
        snprintf(err, errlen, "all %d c_process slots in use", USER_C_DSP_SLOTS);
        return -1;
    }

    tcc_err_sink_t sink = { err, errlen };
    TCCState *s = tcc_new();
    if (s == NULL) {
        snprintf(err, errlen, "tcc_new failed");
        return -1;
    }
    tcc_set_error_func(s, &sink, tcc_err_cb);
    tcc_set_lib_path(s, tcc_runtime_path());
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    if (tcc_compile_string(s, src) == -1) {
        if (err[0] == 0) snprintf(err, errlen, "compile failed");
        tcc_delete(s);
        return -1;
    }
    if (tcc_relocate(s) < 0) {
        if (err[0] == 0) snprintf(err, errlen, "relocate failed");
        tcc_delete(s);
        return -1;
    }
    user_c_dsp_fn_t fn = (user_c_dsp_fn_t)tcc_get_symbol(s, "process");
    if (fn == NULL) {
        snprintf(err, errlen, "no process(int16_t *buf, int frames, int chans) defined");
        tcc_delete(s);
        return -1;
    }

    // Replace-in-place: keep bus enables so the dev loop is edit + reinstall.
    free_slot_code(&slots[i]);
    snprintf(slots[i].name, USER_C_DSP_NAME_LEN, "%s", name);
    slots[i].compiler_state = s;
    slots[i].calls = 0;
    __atomic_store_n(&slots[i].fn, fn, __ATOMIC_RELEASE);
    return i;
}

int user_c_dsp_uninstall(const char *name) {
    int i = find_slot(name);
    if (i < 0) return -1;
    slots[i].bus_mask = 0;
    free_slot_code(&slots[i]);
    slots[i].name[0] = 0;
    slots[i].calls = 0;
    return 0;
}

#else  // !TULIP_DESKTOP

int user_c_dsp_install(const char *name, const char *src, char *err, int errlen) {
    (void)name; (void)src;
    snprintf(err, errlen, "no C compiler backend on this platform yet");
    return -1;
}

int user_c_dsp_uninstall(const char *name) {
    (void)name;
    return -1;
}

#endif  // TULIP_DESKTOP

#endif  // TULIP_USER_C_DSP
