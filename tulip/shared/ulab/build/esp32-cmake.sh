#!/bin/bash

export BUILD_DIR=$(pwd)

echo "--- CLONING ULAB ---"
git clone --depth 1 https://github.com/v923z/micropython-ulab.git ulab

echo "--- CLONING MICROPYTHON ---"
git clone --depth 1 https://github.com/micropython/micropython.git

echo "--- CLONING ESP-IDF ---"
cd $BUILD_DIR/micropython/
git clone --depth 1 -b v4.0.2 --recursive https://github.com/espressif/esp-idf.git

echo "--- INSTALL ESP-IDF ---"
cd $BUILD_DIR/micropython/esp-idf
./install.sh
. ./export.sh

echo "--- MPY-CROSS ---"
cd $BUILD_DIR/micropython/mpy-cross
make

echo "--- ESP32 SUBMODULES ---"
cd $BUILD_DIR/micropython/ports/esp32
make submodules

echo "--- PATCH MAKEFILE ---"
cp $BUILD_DIR/micropython/ports/esp32/Makefile $BUILD_DIR/micropython/ports/esp32/MakefileOld
echo "BOARD = GENERIC" > $BUILD_DIR/micropython/ports/esp32/Makefile
echo "USER_C_MODULES = \$(BUILD_DIR)/ulab/code/micropython.cmake" >> $BUILD_DIR/micropython/ports/esp32/Makefile
cat $BUILD_DIR/micropython/ports/esp32/MakefileOld >> $BUILD_DIR/micropython/ports/esp32/Makefile

echo "--- MAKE ---"
make
