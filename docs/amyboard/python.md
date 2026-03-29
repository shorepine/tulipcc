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

AMY has 256+ built-in patches: 0-127 are Juno-6 analog, 128-255 are DX7 FM, 256 is piano, and 257+ are PCM drum kits.
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

## The `amyboard` module

The `amyboard` module provides functions specific to the AMYboard hardware:

```python
import amyboard

# CV I/O
amyboard.cv_out(5.0, channel=0)    # Output 5V on CV out 1
volts = amyboard.cv_in(channel=0)   # Read CV in 1

# Rotary encoders.  The defaults are for the Adafruit single-encoder https://www.adafruit.com/product/5880
print(amyboard.read_encoder())
amyboard.init_buttons()
print(amyboard.read_buttons())

# OLED display (if connected)
amyboard.init_display()
amyboard.display.fill(0)
amyboard.display_refresh()

# I2C
i2c = amyboard.get_i2c()
devices = i2c.scan()
print(devices)

# SD card
amyboard.mount_sd()
```

See the [Modular Synth Setup](modular.md) page for detailed CV and encoder examples.

## The env.py startup script

AMYboard automatically runs `env.py` from your current environment directory on boot. Use it to set up your default configuration:

```python
# /user/current/env.py
import amy, amyboard

# Set up my preferred patches
amy.send(synth=1, patch=0, num_voices=6)      # Channel 1: Juno patch 0, 6-voice poly
amy.send(synth=10, num_voices=1, oscs_per_voice=1, synth_flags=3)    # Channel 10: Drums (synth_flags magic).

# Set CV out 1 to 0V on startup
amyboard.cv_out(0.0, channel=0)
```

You can edit `env.py` on the hardware REPL:

**TODO: make this real**
```python
edit('env.py')
```

Or create it from the web editor and send it to hardware with **Send to AMYboard**.

## File management

### On hardware

```python
from upysh import *

ls              # List files
cat('env.py')   # Print file contents
cd('/user')     # Change directory
pwd             # Print working directory
```

### Transfer files with mpremote

```bash
# Copy a file from your computer to AMYboard
mpremote resume fs cp my_script.py :my_script.py

# Copy a file from AMYboard to your computer
mpremote resume fs cp :env.py env.py

# Edit a file on AMYboard with your local editor
mpremote resume edit env.py
```

## Working with MIDI in Python

```python
import tulip

# Set up a MIDI callback
def my_midi_callback(message):
    # message is a bytes object
    status = message[0]
    if status & 0xF0 == 0x90:  # Note on
        note = message[1]
        vel = message[2]
        print(f"Note on: {note} velocity: {vel}")

tulip.midi_callback(my_midi_callback)
```

## Example: simple arpeggiator

```python
import amy, time

notes = [60, 64, 67, 72]  # C major arpeggio
amy.send(osc=0, patch=0)   # Juno patch 0

while True:
    for note in notes:
        amy.send(osc=0, note=note, vel=1)
        time.sleep(0.15)
        amy.send(osc=0, vel=0)
        time.sleep(0.05)
```

## Example: CV-controlled pitch

```python
import amy, amyboard, time

amy.send(osc=0, wave=amy.SINE, vel=1)  # Sine wave, on

while True:
    v = amyboard.cv_in(channel=0)
    # Map -10..+10V to 100..1000 Hz
    freq = 100 + ((v + 10) / 20.0) * 900
    amy.send(osc=0, freq=freq)
    time.sleep(0.01)
```

## More resources

 - [AMY wire protocol reference](https://github.com/shorepine/amy/blob/main/docs/api.md) -- every parameter you can control
 - [AMY synth architecture](https://github.com/shorepine/amy/blob/main/docs/synth.md) -- deep dive into oscillators, filters, modulation
 - [MicroPython documentation](https://docs.micropython.org/en/latest/) -- general MicroPython reference

[Back to Getting Started](README.md)
