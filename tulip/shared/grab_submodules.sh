#!/bin/bash

# grab_submodules.sh

# Do the first run submodule init if needed
echo `pwd`
pushd ../../ > /dev/null
if test -f ".submodules_ok"; then
    echo "Submodules already loaded";
else
    echo "Syncing submodules for first build..."
    git submodule update --init amy
    git submodule update --init micropython
    cd micropython
    # Patch mpy-cross warnings for newer clang versions on macOS.
    if ! grep -q -- "-Wno-gnu-folding-constant" mpy-cross/Makefile; then
        sed -i.bak 's/^CWARN += /CWARN += -Wno-gnu-folding-constant /' mpy-cross/Makefile
    fi
    git submodule update --init lib/axtls 
    git submodule update --init lib/libffi 
    git submodule update --init lib/mbedtls
    git submodule update --init lib/berkeley-db-1.xx
    git submodule update --init lib/micropython-lib 
    git submodule update --init lib/tinyusb 
    cd ..
    touch .submodules_ok
fi
popd > /dev/null
