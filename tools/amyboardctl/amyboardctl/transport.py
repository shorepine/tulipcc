"""MIDI transports for the AMYboard SysEx control API.

Two interchangeable backends:

  * MidoTransport — mido + python-rtmidi. Cross-platform; the default wherever
    mido is importable (macOS dev machines, most desktops).
  * AlsaTransport — the ALSA CLI tools (`amidi`). No Python MIDI libraries or
    root needed; the backend the Raspberry Pi HW-CI runner uses. Sends are
    one-shot `amidi -S`; receives are a persistent line-buffered `amidi -d`
    reader parsed into SysEx frames.

A transport knows nothing about the AMYboard protocol. It moves raw MIDI:

  transport.send(bytes_or_list)   # one complete MIDI message, status byte first
  transport.on_sysex = fn         # fn(payload: bytes) — the bytes between F0/F7
  transport.ack_available         # False if we can send but not receive

Port selection: `port` is a case-insensitive substring matched against the
available port names ("amyboard" matches "AMYboard:AMYboard MIDI 1 24:0"), or —
ALSA only — a literal `hw:X,Y[,Z]` device which is used as-is.
"""

import shutil
import subprocess
import sys
import threading
import time

DEFAULT_PORT_MATCH = "amyboard"


def find_port(names, match):
    """Return the first port name containing `match` (case-insensitive)."""
    match = match.lower()
    for n in names:
        if match in n.lower():
            return n
    return None


def have_alsa():
    return bool(shutil.which("amidi"))


def have_mido():
    try:
        import mido  # noqa: F401
        return True
    except ImportError:
        return False


def pick_backend(backend="auto"):
    """Resolve 'auto' to 'mido' or 'alsa'. Prefers mido when importable."""
    if backend in ("mido", "alsa"):
        return backend
    if have_mido():
        return "mido"
    if have_alsa():
        return "alsa"
    raise RuntimeError(
        "No MIDI backend available: `pip install mido python-rtmidi`, "
        "or install ALSA's `amidi` (Linux).")


def make_transport(port=DEFAULT_PORT_MATCH, backend="auto"):
    backend = pick_backend(backend)
    return AlsaTransport(port) if backend == "alsa" else MidoTransport(port)


def board_present(port_match=DEFAULT_PORT_MATCH, backend="auto"):
    """Return the board's port name if its MIDI port is enumerated, else None.
    Cheap — safe to poll while waiting out a reboot."""
    backend = pick_backend(backend)
    if backend == "alsa":
        return _alsa_find(port_match)
    import mido
    out = find_port(mido.get_output_names(), port_match)
    inp = find_port(mido.get_input_names(), port_match)
    return out if (out and inp) else None


def list_ports(backend="auto"):
    """Return {'inputs': [...], 'outputs': [...]} of available MIDI ports."""
    backend = pick_backend(backend)
    if backend == "alsa":
        out = subprocess.run(["amidi", "-l"], capture_output=True, text=True).stdout
        devs = [line.split(None, 2) for line in out.splitlines()[1:] if line.strip()]
        names = ["%s  %s" % (d[1], d[2] if len(d) > 2 else "") for d in devs if len(d) >= 2]
        return {"inputs": names, "outputs": names}
    import mido
    return {"inputs": mido.get_input_names(), "outputs": mido.get_output_names()}


def _alsa_find(match):
    """Find an ALSA rawmidi device (hw:X,Y,Z) whose name contains `match`."""
    out = subprocess.run(["amidi", "-l"], capture_output=True, text=True).stdout
    for line in out.splitlines():
        if match.lower() in line.lower():
            parts = line.split()
            if len(parts) >= 2:
                return parts[1]
    return None


class MidoTransport:
    def __init__(self, port=DEFAULT_PORT_MATCH):
        self.port_match = port
        self.on_sysex = None
        self.ack_available = False
        self.in_name = self.out_name = None
        self._mido = None
        self._in = self._out = None

    def open(self):
        import mido
        self._mido = mido
        self.in_name = find_port(mido.get_input_names(), self.port_match)
        self.out_name = find_port(mido.get_output_names(), self.port_match)
        if not self.in_name or not self.out_name:
            raise RuntimeError(
                "Could not find an AMYboard MIDI port (match=%r).\n  inputs:  %s\n  outputs: %s"
                % (self.port_match, mido.get_input_names(), mido.get_output_names()))
        self._out = mido.open_output(self.out_name)
        self._in = mido.open_input(self.in_name, callback=self._on_message)
        self.ack_available = True
        return self

    def close(self):
        for p in (self._in, self._out):
            try:
                if p:
                    p.close()
            except Exception:
                pass
        self._in = self._out = None
        self.ack_available = False

    def _on_message(self, msg):
        if msg.type == "sysex" and self.on_sysex:
            self.on_sysex(bytes(msg.data))

    def send(self, data):
        self._out.send(self._mido.Message.from_bytes(list(data)))


class AlsaTransport:
    """`amidi` CLI backend. `port` may be a literal hw:X,Y[,Z] or a name match."""

    SEND_RETRIES = 3   # `amidi -S` can exit non-zero for a beat right after the
                       # board re-inits its USB-MIDI endpoint (e.g. post-restart)

    def __init__(self, port=DEFAULT_PORT_MATCH):
        self.port_match = port
        self.port = None
        self.on_sysex = None
        self.ack_available = False
        self.in_name = self.out_name = None
        self._reader = None
        self._closing = False

    def open(self):
        self.port = self.port_match if self.port_match.startswith("hw:") \
            else _alsa_find(self.port_match)
        if not self.port:
            raise RuntimeError("Could not find an AMYboard ALSA MIDI device "
                               "(match=%r); see `amidi -l`." % self.port_match)
        self.in_name = self.out_name = self.port
        try:
            self._spawn_reader()
        except Exception as e:
            print("[amyboardctl] no amidi read stream (%s); send-only" % e,
                  file=sys.stderr)
        return self

    def _spawn_reader(self):
        # Persistent receive stream: line-buffered so frames arrive in real time
        # (block buffering would sit on the board's ACKs). Coexists fine with the
        # one-shot `amidi -S` sends on the same rawmidi device.
        self._reader = subprocess.Popen(
            ["stdbuf", "-oL", "amidi", "-p", self.port, "-d"],
            stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True, bufsize=1)
        threading.Thread(target=self._read_loop, daemon=True).start()
        self.ack_available = True

    def close(self):
        self._closing = True
        if self._reader:
            try:
                self._reader.terminate()
            except Exception:
                pass
            self._reader = None
        self.ack_available = False

    def _read_loop(self):
        """Parse the `amidi -d` hex dump into SysEx frames and dispatch each
        frame's payload (the bytes between F0 and F7) to on_sysex."""
        reader = self._reader
        buf, insx = [], False
        try:
            for line in reader.stdout:
                for tok in line.split():
                    try:
                        b = int(tok, 16)
                    except ValueError:
                        continue
                    if b == 0xF0:
                        insx, buf = True, []
                    elif b == 0xF7:
                        if insx and self.on_sysex:
                            self.on_sysex(bytes(buf))
                        insx, buf = False, []
                    elif insx:
                        buf.append(b)
        except Exception:
            pass
        # `amidi -d` dies when the board re-enumerates (its rawmidi node goes
        # away). That used to be silent — every later send_and_ack just timed
        # out and transfers aborted with no trace. Respawn it once the device
        # is back; if it never comes back, drop to send-only pacing and say so.
        if self._closing:
            return
        print("[amyboardctl] amidi read stream died (rc=%s); respawning"
              % reader.poll(), file=sys.stderr)
        self.ack_available = False
        deadline = time.time() + 15.0
        while not self._closing and time.time() < deadline:
            time.sleep(1.0)
            port = self.port_match if self.port_match.startswith("hw:") \
                else _alsa_find(self.port_match)
            if not port:
                continue
            self.port = self.in_name = self.out_name = port
            try:
                self._spawn_reader()
                print("[amyboardctl] amidi read stream back on %s" % port,
                      file=sys.stderr)
                return
            except Exception:
                continue
        if not self._closing:
            print("[amyboardctl] amidi read stream gone for good; "
                  "continuing send-only (paced)", file=sys.stderr)

    def send(self, data):
        cmd = ["amidi", "-p", self.port, "-S", " ".join("%02X" % b for b in data)]
        for attempt in range(self.SEND_RETRIES + 1):
            r = subprocess.run(cmd, capture_output=True, text=True)
            if r.returncode == 0:
                return
            if attempt < self.SEND_RETRIES:
                time.sleep(0.1)
        # A dropped frame must not abort a long transfer/capture: warn + continue.
        print("[amyboardctl] amidi -S failed after %d tries (%s); continuing"
              % (self.SEND_RETRIES + 1, (r.stderr or "").strip()), file=sys.stderr)
