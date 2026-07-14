# Real Windows 11 run — RESULT: FAIL (user-executed, 2026-07-14)

Both binaries ran to completion in seconds, exit code 0, no hang, no crash —
but the firmware never created its filesystem and most checks failed.

- Windows version (`ver`): `Microsoft Windows [Version 10.0.26200.8246]` (Windows 11 Pro)
- Run under cmd.exe: `cmd /c ".\x.exe > out.txt 2>&1"`
- Environment note: the shell's "My Documents" is redirected to OneDrive
  (`[Environment]::GetFolderPath('MyDocuments')` → `C:\Users\brian\OneDrive\Documents`),
  but a real, plain `C:\Users\brian\Documents` also exists (empty). The firmware
  targeted the latter (see mixed-separator path below) and still created nothing.

## board_smoke.exe — exit code 0 — board_out.txt verbatim

```
RuntimeError: pystack exhausted
== starting AMY + firmware
== booted after ~3 ms of rendering
== boot + default sketch    peak=  3039 rms=   618.6 AUDIBLE
== juno note on ch1         peak=     0 rms=     0.0 (silent)
RuntimeError: pystack exhausted
RuntimeError: pystack exhausted
RuntimeError: pystack exhausted
RuntimeError: pystack exhausted
== after exec batch         peak=     0 rms=     0.0 (silent)
RuntimeError: pystack exhausted
== cv_out[1] = 0.000000 (expect -4.5)
== framebuf generation = 0 (expect > 0)
== after restart_sketch     peak=     0 rms=     0.0 (silent)
== stopping
== clean exit
synth 1 not defined (get_flags)
synth 1 not defined (voice_for_event)
synth 1 not defined (noteon_delay)
synth 1 not defined (get_flags)
synth 1 not defined (voice_for_event)
synth 1 not defined (noteon_delay)
synth 1 not defined (get_flags)
synth 1 not defined (voice_for_event)
synth 1 not defined (noteon_delay)
synth 1 not defined (get_flags)
synth 1 not defined (voice_for_event)
synth 1 not defined (noteon_delay)
```

Versus expected: boot + default sketch is AUDIBLE (rms 618.6), but juno note is
silent, the expected `PY cv_in -> 3.25` / `PY encoder -> 7` lines never appear
at all, `cv_out[1]` is 0.0 instead of -4.5, and framebuf generation is 0
instead of 2. Every Python exec appears to die with
`RuntimeError: pystack exhausted` (6 occurrences, including one before any
`==` line).

## sysex_smoke.exe — exit code 0 — sysex_out.txt verbatim

```
RuntimeError: pystack exhausted
OSError: [Errno 2] ENOENT
RuntimeError: pystack exhausted
RuntimeError: pystack exhausted
== booted
== zT header                    -> AK (frame 7b)
== zT b64 chunk                 -> AK (frame 7b)
== zP transfer done             -> AK (frame 7b)
== sketch.py NOT FOUND at C:\Users\brian/Documents/AMYboard/user/current/sketch.py
== zA knobs                     -> AK (frame 7b)
== zD dump: 1 data frame(s), ack=1
== overall: FAIL
== clean exit
AMYboard VCV: control frame raised, ignoring
Unrecognized transfer-level command 'dGVwKToKICAgIHBhc3MKIyBWQ1ZfU1lTRVhfVEVTVF9NQVJLRVIK'
queue for patch number 0 is out of range (1024 .. 1056)
event ignored
mp_exec_hook: exec raised, ignoring
mp_update_file_hook: update_sketch_knobs raised, ignoring
```

Versus expected: all frames do get `-> AK`, but sketch.py was never written to
disk, the knobs block check never printed PRESENT/ABSENT (line absent
entirely), zD produced 1 data frame instead of 2, and overall is FAIL instead
of PASS. The base64 payload of the sketch chunk was handed to the transfer
layer as a "command" (`Unrecognized transfer-level command 'dGVwKTo...'` —
decodes to the tail of the sketch: `...tep):\n    pass\n# VCV_SYSEX_TEST_MARKER\n`).

## Filesystem check — the AMYboard tree was NEVER created

- `dir %USERPROFILE%\Documents\AMYboard\user\current` → The system cannot find
  the path specified.
- `type ...\sketch.py` → The system cannot find the path specified.
- `C:\Users\brian\Documents` exists but contains 0 files, 0 subdirectories.
- No AMYboard folder under `C:\Users\brian\OneDrive\Documents` either; a
  recursive search of the user profile (depth 3) found no AMYboard directory
  anywhere. (`C:\Users\brian\amy` is a pre-existing AMY repo clone, untouched.)

The `OSError: [Errno 2] ENOENT` at the very top of the sysex boot, plus the
mixed-separator path `C:\Users\brian/Documents/AMYboard/...` in its own
output, point at the path-handling suspect the doc lists first: the
POSIX-style VFS root with forward slashes appears to fail at (or before)
mkdir, and everything downstream cascades from that plus the repeated
`pystack exhausted` failures on the MicroPython side.

## Cross-check vs Wine run (see REPORT.md)

The failure signature on real Windows 11 (10.0.26200) is **identical** to the
Wine 9.0 run on Linux: same pystack-exhausted count and placement, same silent
juno/exec/cv_out/framebuf checks, same identical rms (618.6 matches Wine
run 1 exactly), same ENOENT, same base64-as-command misparse, same
`1 data frame(s)` zD dump, same never-created AMYboard tree, same mixed
`\`+`/` path (only the username differs). Differences are cosmetic only:
stdout/stderr interleaving order under cmd.exe redirection groups the stderr
lines (`synth 1 not defined`, hook messages) at the end instead of inline.
Conclusion: the bugs are real firmware behavior, not emulation artifacts, and
they are deterministic across two independent machines/OSes.
