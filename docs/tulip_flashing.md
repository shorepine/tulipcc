# Compiling and flashing Tulip CC

## Compile TulipCC for ESP32-S3

[Tulip CC](../README.md) should build on all platforms, although I've only tested macOS and Linux so far. Please let me know if you have any trouble!

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

For both macOS & Linux, next, download the supported version of ESP-IDF. That is currently 5.1-rc2. [You can download it directly here.](https://dl.espressif.com/github_assets/espressif/esp-idf/releases/download/v5.1-rc2/esp-idf-v5.1-rc2.zip) Unpack it to a folder. I like to keep them in `~/esp/`, as you'll likely want to use different versions eventually. So we'll assume it's in `~/esp/esp-idf-v5.1-rc2`.

Also, download or clone this Tulip repository. We'll assume it's in `~/tulipcc`.

```bash
cd ~
mkdir esp
cd esp
curl -O https://dl.espressif.com/github_assets/espressif/esp-idf/releases/download/v5.1-rc2/esp-idf-v5.1-rc2.zip
unzip esp-idf-v5.1-rc2.zip
esp-idf-v5.1-rc2/install.sh esp32s3
source ~/esp/esp-idf-v5.1-rc2/export.sh

cd ~
git clone https://github.com/bwhitman/tulipcc.git # or download https://github.com/bwhitman/tulipcc/archive/refs/heads/main.zip
pip3 install Cython
pip3 install littlefs-python==0.4.0 # needed to flash the filesystem
cd ~/tulipcc/tulip/esp32s3
```

### Build and flash Tulip 

Now connect your Tulip to your computer over USB. If using a breakout board, connect it to the UART connector, not the USB connector. If using our Tulip board, use the USB-C connector and make sure Tulip is on. 

If you're using the `N8R8` (8MB flash), make sure to use `-DMICROPY_BOARD=TULIP4_N8R8`. For the 32MB (N32R8), including the Tulip CC integrated board, you can omit specifying it.

For example, for the N8R8 chip only:

```bash
idf.py -D MICROPY_BOARD=TULIP4_N8R8 flash 
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `fs` if you're developing Tulip itself.
python tulip_fs_create.py N8R8
```

For all other boards/chips:

```bash
idf.py -D flash 
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `fs` if you're developing Tulip itself.
python tulip_fs_create.py
```

You may need to restart Tulip after the flash, bt Tulip should now just turn on whenever you connect USB or power it on. 

To build / debug going forward:

```bash
cd tulip/esp32s3
source ~/esp/esp-idf-v5.1-rc2/export.sh # do this once per terminal window
idf.py flash
idf.py monitor # shows stderr, use control-] to quit

# If you make changes to the underlying python libraries on micropython, you want to fully clean the build 
idf.py fullclean
idf.py flash
```

## Flash a Tulip without compiling

**Please note, we don't recommend doing this yet.** Tulip is fast moving and it's really better if you figure out how to get the build environment going. It's not that hard. But if you want to try flashing your board without having to build Tulip, here's how:

 * Install [`esptool`](https://docs.espressif.com/projects/esptool/en/latest/esp32/installation.html) if you don't already have it
 * Make sure you have the serial port driver for your board installed. You should see a new /dev/\*USB\* or COM port when attaching it. If you don't, find the drivers for it and install them.
 * Download the 4 bin files in the [latest ESP32-S3 release](https://github.com/bwhitman/tulipcc/releases) and put them all in a folder
 * Connect your board / chip to USB
 * From the folder you have the .bin files in, run `esptool.py write_flash 0x0 bootloader.bin 0x10000 micropython.bin 0x8000 partition-table.bin 0x300000 tulip-lfs.bin`
 * Turn off and on the power to Tulip to start it 



## Windows build and flash

TODO 

## Questions

Any questions? [Chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)

