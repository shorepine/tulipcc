#!/bin/bash
# build.sh
# Just builds locally

source ../shared/grab_submodules.sh

# Make debug and set up app bundle on mac
make DEBUG=1
mkdir -p dev
rm -rf dev/Tulip\ CC.app
mkdir -p dev/Tulip\ CC.app/Contents/{MacOS,Resources,Frameworks,libs}
cp build-standard/tulip.$(arch) dev/Tulip\ CC.app/Contents/MacOS/tulip
cp Info.plist dev/Tulip\ CC.app/Contents/
cp -rf ../fs dev/Tulip\ CC.app/Contents/Resources/tulip_home
cp -a SDL2.framework dev/Tulip\ CC.app/Contents/Frameworks/
install_name_tool -add_rpath @executable_path/../Frameworks dev/Tulip\ CC.app/Contents/MacOS/tulip



