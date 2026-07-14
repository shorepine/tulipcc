# AMYboard VCV kernel smoke — validation evidence

Protocol: `tulip/vcvrack/test/WINDOWS_TESTING.md` on branch `claude/vcv-win-smoke`
(sparse clone; binaries at that branch's committed SHAs:
`board_smoke.exe` 13850fcb90ca3ec936bb19f19fd5bb4fd6cd5866cab765ece467e2d6857e5c3f,
`sysex_smoke.exe` 3bac0cc27e4aef34a78e3d63959fec6522208242d5b0194492f9e3fae0303c53).

## Environment caveat — NOT real Windows

The test machine available was Linux, so the PE32+ x64 binaries were run under
**Wine 9.0** (`wine-9.0 (Ubuntu 9.0~repack-4build3)`), which reports itself as
`Microsoft Windows 10.0.19043`. Host: Ubuntu 24.04.4 LTS, kernel 6.18.5 x86_64.
Wine implements the Win32 file/path APIs but is not bit-identical to real
Windows; treat path-handling evidence as strong but not final. The
`wine32 is missing` / `nodrv_CreateWindow` / explorer lines in the captures are
Wine loader noise, not program output.

## Results (deterministic across 2 runs of each binary)

Both binaries print `== clean exit` and exit with process code 0.

### board_smoke — partial FAIL
- `RuntimeError: pystack exhausted` immediately at boot, again 4x during the
  exec batch, and again on restart_sketch.
- `boot + default sketch` **AUDIBLE** (peak=3039, rms=606–618) — PASSES.
- `juno note on ch1` **silent** (rms=0.0, expected ~630), preceded by repeated
  `synth 1 not defined (get_flags / voice_for_event / noteon_delay)`.
- The `PY cv_in -> 3.25` / `PY encoder -> 7` lines never appear.
- `cv_out[1] = 0.000000 (expect -4.5)` — FAIL.
- `framebuf generation = 0 (expect > 0)` — FAIL.
- `after exec batch` and `after restart_sketch` both silent.

### sysex_smoke — overall FAIL
- `RuntimeError: pystack exhausted` at boot and inside `mp_exec_hook` /
  `mp_update_file_hook` (both "raised, ignoring").
- All four frames still ACK (`-> AK (frame 7b)`).
- `OSError: [Errno 2] ENOENT` during the zT header frame.
- b64 chunk mishandled: `Unrecognized transfer-level command
  'dGVwKToKICAgIHBhc3MKIyBWQ1ZfU1lTRVhfVEVTVF9NQVJLRVIK'` (that string is the
  tail of the sketch payload itself, base64 for `...tep):\n    pass\n#
  VCV_SYSEX_TEST_MARKER\n`), plus `queue for patch number 0 is out of range
  (1024 .. 1056)` / `event ignored`.
- `sketch.py NOT FOUND at C:\users\root/Documents/AMYboard/user/current/sketch.py`
  — note the **mixed `\` and `/` separators** in the reported path.
- `zD dump: 1 data frame(s), ack=1` (expected 2 data frames).
- `== overall: FAIL`.

### Filesystem
No `AMYboard` directory exists anywhere on disk after both runs — the
`%USERPROFILE%\Documents\AMYboard` tree was **never created** (Documents is
empty; a whole-filesystem `find -name AMYboard -type d` returns nothing). So
the requested `dir` listing and `sketch.py` contents do not exist.

## Files
- `board_out_run1.txt` / `board_out_run2.txt` — verbatim board_smoke captures
  (run1 includes one-time Wine prefix-creation noise).
- `sysex_out_run1.txt` / `sysex_out_run2.txt` — verbatim sysex_smoke captures.
