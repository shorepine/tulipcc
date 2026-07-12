# User C DSP plugins for AMYboard / Tulip — design & feasibility

*Status: proposal, 2026-07-11. Core unknowns validated by experiment (see §3).
Since shipped on all platforms — if you want to **use** this rather than read
about how it's built, see the user guide: [Writing audio effects and
oscillators in C](user_c_dsp.md).*

The idea: let people write a tiny C function — a bit-crusher, a weird oscillator —
as a **string in Python**, compile it **on the device itself**, and inject it into
AMY's render loop at audio rate. No toolchain, no reflash, no wire-protocol
extension for every effect anyone dreams up.

```python
crusher = """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 18) << 18;
        i = i + 1;
    }
"""
tulip.install_c_process("crush", crusher)   # compiles + loads, ~ms
tulip.c_process("crush", True)              # insert at end of bus 0 FX chain
```

User code runs **directly on AMY's S8.23 buffers** (int32, 1.0 == `1<<23`),
`chans` sequential channel blocks of `frames` samples — zero copies. Exported
helpers make the fixed point approachable (available in every snippet):
`cos_lut(phase_q23)` (AMY's cosine table, phase normalized to one cycle in
S8.23), `fxmul(a, b)` (S8.23 multiply), and `to_int16(s)` / `from_int16(v)`
for people who prefer thinking in 16-bit PCM.

**Verdict: very feasible on ESP32-S3, trivial on desktop, and plausible on web.**
Most of the machinery already exists; the genuinely new work is small.

---

## 1. What already exists

### AMY hook points (at current pin `d1f5d18`)

- **Oscillators** — `amy_config.amy_external_render_hook(uint16_t osc, SAMPLE *buf, uint16_t len)`
  is called for **every audible osc** right after AMY renders it
  ([amy.c:1804]). Return nonzero = "I handled/replaced this osc's audio, skip
  AMY's mix". tulipcc **already chains this** for CV output via `external_map`
  in `tulip/shared/amy_connector.c:427` (`external_cv_render`) — user
  oscillators are one more dispatch case in that function, zero AMY changes.
  (AMY also has the compile-time `CUSTOM` wave type + `amy_set_custom()` 7-function
  struct — see `examples/AMY_custom_osc` — but the render hook is the runtime path.)

- **Effects** — there is currently **no hook at the end of the per-bus FX
  chain**. `amy_fill_buffer()` runs EQ → chorus → echo → reverb per bus on
  `fbl[0][bus]` (SAMPLE = int32 S8.23 fixed point, `AMY_BLOCK_SIZE` frames per
  channel, channels sequential not interleaved), then mixes buses → volume →
  soft-clip → int16 `output_block`, and on ESP AMY itself writes that to I2S
  (`src/i2s.c: esp_fill_audio_buffer_task`). So the effect insertion point is a
  **new, small AMY hook** (§4). The recent `examples/AMY_custom_dsp` bit-reduction
  example is a different trick (audio-in → user buffer → `AUDIO_EXT0/1` waves) —
  useful, but not a bus insert.

### Integer audio is a feature here

AMY's internal sample type is **int32** (S8.23) and the tiny on-device compiler
(below) supports **only integer math — no floats** — so the whole pipeline stays
integer, which is also what you want on the S3 anyway. User code gets the raw
S8.23 buffers (zero-copy, full headroom — bus values can exceed ±1.0 pre-clip),
with exported helper functions (`cos_lut`, `fxmul`, `to_int16`/`from_int16`)
resolved by each backend's symbol mechanism so nobody has to *derive* fixed
point — they call helpers. (The xcc700 fork still supports `int16_t`
pointers/arrays for effects that want to store 16-bit state compactly.)

## 2. The on-device compiler + loader (ESP32-S3)

Two off-the-shelf pieces, apparently made for this exact plan:

- **[xcc700](https://github.com/valdanylchuk/xcc700)** (MIT, one 700-line .c
  file): mini C compiler emitting **Xtensa REL ELF**, runs on Mac/Linux *and on
  the ESP32-S3 itself* (self-hosted: ~3,900 lines/sec on S3 — a user effect
  compiles in ~10 ms). Subset: `while`, `if/else`, `int/char/pointers/arrays`,
  `enum` (the `#define` substitute), function defs/calls, arithmetic + bitwise
  ops, `.bss` globals (user state/delay lines). Missing: `for`, floats,
  structs, preprocessor, initialized globals. Enough for real DSP in the
  integer domain. There's also **[rcc700](https://github.com/valdanylchuk/rcc700)**
  for ESP32 RISC-V parts (tested on P4) if we ever move architectures.

- **[espressif/elf_loader](https://components.espressif.com/components/espressif/elf_loader)**
  (IDF component, source in
  [esp-iot-solution](https://github.com/espressif/esp-iot-solution/tree/master/components/elf_loader)):
  loads + relocates ELF at runtime on ESP32-S3, **supports running loaded code
  from PSRAM** (we have 8 MB; no IRAM pressure), and resolves the ELF's imports
  against a **firmware-exported symbol table** (`symbols.py` generates it; we
  can export whatever AMY/tulip helpers we want users to call — they just
  declare them, xcc700-style: `int amy_osc_freq_q16(int osc);`). Also has
  `dlopen`/`dlsym`-style API for grabbing `process` by name.

xcc700's README explicitly states its output "can be run directly by the
ESP-IDF elf_loader component, which links them on load via relocation table to
anything you have exposed in your firmware."

## 3. Validated by experiment (today, on this Mac)

Built xcc700 with clang, compiled the bit-crusher from the top of this doc:

```
> IN  : 14 Lines / 52 Tokens
> SYM : 2 Funcs / 1 Globals
> OUT : 139 B .text / 608 B ELF
```

`xtensa-esp32s3-elf-objdump -d` shows correct windowed-ABI Xtensa
(`entry a1,80` … `ssr/sra` + `ssl/sll` for the shifts … `retw.n`), and the
symbol table exports `process` (`00000034 g F .text process`) for `dlsym`.

**Cost estimate**: the codegen is a naive stack machine, ~40 instructions per
sample for the crusher loop. At 44.1 kHz stereo, 256-frame blocks:
~20k instr / 5.8 ms block ≈ **<2% of one 240 MHz core**. Even a 10×-heavier
user effect fits comfortably; and the compiler is 700 lines — forkable if we
ever want peephole opts.

## 4. AMY changes required (one small PR)

Following the existing `amy_external_*_hook` pattern in `amy_config_t`
(amy.h:726):

```c
// Called at the end of each bus's FX chain (after EQ/chorus/echo/reverb),
// before buses are mixed to output. buf is AMY_NCHANS sequential channel
// blocks of len SAMPLEs (S8.23).
void (*amy_external_bus_postprocess_hook)(uint8_t bus, SAMPLE *buf, uint16_t len);
```

One call site in `amy_fill_buffer()` at the end of the per-bus FX loop.
Optionally a second, later hook on the final int16 interleaved `output_block`
(post volume/clip, matches "process the master out"), but the per-bus SAMPLE
hook is the one that composes with AMY's routing ("effects into a bus") and
matches the int-only compiler subset. The oscillator side needs **no AMY
change** (render hook exists).

On/off + mix control: start tulip-side (`tulip.c_process(name, bus, on)` just
toggles dispatch in `amy_connector.c`). If we later want it sequencable /
wire-addressable, add a per-bus "user effect level" param to AMY — but that's
v2; don't block on wire-protocol design.

## 5. Tulip integration (`tulip/shared/amy_connector.c` + a new module)

- Vendor `xcc700.c` (MIT) into `tulip/shared/3rdparty/` adapted per its
  "option D" (call as a function, in-memory output instead of fd — small patch).
- Add `elf_loader` to `tulip/amyboard` (and esp32s3) `idf_component.yml`;
  generate a minimal symbol table exporting our chosen helper API.
- `tulip.install_c_process(name, src, kind="effect"|"osc")`:
  compile → load ELF into PSRAM → `dlsym("process"/"render")` → store in a
  slot table. Errors from xcc700 raise Python exceptions with line numbers.
- Effect dispatch: tulip's `bus_dsp` callback (registered as the new AMY hook)
  walks enabled slots per bus. Oscillator dispatch: extend `external_cv_render`'s
  `external_map` with user-osc entries; expose msynth params to user code as
  fixed-point via exported helpers (`amy_osc_freq_q16(osc)`,
  `amy_osc_amp_q23(osc)`, `amy_osc_phase(osc)`…) since the subset has no floats.
- **Desktop (macOS Tulip)** — *implemented*: same Python API, backed by an
  embedded **libtcc** (TinyCC, `tinycc` submodule, LGPL) JIT — no Xcode CLT
  needed. `tulip/shared/user_c_dsp.c` holds the slot table + dispatcher
  (registered as `amy_external_bus_postprocess_hook`) and the tcc backend;
  `tulip/macos/build.sh` builds `libtcc.a` and stages the tcc runtime
  (`libtcc1.a` + headers) into the app bundle's `Resources/tcc`. Compile
  errors raise `ValueError` with tcc's line numbers; reinstalling the same
  name swaps code in place and keeps its bus enables (edit + reinstall dev
  loop); `tulip.c_process_calls(name)` confirms the code is running.
  Caveats: tcc accepts more C than xcc700 will on hardware (add an xcc700
  lint pass later); the signed/notarized `package.sh` build needs the
  `com.apple.security.cs.allow-jit` hardened-runtime entitlement and per-arch
  libtcc builds before the feature ships in releases (until then it compiles
  out of that build).

  ```python
  tulip.install_c_process("crush", src)   # effect -> slot; ValueError on bad C
  tulip.c_process("crush", True, 0)       # enable on bus 0 (on, bus=0)
  tulip.install_c_osc("cz", osc_src)      # oscillator kind
  tulip.c_osc("cz", 20)                   # AMY osc 20 now plays your C code
  tulip.c_process_calls("crush")          # blocks processed so far
  tulip.uninstall_c_process("crush")
  ```

  `src` can be a **full program or just the function body** — bodies are
  wrapped in the signature + standard includes automatically (with `#line`
  so tcc errors keep the user's line numbers). Effect body sees
  `buf/frames/chans`; osc body sees `buf/frames/osc/phase_inc_q16/amp_q15`
  (mono buf; the dispatcher derives phase step + envelope from `msynth` so
  pitch bend and ADSR just work, and AMY still applies pan). **State between
  calls** = `static` variables in the body; polyphonic per-osc state =
  static arrays indexed by `osc`. See the shipped example
  `tulip/fs/tulip/ex/c_dsp_demo.py` (bitcrusher + CZ-101-style phase
  distortion osc).

### Failure modes, honestly

This is native code in the audio task. A wild pointer = panic + reboot, same as
any firmware bug; xcc700's error handling is "wildly optimistic." Mitigations
worth doing: cap `.text`/`.bss` sizes; reject ELFs importing symbols outside
our exported table (the loader does this inherently — unresolved symbol =
clean load failure); dry-run `process()` once on a scratch buffer at install
time (catches the dumbest crashes before it's in the render path); document
"you can crash your synth, that's the fun part."

## 6. Web

**Shipped.** As built it differs from the original sketch in the injection
mechanism (no structured-cloning through the worklet port, no `addFunction`):

- **The compiler**: `xcc700w.c`, the wasm backend of the vendored xcc700 fork
  (same small-C front end as the Xtensa `xcc700t`). It runs inside
  micropython's wasm module on the page and emits a complete standalone wasm
  module: linear memory imported (`env.memory`), statics/rodata at imported
  base globals the host allocates from AMY's heap, undeclared functions as
  `env.*` imports, `fxmul`/`to_int16`/`from_int16` inlined as intrinsics.
- **The injection**: AMY's render loop runs in an AudioWorklet whose scope
  only ever evaluates `amy.js`, and the page can't inject code there — so the
  web builds *append* `tulip/shared/user_c_dsp_web.js` to the copied `amy.js`
  and it runs in both scopes. AMY's js hooks (`amy_render_js_hook`,
  `amy_bus_postprocess_js_hook`, called per block on the worklet thread) pass
  their scope's `Module`, and the page and worklet rendezvous on a shared
  control block in AMY's linear memory (a SharedArrayBuffer) registered via
  AMY's `amy_set_external_hook_context()`. Installs mail the module bytes
  through per-slot mailboxes in that block; the worklet instantiates them
  importing AMY's `wasmMemory` + exports (`cos_lut` stays wasm-to-wasm) and
  calls them from the hooks. Bus masks / osc bindings / call counts live in
  the block too, so enable/bind/calls need no messaging at all.
- Same Python/JS API on top; same int32 SAMPLE buffers, zero-copy.

## 7. Staged plan

1. **AMY PR**: add `amy_external_bus_postprocess_hook` (+ call site). Tiny; follows
   house pattern. (Standard AMY PR flow: amy PR → `make test` → pin in tulipcc →
   merge → re-pin.)
2. **ESP spike** (amyboard branch): vendor xcc700, add elf_loader component +
   symbol table, `tulip.install_c_process()` + effect dispatch, bench-test the
   bit-crusher on hardware.
3. **Oscillator kind**: user-osc dispatch in `external_cv_render` + fixed-point
   param helpers; naive-square demo.
4. **Desktop parity** via clang/dlopen.
5. **Web**: wasm backend fork of xcc700 (or server-compile stopgap) + worklet
   injection; Makefile flag PR in amy.
6. Docs + a couple of shipped examples (crusher, sample-rate reducer, ring mod,
   chip-tune osc) — these double as the test corpus.
