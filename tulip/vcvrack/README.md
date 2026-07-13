# AMYboard for VCV Rack

AMYboard as a Rack module: the **AMY** module embeds the real AMYboard
firmware — MicroPython running the same frozen `amyboard.py` / `_boot.py` /
`midi.py` / `amy` package as the hardware — with the AMY synth engine
in-process. `tulip.board()` returns `"AMYBOARD_VCV"`, and the firmware
branches on that exactly as it does for `AMYBOARD` / `AMYBOARD_WEB` /
`DESKTOP`.

## The AMY module

- **Sketch**: on first run the firmware creates
  `~/Documents/AMYboard/user/current/sketch.py` (same layout as hardware).
  Edit it in any editor, then right-click the module → **Restart sketch**
  (or use "Open sketch folder"). The sketch's `loop(step)` is scheduled by
  AMY's sequencer every 32nd note, bar-locked, exactly as on the board.
- **OLED**: the 128×128 display renders on the panel; `amyboard.display`
  works as on hardware (text, lines, `draw_waveform()`, …).
- **Encoder**: one endless knob + push button (like the web simulator),
  readable via `amyboard.encoder()` (`type == "vcv"`) or the legacy
  `read_encoder()` / `read_buttons()` helpers.
- **AUDIO IN L/R**: host audio fed to AMY as the external input — create an
  oscillator with `wave=amy.AUDIO_EXT0/1` in a sketch to route it through
  AMY's filters and effects.
- **MIDI in** (right-click to pick a device): drives AMY's MIDI engine —
  Juno-6 on ch 1, GM drums (Gamma9001 TR-808 kit) on ch 10 by default —
  and Python `midi.add_callback()` fires as on hardware. Sysex passes
  through.
- **CV1/CV2 inputs** (±10 V): AMY `ext0`/`ext1` CtrlCoefs, `cv_trigger`
  sources, `amyboard.cv_in()`.
- **CV1/CV2 outputs**: `amyboard.set_cv_out(channel, synth)` routes a
  synth's audio to the jack (the hardware DAC path re-pointed at Rack);
  `amyboard.cv_out(volts, channel)` sets a static voltage.
- **AUDIO OUT L/R**: AMY's stereo mix (±5 V). AMY renders 256-frame blocks
  at 44.1 kHz, resampled to/from the Rack engine rate. The panel is 10HP,
  same as the hardware.
- One AMY module per patch owns the engine (AMY + MicroPython are global);
  extra instances stay silent.
- **amyboard.com works against the module** (macOS): the plugin publishes a
  virtual CoreMIDI in/out pair named **"AMYboard VCV"**. Open
  [amyboard.com](https://amyboard.com), switch to control mode, and pick the
  AMYboard VCV ports — sketch write (zT), knobs (zA), run-python (zP), dumps
  (zD) and ping (zI) all behave as on hardware, ACK'd per frame. Wire paths
  like /user/current/sketch.py are relocated onto ~/Documents/AMYboard/.
  (Don't also select "AMYboard VCV" as the module's Rack MIDI input — that
  would loop the editor traffic through the note path twice.)

## Building

```
# one-time: bootstrap submodules and generate the drum kit data
cd tulip/shared && ./grab_submodules.sh
cd ../../amy && python3 -m amy.headers gamma9001

# Rack SDK from https://vcvrack.com/downloads/ (arch-specific zip)
export RACK_DIR=<path to unzipped Rack-SDK>

cd tulip/vcvrack
make            # builds build-mp/libamyboardmp.a (MicroPython+firmware+AMY), then the plugin
make dist       # package a .vcvplugin
make install    # copy into the local Rack2 user plugins folder
```

`RACK_DIR` defaults to `~/outside/Rack-SDK`. Two-stage build: `Makefile.mp`
drives MicroPython's own build system (qstr generation, frozen manifest —
see `manifest.py`, which mirrors Tulip Desktop's) into a static archive;
Rack's `plugin.mk` compiles the module and links it. The `amy` symlink
points at the repo's amy submodule.

## Architecture notes

- The `AMYBOARD_VCV` build arm lives in `tulip/shared/modtulip.c`,
  `amy_connector.c`, `tulip_helpers.c`, `desktop/unix_mphal.c` and the
  Python firmware (`_boot.py`, `tulip.py`, `amyboard.py`).
- `src/mp_embed.c` is the headless analogue of Tulip Desktop's `main.c`:
  MicroPython on its own pthread (GIL off), VFS-posix at `/`, frozen
  `_boot.py`, then a pump loop draining `mp_handle_pending()` and a queue
  of Python strings from the Rack thread (`amyboard_vcv_exec`).
- Rack's audio thread pulls AMY blocks (`amy_simple_fill_buffer`) and
  injects MIDI; AMY's sequencer hook posts Python callbacks via
  `mp_sched_schedule` from that thread, same as hardware's render task.
