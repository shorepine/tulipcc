#!/bin/bash
# Just builds for simulator locally
set -o xtrace
set -e
source ../shared/grab_submodules.sh

SCHEME=Release # override by giving this script any parameter

IOS_SIM_DEVICE='iPad (10th generation)'
#IOS_SIM_DEVICE='iPhone 14 Pro'

IOS_SIM_UDID=`findsimulator -o ios -m latest "${IOS_SIM_DEVICE}" | grep -o ".\{36\}$"`
IOS_SIM_DESTINATION=`findsimulator -o ios -m latest "${IOS_SIM_DEVICE}"`
SDL_URL=https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.zip
SDL_FILENAME=SDL2-2.28.1.zip

# Download SDL if you don't have it 
if test -f "$SDL_FILENAME"; then
    echo "$SDL_FILENAME already exists."
else
    echo "Downloading $SDL_URL"
    curl -L -O $SDL_URL
    unzip $SDL_FILENAME
fi

if [ -z "$1" ]
  then
    SCHEME=Release
    echo "Building for release"
    make TOOLCHAIN=iphonesimulator lib
  else
    SCHEME=Debug
    echo "Building for debug"
    make TOOLCHAIN=iphonesimulator DEBUG=1 lib
fi

cd tulip-desktop
xcodebuild -scheme "Simulator ${SCHEME}" -destination "${IOS_SIM_DESTINATION}"
APP_PATH=DerivedData/tulip-desktop/Build/Products/${SCHEME}-iphonesimulator/tulip-desktop.app

# Open the simulator
open -a 'Simulator' --args -CurrentDeviceUDID $IOS_SIM_UDID

# Install
xcrun simctl install booted $APP_PATH

# Run
xcrun simctl launch --console $IOS_SIM_UDID org.rosaline.tulip-desktop



