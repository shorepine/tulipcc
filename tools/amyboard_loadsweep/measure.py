#!/usr/bin/env python3
"""Flash one AMYboard firmware and record its render load running a sketch.

Flashes a full merged image (bootloader + partition table + app + sys + vfs)
at 0x0 over the debug-UART dongle — always the full image, because the
partition table has changed over the project's history and an app-only flash
onto an old table bricks the boot. Then boots the board, uploads the sketch
over USB-MIDI SysEx (amyboardctl), and tails the same UART for the
`RENDER_LOAD ms=<ms> load=<0..1>` lines that patch_render_load.py adds to the
firmware. Writes <out>/{load.csv,serial.log,meta.json}.

The dongle's DTR/RTS must drive the board's auto-reset circuit (DTR->IO0,
RTS->EN): esptool then enters the ROM bootloader by itself, and — important
for this tool — RTS recovers a board that the sketch wedged. If the handshake
fails, we fall back to `zP machine.bootloader()` over USB-MIDI and flash via
the board's native USB (the ROM USB-Serial-JTAG usually enumerates at the
same /dev/cu.usbmodem name on macOS).

Requires: pyserial, esptool, mido + python-rtmidi (all in the amyboardctl
install), and the board's USB-MIDI visible as "AMYboard".
"""
import argparse
import csv
import json
import os
import re
import subprocess
import sys
import threading
import time

HERE = os.path.dirname(os.path.abspath(__file__))
try:
    import amyboardctl  # noqa: F401
except ImportError:
    sys.path.insert(0, os.path.join(HERE, "..", "amyboardctl"))
from amyboardctl import AMYboardLink, board_present  # noqa: E402

RL = re.compile(r"RENDER_LOAD ms=(\d+) load=([\d.]+)")
MIDI_NAME = "AMYboard"


def esptool(port, *args, timeout=300):
    cmd = [sys.executable, "-m", "esptool", "--chip", "esp32s3",
           "--port", port] + list(args)
    return subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)


class SerialTail:
    """Line-oriented tail of the debug UART, host-timestamping each line.

    Opens with DTR/RTS de-asserted, but the open itself can still pulse the
    modem lines and reboot the board through the auto-reset circuit — so open
    the tail BEFORE starting the sketch and wait for MIDI to (re)appear.
    """

    def __init__(self, port, baud):
        import serial
        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = baud
        self.ser.timeout = 0.2
        self.ser.dtr = False
        self.ser.rts = False
        self.lines = []           # (host_time, text)
        self._stop = threading.Event()
        self._buf = b""
        self._t = threading.Thread(target=self._run, daemon=True)

    def open(self, retries=20):
        for _ in range(retries):
            try:
                self.ser.open()
                self._t.start()
                return True
            except Exception:
                time.sleep(0.4)
        return False

    def _run(self):
        while not self._stop.is_set():
            try:
                data = self.ser.read(4096)
            except Exception:
                break
            if data:
                self._buf += data
                while b"\n" in self._buf:
                    line, self._buf = self._buf.split(b"\n", 1)
                    self.lines.append(
                        (time.time(), line.decode("utf-8", "replace").rstrip("\r")))

    def stop(self):
        self._stop.set()
        self._t.join(timeout=3)
        try:
            self.ser.close()
        except Exception:
            pass


def wait_for_midi(timeout=40):
    deadline = time.time() + timeout
    while time.time() < deadline:
        p = board_present(MIDI_NAME, backend="mido")
        if p:
            return p
        time.sleep(0.5)
    return None


def reset_via_chipid(uart_port):
    """Hard-reset cycle over the dongle (also recovers a wedged board).
    hwci lesson: this boots the board more reliably than a flash's own
    post-flash hard-reset."""
    r = esptool(uart_port, "chip-id", timeout=90)
    ok = r.returncode == 0 and "MAC:" in r.stdout
    print(f"[reset] chip-id {'ok' if ok else 'FAILED'}")
    return ok


def software_enter_bootloader(native_port):
    midi = wait_for_midi(15)
    if not midi:
        return False
    try:
        with AMYboardLink(port_match=midi, verbose=False, backend="mido",
                          ack_timeout=2.0) as link:
            link.send_python("import machine; machine.bootloader()", timeout=3.0)
    except Exception as e:
        print(f"[flash] bootloader cmd: {e!r}")
    time.sleep(3.0)
    r = esptool(native_port, "--before", "no-reset", "--after", "no-reset",
                "chip-id", timeout=60)
    return r.returncode == 0 and "MAC:" in r.stdout


def flash(bin_path, uart_port, native_port, baud):
    print("[flash]", os.path.basename(bin_path))
    r = esptool(uart_port, "--baud", str(baud), "--before", "default-reset",
                "--after", "hard-reset", "write-flash", "0x0", bin_path)
    if r.returncode == 0:
        print("[flash] ok (dongle)")
        return
    print("[flash] dongle handshake failed; trying software bootloader entry")
    if not software_enter_bootloader(native_port):
        raise RuntimeError("could not get board into ROM bootloader "
                           "(board wedged and no working reset line?)")
    r = esptool(native_port, "--baud", str(baud), "--before", "no-reset",
                "--after", "hard-reset", "write-flash", "0x0", bin_path)
    if r.returncode != 0:
        sys.stdout.write(r.stdout[-2000:] + r.stderr[-2000:])
        raise RuntimeError("flash failed")
    print("[flash] ok (native USB)")


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("firmware", help="full merged image (amyboard-full-*.bin)")
    ap.add_argument("--sketch", required=True, help="sketch .py file to run")
    ap.add_argument("--out", required=True, help="output directory")
    ap.add_argument("--port", default="/dev/cu.usbserial-A5069RR4",
                    help="debug-UART dongle (flash + stderr console)")
    ap.add_argument("--native-port", default="/dev/cu.usbmodem22234101",
                    help="board native USB (software-bootloader fallback)")
    ap.add_argument("--baud", type=int, default=921600, help="flash baud")
    ap.add_argument("--debug-baud", type=int, default=115200, help="console baud")
    ap.add_argument("--seconds", type=float, default=60.0,
                    help="capture window after sketch start")
    ap.add_argument("--label", default=None, help="label stored in meta.json")
    args = ap.parse_args()

    os.makedirs(args.out, exist_ok=True)
    meta = {"label": args.label or os.path.basename(args.out.rstrip("/")),
            "firmware": args.firmware, "sketch": args.sketch,
            "seconds": args.seconds, "wedged": False,
            "midi_alive_after": None, "errors": []}

    flash(args.firmware, args.port, args.native_port, args.baud)
    reset_via_chipid(args.port)
    time.sleep(5.0)

    midi = wait_for_midi(40)
    if not midi:
        print("[boot] MIDI not up after flash; retrying chip-id reset")
        reset_via_chipid(args.port)
        time.sleep(5.0)
        midi = wait_for_midi(40)
    if not midi:
        meta["errors"].append("board never enumerated MIDI after flash")
        json.dump(meta, open(os.path.join(args.out, "meta.json"), "w"), indent=1)
        sys.exit("[boot] board never enumerated MIDI")
    print(f"[boot] MIDI up: {midi}")

    tail = SerialTail(args.port, args.debug_baud)
    if not tail.open():
        meta["errors"].append("could not open debug UART for tail")
        sys.exit("[tail] cannot open debug UART")
    time.sleep(3.0)
    midi = wait_for_midi(30)   # opening the tail may have rebooted the board
    if not midi:
        meta["errors"].append("MIDI gone after opening UART tail")
        tail.stop()
        sys.exit("[tail] board did not come back after UART open")
    time.sleep(1.5)

    print(f"[sketch] uploading {os.path.basename(args.sketch)}")
    text = open(args.sketch).read()
    t_start = None
    try:
        with AMYboardLink(port_match=midi, verbose=False, backend="mido",
                          ack_timeout=2.0) as link:
            link.reset_amy()
            time.sleep(0.3)
            link.transfer_file(text)
            link.environment_transfer_done()
            t_start = time.time()
            print(f"[sketch] started; capturing {args.seconds:.0f}s")
            time.sleep(args.seconds)
            try:
                meta["midi_alive_after"] = bool(link.ping_ok(timeout=2.5))
            except Exception:
                meta["midi_alive_after"] = False
    except Exception as e:
        meta["errors"].append(f"sketch upload/run: {e!r}")
        print(f"[sketch] ERROR: {e!r}")
        if t_start is None:
            t_start = time.time()
            time.sleep(args.seconds)

    tail.stop()
    if meta["midi_alive_after"] is False:
        meta["wedged"] = True
        print("[result] board WEDGED (MIDI dead after run)")

    with open(os.path.join(args.out, "serial.log"), "w") as f:
        for ts, line in tail.lines:
            f.write(f"{ts - t_start:+9.3f} {line}\n")

    rows = []
    for ts, line in tail.lines:
        m = RL.search(line)
        if m:
            rows.append((round(ts - t_start, 3), int(m.group(1)), float(m.group(2))))
    with open(os.path.join(args.out, "load.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["t_s", "board_ms", "load"])
        w.writerows(rows)

    during = [r for r in rows if r[0] >= 0]
    meta["n_samples_total"] = len(rows)
    meta["n_samples_during"] = len(during)
    if during:
        meta["load_max"] = max(r[2] for r in during)
        meta["load_last"] = during[-1][2]
        meta["last_sample_t"] = during[-1][0]
        if during[-1][0] < args.seconds - 3.0:
            meta["serial_died_early"] = True   # prints stopped: hard crash
    print(f"[result] {len(rows)} samples ({len(during)} during run), "
          f"max load {meta.get('load_max')}, alive={meta['midi_alive_after']}")

    json.dump(meta, open(os.path.join(args.out, "meta.json"), "w"), indent=1)
    # leave the board freshly reset (also un-wedges it for the next flash)
    reset_via_chipid(args.port)
    return 0


if __name__ == "__main__":
    sys.exit(main())
