# AMYboard for VCV Rack

Phase 1 of the AMYboard VCV Rack port: the **AMY** module embeds the AMY
synthesizer engine (the same core that runs on AMYboard hardware) in-process,
the way the `TULIP_DESKTOP` build does — no web/WASM machinery involved.

## AMY module

- **MIDI in** (right-click the module to pick a MIDI device): drives AMY's
  built-in MIDI handling with the default synths — Juno-6 on channel 1, DX7 on
  channel 2, GM drums (808 PCM) on channel 10. Program changes and CCs work as
  on hardware; sysex is passed through to AMY.
- **CV1 / CV2 inputs**: exposed to AMY as the `ext0`/`ext1` CtrlCoefs and as
  `cv_trigger` sources — the same two ±10V CV inputs AMYboard hardware has.
- **OUT L / OUT R**: AMY's stereo mix (±5V at unity LEVEL).
- AMY renders 256-frame blocks at 44.1kHz; the module resamples to the Rack
  engine rate when they differ.
- AMY is global state, so **one AMY module per patch** owns the engine; extra
  instances stay silent (noted in their context menu).

Planned for later phases: CV outputs (via AMY's `set_cv_synth` path), stereo
audio in (`AUDIO_EXT0/1`), and the full AMYboard experience — embedded
MicroPython running real AMYboard sketches, OLED panel display, encoders, and
sysex control from the amyboard.com editor.

## Building

```
# one-time: bootstrap submodules so amy/ is populated
cd tulip/shared && ./grab_submodules.sh

# Rack SDK from https://vcvrack.com/downloads/ (arch-specific zip)
export RACK_DIR=<path to unzipped Rack-SDK>

cd tulip/vcvrack
make            # build plugin.dylib/.so
make dist       # package a .vcvplugin
make install    # copy into the local Rack2 user plugins folder
```

`RACK_DIR` defaults to `~/outside/Rack-SDK` if unset. The `amy` symlink points
at the repo's `amy` submodule.
