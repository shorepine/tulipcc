# Tulip web port

this is a very work in progress port of Tulip Desktop to the web. 

Our _goal_ is to have this be the official tulip desktop experience, if we can get it there. you can still run "native" tulip desktop on linux/mac by compiling it if you'd rather. 

This will help us lessen our support load and also expose Tulip to people on other platforms (windows!) without having to download anything. 

## Build

```
./build.sh
# go to localhost:8000 
```

## deploy

```
(first make sure you've build.sh)
./deploy.sh
```

## Use

The tulip "sys" files will load into `/tulip4/sys`. `/tulip4/user` is there for your files. Unlike other tulip ports, `/tulip4/sys` is "read only" in the sense that it resets back to our shipped FS default every page load. `/tulip4/user` will persist in your browser as an IndexedDB localstorage thing. 


## what works
 - python REPL
 - tulip BG functions, graphics layer, TFB, sprites etc
 - LVGL
 - AMY (loaded as a separate audioworklet), including sequencer
 - keyboard & mouse via SDL
 - persistent filesystem in localstorage, including `sys` examples folder
 - MIDI input & output w/ selectable devices
 - Tulip world, `url_save`


## what doesn't (yet)
 - `input`
 - `time.sleep()`
 - we boot AMY without audio in, because the UX for microphone access is scary and bad. We can later add an explicit opt in for it

## TODO
 - ~~T-sequencer running too slowly to hit every tick #, so % dividers get missed (see LEDs in drums)~~
 - ~~FS needs examples loaded on boot~~
 - ~~FS needs to persist to localstorage~~
 - ~~MIDI output~~
 - ~~MIDI device selector dropdown~~
 - ~~audio device dropdown if possible?~~ - [seems not fully supported yet](https://developer.mozilla.org/en-US/docs/Web/API/Audio_Output_Devices_API#browser_compatibility)
 - ~~restart tulip button~~
 - ~~delete storage button~~
 - ~~get working on safari / iOS safari minus webmidi~~
 - ~~start LVGL before onclick for audio so it's not a black screen on launch~~
 - ~~sync my AMY changes to main AMY~~
 - ~~build in release.sh~~ 
 - ~~speed of LVGL object creation(?) (try `run('drums')`)~~
 - ~~startup bleep (and fix the juno bleep on macos desktop!)~~
 - ~~test changes on tulip desktop mac / linux and tulip cc~~
 - ~~test github hosting on tulip.computer/~~
 - ~~safari startup bleep sounds bad -- chrome is fine~~
 - ~~"sockets"/web requests - tulip world etc~~
 - ~~some solution for `time.sleep` / `sleep_ms` -- see `parallax`, `bunny_bounce`, `xanadu`~~
 - ~~resizeable/responsive SDL canvas~~ 
 - ~~maybe some simple intro HOWTO on the page~~ 
 - audio input opt-in checkbox 

 
