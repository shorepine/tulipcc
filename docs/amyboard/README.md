# Getting Started with AMYboard

Welcome to **AMYboard** -- a powerful music synthesizer on a small modular-compatible board, for $29. AMYboard is built on the [AMY synthesizer](https://github.com/shorepine/amy) and runs [MicroPython](https://micropython.org) on an ESP32-S3.

<img src="https://amyboard.com/img/amyboard_preview.png" width=500>

AMYboard gives you:

 - **128 Juno-6 analog patches** and **128 DX7 FM patches**, plus piano, drums, and custom synthesis
 - **MIDI in and out** (USB and TRS)
 - **CV in and out** for modular synth integration (-10V to +10V)
 - **S/PDIF in and out** for digital audio
 - **SD card** for extra storage and samples
 - **I2C port** for [accessories](accessories.md) -- encoders, displays, and more
 - **Python programmability** -- write code to control every parameter of the synth
 - **Web editor** at [amyboard.com](https://amyboard.com) for patch design and environment management

If you have problems with AMYboard, you can find us on GitHub [issues](https://github.com/shorepine/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG). We'll do our best to help out!

-- DAn and Brian

## What's in the box

AMYboard ships from [Makerfabs](https://amyboard.com). In the box you'll get the board itself with the connectors already on, and an optional front panel. See [Modular Synth Setup](modular.md) for details on the front panel and Eurorack mounting.

## Board overview

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/amyboard_preview.png" width=500>

**Front panel connectors** (top to bottom, 10 jacks):

| Connector | Description |
|-----------|-------------|
| **S/PDIF in** | 3.5mm digital audio input |
| **S/PDIF out** | 3.5mm digital audio output |
| **Line in** | 3.5mm stereo analog audio input |
| **Line out** | 3.5mm stereo analog audio output |
| **MIDI in** | 3.5mm TRS MIDI input |
| **MIDI out** | 3.5mm TRS Type-A MIDI output |
| **CV1 in** | 3.5mm analog input, -10V to +10V (ADS1115 ADC) |
| **CV1 out** | 3.5mm analog output, -10V to +10V (GP8413 DAC) |
| **CV2 in** | 3.5mm analog input, -10V to +10V (ADS1115 ADC) |
| **CV2 out** | 3.5mm analog output, -10V to +10V (GP8413 DAC) |

**Other connectors:**

| Connector | Location | Description |
|-----------|----------|-------------|
| **USB-C** | Side | Power, serial REPL, USB MIDI, firmware updates |
| **I2C front panel** | Front | I2C / Grove / STEMMA QT / Qwiic port for [accessories](accessories.md) (encoders, displays) |
| **I2C host** | Back | For connecting to a [Tulip Creative Computer](https://github.com/shorepine/tulipcc) |
| **MicroSD card** | Side | Extra storage for samples and patches |
| **Modular power** | Back | 10-pin Eurorack power supply connector |
| **Debug header** | Back | For firmware development |

**What you'll need:**

 - A USB-C cable (for power and connection to your computer)
 - Headphones or powered speakers / a mixer (3.5mm audio cable)
 - Optionally: MIDI controller, modular synth cables, USB power adapter, SP/DIF connection, SD card, I2C accessories

## Quick start

1. **Plug in USB-C** from your computer to AMYboard. It powers the board and gives you a serial connection.
2. **Plug in headphones or speakers** to the audio out jack.
3. **Connect a MIDI controller** (USB MIDI or TRS MIDI) and play! AMYboard boots with Juno-6 patch #0 on MIDI channel 1.

No MIDI controller? No problem -- head to [amyboard.com](https://amyboard.com) and use the web editor to design sounds and send them to your board, or use the Python REPL to play notes directly.

## Upgrading firmware

You should upgrade AMYboard's firmware when you first receive it, and periodically after that. We constantly add features and fix bugs.

Connect AMYboard to your computer via USB-C. Install [`mpremote`](https://docs.micropython.org/en/latest/reference/mpremote.html) on your computer (`pip install mpremote`). Then check the [releases page](https://github.com/shorepine/tulipcc/releases) for the latest AMYboard firmware and follow the flashing instructions.



## MIDI

AMYboard supports MIDI in and out on 3.5mm TRS jacks (Type A by default).

### MIDI input

Just plug in a MIDI controller. AMYboard listens on all channels by default and plays the assigned patch. Channel 1 boots with Juno-6 patch #0.

### MIDI output

```python
import amyboard
# Initialize MIDI output (Type A is default)
amyboard.init_midi(type='A')

# For Type B connectors, use:
# amyboard.init_midi(type='B')
```

AMYboard supports both **Type A** and **Type B** TRS MIDI wiring. Most modern gear uses Type A. If your gear uses the older Type B standard, just change the init call.

### USB MIDI

AMYboard also appears as a USB MIDI device when connected to your computer. You can send MIDI from a DAW or other software directly over USB -- no adapter needed.


## I2C expansion

The I2C bus (SCL=18, SDA=17, 400kHz) is available for connecting additional hardware:

```python
import amyboard
i2c = amyboard.get_i2c()

# Scan for connected devices
print(i2c.scan())

# Read/write registers on any I2C device
val = amyboard.read_register(addr, reg)
amyboard.write_register(addr, reg, val)
```

You can add more DACs, ADCs, displays, or sensors to expand AMYboard's capabilities.

## I2C follower mode

AMYboard also listens as an I2C follower device at address **`0x3F`**. This means an external microcontroller can send AMY wire protocol commands directly to AMYboard over I2C, making it act as a synth module controlled by your own hardware. This is especially useful for Tulip.

```c
// From an external Arduino/microcontroller:
Wire.beginTransmission(0x3F);
Wire.write("v0w1f440l1");  // Play A4 on voice 0
Wire.endTransmission();
```

## SD card

Mount a MicroSD card for additional patch storage or sample playback:

```python
import amyboard
amyboard.mount_sd()
# Card is now available at /sd
```

[Back to Getting Started](index.md)


## Guides

 - **[Using AMYboard Online](online.md)** -- The web editor at amyboard.com for patch design and management
 - **[Arduino Setup](arduino.md)** -- Using the AMY synth engine in Arduino projects
 - **[Modular Synth Setup](modular.md)** -- CV, gate, and MIDI integration with your modular rig
 - **[Using Python](python.md)** -- Programming AMYboard with MicroPython
 - **[Accessories](accessories.md)** -- Compatible displays, encoders, and other I2C accessories
 - **[Troubleshooting](troubleshooting.md)** -- Common issues and how to fix them

## Learn more about AMY

AMYboard is powered by AMY, an open-source synthesizer engine. To learn more about synthesis, patches, and the wire protocol:

 - [AMY synth documentation](https://github.com/shorepine/amy/blob/main/docs/synth.md)
 - [AMY API reference](https://github.com/shorepine/amy/blob/main/docs/api.md)
 - [AMY MIDI documentation](https://github.com/shorepine/amy/blob/main/docs/midi.md)

## Community

[![shore pine sound systems discord](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/shorepine100.png) **Chat about AMYboard on our Discord!**](https://discord.gg/TzBFkUb8pG)
