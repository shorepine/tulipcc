# Arduino Setup for AMY

AMY is on the Arduino Libraries repository. Simply search for "AMY" in the Library Manager and you should find us!

You can run AMY on many types of Arduino boards. The simplest way is to get our new [AMYboard](https://amyboard.com) which is only US$29 and has a lot of great built-in features. But AMY also runs great on 
 - Any esp32, esp32-s3, esp32-p4 or esp32-c6
 - Teensy 4.x
 - Pi Pico RP2350 or RP2040

<img src="https://github.com/shorepine/amy/raw/main/docs/arduino1.png" width="400"/>

We recommend always using the latest released version in the Arduino Library Manager. 

However, if you are directed to a bleeding edge release of AMY, you can simply copy this repository to your `Arduino/libraries` folder as `Arduino/libraries/amy`. (Make sure you delete whatever you already had in `libraries/amy`.)

To use AMY in Arduino, just `#include <AMY-Arduino.h>`. You need at a minimum the following code in your sketch, a call to `amy_start` with some options set in an `amy_config_t`, and then a call to `amy_update()` somewhere in your `loop()`. 

```c
#include <AMY-Arduino.h>

void setup() {
  // .. in your setup ...
  amy_config_t amy_config = amy_default_config();
  // set your pins, etc -- see the AMY_MIDI_Synth example
  amy_start(amy_config);
}

void loop() {
  // Your loop() must contain this call to amy:
  amy_update(); 
}
```

AMY in Arduino handles MIDI input and output as well as I2S and USB (where supported) audio. You do not need to set up an I2S interface, just the pins in `amy_config`.

(For experts: if you're rendering AMY audio to your own audio hardware, you can set `amy_config.audio = AMY_AUDIO_IS_NONE` before calling `amy_start()`, then use the return value from `amy_update()` as a pointer to the next block of `AMY_BLOCK_SIZE` 16 bit stereo sample frames, to send out to whatever interface you set up.)

[Please see our latest matrix of supported features per chip/board.](https://github.com/shorepine/amy/issues/354)

We ship a single example in the Examples folder for AMY called `AMY_MIDI_Synth`. It receives MIDI input and plays a Juno-6 patch 0 in response. should work on all our supported boards. Make sure you've installed the board support package for the board / chip you are using:

 * RP2040 / RP2350 / Pi Pico: [`arduino-pico`](https://arduino-pico.readthedocs.io/en/latest/install.html#installing-via-arduino-boards-manager)
 * Teensy: [`teensyduino`](https://www.pjrc.com/teensy/td_download.html)
 * ESP32 / ESP32-S3 / etc: [`arduino-esp32`](https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/installing.html)

You can use both cores of supported chips (RP2040, RP2350 or ESP32 variants) for more oscillators and voices.

Once you've confirmed AMY is running on your chip, read our [tutorial](tutorial.html) or our [getting started](../README.md) page on how to control AMY in Arduino:

```c
// Will play MIDI note 50 on patch 130
amy_event e = amy_default_event();
e.osc = 0;
e.patch_number = 130;
e.velocity = 1;
e.midi_note = 50;
e.voices[0] = 0;
amy_add_event(&e);
```

## Recommended DAC / ADC configurations

The supported microcontrollers (excepting the Daisy, which is built in) use I2S as the default audio interface. I2S is a standard but some chips / boards may require setup beyond just wiring up the pins. For example, the [`WM8960`](https://www.waveshare.com/wiki/WM8960_Audio_HAT) and the `PCM9211` require registers to be set to route audio signals and amplification. 

We can recommend simpler audio out DACs that "just work" with AMY with no extra setup:

 * [PCM5101 / PCM5102](https://www.amazon.com/dp/B0DCGF2TN1?th=1)
 * [UDA1334](https://www.adafruit.com/product/3678)


### Audio input DACs

Here's an example of a PCM1808 and PCM5101 working together to provide audio in and out for AMY on an ESP32S3:

<img src="AMY-Arduino-ESP32S3_bb.png" width="600"/>


## Per-board notes

### AMYboard

Our built-for-AMY board [AMYboard](https://amyboard.com) should just work out of the box. You can find AMYboard in the Boards list of the espressif arduino-esp32 package. See the specific [AMYboard Arduino details](https://github.com/shorepine/tulipcc/blob/main/docs/amyboard/arduino.md) for more information. 

### ESP32, ESP32-S3, ESP32-P4

Tested: Arduino IDE 2.3.6 (mac) + arduino-esp32 3.2.0


### Teensy 4.0, 4.1

Tested: Arduino IDE 2.3.6 (mac) + [Teensy 1.59.0](https://www.pjrc.com/teensy/teensyduino.html)

USB audio and i2s audio output happen at the same time

if you set USB Type to Audio, it will appear as a USB audio device on your computer, called "AMY Synthesizer"

Note: USB audio from Teensy is fiddly and often slow to enumerate, i have to wait a minute or two before it shows up on my Mac. This seems to be unrelated to AMY. Please ask on the Teensy forums if you're having trouble with USB Audio. Once it is enumerated and running, it does seem stable.

For I2S, you have to use the default i2s1 pins. The pins you set in AMY config are ignored. That's `21 -> BLCK`, `20 -> LRC`, `DOUT -> 7`. This works with the [Teensy Audio Shield](https://www.pjrc.com/store/teensy3_audio.html) and the simpler [PT8211](https://www.pjrc.com/store/pt8211_kit.html). It will also work with other I2S DAC boards. 

For UART MIDI, we use Serial8, the pins you set in AMY config are ignored. That's `MIDI_OUT -> 35`, `MIDI_IN -> 34`.



### Pi Pico RP2040 and Pi Pico 2 RP2350

Tested: Arduino IDE 2.3.6 (mac)

The `i2s_lrc` pin has to be `i2s_bclk` + 1.


### Electro-Smith Daisy Seed

The Daisy works great with AMY.  However, it does not work with the Arduino [DaisyDuino](https://daisy.audio/tutorials/arduino-dev-env/#welcome-to-daisyduino). The size of AMY plus the size of the DaisyDuino support exceeds the segment size that DaisyDuino currently supports. We hope they fix this! [See this thread for more information.](https://forum.electro-smith.com/t/arduino-how-to-use-512k-ram-area-for-program-space/4839). 

To use your Daisy with AMY, [you first have to set up the C/C++ toolchain.](https://daisy.audio/tutorials/cpp-dev-env/#follow-along-with-the-video-guide)

It supports audio in, serial MIDI in, AMY sequencer.

To use, get our [amy_daisy.cpp](https://github.com/shorepine/amy/raw/main/daisy/amy_daisy.cpp) and [Makefile](https://github.com/shorepine/amy/raw/main/daisy/Makefile), set your location of AMY in the `Makefile`, then set up your Daisy dev environment, then put the board into DFU mode, and run

```
make clean && make && make program-boot; sleep 1.5; make program-dfu
```

After it flashes, you should hear the AMY startup chime from the Daisy's line out.  If you have a MIDI serial interface hooked up to the Daisy (D14 USART1Rx) you should be able to play the default Juno synth on channel 1.

