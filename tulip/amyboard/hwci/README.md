# AMYboard hardware-in-the-loop (HW CI)

`hwci.py` runs a real AMYboard through a full firmware test: flash → reboot →
drive over USB-MIDI + AMY `zP` sysex → record the analog audio out → RMS-compare
to a committed reference (same idea as `amy/test.py`, but on real hardware).

Prototype runs on a **Mac**; the eventual target is a **Raspberry Pi** acting as
a self-hosted GitHub Actions runner so it runs per-PR.

## Bench setup
- **Serial debug header → USB**: carries RTS/DTR, so esptool can enter download
  mode and reboot the board with no buttons.
- **Audio out → a USB audio interface input** wired into the host. `hwci.py
  --list-devices` shows the input devices; pass it with `--input-device`.
- **USB-MIDI**: the board enumerates as the `AMYboard` MIDI device after boot.

## Install
```
pip install -r requirements.txt
```
(`esptool` must be importable by the same Python: `python3 -m esptool version`.)

## Use
```
# discover serial / MIDI / audio devices
python3 hwci.py --list-devices

# first good run: capture the golden reference (then LISTEN to the wav!)
python3 hwci.py --pr 993 --port /dev/cu.usbserial-XXX \
    --input-device "Your Interface" --update-reference

# pass/fail vs the reference
python3 hwci.py --pr 993 --port /dev/cu.usbserial-XXX --input-device "Your Interface"
```

Firmware comes from the **per-PR preview** (`amyboard-pr-<N>.vercel.app/firmware/
amyboard-full-AMYBOARD.bin`); use `--firmware <path>` or `--url <url>` instead.

Bring-up in stages: `--list-devices`, then `--flash-only`, then `--no-flash`
(skip flashing to iterate on the MIDI/record/compare steps against an
already-flashed board).

## The test stimulus
`run_test_sequence()` in `hwci.py` is the editable definition of what the test
exercises — currently a C-major arpeggio over USB-MIDI on channel 1 (the board
boots with the K257 default patch + default CCs) plus a `zP` AMY bleep. The
reference wav captures whatever it produces, so keep the sequence deterministic.

## The compare
Hardware audio is **not** sample-exact (capture latency, noise floor, levels,
DAC/ADC clock drift), so `compare()` onset-aligns, peak-normalizes, and takes the
RMS difference in dB against `--threshold` (default −12 dB). Expect to calibrate
the threshold on your bench once you have a stable reference.

## Open items / next steps
- **PR upload**: GitHub's API can't attach binaries to PR *comments*. Plan: when
  this runs in CI on the Pi, upload the recording as a **workflow artifact** and
  comment the run/artifact link with the RMS pass/fail; locally we just save the
  wav for now.
- **CI wiring**: a self-hosted-runner workflow (label- or dispatch-triggered) on
  the Pi that runs `hwci.py --pr <N>` and posts results.
- **Reference management**: store per-test reference wavs under `ref/`.
