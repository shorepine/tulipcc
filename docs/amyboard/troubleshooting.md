# Troubleshooting

Here are solutions to common issues with AMYboard. If you're stuck, reach out to the community -- we're happy to help.

## Get help

 - [Discord](https://discord.gg/TzBFkUb8pG) -- quickest way to get help from the community
 - [GitHub Issues](https://github.com/shorepine/tulipcc/issues) -- bug reports and feature requests
 - [Makerfabs contact](https://makerfabs.com/contact.html) -- hardware defect / shipping issues

**Please remember**: AMYboard is supported by volunteers. We'll do our best to help, but please be patient and try the steps below first.

## Always upgrade first

Many issues are fixed in newer firmware. Before troubleshooting, [update to the latest release](firmware.md).

## No sound

**Check the basics:**
 - Is the USB-C cable, modular power or I2c power connected? AMYboard needs power.
 - Are cables plugged into the **audio out** jack (not audio in)?

**Reset the sketch**
 - Connect to your AMYboard in [AMYboard Online](online.md) and use the "Reset" tab. This should revert your AMYboard to a default patch.

**Check the synth:**
 - Connect via [serial connection](python.md) and try:
   ```python
   import amy
   amy.send(osc=110, wave=amy.SINE, freq=440, vel=1)
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

 - Did you hit RST? Try that first.
 - AMYboard appears as a USB MIDI device with VID `0xCAF0` and PID `0x4009`.
 - On macOS, open **Audio MIDI Setup** and check if AMYboard appears.
 - On Windows, check Device Manager under "Sound, video and game controllers".
 - Try a different USB port or cable.
 - Some USB hubs can cause issues -- try connecting directly.

## AMYboard Online issues

### Web MIDI not working
 - Web MIDI requires Chrome or Edge. Firefox and Safari do not support Web MIDI.
 - Make sure your MIDI device is connected before opening amyboard.com.
 - Click the MIDI device selector and choose your device.
 - On some systems, you may need to grant MIDI access permission.

### AMYboard Online can't find the board

 - Make sure you are not connected to AYMboard over a serial connection when using AMYboard online.
 - Look at the browsers' JS console to see helpful diagnostic messages.
 - [Re-flash your firmware](firmware.md) if you're having issues.

## MIDI issues

### Wrong MIDI type (Type A vs Type B)
 - AMYboard defaults to Type A MIDI. If your gear uses Type B:
   ```python
   import amyboard
   amyboard.init_midi(type='B')
   ```
 - Add this to your `sketch.py` to make it permanent.

## Board won't boot / crashes on startup

 - **Safe mode**: Hold the BOOT button while AMYboard is powering up. This skips running your `sketch.py` and also runs a hardware self-test (audio input, CV in/out). You'll hear a chime if all tests pass. Once it finishes you'll have a normal REPL where you can fix or delete your sketch.
 - If your `sketch.py` or `boot.py` has a bug, hold BOOT to skip it, then delete it via the REPL or `mpremote`:
   ```bash
   mpremote resume fs rm :user/current/sketch.py
   ```
 - Try a full "factory reset" by erasing the flash and reflashing. See our [firmware upgrade page](firmware.md).

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

[Back to Getting Started](README.md)
