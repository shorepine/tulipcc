#!/bin/bash
# build.sh
# Just builds locally
set -e

source ../shared/grab_submodules.sh

# TinyCC (libtcc) for tulip.install_c_process() (runtime user C DSP plugins).
# Like micropython, the tinycc submodule is expected to look dirty after this
# (configure writes config.mak/config.h in-tree).
pushd ../../ > /dev/null
if [ ! -f tinycc/libtcc.a ]; then
    git submodule update --init tinycc
    cd tinycc
    ./configure
    make libtcc.a libtcc1.a
    cd ..
fi
popd > /dev/null

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

# Stage the tcc runtime (libtcc1.a + headers) so install_c_process works from
# the app bundle without the dev tree (user_c_dsp.c looks in Resources/tcc).
if [ -f ../../tinycc/libtcc1.a ]; then
    mkdir -p dev/Tulip\ Desktop.app/Contents/Resources/tcc
    cp ../../tinycc/libtcc1.a dev/Tulip\ Desktop.app/Contents/Resources/tcc/
    cp -r ../../tinycc/include dev/Tulip\ Desktop.app/Contents/Resources/tcc/
fi

mkdir -p ~/.local/share/tulipcc
mkdir -p ~/.local/share/tulipcc/user
rm -rf ~/.local/share/tulipcc/sys
mkdir -p ~/.local/share/tulipcc/sys
cp -Rf ../fs/* ~/.local/share/tulipcc/sys/
