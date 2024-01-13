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

For both macOS & Linux, next, download the supported version of ESP-IDF. That is currently 5.2-beta2. [You can download it directly here.](https://dl.espressif.com/github_assets/espressif/esp-idf/releases/download/v5.2-beta2/esp-idf-v5.2-beta2.zip) Unpack it to a folder. I like to keep them in `~/esp/`, as you'll likely want to use different versions eventually. So we'll assume it's in `~/esp/esp-idf-v5.2-beta2`.

Also, clone this Tulip repository. We'll assume it's in `~/tulipcc`.

```bash
cd ~
mkdir esp
cd esp
curl -O https://dl.espressif.com/github_assets/espressif/esp-idf/releases/download/v5.2-beta2/esp-idf-v5.2-beta2.zip
unzip esp-idf-v5.2-beta2.zip
esp-idf-v5.2-beta2/install.sh esp32s3
source ~/esp/esp-idf-v5.2-beta2/export.sh

cd ~
git clone https://github.com/bwhitman/tulipcc.git 
pip3 install Cython
pip3 install littlefs-python # needed to flash the filesystem
cd ~/tulipcc/tulip/esp32s3
```

### Build and flash Tulip 

Now connect your Tulip to your computer over USB. If using a breakout board, connect it to the UART connector, not the USB connector. If using our Tulip board, use the USB-C connector and make sure Tulip is on. 

Choose the right `MICROPY_BOARD` value for your board. 

 * Tulip4 (our integrated board): `TULIP4_N32R8`
 * Any Tulip board based on the N8R8 (8MB flash): `TULIP4_N8R8`
 * Any Tulip board based on the N16R8 (16MB flash): `TULIP4_N16R8`
 * For the [T-Deck](../tulip/tdeck/README.md), omit `MICROPY_BOARD` and make sure you're in the `tdeck` folder. 
 * For the [MaTouch 7", or Tulip CC v4r10](https://github.com/bwhitman/tulipcc/issues/160), omit `MICROPY_BOARD` and make sure you're in the `tulipcc_r10` folder.

The default is `TULIP4_N32R8`, so if you omit it that's what it'll use.

You have to flash the firmware (`idf.py flash`) and then, for **only the first install**, run `tulip_fs_create.py`. The latter will set up the storage on Tulip. Only do that once per chip, or if you make changes to the underlying filesystem.

For example, for the N8R8 chip only:

```bash
idf.py -D MICROPY_BOARD=TULIP4_N8R8 flash 
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `fs` if you're developing Tulip itself.
python tulip_fs_create.py TULIP4_N8R8
```

For the Tulip4 PCB or a board based on a N32R8:

```bash
idf.py flash 
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `fs` if you're developing Tulip itself.
python tulip_fs_create.py
```

You may need to restart Tulip after the flash, bt Tulip should now just turn on whenever you connect USB or power it on. 

To build going forward:

```bash
cd tulip/esp32s3
source ~/esp/esp-idf-v5.2-beta2/export.sh # do this once per terminal window
idf.py flash
idf.py monitor # shows stderr and stdin for controlling Tulip, use control-] to quit

# If you (or we!) make changes to the underlying libraries on AMY or micropython, you want to fully clean the build 
rm ../../.submodules_ok # this forces the submodules to re-init
idf.py fullclean
idf.py flash
```

[To debug using GDB or profile code, see our new guide on live debugging of the ESP32S3.](tulip_debug.md)

### Set REPL size smaller

If you want a smaller REPL, add this line

```
idf_build_set_property(COMPILE_OPTIONS "-DTULIP_REPL_FONT_8X6" APPEND)
```

To the bottom of `esp32s3/CMakeLists.txt`.


## Windows build and flash

TODO 

## Questions

Any questions? [Chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)

