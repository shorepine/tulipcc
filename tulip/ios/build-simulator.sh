#!/bin/bash
# Just builds for simulator locally

# https://gist.github.com/jerrymarino/1f9eb6a06c423f9744ea297d80193a9b


source ../shared/grab_submodules.sh
SCHEME=Debug

#iPad 10
#IOS_SIM_UDID=F3633360-AAFB-4BE8-9F1A-FAD58D475A0F
#IOS_SIM_STRING='iOS Simulator,name=iPad (10th generation),OS=16.4'

#iPhone 14 Pro
IOS_SIM_UDID=782C52DA-FBD4-4027-BA54-CCC62344A7EA
IOS_SIM_STRING="iOS Simulator,name=iPhone 14 Pro,OS=16.4"


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
    make TOOLCHAIN=iphonesimulator lib
  else
    SCHEME=Debug
    echo "Building for debug"
    make TOOLCHAIN=iphonesimulator DEBUG=1 lib
fi

cd tulip-desktop
xcodebuild -scheme "Simulator ${SCHEME}" -destination "platform=${IOS_SIM_STRING}"
APP_PATH=DerivedData/tulip-desktop/Build/Products/${SCHEME}-iphonesimulator/tulip-desktop.app

# Install the program 
xcrun simctl install booted $APP_PATH

# Start the program 
open -a 'Simulator' --args -CurrentDeviceUDID $IOS_SIM_UDID
xcrun simctl launch --console $IOS_SIM_UDID org.rosaline.tulip-desktop



