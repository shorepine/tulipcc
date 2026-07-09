"""AMYboardLink — a Python client for the AMYboard SysEx control API.

Implements the full protocol documented in docs/amyboard/control_api.md (the
same one the web editor's spss.js speaks): write files to the board (zT),
read files and dump live synth state back (zD), save state into a sketch (zA),
run Python on the board (zP), sequencer transport (zY), reboot (zB), ping (zI),
plus plain channel-voice MIDI to play the loaded synth.

All control frames are MIDI SysEx framed F0 <mfr> <ascii payload> F7,
mfr = 00 03 45 ("SPSS" — Shorepine Synth Systems).

  Host -> board payloads (see control_api.md for the full table):
    zT<path>,<size>Z         begin a file transfer; size = raw byte count
    <base64>                 one file chunk (<=188 raw bytes, base64), repeated
    zD Z / zD<path>Z         dump synth state / read a file back as SysEx
    zA Z / zA<path>Z         write current AMY state into a sketch on disk
    zP<python>Z              run one line of python on the board (<=255 bytes)
    zY1Z / zY0Z              sequencer start / stop
    zBZ / zB1Z / zB2Z        reboot: skip-sketch / normal / ROM download mode
    zIZ                      ping (replies OK)

  Board -> host frames (tag byte right after the manufacturer ID):
    'A' 'K'                  ACK, sent after the board finishes processing each sysex
    'O' 'K'                  pong — reply to zI
    'X' <b64>                sketch load error: base64 of a python traceback
    'V' <ascii>              firmware version string
    '0' <b64>                dump reply, single frame (whole payload)
    'C' <b64>                dump reply chunk, more follow
    'E' <b64>                dump reply, final chunk

The board ACKs every sysex it processes, so we send one frame and wait for its
ACK before sending the next (single message in flight) — this is what keeps the
board's sysex ring buffer from overflowing. Standard MIDI note on/off are
channel-voice messages (not sysex), are not ACKed, and make the loaded synth play.
"""

import base64
import sys
import threading
import time

from .transport import DEFAULT_PORT_MATCH, board_present, make_transport

MFR = [0x00, 0x03, 0x45]
MFR_T = (0x00, 0x03, 0x45)
CHUNK_BYTES = 188          # AMYBOARD_TRANSFER_CHUNK_BYTES in spss.js
ACK_TIMEOUT = 5.0          # spss.js waits 5s for the ACK then proceeds anyway
DUMP_TIMEOUT = 30.0        # zD replies can be large (sketches with embedded MIDI)
PACE = 0.03                # inter-frame delay when the transport can't hear ACKs
SKETCH_PATH = "/user/current/sketch.py"

TAG_ACK = 0x41       # 'A' (followed by 'K')
TAG_OK = 0x4F        # 'O' (followed by 'K') — zI pong
TAG_ERROR = 0x58     # 'X'
TAG_OVERLOAD = 0x4C  # 'L' — AMY overload failsafe tripped (b64 load percent)
TAG_VERSION = 0x56   # 'V'
TAG_DUMP_ONE = 0x30  # '0' single-frame dump
TAG_DUMP_MID = 0x43  # 'C' chunk, more follow
TAG_DUMP_END = 0x45  # 'E' final chunk


class SketchError(Exception):
    pass


class AMYboardLink:
    """One connection to an AMYboard over USB MIDI.

    port_match: substring matched against MIDI port names (or an ALSA hw:X,Y).
    backend:    'auto' (mido if importable, else ALSA amidi), 'mido', or 'alsa'.
    """

    def __init__(self, port_match=DEFAULT_PORT_MATCH, verbose=True, backend="auto",
                 ack_timeout=ACK_TIMEOUT):
        self.port_match = port_match
        self.verbose = verbose
        self.backend = backend  # resolved by make_transport() at open()
        self.ack_timeout = ack_timeout
        self.transport = None
        self._ack = threading.Event()
        self._ok = threading.Event()
        self._version_evt = threading.Event()
        self._lock = threading.Lock()
        self._dump = bytearray()
        self._dump_done = threading.Event()
        self.errors = []      # decoded sketch-error tracebacks since last clear
        self.overloads = []   # load percents from 'L' overload-failsafe frames
        self.version = None
        self.in_name = None
        self.out_name = None

    # -- connection ---------------------------------------------------------
    def open(self):
        self.transport = make_transport(self.port_match, self.backend)
        self.transport.on_sysex = self._on_sysex
        self.transport.open()
        self.in_name = self.transport.in_name
        self.out_name = self.transport.out_name
        self._log("connected: in=%r out=%r (%s)"
                  % (self.in_name, self.out_name, type(self.transport).__name__))
        return self

    def close(self):
        if self.transport:
            self.transport.close()
        self.transport = None

    def __enter__(self):
        return self.open()

    def __exit__(self, *a):
        self.close()

    # -- incoming sysex -----------------------------------------------------
    def _on_sysex(self, data):
        """data = the bytes between F0 and F7."""
        if len(data) < 4 or tuple(data[:3]) != MFR_T:
            return
        tag = data[3]
        if tag == TAG_ACK and len(data) >= 5 and data[4] == 0x4B:
            self._ack.set()
        elif tag == TAG_OK and len(data) >= 5 and data[4] == 0x4B:
            self._ok.set()
        elif tag == TAG_ERROR:
            try:
                text = base64.b64decode(bytes(data[4:])).decode("utf-8", "replace").strip()
            except Exception:
                text = "<undecodable sketch error frame>"
            self.errors.append(text)
            self._log("  ⚠ sketch error frame received (%d bytes)" % len(text))
        elif tag == TAG_OVERLOAD:
            # amyboard.py's _on_amy_overload: AMY's CPU failsafe tripped on the
            # board (synth reset + bleep already happened, sketch stopped).
            # Always loud — a run that hits this has stopped making the sound
            # under test, whatever the harness thinks it's recording.
            try:
                pct = base64.b64decode(bytes(data[4:])).decode("ascii", "replace").strip()
            except Exception:
                pct = "?"
            self.overloads.append(pct)
            print("[amyboardctl] ⚠ AMY OVERLOAD failsafe tripped on the board "
                  "(load %s%%) — sketch stopped" % pct, file=sys.stderr)
        elif tag == TAG_VERSION:
            self.version = bytes(data[4:]).decode("ascii", "replace").strip()
            self._version_evt.set()
        elif tag in (TAG_DUMP_ONE, TAG_DUMP_MID, TAG_DUMP_END):
            try:
                chunk = base64.b64decode(bytes(data[4:]))
            except Exception:
                chunk = b""
            if tag == TAG_DUMP_ONE:
                # single-frame reply: any accumulated bytes are stale (spss.js
                # resets its accumulator the same way)
                self._dump = bytearray(chunk)
            else:
                self._dump.extend(chunk)
            if tag in (TAG_DUMP_ONE, TAG_DUMP_END):
                self._dump_done.set()

    # -- outgoing -----------------------------------------------------------
    def send_nowait(self, payload):
        """Send one control frame without waiting for the ACK. payload is the
        ASCII command (bytes or str), without the F0/mfr/F7 framing."""
        if isinstance(payload, str):
            payload = payload.encode("utf-8")
        self.transport.send([0xF0] + MFR + list(payload) + [0xF7])

    def send_and_ack(self, payload, timeout=None):
        """Send one control frame and wait for the board's ACK (flow control).
        Returns True if acked. If the transport can't receive (send-only ALSA
        fallback), paces frames with a fixed delay instead and returns True."""
        timeout = self.ack_timeout if timeout is None else timeout
        with self._lock:
            if not self.transport.ack_available:
                self.send_nowait(payload)
                time.sleep(PACE)
                return True
            self._ack.clear()
            self.send_nowait(payload)
            return self._ack.wait(timeout)

    def send_python(self, code, timeout=None):
        """Run one line of python on the board via zP (max 255 bytes). Returns
        True if acked."""
        return self.send_and_ack(b"zP" + code.encode("utf-8") + b"Z", timeout=timeout)

    # -- liveness -----------------------------------------------------------
    def ping(self, timeout=2.5):
        """Inert liveness check via zP. A runaway sketch loop() starves the
        MicroPython scheduler, so this won't get ACKed when the board is wedged
        — which is exactly what makes it a good wedge detector."""
        return self.send_and_ack(b"zPpassZ", timeout=timeout)

    def ping_ok(self, timeout=2.5):
        """zI ping, answered 'OK' in pure C before the scheduler — proves the
        board is alive/connected even if you don't care about the sketch."""
        if not self.transport.ack_available:
            self.send_nowait(b"zIZ")
            return True
        with self._lock:
            self._ok.clear()
            self.send_nowait(b"zIZ")
            return self._ok.wait(timeout)

    def report_version(self, timeout=5.0):
        """Ask the board for its firmware version string (or None on timeout)."""
        self._version_evt.clear()
        self.version = None
        self.send_python("import amyboard; amyboard.report_version()", timeout=timeout)
        self._version_evt.wait(timeout)
        return self.version

    # -- reset / reboot -----------------------------------------------------
    def reset_amy(self):
        """Clear all AMY oscillators for a clean slate (no USB re-enumeration)."""
        return self.send_python("import amy; amy.reset()")

    def restart_sketch(self):
        return self.send_python("import amyboard; amyboard.restart_sketch()")

    def factory_reset(self):
        """Clear /user/current and reload the default sketch."""
        return self.send_python("import amyboard; amyboard.factory_reset()")

    def reboot(self, mode="bootloader"):
        """zB reboot. No ACK comes back; USB re-enumerates (except 'rom').
          'bootloader' -> skip sketch.py on next boot (frees the scheduler)
          'normal'     -> run sketch.py on next boot
          'rom'        -> ESP ROM download / flash mode
        Follow with wait_for_ready() (not after 'rom')."""
        payload = {"bootloader": b"zBZ", "normal": b"zB1Z", "rom": b"zB2Z"}[mode]
        self.send_nowait(payload)

    def reboot_bootloader(self):
        """Send zB: reboot into skip-sketch mode. The board's USB re-enumerates;
        no ACK comes back. Follow with wait_for_ready()."""
        self.reboot("bootloader")

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
            self.factory_reset()
        return ok

    def wait_for_ready(self, timeout=12.0):
        """After a zB reboot, wait for the AMYboard MIDI port to drop and come
        back, then reconnect. Returns True on success."""
        self.close()
        deadline = time.time() + timeout
        # wait for it to disappear (best effort, short)
        t = time.time() + 3.0
        while time.time() < t:
            if not board_present(self.port_match, self.backend):
                break
            time.sleep(0.2)
        # wait for it to come back
        while time.time() < deadline:
            if board_present(self.port_match, self.backend):
                time.sleep(1.0)  # settle
                try:
                    self.open()
                    return True
                except Exception:
                    pass
            time.sleep(0.3)
        return False

    # -- file / state transfer ----------------------------------------------
    def transfer_file(self, text, path=SKETCH_PATH, progress=None, max_consecutive_timeouts=3):
        """Send a file to the board (zT header + base64 chunks). `text` may be
        str or bytes. Returns (chunks_sent, chunks_acked). Aborts early if the
        board stops ACKing for `max_consecutive_timeouts` frames in a row (it's
        wedged) — otherwise a hung board would make every frame wait out its
        full timeout."""
        data = text.encode("utf-8") if isinstance(text, str) else bytes(text)
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
            ok = self.send_and_ack(base64.b64encode(chunk))
            sent += 1
            if ok:
                acked += 1
                consecutive = 0
            else:
                consecutive += 1
            if progress:
                progress(i + 1, n_chunks)
        return sent, acked

    def _collect_dump(self, payload, timeout=DUMP_TIMEOUT):
        """Send one zD-style request and reassemble the C…C E (or single 0)
        reply frames into bytes. Raises TimeoutError if no complete reply."""
        with self._lock:
            self._dump = bytearray()
            self._dump_done.clear()
            self.send_nowait(payload)
            if not self._dump_done.wait(timeout):
                raise TimeoutError("no complete dump reply to %r within %.0fs "
                                   "(got %d bytes)" % (payload, timeout, len(self._dump)))
            return bytes(self._dump)

    def read_file(self, path, timeout=DUMP_TIMEOUT):
        """Read a file off the board's filesystem (zD<path>). Returns bytes."""
        if not self.transport.ack_available:
            raise RuntimeError("this transport is send-only; cannot read from the board")
        if path.endswith("Z"):
            # the trailing Z is the sysex end-of-message marker, so such a
            # filename is not addressable over this protocol
            raise ValueError("filenames ending in 'Z' are not addressable: %r" % path)
        return self._collect_dump(("zD%sZ" % path).encode("utf-8"), timeout=timeout)

    def dump_state(self, timeout=DUMP_TIMEOUT):
        """Dump the full live synth state (zD with no path): every active
        instrument plus global effects, as newline-separated AMY wire-command
        text you could replay with amy.send(...). Returns str."""
        if not self.transport.ack_available:
            raise RuntimeError("this transport is send-only; cannot read from the board")
        return self._collect_dump(b"zDZ", timeout=timeout).decode("utf-8", "replace")

    def save_state_to_sketch(self, path=None):
        """zA: write the board's current AMY state into the _auto_generated_knobs
        block of /user/current/sketch.py (or `path`) on the board's disk."""
        payload = b"zAZ" if not path else ("zA%sZ" % path).encode("utf-8")
        return self.send_and_ack(payload)

    def environment_transfer_done(self):
        """Tell the board the sketch.py transfer is complete and (re)start it."""
        return self.send_python("import amyboard; amyboard.environment_transfer_done()")

    # -- sequencer / volume / display ----------------------------------------
    def sequencer(self, running):
        """Start/stop the step sequencer transport (zY) without MIDI clock."""
        return self.send_and_ack(b"zY1Z" if running else b"zY0Z")

    def set_volume(self, volume):
        """Set AMY's global output volume (amy.send(volume=...))."""
        return self.send_python("import amy; amy.send(volume=%g)" % float(volume))

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

    # -- high level -----------------------------------------------------------
    def load_sketch(self, text, reset="amy", error_wait=1.5, progress=None, volume=None,
                    display_reset="logo", liveness_timeout=2.5):
        """Push a sketch and start it, mirroring the web editor's import flow.

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
            if not ready or not self.transport:
                return {"sent": 0, "acked": 0, "ready": False, "wedged": False,
                        "errors": ["board did not re-enumerate after reboot; "
                                   "reconnect USB or use reset='amy'"]}
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
        wedged = self.transport.ack_available and not self.ping(timeout=liveness_timeout)
        return {"sent": sent, "acked": acked, "ready": ready, "errors": errs, "wedged": wedged}

    # -- notes (make the synth play) ----------------------------------------
    def note_on(self, note, velocity=100, channel=0):
        self.transport.send([0x90 | (channel & 0x0F), note & 0x7F, velocity & 0x7F])

    def note_off(self, note, channel=0):
        self.transport.send([0x80 | (channel & 0x0F), note & 0x7F, 0])

    def note(self, note, velocity=100, duration=0.25, channel=0):
        """Play one note for `duration` seconds (blocking)."""
        self.note_on(note, velocity, channel)
        time.sleep(duration)
        self.note_off(note, channel)

    def all_notes_off(self, channel=0):
        # CC 123 = all notes off
        try:
            self.transport.send([0xB0 | (channel & 0x0F), 123, 0])
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
        # stderr, so CLI commands that stream file contents to stdout stay clean
        if self.verbose:
            print(msg, file=sys.stderr)
