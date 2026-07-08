# Hardware-in-the-loop (HW CI): AMYboard + Tulip

Two physical boards share one Pi bench, each with its own runner script:

- **`hwci.py`** runs a real **AMYboard** through a full firmware test: **flash →
  reboot → drive over USB-MIDI + AMY `zP` sysex → record the analog audio out →
  spectral compare to a committed reference** (the hardware analogue of
  `amy/test.py`). It tails both serial consoles — the **debug UART** (ESP-IDF
  console / stderr) and the native **CDC** (MicroPython stdout) — into a combined
  `*-serial.log`.
- **`tulip_hwci.py`** does the same for a **Tulip (TULIP4_R11)**, but driven over
  the **MicroPython serial REPL** instead of MIDI (Tulip has no USB-MIDI). See the
  [Tulip](#tulip-tulip4_r11) section below.

Exit code 0 = pass, 1 = fail. Validated on a **Raspberry Pi 5 / Debian 13** (the
intended self-hosted CI runner); also runs on macOS for dev. The CI workflow that
drives both on every PR is `.github/workflows/amyboard-hwci.yml`.

Board control (the SysEx protocol, MIDI backends, AMYboard World fetch) comes
from the shared [`tools/amyboardctl`](../../../tools/amyboardctl/) library —
also usable standalone as the `amyboardctl` CLI. In-repo it's imported via a
relative path; on the CI runner the workflow extracts the package next to
`hwci.py`. The wire protocol is documented in
[`docs/amyboard/control_api.md`](../../../docs/amyboard/control_api.md).

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

## Stable device names (udev, recommended on the Pi)
`ttyACM*` numbering and ALSA card indices are assigned in probe order, so a reboot
can swap them and make the runner flash or record the wrong device. Install
[`99-amyboard.rules`](99-amyboard.rules) to pin names by USB vendor:product:
```
sudo install -m 0644 99-amyboard.rules /etc/udev/rules.d/99-amyboard.rules
sudo udevadm control --reload && sudo udevadm trigger --subsystem-match=tty
```
That gives `/dev/amyboard-dongle` (the CH9102F flashing dongle) and
`/dev/amyboard-cdc` (the board's native CDC). Audio is referenced by its stable
ALSA card id `hw:CARD=Device,DEV=0` (the C-Media capture) rather than `hw:1,0`.
The CI workflow uses all three; for manual runs pass them with `--port`,
`--cdc-port`, `--audio-device`.

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
- Each run writes `{name}-recording.wav` **and** `{name}-serial.log` (both meant
  to be attached to the PR). The log captures the debug UART (`--debug-port`,
  default `--port` else `/dev/ttyACM1`) and the native CDC (`--cdc-port`, default
  `/dev/ttyACM0`); `--no-serial-log` disables it.

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
- **Render load** (informational, doesn't gate pass/fail): while the tests run,
  `tulip.amy_render_load()` (0..1, ~1.0 = AMY can't render realtime; amy#826 /
  PR #1105) is polled once a second over `zP`. Each sample prints an
  `hwci_load <v>` line on MicroPython stdout, so the values ride the CDC
  serial-log capture; the run's mean/max are printed with the results
  (`amyboard render load: mean=… max=… (n=…)`). Firmware without the binding
  reports `not supported by this firmware`. Polling pauses during sketch
  transfers and needs the CDC log (off under `--no-serial-log`).

## Serial logs: `*-serial.log`
Two consoles are tailed for the whole run and written to one combined text file:
- **Debug UART** — the dongle on the board's debug header is a stable hardware
  UART carrying the ESP-IDF console (boot log, `ESP_LOGx`, panics): the board's
  "stderr". Connecting pulses the auto-reset line, so the board reboots as we
  attach — that's how we capture a clean boot log.
- **CDC** — the board's native USB is MicroPython's stdout (`print()`,
  tracebacks). It re-enumerates during boot (ROM USB-Serial-JTAG → TinyUSB CDC),
  so we start tailing it only *after* the board is back up; the stimulus prints a
  marker line so the capture is always exercised.

## Reference: `ref/hwci_basic.wav`
Committed golden for the `hwci_basic` test. It's **bench-specific** (depends on
this board + audio interface + gain); re-capture with `--update-reference` if the
hardware changes.

## Tulip (TULIP4_R11)

`tulip_hwci.py` is the Tulip analogue of `hwci.py`. The Tulip differs from the
AMYboard in three ways that shape the script:

- **One serial port does everything.** Tulip's USB is an **onboard CH340K**
  USB-UART (not a separate debug dongle). esptool flashes over it (its DTR/RTS
  drive the auto-reset circuit → ROM bootloader), and the *same* port is the
  MicroPython REPL we drive the test from **and** the ESP-IDF/boot console we log.
  TULIP4_R11 builds with `MICROPY_HW_ENABLE_UART_REPL` and no native USB.
- **No USB-MIDI** — the whole stimulus is REPL lines.
- **Firmware is a built artifact**, not a Vercel preview: pass `--firmware
  <tulip-firmware-TULIP4_R11.bin>` (CI downloads the `tulip-firmware` artifact the
  preview workflow builds).

It runs two checks:

1. **WiFi + screenshot (graphics):** join WiFi, set `world.username`, `execfile`
   `sys/ex/rgb332.py` (a static RGB332 palette), `tulip.tfb_stop()`, then
   `tulip.screenshot()` (uploads a PNG to Tulip World). The harness downloads that
   PNG from the **public** world API — requiring a row *newer* than the pre-run id,
   so a failed upload can't pass against a stale image — and pixel-compares it to
   `ref/tulip_screenshot.png`. **`tfb_stop()` is essential:** it keeps the REPL
   transcript (including the echoed WiFi password) out of the screenshot and makes
   the image deterministic (`mean_pixel_diff` is 0 run-to-run). The uploaded copy
   is then deleted from Tulip World (admin token) so CI runs don't clutter it.
2. **Audio (synth):** `amy.send(synth=1, patch=0, num_voices=6)` to define a synth,
   then a chromatic scale (notes 60–72) and a sustained 440 Hz sine; record the
   analog out and spectral-compare to `ref/tulip_basic.wav`.

**WiFi creds** come from `$TULIP_WIFI_SSID` / `$TULIP_WIFI_PASSWORD` (preferred over
CLI args, which show in `ps`). The REPL echoes the `tulip.wifi(...)` line, so the
harness **redacts** those values from the serial-log artifact + stdout. In CI
they're repo secrets and the job passes `--require-wifi` (a missing secret fails
loudly); `--no-wifi` runs audio-only.

Bench wiring: Tulip's analog out is summed into the **same** capture card as the
AMYboard, so the two tests must run sequentially (the CI job does this). Power the
Tulip from a **separate 5V supply** (a USB power-injection cable: D+/D-/GND to the
Pi, GND common, VBUS from the external adapter) — a Tulip on the Pi's own USB
power can trip the Pi 5's shared USB over-current protection and drop the whole
bench.

```
# capture references once (audio always; screenshot too if WiFi creds are set):
TULIP_WIFI_SSID=net TULIP_WIFI_PASSWORD=pw python3 tulip_hwci.py \
    --firmware ../../esp32s3/dist/tulip-firmware-TULIP4_R11.bin --update-reference
# pass/fail run (flash + wifi/screenshot + audio + compare):
TULIP_WIFI_SSID=net TULIP_WIFI_PASSWORD=pw python3 tulip_hwci.py \
    --firmware tulip-firmware-TULIP4_R11.bin
```

`--port` is the Tulip's CH340K (udev pins it to `/dev/tulip-repl`; raw it's a
`ttyUSB*`). `--no-flash` skips flashing to iterate on the checks. References are
**bench-specific** (board + audio gain); re-capture with `--update-reference`.

**Flashing is app-only by default** (`--flash-offset 0x10000`, the micropython
partition, ~30 s), since the bootloader/partition-table/filesystem rarely change.
To recover a wiped board, flash the **full** image:
`--firmware tulip-full-TULIP4_R11.bin --flash-offset 0x0` (~3 min — esptool erases
the whole 32 MB).

> **Note:** screenshot/file uploads from Tulip *and* AMYboard hardware
> (`world.upload`) require the `world.py` Content-Length fix in this change —
> older firmware sent HTTP/1.0 + chunked, which the server resets (`-104`).

## CI
`.github/workflows/amyboard-hwci.yml` drives both boards on the self-hosted Pi
after the "AMYboard PR preview" workflow builds the firmware (`workflow_run`), and
on manual `workflow_dispatch`. It flashes + tests the AMYboard, then the Tulip
(downloading the `tulip-firmware` artifact from the preview run), uploads each
board's recording + serial log + run log + the Tulip screenshot as the
`hwci-pr<N>` artifact, and upserts one combined PASS/FAIL comment. The Tulip step
needs repo secrets `TULIP_WIFI_SSID` / `TULIP_WIFI_PASSWORD` (+ optional
`WORLD_ADMIN_TOKEN` to delete each run's uploaded screenshot from Tulip World).
Gated to same-repo
PRs (the runner is on a public repo). Stable `/dev` names come from
[`99-amyboard.rules`](99-amyboard.rules); install it on the Pi (needs sudo) after
adding a board.
