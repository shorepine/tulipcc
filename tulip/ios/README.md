# Tulip Desktop for iOS

Work in progress port of Tulip Desktop to iOS devices (iPad & iPhone.)

```
# First install findsimulator
brew tap a7ex/homebrew-formulae
brew install findsimulator

# To build for simulator
./clean.sh
./build-simulator.sh # builds release for simulator
./build-simulator.sh Debug
# On first run of the simulator build, you'll want to run ./build-simulator.sh twice as the simulator will still be booting
# Quit the simulator if you're changing device types (you can set them in ./build-simulator.sh)

# To build for device
./clean.sh
./build-device.sh # builds release for device
./build-device.sh Debug
# You have to click the run button manually in xcode to install/run on device
```

Will pull down SDL source on first run. 

Builds micropython/Tulip as a static lib, then runs xcodebuild of the app bundle, then opens the simulator, installs and runs (with console logging)



TODOs:
 - XXX download/move SDL into here -- or share SDL among the ports?
 - XXX get simulator/real device build as a makefile parameter
 - XXX debug/release setting as well
 - XXX pass off makefile build to xcode via `./build.sh`
 - XXX Keyboard crasher on first launch
 - XXX screen size is not using hidpi -- for phone this is required
 - XXX reload display when rotation changes
 - XXX screen not in proper viewport rect
 - XXX get simulator UDID automatically from string
 - XXX soft keyboard is not returning symbols to Tulip, like `(` etc  (hard keyboard works)
 - XXX install / run on device
 - XXX "hardware keyboard" uses keysyms, where the soft keyboard uses textinput. annoying 
 - control-C, arrow keys, tab as soft buttons above keyboard
 - use keyboard height to set screen size
 - fill up display nicely with default H_RES/V_RES and give option to go into 1:1 tulip mode
 - load example filesystem on first boot to Documents folder
 - MIDI
 - touch
 - testflight




