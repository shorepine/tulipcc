"""AMYboard USB-MIDI link.

Replicates the AMYboard Web "write to sketch" SysEx protocol so we can push an
AMYboard World sketch onto a physically connected board over USB MIDI, exactly
the way the web editor's `_send_text_file_to_amyboard()` does.

Protocol (verified against the tree):
  - tulip/amyboardweb/static/spss.js  (sysex_write_amy_message, _send_text_file_to_amyboard)
  - tulip/shared/modtulip.c           (tulip_amy_send_sysex -> the 'AK' ACK)
  - amy/src/transfer.c, amy/src/parse.c (zT file receive + base64 decode)
  - tulip/shared/amyboard-py/amyboard.py (environment_transfer_done, _report_sketch_error)

All control frames are MIDI SysEx framed F0 <mfr> <ascii payload> F7, mfr = 00 03 45.

  Host -> board payloads:
    zBZ                      reboot into "skip sketch" bootloader (USB re-enumerates)
    zT<path>,<size>Z         begin a file transfer; size = raw byte count
    <base64>                 one file chunk (<=188 raw bytes, base64-encoded), repeated
    zP<python>Z              run one line of python on the board

  Board -> host frames:
    F0 00 03 45 'A' 'K' F7   ACK, sent after the board finishes processing each sysex
    F0 00 03 45 'X' <b64> F7 sketch load error: base64 of a python traceback
    F0 00 03 45 'V' <ascii>  firmware version string

The board ACKs every sysex it processes, so we send one frame and wait for its ACK
before sending the next (single message in flight) — this is what keeps the board's
sysex ring buffer from overflowing. Standard MIDI note on/off are channel-voice
messages (not sysex), are not ACKed, and make the loaded synth play.
"""

import base64
import threading
import time

import mido

MFR = [0x00, 0x03, 0x45]
MFR_T = (0x00, 0x03, 0x45)
CHUNK_BYTES = 188          # AMYBOARD_TRANSFER_CHUNK_BYTES in spss.js
ACK_TIMEOUT = 5.0          # spss.js waits 5s for the ACK then proceeds anyway
DEFAULT_PORT_MATCH = "amyboard"
SKETCH_PATH = "/user/current/sketch.py"

TAG_ACK = 0x41      # 'A' (followed by 'K')
TAG_ERROR = 0x58    # 'X'
TAG_VERSION = 0x56  # 'V'


def find_port(names, match):
    """Return the first port name containing `match` (case-insensitive)."""
    match = match.lower()
    for n in names:
        if match in n.lower():
            return n
    return None


class SketchError(Exception):
    pass


class AMYboardLink:
    def __init__(self, port_match=DEFAULT_PORT_MATCH, verbose=True):
        self.port_match = port_match
        self.verbose = verbose
        self.inport = None
        self.outport = None
        self._ack = threading.Event()
        self._lock = threading.Lock()
        self.errors = []      # decoded sketch-error tracebacks since last clear
        self.version = None
        self.in_name = None
        self.out_name = None

    # -- connection ---------------------------------------------------------
    def open(self):
        self.in_name = find_port(mido.get_input_names(), self.port_match)
        self.out_name = find_port(mido.get_output_names(), self.port_match)
        if not self.in_name or not self.out_name:
            raise RuntimeError(
                "Could not find an AMYboard MIDI port (match=%r).\n  inputs:  %s\n  outputs: %s"
                % (self.port_match, mido.get_input_names(), mido.get_output_names()))
        self.outport = mido.open_output(self.out_name)
        self.inport = mido.open_input(self.in_name, callback=self._on_message)
        self._log("connected: in=%r out=%r" % (self.in_name, self.out_name))
        return self

    def close(self):
        for p in (self.inport, self.outport):
            try:
                if p:
                    p.close()
            except Exception:
                pass
        self.inport = self.outport = None

    def __enter__(self):
        return self.open()

    def __exit__(self, *a):
        self.close()

    # -- incoming sysex -----------------------------------------------------
    def _on_message(self, msg):
        if msg.type != "sysex":
            return
        data = msg.data  # tuple of the bytes between F0 and F7
        if len(data) < 4 or tuple(data[:3]) != MFR_T:
            return
        tag = data[3]
        if tag == TAG_ACK and len(data) >= 5 and data[4] == 0x4B:
            self._ack.set()
        elif tag == TAG_ERROR:
            try:
                text = base64.b64decode(bytes(data[4:])).decode("utf-8", "replace").strip()
            except Exception:
                text = "<undecodable sketch error frame>"
            self.errors.append(text)
            self._log("  ⚠ sketch error frame received (%d bytes)" % len(text))
        elif tag == TAG_VERSION:
            self.version = bytes(data[4:]).decode("ascii", "replace").strip()

    # -- outgoing -----------------------------------------------------------
    def _send_sysex(self, payload):
        """Send one raw sysex frame; payload is a bytes/list of 7-bit values."""
        self.outport.send(mido.Message("sysex", data=MFR + list(payload)))

    def send_and_ack(self, payload, timeout=ACK_TIMEOUT):
        """Send one sysex frame and wait for the board's ACK. Returns True if acked."""
        with self._lock:
            self._ack.clear()
            self._send_sysex(payload)
            got = self._ack.wait(timeout)
        return got

    def send_python(self, code, timeout=ACK_TIMEOUT):
        """Run one line of python on the board via zP. Returns True if acked."""
        return self.send_and_ack(b"zP" + code.encode("utf-8") + b"Z", timeout=timeout)

    def ping(self, timeout=2.5):
        """Inert liveness check. A runaway sketch loop() starves the MicroPython
        scheduler, so this zP won't get ACKed when the board is wedged."""
        return self.send_and_ack(b"zPpassZ", timeout=timeout)

    # -- high level ---------------------------------------------------------
    def reset_amy(self):
        """Clear all AMY oscillators for a clean slate (no USB re-enumeration)."""
        return self.send_python("import amy; amy.reset()")

    def reboot_bootloader(self):
        """Send zB: reboot into skip-sketch mode. The board's USB re-enumerates;
        no ACK comes back. Follow with wait_for_ready()."""
        self._send_sysex(b"zBZ")

    def recover(self, factory_reset=True, timeout=20):
        """Unstick a board wedged by a runaway sketch loop(). zB is handled in
        pure C on the MIDI task, so it reboots even when loop() is hogging the
        MicroPython scheduler and normal zP/zT can't get through. Reboots skipping
        the bad sketch, waits for re-enumeration, then (optionally) factory_resets
        so a later power-cycle won't re-run the bad sketch.py. Returns True if the
        board came back."""
        self.reboot_bootloader()
        ok = self.wait_for_ready(timeout=timeout)
        if ok and factory_reset:
            self.send_python("import amyboard; amyboard.factory_reset()")
        return ok

    def wait_for_ready(self, timeout=12.0):
        """After a zB reboot, wait for the AMYboard MIDI port to drop and come
        back, then reconnect. Returns True on success."""
        self.close()
        deadline = time.time() + timeout
        # wait for it to disappear (best effort, short)
        t = time.time() + 3.0
        while time.time() < t:
            if not find_port(mido.get_output_names(), self.port_match):
                break
            time.sleep(0.2)
        # wait for it to come back
        while time.time() < deadline:
            if find_port(mido.get_output_names(), self.port_match) and \
               find_port(mido.get_input_names(), self.port_match):
                time.sleep(1.0)  # settle
                try:
                    self.open()
                    return True
                except Exception:
                    pass
            time.sleep(0.3)
        return False

    def transfer_file(self, text, path=SKETCH_PATH, progress=None, max_consecutive_timeouts=3):
        """Send a text file to the board (zT header + base64 chunks). Returns
        (chunks_sent, chunks_acked). Aborts early if the board stops ACKing for
        `max_consecutive_timeouts` frames in a row (it's wedged) — otherwise a
        hung board would make every frame wait out its full 5 s timeout."""
        data = text.encode("utf-8")
        size = len(data)
        header = "zT%s,%dZ" % (path, size)
        ok = self.send_and_ack(header.encode("utf-8"))
        acked = 1 if ok else 0
        sent = 1
        consecutive = 0 if ok else 1
        n_chunks = (size + CHUNK_BYTES - 1) // CHUNK_BYTES
        for i in range(n_chunks):
            if consecutive >= max_consecutive_timeouts:
                break  # board not responding — abort rather than wait out every frame
            chunk = data[i * CHUNK_BYTES:(i + 1) * CHUNK_BYTES]
            b64 = base64.b64encode(chunk)
            ok = self.send_and_ack(b64)
            sent += 1
            if ok:
                acked += 1
                consecutive = 0
            else:
                consecutive += 1
            if progress:
                progress(i + 1, n_chunks)
        return sent, acked

    def environment_transfer_done(self):
        """Tell the board the sketch.py transfer is complete and (re)start it."""
        return self.send_python("import amyboard; amyboard.environment_transfer_done()")

    def restart_sketch(self):
        return self.send_python("import amyboard; amyboard.restart_sketch()")

    def set_volume(self, volume):
        """Set AMY's global output volume (amy.send(volume=...))."""
        return self.send_python("import amy; amy.send(volume=%g)" % float(volume))

    # sync marker: two short high sine pulses, played on a spare AMY oscillator
    # (so it sounds the same regardless of the loaded sketch's patch). Used to
    # locate each sketch inside an externally-recorded phone video.
    BEEP_FREQ = 3520
    BEEP_OSC = 62

    def beep(self, pulses=2, ms=100, gap_ms=80):
        for i in range(pulses):
            self.send_python("import amy; amy.send(osc=%d,wave=0,freq=%d,amp=1,vel=1)"
                             % (self.BEEP_OSC, self.BEEP_FREQ))
            time.sleep(ms / 1000.0)
            self.send_python("import amy; amy.send(osc=%d,vel=0)" % self.BEEP_OSC)
            if i < pulses - 1:
                time.sleep(gap_ms / 1000.0)

    def reset_display(self, mode="logo"):
        """Reset the OLED between sketches so the previous sketch's leftover
        screen contents don't persist.
          'logo'  -> re-init the display and show the AMYboard!!! boot logo
          'clear' -> blank the screen
          'none'  -> do nothing
        """
        if mode == "logo":
            # init_display() makes a fresh (blank) framebuffer and draws the logo.
            return self.send_python("import amyboard; amyboard.init_display()")
        if mode == "clear":
            return self.send_python("import amyboard as A; A.display.clear() if A.display else None")
        return None

    def load_sketch(self, text, reset="amy", error_wait=1.5, progress=None, volume=None,
                    display_reset="logo", liveness_timeout=2.5):
        """Push a sketch and start it, mirroring the web World-import flow.

        reset: 'amy'    -> amy.reset() first (clean slate, no re-enumeration; default)
               'reboot' -> full zB reboot + wait_for_ready (guaranteed clean slate)
               'none'   -> just transfer over whatever is running
        volume: if set, send amy.send(volume=...) AFTER the sketch restarts (so the
                sketch's own init can't clobber it) — a global recording-level boost.
        display_reset: reset the OLED just BEFORE the sketch restarts so leftover
                screen content doesn't carry over ('logo' | 'clear' | 'none').
        Returns a dict: {acked, sent, ready, errors:[traceback,...], wedged}.
        Sketch-load errors arrive asynchronously after the restart, so we wait
        `error_wait` seconds and collect any 'X' frames. `wedged` is True if the
        just-started sketch hung the board (no response to a liveness ping) — the
        caller should recover() and denylist it.
        """
        self.clear_errors()
        ready = True
        if reset == "reboot":
            self.reboot_bootloader()
            ready = self.wait_for_ready()
            if not ready or not self.outport:
                return {"sent": 0, "acked": 0, "ready": False,
                        "errors": ["board did not re-enumerate after reboot; "
                                   "reconnect USB or use --reset amy"]}
        elif reset == "amy":
            self.reset_amy()
        sent, acked = self.transfer_file(text, progress=progress)
        if display_reset and display_reset != "none":
            # before the restart: the new sketch can still draw over it
            self.reset_display(display_reset)
        self.environment_transfer_done()
        if volume is not None:
            self.set_volume(volume)
        time.sleep(error_wait)
        errs = self.drain_errors()
        # watchdog: if the sketch we just started has a runaway loop(), it starves
        # the scheduler and this inert ping won't be ACKed.
        wedged = not self.ping(timeout=liveness_timeout)
        return {"sent": sent, "acked": acked, "ready": ready, "errors": errs, "wedged": wedged}

    # -- notes (make the synth play) ----------------------------------------
    def note_on(self, note, velocity=100, channel=0):
        self.outport.send(mido.Message("note_on", note=note, velocity=velocity, channel=channel))

    def note_off(self, note, channel=0):
        self.outport.send(mido.Message("note_off", note=note, velocity=0, channel=channel))

    def all_notes_off(self, channel=0):
        # CC 123 = all notes off; also blast explicit note_offs as a fallback.
        try:
            self.outport.send(mido.Message("control_change", control=123, value=0, channel=channel))
        except Exception:
            pass

    # -- errors -------------------------------------------------------------
    def clear_errors(self):
        self.errors = []
        self.version = None

    def drain_errors(self):
        errs, self.errors = self.errors, []
        return errs

    def _log(self, msg):
        if self.verbose:
            print(msg)
