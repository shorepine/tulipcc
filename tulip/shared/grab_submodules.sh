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
    git submodule update --init --recursive lv_binding_micropython
    cd micropython
    git submodule update --init lib/axtls 
    git submodule update --init lib/libffi 
    git submodule update --init lib/mbedtls
    git submodule update --init lib/berkeley-db-1.xx
    git submodule update --init lib/micropython-lib 
    cd ..
    touch .submodules_ok
fi
popd > /dev/null

