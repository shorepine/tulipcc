#!/bin/bash
# Just builds for device locally

source ../shared/grab_submodules.sh
SCHEME=Debug
IOS_STRING='iOS,name=Yellow'

SDL_URL=https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.zip
SDL_FILENAME=SDL2-2.28.1.zip
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
    make TOOLCHAIN=iphoneos lib
  else
    SCHEME=Debug
    echo "Building for debug"
    make TOOLCHAIN=iphoneos DEBUG=1 lib
fi

cd tulip-desktop
xcodebuild -scheme "Simulator ${SCHEME}" -destination "platform=${IOS_STRING}"
APP_PATH=DerivedData/tulip-desktop/Build/Products/${SCHEME}-iphoneos/tulip-desktop.app


# Install the program 
#xcrun simctl install booted $APP_PATH

# Start the program 
#open -a 'Simulator' --args -CurrentDeviceUDID $IOS_SIM_UDID
#xcrun simctl launch $IOS_SIM_UDID org.rosaline.tulip-desktop



