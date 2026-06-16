#!/usr/bin/env python3
"""AMYboard hardware-in-the-loop (HW CI) test.

Flashes a built AMYboard firmware onto a physically connected board (over the
serial debug header, which carries RTS/DTR for auto-reset + download mode),
reboots it, drives it with USB-MIDI notes and AMY `zP` sysex commands, records
the analog audio out from a connected audio input device, and RMS-compares the
recording against a committed reference (like amy/test.py) for pass/fail.

Prototype target: a Mac with the board on USB-serial + audio out wired into a
USB audio interface. Eventually: a self-hosted GitHub Actions runner (Pi).

Quick start
-----------
    pip install -r requirements.txt
    # 1) find your devices:
    python3 hwci.py --list-devices
    # 2) first good run captures the golden reference (listen to the wav!):
    python3 hwci.py --pr 993 --port /dev/cu.usbserial-XXX \\
        --input-device "Your Interface" --update-reference
    # 3) thereafter, pass/fail vs the reference:
    python3 hwci.py --pr 993 --port /dev/cu.usbserial-XXX \\
        --input-device "Your Interface"

Stages can be isolated: --no-flash (test an already-flashed board),
--flash-only, --list-devices.
"""
import argparse
import os
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
REF_DIR = os.path.join(HERE, "ref")
PREVIEW_URL = "https://amyboard-pr-{pr}.vercel.app/firmware/amyboard-full-AMYBOARD.bin"
AMY_SYSEX_PREFIX = [0x00, 0x03, 0x45]  # AMYboard sysex manufacturer-ish prefix
MIDI_NAME = "AMYboard"


# ── device discovery ────────────────────────────────────────────────────────
def list_devices():
    import serial.tools.list_ports
    import sounddevice as sd
    import mido
    print("Serial ports:")
    for p in serial.tools.list_ports.comports():
        print(f"  {p.device}   {p.description}")
    print("\nMIDI outputs:")
    for n in mido.get_output_names():
        print(f"  {n}")
    print("\nAudio input devices:")
    for i, d in enumerate(sd.query_devices()):
        if d["max_input_channels"] > 0:
            print(f"  [{i}] {d['name']}  ({d['max_input_channels']}ch @ {int(d['default_samplerate'])}Hz)")


def autodetect_serial():
    import serial.tools.list_ports
    # ESP32-S3 USB-serial bridges: CP210x, CH34x, or the native USB-JTAG.
    for p in serial.tools.list_ports.comports():
        blob = f"{p.description} {p.manufacturer or ''} {p.device}".lower()
        if any(k in blob for k in ("cp210", "ch34", "usbserial", "uart", "esp32", "jtag")):
            return p.device
    return None


# ── firmware ────────────────────────────────────────────────────────────────
def resolve_firmware(args):
    if args.firmware:
        return args.firmware, False
    url = args.url or (PREVIEW_URL.format(pr=args.pr) if args.pr else None)
    if not url:
        sys.exit("Need one of --firmware <path>, --url <url>, or --pr <N> (or --no-flash).")
    import requests
    print(f"[fw] downloading {url}")
    r = requests.get(url, timeout=60)
    if r.status_code != 200:
        sys.exit(f"[fw] download failed: HTTP {r.status_code} (is the PR preview deployed + public?)")
    if r.content[:64].lstrip().startswith(b"<"):
        sys.exit("[fw] got HTML, not a .bin — bad URL or the preview is auth-gated.")
    fd, path = tempfile.mkstemp(suffix="-amyboard-full.bin")
    os.write(fd, r.content)
    os.close(fd)
    print(f"[fw] {len(r.content)} bytes -> {path}")
    return path, True


def flash(port, bin_path, baud):
    # The merged "full" image flashes at offset 0x0. esptool uses RTS/DTR on the
    # debug header to enter download mode (--before) and reboot after (--after).
    cmd = [sys.executable, "-m", "esptool", "--chip", "esp32s3", "--port", port,
           "--baud", str(baud), "--before", "default_reset", "--after", "hard_reset",
           "write_flash", "0x0", bin_path]
    print("[flash]", " ".join(cmd))
    subprocess.run(cmd, check=True)


# ── board comms ─────────────────────────────────────────────────────────────
def wait_for_midi(timeout=40):
    import mido
    print(f"[boot] waiting for USB-MIDI '{MIDI_NAME}' to enumerate...")
    deadline = time.time() + timeout
    while time.time() < deadline:
        for n in mido.get_output_names():
            if MIDI_NAME in n:
                print(f"[boot] found {n}")
                return n
        time.sleep(0.5)
    sys.exit(f"[boot] '{MIDI_NAME}' MIDI device never appeared (board didn't boot / no USB MIDI?).")


def amy_sysex(out, msg):
    """Send an AMY wire message (e.g. a zP command) over sysex."""
    import mido
    out.send(mido.Message("sysex", data=AMY_SYSEX_PREFIX + list(msg.encode("ascii"))))
    time.sleep(0.02)


def run_test_sequence(out):
    """The actual stimulus. EDIT ME to define what the HW CI exercises.

    On boot the board sets up channel 1 with the amyboard default patch (K257)
    and its default CC map, so MIDI notes on ch1 make sound immediately. Then we
    run a couple of AMY commands via zP. Keep this deterministic + reproducible;
    the reference wav captures whatever it produces.
    """
    import mido
    # 1) USB-MIDI: a C-major arpeggio on channel 1.
    for note in (60, 64, 67, 72):
        out.send(mido.Message("note_on", channel=0, note=note, velocity=100))
        time.sleep(0.25)
        out.send(mido.Message("note_off", channel=0, note=note, velocity=0))
    time.sleep(0.3)
    # 2) AMY via zP: a distinctive bleep on a fresh osc.
    amy_sysex(out, "zPimport amy; amy.send(osc=0, wave=amy.SINE, freq=330, vel=1)Z")
    time.sleep(0.6)
    amy_sysex(out, "zPimport amy; amy.send(osc=0, vel=0)Z")
    time.sleep(0.3)


# ── audio ───────────────────────────────────────────────────────────────────
def record_and_drive(input_device, samplerate, channels, duration, drive_fn):
    import sounddevice as sd
    import numpy as np
    print(f"[audio] recording {duration}s @ {samplerate}Hz from device {input_device!r}")
    frames = int(duration * samplerate)
    rec = sd.rec(frames, samplerate=samplerate, channels=channels, device=input_device, dtype="float32")
    time.sleep(0.25)  # let recording settle before stimulus
    drive_fn()
    sd.wait()
    return np.mean(rec, axis=1) if channels > 1 else rec[:, 0]


def _onset(x, thresh=0.02):
    import numpy as np
    above = np.flatnonzero(np.abs(x) > thresh)
    return int(above[0]) if above.size else 0


def compare(rec, ref):
    """RMS-difference in dB after onset-align + peak-normalize. Hardware audio
    isn't sample-exact (latency, noise floor, levels, clock drift), so this is a
    looser, tunable metric than amy/test.py's bit-exact compare."""
    import numpy as np
    a, b = rec[_onset(rec):], ref[_onset(ref):]
    n = min(len(a), len(b))
    a, b = a[:n], b[:n]
    pa, pb = np.max(np.abs(a)) or 1.0, np.max(np.abs(b)) or 1.0
    a, b = a / pa, b / pb
    rms = lambda s: float(np.sqrt(np.mean(s ** 2)))
    db = lambda v: 20 * np.log10(v + 1e-9)
    return db(rms(a - b)), db(rms(rec))


# ── main ────────────────────────────────────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(description="AMYboard hardware-in-the-loop test")
    ap.add_argument("--list-devices", action="store_true", help="list serial/MIDI/audio devices and exit")
    src = ap.add_argument_group("firmware source")
    src.add_argument("--pr", type=int, help="download firmware from amyboard-pr-<N>.vercel.app")
    src.add_argument("--url", help="download firmware from this URL")
    src.add_argument("--firmware", help="local firmware .bin (full image, flashed at 0x0)")
    ap.add_argument("--port", help="serial debug port (auto-detected if omitted)")
    ap.add_argument("--baud", type=int, default=921600)
    ap.add_argument("--no-flash", action="store_true", help="skip flashing; test the board as-is")
    ap.add_argument("--flash-only", action="store_true", help="flash then exit")
    ap.add_argument("--input-device", help="audio input device name or index")
    ap.add_argument("--samplerate", type=int, default=48000)
    ap.add_argument("--channels", type=int, default=1)
    ap.add_argument("--duration", type=float, default=4.0, help="record window (s)")
    ap.add_argument("--name", default="hwci_basic", help="test name (reference wav basename)")
    ap.add_argument("--reference", help="reference wav (default ref/<name>.wav)")
    ap.add_argument("--update-reference", action="store_true", help="save this run as the reference")
    ap.add_argument("--threshold", type=float, default=-12.0, help="max RMS-diff dB to pass")
    ap.add_argument("--out", help="where to save the recording (default ./<name>-recording.wav)")
    args = ap.parse_args()

    if args.list_devices:
        list_devices()
        return 0

    import soundfile as sf
    import numpy as np

    if not args.no_flash:
        port = args.port or autodetect_serial()
        if not port:
            sys.exit("No serial port found; pass --port (see --list-devices).")
        bin_path, tmp = resolve_firmware(args)
        try:
            flash(port, bin_path, args.baud)
        finally:
            if tmp:
                os.unlink(bin_path)
        if args.flash_only:
            print("[done] flashed.")
            return 0

    out_name = wait_for_midi()

    import mido
    with mido.open_output(out_name) as out:
        rec = record_and_drive(args.input_device, args.samplerate, args.channels,
                               args.duration, lambda: run_test_sequence(out))

    out_wav = args.out or f"{args.name}-recording.wav"
    sf.write(out_wav, rec, args.samplerate)
    print(f"[audio] saved {out_wav}")

    ref_path = args.reference or os.path.join(REF_DIR, f"{args.name}.wav")
    if args.update_reference:
        os.makedirs(REF_DIR, exist_ok=True)
        sf.write(ref_path, rec, args.samplerate)
        print(f"[ref] wrote reference {ref_path} — LISTEN to it to confirm it's correct.")
        return 0

    if not os.path.exists(ref_path):
        sys.exit(f"[ref] no reference at {ref_path}; run once with --update-reference.")
    ref, _ = sf.read(ref_path, dtype="float32")
    if ref.ndim > 1:
        ref = np.mean(ref, axis=1)
    diff_db, sig_db = compare(rec, ref)
    ok = diff_db <= args.threshold
    print(f"\n{args.name}: signal={sig_db:.1f}dB  rms_diff={diff_db:.1f}dB  "
          f"threshold={args.threshold:.1f}dB  ->  {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
