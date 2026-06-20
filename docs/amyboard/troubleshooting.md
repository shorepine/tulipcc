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

## Can't flash / the firmware upgrader won't connect

Only **one** program can use the serial port at a time, so before flashing make sure nothing else has it open. A port held by another program is the most common reason **Search for AMYboard** never connects.

 - **Close the Arduino IDE** (or at least its **Serial Monitor**) -- it holds the port open and blocks flashing.
 - **Quit any `mpremote` session** connected to the board, and close the Python REPL or any serial terminal.
 - **Quit any `screen` session** on the port -- `Ctrl-A` then `K` to kill it.
 - Any **DAW** that grabbed the port will block flashing too -- close it.
 - When in doubt, unplug and replug the AMYboard, then click **Search for AMYboard** again.

## USB MIDI not recognized

A healthy AMYboard enumerates as **two** USB things at once: a USB **MIDI** device *and* a USB **serial** (CDC) port, with USB VID `0xCAF0` and PID `0x4009`. If you only ever see a *"USB JTAG/serial debug unit"* (VID `0x303A`), the firmware's USB stack never came up -- see [Is it a boot loop?](#is-it-a-boot-loop) below. That is the most common cause of "no USB MIDI" right now, and we're actively working on it ([#952](https://github.com/shorepine/tulipcc/issues/952), [#980](https://github.com/shorepine/tulipcc/issues/980)).

 - Did you hit **RST** after flashing? Try that first.
 - Use a **data** USB-C cable (many cables are charge-only) and plug **directly** into the computer -- no hubs or extension cables.
 - Make sure **nothing else is holding the port**: close `mpremote`, serial terminals, the Python REPL, and any DAW that grabbed it. Only one program can own the connection at a time.
 - On **macOS**, open **Audio MIDI Setup** and check if AMYboard appears.
 - On **Windows**, check Device Manager under **Sound, video and game controllers** (see the [Windows](#windows) section below).
 - On **Linux**, check `aplaymidi -l` / `amidi -l` and `lsusb` -- you want to see `caf0:4009`, not `303a:...`.

### Is it a boot loop?

A number of boards get **stuck re-enumerating** instead of finishing boot. Tell-tale signs:

 - It appears **only** as a `303a:...` *USB JTAG/serial debug unit*, never as `caf0:4009`.
 - You hear a faint **click/pop in the audio out about once per second** -- that's the board rebooting over and over.
 - `mpremote` connects but hangs with **no `>>>` prompt**, and Ctrl-C won't break in.

A strong clue this is firmware and not your computer: the **Arduino** AMY firmware on the *same board* enumerates fine (as `caf0:0001`, USB MIDI works), while the **MicroPython** `amyboard-full` firmware gets stuck. To get going right now:

 - **First, just retry:** unplug, replug, hit **RST**. It often comes up on the 2nd or 3rd try.
 - **Most reliable workaround -- use an Android phone:** plug AMYboard into an **Android phone** with a USB-C cable and open [amyboard.com](https://amyboard.com/editor/) in Chrome there. This works for many people for whom Windows doesn't, and lets you load sketches and patches in the meantime. (iPhones/iPads don't do USB MIDI, so they won't work for this.)
 - **Linux:** force a clean re-enumeration without cutting power -- `sudo usbreset 303a:1001`, or `sudo usb_modeswitch -v303a -p1001 -W -R`. The `caf0:4009` MIDI device then appears.
 - Try a **known-good data cable** and a **rear/motherboard USB port** -- a port that can't supply enough current can trigger the reboot loop.

### Windows

Windows is the hardest case: once it has cached the board as a JTAG/serial device it tends to keep doing so, even after the board itself is fine.

 - **Confirm the symptom** in Device Manager. A broken board shows as **"USB JTAG/serial debug unit"** (and a **"USB Composite Device"** under *Universal Serial Bus controllers*) but has **nothing** under **Sound, video and game controllers**. A working board shows **AMYboard** under *Sound, video and game controllers*.
 - **Force a re-detect** (this has worked for several people): run the built-in hardware troubleshooter, apply its fix, and **reboot**:
   ```
   msdt.exe -id DeviceDiagnostic
   ```
   After the reboot, AMYboard often appears under *Sound, video and game controllers* and the online editor can connect. You may have to repeat this after each unplug/replug until the firmware fix lands.
 - **Remove ghost devices:** in Device Manager choose **View → Show hidden devices**, then uninstall any greyed-out AMYboard / "USB Serial Device" / JTAG entries and re-scan for hardware. Windows sometimes assigns a **new COM port each time** (COM4 vs COM6, etc.) and only binds MIDI on some of them.
 - If it works in a **DAW** but not the editor, the board is fine -- it's a browser/WebMIDI issue; see the editor steps below.

## AMYboard Online issues

### Web MIDI not working
 - Web MIDI requires **Chrome or Edge** (desktop). Firefox and Safari do not support Web MIDI.
 - Make sure your MIDI device is connected before opening amyboard.com.
 - Click the MIDI device selector and choose your device.
 - On some systems, you may need to grant MIDI access permission.

### "Could not connect to AMYboard" / the "Pull from AMYboard" popup keeps reappearing

 - Make sure you are **not** connected to AMYboard over a serial connection (mpremote, a terminal, the Python REPL) -- disconnect it and try again.
 - **Clear the site data** for amyboard.com and restart the browser: DevTools → **Application → Storage → Clear site data** (or the address-bar site-settings menu). Stale stored state is a common cause of the Pull popup looping.
 - If the board responds to MIDI elsewhere (a DAW, MIDIKeys, [a web MIDI tester](https://controllertest.io/midi-tester)) but the editor still can't Pull, **retry the Pull** -- it often succeeds on the second attempt once the board has finished its reboot.
 - Look at the browser's **JS console** for diagnostic messages.
 - [Re-flash your firmware](firmware.md) if you're still stuck.

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
 - Expected addresses: `0x40` (PCM9211), `0x48` (ADS1015 ADC), `0x58` (GP8413 DAC), `0x49` (Seesaw encoders).
 - Check your wiring -- SDA and SCL may be swapped.
 - Make sure pull-up resistors are present on your I2C lines (AMYboard has them built in for the onboard devices).

[Back to Getting Started](README.md)
