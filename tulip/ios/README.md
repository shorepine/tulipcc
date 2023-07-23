# tulip for iOS

Work in progress

```
cd tulip/ios
./build-simulator.sh
./build-simulator.sh Debug
./build-device.sh
./build-device.sh debug
```

Will pull down SDL on first run. 

It will build micropython/Tulip as a static lib, then run xcode and open the simulator

TODOs:
 - XXX download/move SDL into here -- or share SDL among the ports?
 - XXX get simulator/real device build as a makefile parameter
 - XXX debug/release setting as well
 - XXX pass off makefile build to xcode via `./build.sh`
 - Keyboard crasher on first launch
 - install / run on device
 - soft keyboard is not returning symbols to Tulip, like `(` etc  (hard keyboard works)
 - screen size is not using hidpi -- for phone this is required
 - load example filesystem on first boot to Documents folder
 - MIDI
 - test touch




