# AMYboard VCV — Windows kernel smoke test protocol

You are testing two statically linked Windows x64 console binaries that
contain the full AMYboard firmware (MicroPython + frozen firmware + the AMY
synth engine), cross-compiled with MinGW. No DLLs, no install, no audio
device needed (they render into buffers and measure RMS).

## Steps

1. From this directory (`tulip/vcvrack/test/`), in cmd or PowerShell:

       .\board_smoke.exe > board_out.txt 2>&1
       type board_out.txt

2. Then:

       .\sysex_smoke.exe > sysex_out.txt 2>&1
       type sysex_out.txt

3. Check the filesystem the firmware created:

       dir %USERPROFILE%\Documents\AMYboard\user\current
       type %USERPROFILE%\Documents\AMYboard\user\current\sketch.py

## Expected (matches macOS/Linux runs)

board_smoke.exe:
- "booted after ~N ms"
- "boot + default sketch ... AUDIBLE" (rms ~440+)
- "juno note on ch1 ... AUDIBLE" (rms ~630)
- "PY cv_in -> 3.25", "PY encoder -> 7"
- "cv_out[1] = -4.500000", "framebuf generation = 2"
- "clean exit"

sysex_smoke.exe:
- every frame "-> AK"
- "sketch.py on disk: 60 bytes, marker FOUND"
- "knobs block after zA: PRESENT"
- "zD dump: 2 data frame(s), ack=1"
- "== overall: PASS"

## If something fails

Likely suspects, in order:
- Path handling: mixed / and \ separators through the POSIX-style VFS
  (root is %USERPROFILE%/Documents/AMYboard with forward slashes). Note
  exactly which operation failed (mkdir? open? chdir?) from the output.
- A silent hang: note how far the output got before stalling, then kill it.
- Crash with no output: rerun in cmd.exe directly (not PowerShell) and
  capture the exit code with `echo %errorlevel%`.

Report VERBATIM output of both runs (the .txt files), the dir listing, the
sketch.py contents, and Windows version (`ver`). Do not attempt to fix the
C code — the fix loop runs on the mac side; your job is precise evidence.
