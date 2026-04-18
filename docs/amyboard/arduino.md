# AMYboard Arduino Setup

AMYboard runs MicroPython firmware by default, but the underlying **AMY synthesizer engine** is available as a standalone Arduino library. This means you can use AMY's synthesis on AMYboard's ESP32-S3 hardware (or [many other boards](https://github.com/shorepine/amy/blob/main/docs/arduino.md)) from Arduino sketches.

### 1. Install the AMY library and AMYboard board support

AMY library and AMYboard board support are both available in the Arduino Library Manager. Follow these steps in order:

<img src="https://github.com/shorepine/tulipcc/raw/main/docs/amyboard/img/arduino-board.jpg" width="400"/>

 1. **Update or install ESP32 board support**: If you already have the `esp32 by Espressif` board package installed, make sure it's at version 3.3.8 or higher. If you don't yet have it, in the Arduino IDE, go to **File > Preferences** and add the ESP32 board manager URL: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`. Then go to **Tools > Board > Boards Manager**, search for **"esp32"** by Espressif, and install it.
 2. **Select the AMYboard board**: Go to **Tools > Board > ESP32 Arduino** and select **AMYboard** (It's all the way at the bottom.)
 3. **Install the AMY library**: Go to **Sketch > Include Library > Manage Libraries**, search for **"AMY"**, and install it.

<img src="https://github.com/shorepine/amy/raw/main/docs/arduino1.png" width="400"/>

 4. Open the example sketch: **File > Examples > AMY > AMY_MIDI_Synth**
 5. Make sure AMYboard is selected as your board, then click **Upload**.

The example receives MIDI input and plays a Juno-6 patch. It's a good starting point for any project.

### 2. Flash AMYboard hardware with Arduino sketch

It should flash directly over USB-C without any special steps. If you see an upload error, put AMYboard into DFU (bootloader) mode:

 1. Hold down both **BOOT** and **RST** buttons on the back of the board.
 2. Release **RST** first, then release **BOOT**. It'll feel like "rolling" your finger over both buttons from bottom to top.
 3. Try uploading again.

You can always go back to our stock [AMYboard MicroPython firmware](firmware.md).

## Minimal Arduino sketch

```c
#include <AMY-Arduino.h>

void setup() {
  amy_config_t config = amy_default_config();
  amy_config.features.default_synths = 1;
  amy_start(config);
}

void loop() {
  amy_update();
}
```

This minimal sketch will respond to MIDI input over USB -- look for "AMYboard" as a MIDI input device on your computer or in your DAW. Send it MIDI notes and you'll hear the default Juno-6 patch.

Check out the other examples in **File > Examples > AMY** for more ideas, including sequencer demos and multi-voice setups.

## Playing notes from Arduino

Loading a preset synth patch:
```c
amy_event e = amy_default_event();
e.synth = 1;         // The synth to be set up.
e.patch_number = 6;  // Juno A17 Choir preset patch.
e.num_voices = 4;    // How many simultaneous voices to provision.
amy_add_event(&e);
```
Playing a note on a configured synth:
```c
amy_event e = amy_default_event();
e.synth = 1;         // Which synth to use
e.midi_note = 60;    // Middle C
e.velocity = 1;      // Full strike
amy_add_event(&e);
```

## More information

 - [AMY Arduino getting started guide](https://github.com/shorepine/amy/blob/main/docs/arduino.md) -- detailed setup for all supported boards
 - [AMY API reference](https://github.com/shorepine/amy/blob/main/docs/api.md) -- full C API documentation
 - [Supported boards and features matrix](https://github.com/shorepine/amy/issues/354)

[Back to Getting Started](README.md)
