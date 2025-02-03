# Tulip folder structure

```
amyboard - Tulip (headless) for the AMYboard
amyrepl - micropython build for AMY-included python with no display or tulip stuff
esp32s3 - all TulipCC hardware specific files for the supported boards 
fs - the filesystem that gets flashed as /sys on first run -- examples, images, etc
linux - all Tulip Desktop for Linux specific files
macos - all Tulip Desktop for macOS specific files
server - code that handles server proxies for Tulip World
shared - code shared between all Tulip ports (hardware & Desktop & web)
shared/py - Python modules that get loaded into Tulip
shared/desktop - code shared between Tulip Desktop ports (macOS, iOS, Linux, Web)
shared/ulab - our fork of ulab, a numpy/scipy wrapper for micropython
web - all Tulip Web specific files
```


