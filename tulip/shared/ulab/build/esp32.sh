#!/bin/bash

export BUILD_DIR=$(pwd)

git clone https://github.com/v923z/micropython-ulab.git ulab
git clone https://github.com/micropython/micropython.git

cd $BUILD_DIR/micropython/
git checkout tags/v1.14

git submodule update --init
cd ./mpy-cross && make # build cross-compiler (required)

cd $BUILD_DIR/micropython/ports/esp32
make ESPIDF= # will display supported ESP-IDF commit hashes
# output should look like: """
# ...
# Supported git hash (v3.3): 9e70825d1e1cbf7988cf36981774300066580ea7
# Supported git hash (v4.0) (experimental): 4c81978a3e2220674a432a588292a4c860eef27b

ESPIDF_VER=9e70825d1e1cbf7988cf36981774300066580ea7

mkdir $BUILD_DIR/micropython/esp32

cd $BUILD_DIR/micropython/esp32
git clone https://github.com/espressif/esp-idf.git esp-idf
cd $BUILD_DIR/micropython/esp32/esp-idf
git checkout $ESPIDF_VER
git submodule update --init --recursive # get idf submodules
pip install -r ./requirements.txt # install python reqs

curl https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xvz

cd $BUILD_DIR/micropython/ports/esp32
# temporarily add esp32 compiler to path
export PATH=$BUILD_DIR/micropython/esp32/esp-idf/xtensa-esp32-elf/bine:$PATH
export ESPIDF=$BUILD_DIR/micropython/esp32/esp-idf
export BOARD=GENERIC # board options are in ./board
export USER_C_MODULES=$BUILD_DIR/ulab # include ulab in firmware

make submodules & make all
