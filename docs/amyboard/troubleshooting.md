# Troubleshooting

Here are solutions to common issues with AMYboard. If you're stuck, reach out to the community -- we're happy to help.

## Get help

 - [Discord](https://discord.gg/TzBFkUb8pG) -- quickest way to get help from the community
 - [GitHub Issues](https://github.com/shorepine/tulipcc/issues) -- bug reports and feature requests
 - [GitHub Discussions](https://github.com/shorepine/tulipcc/discussions) -- general questions
 - [Makerfabs contact](https://makerfabs.com/contact.html) -- hardware defect / shipping issues

**Please remember**: AMYboard is supported by volunteers. We'll do our best to help, but please be patient and try the steps below first.

## Always upgrade first

Many issues are fixed in newer firmware. Before troubleshooting, update to the latest release:

1. Check the [releases page](https://github.com/shorepine/tulipcc/releases) for the latest AMYboard firmware
2. Flash it to your board using `esptool` or `mpremote`

## No sound

**Check the basics:**
 - Is the USB-C cable connected? AMYboard needs power.
 - Are headphones or speakers plugged into the **audio out** jack (not audio in)?
 - Try turning the volume up on your headphones/speakers.

**Check the synth:**
 - Connect via `mpremote resume` and try:
   ```python
   import amy
   amy.send(osc=0, wave=amy.SINE, freq=440, vel=1)
   ```
   You should hear a 440Hz sine wave. If you do, the audio hardware is working.

**Check MIDI:**
 - AMYboard boots with Juno-6 patch #0 on MIDI channel 1. Make sure your controller is sending on channel 1.
 - Try a different MIDI cable or controller.
 - Check USB MIDI vs TRS MIDI -- are you using the right connection?

## No serial connection / mpremote can't find the board

 - Try a different USB-C cable. Some cables are charge-only and don't carry data.
 - **Install the CH340K driver**: Some operating systems need a USB-to-serial driver. [Download CH340K drivers here](https://www.wch-ic.com/downloads/CH341SER_ZIP.html).
 - On macOS, check **System Information > USB** to see if the board appears.
 - On Linux, check `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`.
 - On Windows, check Device Manager for COM ports.
 - Make sure no other program (serial monitor, DAW) is using the port.

## USB MIDI not recognized

 - AMYboard appears as a USB MIDI device with VID `0xCAF0` and PID `0x4009`.
 - On macOS, open **Audio MIDI Setup** and check if AMYboard appears.
 - On Windows, check Device Manager under "Sound, video and game controllers".
 - Try a different USB port or cable.
 - Some USB hubs can cause issues -- try connecting directly.

## AMYboard Online issues

### No sound in the browser
 - Click anywhere on the page to unlock the browser's AudioContext. Browsers block audio until user interaction.
 - Check that your browser's audio output is set to the correct device.
 - Try Chrome -- it has the best Web Audio support.

### Web MIDI not working
 - Web MIDI requires Chrome or Edge. Firefox and Safari do not support Web MIDI.
 - Make sure your MIDI device is connected before opening amyboard.com.
 - Click the MIDI device selector and choose your device.
 - On some systems, you may need to grant MIDI access permission.

### "Send to AMYboard" fails
 - Make sure AMYboard is connected via USB and no other program is using the serial port.
 - Close any `mpremote` sessions before transferring.
 - Try refreshing the page and reconnecting.

## MIDI issues

### Wrong MIDI type (Type A vs Type B)
 - AMYboard defaults to Type A MIDI. If your gear uses Type B:
   ```python
   import amyboard
   amyboard.init_midi(type='B')
   ```
 - Add this to your `env.py` to make it permanent.

## Board won't boot / crashes on startup

 - Try a "factory reset" by erasing the flash and reflashing:
   ```bash
   esptool.py --chip esp32s3 erase_flash
   ```
   Then flash the latest firmware.
 - If your `env.py` or `boot.py` has a bug, it can prevent the board from booting properly. You can delete it via `mpremote`:
   ```bash
   mpremote resume fs rm :user/current/env.py
   ```

## SD card not mounting

 - Make sure the SD card is formatted as FAT32.
 - Try a different SD card -- some cards have compatibility issues.
 - Mount manually:
   ```python
   import amyboard
   amyboard.mount_sd()
   ```
   Check for error messages.

## I2C devices not found

 - Scan the bus to see what's connected:
   ```python
   import amyboard
   i2c = amyboard.get_i2c()
   print(i2c.scan())
   ```
 - Expected addresses: `0x40` (PCM9211), `0x48` (ADS1115 ADC), `0x58` (GP8413 DAC), `0x49` (Seesaw encoders).
 - Check your wiring -- SDA and SCL may be swapped.
 - Make sure pull-up resistors are present on your I2C lines (AMYboard has them built in for the onboard devices).

[Back to Getting Started](index.md)
