# Using Python

AMYboard runs MicroPython and gives you a full Python REPL for controlling the synthesizer, reading inputs, and building instruments and effects in real time.

## Connecting to the REPL

### From your computer

Connect AMYboard via USB-C and use a serial terminal or `mpremote`:

```bash
pip install mpremote
mpremote resume
```

You'll see the MicroPython `>>>` prompt. You can type Python commands directly.

### From the web

Open [amyboard.com](https://amyboard.com) -- it includes a Python REPL running in the browser. You can test code online before deploying it to hardware.

### Over WiFi (WebREPL)

AMYboard can also expose its REPL over your WiFi network with MicroPython's **WebREPL**, so you can connect wirelessly from a browser -- no USB cable. Join a network, then start the WebREPL server with a password (up to 9 characters):

```python
import amyboard, webrepl

amyboard.wifi('your_ssid', 'your_password')  # join WiFi (returns the board's IP)
webrepl.start(password='amyboard')           # start the server on port 8266
```

`webrepl.start()` prints the address it's listening on, for example:

```
WebREPL server started on http://192.168.1.42:8266/
```

To connect, open the WebREPL client at [micropython.org/webrepl](https://micropython.org/webrepl/), enter `ws://192.168.1.42:8266/` (use your board's IP), click **Connect**, and type the password. You now have a full `>>>` prompt over the network. The same client can also transfer files to and from the board.

To start WebREPL automatically on every boot, add the three lines above (including `amyboard.wifi(...)`) to your `sketch.py` -- see [The sketch.py startup script](#the-sketchpy-startup-script) below.

> **Security:** WebREPL gives anyone on the network with the password full control of your AMYboard. Use it only on trusted networks, and pick a non-trivial password.

## Playing sounds with AMY

The `amy` module gives you direct access to the synthesizer engine:

```python
import amy

# Clear any existing setup
amy.reset()

# Play a 440Hz sine wave
amy.send(osc=0, wave=amy.SINE, freq=440, vel=1)

# Stop the note
amy.send(osc=0, vel=0)
```

### Using patches

AMY has hundreds of built-in patches: 0-127 are Juno-6 analog, 128-255 are DX7 FM, 256 is piano, and 384-390 are the General MIDI drum kits — 384 TR-808, 385 TR-909, 386 Linn 9000, 387 Univox MR-12, 388 Tokyo Synthetics, 389 80s Power Kit, 390 Percussion. Load a kit on a synth with `synth_flags=3` and play GM note numbers (36=kick, 38=snare, 42=hat...) to trigger drums; switch kits with `amy.send(synth=10, patch=38x)` or over MIDI with bank select MSB 3 (CC0=3) + program change 0-6.
In general, a patch will use multiple oscs.  `synth` is our abstraction to manage multiple oscs at the same time.

```python
# Load Juno-6 patch #10 on synth 1
amy.send(synth=1, patch=10, num_voices=4)

# Play middle C
amy.send(synth=1, note=60, vel=1)

# Send the note-off
amy.send(synth=1, note=60, vel=0)

# Switch to DX7 patch #5
amy.send(synth=1, patch=133)  # 128 + 5
```

### Polyphony with synths

The `synth` objects provide the specified level of polyphony.  They also connect directly to MIDI channels:

```python
# Set up a 4-voice polyphonic Juno patch on synth 1 (MIDI channel 1).
amy.send(synth=1, patch=0, num_voices=4)

# Play notes on individual voices
amy.send(synth=1, note=60, vel=1)  # C
amy.send(synth=1, note=64, vel=1)  # E
amy.send(synth=1, note=67, vel=1)  # G

# Send vel=0 with no note number for "all notes off"
amy.send(synth=1, vel=0)

# You can also play polyphonically with a MIDI keyboard (connected via the "MIDI in" jack) or your computer (writing to the AMYboard USB MIDI gadget) sending notes on channel 1.  
```

### Writing your own DSP in C

AMYboard can compile C code **on the board itself** and run it inside the
synthesizer — custom effects at the end of a bus' FX chain, or whole new
oscillator types, hot-swapped from Python while audio keeps playing:

```python
import tulip
tulip.install_c_process('crush', """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 18) << 18;   // bitcrush!
        i = i + 1;
    }
""")
tulip.c_process('crush', True)
```

See [Writing audio effects and oscillators in C](../user_c_dsp.md) for the
full guide — sample format, oscillator API, and worked examples (bitcrusher,
heavy distortion, a CZ-101 phase-distortion oscillator, bytebeat).

## The `amyboard` module

The `amyboard` module provides functions specific to the AMYboard hardware:

```python
import amyboard

# CV I/O
amyboard.cv_out(5.0, channel=0)    # Output 5V on CV out 1
volts = amyboard.cv_in(channel=0)   # Read CV in 1

# Rotary encoders.  amyboard.encoder() autodetects whichever accessory is
# connected (Adafruit single/quad or M5Stack 8Encoder) and gives one API for all.
enc = amyboard.encoder()
print(enc.type, enc.encoders)       # e.g. "m5stack" 8
print(enc.read(0))                  # position of encoder 0 (starts at 0)
print(enc.button(0))                # True while held
if enc.leds:
    enc.led(0, 0, 64, 0)            # light encoder 0's LED dim green
# See accessories.md for the legacy per-device read_encoder()/read_buttons() helpers.

# OLED display (if connected)
amyboard.init_display()
amyboard.display.fill(0)
amyboard.display_refresh()

# Rotate the OLED (sh1107 only).  One of 0, 90, 180, 270 degrees.
# The setting is saved and re-applied automatically on every boot.
amyboard.set_display_rotation(90)
print(amyboard.display_rotation())   # -> 90

# I2C
i2c = amyboard.get_i2c()
devices = i2c.scan()
print(devices)

# SD card
amyboard.mount_sd()
```

See the [Modular Synth Setup](modular.md) page for detailed CV and encoder examples.

## The sketch.py startup script

AMYboard automatically runs `sketch.py` from your current environment directory on boot. The top-level code runs once at startup, and if you define a `loop()` function it will be called repeatedly (~60ms interval). Use it to set up your default configuration:

```python
# /user/current/sketch.py
import amy, amyboard

# Set up my preferred patches
amy.send(synth=1, patch=0, num_voices=6)      # Channel 1: Juno patch 0, 6-voice poly
amy.send(synth=10, num_voices=1, patch=384, synth_flags=3)    # Channel 10: TR-808 GM drum kit (note 36=kick, ...; drum kits are single-voice, always num_voices=1). Kits 385-390: 909/Linn/MR-12/Synthetics/Power/Percussion.

# Set CV out 1 to 0V on startup
amyboard.cv_out(0.0, channel=0)

def loop():
    pass
```

You can edit `sketch.py` directly on the hardware using the built-in text editor. Connect to your AMYboard with `screen` or `mpremote` (not `idf.py monitor`, which doesn't support full terminal emulation):

```bash
screen /dev/YOUR_SERIAL_PORT 115200
# or
mpremote connect /dev/YOUR_SERIAL_PORT
```

Then at the REPL:

```python
edit('current/sketch.py')
```

This opens a full-screen text editor. On macOS, use **Esc then the key** instead of Ctrl (macOS terminals intercept many Ctrl sequences):

 - **Esc, S** — Save
 - **Esc, Q** — Quit
 - **Esc, X** — Cut line
 - **Esc, V** — Paste
 - **Esc, Z** — Undo
 - **Esc, F** — Find

On Linux or Windows terminals, Ctrl-S, Ctrl-Q, etc. work as usual. See the [pye documentation](https://github.com/robert-hh/Micropython-Editor) for the full list of key bindings.

You can also create or edit files from [AMYboard Online](https://amyboard.com/editor) and send them to hardware with **Send to AMYboard**.

## File management

### On hardware

```python
from upysh import *

ls                  # List files
cat('sketch.py')    # Print file contents
cd('/user')         # Change directory
pwd                 # Print working directory
```

### Transfer files with mpremote

```bash
# Copy a file from your computer to AMYboard
mpremote resume fs cp my_script.py :my_script.py

# Copy a file from AMYboard to your computer
mpremote resume fs cp :sketch.py sketch.py

# Edit a file on AMYboard with your local editor
mpremote resume edit sketch.py
```

### Transfer files using the microSD card

First, copy the files you want to the card on your computer. Then insert it into the AMYboard and reboot (or try `amyboard.mount_sd()`). Then you can access those files directly from Python or copy them to user storage: `cp('/sd/file.wav', '/user/file.wav')`.


## Working with MIDI in Python

To receive MIDI, register a callback with `midi.add_callback()`. Your function is called once per incoming message, with the message as a `bytes` object:

```python
import midi

def my_midi_callback(message):
    status = message[0]
    if status & 0xF0 == 0x90:  # Note on
        note = message[1]
        vel = message[2]
        print(f"Note on: {note} velocity: {vel}")

midi.add_callback(my_midi_callback)
# ... later, to stop receiving:
midi.remove_callback(my_midi_callback)
```

You can register as many callbacks as you like; they all receive every message. This is how AMYboard's own features (like `amyboard.show_midi_ccs()`) listen for MIDI, and the factory self-test uses the same pattern for its MIDI loopback check.

Note that AMYboard's system MIDI dispatcher drains the incoming MIDI queue for you at boot, so polling `tulip.midi_in()` yourself will always return `None` — the dispatcher has already consumed each message and handed it to the registered callbacks. Likewise, don't call `tulip.midi_callback()` directly: that would replace the system dispatcher and silently break everything else listening for MIDI. Always use `midi.add_callback()` / `midi.remove_callback()`.

For sysex messages, set `midi.sysex_callback` to a function; it receives the full sysex payload as `bytes`:

```python
import midi

def my_sysex_callback(payload):
    print("sysex:", payload.hex())

midi.sysex_callback = my_sysex_callback
```

### Sending MIDI out

There are two ways to send MIDI out of AMYboard.

For one-off or arbitrary messages, use `tulip.midi_out()` with the raw MIDI bytes (a list, tuple or `bytes`). It sends immediately, out the MIDI out jack and the USB MIDI connection:

```python
import tulip

tulip.midi_out([0x90, 60, 127])  # note on,  channel 1, note 60, velocity 127
tulip.midi_out([0x80, 60, 0])    # note off, channel 1, note 60
tulip.midi_out([0xB0, 74, 64])   # control change, channel 1, CC 74 = 64
```

For musically-timed output, let AMY send the MIDI for you. Set an oscillator's `wave` to `amy.AMY_MIDI`: instead of making a sound, that osc emits a MIDI note on/off whenever it receives a note. Because it's an ordinary osc, you can drive it from AMY's sequencer to get sample-accurate MIDI output that stays locked to AMY's tempo — for example, a pattern that plays an external synth:

```python
import amy

amy.send(osc=0, wave=amy.AMY_MIDI)                    # osc 0 now sends MIDI instead of audio

# Play a MIDI note on channel 1 every quarter note (48 ticks), held for an eighth note.
amy.send(osc=0, note=60, vel=1, sequence="0,48,1")    # note on  at tick 0  of each period
amy.send(osc=0, note=60, vel=0, sequence="24,48,2")   # note off at tick 24 of each period
```

`amy.AMY_MIDI` always sends on MIDI channel 1, and notes that arrived over MIDI in are not echoed back out. See the [AMY MIDI docs](https://github.com/shorepine/amy/blob/main/docs/midi.md#sending-midi-out) for the full details.

#### MIDI out TRS type (A vs B)

The MIDI out jack is a 3.5mm TRS connector, and there are two incompatible wiring standards for these: **Type A** (the official MIDI-spec standard, used by most modern gear) and **Type B** (older, used by some early Arturia/Novation/Akai devices). They swap the TRS tip and ring, so if your external device isn't receiving MIDI, it likely expects the other type. AMYboard defaults to **Type A**; only MIDI out is affected — MIDI in works with either type.

Switch it at runtime with `amyboard.set_midi_type()` (no reboot needed; AMY keeps running):

```python
import amyboard

amyboard.set_midi_type('B')   # switch MIDI out to Type B
amyboard.set_midi_type('A')   # back to Type A (the default)
amyboard.midi_type()          # -> 'A' or 'B', whichever is currently active
```

This is not persisted across reboots — AMYboard always comes up as Type A.

### Syncing to MIDI clock

AMYboard's sequencer runs on its own internal clock by default. You can instead **follow** an external MIDI clock — slaving AMYboard to a DAW or drum machine — or **send** MIDI clock out to drive other gear from AMYboard.

**Following an external clock.** External realtime sync is off by default: incoming clock is ignored so AMYboard keeps its own tempo. Turn it on with `tulip.external_midi_sync()`:

```python
import tulip

tulip.external_midi_sync(True)   # follow incoming MIDI clock
tulip.external_midi_sync(False)  # back to AMYboard's internal clock (the default)
```

While following is on, realtime messages arriving on MIDI in (DIN jack or USB) drive the sequencer: `F8` (Timing Clock) sets the tempo and advances the sequencer in lock-step (24 clocks per quarter note), `FA` (Start) starts it, and `FC` (Stop) stops it. Start/Stop only take effect while sync is enabled, so a connected DAW can't start or stop your AMYboard pattern unless you've opted in. Turn sync back off to return to the internal clock.

**Sending clock.** There's no separate "send sync" switch — emit the realtime bytes yourself with `tulip.midi_out()` (out the MIDI out jack and USB, as above):

```python
tulip.midi_out([0xFA])   # Start
tulip.midi_out([0xF8])   # one Timing Clock tick — send 24 per quarter note
tulip.midi_out([0xFC])   # Stop
```

To send a steady clock locked to AMYboard's own tempo, drive `F8` from a sequencer callback. The sequencer runs at 48 ticks per quarter note (as above) and MIDI clock is 24, so send on every other tick (`period=2`):

```python
import tulip, sequencer

sequencer.tempo(120)             # AMYboard's sequencer tempo, in BPM

def send_clock(tick):
    tulip.midi_out([0xF8])

tulip.midi_out([0xFA])                           # tell downstream gear to start
slot = tulip.seq_add_callback(send_clock, 0, 2)  # fire every 2nd tick -> 24 / quarter

# ...later, to stop:
# tulip.midi_out([0xFC])
# tulip.seq_remove_callback(slot)
```

`tulip.seq_add_callback(fn, tick, period)` calls `fn(tick_count)` whenever `tick_count % period == tick`, and returns a slot id you pass to `tulip.seq_remove_callback()` to stop.

## Example: simple arpeggiator

```python
import amy, time

notes = [60, 64, 67, 72]  # C major arpeggio
amy.send(synth=1, patch=0, num_voices=4)   # Juno patch 0

for _ in range(10):
    for note in notes:
        amy.send(synth=1, note=note, vel=1)
        time.sleep(0.15)
        amy.send(synth=1, note=note, vel=0)
        time.sleep(0.05)
```
**Note:** Keyboard interrupt (Ctrl-C) doesn't work while the loop is running, so beware of getting stuck.  Pressing the "Reset" button will start you over.

## Example: CV-controlled pitch

```python
import amy, amyboard, time

amy.reset()
amy.send(osc=0, wave=amy.SINE, vel=1)  # Sine wave, on

for _ in range(100):
    v = amyboard.cv_in(channel=0)
    # Map -10..+10V to 100..1000 Hz
    freq = 100 + ((v + 10) / 20.0) * 900
    amy.send(osc=0, freq=freq)
    time.sleep(0.05)
```

## More resources

 - [AMY wire protocol reference](https://github.com/shorepine/amy/blob/main/docs/api.md) -- every parameter you can control
 - [AMY synth architecture](https://github.com/shorepine/amy/blob/main/docs/synth.md) -- deep dive into oscillators, filters, modulation
 - [MicroPython documentation](https://docs.micropython.org/en/latest/) -- general MicroPython reference

[Back to Getting Started](README.md)
