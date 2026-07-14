# Root-cause analysis (read-only; no code changed)

Single root cause explains every observed failure on Windows, including the
"path handling" symptoms: **the Windows build runs with an uninitialized
MicroPython pystack, so no Python code has ever executed successfully in it.**

## The chain

1. The pinned micropython (`82e69df3`) enables the pystack **only in the
   windows port**: `ports/windows/mpconfigport.h:53` has
   `#define MICROPY_ENABLE_PYSTACK (1)`. The unix port (used by the
   mac/linux VCV builds) leaves it at the default 0 — which is why only
   Windows fails.
2. `tulip/vcvrack/win/mpconfigport.h` includes that windows port header and
   does not `#undef` it.
3. In a normal windows-port build, `ports/unix/main.c:524-527` allocates the
   pool: `static mp_obj_t pystack[1024]; mp_pystack_init(...)`. But the VCV
   embed host `tulip/vcvrack/src/mp_embed.c` replaces main.c and only calls
   `mp_cstack_init_with_sp_here()` (line 174-175) — it **never calls
   `mp_pystack_init()`**. With `MICROPY_ENABLE_PYSTACK` on and an empty pool,
   the VM's very first bytecode frame allocation raises
   `RuntimeError: pystack exhausted`.
4. Therefore `pyexec_frozen_module("_boot.py")` (mp_embed.c:209) dies
   immediately — that is the lone `pystack exhausted` printed **before** any
   `==` line in both binaries. `_boot.py` is what creates
   `%USERPROFILE%/Documents/AMYboard/...`; since it never ran, **no mkdir was
   ever attempted**. The tree's absence is not a path-separator bug.
5. Every subsequent symptom is the same failure repeating:
   - juno silent + `synth 1 not defined`: the synth setup exec died.
   - `PY cv_in` / `PY encoder` lines absent, `cv_out[1]=0`, framebuf gen 0,
     restart_sketch silent: each exec batch entry died (the 4+1 pystack
     errors).
   - sysex `OSError: ENOENT` + "control frame raised": the zT header hook
     opens the sketch file under the never-created tree.
   - `Unrecognized transfer-level command 'dGVwKTo...'`: the header frame
     aborted before initializing transfer state, so the next frame's base64
     payload fell through to the command parser.
   - `sketch.py NOT FOUND`, knobs line absent, `zD dump: 1 data frame(s)`:
     all downstream of no sketch on disk.
   - "boot + default sketch AUDIBLE" passing is consistent: that audio comes
     from the C side (AMY), which is healthy.

## Notes for the fix loop (mac side)

- Two one-line candidate fixes, either sufficient:
  a) in `mp_thread_body()` (mp_embed.c, next to the cstack init):
     `#if MICROPY_ENABLE_PYSTACK static mp_obj_t pystack[1024];
     mp_pystack_init(pystack, &pystack[1024]); #endif` — matches upstream
     ports/unix/main.c:524, works for all platforms; or
  b) `#undef MICROPY_ENABLE_PYSTACK` / `#define MICROPY_ENABLE_PYSTACK (0)`
     in `tulip/vcvrack/win/mpconfigport.h`, matching the mac/linux builds'
     behavior exactly.
- The doc's first suspect (POSIX-vs-Windows path separators in the VFS) has
  **no supporting evidence yet** — the mixed-separator string in the sysex
  output is the smoke test's own printf (getenv("USERPROFILE") + a
  forward-slash suffix), and Windows CRT accepts forward slashes anyway. It
  may still surface as a second bug once Python actually runs, but currently
  everything traces to the pystack.
- Prediction: with the pystack fixed, the first re-run either goes fully
  green or fails at a genuinely new place (most likely the first real
  filesystem operation in _boot.py). Either result is informative.
