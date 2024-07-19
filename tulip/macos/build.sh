#!/bin/bash
# build.sh
# Just builds locally
set -e

source ../shared/grab_submodules.sh

# Make debug and set up app bundle on mac
make DEBUG=1
mkdir -p dev
rm -rf dev/Tulip\ Desktop.app
mkdir -p dev/Tulip\ Desktop.app/Contents/{MacOS,Resources,Frameworks,libs}
cp build-standard/tulip/obj/tulip.$(arch) dev/Tulip\ Desktop.app/Contents/MacOS/tulip
cp Info.plist dev/Tulip\ Desktop.app/Contents/
cp tulip.icns dev/Tulip\ Desktop.app/Contents/Resources/
cp -rf ../fs dev/Tulip\ Desktop.app/Contents/Resources/tulip_home
cp -a SDL2.framework dev/Tulip\ Desktop.app/Contents/Frameworks/
install_name_tool -add_rpath @executable_path/../Frameworks dev/Tulip\ Desktop.app/Contents/MacOS/tulip

mkdir -p ~/.local/share/tulipcc
mkdir -p ~/.local/share/tulipcc/user
rm -rf ~/.local/share/tulipcc/sys
mkdir -p ~/.local/share/tulipcc/sys
cp -Rf ../fs/* ~/.local/share/tulipcc/sys/
