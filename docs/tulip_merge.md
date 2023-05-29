# Merging in Micropython and ESP-IDF guide

This is a very informal guide to making Tulip CC work with clean copies of Micropython and ESP-IDF. You should never need to do this work, unless you're interested in getting Tulip to run on a version of ESP-IDF or Micropython that we haven't supported yet. These are my notes from doing the work in May 2023 after upgrading to newer versions of MP and ESP-IDF (last updated a year ago!)

## Start with Desktop

It's best to get Desktop working first (no ESP-IDF yet). 

Get the latest MP build. And latest Tulip. 

In MP, copy ports/unix to ports/macos.

From Tulip, copy over the following from ports/macos:
 * Makefile
 * build.sh
 * package.sh
 * multicast.c
 * unix_display.c
 * SDL2.framework/
 * virtualmidi.m
 * variants/manifest.py
 * Info.plist
 * These are for libsoundio, which we want to replace, but:
    * libs
    * libsoundio*
    * soundio*

We have to patch only one thing in Micropython itself, and I would love to stop doing this (suggestions welcome.) Something in MacOS conflicts with MP, `unichar`. I do a global find/replace in the MP repo for `unichar` and replace it with `mp_unichar`. This works fine. I think we could eventually do some `undef` magic to get the conflict to stop.

Now, manually cherry-pick merge in our changes in ports/macos for these files:

 * main.c
 * unix_mphal.c
 * mpconfigport.h

This is all you need to get Desktop working on a fresh MP.

## ESP-IDF

ESP-IDF's main branch is rarely what you want. You should first check what the latest ESP-IDF version MP supports. As of now, that's 4.4.4. So download their combined zip from their github release page and put it in `esp-idf` in our Tulip root. Run ./install.sh in it to make sure it grabs the latest toolchain.

Make sure the MP latest compiles fine in its `esp32` port. Just go into the `esp32` folder and do `source ../../esp-idf/export.sh`, then `make submodules`, `idf.py build`. That should make a working MP-only (no Tulip) bin to flash to a ESP. 

Next, copy `ports/esp32` into `esp32s3`. Copy across the following files from Tulip:

  * multicast.c
  * esp32s3_display*
  * esp32s3_joy*
  * usb_keyboard*
  * components/bw_esp_lcd*
  * touchscreen*
  * tasks.h
  * tulip_fs_create.py

[Note: we are hoping to move away from `bw_esp_lcd`, but the bounce buffer support is 5.X only, and MP is stuck to 4.4x right now.]

In `mpconfigport.h` change all the `esp32` directory names to `esp32s3`.

I also had to change `lib/berkeleydb-1.xx/PORT/include/queue.h` to test `#ifndef LIST_INT` as it was conflicting with something. 

Now merge in Tulip files:

 * main/CMakeLists.txt -- the compiler options and list of Tulip files to compile
 * main.c
 * uart.c

Now, set up the menuconfig / board. 
 * Copy `boards/ESP32S3_SPIRAM_OCT` to `boards/TULIP4`. 
 * Rename the directory names in `boards.json` and `mpconfigboard...` to point to `TULIP4`. 
 * Change the partitions file name to `partitions.csv`.

Merge in `boards/manifest.py` from Tulip. This changes a lot -- some modules will be in different places.

Remove `_boot.py` from `modules`. We have our own.

Now, `idf.py -DMICROPY_BOARD=TULIP4 menuconfig`. Make the following changes:
 * Component config -> ESP32S3 specific -> CPU frequency - 240MHz
 * Component config -> ESP32S3 specific -> SPIRAM -> Make SPIRAM allocatable by heap_caps...
 * Component config -> ESP32S3 specific -> SPIRAM -> Try to allocate memories of Wifi and LWIP...
 * Component config -> FreeRTOS -> Enable FreeRTOS trace facility
 * Serial Flasher -> 16MB size (even if you have 32, as of right now there's a bug with 32)
 * Serial Flash -> Enable octal flash

Save it, and copy it `cp build/sdkconfig boards/TULIP4/sdkconfig.board`

Update the partitions.csv if need be. Also update `tulip_fs_create` with a new FS size if need be. 

That should be it. `idf.py -DMICROPY_BOARD=TULIP4 flash` should build and flash.





