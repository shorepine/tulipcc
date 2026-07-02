#!/usr/bin/env python3
"""AMYboard hardware-in-the-loop (HW CI) test.

Flashes a built AMYboard firmware onto a physically connected board, reboots it,
drives it with USB-MIDI notes and AMY `zP` sysex, records the analog audio out,
and spectral-compares the recording against a committed reference (like
amy/test.py) for pass/fail.

Two phases per run:
  1. Built-in reference tones (ref/hwci_basic.wav): a scale + chord over USB-MIDI
     and a sustained board-scheduled `zP` sine chord.
  2. AMYboard World sketches: download real community sketches from World and push
     each onto the board over the SysEx "write to sketch" control API (the same
     flow the web editor uses — see docs/amyboard/control_api.md), drive them with
     a simple MIDI pattern, and record each to `{name}-recording.wav` /
     `ref/{name}.wav`. This exercises far more of AMY than the tones — the drum-
     heavy generators would have caught the PR #1067 drums regression the tone-only
     test missed. The generators are random, so we compare with the timing/phase-
     invariant avg-spectrum metric (their timbre is stable even when the notes
     aren't). Disable with --no-world.

The two serial consoles are tailed for the whole run — the debug UART (ESP-IDF
console / stderr) and the native CDC (MicroPython stdout) — and saved to a
combined `{name}-serial.log`, so a CI run can attach the audio and the logs to
the PR.

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
import base64
import json
import os
import shutil
import subprocess
import sys
import tempfile
import threading
import time
import urllib.parse
import urllib.request
import wave

HERE = os.path.dirname(os.path.abspath(__file__))
REF_DIR = os.path.join(HERE, "ref")
PREVIEW_URL = "https://amyboard-pr-{pr}.vercel.app/firmware/amyboard-full-AMYBOARD.bin"
AMY_SYSEX_PREFIX = [0x00, 0x03, 0x45]
MIDI_NAME = "AMYboard"
ALSA = bool(shutil.which("amidi") and shutil.which("arecord"))

# AMYboard World: the second half of the test loads real community sketches over
# the SysEx "write to sketch" flow (the same one the web editor uses) and records
# each one, so the HW CI exercises more than the built-in reference tones. See
# docs/amyboard/control_api.md and tools/amyworld_recorder/. PR #1067 was a drums
# regression the tone-only test missed — the drum-heavy generators below cover it.
WORLD_BASE = "https://tulipcc-production.up.railway.app"
SKETCH_PATH = "/user/current/sketch.py"
CHUNK_BYTES = 188          # AMYBOARD_TRANSFER_CHUNK_BYTES (spss.js / transfer.c)

# The World sketches to audition, in order. `generative` sketches drive themselves
# from loop() (started by the sequencer that environment_transfer_done() kicks off)
# and use random() — so run-to-run they differ in the notes but not the timbre, and
# the timing/phase-invariant avg-spectrum compare still matches. `woodpiano` is a
# bare DX7 patch that only sounds when we send it MIDI notes. We send the simple
# MIDI pattern to every sketch (it also proves the USB-MIDI note path per sketch).
WORLD_SUITE = [
    {"name": "acid_generator",  "author": "shorepine", "generative": True,  "min_sim": 0.80},
    {"name": "house_generator", "author": "shorepine", "generative": True,  "min_sim": 0.80},
    {"name": "universal_hair",  "author": "shorepine", "generative": True,  "min_sim": 0.82},
    {"name": "woodpiano",       "author": "shorepine", "generative": False, "min_sim": 0.85},
]


# ── AMYboard World sketch fetch (stdlib only — the Pi CI venv has no `requests`) ─
def world_fetch_sketch(name, author, base=WORLD_BASE, timeout=30):
    """Resolve `author`/`name` to a World sketch and return its python source.

    Resolves by name+author at run time (rather than a hard-coded id) so a
    re-upload doesn't silently test the wrong file. Backed by the public
    read-only API in tulip/server/amyboardworld_db_api.py."""
    params = urllib.parse.urlencode({
        "limit": 20, "item_type": "environment", "latest_per_user_env": "true",
        "q": name, "username": author})
    with urllib.request.urlopen("%s/api/amyboardworld/files?%s" % (base, params),
                                timeout=timeout) as r:
        items = json.load(r).get("items", [])
    want = name + ".py"
    match = next((it for it in items if str(it.get("filename", "")) == want
                  and str(it.get("username", "")).lower() == author.lower()), None)
    match = match or next((it for it in items if str(it.get("filename", "")) == want), None)
    if not match:
        raise RuntimeError("World sketch %s/%s not found" % (author, name))
    url = match.get("download_url") or ("/api/amyboardworld/files/%s/download" % match["id"])
    if not url.startswith("http"):
        url = base + url
    with urllib.request.urlopen(url, timeout=timeout) as r:
        return r.read().decode("utf-8")


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


def reset_via_chipid(port):
    """Clean-boot the board with an esptool `chip-id`: it enters download mode,
    reads the chip id (returning the MAC), then hard-resets and releases the
    port. On this bench the flash's own `--after hard-reset` can leave the board
    wedged in a boot-loop, but a chip-id cycle reliably boots it. Returns True
    only if the MAC came back -- i.e. esptool actually completed the handshake
    and reset the board (a bare `chip-id` defaults to --before default-reset /
    --after hard-reset). Give the board a few seconds afterward to boot and
    enumerate USB-MIDI before looking for it (see --boot-settle)."""
    cmd = [sys.executable, "-m", "esptool", "--chip", "esp32s3", "--port", port, "chip-id"]
    print("[reset]", " ".join(cmd))
    r = subprocess.run(cmd, capture_output=True, text=True)
    sys.stdout.write(r.stdout)
    ok = r.returncode == 0 and "MAC:" in r.stdout
    if not ok:
        print("[reset] chip-id did not return a MAC -- board/dongle not responding")
    return ok


# ── MIDI / sysex backends ───────────────────────────────────────────────────
def alsa_find_midi():
    out = subprocess.run(["amidi", "-l"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if MIDI_NAME in line:
            return line.split()[1]  # e.g. hw:0,0,0
    return None


class Midi:
    """MIDI to the board, via amidi (ALSA) or mido.

    Send-only for the built-in reference test. For the AMYboard World sketch
    transfers (many chunked SysEx frames) we MUST flow-control so the board's
    SysEx ring buffer can't overflow and drop chunks (a dropped chunk corrupts
    sketch.py → the board self-heals to the silent default sketch). So we wait for
    the board's `AK` after every frame — one frame in flight — exactly like the
    web editor and tools/amyworld_recorder/amyboard_link.py (see
    docs/amyboard/control_api.md). Both backends read the board's MIDI *input*:

      * mido backend (mac dev): an input port with a callback.
      * ALSA backend (Pi CI): a persistent `stdbuf -oL amidi -p PORT -d` reader
        (line-buffered so `AK` frames arrive in real time, not block-buffered)
        alongside the one-shot `amidi -S` sends — the two coexist fine on the
        board's rawmidi device.

    If the input reader can't be started, we degrade to pacing the frames with a
    fixed delay (works for small sketches, may drop chunks on large ones).
    """
    ACK_TIMEOUT = 2.0          # wait this long for the board's AK, then proceed
    PACE = 0.03                # inter-frame delay when we can't wait on AK

    def __init__(self, port):
        self.port = port
        self._mido = None
        self._mido_out = None
        self._mido_in = None
        self._ack_proc = None
        self._ack = threading.Event()
        self.ack_available = False
        if not ALSA:
            import mido
            self._mido = mido
            self._mido_out = mido.open_output(port)
            in_name = _mido_find_input()
            if in_name:
                self._mido_in = mido.open_input(in_name, callback=self._on_mido_message)
                self.ack_available = True
        else:
            try:
                self._ack_proc = subprocess.Popen(
                    ["stdbuf", "-oL", "amidi", "-p", port, "-d"],
                    stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True, bufsize=1)
                threading.Thread(target=self._alsa_ack_reader, daemon=True).start()
                self.ack_available = True
            except Exception as e:
                print("[midi] no ACK reader (%s); pacing sysex instead" % e)

    # -- incoming — set the ACK event on the board's `AK` frame -----------------
    def _on_mido_message(self, msg):
        if msg.type != "sysex":
            return
        d = msg.data
        if len(d) >= 5 and tuple(d[:3]) == tuple(AMY_SYSEX_PREFIX) \
                and d[3] == 0x41 and d[4] == 0x4B:      # 'A' 'K'
            self._ack.set()

    def _alsa_ack_reader(self):
        """Parse the `amidi -d` hex stream for `F0 00 03 45 41 4B F7` (AK) frames
        and set the ACK event. Other reply frames (OK/X/…) are ignored."""
        buf, insx = [], False
        try:
            for line in self._ack_proc.stdout:      # line-buffered by stdbuf -oL
                for tok in line.split():
                    try:
                        b = int(tok, 16)
                    except ValueError:
                        continue
                    if b == 0xF0:
                        insx, buf = True, []
                    elif b == 0xF7:
                        if insx and buf[:3] == AMY_SYSEX_PREFIX and buf[3:5] == [0x41, 0x4B]:
                            self._ack.set()
                        insx, buf = False, []
                    elif insx:
                        buf.append(b)
        except Exception:
            pass

    def _raw(self, data, retries=3):
        if ALSA:
            # `amidi -S` occasionally exits non-zero for a beat right after a
            # sketch restart re-inits the board's USB-MIDI endpoint (seen on the
            # first note after environment_transfer_done). A dropped frame must
            # not abort the whole capture, so retry briefly then warn + continue
            # rather than raise.
            cmd = ["amidi", "-p", self.port, "-S", " ".join("%02X" % b for b in data)]
            for attempt in range(retries + 1):
                r = subprocess.run(cmd, capture_output=True, text=True)
                if r.returncode == 0:
                    return
                if attempt < retries:
                    time.sleep(0.1)
            print("[midi] amidi -S failed after %d tries (%s); continuing"
                  % (retries + 1, (r.stderr or "").strip()))
        else:
            # split running-status-free messages; data is one message here
            self._mido_out.send(self._mido.Message.from_bytes(data))

    def note_on(self, n, vel=100, channel=0):
        self._raw([0x90 | (channel & 0x0F), n, vel])

    def note_off(self, n, channel=0):
        self._raw([0x80 | (channel & 0x0F), n, 0])

    def all_notes_off(self, channel=0):
        self._raw([0xB0 | (channel & 0x0F), 123, 0])   # CC 123 = all notes off

    def note(self, n, vel, dur):
        self.note_on(n, vel); time.sleep(dur); self.note_off(n)

    def _frame(self, code):
        return [0xF0] + AMY_SYSEX_PREFIX + list(code.encode("ascii")) + [0xF7]

    def sysex(self, code):
        """Fire-and-forget one control frame (used by the built-in reference test,
        whose frames are small and few — its committed ref/hwci_basic.wav depends
        on this timing, so leave it alone)."""
        self._raw(self._frame(code))
        time.sleep(0.02)

    def sysex_ack(self, code, timeout=ACK_TIMEOUT):
        """Send one control frame with flow control: wait for the board's `AK`
        (mido) or pace with a fixed delay (ALSA). Returns True if acked/sent."""
        if self.ack_available:
            self._ack.clear()
            self._raw(self._frame(code))
            return self._ack.wait(timeout)
        self._raw(self._frame(code))
        time.sleep(self.PACE)
        return True

    def send_python(self, code, timeout=ACK_TIMEOUT):
        """Run one line of python on the board via zP (control_api.md)."""
        return self.sysex_ack("zP" + code + "Z", timeout=timeout)

    # -- push a sketch onto the board (zT header + base64 chunks) ---------------
    def transfer_sketch(self, text, path=SKETCH_PATH):
        """Write a sketch to the board, mirroring the web editor's zT flow: a
        `zT<path>,<size>Z` header then the file bytes in <=188-byte base64 chunks,
        one frame at a time with flow control."""
        data = text.encode("utf-8")
        self.sysex_ack("zT%s,%dZ" % (path, len(data)))
        for i in range(0, len(data), CHUNK_BYTES):
            self.sysex_ack(base64.b64encode(data[i:i + CHUNK_BYTES]).decode("ascii"))

    def load_world_sketch(self, text, settle=1.5):
        """amy.reset() → transfer → environment_transfer_done() (restarts the
        sketch and starts the sequencer, so generative loop()s begin playing).
        Give the sketch a moment to init before we start recording it."""
        self.send_python("import amy; amy.reset()")
        time.sleep(0.3)
        self.transfer_sketch(text)
        self.send_python("import amyboard; amyboard.environment_transfer_done()")
        time.sleep(settle)

    def play_pattern(self, seconds, channel=0):
        """A simple, DETERMINISTIC A-minor arpeggio looped for `seconds` on ch1
        (what World sketches listen on). Deterministic so a patch sketch like
        woodpiano yields a stable reference; generative sketches self-drive and
        just get these as extra notes in the same timbre."""
        root, arp, step = 57, [0, 3, 7, 12, 7, 3], 0.18   # A3, minor triad + octave
        end = time.time() + seconds
        i = 0
        while time.time() < end:
            n = root + arp[i % len(arp)]
            self.note_on(n, 100, channel)
            time.sleep(step * 0.9)
            self.note_off(n, channel)
            time.sleep(step * 0.1)
            i += 1
        self.all_notes_off(channel)

    def silence(self):
        """Leave the board quiet after the World suite so it doesn't bleed into
        the Tulip recording that runs next on the shared capture card: stop the
        sequencer (halts generative loop()s), reset AMY, kill any held notes."""
        try:
            self.sysex("zY0Z")                       # stop sequencer transport
            self.send_python("import amyboard; amyboard.stop_sketch()")  # kill loop()
            self.send_python("import amy; amy.reset()")
            self.all_notes_off(0)
        except Exception:
            pass

    def close(self):
        for p in (self._mido_out, self._mido_in):
            try:
                if p:
                    p.close()
            except Exception:
                pass
        if self._ack_proc:
            try:
                self._ack_proc.terminate()
            except Exception:
                pass


def wait_for_board(timeout=40):
    """Wait for the board's USB-MIDI to (re)enumerate; return its port, or None
    if it doesn't appear within `timeout` (so the caller can reset + retry)."""
    print(f"[boot] waiting for '{MIDI_NAME}' MIDI ...")
    deadline = time.time() + timeout
    while time.time() < deadline:
        port = alsa_find_midi() if ALSA else _mido_find()
        if port:
            print(f"[boot] found MIDI at {port}")
            return port
        time.sleep(0.5)
    print("[boot] board MIDI never appeared (didn't boot?).")
    return None


def _mido_find():
    import mido
    for n in mido.get_output_names():
        if MIDI_NAME in n:
            return n
    return None


def _mido_find_input():
    import mido
    for n in mido.get_input_names():
        if MIDI_NAME in n:
            return n
    return None


# ── serial console capture ───────────────────────────────────────────────────
class SerialLog:
    """Tail a serial port into memory on a background thread, for the run's log.

    Opens with DTR/RTS de-asserted to minimize line disturbance. Note the
    kernel still briefly raises the modem lines as the tty opens, so on the
    debug-header dongle (whose DTR/RTS feed the board's auto-reset circuit:
    DTR->IO0, RTS->EN) connecting reboots the board — which is how we get a
    clean boot log. Retries the open for a few seconds because the port may
    still be re-enumerating right after a flash/reset."""
    def __init__(self, port, baud, label):
        self.port, self.baud, self.label = port, baud, label
        self.buf = bytearray()
        self.opened = False
        self._stop = threading.Event()
        self._t = threading.Thread(target=self._run, daemon=True)

    def start(self):
        self._t.start()

    def _run(self):
        try:
            import serial  # pyserial (ships with esptool)
        except ImportError:
            print(f"[log] pyserial missing; cannot capture {self.port}")
            return
        ser = None
        deadline = time.monotonic() + 8.0
        while ser is None and not self._stop.is_set() and time.monotonic() < deadline:
            try:
                ser = serial.Serial()
                ser.port = self.port
                ser.baudrate = self.baud
                ser.timeout = 0.2
                ser.dtr = False  # don't trigger auto-reset on open
                ser.rts = False
                ser.open()
            except Exception:
                ser = None
                time.sleep(0.3)
        if ser is None:
            print(f"[log] could not open {self.port} ({self.label})")
            return
        self.opened = True
        while not self._stop.is_set():
            try:
                data = ser.read(4096)
            except Exception:
                break
            if data:
                self.buf.extend(data)
        try:
            ser.close()
        except Exception:
            pass

    def stop(self):
        self._stop.set()
        self._t.join(timeout=3)

    def text(self):
        return self.buf.decode("utf-8", "replace")


def start_serial_log(port, baud, label):
    """Start tailing one serial port in the background; returns the SerialLog
    (or None if no port given)."""
    if not port:
        return None
    sl = SerialLog(port, baud, label)
    sl.start()
    print(f"[log] capturing {port} ({label})")
    return sl


def write_serial_logs(logs, path):
    """Stop the tailers and write one combined text log (one section per port)."""
    for sl in logs:
        sl.stop()
    with open(path, "w", encoding="utf-8") as f:
        for sl in logs:
            f.write(f"===== {sl.label}  [{sl.port} @ {sl.baud} baud] =====\n")
            if not sl.opened:
                f.write(f"(could not open {sl.port})\n")
            body = sl.text()
            f.write(body)
            if not body.endswith("\n"):
                f.write("\n")
            f.write("\n")
    print(f"[log] wrote serial log -> {path}")


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
    # Ensure ch1 has the default patch (K257, 6 voices) so the USB-MIDI notes
    # make sound even if a previous run's amy.reset() cleared it (there's no
    # reboot between --no-flash runs).
    m.sysex("i1K257iv6Z")
    time.sleep(0.5)
    # Emit a marker on MicroPython stdout (the CDC port) so the serial log proves
    # the stdout-capture path and carries a little board provenance.
    m.sysex("zPimport gc; print('hwci: stdout ok, mem_free', gc.mem_free())Z")
    time.sleep(0.2)
    # 1) C-major scale over USB-MIDI (vel 100) on ch1's patch
    for n in (60, 62, 64, 65, 67, 69, 71, 72):
        m.note(n, 100, 0.22); time.sleep(0.05)
    time.sleep(0.3)
    # 2) hold a C-major chord (C/E/G) over USB-MIDI
    for n in (60, 64, 67):
        m.note_on(n, 100)
    time.sleep(1.5)
    for n in (60, 64, 67):
        m.note_off(n)
    # 3) pause
    time.sleep(1.0)
    # 4) reset, then a sustained 3-osc chord (A4/C#5/E5) via zP
    m.sysex("zPimport amy; amy.reset()Z")
    time.sleep(0.5)
    m.sysex("zPimport amy; amy.send(osc=0,wave=amy.SINE,freq=440,vel=0.3); "
            "amy.send(osc=1,wave=amy.SINE,freq=554,vel=0.3); "
            "amy.send(osc=2,wave=amy.SINE,freq=659,vel=0.3)Z")
    time.sleep(2.0)
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


# ── AMYboard World sketch suite ──────────────────────────────────────────────
def run_world_suite(args, m):
    """Download each WORLD_SUITE sketch, push it onto the board, drive it with the
    simple MIDI pattern, and record it. Returns [(name, recording|None, min_sim)];
    a None recording means fetch/load failed for that sketch (a test failure, but
    the run continues so the other sketches + artifacts are still produced)."""
    results = []
    for spec in WORLD_SUITE:
        name, author = spec["name"], spec["author"]
        min_sim = (args.world_min_similarity if args.world_min_similarity is not None
                   else spec.get("min_sim", 0.80))
        print(f"\n[world] === {author}/{name} (generative={spec['generative']}) ===")
        try:
            text = world_fetch_sketch(name, author, base=args.world_base)
        except Exception as e:
            print(f"[world] fetch failed for {author}/{name}: {e}")
            results.append((name, None, min_sim))
            continue
        print(f"[world] fetched {len(text)} bytes; transferring + starting sketch")
        try:
            m.load_world_sketch(text)
            rec = record_and_drive(args, args.world_duration,
                                   lambda: m.play_pattern(args.world_duration))
        except Exception as e:
            print(f"[world] load/record failed for {author}/{name}: {e}")
            results.append((name, None, min_sim))
            continue
        results.append((name, rec, min_sim))
    return results


# ── main ────────────────────────────────────────────────────────────────────
def main():
    ap = argparse.ArgumentParser(description="AMYboard hardware-in-the-loop test")
    ap.add_argument("--list-devices", action="store_true")
    ap.add_argument("--pr", type=int); ap.add_argument("--url"); ap.add_argument("--firmware")
    ap.add_argument("--port", help="esptool serial port (the USB-serial dongle)")
    ap.add_argument("--baud", type=int, default=921600)
    ap.add_argument("--boot-attempts", type=int, default=3,
                    help="chip-id reset + boot tries before failing. Retries the "
                         "bench's intermittent boot-wedge (a real boot failure fails "
                         "every try).")
    ap.add_argument("--boot-settle", type=float, default=5.0,
                    help="seconds to wait after a chip-id reset for the board to "
                         "boot and enumerate USB-MIDI before looking for it.")
    ap.add_argument("--no-flash", action="store_true")
    ap.add_argument("--flash-only", action="store_true")
    ap.add_argument("--midi-port", help="ALSA hw:X,Y or mido name (auto if omitted)")
    ap.add_argument("--audio-device", default="hw:1,0" if ALSA else None,
                    help="ALSA hw:X,Y or sounddevice index/name")
    ap.add_argument("--samplerate", type=int, default=48000)
    ap.add_argument("--channels", type=int, default=1)
    ap.add_argument("--duration", type=float, default=10.0)
    ap.add_argument("--name", default="hwci_basic")
    ap.add_argument("--reference"); ap.add_argument("--update-reference", action="store_true")
    ap.add_argument("--min-similarity", type=float, default=0.90,
                    help="min spectral cosine similarity to pass (0..1)")
    ap.add_argument("--min-level-db", type=float, default=-30.0,
                    help="min recording RMS level (dB) to pass")
    ap.add_argument("--out")
    ap.add_argument("--debug-port", default=None,
                    help="serial debug UART to log (ESP-IDF console/stderr); "
                         "default: --port, else /dev/ttyACM1 on Linux")
    ap.add_argument("--cdc-port", default="/dev/ttyACM0" if ALSA else None,
                    help="board native CDC to log (MicroPython stdout)")
    ap.add_argument("--debug-baud", type=int, default=115200)
    ap.add_argument("--serial-log", default=None,
                    help="combined serial log path (default: {name}-serial.log)")
    ap.add_argument("--no-serial-log", action="store_true",
                    help="don't capture the serial console")
    ap.add_argument("--no-world", dest="world", action="store_false",
                    help="skip the AMYboard World sketch suite (basic tones only)")
    ap.set_defaults(world=True)
    ap.add_argument("--world-duration", type=float, default=10.0,
                    help="seconds to record each World sketch")
    ap.add_argument("--world-min-similarity", type=float, default=None,
                    help="override the per-sketch min spectral similarity for the "
                         "whole World suite (default: each sketch's own threshold)")
    ap.add_argument("--world-base", default=WORLD_BASE,
                    help="AMYboard World API base URL")
    args = ap.parse_args()
    print(f"[hwci] backend: {'ALSA cli' if ALSA else 'python libs'}")

    if args.list_devices:
        list_devices(); return 0

    if not args.no_flash and not args.port:
        sys.exit("--port (the USB-serial dongle) is required to flash.")
    bin_path, tmp = resolve_firmware(args) if not args.no_flash else (None, False)

    # We deliberately do NOT hold the debug UART open while the board boots: the
    # proven-good reset on this bench is `esptool chip-id` (hard-reset + release
    # the port), wait a few seconds, then `amidi -l` -- holding the dongle's
    # DTR/RTS lines across boot is what left the board wedged in a boot-loop. The
    # consoles are tailed only *after* MIDI is up.
    debug_port = args.debug_port or args.port or ("/dev/ttyACM1" if ALSA else None)
    loggers = []
    try:
        # Flash once, then bring the board up with a clean esptool chip-id reset
        # cycle. A plain flash hard-reset can leave it wedged in a boot-loop on
        # this bench; a chip-id cycle reliably boots it. Retries do the (cheap)
        # reset again, not a re-flash -- the firmware is already written + verified.
        if not args.no_flash:
            flash(args.port, bin_path, args.baud)
            if args.flash_only:
                print("[done] flashed."); return 0
        for attempt in range(1, args.boot_attempts + 1):
            if args.port:
                reset_via_chipid(args.port)
            time.sleep(args.boot_settle)   # board boots + USB-MIDI enumerates
            args.midi_port = args.midi_port or wait_for_board()
            if args.midi_port:
                break
            if attempt < args.boot_attempts:
                print(f"[boot] attempt {attempt}/{args.boot_attempts}: MIDI not up; "
                      "re-resetting (chip-id) and retrying")
        else:
            # Real boot failure -- grab a short debug-UART snapshot so it's still
            # debuggable (the only time we open the dongle before MIDI is up).
            if not args.no_serial_log:
                snap = start_serial_log(debug_port, args.debug_baud,
                                        "DEBUG UART (post-failure snapshot)")
                if snap:
                    loggers.append(snap)
                    time.sleep(3.0)
            sys.exit(f"[boot] board never enumerated MIDI after {args.boot_attempts} "
                     "chip-id reset attempt(s) — see serial log.")

        time.sleep(1.5)      # let the MIDI endpoint + synth settle post-boot
        if not args.no_serial_log:
            # CDC is the board's own USB (no reset lines), so opening it mid-run
            # can't perturb the board or the recording. We skip the debug UART
            # here for the same reason -- its log only matters on boot failure
            # (captured in the snapshot above).
            cdc = start_serial_log(args.cdc_port, 115200, "CDC (MicroPython stdout)")
            if cdc:
                loggers.append(cdc)
        if not args.audio_device:
            sys.exit("--audio-device required (see --list-devices).")
        # Record everything while the serial consoles are tailed and the board is
        # up: first the built-in reference tones, then (unless --no-world) each
        # AMYboard World sketch. results = [(name, recording|None, min_sim)].
        m = Midi(args.midi_port)
        results = []
        try:
            rec = record_and_drive(args, args.duration, lambda: run_test_sequence(m))
            results.append((args.name, rec, args.min_similarity))
            if args.world:
                results += run_world_suite(args, m)
                m.silence()   # leave the board quiet for the Tulip test that follows
        finally:
            m.close()
    finally:
        if loggers:
            write_serial_logs(loggers, args.serial_log or f"{args.name}-serial.log")
        if tmp and bin_path:
            os.unlink(bin_path)

    # Per-test: save the recording, then either capture the reference
    # (--update-reference) or spectral-compare against it. The run passes only if
    # every test passes.
    overall_ok = True
    for name, rec, min_sim in results:
        out_wav = (args.out if name == args.name and args.out else f"{name}-recording.wav")
        if rec is None:                       # fetch/load failed in the World suite
            print(f"\n{name}: FAIL (no recording — sketch fetch/load error)")
            overall_ok = False
            continue
        write_wav_mono(out_wav, rec, args.samplerate)
        print(f"[audio] saved {out_wav}")
        ref_path = (args.reference if name == args.name and args.reference
                    else os.path.join(REF_DIR, f"{name}.wav"))
        if args.update_reference:
            os.makedirs(REF_DIR, exist_ok=True)
            write_wav_mono(ref_path, rec, args.samplerate)
            print(f"[ref] wrote {ref_path} — LISTEN to confirm it's correct.")
            continue
        if not os.path.exists(ref_path):
            print(f"\n{name}: FAIL — no reference at {ref_path}; run once with "
                  "--update-reference.")
            overall_ok = False
            continue
        ref, _ = read_wav_mono(ref_path)
        sim, level_db = compare(rec, ref)
        ok = sim >= min_sim and level_db >= args.min_level_db
        print(f"\n{name}: spectral_similarity={sim:.4f} (min {min_sim})  "
              f"level={level_db:.1f}dB (min {args.min_level_db})  ->  {'PASS' if ok else 'FAIL'}")
        overall_ok = overall_ok and ok

    if args.update_reference:
        return 0
    return 0 if overall_ok else 1


if __name__ == "__main__":
    sys.exit(main())
