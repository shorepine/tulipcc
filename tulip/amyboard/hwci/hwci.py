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

While the tests run, AMY's render load (`tulip.amy_render_load()`, 0..1 —
amy#826 / PR #1105) is polled once a second over `zP`; each sample prints an
`hwci_load` line on MicroPython stdout (the CDC log), and the mean/max are
reported next to the pass/fail results. Firmware without the binding reports
"not supported". Informational only — it does not gate pass/fail.

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
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
import urllib.request
import wave

HERE = os.path.dirname(os.path.abspath(__file__))

# Board control (SysEx protocol, MIDI transports, World API) comes from the
# shared tools/amyboardctl library — see docs/amyboard/control_api.md. In the
# repo it's found relative to this file; on the CI runner the workflow extracts
# the amyboardctl package next to hwci.py (see amyboard-hwci.yml).
try:
    import amyboardctl
except ImportError:
    sys.path.insert(0, os.path.join(HERE, "..", "..", "..", "tools", "amyboardctl"))
    import amyboardctl
from amyboardctl import AMYboardLink, board_present, world

REF_DIR = os.path.join(HERE, "ref")
PREVIEW_URL = "https://amyboard-pr-{pr}.vercel.app/firmware/amyboard-full-AMYBOARD.bin"
MIDI_NAME = "AMYboard"
ALSA = bool(shutil.which("amidi") and shutil.which("arecord"))
BACKEND = "alsa" if ALSA else "mido"   # keep the Pi on the CLI tools (no mido needed)

# AMYboard World: the second half of the test loads real community sketches over
# the SysEx "write to sketch" flow (the same one the web editor uses) and records
# each one, so the HW CI exercises more than the built-in reference tones. See
# docs/amyboard/control_api.md and tools/amyworld_recorder/. PR #1067 was a drums
# regression the tone-only test missed — the drum-heavy generators below cover it.
WORLD_BASE = world.DEFAULT_BASE

# The World sketches to audition, in order. `generative` sketches drive themselves
# from loop() (started by the sequencer that environment_transfer_done() kicks off)
# and use random() — so run-to-run they differ in the notes but not the timbre, and
# the timing/phase-invariant avg-spectrum compare still matches. `woodpiano` is a
# bare DX7 patch that only sounds when we send it MIDI notes. Only non-generative
# sketches get the simple MIDI pattern (which also proves the USB-MIDI note path);
# generative sketches are recorded as-is so the reference is purely their own output.
WORLD_SUITE = [
    {"name": "acid_generator",  "author": "shorepine", "generative": True,  "min_sim": 0.80},
    {"name": "house_generator", "author": "shorepine", "generative": True,  "min_sim": 0.80},
    {"name": "woodpiano",       "author": "shorepine", "generative": False, "min_sim": 0.85},
]


# ── bench lock ───────────────────────────────────────────────────────────────
# shorepine/amy's "AMY HW CI" + loadsweep sweeps share this physical bench (same
# board, same dongle, same summed capture card) from a SEPARATE runner
# registration, so GitHub never serializes the two repos' jobs. Every harness
# that touches the bench takes this flock for its whole run — same path and
# semantics as amy's tools/arduino_loadsweep/measure.py.
BENCH_LOCK = "/tmp/amyboard-bench.lock"


def acquire_bench(port, timeout_s=900):
    """Serialize bench access across processes/sessions.

    Holds an flock on BENCH_LOCK for the rest of this process (the caller keeps
    the returned fd alive) AND waits for the serial port itself to be free —
    tty devices are not exclusive, so a lock alone doesn't protect against
    harnesses that don't take it."""
    import fcntl
    fd = os.open(BENCH_LOCK, os.O_CREAT | os.O_RDWR, 0o666)
    deadline = time.time() + timeout_s
    waited = False
    while True:
        got_lock = False
        try:
            fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
            got_lock = True
        except OSError:
            pass
        holders = ""
        if port:
            try:
                holders = subprocess.run(
                    ["lsof", "-t", port, port.replace("/cu.", "/tty.")],
                    capture_output=True, text=True).stdout.strip()
            except FileNotFoundError:      # no lsof on this bench: flock only
                pass
        if got_lock and not holders:
            if waited:
                print("[bench] free, proceeding")
            return fd   # keep open: lock is held until process exit
        if got_lock:
            fcntl.flock(fd, fcntl.LOCK_UN)
        if time.time() > deadline:
            sys.exit(f"[bench] {port or BENCH_LOCK} still busy after {timeout_s}s "
                     f"(lock={'ok' if got_lock else 'held'}, "
                     f"holders={holders or 'none'})")
        if not waited:
            print(f"[bench] waiting for {port or BENCH_LOCK} "
                  f"(lock={'ok' if got_lock else 'held elsewhere'}, "
                  f"holder pids={holders or '?'})")
            waited = True
        time.sleep(5.0)


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


# ── MIDI / sysex (amyboardctl wraps both backends) ──────────────────────────
class Midi:
    """MIDI to the board, via amyboardctl's AMYboardLink.

    The link speaks the SysEx control API with flow control — one frame in
    flight, waiting for the board's `AK` after each — so multi-frame sketch
    transfers can't overflow the board's SysEx ring buffer (a dropped chunk
    corrupts sketch.py → the board self-heals to the silent default sketch).
    Backend per bench: ALSA CLI (`amidi`) on the Pi CI runner, mido on macOS
    dev machines. This class only adds the test's own stimulus timing on top.
    """
    ACK_TIMEOUT = 2.0          # wait this long for the board's AK, then proceed

    def __init__(self, port):
        self.link = AMYboardLink(port_match=port, verbose=False, backend=BACKEND,
                                 ack_timeout=self.ACK_TIMEOUT).open()

    def note_on(self, n, vel=100, channel=0):
        self.link.note_on(n, vel, channel)

    def note_off(self, n, channel=0):
        self.link.note_off(n, channel)

    def all_notes_off(self, channel=0):
        self.link.all_notes_off(channel)

    def note(self, n, vel, dur):
        self.link.note(n, vel, dur)

    def sysex(self, code):
        """Fire-and-forget one control frame (used by the built-in reference test,
        whose frames are small and few — its committed ref/hwci_basic.wav depends
        on this timing, so leave it alone)."""
        self.link.send_nowait(code)
        time.sleep(0.02)

    def send_python(self, code, timeout=ACK_TIMEOUT):
        """Run one line of python on the board via zP (control_api.md)."""
        return self.link.send_python(code, timeout=timeout)

    # AMY render load (0..1, ~1.0 = overloaded), tulip.amy_render_load() from
    # amy#826 / PR #1105. getattr-guarded so firmware without the binding
    # prints -1 instead of pushing a traceback frame every poll.
    LOAD_POLL_CODE = ("print('hwci_load %.4f' % getattr(__import__('tulip'), "
                      "'amy_render_load', lambda: -1.0)())")

    def poll_load_start(self, interval=1.0):
        """Poll the render load once a second for the rest of the run. Each
        sample prints an `hwci_load <v>` line on MicroPython stdout, i.e. into
        the CDC serial log; render_load_report() averages them at the end.
        send_python serializes on the link's lock, so polls interleave safely
        between other control frames — but they're paused during sketch
        transfers anyway (load_world_sketch) to keep multi-frame timing tight."""
        self._poll_stop = threading.Event()
        self._poll_pause = threading.Event()

        def _poll():
            while not self._poll_stop.wait(interval):
                if self._poll_pause.is_set():
                    continue
                try:
                    self.link.send_python(self.LOAD_POLL_CODE, timeout=1.0)
                except Exception:
                    pass   # board busy/rebooting; keep polling

        self._poll_thread = threading.Thread(target=_poll, daemon=True)
        self._poll_thread.start()

    def poll_load_stop(self):
        if getattr(self, "_poll_thread", None):
            self._poll_stop.set()
            self._poll_thread.join(timeout=3)
            self._poll_thread = None

    def load_world_sketch(self, text, settle=1.5):
        """Silence the PREVIOUS sketch first, then amy.reset() → transfer →
        environment_transfer_done() (restarts the sketch and starts the
        sequencer, so generative loop()s begin playing). The old sketch's
        loop() keeps executing during the transfer and can re-arm sequencer
        events after our amy.reset(), so its audio bleeds into the next
        recording (heard as house_generator under woodpiano) unless the
        transport is stopped and the loop() killed before transferring.
        Give the new sketch a moment to init before we start recording it."""
        if getattr(self, "_poll_pause", None):
            self._poll_pause.set()   # no load polls between transfer frames
        try:
            try:
                self.sysex("zY0Z")                   # stop sequencer transport
                self.send_python("import amyboard; amyboard.stop_sketch()")  # kill loop()
                self.all_notes_off(0)
            except Exception:
                pass
            self.link.reset_amy()
            time.sleep(0.3)
            # An unACKed transfer means the sketch may not actually be on the
            # board — recording it anyway just measures 10s of silence with a
            # clean log (the 2026-07-08 woodpiano incident). Retry once, then
            # fail this sketch loudly instead of recording nothing.
            for attempt in (1, 2):
                sent, acked = self.link.transfer_file(text)
                if acked == sent:
                    break
                print(f"[world] TRANSFER NOT ACKED ({acked}/{sent} frames), "
                      f"attempt {attempt}/2")
                if attempt == 2:
                    raise RuntimeError(
                        f"sketch transfer failed: {acked}/{sent} frames acked")
                time.sleep(1.0)
            self.link.environment_transfer_done()
            time.sleep(settle)
        finally:
            if getattr(self, "_poll_pause", None):
                self._poll_pause.clear()

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
        self.link.close()


def wait_for_board(timeout=40):
    """Wait for the board's USB-MIDI to (re)enumerate; return its port, or None
    if it doesn't appear within `timeout` (so the caller can reset + retry)."""
    print(f"[boot] waiting for '{MIDI_NAME}' MIDI ...")
    deadline = time.time() + timeout
    while time.time() < deadline:
        port = board_present(MIDI_NAME, backend=BACKEND)
        if port:
            print(f"[boot] found MIDI at {port}")
            return port
        time.sleep(0.5)
    print("[boot] board MIDI never appeared (didn't boot?).")
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

    def _open(self, serial, window_s):
        ser = None
        deadline = time.monotonic() + window_s
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
        return ser

    def _run(self):
        try:
            import serial  # pyserial (ships with esptool)
        except ImportError:
            print(f"[log] pyserial missing; cannot capture {self.port}")
            return
        first = True
        while not self._stop.is_set():
            # A read error mid-run usually means the board re-enumerated its
            # USB (reboot / self-heal). Dying silently here truncated the log
            # with no trace (the 2026-07-08 woodpiano incident) — instead say
            # so, mark the log, and reopen so the post-reboot console (boot
            # log, tracebacks) is captured too. 15s covers a re-enumeration.
            ser = self._open(serial, 8.0 if first else 15.0)
            if ser is None:
                if self._stop.is_set():
                    return
                if first:
                    print(f"[log] could not open {self.port} ({self.label})")
                else:
                    print(f"[log] {self.port} ({self.label}) did not come back; "
                          "capture stops here")
                    self.buf.extend(b"\n===== [log] port did not come back; "
                                    b"capture truncated =====\n")
                return
            self.opened = True
            if not first:
                print(f"[log] {self.port} ({self.label}) reopened")
                self.buf.extend(b"\n===== [log] reopened after read error =====\n")
            first = False
            while not self._stop.is_set():
                try:
                    data = ser.read(4096)
                except Exception as e:
                    print(f"[log] {self.port} ({self.label}) read error: {e}; "
                          "reopening")
                    self.buf.extend(f"\n===== [log] read error: {e}; "
                                    "reopening =====\n".encode())
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


# ── piano polyphony probe (diagnostic, not pass/fail) ───────────────────────
# Chases a reported artifact: with patch 256 (piano) at num_voices=8, a periodic
# crackle appears once the 8th note is held. This records the note-by-note ramp
# to 8 held voices and then measures the recording for periodic broadband
# transients, so the *period* is a number in the CI log rather than an ear
# judgement. Diagnostic only — it has no committed reference and never gates the
# run's pass/fail.
def setup_piano_poly(m, patch=256, voices=8, no_cv=False):
    """Put the board in the reported state: bare piano synth, N voices.

    Also arms AMY's CPU-overload callback. That failsafe is the prime suspect:
    it resets all notes/oscs and plays a 4-note descending bleep spaced
    i*160ms (amy api.c amy_overload_failsafe), which would read as periodic
    transients ~160ms apart. Its fprintf goes to the ESP-IDF console on the
    debug UART, which we deliberately don't hold open mid-run (the dongle's
    DTR/RTS would reset the board), so route it to MicroPython stdout instead
    -- that lands in the CDC log we do capture. It also zeroes render_us on the
    way out, which is why 1Hz load polling reads low."""
    m.sysex("zPimport amy; amy.reset()Z")
    time.sleep(0.5)
    try:
        m.send_python("import tulip; tulip.amy_overload_callback("
                      "lambda l: print('hwci_overload %d' % l))")
    except Exception as e:
        print(f"[piano8] could not arm overload callback: {e}")
    time.sleep(0.2)
    if no_cv:
        # cv_read_task (amyboard_support.c) does TWO blocking ADS1015 I2C reads
        # every 6 RTOS ticks -- 6.000ms against a 5.805ms audio block, two
        # near-equal periods that beat slowly. tulip.cv_local(ch, v) sets
        # cv_local_override[ch], which makes the task skip read_ads1015_raw()
        # entirely while still running and delaying. So this removes ~344 I2C
        # transactions/sec and changes nothing else -- a clean A/B for "is a
        # periodic task stealing time from the render task?".
        try:
            m.send_python("import tulip; tulip.cv_local(0, 0.0); tulip.cv_local(1, 0.0)")
            print("[piano8] CV local override ON: cv_read_task now does NO I2C")
        except Exception as e:
            print(f"[piano8] could not set cv_local override: {e}")
        time.sleep(0.2)
    m.sysex(f"zPimport amy; amy.send(synth=1, patch={patch}, num_voices={voices})Z")
    time.sleep(1.5)   # patch 256 pulls its PCM from flash


def run_piano_poly_sequence(m, marks, notes, step_s, hold_s):
    """Add `notes` one at a time (never releasing), hold them all, then a
    simultaneous re-strike of the same chord, then release.

    The ramp is the reported repro. The re-strike is the useful control: on the
    ramp the earliest notes have been decaying for seconds, so "8 held" may not
    mean 8 *loud* voices — striking all 8 at once guarantees it does. If the
    artifact tracks the re-strike too, it follows active voice count rather than
    time-since-note-on.

    `marks` collects (seconds-from-drive-start, label) so the analysis can window
    on "all 8 held" instead of guessing. Drive starts ~0.3s into the recording
    (record_and_drive's lead-in), which is added back by the caller."""
    t0 = time.time()

    def mark(label):
        t = time.time() - t0
        marks.append((t, label))
        print(f"[piano8] t={t:6.3f}s  {label}")

    mark("drive start (silence baseline)")
    time.sleep(0.5)
    for i, n in enumerate(notes, 1):
        m.note_on(n, 100)
        mark(f"note_on {n} -> {i} held")
        time.sleep(step_s)
    mark(f"ALL {len(notes)} HELD - hold {hold_s}s")
    time.sleep(hold_s)
    mark("release all")
    for n in notes:
        m.note_off(n)
    time.sleep(0.4)
    # Control: all 8 struck together, so every voice is at full amplitude.
    mark(f"re-strike all {len(notes)} together")
    for n in notes:
        m.note_on(n, 100)
    time.sleep(hold_s)
    mark("release all (re-strike)")
    for n in notes:
        m.note_off(n)
    m.all_notes_off(0)
    time.sleep(0.3)


def analyze_transients(x, sr, t0=0.0, t1=None, hop_ms=1.0, win_ms=4.0,
                       sigma=6.0, refractory_ms=25.0,
                       period_lo_ms=20.0, period_hi_ms=1000.0):
    """Find broadband transients in x[t0:t1] and estimate their period.

    A crackle is an impulse: broadband and phase-discontinuous, unlike the tonal
    piano behind it. Taking the 2nd difference suppresses the low-frequency tone
    and leaves the impulses, then a short-frame RMS envelope of that makes them
    peaks. The threshold is median + sigma*MAD (robust — a few big clicks don't
    inflate their own floor). Period comes from the envelope's autocorrelation,
    which finds a regular spacing even when individual peaks fall under the
    threshold. Returns None if the window is too short to judge."""
    import numpy as np
    a = int(max(0, t0) * sr)
    b = int(t1 * sr) if t1 is not None else len(x)
    seg = np.asarray(x[a:min(b, len(x))], dtype=np.float64)
    if len(seg) < int(0.10 * sr):
        return None
    d = np.diff(seg, n=2)                      # kill the tone, keep the clicks
    hop = max(1, int(sr * hop_ms / 1000.0))
    win = max(hop, int(sr * win_ms / 1000.0))
    nfr = 1 + (len(d) - win) // hop
    if nfr < 16:
        return None
    # Framed RMS via cumulative sums — O(n) instead of a Python loop per frame.
    p = np.concatenate(([0.0], np.cumsum(d * d)))
    idx = np.arange(nfr) * hop
    env = np.sqrt(np.maximum(p[idx + win] - p[idx], 0.0) / win)
    med = float(np.median(env))
    mad = float(np.median(np.abs(env - med)))
    thr = med + sigma * 1.4826 * mad + 1e-12
    # Peak-pick with a refractory window so one click isn't counted many times.
    refr = max(1, int(refractory_ms / hop_ms))
    peaks, i = [], 0
    while i < nfr:
        if env[i] > thr:
            j = min(nfr, i + refr)
            k = i + int(np.argmax(env[i:j]))
            peaks.append(t0 + k * hop_ms / 1000.0)
            i = k + refr
        else:
            i += 1
    iois = [round((b_ - a_) * 1000.0, 1) for a_, b_ in zip(peaks, peaks[1:])]
    e = env - env.mean()
    ac = np.correlate(e, e, mode="full")[len(e) - 1:]
    ac = ac / (ac[0] + 1e-20)
    lo, hi = int(period_lo_ms / hop_ms), min(len(ac) - 1, int(period_hi_ms / hop_ms))
    period_ms = ac_peak = None
    if hi > lo + 1:
        lag = lo + int(np.argmax(ac[lo:hi]))
        period_ms, ac_peak = lag * hop_ms, float(ac[lag])
    return {
        "window": (round(t0, 3), round((b / sr), 3)),
        "n_clicks": len(peaks),
        "fold": fold_period(peaks),
        "click_times": [round(t, 3) for t in peaks[:40]],
        "iois_ms": iois[:40],
        "median_ioi_ms": (round(float(np.median(iois)), 1) if iois else None),
        "ac_period_ms": period_ms,
        "ac_strength": (round(ac_peak, 3) if ac_peak is not None else None),
        "rms_db": round(20 * np.log10(float(np.sqrt(np.mean(seg ** 2))) + 1e-12), 1),
        "crest_db": round(20 * np.log10((float(np.max(np.abs(seg))) + 1e-12) /
                                        (float(np.sqrt(np.mean(seg ** 2))) + 1e-12)), 1),
    }


def narrowband_report(x, sr, f0, t0=0.0, t1=None, hop_ms=4.0, win_ms=64.0):
    """Track energy at exactly f0 Hz over time, and fold any bursts.

    Used to test for AMY's CPU-overload bleep, whose four sines are A5 880,
    E5 659.26, C5 523.25 and A4 440 Hz spaced 160ms apart. The probe chord is
    C-E-G, so 440 and 880 appear in neither the notes nor their harmonics
    (C3 130.8 -> 261.6/392.4/523.2, E3 164.8 -> 329.6/494.4/659.2,
    G3 196 -> 392/588/784) -- narrowband energy there is therefore evidence of
    the bleep rather than of the piano. 523.25/659.26 ARE in the chord and
    prove nothing, so don't test those.

    Returns dB of the f0 bin above the median of its neighbourhood, plus a fold
    of the f0 envelope's peaks."""
    import numpy as np
    a = int(max(0, t0) * sr)
    b = int(t1 * sr) if t1 is not None else len(x)
    seg = np.asarray(x[a:min(b, len(x))], dtype=np.float64)
    win = int(sr * win_ms / 1000.0)
    hop = max(1, int(sr * hop_ms / 1000.0))
    if len(seg) < win * 2:
        return None
    w = np.hanning(win)
    k = 2 * np.pi * f0 / sr
    ref = np.exp(-1j * k * np.arange(win)) * w      # single-bin DFT (Goertzel-ish)
    nfr = 1 + (len(seg) - win) // hop
    env = np.array([np.abs(np.dot(seg[i * hop:i * hop + win], ref)) for i in range(nfr)])
    env = env / (win / 2.0)
    # Neighbourhood: same measurement a few semitones away, avoiding f0's own
    # partials, as the "is this a real peak" baseline.
    # Neighbourhood envelopes, measured the same way. Compare peak-to-PEAK and
    # median-to-median: an earlier version compared max-at-f0 against
    # median-at-neighbour, which any broadband transient inflates (a click has
    # energy at every frequency, so it raises f0's max while the neighbours'
    # median stays low). That produced false "bleep present" readings on
    # recordings that were simply full of clicks. Same-statistic comparison
    # makes broadband energy cancel, so a positive peak ratio means genuinely
    # TONAL energy at f0.
    nb_env = []
    for fo in (f0 * 0.87, f0 * 0.93, f0 * 1.07, f0 * 1.14):
        r = np.exp(-1j * 2 * np.pi * fo / sr * np.arange(win)) * w
        nb_env.append(np.array([np.abs(np.dot(seg[i * hop:i * hop + win], r))
                                for i in range(nfr)]) / (win / 2.0))
    nb_peak = float(np.median([float(np.max(e)) for e in nb_env])) + 1e-12
    nb_med = float(np.median([float(np.median(e)) for e in nb_env])) + 1e-12
    peak_db = 20 * np.log10((float(np.max(env)) + 1e-12) / nb_peak)
    med_db = 20 * np.log10((float(np.median(env)) + 1e-12) / nb_med)
    # Peaks in the f0 envelope, then fold them for a period.
    med, mad = float(np.median(env)), float(np.median(np.abs(env - np.median(env))))
    thr = med + 6.0 * 1.4826 * mad + 1e-12
    refr = max(1, int(60.0 / hop_ms))
    pk, i = [], 0
    while i < nfr:
        if env[i] > thr:
            j = min(nfr, i + refr)
            c = i + int(np.argmax(env[i:j]))
            pk.append(t0 + c * hop_ms / 1000.0)
            i = c + refr
        else:
            i += 1
    return {"f0": f0, "peak_db_over_neighbourhood": round(peak_db, 1),
            "median_db_over_neighbourhood": round(med_db, 1),
            "n_bursts": len(pk), "burst_times": [round(t, 3) for t in pk[:20]],
            "fold": fold_period(pk)}


def report_narrowband(label, r):
    if r is None:
        print(f"[piano8] {label}: window too short for narrowband test")
        return
    verdict = ("PRESENT" if r["peak_db_over_neighbourhood"] > 12.0 and r["n_bursts"] >= 3
               else "not present")
    print(f"[piano8] {label}: {r['f0']:.0f}Hz peak={r['peak_db_over_neighbourhood']}dB "
          f"median={r['median_db_over_neighbourhood']}dB over neighbourhood, "
          f"bursts={r['n_bursts']}  ->  {verdict}")
    if r["fold"]:
        print(f"[piano8] {label}: {r['f0']:.0f}Hz fold period="
              f"{r['fold']['period_ms']}ms (coherence {r['fold']['coherence']}) "
              f"gaps={r['fold']['gaps_ms'][:12]}")
    if r["burst_times"]:
        print(f"[piano8] {label}: {r['f0']:.0f}Hz burst times = {r['burst_times']}")


def fold_period(click_times, gap_ms=60.0, lo_ms=60.0, hi_ms=400.0):
    """Group clicks into bursts, then fit ONE period to the burst onsets.

    Bench data showed the raw median inter-click interval understates the true
    recurrence: each event is a burst of 1-2 clicks a few tens of ms apart, so
    the median mixes within-burst and between-burst gaps (131ms median for a
    167ms recurrence). Clustering first, then fitting a single period by
    maximizing phase coherence, recovers it — and tolerates dropped bursts,
    since a missed one just leaves a 2x gap. Returns None if too few bursts."""
    import numpy as np
    if len(click_times) < 4:
        return None
    ct = np.asarray(sorted(click_times), dtype=float)
    bursts = [[ct[0]]]
    for t in ct[1:]:
        if t - bursts[-1][-1] <= gap_ms / 1000.0:
            bursts[-1].append(t)
        else:
            bursts.append([t])
    onsets = np.array([b[0] for b in bursts])
    if len(onsets) < 4:
        return None
    best_p, best_r = None, -1.0
    for p in np.arange(lo_ms / 1000.0, hi_ms / 1000.0, 0.0005):
        ph = 2 * np.pi * (((onsets - onsets[0]) / p) % 1.0)
        r = float(np.abs(np.mean(np.exp(1j * ph))))
        if r > best_r:
            best_p, best_r = p, r
    gaps = np.diff(onsets) * 1000.0
    return {
        "n_bursts": len(bursts),
        "burst_sizes": sorted(set(len(b) for b in bursts)),
        "gaps_ms": np.round(gaps, 0).astype(int).tolist()[:40],
        "median_gap_ms": round(float(np.median(gaps)), 1),
        "period_ms": round(best_p * 1000.0, 1),
        "coherence": round(best_r, 3),
        "gap_multiples": np.round(gaps / (best_p * 1000.0)).astype(int).tolist()[:40],
    }


def classify_clicks(x, sr, click_times, n=6, pre_ms=12.0, post_ms=12.0):
    """Characterise the glitch WAVEFORM, which discriminates mechanisms that
    timing alone cannot:

      * a run of near-zero samples  -> the DAC/I2S was starved (underrun fed
        silence); length tells you how many blocks were missed
      * a repeated chunk            -> the I2S driver replayed the last DMA
        buffer, the classic underrun-with-repeat
      * a bare step discontinuity   -> a sample was dropped or state was reset
        mid-waveform, no starvation

    Reported per click: the largest single-sample jump vs the local typical
    jump, the longest near-silent run, and the best correlation between the
    post-click window and the block-sized chunk before it (a repeat scores
    near 1.0 at a lag of one AMY block)."""
    import numpy as np
    out = []
    blk = int(round(256 * sr / 44100.0))   # one AMY block at 256/44100, in capture samples
    for t in click_times[:n]:
        c = int(t * sr)
        a, b = max(0, c - int(pre_ms * sr / 1000)), min(len(x), c + int(post_ms * sr / 1000))
        if b - a < 32:
            continue
        seg = np.asarray(x[a:b], dtype=np.float64)
        d = np.abs(np.diff(seg))
        step_ratio = float(np.max(d)) / (float(np.median(d)) + 1e-12)
        # longest run of samples below 2% of the segment's own RMS
        rms = float(np.sqrt(np.mean(seg ** 2))) + 1e-12
        quiet = np.abs(seg) < 0.02 * rms
        best = cur = 0
        for q in quiet:
            cur = cur + 1 if q else 0
            best = max(best, cur)
        # does the material just after the click repeat the block before it?
        rep = None
        if c - blk - a >= 0 and b - c >= blk:
            prev = np.asarray(x[c - blk:c], dtype=np.float64)
            post = np.asarray(x[c:c + blk], dtype=np.float64)
            if np.std(prev) > 1e-9 and np.std(post) > 1e-9:
                rep = float(np.corrcoef(prev, post)[0, 1])
        out.append({"t": round(t, 3), "step_ratio": round(step_ratio, 1),
                    "zero_run_ms": round(1000.0 * best / sr, 2),
                    "repeat_corr": (round(rep, 3) if rep is not None else None)})
    return out


def report_click_shapes(label, shapes, sr):
    if not shapes:
        return
    import numpy as np
    zr = [s["zero_run_ms"] for s in shapes]
    rc = [s["repeat_corr"] for s in shapes if s["repeat_corr"] is not None]
    sr_ratio = [s["step_ratio"] for s in shapes]
    verdict = "step discontinuity (no starvation)"
    if max(zr) > 1.0:
        verdict = f"SILENCE GAP up to {max(zr):.2f}ms (~{max(zr) / 5.805:.1f} AMY blocks)"
    elif rc and max(rc) > 0.9:
        verdict = f"REPEATED BLOCK (corr {max(rc):.3f}) -- I2S replayed a DMA buffer"
    print(f"[piano8] {label}: click shapes -> {verdict}")
    print(f"[piano8] {label}:   median step_ratio={np.median(sr_ratio):.1f}x  "
          f"max zero_run={max(zr):.2f}ms  "
          f"max repeat_corr={max(rc) if rc else 'n/a'}")
    for s in shapes:
        print(f"[piano8]     t={s['t']}s step={s['step_ratio']}x "
              f"zero_run={s['zero_run_ms']}ms repeat_corr={s['repeat_corr']}")


def report_transients(label, r):
    """`clicks` + `median_IOI` are the evidence. autocorr_period is only
    meaningful alongside a nonzero click count — a clean tonal chord still
    autocorrelates strongly at its own beat period (~26ms in bench tests), so
    that number on its own is not a crackle."""
    if r is None:
        print(f"[piano8] {label}: window too short to analyze")
        return
    verdict = ("no periodic transients" if r["n_clicks"] < 3 else
               f"PERIODIC TRANSIENTS ~{r['median_ioi_ms']}ms apart")
    print(f"[piano8] {label}: window={r['window'][0]}..{r['window'][1]}s  "
          f"clicks={r['n_clicks']}  median_IOI={r['median_ioi_ms']}ms  "
          f"autocorr_period={r['ac_period_ms']}ms (strength {r['ac_strength']}; "
          f"only meaningful if clicks>0)  "
          f"rms={r['rms_db']}dB crest={r['crest_db']}dB  ->  {verdict}")
    if r["iois_ms"]:
        print(f"[piano8] {label}: inter-click intervals (ms) = {r['iois_ms']}")
    if r["click_times"]:
        print(f"[piano8] {label}: click times (s) = {r['click_times']}")
    f = r.get("fold")
    if f:
        print(f"[piano8] {label}: FOLDED -> {f['n_bursts']} bursts "
              f"(sizes {f['burst_sizes']})  period={f['period_ms']}ms "
              f"(phase coherence {f['coherence']})  median_gap={f['median_gap_ms']}ms")
        print(f"[piano8] {label}: burst gaps (ms) = {f['gaps_ms']}")
        print(f"[piano8] {label}: gaps as multiples of period = {f['gap_multiples']}")


# ── AMYboard World sketch suite ──────────────────────────────────────────────
def run_world_suite(args, m):
    """Download each WORLD_SUITE sketch, push it onto the board, and record it.
    Generative sketches drive themselves and are recorded as-is; only
    non-generative patch sketches (woodpiano) get the simple MIDI pattern, which
    also proves the USB-MIDI note path. Returns [(name, recording|None, min_sim)];
    a None recording means fetch/load failed for that sketch (a test failure, but
    the run continues so the other sketches + artifacts are still produced)."""
    results = []
    for spec in WORLD_SUITE:
        name, author = spec["name"], spec["author"]
        min_sim = (args.world_min_similarity if args.world_min_similarity is not None
                   else spec.get("min_sim", 0.80))
        print(f"\n[world] === {author}/{name} (generative={spec['generative']}) ===")
        try:
            text = world.fetch_sketch(name, author, base=args.world_base)
        except Exception as e:
            print(f"[world] fetch failed for {author}/{name}: {e}")
            results.append((name, None, min_sim))
            continue
        print(f"[world] fetched {len(text)} bytes; transferring + starting sketch")
        try:
            m.load_world_sketch(text)
            if spec["generative"]:
                rec = record_and_drive(args, args.world_duration, lambda: None)
            else:
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
    # Piano polyphony probe (diagnostic only — no reference, never gates pass/fail).
    ap.add_argument("--no-piano-poly", dest="piano_poly", action="store_false",
                    help="skip the patch-256 8-voice piano polyphony probe")
    ap.set_defaults(piano_poly=True)
    ap.add_argument("--piano-patch", type=int, default=256,
                    help="patch for the polyphony probe (default 256 = piano)")
    ap.add_argument("--piano-voices", type=int, default=8,
                    help="num_voices for the polyphony probe")
    ap.add_argument("--piano-notes", default="48,52,55,60,64,67,72,76",
                    help="the repro: 8 low/mid notes stacked one at a time")
    ap.add_argument("--piano-notes-7", default="48,52,55,60,64,67,72",
                    help="bisect set: 7 of the same low notes")
    ap.add_argument("--dx7-notes",
                    default="36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84",
                    help="25 notes for the osc-count-matched DX7 control (patch 253, "
                         "8 oscs/voice x 25 voices = 200 oscs, no partials)")
    ap.add_argument("--piano-notes-6", default="48,52,55,60,64,67",
                    help="clean control: the first 6 of the same low notes, at the "
                         "same num_voices — isolates active voice count with no "
                         "change in register, loudness, decay or osc layout")
    ap.add_argument("--piano-notes-hi", default="84,88,90,92,96,98,100,104",
                    help="high-register set; CONFOUNDED (these decay to near "
                         "silence), kept only for continuity")
    ap.add_argument("--piano-step", type=float, default=0.5,
                    help="seconds between successive note_ons in the ramp")
    ap.add_argument("--piano-hold", type=float, default=6.0,
                    help="seconds to hold all notes (ramp and re-strike each)")
    ap.add_argument("--piano-duration", type=float, default=22.0,
                    help="seconds to record the polyphony probe")
    ap.add_argument("--no-bench-lock", action="store_true",
                    help="skip the /tmp/amyboard-bench.lock flock (local bring-up "
                         "on a bench nothing else shares)")
    args = ap.parse_args()
    print(f"[hwci] backend: {'ALSA cli' if ALSA else 'python libs'}")

    if args.list_devices:
        list_devices(); return 0

    if not args.no_flash and not args.port:
        sys.exit("--port (the USB-serial dongle) is required to flash.")
    bin_path, tmp = resolve_firmware(args) if not args.no_flash else (None, False)

    # Take the bench lock AFTER the firmware download (network time shouldn't
    # hold the bench) and keep the fd referenced until the process exits.
    bench_lock = None if args.no_bench_lock else acquire_bench(args.port)

    # We deliberately do NOT hold the debug UART open while the board boots: the
    # proven-good reset on this bench is `esptool chip-id` (hard-reset + release
    # the port), wait a few seconds, then `amidi -l` -- holding the dongle's
    # DTR/RTS lines across boot is what left the board wedged in a boot-loop. The
    # consoles are tailed only *after* MIDI is up.
    debug_port = args.debug_port or args.port or ("/dev/ttyACM1" if ALSA else None)
    loggers = []
    cdc = None   # the CDC tail also carries the polled hwci_load samples
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
        diagnostics = []   # (name, recording, marks) — recorded + analyzed, never gating
        try:
            # The board boots into its SAVED sketch (e.g. woodpiano left over from
            # a previous run's transfer), which also answers our MIDI notes and
            # contaminates the built-in-tones recording. Silence it first so
            # hwci_basic only measures the default patch.
            m.silence()
            time.sleep(0.5)
            if cdc:   # samples land on the CDC log; without it there's nowhere to read them
                m.poll_load_start()
            rec = record_and_drive(args, args.duration, lambda: run_test_sequence(m))
            results.append((args.name, rec, args.min_similarity))
            # Piano polyphony probe (diagnostic; never gates pass/fail). Runs
            # before the World suite so the board is still in its clean default
            # state — no pushed sketch loop() answering our notes.
            if args.piano_poly:
                # Three note sets, ALL at the same num_voices, so the synth config
                # and osc layout are byte-identical across them and only what's
                # actually sounding differs.
                #
                #  * 8 low  — the repro.
                #  * 6 low  — THE clean control: same register, same loudness, same
                #    decay, same allocation (num_voices is unchanged, so voices are
                #    still spaced patch_oscs[256]=25 apart); only the count of
                #    active voices drops. If the artifact needs 7-8 voices this
                #    isolates it with no confound.
                #  * 8 high — kept for continuity, but CONFOUNDED: high piano notes
                #    both use fewer partials AND decay to near-silence (-51.9dB in
                #    the 2026-07-26 run), so "no clicks" there may just mean
                #    "nothing was sounding". Don't draw conclusions from it alone.
                #
                # Note the earlier osc-pool theory was WRONG: max partials is a
                # per-PRESET constant (24, from use_this_partial_map over
                # num_harmonics[0]), not per-note, and patch_oscs[256]=25, so 8
                # voices reserve 200 oscs against a default max_oscs of 250 — it
                # fits, and the OOM path never triggers here.
                # patch_oscs[256]=25, so piano voices cost 25 oscs each. The DX7
                # set is osc-count-matched to piano@8 (25 voices x 8 oscs = 200)
                # but uses FM, no partials at all -- so it separates "200 oscs of
                # render work" from "the INTERP_PARTIALS path".
                # Focused on ONE question this round: is a periodic background
                # task stealing time from the render task? piano_poly8 vs
                # piano_poly8_nocv is that A/B and nothing else differs.
                # Dropped from the previous round: dx7_osc_matched and
                # piano_poly8_hi both recorded at about -53dB (effectively
                # silent), so their "0 clicks" measured nothing.
                sets = [
                    dict(tag="piano_poly8", patch=256, voices=8, notes=args.piano_notes,
                         why="8 low notes, the repro, 200 oscs, CV reads ACTIVE"),
                    dict(tag="piano_poly6", patch=256, voices=8, notes=args.piano_notes_6,
                         why="6 low notes, known-clean anchor, 150 oscs"),
                ]
                for s in sets:
                    tag, why = s["tag"], s["why"]
                    try:
                        notes = [int(n) for n in s["notes"].split(",") if n.strip()]
                        step = s.get("step", args.piano_step)
                        print(f"\n[piano8] === {tag}: patch {s['patch']}, "
                              f"num_voices={s['voices']}, {len(notes)} notes={notes} "
                              f"({why}) ===")
                        m.silence()
                        time.sleep(0.3)
                        setup_piano_poly(m, s["patch"], s["voices"],
                                         no_cv=s.get("no_cv", False))
                        # No zP traffic during the recording: a load poll mid-hold
                        # would inject its own transients into exactly what we're
                        # measuring. Load is read once after, still holding.
                        if getattr(m, "_poll_pause", None):
                            m._poll_pause.set()
                        marks = []
                        try:
                            piano_rec = record_and_drive(
                                args, args.piano_duration,
                                lambda: run_piano_poly_sequence(
                                    m, marks, notes, step, args.piano_hold))
                        finally:
                            if getattr(m, "_poll_pause", None):
                                m._poll_pause.clear()
                        diagnostics.append((tag, piano_rec, marks))
                    except Exception as e:
                        print(f"[piano8] {tag} failed: {e}")
            if args.world:
                results += run_world_suite(args, m)
                m.silence()   # leave the board quiet for the Tulip test that follows
        finally:
            m.poll_load_stop()
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

    # Diagnostics: save the recording and measure it for periodic transients.
    # Never affects overall_ok — these have no reference and exist to put numbers
    # in the log for whoever's chasing the artifact.
    LEAD_IN = 0.3   # record_and_drive starts arecord this long before drive_fn
    for name, rec, marks in diagnostics:
        if rec is None:
            print(f"\n[piano8] {name}: no recording")
            continue
        out_wav = f"{name}-recording.wav"
        write_wav_mono(out_wav, rec, args.samplerate)
        print(f"\n[audio] saved {out_wav}")
        if marks:
            print("[piano8] timeline (seconds into the wav):")
            for t, label in marks:
                print(f"[piano8]   {t + LEAD_IN:6.3f}s  {label}")
        # Compare like-for-like: the fully-held windows against the ramp that
        # led into them. If a period shows up only once all 8 are held, that's
        # the reported artifact and its spacing is printed here.
        spans = {}
        held = [t + LEAD_IN for t, l in marks if l.startswith("ALL ")]
        rel = [t + LEAD_IN for t, l in marks if l.startswith("release all")]
        strike = [t + LEAD_IN for t, l in marks if l.startswith("re-strike")]
        first_on = [t + LEAD_IN for t, l in marks if l.startswith("note_on")]
        if first_on and held:
            spans["ramp 1->8 notes"] = (first_on[0], held[0])
        if held and rel:
            spans["ALL 8 HELD (ramp)"] = (held[0] + 0.15, rel[0])
        if strike and len(rel) > 1:
            spans["ALL 8 HELD (re-strike)"] = (strike[0] + 0.15, rel[1])
        if not spans:
            spans["whole recording"] = (0.0, len(rec) / args.samplerate)
        for label, (a, b) in spans.items():
            tr = analyze_transients(rec, args.samplerate, a, b)
            report_transients(label, tr)
            # What does the glitch actually look like? Silence gap vs repeated
            # block vs bare step points at different mechanisms.
            if tr and tr["click_times"]:
                report_click_shapes(label, classify_clicks(rec, args.samplerate,
                                                           tr["click_times"]),
                                    args.samplerate)
            # Is AMY's CPU-overload bleep in here? 440/880Hz only -- 523/659 are
            # in the C-E-G probe chord and would prove nothing.
            for f0 in (440.0, 880.0):
                report_narrowband(label, narrowband_report(rec, args.samplerate,
                                                           f0, a, b))

    # AMY render load polled during the run (informational, doesn't gate
    # pass/fail): mean/max of the hwci_load samples on the CDC log. -1 samples
    # mean the firmware predates tulip.amy_render_load() (amy#826 / PR #1105).
    if cdc is not None:
        # AMY's CPU-overload failsafe, routed to stdout by setup_piano_poly's
        # callback. Each trip resets all notes/oscs and plays a 4-note bleep at
        # i*160ms spacing, so a nonzero count here is the direct explanation for
        # periodic ~160ms transients -- and it zeroes render_us, which is why the
        # 1Hz load samples read low.
        ovl = re.findall(r"hwci_overload (-?\d+)", cdc.text())
        if ovl:
            print(f"\n*** AMY CPU OVERLOAD FAILSAFE TRIPPED {len(ovl)} time(s) "
                  f"during this run (load% at trip: {ovl[:20]}) ***")
            print("    Each trip resets all notes/oscs and plays a descending "
                  "bleep spaced 160ms (amy api.c amy_overload_failsafe).")
        else:
            print("\namy CPU overload failsafe: never tripped (callback armed)")
        vals = [float(x) for x in re.findall(r"hwci_load (-?[\d.]+)", cdc.text())]
        good = [v for v in vals if v >= 0]
        if good:
            print(f"\namyboard render load: mean={sum(good) / len(good):.3f}  "
                  f"max={max(good):.3f}  (n={len(good)})")
        elif vals:
            print(f"\namyboard render load: not supported by this firmware "
                  f"({len(vals)} polls)")
        else:
            print("\namyboard render load: no samples captured")

    if args.update_reference:
        return 0
    return 0 if overall_ok else 1


if __name__ == "__main__":
    sys.exit(main())
