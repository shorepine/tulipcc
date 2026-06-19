#!/usr/bin/env python3
"""Tulip CC hardware-in-the-loop (HW CI) test.

The Tulip analogue of `hwci.py` (which tests the AMYboard). Flashes a built
TULIP4_R11 firmware onto a physically connected Tulip, reboots it, and runs two
checks over the **MicroPython serial REPL**:

  1. WiFi + graphics: join WiFi, render `sys/ex/rgb332.py`, `tulip.screenshot()`
     (which uploads a PNG to Tulip World), then download that PNG and pixel-compare
     it to a committed `ref/tulip_screenshot.png`.
  2. Audio: define a synth, play a chromatic scale + a sustained sine, record the
     analog out, and spectral-compare to a committed `ref/tulip_basic.wav`.

How Tulip differs from the AMYboard, and why this is a separate script:
  * Tulip's USB is an **onboard CH340K** USB-UART (with DTR/RTS), not a separate
    debug dongle. That ONE port does everything: esptool flashing (its DTR/RTS
    drive the auto-reset → ROM bootloader), the MicroPython REPL we drive from,
    AND the ESP-IDF/boot console we log. TULIP4_R11 builds with
    MICROPY_HW_ENABLE_UART_REPL and no native USB.
  * There is **no USB-MIDI** — the whole stimulus is REPL lines.
  * Firmware is a **built artifact** (CI builds TULIP4_R11), via `--firmware`/`--url`.

WiFi creds come from `--wifi-ssid`/`--wifi-password` or (preferred, so they don't
show in `ps`) the env vars `TULIP_WIFI_SSID` / `TULIP_WIFI_PASSWORD`. The REPL
ECHOES the `tulip.wifi(...)` line, so those secret values are **redacted** (→ `***`)
from everything this script prints or writes (the serial log is a CI artifact).

Quick start (Pi):
    python3 tulip_hwci.py --list-devices
    # capture references once (audio always; screenshot too if WiFi creds are set):
    TULIP_WIFI_SSID=net TULIP_WIFI_PASSWORD=pw \\
      python3 tulip_hwci.py --firmware tulip-full-TULIP4_R11.bin --update-reference
    # pass/fail run (flash + wifi/screenshot + audio + compare):
    TULIP_WIFI_SSID=net TULIP_WIFI_PASSWORD=pw \\
      python3 tulip_hwci.py --firmware tulip-full-TULIP4_R11.bin
"""
import argparse
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
ALSA = bool(shutil.which("arecord"))
WORLD_BASE = "https://tulipcc-production.up.railway.app"


# ── firmware ──────────────────────────────────────────────────────────────────
def resolve_firmware(args):
    """Return (path, is_temp). Tulip firmware is a built artifact, not a preview."""
    if args.firmware:
        return args.firmware, False
    if not args.url:
        sys.exit("Need --firmware <path> or --url <url> (or --no-flash).")
    print(f"[fw] downloading {args.url}")
    fd, path = tempfile.mkstemp(suffix="-tulip-full.bin")
    with urllib.request.urlopen(args.url, timeout=120) as r, os.fdopen(fd, "wb") as f:
        data = r.read()
        if data[:64].lstrip().startswith(b"<"):
            sys.exit("[fw] got HTML, not a .bin — bad URL?")
        f.write(data)
    print(f"[fw] {len(data)} bytes -> {path}")
    return path, True


def flash(port, bin_path, baud, offset="0x10000"):
    """Flash `bin_path` at `offset` over the CH340K. Default offset 0x10000 is the
    app (micropython) partition only — fast (~20s), which is all a per-PR test needs
    since the bootloader/partition-table/filesystem rarely change. esptool's --before
    default-reset uses the CH340K's DTR/RTS auto-reset to enter the ROM bootloader;
    --after hard-reset boots. To restore a wiped board, flash the FULL image
    (`tulip-full-*.bin`) at 0x0 instead (`--flash-offset 0x0`)."""
    cmd = [sys.executable, "-m", "esptool", "--chip", "esp32s3", "--port", port,
           "--baud", str(baud), "--before", "default-reset", "--after", "hard-reset",
           "write-flash", offset, bin_path]
    print("[flash]", " ".join(cmd))
    subprocess.run(cmd, check=True)


# ── serial REPL (with secret redaction) ───────────────────────────────────────
class Repl:
    """One CH340K serial port, used to BOTH drive the MicroPython REPL (write a
    line, press enter) and tail the whole console (boot log + REPL echo) for the
    run's log. A background thread owns the reads; the main thread writes — the
    standard miniterm pattern, safe on one pyserial handle.

    `redact` is a list of secret strings (the WiFi password/SSID) that are scrubbed
    to `***` from everything this class prints or returns — the REPL echoes input,
    so the `tulip.wifi(...)` line would otherwise leak into the serial-log artifact.

    Opening de-asserts DTR/RTS, but the kernel still briefly raises the modem lines
    as the tty opens, which pulses the Tulip's auto-reset → the board reboots as we
    attach, giving a clean boot log."""
    def __init__(self, port, baud=115200, redact=None):
        self.port, self.baud = port, baud
        self.buf = bytearray()
        self._lock = threading.Lock()
        self._stop = threading.Event()
        self.ser = None
        self._t = None
        self.redact = [s for s in (redact or []) if s]

    def _scrub(self, s):
        for secret in self.redact:
            s = s.replace(secret, "***")
        return s

    def open(self, retry_s=8.0):
        import serial  # pyserial (ships with esptool)
        deadline = time.monotonic() + retry_s
        last = None
        while time.monotonic() < deadline:
            try:
                ser = serial.Serial()
                ser.port = self.port
                ser.baudrate = self.baud
                ser.timeout = 0.2
                ser.dtr = False
                ser.rts = False
                ser.open()
                self.ser = ser
                break
            except Exception as e:  # port may still be re-enumerating after a flash
                last = e
                time.sleep(0.3)
        if self.ser is None:
            sys.exit(f"[repl] could not open {self.port}: {last}")
        self._t = threading.Thread(target=self._read_loop, daemon=True)
        self._t.start()
        print(f"[repl] opened {self.port} @ {self.baud}")

    def _read_loop(self):
        while not self._stop.is_set():
            try:
                data = self.ser.read(4096)
            except Exception:
                break
            if data:
                with self._lock:
                    self.buf.extend(data)

    def _tail(self, n=400):
        with self._lock:
            return self._scrub(self.buf[-n:].decode("utf-8", "replace"))

    def text(self):
        with self._lock:
            return self._scrub(self.buf.decode("utf-8", "replace"))

    def write(self, data):
        self.ser.write(data)
        self.ser.flush()

    def send(self, line, settle=0.4):
        """Send one REPL line (echoed back into the log) and let it execute.
        The printed/logged copy is scrubbed of any redacted secrets."""
        print(f"[repl] >>> {self._scrub(line)}")
        self.write(line.encode() + b"\r\n")
        time.sleep(settle)

    def interrupt(self, settle=0.6):
        """Ctrl-C twice → break any running program back to a clean `>>>`."""
        self.write(b"\r\x03\x03")
        time.sleep(settle)

    def wait_for_prompt(self, boot_s=5.0, timeout=12.0):
        """Wait out a reboot / blocking call, then confirm a live `>>>` prompt."""
        if boot_s:
            print(f"[repl] waiting {boot_s:.0f}s for boot ...")
            time.sleep(boot_s)
        deadline = time.time() + timeout
        while time.time() < deadline:
            self.write(b"\r")
            time.sleep(0.3)
            if ">>>" in self._tail(400):
                return True
        print("[repl] WARNING: never saw a '>>>' prompt; continuing anyway")
        return False

    def close(self):
        self._stop.set()
        if self._t:
            self._t.join(timeout=2)
        try:
            if self.ser:
                self.ser.close()
        except Exception:
            pass


def write_serial_log(repl, path):
    with open(path, "w", encoding="utf-8") as f:
        f.write(f"===== Tulip console (boot log + REPL)  [{repl.port} @ {repl.baud} baud] =====\n")
        body = repl.text()  # already scrubbed of secrets
        f.write(body)
        if not body.endswith("\n"):
            f.write("\n")
    print(f"[log] wrote serial log -> {path}")


# ── audio ─────────────────────────────────────────────────────────────────────
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
    """Record `duration`s of audio while `drive_fn` runs concurrently."""
    import numpy as np
    sr, dev = args.samplerate, args.audio_device
    print(f"[audio] recording {duration}s @ {sr}Hz from {dev!r}")
    if ALSA:
        tmp = tempfile.mktemp(suffix=".wav")
        proc = subprocess.Popen(["arecord", "-q", "-D", dev, "-f", "S16_LE", "-r", str(sr),
                                 "-c", str(args.channels), "-d", str(int(duration + 1)), tmp],
                                stderr=subprocess.DEVNULL)
        time.sleep(0.4)
        drive_fn()
        proc.wait()
        d, _ = read_wav_mono(tmp)
        os.unlink(tmp)
        return d[:int(duration * sr)]
    import sounddevice as sd
    rec = sd.rec(int(duration * sr), samplerate=sr, channels=args.channels,
                 device=dev, dtype="float32")
    time.sleep(0.4); drive_fn(); sd.wait()
    return rec.mean(axis=1) if args.channels > 1 else rec[:, 0]


def avg_spectrum(x, nfft=4096):
    """Average magnitude spectrum (Welch-ish) — timing/phase invariant."""
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


def compare_audio(rec, ref):
    """Returns (spectral cosine similarity in 0..1, recording level in dB)."""
    import numpy as np
    sim = float(np.dot(avg_spectrum(rec), avg_spectrum(ref)))  # 1.0 == identical
    level_db = 20 * np.log10(float(np.sqrt(np.mean(rec ** 2))) + 1e-9)
    return sim, level_db


# ── wifi + screenshot ─────────────────────────────────────────────────────────
def run_wifi_screenshot(repl, ssid, pw, username, wifi_timeout=25):
    """Join WiFi, render rgb332.py, and `tulip.screenshot()` (which uploads the PNG
    to Tulip World). Returns True if WiFi connected (so the screenshot was sent).

    The associate + DHCP can take >10s, so we pass a longer `wait_timeout` to
    tulip.wifi() and then poll tulip.ip() (it can lag a beat after wifi() returns).
    The connected-check marker `WIFICONN True` only appears in the print's OUTPUT,
    never in the echoed command, so it can't false-positive on the echo."""
    print("[wifi] joining network, rendering rgb332, uploading screenshot")
    repl.interrupt()
    repl.send("import world, tulip", 0.5)
    repl.send("_ip = tulip.wifi(%r, %r, %d)" % (ssid, pw, wifi_timeout), 0.0)  # echo scrubbed
    repl.wait_for_prompt(boot_s=0.0, timeout=wifi_timeout + 8)  # wifi() blocks up to wifi_timeout
    connected = False
    for _ in range(6):
        repl.send("print('WIFICONN', tulip.ip() is not None)", 0.8)
        if "WIFICONN True" in repl.text():
            connected = True
            break
        time.sleep(1)
    if not connected:
        print("[wifi] NOT connected (tulip.ip() is None) — skipping screenshot")
        return False
    print("[wifi] connected")
    repl.send("world.username = %r" % username, 0.3)
    repl.send("execfile(tulip.root_dir()+'sys/ex/rgb332.py')", 1.2)  # static palette render
    # Stop the text framebuffer so the REPL transcript is NOT composited into the
    # screenshot. Without this the captured/uploaded PNG would (a) leak the WiFi
    # password from the echoed tulip.wifi(...) line, and (b) be non-deterministic
    # (boot banner + varying transcript). tfb_stop() leaves only the rgb332 palette.
    repl.send("tulip.tfb_stop()", 0.7)
    repl.send("tulip.screenshot()", 4.0)  # encodes PNG + POSTs to /api/tulipworld/upload
    repl.send("tulip.tfb_start()", 0.3)   # restore the REPL console
    return True


def latest_screenshot_id(username, base=WORLD_BASE):
    """Current newest screenshot.png id for `username` (or -1). Captured BEFORE the
    run so fetch_screenshot can require a strictly-newer upload (freshness guard)."""
    try:
        url = ("%s/api/tulipworld/files_latest?filename=screenshot.png&username=%s"
               % (base, urllib.parse.quote(username)))
        with urllib.request.urlopen(url, timeout=30) as r:
            m = json.load(r)
        return m.get("id", -1) if m else -1
    except Exception:
        return -1


def fetch_screenshot(username, out_path, base=WORLD_BASE, min_id=-1, tries=6):
    """Download the screenshot.png `username` just uploaded → out_path. Requires a
    row with id > min_id, so a failed upload can't make us compare a STALE image
    (which would be a false pass). Returns the file-metadata dict, or None."""
    meta = None
    for i in range(tries):
        try:
            url = ("%s/api/tulipworld/files_latest?filename=screenshot.png&username=%s"
                   % (base, urllib.parse.quote(username)))
            with urllib.request.urlopen(url, timeout=30) as r:
                m = json.load(r)
            if m and m.get("download_url") and m.get("id", -1) > min_id:
                meta = m
                break
        except Exception as e:
            print(f"[shot] fetch attempt {i + 1}/{tries}: {e}")
        time.sleep(2)
    if not meta:
        print(f"[shot] no fresh screenshot.png (id > {min_id}) for username {username!r}")
        return None
    with urllib.request.urlopen(base + meta["download_url"], timeout=60) as r:
        data = r.read()
    with open(out_path, "wb") as f:
        f.write(data)
    print(f"[shot] downloaded {len(data)} bytes (id {meta.get('id')}) -> {out_path}")
    return meta


def delete_world_file(item_id, base, token):
    """Best-effort: remove the uploaded screenshot from Tulip World (admin) so CI
    runs don't clutter it — we keep the downloaded local copy for the artifact."""
    if not token:
        print(f"[shot] no admin token (set $WORLD_ADMIN_TOKEN) — leaving id {item_id} on Tulip World")
        return False
    try:
        req = urllib.request.Request(f"{base}/api/tulipworld/files/{item_id}",
                                     method="DELETE", headers={"X-Admin-Token": token})
        with urllib.request.urlopen(req, timeout=30) as r:
            ok = 200 <= r.status < 300
        print(f"[shot] deleted uploaded id {item_id} from Tulip World"
              if ok else f"[shot] delete id {item_id} -> HTTP {r.status}")
        return ok
    except Exception as e:
        print(f"[shot] could not delete id {item_id} from Tulip World: {e}")
        return False


def compare_images(a_path, b_path):
    """Mean absolute per-pixel difference (0..255 scale) between two PNGs, plus
    each shape. Lower = more identical; mismatched shapes return (None, shapes)."""
    from PIL import Image
    import numpy as np
    a = np.asarray(Image.open(a_path).convert("RGB"), dtype=np.float32)
    b = np.asarray(Image.open(b_path).convert("RGB"), dtype=np.float32)
    if a.shape != b.shape:
        return None, (a.shape, b.shape)
    return float(np.mean(np.abs(a - b))), a.shape


# ── stimulus ──────────────────────────────────────────────────────────────────
def run_audio_sequence(repl, duration, scale_step=0.5, n_lo=60, n_hi=72):
    """Deterministic audio stimulus. EDIT to change what the HW CI exercises.

    `synth=1` is undefined on a fresh boot, so we define it first (Juno-6 patch 0,
    6 voices). Then a chromatic scale (notes 60..72) and a sustained bare-osc sine
    (defaults to SINE) — both reproducible, so the averaged spectrum is stable. We
    `amy.reset()` at the start (clearing boot state) and end (leaving the board
    silent — it shares one audio input with the AMYboard)."""
    repl.interrupt()
    repl.send("import amy", 0.3)
    repl.send("amy.reset()", 0.5)
    repl.send("amy.send(synth=1, patch=0, num_voices=6)", 0.6)  # define synth 1

    scale_secs = (n_hi - n_lo + 1) * scale_step

    def drive():
        for n in range(n_lo, n_hi + 1):
            repl.send("amy.send(synth=1, vel=1, note=%d)" % n, scale_step)
        repl.send("amy.reset()", 0.3)
        repl.send("amy.send(freq=440, vel=1)", 0.0)            # sustained sine
        sine_end = time.time() + max(0.0, duration - scale_secs - 0.5)
        while time.time() < sine_end:
            time.sleep(0.1)

    rec = record_and_drive(args_g, duration, drive)
    repl.send("amy.reset()", 0.3)                              # leave board silent
    return rec


def dominant_freq(x, sr):
    import numpy as np
    if len(x) < 4096:
        return 0.0
    seg = x[int(len(x) * 0.6):]  # the tail is the sustained sine
    if len(seg) < 4096:
        seg = x
    win = np.hanning(len(seg))
    sp = np.abs(np.fft.rfft(seg * win))
    freqs = np.fft.rfftfreq(len(seg), 1.0 / sr)
    return float(freqs[int(np.argmax(sp))])


def list_devices():
    if ALSA:
        print("=== serial ports ==="); os.system("ls /dev/ttyUSB* /dev/ttyACM* /dev/tulip-* 2>/dev/null")
        print("=== audio capture (arecord -l) ==="); os.system("arecord -l")
    else:
        import serial.tools.list_ports, sounddevice as sd
        for p in serial.tools.list_ports.comports():
            print("serial:", p.device, p.description)
        for i, d in enumerate(sd.query_devices()):
            if d["max_input_channels"]:
                print(f"audio in [{i}] {d['name']}")


# ── main ────────────────────────────────────────────────────────────────────
args_g = None  # set in main(); run_audio_sequence reads samplerate/channels/etc.


def main():
    global args_g
    ap = argparse.ArgumentParser(description="Tulip CC hardware-in-the-loop test")
    ap.add_argument("--list-devices", action="store_true")
    ap.add_argument("--firmware", help="app image to flash (tulip-firmware-*.bin) at "
                    "--flash-offset; or the full image (tulip-full-*.bin) with --flash-offset 0x0")
    ap.add_argument("--url")
    ap.add_argument("--port", default="/dev/tulip-repl" if ALSA else None,
                    help="the Tulip's CH340K serial port (flash + REPL + console)")
    ap.add_argument("--baud", type=int, default=921600, help="esptool flash baud")
    ap.add_argument("--flash-offset", default="0x10000",
                    help="flash offset (default 0x10000 = app only, ~20s; 0x0 = full image)")
    ap.add_argument("--repl-baud", type=int, default=115200)
    ap.add_argument("--no-flash", action="store_true")
    ap.add_argument("--flash-only", action="store_true")
    ap.add_argument("--audio-device", default="hw:CARD=Device,DEV=0" if ALSA else None,
                    help="ALSA hw:X,Y / card id, or sounddevice index/name")
    ap.add_argument("--samplerate", type=int, default=48000)
    ap.add_argument("--channels", type=int, default=1)
    ap.add_argument("--duration", type=float, default=11.0,
                    help="seconds to record (chromatic scale + sustained sine)")
    ap.add_argument("--name", default="tulip_basic")
    ap.add_argument("--reference"); ap.add_argument("--update-reference", action="store_true")
    ap.add_argument("--min-similarity", type=float, default=0.90,
                    help="min audio spectral cosine similarity to pass (0..1)")
    ap.add_argument("--min-level-db", type=float, default=-30.0,
                    help="min recording RMS level (dB) to pass")
    # WiFi + screenshot
    ap.add_argument("--wifi-ssid", default=os.environ.get("TULIP_WIFI_SSID"),
                    help="WiFi SSID (default: $TULIP_WIFI_SSID; prefer env over CLI)")
    ap.add_argument("--wifi-password", default=os.environ.get("TULIP_WIFI_PASSWORD"),
                    help="WiFi password (default: $TULIP_WIFI_PASSWORD; prefer env)")
    ap.add_argument("--no-wifi", action="store_true", help="skip the WiFi+screenshot check")
    ap.add_argument("--require-wifi", action="store_true",
                    help="fail if WiFi creds are missing or the connection fails (use in CI)")
    ap.add_argument("--world-username", default="tulipci")
    ap.add_argument("--world-base", default=WORLD_BASE)
    ap.add_argument("--world-admin-token", default=os.environ.get("WORLD_ADMIN_TOKEN"),
                    help="admin token to delete the uploaded screenshot from Tulip World "
                         "after download (default: $WORLD_ADMIN_TOKEN)")
    ap.add_argument("--screenshot-name", default="tulip_screenshot")
    ap.add_argument("--screenshot-reference")
    ap.add_argument("--max-pixel-diff", type=float, default=8.0,
                    help="max mean abs per-pixel diff (0..255) for the screenshot to pass")
    ap.add_argument("--out")
    ap.add_argument("--serial-log", default=None,
                    help="serial log path (default: {name}-serial.log)")
    ap.add_argument("--no-serial-log", action="store_true")
    args = ap.parse_args()
    args_g = args
    print(f"[hwci] backend: {'ALSA cli' if ALSA else 'python libs'}")

    if args.list_devices:
        list_devices(); return 0

    if not args.port:
        sys.exit("--port (the Tulip's CH340K serial port) is required.")

    do_wifi = not args.no_wifi and bool(args.wifi_ssid and args.wifi_password)
    if not do_wifi and not args.no_wifi:
        print("[wifi] no creds (set $TULIP_WIFI_SSID / $TULIP_WIFI_PASSWORD) — "
              "skipping WiFi+screenshot check")
        if args.require_wifi:
            sys.exit("[wifi] --require-wifi set but no WiFi creds provided.")

    if not args.no_flash:
        bin_path, tmp = resolve_firmware(args)
        try:
            flash(args.port, bin_path, args.baud, args.flash_offset)
        finally:
            if tmp:
                os.unlink(bin_path)
        if args.flash_only:
            print("[done] flashed."); return 0
        time.sleep(1.5)  # let the board re-enumerate / settle after hard-reset

    if not args.audio_device:
        sys.exit("--audio-device required (see --list-devices).")

    # Drive the REPL + capture the single console (boot log + REPL) for the run.
    # Secrets are scrubbed from everything the Repl prints/returns.
    repl = Repl(args.port, args.repl_baud, redact=[args.wifi_password, args.wifi_ssid])
    repl.open()
    wifi_connected = None
    pre_shot_id = -1
    try:
        repl.wait_for_prompt()
        if do_wifi:  # WiFi + screenshot first (the user's sequence), then audio
            pre_shot_id = latest_screenshot_id(args.world_username, args.world_base)
            wifi_connected = run_wifi_screenshot(repl, args.wifi_ssid, args.wifi_password,
                                                 args.world_username)
        rec = run_audio_sequence(repl, args.duration)
    finally:
        if not args.no_serial_log:
            write_serial_log(repl, args.serial_log or f"{args.name}-serial.log")
        repl.close()

    # Download the screenshot the board just uploaded (must be newer than pre_shot_id),
    # then delete it from Tulip World so CI runs don't clutter it. We keep the local
    # copy (for the artifact + compare); the World delete is best-effort.
    shot_path = None
    if wifi_connected:
        shot_path = f"{args.screenshot_name}-capture.png"
        meta = fetch_screenshot(args.world_username, shot_path, args.world_base,
                                min_id=pre_shot_id)
        if meta is None:
            shot_path = None
        elif meta.get("id") is not None:
            delete_world_file(meta["id"], args.world_base, args.world_admin_token)

    out_wav = args.out or f"{args.name}-recording.wav"
    write_wav_mono(out_wav, rec, args.samplerate)
    print(f"[audio] saved {out_wav}")

    audio_ref = args.reference or os.path.join(REF_DIR, f"{args.name}.wav")
    shot_ref = args.screenshot_reference or os.path.join(REF_DIR, f"{args.screenshot_name}.png")

    if args.update_reference:
        os.makedirs(REF_DIR, exist_ok=True)
        write_wav_mono(audio_ref, rec, args.samplerate)
        peak = dominant_freq(rec, args.samplerate)
        print(f"[ref] wrote {audio_ref} (sine peak {peak:.0f} Hz) — LISTEN to confirm.")
        if shot_path:
            shutil.copyfile(shot_path, shot_ref)
            print(f"[ref] wrote {shot_ref} — VIEW to confirm it's the rgb332 palette.")
        elif do_wifi:
            print("[ref] WARNING: WiFi/screenshot did not produce a PNG; screenshot ref NOT updated.")
        return 0

    # ── pass/fail ──
    ok = True

    if not os.path.exists(audio_ref):
        sys.exit(f"[ref] no audio reference at {audio_ref}; run once with --update-reference.")
    ref, _ = read_wav_mono(audio_ref)
    sim, level_db = compare_audio(rec, ref)
    peak = dominant_freq(rec, args.samplerate)
    audio_ok = sim >= args.min_similarity and level_db >= args.min_level_db
    ok = ok and audio_ok
    print(f"\n[AUDIO] spectral_similarity={sim:.4f} (min {args.min_similarity})  "
          f"level={level_db:.1f}dB (min {args.min_level_db})  sine_peak={peak:.0f}Hz  ->  "
          f"{'PASS' if audio_ok else 'FAIL'}")

    if do_wifi:
        if not wifi_connected:
            print("[SCREENSHOT] FAIL — WiFi did not connect, no screenshot uploaded")
            ok = False
        elif not shot_path:
            print("[SCREENSHOT] FAIL — could not download the uploaded screenshot")
            ok = False
        elif not os.path.exists(shot_ref):
            sys.exit(f"[ref] no screenshot reference at {shot_ref}; run once with --update-reference.")
        else:
            mad, shapes = compare_images(shot_path, shot_ref)
            if mad is None:
                print(f"[SCREENSHOT] FAIL — size mismatch capture {shapes[0]} vs ref {shapes[1]}")
                ok = False
            else:
                shot_ok = mad <= args.max_pixel_diff
                print(f"[SCREENSHOT] mean_pixel_diff={mad:.2f} (max {args.max_pixel_diff})  ->  "
                      f"{'PASS' if shot_ok else 'FAIL'}")
                ok = ok and shot_ok

    print(f"\n{args.name}: {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
