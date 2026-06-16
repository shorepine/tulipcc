#!/usr/bin/env python3
"""AMYboard hardware-in-the-loop (HW CI) test.

Flashes a built AMYboard firmware onto a physically connected board, reboots it,
drives it with USB-MIDI notes and AMY `zP` sysex, records the analog audio out,
and RMS-compares the recording against a committed reference (like amy/test.py)
for pass/fail.

Two backends, auto-selected:
  * ALSA CLI (`amidi` + `arecord`)  — Linux / Raspberry Pi CI runner. No extra
    Python libs or root needed; only pip `esptool numpy` in a venv.
  * Python libs (`mido` + `sounddevice`) — macOS / dev bring-up.

Flashing is over a USB-serial dongle wired to the AMYboard debug header (the
dongle's DTR/RTS drive the board's auto-reset circuit, so esptool enters the
ROM bootloader and flashes over the dongle UART — robust, and recovers a
bricked board, unlike flashing over the board's own native USB).

Quick start (Pi):
    python3 hwci.py --list-devices
    python3 hwci.py --pr 993 --port /dev/ttyACM1 \\
        --midi-port hw:0,0,0 --audio-device hw:1,0 --update-reference
    python3 hwci.py --pr 993 --port /dev/ttyACM1 --midi-port hw:0,0,0 --audio-device hw:1,0
"""
import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import time
import urllib.request
import wave

HERE = os.path.dirname(os.path.abspath(__file__))
REF_DIR = os.path.join(HERE, "ref")
PREVIEW_URL = "https://amyboard-pr-{pr}.vercel.app/firmware/amyboard-full-AMYBOARD.bin"
AMY_SYSEX_PREFIX = [0x00, 0x03, 0x45]
MIDI_NAME = "AMYboard"
ALSA = bool(shutil.which("amidi") and shutil.which("arecord"))


# ── firmware ────────────────────────────────────────────────────────────────
def resolve_firmware(args):
    if args.firmware:
        return args.firmware, False
    url = args.url or (PREVIEW_URL.format(pr=args.pr) if args.pr else None)
    if not url:
        sys.exit("Need --firmware <path>, --url <url>, or --pr <N> (or --no-flash).")
    print(f"[fw] downloading {url}")
    fd, path = tempfile.mkstemp(suffix="-amyboard-full.bin")
    with urllib.request.urlopen(url, timeout=120) as r, os.fdopen(fd, "wb") as f:
        data = r.read()
        if data[:64].lstrip().startswith(b"<"):
            sys.exit("[fw] got HTML, not a .bin — bad URL or preview is auth-gated.")
        f.write(data)
    print(f"[fw] {len(data)} bytes -> {path}")
    return path, True


def flash(port, bin_path, baud):
    cmd = [sys.executable, "-m", "esptool", "--chip", "esp32s3", "--port", port,
           "--baud", str(baud), "--before", "default-reset", "--after", "hard-reset",
           "write-flash", "0x0", bin_path]
    print("[flash]", " ".join(cmd))
    subprocess.run(cmd, check=True)


# ── MIDI / sysex backends ───────────────────────────────────────────────────
def alsa_find_midi():
    out = subprocess.run(["amidi", "-l"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if MIDI_NAME in line:
            return line.split()[1]  # e.g. hw:0,0,0
    return None


class Midi:
    """Send-only MIDI to the board, via amidi (ALSA) or mido."""
    def __init__(self, port):
        self.port = port
        self._mido_out = None
        if not ALSA:
            import mido
            self._mido = mido
            self._mido_out = mido.open_output(port)

    def _raw(self, data):
        if ALSA:
            subprocess.run(["amidi", "-p", self.port, "-S",
                            " ".join("%02X" % b for b in data)], check=True)
        else:
            # split running-status-free messages; data is one message here
            self._mido_out.send(self._mido.Message.from_bytes(data))

    def note(self, n, vel, dur):
        self._raw([0x90, n, vel]); time.sleep(dur); self._raw([0x80, n, 0])

    def sysex(self, code):
        self._raw([0xF0] + AMY_SYSEX_PREFIX + list(code.encode("ascii")) + [0xF7])
        time.sleep(0.02)

    def close(self):
        if self._mido_out:
            self._mido_out.close()


def wait_for_board(timeout=40):
    """Wait for the board's USB-MIDI to (re)enumerate; return its port."""
    print(f"[boot] waiting for '{MIDI_NAME}' MIDI ...")
    deadline = time.time() + timeout
    while time.time() < deadline:
        port = alsa_find_midi() if ALSA else _mido_find()
        if port:
            print(f"[boot] found MIDI at {port}")
            return port
        time.sleep(0.5)
    sys.exit("[boot] board MIDI never appeared (didn't boot?).")


def _mido_find():
    import mido
    for n in mido.get_output_names():
        if MIDI_NAME in n:
            return n
    return None


# ── audio ───────────────────────────────────────────────────────────────────
def read_wav_mono(path):
    import numpy as np
    w = wave.open(path, "rb")
    d = np.frombuffer(w.readframes(w.getnframes()), dtype=np.int16).astype(np.float32) / 32768.0
    if w.getnchannels() > 1:
        d = d.reshape(-1, w.getnchannels()).mean(axis=1)
    return d, w.getframerate()


def write_wav_mono(path, samples, sr):
    import numpy as np
    w = wave.open(path, "wb")
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(sr)
    w.writeframes((np.clip(samples, -1, 1) * 32767).astype(np.int16).tobytes())
    w.close()


def record_and_drive(args, duration, drive_fn):
    import numpy as np
    sr, dev = args.samplerate, args.audio_device
    print(f"[audio] recording {duration}s @ {sr}Hz from {dev!r}")
    if ALSA:
        tmp = tempfile.mktemp(suffix=".wav")
        proc = subprocess.Popen(["arecord", "-D", dev, "-f", "S16_LE", "-r", str(sr),
                                 "-c", str(args.channels), "-d", str(int(duration + 1)), tmp],
                                stderr=subprocess.DEVNULL)
        time.sleep(0.3)
        drive_fn()
        proc.wait()
        d, _ = read_wav_mono(tmp)
        os.unlink(tmp)
        return d[:int(duration * sr)]
    import sounddevice as sd
    rec = sd.rec(int(duration * sr), samplerate=sr, channels=args.channels,
                 device=dev, dtype="float32")
    time.sleep(0.3); drive_fn(); sd.wait()
    return rec.mean(axis=1) if args.channels > 1 else rec[:, 0]


def list_devices():
    if ALSA:
        print("=== serial ports ==="); os.system("ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null")
        print("=== MIDI (amidi -l) ==="); os.system("amidi -l")
        print("=== audio capture (arecord -l) ==="); os.system("arecord -l")
    else:
        import serial.tools.list_ports, sounddevice as sd, mido
        for p in serial.tools.list_ports.comports():
            print("serial:", p.device, p.description)
        print("midi out:", mido.get_output_names())
        for i, d in enumerate(sd.query_devices()):
            if d["max_input_channels"]:
                print(f"audio in [{i}] {d['name']}")


# ── stimulus + compare ──────────────────────────────────────────────────────
def run_test_sequence(m):
    """Deterministic stimulus. EDIT to change what the HW CI exercises.

    Host-timed MIDI arpeggios are too jittery to compare audio on (timing/drops
    shift the spectrum run-to-run). Instead we play a *sustained* board-scheduled
    chord via zP, so the averaged spectrum is stable. We still poke the USB-MIDI
    path first (a quick note) to confirm it's alive; its ring-out is cleared
    before the recorded chord."""
    # 1) exercise USB-MIDI (not part of the spectral compare)
    m.note(60, 100, 0.15)
    m.sysex("zPimport amy; amy.reset()Z")   # clean slate, kill any ring-out
    time.sleep(0.5)
    # 2) sustained 3-osc chord (A4/C#5/E5) — deterministic, reproducible spectrum
    m.sysex("zPimport amy; amy.send(osc=0,wave=amy.SINE,freq=440,vel=1); "
            "amy.send(osc=1,wave=amy.SINE,freq=554,vel=1); "
            "amy.send(osc=2,wave=amy.SINE,freq=659,vel=1)Z")
    time.sleep(2.6)
    m.sysex("zPimport amy; amy.send(osc=0,vel=0); amy.send(osc=1,vel=0); amy.send(osc=2,vel=0)Z")


def avg_spectrum(x, nfft=4096):
    """Average magnitude spectrum (Welch-ish) — timing/phase invariant, so two
    recordings of the same notes/timbre match even with host timing jitter."""
    import numpy as np
    win = np.hanning(nfft); step = nfft // 2; mags = []
    for i in range(0, max(1, len(x) - nfft + 1), step):
        seg = x[i:i + nfft]
        if len(seg) == nfft:
            mags.append(np.abs(np.fft.rfft(seg * win)))
    if not mags:
        seg = np.zeros(nfft); seg[:min(len(x), nfft)] = x[:nfft]
        mags = [np.abs(np.fft.rfft(seg * win))]
    s = np.mean(mags, axis=0)
    return s / (np.linalg.norm(s) + 1e-9)


def compare(rec, ref):
    """Returns (spectral cosine similarity in 0..1, recording level in dB)."""
    import numpy as np
    sim = float(np.dot(avg_spectrum(rec), avg_spectrum(ref)))  # 1.0 == identical
    level_db = 20 * np.log10(float(np.sqrt(np.mean(rec ** 2))) + 1e-9)
    return sim, level_db


# ── main ────────────────────────────────────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(description="AMYboard hardware-in-the-loop test")
    ap.add_argument("--list-devices", action="store_true")
    ap.add_argument("--pr", type=int); ap.add_argument("--url"); ap.add_argument("--firmware")
    ap.add_argument("--port", help="esptool serial port (the USB-serial dongle)")
    ap.add_argument("--baud", type=int, default=921600)
    ap.add_argument("--no-flash", action="store_true")
    ap.add_argument("--flash-only", action="store_true")
    ap.add_argument("--midi-port", help="ALSA hw:X,Y or mido name (auto if omitted)")
    ap.add_argument("--audio-device", default="hw:1,0" if ALSA else None,
                    help="ALSA hw:X,Y or sounddevice index/name")
    ap.add_argument("--samplerate", type=int, default=48000)
    ap.add_argument("--channels", type=int, default=1)
    ap.add_argument("--duration", type=float, default=4.0)
    ap.add_argument("--name", default="hwci_basic")
    ap.add_argument("--reference"); ap.add_argument("--update-reference", action="store_true")
    ap.add_argument("--min-similarity", type=float, default=0.90,
                    help="min spectral cosine similarity to pass (0..1)")
    ap.add_argument("--min-level-db", type=float, default=-30.0,
                    help="min recording RMS level (dB) to pass")
    ap.add_argument("--out")
    args = ap.parse_args()
    print(f"[hwci] backend: {'ALSA cli' if ALSA else 'python libs'}")

    if args.list_devices:
        list_devices(); return 0

    if not args.no_flash:
        if not args.port:
            sys.exit("--port (the USB-serial dongle) is required to flash.")
        bin_path, tmp = resolve_firmware(args)
        try:
            flash(args.port, bin_path, args.baud)
        finally:
            if tmp:
                os.unlink(bin_path)
        if args.flash_only:
            print("[done] flashed."); return 0

    args.midi_port = args.midi_port or wait_for_board()
    if not args.audio_device:
        sys.exit("--audio-device required (see --list-devices).")
    m = Midi(args.midi_port)
    try:
        rec = record_and_drive(args, args.duration, lambda: run_test_sequence(m))
    finally:
        m.close()

    out_wav = args.out or f"{args.name}-recording.wav"
    write_wav_mono(out_wav, rec, args.samplerate)
    print(f"[audio] saved {out_wav}")

    ref_path = args.reference or os.path.join(REF_DIR, f"{args.name}.wav")
    if args.update_reference:
        os.makedirs(REF_DIR, exist_ok=True)
        write_wav_mono(ref_path, rec, args.samplerate)
        print(f"[ref] wrote {ref_path} — LISTEN to confirm it's correct.")
        return 0
    if not os.path.exists(ref_path):
        sys.exit(f"[ref] no reference at {ref_path}; run once with --update-reference.")
    ref, _ = read_wav_mono(ref_path)
    sim, level_db = compare(rec, ref)
    ok = sim >= args.min_similarity and level_db >= args.min_level_db
    print(f"\n{args.name}: spectral_similarity={sim:.4f} (min {args.min_similarity})  "
          f"level={level_db:.1f}dB (min {args.min_level_db})  ->  {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
