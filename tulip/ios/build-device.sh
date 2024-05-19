#!/bin/bash
# Just builds for simulator locally
set -o xtrace
set -e
source ../shared/grab_submodules.sh

SCHEME=Release # override by giving this script any parameter
SDL_VERSION=SDL2-2.28.1



SDL_URL=https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.zip
SDL_FILENAME=${SDL_VERSION}.zip

# Download SDL if you don't have it 
if test -f "$SDL_FILENAME"; then
    echo "$SDL_FILENAME already exists."
else
    echo "Downloading $SDL_URL"
    curl -L -O $SDL_URL
    unzip $SDL_FILENAME
fi

# We have to keep a few files local here as we have to make changes to them
cp SDL_* ${SDL_VERSION}/src/video/uikit/

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
xcodebuild  test -scheme "Simulator ${SCHEME}" -destination "platform=iOS,name=cardinal"

#APP_PATH=DerivedData/tulip-desktop/Build/Products/${SCHEME}-iphoneos/tulip-desktop.app



