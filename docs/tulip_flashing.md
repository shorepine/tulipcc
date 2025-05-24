# Upgrading, flashing or compiling Tulip CC

We will aim to release new versions of the Tulip firmware regularly. If you're developing on Tulip itself, you should learn how to compile and flash your own code. Otherwise, you can use our built in upgrading or full-device flash files to flash Tulip.

To flash from a computer, you *may* need a driver for a serial-USB chip on the Tulip you're working with. If your board uses a CH340X chip, and it is not detected by `idf.py`, [you may need to follow the instructions here to install a driver.](https://github.com/WCHSoftGroup/ch34xser_macos)


## Upgrade your working Tulip to the latest version

If your Tulip was flashed after our January 2024 release and is running fine and you just want to upgrade to the latest firmware, you can do that "over the air." Make sure your Tulip is connected to wifi and run `tulip.upgrade()`:

```python
>>> tulip.wifi("ssid", "password")
>>> tulip.upgrade()
```

It will ask you if you want to upgrade your firmware and/or your `/sys` folder (the place we store baked in programs and examples.) Usually, a new firmware means new features, so it's recommended to upgrade both so that they're using the same APIs. This whole process takes about five minutes, and your Tulip will reboot. Your saved files (in `/user`) are safe. Easy!

## Flash Tulip from a compiled release

If you've got an unflashed Tulip, just finished a DIY, or somehow messed up the flash / firmware, you can flash the entire Tulip and filesystem with one file from our releases. We aim to release versions of Tulip regularly. You can find the latest in our [releases section](https://github.com/shorepine/tulipcc/releases). Find the `.bin` file for your type of Tulip:

 * Any DIY Tulip board based on the N16R8 (16MB flash): `N16R8`
 * Any DIY Tulip board based on the N32R8 (32MB flash): `N32R8`
 * The [T-Deck](../tulip/tdeck/README.md): `TDECK`
 * Tulip CC (our integrated board with display): `TULIP4_R11`

Connect your Tulip to your computer with a USB cable. **Note**: Many Tulip-capable boards have two USB ports, one called UART, TTL or Serial, and one called NATIVE, JTAG, or Host. You should use the UART one if available and try the NATIVE one if not. For example, on the Tulip CC, you can use either USB port, but if you use the NATIVE port you have to hold down the BOOT button while attaching the USB cable. We recommend flashing using the top UART USB connector. On the T-Deck, you only have access to the NATIVE port, and you may need to hold down the BOOT button (the trackball button) while you turn it on. If you've tried both ports and the following commands can't find a serial port to flash to, ensure that you've [installed a driver.](https://github.com/WCHSoftGroup/ch34xser_macos)

Download the `.bin` for your board and use [`esptool.py`](https://docs.espressif.com/projects/esptool/en/latest/esp32/) or any other ESP32-flasher tool to write the entire `.bin` to flash:

**NOTE: This will delete anything you've saved on the Tulip or anything else on the flash of the device. It may also take up to 10-15 minutes for 32MB boards and appear to 'stall' near the end -- but don't worry, it's working!**

```bash
% pip install esptool # if you haven't installed it yet
% esptool.py write_flash 0x0 tulip-full-XXX.bin
```

Tulip should boot when it's finished (you may need to unplug/plug your USB cable.) You can use `tulip.upgrade()` going forward after this first flash.


## Compile and flash TulipCC for ESP32-S3

If you're working on Tulip itself or just like to see the latest bleeding edge before a release, you can compile [Tulip CC](../README.md) yourself. It should build on all platforms, although I've only tested macOS and Linux so far. Please let me know if you have any trouble!

### First time setup

macOS:
```bash
# install homebrew first, skip this if you already have it...
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Then restart your terminal
# install esp-idf's requirements
brew install cmake ninja dfu-util
```


Linux:
```bash
# install esp-idf's requirements
sudo apt install cmake ninja-build dfu-util virtualenv
```

For both macOS & Linux, next, download the supported version of ESP-IDF. That is release 5.4.1. You need to get this with `git`, so install that if you don't already have it. Once Espressif updates the release, we can provide a direct download link that's a bit easier. 

I like to keep them in `~/esp/`, as you'll likely want to use different versions eventually. So we'll assume it's in `~/esp/esp-idf`.

Also, clone this Tulip repository. We'll assume it's in `~/tulipcc`.

```bash
cd ~
mkdir esp
cd esp
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.4.1
cd esp-idf-v5.4.1
./install.sh esp32s3
source export.sh

cd ~
git clone https://github.com/shorepine/tulipcc.git 
pip3 install Cython
pip3 install littlefs-python # needed to flash the filesystem
cd ~/tulipcc/tulip/esp32s3
```

### Flash Tulip after compiling

Now connect your Tulip to your computer over USB. See the notes about which USB port to use above. 

Choose the right `MICROPY_BOARD` value for your board. 

 * Any DIY Tulip board based on the N16R8 (16MB flash): `N16R8`
 * Any DIY Tulip board based on the N32R8 (32MB flash): `N32R8`
 * The [T-Deck](../tulip/tdeck/README.md): `TDECK`
 * Tulip CC (our integrated board with display): `TULIP4_R11`

The default is `TULIP4_R11`, so if you omit it that's what it'll use.

You have to build the firmware (`idf.py -DMICROPY_BOARD=[X] build`) and then, for **only the first install**, run `fs_create.py tulip flash` to flash the entire filesystem to Tulip. This sets up the storage on Tulip. Only do that once per chip, or if you make changes to the underlying filesystem. Any further flashing can just use (the much faster!) `idf.py -DMICROPY_BOARD=[X] flash`, which will save time and not overwrite your file system!

For example, for a Tulip4 DIY board based on a N32R8:

```bash
idf.py -DMICROPY_BOARD=N32R8 build
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `fs` if you're developing Tulip itself.
cd ..
python fs_create.py tulip flash
```

You may need to restart Tulip after the flash, but Tulip should now just turn on whenever you connect USB or power it on. 

To build and flash going forward, without modifying the filesystem:

```bash
cd tulip/esp32s3
source ~/esp/esp-idf/export.sh # do this once per terminal window
idf.py -DMICROPY_BOARD=[X] flash 
idf.py monitor # shows stderr and stdin for controlling Tulip, use control-] to quit

# If you (or we!) make changes to the underlying libraries on AMY or micropython, you want to fully clean the build 
rm ../../.submodules_ok # this forces the submodules to re-init
idf.py fullclean
idf.py -DMICROPY_BOARD=[X] flash
```

[To debug using GDB or profile code, see our new guide on live debugging of the ESP32S3.](tulip_debug.md)


## Questions

Any questions? [Chat with us on our discussions page](https://github.com/shorepine/tulipcc/discussions) or [**chat about Tulip on our Discord!**](https://discord.gg/TzBFkUb8pG).

