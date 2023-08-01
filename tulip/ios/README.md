# Tulip Desktop for iOS

Work in progress port of Tulip Desktop to iOS devices (iPad & iPhone.)


```
# First install findsimulator
brew tap a7ex/homebrew-formulae
brew install findsimulator

# To build for simulator
./clean.sh
./build-simulator.sh # builds release for simulator, add "debug" if you want that
# On first run of the simulator build, you'll want to run ./build-simulator.sh twice as the simulator will still be booting
# Quit the simulator if you're changing device types (you can set them in ./build-simulator.sh)

# To build for device
./clean.sh
./build-device.sh # builds release for device
# You have to click the run button manually in xcode to install/run on device
```

Will pull down SDL source on first run. 

Builds micropython/Tulip as a static lib, then runs xcodebuild of the app bundle, then opens the simulator, installs and runs (with console logging)

TODOs:
 - unreliable resizing still, tbd 
 - testflight

![iOS](../../docs/pics/ios_1.png)
![iOS](../../docs/pics/ios_2.png)


