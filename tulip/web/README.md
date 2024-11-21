# Tulip web port

this is a very work in progress port of Tulip Desktop to the web. 

Our _goal_ is to have this be the official tulip desktop experience, if we can get it there. you can still run "native" tulip desktop on linux/mac by compiling it if you'd rather. 

This will help us lessen our support load and also expose Tulip to people on other platforms (windows!) without having to download anything. 

## Build

```
./build.sh
python3 server.py
# go to localhost:8000 
```

## Use

I test on Chrome right now. I don't think safari supports webmidi.

The tulip "sys" files will load into `/tulip4/sys`. `/tulip4/user` is there for your files.

The file system persists in your browser as an IndexedDB localstorage thing. To delete it, do `indexedDB.deleteDatabase('/tulip4/user')` in the console.


## what works
 - python REPL
 - tulip BG functions, graphics
 - LVGL
 - AMY (loaded as a separate audioworklet), including sequencer
 - keyboard & mouse via SDL
 - persistent filesystem in localstorage, including `sys` folder

## TODO
 - speed of LVGL (try `run('drums')`)
 - ~~T-sequencer running too slowly to hit every tick #, so % dividers get missed (see LEDs in drums)~~
 - "sockets"/web requests - tulip world etc
 - ~~FS needs examples loaded on boot~~
 - ~~FS needs to persist to localstorage~~
 - MIDI
 - some solution for `time.sleep` / `sleep_ms` -- see `parallax`, `bunny_bounce`, `xanadu`
 - sync my AMY changes to main AMY 
 - test github hosting




