# Compiling and flashing Tulip CC

[Tulip CC](../README.md) should build on all platforms, although I've only tested macOS so far. Please let me know if you have any trouble!

## macOS build and flash

To compile your own firmware, start by cloning this repository. 

On macOS, install homebrew if you haven't already:

```bash
# install homebrew first, skip this if you already have it...
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Then restart your terminal
```

Do some one-time only setup commmands: 

```bash
# install esp-idf's requirements
brew install cmake ninja dfu-util

# and install our version of the ESP-IDF that comes with our repository
cd esp-idf
./install.sh esp32s3
source ./export.sh

pip3 install Cython
pip3 install littlefs-python # needed to flash the filesystem
cd ../ports/esp32s3
```

Now connect your Tulip to your computer over USB. If using a breakout board, connect it to the UART connector, not the USB connector. If using our Tulip board, use the USB-C connector. 

**If you are using our integrated board, or the WROOM-2 N32R8 board, use `-DMICROPY_BOARD=TULIP4_N32R8` below. If using a WROOM-1 N8R8 board, use `-DMICROPY_BOARD=TULIP4` below.**

```bash
idf.py -D MICROPY_BOARD=TULIP4 flash # or -DMICROPY_BOARD=TULIP4_N32R8
# With a brand new chip or devboard, the first time, you'll want to flash Tulip's filesystem 
# to the flash memory. Run this only once, or each time you modify `tulip_home` if you're developing Tulip itself.
python tulip_fs_create.py
```

Tulip should now just turn on whenever you connect USB or power it on. 

To build / debug going forward (and remember to use the right `-DMICROPY_BOARD` value for your board)

```bash
cd ports/esp32s3
export ../../esp-idf/export.sh # do this once per terminal window
idf.py -D MICROPY_BOARD=TULIP4 flash
idf.py -D MICROPY_BOARD=TULIP4 monitor # shows stderr, use control-] to quit

# If you make changes to the underlying python libraries on micropython, you want to fully clean the build 
idf.py -D MICROPY_BOARD=TULIP4 fullclean
idf.py -D MICROPY_BOARD=TULIP4 flash
```


## Linux build and flash

TODO


## Windows build and flash

TODO 

## Questions

Any questions? [Chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)

