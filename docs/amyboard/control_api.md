# Controlling AMYboard over MIDI SysEx

AMYboard exposes a small control protocol over **USB MIDI System Exclusive
(SysEx)** messages. It's the same protocol the [AMYboard Online editor](https://amyboard.com/editor)
uses to write sketches to the board, read files back, dump synth state, run
Python, and reboot. Because it's plain MIDI, you can drive it from any language
with a MIDI library — this page documents the wire format so you can write your
own scripts and programs.

> If you just want to write Python on the board interactively, use the REPL /
> `mpremote` (see [Using Python](python.md)). This control API is for *programs*
> that talk to the board over MIDI — uploaders, automation, test harnesses, CI.

A complete Python reference implementation lives in the repo at
[`tools/amyboardctl`](https://github.com/shorepine/tulipcc/tree/main/tools/amyboardctl)
— an installable library (`from amyboardctl import AMYboardLink`) with a
command-line front end:

```
pip install ./tools/amyboardctl
amyboardctl upload_sketch sketch.py
amyboardctl download_sketch -o sketch.py
amyboardctl download_state
amyboardctl run "import amy; amy.send(volume=0.5)"
amyboardctl reboot
```

It's the shared client behind both the AMYboard World recorder
(`tools/amyworld_recorder`) and the hardware CI (`tulip/amyboard/hwci`).

---

## The envelope

Every control frame is one MIDI SysEx message with AMYboard's manufacturer ID
`00 03 45`:

```
F0  00 03 45  <ASCII payload ...>  F7
```

- `F0` … `F7` are the standard SysEx start/end bytes.
- `00 03 45` is the manufacturer ID ("SPSS" — Shorepine Sound Systems). The board
  ignores any SysEx that doesn't start with it.
- The payload is plain 7-bit ASCII. Binary data (file contents, samples, state
  dumps) is **base64-encoded** so it stays inside the 7-bit SysEx range.

The board appears as a MIDI port named **`AMYboard`** (input *and* output — the
same port carries control frames both ways and ordinary note data). Connect to
both directions of that port.

Ordinary **MIDI channel-voice messages** (note on/off, control change, program
change, pitch bend) are *not* SysEx — send them as normal MIDI to play the loaded
synth. See [Playing notes](#playing-notes).

---

## Flow control: wait for the ACK

The board **acknowledges every SysEx control frame it processes** by sending back:

```
F0 00 03 45 'A' 'K' F7        (i.e. F0 00 03 45 41 4B F7)
```

The ACK is sent *after* the message is fully handled, not just received. **Send
one control frame, wait for its `AK`, then send the next.** Keeping a single frame
in flight is what prevents the board's SysEx ring buffer from overflowing during
multi-frame transfers. Use a timeout (the web editor uses 5 s) and proceed if no
ACK arrives, but under normal conditions every frame is acked promptly.

`zB` (reboot) and `zI` (ping) are handled in pure C before the ACK path, so they
do **not** produce an `AK` — `zI` produces an `OK` reply (below) and `zB` produces
nothing (the board reboots). Channel-voice MIDI is not acked.

---

## Command reference (host → board)

All payloads below sit inside the `F0 00 03 45 … F7` envelope. A trailing `Z` is
an end-of-message marker that several commands accept/strip.

| Payload | Meaning |
|---|---|
| `zT<path>,<size>Z` | **Begin a file write** to `<path>` (e.g. `/user/current/sketch.py`). `<size>` is the raw byte count. Stops the running sketch, then expects base64 data chunks (next rows). |
| *(base64)* | **One file/sample data chunk.** During a `zT` (or sample) transfer, send the file bytes in ≤188-byte raw chunks, each base64-encoded, one chunk per SysEx frame, until `<size>` bytes have been delivered. |
| `zD Z` | **Dump all synth state** back to the host as SysEx (see [Reading state](#reading-amy-state-zd)). |
| `zD<path>Z` | **Read a file** off the board's filesystem, streamed back as SysEx. |
| `zP<python>Z` | **Run a line of Python** on the board. e.g. `zPimport amyboard; amyboard.restart_sketch()Z`. |
| `zY1Z` / `zY0Z` | **Sequencer transport** — start / stop the step sequencer without MIDI clock. |
| `zB Z` / `zB0Z` | **Reboot to bootloader** — skip `sketch.py` on next boot (frees the scheduler). USB re-enumerates. |
| `zB1Z` | **Reboot normally** — run `sketch.py` on next boot. |
| `zB2Z` | **Reboot to ROM download / flash mode.** |
| `zIZ` | **Ping** — board replies `OK` (below). |
| `z<preset>,<len>,<sr>,<note>,<loopstart>,<loopend>Z` | **Load a PCM sample** into a memory preset, followed by raw sample data chunks. `<len>`=0 unloads the preset. (Advanced; see `amy.load_sample`.) |
| `zF<preset>,<path>,<note>Z` | **Load a PCM preset from a WAV file** already on the board's filesystem. |
| `zS<preset>,<bus>,<frames>,<note>,<loopstart>,<loopend>Z` / `zOZ` | **Sample audio from a bus** into a preset / **stop** sampling. |

You can also send **AMY `send` commands** (the human-readable config protocol, e.g.
`amy.send(synth=1, num_voices=6, patch=22)`) by `zP`-running those commands, although most real-time control is handled more directly
by sending MIDI notes/CCs (assuming CCs have been set up accordingly).  If you want to send
raw wire commands (e.g. `i1nv6K22Z`), you just send them as the encoded sysex payload directly; no need for any `z` wire code wrapping. 

---

## Response reference (board → host)

All replies use the same envelope, with a single-byte **tag** right after the
manufacturer ID: `F0 00 03 45 <tag> <data...> F7`.

| Tag (ASCII / hex) | Meaning |
|---|---|
| `A` `K` (`41 4B`) | **ACK** — the preceding control frame was processed. (Flow control.) |
| `O` `K` (`4F 4B`) | **Pong** — reply to `zI`; the board is alive and ready. |
| `X` (`58`) | **Sketch error** — `<data>` is base64 of a Python traceback (with file/line numbers). Sent when a sketch fails to load/run. |
| `V` (`56`) | **Firmware version** — `<data>` is an ASCII version string (e.g. `20260627-abc1234`). Sent in reply to `amyboard.report_version()`. |
| `0` (`30`) | **Dump: single frame** — `<data>` is base64 of the *entire* `zD` payload (it all fit in one frame). |
| `C` (`43`) | **Dump: chunk, more follow** — `<data>` is base64 of one chunk of a multi-frame `zD` dump. |
| `E` (`45`) | **Dump: final chunk** — last frame of a multi-frame `zD` dump. |

To reassemble a `zD` dump: collect frames until you see a `0` (the only frame) or
an `E` (the last of several `C` frames), base64-decode each frame's data, and
concatenate in order.

---

## Recipes

### Write a sketch to the board (`zT`)

This is the "write to sketch" flow the web editor uses.

1. `zT/user/current/sketch.py,<size>Z` — wait for `AK`.
2. For each ≤188-byte slice of the UTF-8 file bytes: base64-encode it, send it,
   wait for `AK`. Repeat until all `<size>` bytes are sent.
3. `zPimport amyboard; amyboard.environment_transfer_done()Z` — restarts the
   sketch with the new file. (Or `amyboard.restart_sketch()`.)

The receiver base64-decodes each chunk and appends to the file; when `<size>`
bytes have arrived it closes the file and the transfer is done.

### Read a file from the board (`zD<path>`)

`zD/user/current/sketch.py Z` (no space; shown spaced for clarity) → the board
streams the file back as `C…C E` (or a single `0`) frames. Decode and concatenate
each frame's base64 to get the file bytes.

> Filenames whose **last character is `Z`** are not addressable (the trailing `Z`
> is treated as the end marker). Interior `Z`s (e.g. `/ZIP.py`) are fine.

### Read AMY state (`zD`)

`zDZ` (empty filename) dumps the **full live synth state** — every active
instrument plus the global effects (reverb / chorus / echo / EQ) — as a series of
newline-separated **AMY wire-command lines**. Reassemble the frames as above; the
decoded text is the wire protocol you could replay with `amy.send(...)` to restore
that exact state.

### Run Python / restart the sketch (`zP`)

`zP<code>Z` runs one line on the board. Handy ones:

- `zPimport amyboard; amyboard.restart_sketch()Z` — reload & restart `sketch.py`.
- `zPimport amyboard; amyboard.environment_transfer_done()Z` — restart after a `zT`.
- `zPimport amyboard; amyboard.factory_reset()Z` — clear `current/` and reload default.
- `zPimport amyboard; amyboard.report_version()Z` — board replies with a `V` frame.
- `zPimport sequencer; sequencer.tempo(120)Z` — set tempo.

`zP` code is capped at 255 bytes — keep it to a short statement (imports + a call).

### Reset to a clean slate

Two options, lightest first:

- **Reset the synth only (no reboot):** `zPimport amy; amy.reset()Z`. Clears all
  oscillators/voices without dropping USB. Fast; the port stays open. Best for
  scripts that load many sketches back-to-back.
- **Full reboot:** `zBZ` (skip-sketch bootloader) then poll for the `AMYboard`
  MIDI port to disappear and re-enumerate, reconnect, optionally `zIZ` until you
  get `OK`. Guaranteed-clean slate, but the USB device resets so your MIDI
  handles must be reopened.

### See Python errors

When a sketch fails to load or run, the board pushes the **full traceback** back
as an `X` frame — base64 of the Python exception with file and line numbers. Watch
for `X` frames after any `zT`+restart or `zP` that runs sketch code, decode the
base64, and you have the error without needing the serial console. Example decoded
payload:

```
Traceback (most recent call last):
  File "amyboard.py", line 593, in run_sketch
  File "/user/current/sketch.py", line 4, in <module>
NameError: name 'foo' isn't defined
```

### Ping (`zI`)

`zIZ` → `F0 00 03 45 'O' 'K' F7`. Use it to confirm the board is connected and
responsive, e.g. while waiting out a reboot.

### Sequencer transport (`zY`)

`zY1Z` starts the step sequencer, `zY0Z` stops it — lets a host drive playback
without sending MIDI clock.

### Playing notes

To make the loaded synth sound, send **ordinary MIDI** to the `AMYboard` port (not
SysEx):

- Note on: `0x90 <note> <velocity>` (status `0x90`–`0x9F` = channels 1–16)
- Note off: `0x80 <note> 0` (or note-on with velocity 0)
- Also handled: control change (`0xB0`), program change (`0xC0`), pitch bend
  (`0xE0`), sustain pedal (CC 64), all-notes-off (CC 123).

Most AMYboard World sketches listen on **channel 1**.

---

## Minimal example (Python + mido)

```python
import base64, time, threading
import mido

MFR = [0x00, 0x03, 0x45]
ack = threading.Event()

def on_msg(m):
    if m.type == "sysex" and tuple(m.data[:3]) == (0x00, 0x03, 0x45):
        tag = m.data[3]
        if tag == 0x41 and m.data[4] == 0x4B:        # 'AK'
            ack.set()
        elif tag == 0x58:                             # 'X' error
            print("sketch error:\n" + base64.b64decode(bytes(m.data[4:])).decode())

inp  = mido.open_input("AMYboard", callback=on_msg)
outp = mido.open_output("AMYboard")

def send(payload: bytes, timeout=5.0):
    ack.clear()
    outp.send(mido.Message("sysex", data=MFR + list(payload)))
    ack.wait(timeout)

# upload a sketch
code = b"import amy\nprint('hello from amyboard')\n"
send(b"zPimport amy; amy.reset()Z")                  # clean slate
send(("zT/user/current/sketch.py,%dZ" % len(code)).encode())
for i in range(0, len(code), 188):
    send(base64.b64encode(code[i:i+188]))
send(b"zPimport amyboard; amyboard.environment_transfer_done()Z")  # restart

# play a note
outp.send(mido.Message("note_on",  note=60, velocity=100, channel=0))
time.sleep(0.5)
outp.send(mido.Message("note_off", note=60, channel=0))
```

For a fuller implementation — chunked transfers with per-chunk ACK, error
collection, reboot/re-enumeration handling, state dump reassembly, an ALSA
`amidi` backend that needs no Python MIDI libraries — use
[`tools/amyboardctl`](https://github.com/shorepine/tulipcc/tree/main/tools/amyboardctl)
instead of rolling your own.

---

## Where this is implemented (source of truth)

- **Transfer-layer command dispatch** (`zT`, `zD`, `zP`, `zY`, `zF`, `zS`,
  sample load): [`amy/src/parse.c`](https://github.com/shorepine/amy/blob/main/src/parse.c)
  — `amy_parse_transfer_layer_message()`.
- **Reboot / ping / SysEx intake** (`zB`, `zI`, the `00 03 45` gate):
  [`amy/src/amy_midi.c`](https://github.com/shorepine/amy/blob/main/src/amy_midi.c) — `parse_sysex()`.
- **File receive + base64 + state/file dump framing** (`C`/`E`/`0` markers):
  [`amy/src/transfer.c`](https://github.com/shorepine/amy/blob/main/src/transfer.c).
- **The `AK` ACK** (sent after each processed SysEx):
  [`tulip/shared/modtulip.c`](https://github.com/shorepine/tulipcc/blob/main/tulip/shared/modtulip.c) — `tulip_amy_send_sysex()`.
- **Sketch run / restart / error reporting (`X` frame) / version (`V` frame):**
  [`tulip/shared/amyboard-py/amyboard.py`](https://github.com/shorepine/tulipcc/blob/main/tulip/shared/amyboard-py/amyboard.py).
- **Reference host implementation (browser):**
  [`tulip/amyboardweb/static/spss.js`](https://github.com/shorepine/tulipcc/blob/main/tulip/amyboardweb/static/spss.js).
