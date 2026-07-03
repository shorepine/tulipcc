# AMYboard World recorder

Tools to audition and record **every AMYboard World sketch** for a video: pull each
sketch from the World API, push it onto a USB-connected AMYboard (the same SysEx
"write to sketch" flow the web editor uses), record the iPhone (Continuity Camera) +
a Mac audio input to a per-sketch clip, mark it good / not interesting / not working,
then stitch the good ones into a montage with title/username/description overlays.

Two scripts:

- **`record.py`** — interactive capture session (one clip per sketch).
- **`stitch.py`** — build the final montage from the good clips with ffmpeg; also
  lists the broken sketches and the Python errors they hit.

State lives in `<out>/session.json`, so you can stop any time and resume later —
already-handled sketches are skipped.

## Setup

```bash
cd tools/amyworld_recorder
python3.13 -m venv .venv
./.venv/bin/pip install -r requirements.txt
```

`ffmpeg`/`ffprobe` must be on `PATH` (`brew install ffmpeg`).

The `./amyworld` wrapper runs either script with the venv:
`./amyworld record …`, `./amyworld stitch …` (or call `./.venv/bin/python record.py …`).

## Physical setup

- AMYboard in your eurorack, connected to the Mac over USB (shows up as MIDI port
  `AMYboard`).
- iPhone pointed at it, connected via Continuity Camera (appears as an AVFoundation
  video device).
- AMYboard audio routed into a Mac audio input device (e.g. `Model 16`, `BlackHole`).
  Video (iPhone) is captured with ffmpeg; **audio is captured with sounddevice
  (PortAudio/CoreAudio), not ffmpeg**, then the two are muxed when you mark the
  clip. This matters: ffmpeg's avfoundation audio input slips samples against the
  device's USB clock — inaudible in silence but it **clicks on every signal**.
  PortAudio follows the device clock exactly (measured: 0 glitches vs ~70 per take
  with ffmpeg). A/V sync is approximate — fine for a synth-module shot; the audio
  is the take that matters.

  Note: ffmpeg-avfoundation and sounddevice use **different device numbering**, so
  `audio_device` is matched against the sounddevice list — check `--list-devices`,
  which prints both.
- If the audio device is a multi-channel mixer/interface and the AMYboard is on a
  specific channel pair (e.g. Tascam **Model 16 channels 13/14**), set
  `audio_channels` (1-indexed) so only those channels are recorded:
  `"audio_device": "Model 16", "audio_channels": [13, 14]`. Behind the scenes this
  records the whole device and extracts those channels with an ffmpeg `pan` filter
  (`pan=stereo|c0=c12|c1=c13`). One channel → dual-mono. Omit to capture the
  device's default stereo. To find the right pair, capture all channels once and
  check levels:
  `ffmpeg -f avfoundation -i "none:Model 16" -t 5 all.wav` then
  `ffmpeg -i all.wav -af astats=measure_overall=none:measure_perchannel=Peak_level -f null -`.

## 1. Find your devices

```bash
./amyworld record --list-devices
```

Note the iPhone video device name and your audio device name. Copy
`config.example.json` to `config.json` and set `video_device` / `audio_device`
(name substring or numeric index both work):

```json
{ "video_device": "iPhone", "audio_device": "Model 16", "out_dir": "session_out" }
```

## 2. Record

```bash
./amyworld record --config config.json
```

For each sketch it downloads the code, starts the clip recording, pushes the sketch to
the board, and shows:

```
[12/200] id=570  very_basic_pad_dx7.py  by generator
  Very basic pad — DX7 strings patch …
  ● recording -> clips/00570_very_basic_pad_dx7.mov
  transfer: 4/4 frames acked
  [g]ood  [n]ot interesting  [x] not working  |  [p]lay pattern  [s]top  [r]e-transfer  [i]nfo  [q]uit & save
```

Single keypress (no Enter):

| key | action |
|-----|--------|
| `p` | play a random performance over MIDI so the synth sounds — walks a diatonic chord progression, alternating an arpeggio phase with a block chord (pattern, chord, pattern, chord…), with a random root/scale/direction/tempo each time |
| `s` | stop the pattern |
| `g` | mark **good**, stop recording, save, next |
| `n` | mark **not interesting**, next |
| `x` | mark **not working**, next |
| `r` | re-transfer the sketch to the board |
| `i` | print the sketch's Python source |
| `d` | **denylist** this sketch (e.g. it hangs the board) — discards its clip and skips it on every future run |
| `q` | stop the session (saves everything; this sketch stays pending) |

### Denylist

Some sketches hang or crash the board. Denylisted ids are **never transferred**
again. The list lives in `<out>/denylist.json` (hand-editable). Manage it without
the board/camera connected:

```bash
./amyworld record --deny 574 --deny-reason "hangs the board"   # add id(s): 574 or 574,575
./amyworld record --list-denied                                # show the denylist
./amyworld record --undeny 574                                 # remove id(s)
```

These run and exit. After denylisting, just start recording again as usual — the
denied sketches are skipped. You can also press `d` during a session to denylist
the current sketch on the fly.

If a sketch fails to load, the board sends its **full Python traceback back over MIDI**
and it's shown inline and saved — `x` is suggested automatically.

Useful flags: `--tag official`, `--q reverb`, `--limit 50`, `--redo`,
`--redo-status not_working` (retry only the broken ones), `--autoplay`,
`--reset amy|reboot|none` (clean-slate strategy; `amy` resets the synth without a USB
reboot — the default), `--amy-volume N`.

**OLED between sketches** — by default each sketch load resets the OLED to the
**AMYboard!!!** boot logo first (`oled_reset: "logo"`), so a previous sketch's
leftover screen content doesn't carry over and confuse the shot. A sketch that
draws its own screen still does (the reset happens just before it starts). Use
`oled_reset: "clear"` to blank it instead, or `"none"` to leave it alone
(`--oled-reset logo|clear|none`).

**Recording level** — set `amy_volume` (config) or `--amy-volume N` to send
`amy.send(volume=N)` right after each sketch starts, as a global boost so quiet
patches record at a usable level. It's applied *after* the sketch's own init so it
isn't clobbered. On our bench `volume=5` brought a default pad from ≈−24 dBFS to
≈−15 dBFS on the captured channels. Leave it unset to keep each sketch's native
level.

### Watchdog (automatic)

Every load ends with an inert liveness ping. If the just-started sketch has a
runaway `loop()` that hangs the board, the ping gets no ACK → the recorder
**auto-recovers** (`zB` reboot + factory reset), **denylists** that sketch, marks
it `not_working`, and moves on — so one bad sketch can't stall an unattended
session. The transfer also aborts early instead of waiting out a 5 s timeout on
every frame when the board is unresponsive. Disable the auto-denylist with
`--no-denylist-on-wedge` (it still recovers, just doesn't denylist).

### Recovering a wedged board (manual)

Some sketches have a runaway `loop()` that eats the CPU, so the board stops
accepting new sketches (`zP`/`zT` can't get scheduler time). `zB` is handled in
pure C on the MIDI task, so it reboots the board **even when the loop is hogging
the scheduler**. To unstick it:

```bash
./amyworld record --recover            # zB reboot (skips the bad sketch) + factory reset to the default sketch
./amyworld record --recover --no-factory-reset   # just free it; leave sketch.py as-is
```

`--recover` reboots skipping the bad sketch, waits for USB to re-enumerate, and
(by default) overwrites `sketch.py` with the safe default so a later power-cycle
won't re-run the bad one. Then start recording again as usual. Denylist the
offender so it isn't reloaded: `./amyworld record --deny <id>`. If `--recover`
reports the board didn't come back, physically power-cycle (unplug/replug USB).

## 3. Review the broken ones

```bash
./amyworld stitch broken --session session_out     # ids + captured Python errors
./amyworld stitch list   --session session_out --status good
```

## External-video mode (phone films the whole session)

If Continuity capture flickers the OLED (an iPhone shutter/exposure issue that
can't be fixed through ffmpeg), film the session on the phone with a manual-shutter
app (~1/60 s) instead, and let the recorder sync it up afterward.

How it works: the recorder captures only the **clean Model 16 audio** per sketch
(no video) and plays a two-pulse **sync beep** on the AMYboard at each sketch. That
beep lands in both the clean clip and the phone's audio, so the stitcher finds each
sketch's exact position in the long phone video by detecting the beeps.

```bash
# 1. record — pass --external-video (or "capture_video": false in config)
./amyworld record --config config.json --external-video
#    → it prompts you to START the phone filming, then run the session as usual.
#      Keep the phone recording running the WHOLE time; keep the AMYboard audible
#      to the phone (even roomy audio is fine — it's only used to find the beeps).

# 2. build — point it at your phone video; it syncs by beep and uses the clean audio
./amyworld stitch sync --session session_out --phone-video ~/Movies/session.mov --out montage.mp4
```

**Multiple runs / multiple videos.** The session and its beep counter persist, so
you can stop and resume across several sittings — but start a **fresh phone
recording for each run** (it's a new video). At stitch time, pass **all** the
videos **in run order**; each beep lands in exactly one video, so they line up 1:1
with the recorded beeps:

```bash
./amyworld stitch sync --session session_out \
    --phone-video ~/Movies/run1.mov ~/Movies/run2.mov ~/Movies/run3.mov \
    --out montage.mp4
```

`sync` prints the beeps found per video and the total vs recorded — if they match,
alignment is exact; a mismatch means a missed/extra beep shifted things (rare).
Everything else (overlays, denylist, watchdog, OLED reset, volume) works the same.
`--tail N` trims N seconds off each clip's end.

## 4. Build the montage (Continuity/on-Mac capture)

```bash
./amyworld stitch plan   --session session_out      # writes session_out/clips.json
# edit clips.json: reorder, set "enabled": false to drop one,
#                  set "in"/"out" (seconds) to trim each clip
./amyworld stitch render --session session_out --out montage.mp4
```

`plan` lists every **good** clip in capture order, pre-filling each `in` point with the
moment you first hit `p` (so the silent transfer head is skipped). Reorder / disable /
trim freely, then `render`.

Render flags: `--portrait` (1080×1920), `--width/--height`, `--fps`, `--title-size`,
`--meta-size`, `--title-seconds N` (show the overlay only for the first N seconds),
`--font /path/to.ttf`.

## How the transfer works

Verified against the tree (`spss.js`, `modtulip.c`, `amy/src/transfer.c`,
`amyboard.py`). All control frames are MIDI SysEx `F0 00 03 45 … F7`:

- `zT<path>,<size>Z` then base64 chunks (≤188 raw bytes each) → writes
  `/user/current/sketch.py`; `zP import amyboard; amyboard.environment_transfer_done()Z`
  restarts the sketch.
- The board ACKs every SysEx (`F0 00 03 45 'A' 'K' F7`); we wait for each ACK before
  sending the next frame (one in flight), exactly like the web editor.
- Sketch load errors come back as `F0 00 03 45 'X' <base64 traceback> F7`.
- Patterns play via ordinary MIDI note on/off on channel 1.

The board link and the World API client live in
[`tools/amyboardctl`](../amyboardctl/) — the shared AMYboard control library +
CLI (`amyboardctl upload_sketch …`), also used by the HW CI. The recorder picks
it up from the repo automatically (no install needed); the full wire protocol is
documented in [`docs/amyboard/control_api.md`](../../docs/amyboard/control_api.md).
