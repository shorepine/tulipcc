# AMYboard hardware-in-the-loop (HW CI)

`hwci.py` runs a real AMYboard through a full firmware test: **flash → reboot →
drive over USB-MIDI + AMY `zP` sysex → record the analog audio out → spectral
compare to a committed reference** (the hardware analogue of `amy/test.py`).
Exit code 0 = pass, 1 = fail.

Validated on a **Raspberry Pi 5 / Debian 13** (the intended self-hosted CI
runner); also runs on macOS for dev.

## Bench wiring (validated)
- **Flashing — USB-serial dongle on the AMYboard debug header.** A Pi's GPIO
  UART can't drive the DTR/RTS lines esptool needs for auto-reset, so flashing
  goes through a cheap USB-serial dongle (tested: **CH9102F**, `1a86:55d4`). The
  dongle's DTR/RTS hit the board's auto-reset circuit (DTR→IO0, RTS→EN), so
  esptool enters the ROM bootloader and flashes **over the dongle UART** —
  robust, and recovers a bricked board. (Flashing over the board's *own* native
  USB is unreliable: the stub upload fails and the device re-enumerates mid-reset.)

  | AMYboard debug header | CH9102F dongle |
  |---|---|
  | TX | RXD |
  | RX | TXD |
  | DTR | DTR |
  | RTS | RTS |
  | GND | GND |

  (TX/RX cross; DTR/RTS straight; do **not** connect the dongle's VCC — the board
  is powered over its own USB.)
- **Audio** — AMYboard analog out → a USB audio interface input (tested: C-Media
  `0d8c:0014`), recorded with `arecord`.
- **MIDI** — the AMYboard's own USB (MIDI + CDC) is plugged into the Pi; it
  enumerates as ALSA MIDI `AMYboard`.

## Install (Pi, no root)
```
python3 -m venv ~/hwci-venv
~/hwci-venv/bin/pip install esptool numpy        # ALSA tools (amidi/arecord) are already present
```
macOS dev: `pip install -r requirements.txt` (adds mido/python-rtmidi/sounddevice).

## Use
```
python3 hwci.py --list-devices

# capture the golden reference once (then LISTEN to *-recording.wav to confirm):
python3 hwci.py --pr 993 --port /dev/ttyACM1 --update-reference

# pass/fail run (full loop: flash + drive + record + compare):
python3 hwci.py --pr 993 --port /dev/ttyACM1
```
- `--port` is the **dongle** serial port. MIDI (`--midi-port`, auto-detected) and
  audio (`--audio-device`, default `hw:1,0`) default to the validated bench.
- Firmware comes from the per-PR preview (`amyboard-pr-<N>.vercel.app/firmware/`);
  use `--firmware <path>` / `--url`. `--no-flash` skips flashing to iterate on the
  drive/record/compare against an already-flashed board.

## The stimulus & metric
- **Stimulus** (`run_test_sequence()`, editable): a quick USB-MIDI note to prove
  the MIDI path is alive, then a **sustained 3-osc chord scheduled on the board
  via `zP`**. Host-timed MIDI arpeggios are too jittery to compare audio on; a
  sustained, board-scheduled tone is reproducible.
- **Metric**: averaged-magnitude-spectrum **cosine similarity** (`compare()`) —
  timing/phase-invariant, so the same notes/timbre match regardless of capture
  jitter. Pass needs `similarity ≥ --min-similarity` (default 0.90) **and**
  `level ≥ --min-level-db`. On this bench, same-firmware runs score **~0.99**
  repeatably.

## Reference: `ref/hwci_basic.wav`
Committed golden for the `hwci_basic` test. It's **bench-specific** (depends on
this board + audio interface + gain); re-capture with `--update-reference` if the
hardware changes.

## Next steps
- **CI workflow**: a self-hosted-runner job on the Pi (label/dispatch-triggered)
  that runs `hwci.py --pr <N>` and reports pass/fail.
- **Audio to the PR**: GitHub's API can't attach binaries to PR *comments*, so
  upload `*-recording.wav` as a **workflow artifact** and link it in the comment.
