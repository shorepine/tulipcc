# amyboardctl

Control an AMYboard over USB-MIDI SysEx from Python or the shell: upload and
download sketches, read files off the board, dump live synth state, run Python
on the board, drive the sequencer, reboot, ping, play notes.

This is a complete client for the AMYboard control API — the same protocol the
[AMYboard Online editor](https://amyboard.com/editor) speaks. The wire format is
documented in [`docs/amyboard/control_api.md`](../../docs/amyboard/control_api.md);
this package is its reference implementation, and is the shared client used by
[`tools/amyworld_recorder`](../amyworld_recorder/) and
[`tulip/amyboard/hwci`](../../tulip/amyboard/hwci/).

## Install

```sh
pip install ./tools/amyboardctl          # from the repo root
# or, for development:
pip install -e ./tools/amyboardctl
```

That installs the `amyboardctl` command and the `amyboardctl` Python package,
with `mido` + `python-rtmidi` for MIDI. On Linux you can skip the Python MIDI
libs entirely (`pip install --no-deps ./tools/amyboardctl`) — the ALSA backend
drives the board with the stock `amidi` CLI instead (this is what the Raspberry
Pi HW-CI runner does). You don't have to install at all inside this repo:
adding `tools/amyboardctl` to `sys.path` is enough (see "Using in-repo" below).

Plug the AMYboard in over USB; it enumerates as a MIDI device named `AMYboard`.

## CLI

```
amyboardctl ports                                # list MIDI ports
amyboardctl ping                                 # zI -> OK: is the board alive?
amyboardctl version                              # firmware version string

amyboardctl upload_sketch sketch.py              # write + (re)start a sketch
amyboardctl upload_sketch sketch.py --reset reboot   # guaranteed-clean slate first
amyboardctl download_sketch -o sketch.py        # read the current sketch back
amyboardctl download_file /user/current/x.py    # read any file off the board
amyboardctl download_state                       # live synth state as AMY wire cmds
amyboardctl save_state                           # zA: write state into sketch.py on the board

amyboardctl run "import amy; amy.send(volume=0.5)"   # run a line of Python (zP)
amyboardctl reset                                # amy.reset() — synth clean slate, no reboot
amyboardctl restart                              # reload & restart sketch.py
amyboardctl factory_reset                        # clear /user/current, default sketch
amyboardctl reboot                               # zB1: reboot, run sketch.py
amyboardctl reboot --mode bootloader             # zB: reboot, skip sketch.py
amyboardctl seq start                            # sequencer transport (zY)
amyboardctl note 60 --duration 0.5               # play middle C on the loaded synth

amyboardctl world list --tag drums               # browse AMYboard World
amyboardctl world get shorepine/woodpiano -o woodpiano.py
```

Global options (before the command): `-p/--port` to match a different MIDI port
name (or an ALSA `hw:X,Y`), `--backend {auto,mido,alsa}`, `-q/--quiet`.

If a sketch fails on the board, the board pushes the full Python traceback back
over SysEx; `upload_sketch`, `run` and `restart` print it on stderr and exit
non-zero.

## Library

```python
from amyboardctl import AMYboardLink

with AMYboardLink() as link:                 # finds the "AMYboard" MIDI port
    result = link.load_sketch(open("sketch.py").read())   # upload + restart
    if result["errors"]:
        print("\n".join(result["errors"]))   # tracebacks from the board

    link.note(60, velocity=100, duration=0.5)   # play the loaded synth
    state = link.dump_state()                # live AMY state as wire commands
    src = link.read_file("/user/current/sketch.py")        # bytes
    link.send_python("import sequencer; sequencer.tempo(120)")
    link.recover()                           # unstick a wedged board (zB reboot)
```

Everything in the CLI is a one-call method on `AMYboardLink`; see
[`amyboardctl/link.py`](amyboardctl/link.py) for the full surface, and
`amyboardctl.world` for the AMYboard World HTTP client (stdlib-only).

### Using in-repo (no install)

```python
import os, sys
sys.path.insert(0, os.path.join(repo_root, "tools", "amyboardctl"))
from amyboardctl import AMYboardLink
```

### Backends

* **mido** (default when importable) — cross-platform, needs `mido` +
  `python-rtmidi`.
* **alsa** — Linux-only, shells out to `amidi` for sends and holds a persistent
  `amidi -d` reader for the board's replies. Zero Python MIDI deps, no root.

Both backends implement the same flow control: one SysEx frame in flight,
waiting for the board's `AK` acknowledgement before sending the next — which is
what keeps the board's SysEx ring buffer from overflowing during multi-frame
transfers.
