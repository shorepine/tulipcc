# Merging in Micropython and ESP-IDF guide

This is a very informal guide to making Tulip CC work with clean copies of Micropython (MP) and ESP-IDF (IDF). You should never need to do this work, unless you're interested in getting Tulip to run on a version of IDF or MP that we haven't supported yet. These are my notes from doing the work in May 2023 after upgrading to newer versions of MP and IDF (last updated May 2022!)

Can you help? I'd **love** to make this process easier, and act less like a "fork" of MP. It would be great if Tulip's build system pulls down the IDF and MP versions we need before building, so we don't have to ship our repo with all the forked code. We have made changes (and pull requests!) to a few MP libraries, so we do have to keep our versions around or merge them in while we wait for the PRs to come through. I don't know the best way to do all this, so any help is quite welcome.

## MP and Desktop

It's best to get Desktop working first (no IDF yet). 

Get the latest MP build. And latest Tulip. 

In MP, copy ports/unix to ports/macos. Test that this code works first by just building ports/macos (still just `unix`) by following its build instructions and running `build-standard/micropython` -- you should see a bare MP prompt. Now add the Tulip parts:

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

And copy `extmod/tulip` and `extmod/amy` over.

We have to patch only one thing in MP itself, and I would love to stop doing this (suggestions welcome.) Something in MacOS conflicts with MP, `unichar`. I do a global find/replace in the MP repo for `unichar` and replace it with `mp_unichar`. This works fine. I think we could eventually do some `undef` magic to get the conflict to stop.

Now, manually cherry-pick merge in our changes in ports/macos for these files. This part is hard to automate. Sometimes the APIs change so that we can't simply just paste in (or link from another file) the key bits we need to make Tulip boot. `main.c` is usually complicated due to how we thread out the services, and link to SDL. `unix_mphal` is easier -- we just overwrite the printing function to point to our own print. 

 * main.c
 * unix_mphal.c
 * mpconfigport.h

There will be small API changes in MP that you'll have to fix, probably in `modtulip.c`, but it should be straigthforward. 

This is all you need to get Desktop working on a fresh MP.

## ESP-IDF

This assumes you've already done the Desktop merging.

IDF's main branch is rarely what you want. You should first check what the latest IDF version MP supports. As of now, that's 4.4.4. 

So download their combined zip from their github release page and put it in `esp-idf` in our Tulip root. 

Run ./install.sh in it to make sure it grabs the latest toolchain. 

It would be great to use 5.X to get a few key IDF features but until MP has ported their `esp32` port to it, it's going to be hard for us to do the same. 

First, make sure the MP latest you got in the earlier step compiles fine in its `esp32` port. Just go into the `esp32` folder and do `source ../../esp-idf/export.sh`, then `make submodules`, `idf.py build`. That should make a working MP-only (no Tulip) bin to flash to a ESP. 

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
 * main.c -- is more straightforward than Desktop. Just have main() kick off the tasks for the subsystems. We probably could put this code somewhere else and link to it. 
 * uart.c -- we redirect stdout/in for Tulip this way. 

Now, set up the menuconfig / board. 
 * Copy `boards/ESP32S3_SPIRAM_OCT` to `boards/TULIP4`. 
 * Rename the directory names in `boards.json` and `mpconfigboard...` to point to `TULIP4`. 
 * Change the partitions file name to `partitions.csv`.

(TBD -- an 8MB partition and `TULIP4_N8R8` board spec without OTA and octal flash) 

Merge in `boards/manifest.py` from Tulip. This changes a lot -- some modules will be in different places.

Remove `_boot.py` from `modules`. We have our own in `extmod/tulip/py`.

Now, `idf.py -DMICROPY_BOARD=TULIP4 menuconfig`. Make the following changes:
 * Component config -> ESP32S3 specific -> CPU frequency - 240MHz
 * Component config -> ESP32S3 specific -> SPIRAM -> Make SPIRAM allocatable by heap_caps...
 * Component config -> ESP32S3 specific -> SPIRAM -> Try to allocate memories of Wifi and LWIP...
 * Component config -> FreeRTOS -> Enable FreeRTOS trace facility
 * Serial Flasher -> 16MB size (even if you have 32, as of right now there's a bug with 32 and IDF 4.4.4.)
 * Serial Flash -> Enable octal flash

Save it, and copy it `cp build/sdkconfig boards/TULIP4/sdkconfig.board`

Update the partitions.csv if need be. Also update `tulip_fs_create` with a new FS size if need be. 

That should be it. `idf.py -DMICROPY_BOARD=TULIP4 flash` should build and flash.





