"""amyboardctl — control an AMYboard over USB-MIDI SysEx.

Library + CLI for the AMYboard control API (docs/amyboard/control_api.md):
upload/download sketches and files, dump live synth state, run Python on the
board, drive the sequencer, reboot, ping, play notes.

    from amyboardctl import AMYboardLink
    with AMYboardLink() as link:
        link.load_sketch(open("sketch.py").read())
        link.note(60)

This is the shared client used by tools/amyworld_recorder and
tulip/amyboard/hwci — and the reference implementation for the protocol.
"""

__version__ = "0.1.0"

from .link import (ACK_TIMEOUT, CHUNK_BYTES, MFR, SKETCH_PATH,  # noqa: F401
                   AMYboardLink, SketchError)
from .transport import (DEFAULT_PORT_MATCH, board_present, find_port,  # noqa: F401
                        have_alsa, have_mido, list_ports, pick_backend)
from . import world  # noqa: F401
