// user_c_dsp.c
// Runtime-installed user C DSP: compiles a C string from Python and runs it
// inside AMY's render loop, directly on AMY's S8.23 fixed-point buffers
// (int32, 1.0 == 1<<23) -- zero copies. Two kinds:
//   effect — void process(int *buf, int frames, int chans)
//            dispatched from amy_external_bus_postprocess_hook; buf is the
//            bus buffer, chans sequential channel blocks, modify in place
//   osc    — void render(int *buf, int frames, int osc, int phase_inc_q16, int amp)
//            dispatched from amy_external_render_hook; fill buf (mono) with
//            frames samples. phase_inc_q16: per-sample phase step, 65536 ==
//            one cycle (tracks pitch bend). amp: envelope level in S8.23.
// Helpers are exported into user code on both backends (see udsp_helper
// prototypes below): AMY's cos_lut(phase_q23), fxmul(a,b) (S8.23 multiply),
// to_int16(s)/from_int16(v) conversions for people who think in 16-bit PCM.
// Users may pass just the function *body*; we wrap it in the signature and
// helper prototypes. State between calls = statics in the body.
// Compiler backends: libtcc JIT (desktop), xcc700t + esp-idf elf_loader
// (ESP32, code loaded into PSRAM), xcc700w + AudioWorklet-side instantiation
// (web). See docs/user_c_dsp_design.md.

#ifdef TULIP_USER_C_DSP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_c_dsp.h"

#define USER_C_DSP_NAME_LEN 32

// ---- shared across backends ------------------------------------------------

// Prototypes prepended to bare bodies so helpers are just there. Full-program
// sources declare what they use themselves. cos_lut is AMY's: S8.23 cosine,
// phase normalized to one cycle in S8.23 (0..1<<23).
static const char *udsp_helper_protos =
    "int cos_lut(int phase_q23); int fxmul(int a, int b); int to_int16(int s); int from_int16(int v); ";

static const char *kind_symbol(uint8_t kind) {
    return kind == USER_C_DSP_KIND_OSC ? "render" : "process";
}

static const char *kind_signature(uint8_t kind) {
    return kind == USER_C_DSP_KIND_OSC
        ? "void render(int *buf, int frames, int osc, int phase_inc_q16, int amp)"
        : "void process(int *buf, int frames, int chans)";
}

// Does src look like a bare function body (never mentions process(/render()?
static int is_bare_body(const char *src, uint8_t kind) {
    const char *sym = kind_symbol(kind);
    char probe1[16], probe2[24];
    snprintf(probe1, sizeof(probe1), "%s(", sym);
    snprintf(probe2, sizeof(probe2), "void %s", sym);
    return strstr(src, probe1) == NULL && strstr(src, probe2) == NULL;
}

#ifdef __EMSCRIPTEN__
// ------------------------------------------------------------------ web:
// xcc700w (the wasm backend of our vendored mini C compiler) turns the user's
// C into a small standalone WebAssembly module. AMY renders in a *separate*
// wasm module inside an AudioWorklet, so nothing in this module can call the
// compiled code: we hand the module bytes to the page JS (tulip_udsp_install
// in tulip/shared/user_c_dsp_web.js, which the web builds append to AMY's
// amy.js), and it mails them through AMY's shared linear memory — rendezvous
// via amy_set_external_hook_context() — to the worklet side of that same
// file, which instantiates them importing AMY's memory + cos_lut and runs
// them from AMY's js hooks. All slot state (names live on the page; bus
// masks, osc bindings and call counts in the shared control block) lives
// over there, so this backend keeps none.

#include <emscripten.h>
#include "xcc700w.h"

// Bare bodies get the helper prototypes + signature (xcc700 has no
// preprocessor). Keeping the body on the same line as the opening brace
// preserves the user's error line numbers.
static char *wrap_source(const char *src, uint8_t kind) {
    if (!is_bare_body(src, kind)) return NULL;
    size_t n = strlen(udsp_helper_protos) + strlen(kind_signature(kind)) + strlen(src) + 8;
    char *out = malloc(n);
    if (out == NULL) return NULL;
    snprintf(out, n, "%s%s { %s\n}\n", udsp_helper_protos, kind_signature(kind), src);
    return out;
}

int user_c_dsp_install(const char *name, const char *src, uint8_t kind, char *err, int errlen) {
    err[0] = 0;
    if (name[0] == 0 || strlen(name) >= USER_C_DSP_NAME_LEN) {
        snprintf(err, errlen, "name must be 1..%d chars", USER_C_DSP_NAME_LEN - 1);
        return -1;
    }
    char *wrapped = wrap_source(src, kind);
    uint8_t *wasm = NULL;
    uint32_t wasm_size = 0, bss_size = 0, rodata_size = 0;
    int r = xcc700w_compile(wrapped ? wrapped : src, kind_symbol(kind),
                            &wasm, &wasm_size, &bss_size, &rodata_size, err, errlen);
    free(wrapped);
    if (r != 0) return -1;
    // This EM_ASM runs in the page scope (micropython's module), so HEAPU8 is
    // micropython's heap: copy the module bytes out into a plain Uint8Array
    // for the page-side installer.
    int slot = EM_ASM_INT({
        if (typeof globalThis.tulip_udsp_install !== 'function') return -2;
        return globalThis.tulip_udsp_install(UTF8ToString($0), $1, HEAPU8.slice($2, $2 + $3), $4, $5);
    }, name, (int)kind, wasm, (int)wasm_size, (int)bss_size, (int)rodata_size);
    free(wasm);
    if (slot >= 0) return slot;
    if (slot == -2) snprintf(err, errlen, "AMY web module not loaded yet");
    else if (slot == -3) snprintf(err, errlen, "all c_process slots in use");
    else if (slot == -4) snprintf(err, errlen, "'%s' is already installed as a different kind", name);
    else snprintf(err, errlen, "install failed (%d)", slot);
    return -1;
}

int user_c_dsp_set(const char *name, int bus, int on) {
    return EM_ASM_INT({
        return typeof globalThis.tulip_udsp_set === 'function' ? globalThis.tulip_udsp_set(UTF8ToString($0), $1, $2) : -1;
    }, name, bus, on);
}

int user_c_dsp_bind_osc(const char *name, int osc, int on) {
    return EM_ASM_INT({
        return typeof globalThis.tulip_udsp_bind_osc === 'function' ? globalThis.tulip_udsp_bind_osc(UTF8ToString($0), $1, $2) : -1;
    }, name, osc, on);
}

int64_t user_c_dsp_calls(const char *name) {
    return (int64_t)EM_ASM_DOUBLE({
        return typeof globalThis.tulip_udsp_calls === 'function' ? globalThis.tulip_udsp_calls(UTF8ToString($0)) : -1;
    }, name);
}

int user_c_dsp_uninstall(const char *name) {
    return EM_ASM_INT({
        return typeof globalThis.tulip_udsp_uninstall === 'function' ? globalThis.tulip_udsp_uninstall(UTF8ToString($0)) : -1;
    }, name);
}

#else  // native backends (desktop libtcc, ESP32 xcc700t + elf_loader)

#include <unistd.h>
#include "amy.h"

#define USER_C_DSP_SLOTS 8
#define USER_C_DSP_MAX_OSCS 256

typedef void (*user_c_effect_fn_t)(int32_t *buf, int32_t frames, int32_t chans);
typedef void (*user_c_osc_fn_t)(int32_t *buf, int32_t frames, int32_t osc, int32_t phase_inc_q16, int32_t amp);

typedef struct {
    char name[USER_C_DSP_NAME_LEN];
    void *fn;                   // NULL = slot inactive; set last (release) on install
    void *compiler_state;       // TCCState* (desktop) / esp_elf_t* (ESP)
    uint8_t kind;               // USER_C_DSP_KIND_*
    uint32_t bus_mask;          // effect kind: which buses to process
    int64_t calls;
} user_c_dsp_slot_t;

static user_c_dsp_slot_t slots[USER_C_DSP_SLOTS];

// osc kind: AMY osc number -> slot index + 1 (0 = unmapped). Many oscs can
// map to one slot; user code keeps per-osc state indexed by the osc param.
static uint8_t osc_map[USER_C_DSP_MAX_OSCS];

// ---- helpers exported into user code (identical on all backends) ----------
// S8.23 multiply: fxmul(a, b) == a*b with 1.0 == 1<<23.
static int32_t udsp_fxmul(int32_t a, int32_t b) {
    return (int32_t)(((int64_t)a * b) >> 23);
}
// S8.23 <-> familiar 16-bit PCM (-32767..32767).
static int32_t udsp_to_int16(int32_t s) {
    s = s >> 8;
    if (s > 32767) s = 32767;
    if (s < -32767) s = -32767;
    return s;
}
static int32_t udsp_from_int16(int32_t v) {
    return v << 8;
}

typedef struct { const char *name; const void *fn; } udsp_export_t;
static const udsp_export_t udsp_exports[] = {
    { "cos_lut", (const void *)cos_lut },
    { "fxmul", (const void *)udsp_fxmul },
    { "to_int16", (const void *)udsp_to_int16 },
    { "from_int16", (const void *)udsp_from_int16 },
    { NULL, NULL }
};
// ----------------------------------------------------------------------------

// Runs on the AMY render task, once per bus per block. buf is the bus'
// S8.23 buffer, passed to user effects directly.
void tulip_bus_postprocess_hook(uint8_t bus, int32_t *buf, uint16_t len) {
    for (int i = 0; i < USER_C_DSP_SLOTS; i++) {
        user_c_effect_fn_t fn = (user_c_effect_fn_t)__atomic_load_n(&slots[i].fn, __ATOMIC_ACQUIRE);
        if (fn != NULL && slots[i].kind == USER_C_DSP_KIND_EFFECT && (slots[i].bus_mask & (1u << bus))) {
            fn(buf, len, AMY_NCHANS);
            slots[i].calls++;
        }
    }
}

// Runs on the AMY render task for every audible osc, after AMY renders it.
// If the osc is bound to a user oscillator, replace the waveform in buf
// (mono, len frames) and return 0 so AMY still applies pan and mixes it.
uint8_t tulip_user_render_hook(uint16_t osc, int32_t *buf, uint16_t len) {
    if (osc >= USER_C_DSP_MAX_OSCS || osc_map[osc] == 0) return 0;
    user_c_dsp_slot_t *slot = &slots[osc_map[osc] - 1];
    user_c_osc_fn_t fn = (user_c_osc_fn_t)__atomic_load_n(&slot->fn, __ATOMIC_ACQUIRE);
    if (fn == NULL || slot->kind != USER_C_DSP_KIND_OSC) return 0;
    // Current pitch (incl. bend/portamento) as a Q16 per-sample phase step
    // (65536 == one full cycle), and the envelope level in S8.23.
    float freq = freq_of_logfreq(msynth[osc]->logfreq);
    int32_t phase_inc_q16 = (int32_t)(freq * 65536.0f / (float)AMY_SAMPLE_RATE);
    float a = msynth[osc]->amp;
    if (a < 0.0f) a = 0.0f;
    if (a > 1.0f) a = 1.0f;
    int32_t amp = (int32_t)(a * (float)(1 << 23));
    memset(buf, 0, len * sizeof(int32_t));  // replace AMY's carrier entirely
    fn(buf, len, osc, phase_inc_q16, amp);
    slot->calls++;
    return 0;
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
    if (slots[i].kind != USER_C_DSP_KIND_EFFECT) return -3;
    if (on) slots[i].bus_mask |= (1u << bus);
    else slots[i].bus_mask &= ~(1u << bus);
    return 0;
}

int user_c_dsp_bind_osc(const char *name, int osc, int on) {
    if (osc < 0 || osc >= USER_C_DSP_MAX_OSCS) return -2;
    int i = find_slot(name);
    if (i < 0) return -1;
    if (slots[i].kind != USER_C_DSP_KIND_OSC) return -3;
    osc_map[osc] = on ? (uint8_t)(i + 1) : 0;
    return 0;
}

int64_t user_c_dsp_calls(const char *name) {
    int i = find_slot(name);
    if (i < 0) return -1;
    return slots[i].calls;
}

// Per-platform compiler backends implement these.
static int backend_compile(const char *src, uint8_t kind, void **fn_out, void **state_out, char *err, int errlen);
static void backend_free(void *state);

// Drop a slot's compiled code, waiting out any in-flight render block.
static void free_slot_code(user_c_dsp_slot_t *slot) {
    if (slot->fn != NULL) {
        __atomic_store_n(&slot->fn, (void *)NULL, __ATOMIC_RELEASE);
        usleep(3 * 1000 * AMY_BLOCK_SIZE / AMY_SAMPLE_RATE * 1000);  // ~3 block times
    }
    if (slot->compiler_state != NULL) {
        backend_free(slot->compiler_state);
        slot->compiler_state = NULL;
    }
}

int user_c_dsp_install(const char *name, const char *src, uint8_t kind, char *err, int errlen) {
    err[0] = 0;
    if (name[0] == 0 || strlen(name) >= USER_C_DSP_NAME_LEN) {
        snprintf(err, errlen, "name must be 1..%d chars", USER_C_DSP_NAME_LEN - 1);
        return -1;
    }
    int i = find_slot(name);
    if (i >= 0 && slots[i].kind != kind) {
        snprintf(err, errlen, "'%s' is already installed as a different kind", name);
        return -1;
    }
    if (i < 0) {
        for (int j = 0; j < USER_C_DSP_SLOTS; j++) {
            if (slots[j].name[0] == 0) { i = j; break; }
        }
    }
    if (i < 0) {
        snprintf(err, errlen, "all %d c_process slots in use", USER_C_DSP_SLOTS);
        return -1;
    }

    void *fn = NULL, *state = NULL;
    if (backend_compile(src, kind, &fn, &state, err, errlen) != 0) return -1;

    // Replace-in-place: keep bus enables / osc bindings so the dev loop is
    // edit + reinstall.
    free_slot_code(&slots[i]);
    snprintf(slots[i].name, USER_C_DSP_NAME_LEN, "%s", name);
    slots[i].compiler_state = state;
    slots[i].kind = kind;
    slots[i].calls = 0;
    __atomic_store_n(&slots[i].fn, fn, __ATOMIC_RELEASE);
    return i;
}

int user_c_dsp_uninstall(const char *name) {
    int i = find_slot(name);
    if (i < 0) return -1;
    slots[i].bus_mask = 0;
    for (int o = 0; o < USER_C_DSP_MAX_OSCS; o++) {
        if (osc_map[o] == i + 1) osc_map[o] = 0;
    }
    free_slot_code(&slots[i]);
    slots[i].name[0] = 0;
    slots[i].calls = 0;
    return 0;
}

#if defined(TULIP_DESKTOP)
// ---------------------------------------------------------------- desktop:
// libtcc in-memory JIT (full C available to user code).

#include <libgen.h>
#include <mach-o/dyld.h>
#include <libtcc.h>

// Bare bodies get the signature, standard includes, and the exported-helper
// prototypes; #line 1 keeps tcc's error messages pointing at the user's own
// lines. Statics declared inside the body persist between calls.
static char *wrap_source(const char *src, uint8_t kind) {
    if (!is_bare_body(src, kind)) return NULL;  // full program: use as-is
    const char *prelude = "#include <stdint.h>\n#include <stdio.h>\n#include <math.h>\n";
    size_t n = strlen(prelude) + strlen(udsp_helper_protos) + strlen(kind_signature(kind)) + strlen(src) + 64;
    char *out = malloc(n);
    if (out == NULL) return NULL;
    snprintf(out, n, "%s%s\n%s {\n#line 1 \"%s\"\n%s\n}\n",
             prelude, udsp_helper_protos, kind_signature(kind), kind_symbol(kind), src);
    return out;
}

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

static int backend_compile(const char *src, uint8_t kind, void **fn_out, void **state_out, char *err, int errlen) {
    char *wrapped = wrap_source(src, kind);
    tcc_err_sink_t sink = { err, errlen };
    TCCState *s = tcc_new();
    if (s == NULL) {
        free(wrapped);
        snprintf(err, errlen, "tcc_new failed");
        return -1;
    }
    tcc_set_error_func(s, &sink, tcc_err_cb);
    tcc_set_lib_path(s, tcc_runtime_path());
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    for (const udsp_export_t *e = udsp_exports; e->name != NULL; e++)
        tcc_add_symbol(s, e->name, e->fn);
    int compiled = tcc_compile_string(s, wrapped ? wrapped : src);
    free(wrapped);
    if (compiled == -1) {
        if (err[0] == 0) snprintf(err, errlen, "compile failed");
        tcc_delete(s);
        return -1;
    }
    if (tcc_relocate(s) < 0) {
        if (err[0] == 0) snprintf(err, errlen, "relocate failed");
        tcc_delete(s);
        return -1;
    }
    void *fn = tcc_get_symbol(s, kind_symbol(kind));
    if (fn == NULL) {
        snprintf(err, errlen, "no %s defined", kind_signature(kind));
        tcc_delete(s);
        return -1;
    }
    *fn_out = fn;
    *state_out = s;
    return 0;
}

static void backend_free(void *state) {
    tcc_delete((TCCState *)state);
}

#elif defined(ESP_PLATFORM)
// ---------------------------------------------------------------- ESP32-S3:
// xcc700t (vendored mini C compiler, small-C subset: while/if, int + int16_t
// pointers/arrays, enum, static; no preprocessor/floats/structs) emits an
// Xtensa REL ELF that esp-idf's elf_loader relocates into PSRAM. e_entry is
// the user function, so elf->entry is directly callable. Calls to the
// exported helpers resolve through the loader's registered symbol table.

// esp_elf.h lives in a managed component whose include path isn't visible to
// micropython's qstr preprocess pass; that pass defines NO_QSTR and only needs
// preprocessing (not types) to succeed.
#ifndef NO_QSTR
#include "esp_elf.h"
#endif
#include "xcc700t.h"

// Bare bodies get the helper prototypes + signature (xcc700 has no
// preprocessor). Keeping the body on the same line as the opening brace
// preserves the user's error line numbers.
static char *wrap_source(const char *src, uint8_t kind) {
    if (!is_bare_body(src, kind)) return NULL;
    size_t n = strlen(udsp_helper_protos) + strlen(kind_signature(kind)) + strlen(src) + 8;
    char *out = malloc(n);
    if (out == NULL) return NULL;
    snprintf(out, n, "%s%s { %s\n}\n", udsp_helper_protos, kind_signature(kind), src);
    return out;
}

static int backend_compile(const char *src, uint8_t kind, void **fn_out, void **state_out, char *err, int errlen) {
    static int symbols_registered = 0;
    if (!symbols_registered) {
        static struct esp_elfsym udsp_elfsyms[8];
        int n = 0;
        for (const udsp_export_t *e = udsp_exports; e->name != NULL; e++) {
            udsp_elfsyms[n].name = e->name;
            udsp_elfsyms[n].sym = e->fn;
            n++;
        }
        udsp_elfsyms[n].name = NULL;
        udsp_elfsyms[n].sym = NULL;
        esp_elf_register_symbol(udsp_elfsyms);
        symbols_registered = 1;
    }
    char *wrapped = wrap_source(src, kind);
    uint8_t *elf_img = NULL;
    uint32_t elf_size = 0;
    int r = xcc700_compile(wrapped ? wrapped : src, kind_symbol(kind), &elf_img, &elf_size, err, errlen);
    free(wrapped);
    if (r != 0) return -1;
    esp_elf_t *elf = calloc(1, sizeof(esp_elf_t));
    if (elf == NULL) {
        free(elf_img);
        snprintf(err, errlen, "out of memory");
        return -1;
    }
    if (esp_elf_init(elf) != 0) {
        free(elf); free(elf_img);
        snprintf(err, errlen, "elf init failed");
        return -1;
    }
    r = esp_elf_relocate(elf, elf_img);
    free(elf_img);  // the loader copied the segments out
    if (r != 0) {
        esp_elf_deinit(elf); free(elf);
        snprintf(err, errlen, "elf relocate failed (%d) -- unknown function called?", r);
        return -1;
    }
    *fn_out = (void *)elf->entry;
    *state_out = elf;
    return 0;
}

static void backend_free(void *state) {
    esp_elf_t *elf = (esp_elf_t *)state;
    esp_elf_deinit(elf);
    free(elf);
}

#else
// ---------------------------------------------------------------- fallback:

static int backend_compile(const char *src, uint8_t kind, void **fn_out, void **state_out, char *err, int errlen) {
    (void)src; (void)kind; (void)fn_out; (void)state_out;
    snprintf(err, errlen, "no C compiler backend on this platform yet");
    return -1;
}

static void backend_free(void *state) {
    (void)state;
}

#endif

#endif  // __EMSCRIPTEN__ (native backends)

#endif  // TULIP_USER_C_DSP
