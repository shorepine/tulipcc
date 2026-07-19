# AMY C API surface as used by tulipcc — survey for codegen

This documents every AMY C-API function that *external* code currently binds by
hand, across the three binding layers we want to generate automatically:

1. **CPython** — `amy/src/pyamy.c` (the `c_amy` module behind the `amy` pip package)
2. **MicroPython, linked AMY** — `tulip/shared/modtulip.c` (`tulip.*` on AMYboard
   hardware, Tulip hardware, Tulip Desktop macOS/Linux/Windows, VCV Rack)
3. **Emscripten / JS passthrough** — `tulip/web/static/spss.js` and
   `tulip/amyboardweb/static/spss.js` (`cwrap` over the WASM exports listed in
   AMY's `Makefile` `EXPORTED_FUNCTIONS`), bridged back into MicroPython with
   `mp.registerJsModule(...)` because web MicroPython does **not** link AMY
   (`AMY_IS_EXTERNAL`; the `#ifndef __EMSCRIPTEN__` sections of modtulip.c are
   compiled out).

Surveyed at amy pin `8fca573f` (v1.2.78) / tulipcc branch `claude/amy-c-api-codegen-940eec`.

## The function inventory

Legend for the platform columns: **Py** = pyamy.c, **MP** = modtulip.c,
**JS** = spss.js (both copies — they carry identical cwrap blocks today).

| AMY C function (signature) | Py | MP | JS | Python-visible name(s) today |
|---|---|---|---|---|
| `void amy_add_message(char*)` | ✅ `send_wire` | ✅ `tulip.amy_send` | ✅ cwrap `['string']` → `amy_js_message` module | `amy.override_send` routes to each |
| `void amy_add_message_from_sysex(char*)` | ✅ `send_wire_from_sysex` | ✅ inside `tulip.amy_send_sysex` (ring-buffer slot + AK ack) | ➖ (web sends sysex *to* hardware instead) | — |
| `uint32_t amy_sysclock()` | ✅ `ticks_ms` | ✅ `tulip.amy_ticks_ms` | ✅ cwrap → `amy_sysclock` module → `tulip.amy_ticks_ms`, `amy.ticks_ms` | `tulip.amy_ticks_ms()` everywhere |
| `void amy_reset_sysclock()` | ➖ | ➖ | ✅ (called from JS `runCodeBlock`) | — |
| `float amy_get_render_load()` | ✅ `render_load` | ✅ `tulip.amy_render_load` | ❌ missing on web | |
| `void amy_set_render_load_threshold(float)` | ✅ | ✅ `tulip.amy_set_render_load_threshold` | ❌ missing on web | |
| `void *yield_synth_commands(uint8_t synth, char *s, size_t len, bool include_fx, void *state)` | ✅ looped in `get_synth_commands` | ✅ looped in `tulip.amy_get_synth_commands` | ✅ looped in JS `get_synth_commands()` w/ manual `_malloc` + heap-string readback, installed as a Python lambda | three hand-written copies of the same generator loop |
| `char *amy_dump_state_to_string(int *len)` | ✅ `dump_state` | ❌ (removed — was dead) | ✅ exported, used by web tooling | |
| `int amy_get_output_buffer(int16_t*)` | ➖ | ✅ (1024-byte bytes obj) | ✅ manual heap dance → `tulip.amy_get_output_buffer` | |
| `int amy_get_input_buffer(int16_t*)` | ➖ | ✅ | 💤 exported, JS wrapper commented out | |
| `void amy_set_external_input_buffer(int16_t*)` | ➖ | ✅ (buffer-protocol arg) | 💤 exported, commented out | |
| `void amy_event_midi_message_received(uint8_t*, uint32_t, uint8_t, uint32_t)` | ✅ `inject_midi` | ➖ (hardware has real MIDI) | ➖ | also used directly by VCV `vcv_midi.c` |
| `void convert_midi_bytes_to_messages(uint8_t*, size_t, uint8_t)` | ✅ `inject_midi_bytes` | ➖ | ➖ | |
| `void amy_process_single_midi_byte(uint8_t, uint8_t)` | ➖ | ➖ | ✅ (WebMIDI input feed) | |
| `void set_cv_from_osc(int, int)` | ✅ | ➖ | ➖ | |
| `int16_t *amy_simple_fill_buffer()` | ✅ `render_to_list` | ➖ | ➖ | |
| `void amy_start(amy_config_t)` / `amy_default_config()` | ✅ `live`/`start` (kwarg→config parser) | ➖ (called from `amy_connector.c` `run_amy`, not bound to Python) | via `amy_start_web`/`amy_start_web_no_synths` exports | |
| `void amy_stop()` | ✅ `stop` | ➖ | via `amy_live_stop` | |
| `void amy_live_start_web()` / `amy_live_start_web_audioin()` / `amy_live_stop()` | ➖ | ➖ | ✅ (async cwrap, audio-worklet start) | |
| `void amy_bleep(uint32_t)` | ➖ | ➖ | ✅ | |
| `uint32_t sequencer_ticks()` | ➖ | ➖ (linked builds read `amy_global.sequencer_tick_count` directly in `tulip.seq_ticks`) | ✅ cwrap (web `tulip.seq_ticks` reads a JS-maintained `sequencer_tick_count` mirror) | `tulip.seq_ticks()` |
| `void amy_set_external_hook_context(void*)` / `amy_get_external_hook_context()` | ➖ | ➖ | exported for the JS worklet plumbing | |
| `int size_of_amy_event()` | ➖ | ➖ | exported | |
| Global-variable pokes: `external_map[osc]=ch`, `cv_synth_map[synth]=cv` | ➖ | ✅ `tulip.amy_set_external_channel`, `tulip.set_cv_synth` (ESP + VCV only) | ➖ | not functions at all — need C setters before they can be generated |

Not in scope for codegen (different beast): the `amy_config_t` hook wiring in
`tulip/shared/amy_connector.c` (`run_amy()` — render/fopen/exec/reboot/overload
hooks) and pyamy.c's `live()` kwarg→`amy_config_t` parser. These are per-platform
*setup*, not per-function boilerplate, though the config-kwarg parser could be
generated from a field map later.

## Where the three platforms drift today (the bug motivation)

- `amy_render_load` / `set_render_load_threshold`: CPython + MP, **missing on web**.
- `dump_state`: CPython + web, **not exposed to MicroPython**.
- `get_synth_commands`: the generator loop is hand-written **three times** with
  three different buffer strategies (C stack buffer ×2, JS heap malloc).
- `amy_reset_sysclock`: web-only, never exposed to Python anywhere.
- Input-buffer functions: exported to WASM but the JS side has rotted into
  comments.
- Naming: `ticks_ms` (Py) vs `tulip.amy_ticks_ms` (MP) vs `amy.ticks_ms` (web
  monkey-patch in start_amyboard, but *not* in tulip web's start_tulip).

## Calling conventions actually in use

Every binding today falls into one of six shapes — this is the whole type
vocabulary a generator needs:

1. **`(void) -> int/float`** — `amy_sysclock`, `sequencer_ticks`,
   `amy_get_render_load`, `size_of_amy_event`.
2. **`(scalar…) -> void`** — `amy_set_render_load_threshold(float)`,
   `set_cv_from_osc(int,int)`, `amy_bleep(uint32)`,
   `amy_process_single_midi_byte(u8,u8)`.
3. **`(cstring) -> void`** — `amy_add_message`, `amy_add_message_from_sysex`.
   JS: cwrap `['string']` handles the marshalling; Py: `PyArg_ParseTuple("s")`;
   MP: `mp_obj_str_get_str`.
4. **byte-buffer in** — `convert_midi_bytes_to_messages`,
   `amy_event_midi_message_received`, `amy_set_external_input_buffer`.
   Py: sequence→malloc'd `uint8_t*`; MP: buffer protocol; JS: heap `_malloc`+`set`.
5. **byte-buffer out** — `amy_get_output_buffer` / `amy_get_input_buffer`
   (caller-allocated 1024B, returns count), `amy_simple_fill_buffer`
   (returns pointer to internal block), `amy_dump_state_to_string`
   (mallocs, out-param length, caller frees).
6. **generator / stateful iterator** — `yield_synth_commands`: loop with opaque
   `void* state`, string chunk per call, accumulate to a list. The only one
   with control flow, and it's identical logic in all three ports.

Plus one non-function shape: **direct global reads/writes**
(`amy_global.sequencer_tick_count`, `external_map`, `cv_synth_map`). Codegen
can't reach through globals on the web build — first cleanup is to give these
real accessor functions in AMY.

## Proposed shape of the codegen (next step)

A single source-of-truth table, e.g. `amy/scripts/amy_api.yaml` (or a Python
dict next to the existing `scripts/gen_amy_js_api.py`, which already establishes
the "generate bindings from a table at build time" pattern in AMY — see also
`gen_amy_gd_api.py` for Godot):

```yaml
- name: amy_sysclock
  py_name: ticks_ms          # exposed as amy.ticks_ms / tulip.amy_ticks_ms
  args: []
  ret: u32
  platforms: [cpython, micropython, web]
- name: yield_synth_commands
  kind: string_generator     # special-cased template, shared by all 3
  py_name: get_synth_commands
  args: [{name: synth, type: u8}, {name: include_fx, type: bool, default: true}]
  platforms: [cpython, micropython, web]
- name: amy_get_output_buffer
  kind: bytes_out            # caller buffer, size 1024, returns n
  buf_size: 1024
  ...
```

Generators (run as build steps):

- **`gen_pyamy_api.c`** — emitted wrappers + `PyMethodDef` rows, `#include`d by a
  slimmed pyamy.c (which keeps only the irreducible hand-written parts: `live()`
  config parsing, module init).
- **`gen_modtulip_api.h`** — emitted `STATIC mp_obj_t` wrappers +
  `MP_ROM_QSTR` table fragment, `#include`d by modtulip.c inside the existing
  `#ifndef __EMSCRIPTEN__`.
- **`gen_amy_wasm_api.js`** — emitted cwrap block + `registerJsModule` calls +
  the Python monkey-patch snippet, sourced by **both** spss.js copies (today's
  duplicated blocks deleted); plus emitting the `EXPORTED_FUNCTIONS` list into
  the Makefile/em flags so an added function can never be forgotten in the
  export list again.

End state: `amy.<py_name>(...)` (and the `tulip.amy_*` aliases) exist and behave
identically on CPython, all linked-MicroPython targets, and both web apps, with
one YAML row per function.
