#!/bin/bash

export BOARD=RPI_PICO_W
export BUILD_DIR=$(pwd)
export MPY_DIR=$BUILD_DIR/micropython
export ULAB_DIR=$BUILD_DIR/../code

if [ ! -d $ULAB_DIR ]; then
    printf "Cloning ulab\n"
    ULAB_DIR=$BUILD_DIR/ulab/code
    git clone https://github.com/v923z/micropython-ulab.git ulab
fi

if [ ! -d $MPY_DIR ]; then
    printf "Cloning MicroPython\n"
    git clone https://github.com/micropython/micropython.git micropython
fi

cd $MPY_DIR
git submodule update --init
cd ./mpy-cross && make # build cross-compiler (required)

cd $MPY_DIR/ports/rp2
make BOARD=$BOARD clean
make USER_C_MODULES=$ULAB_DIR/micropython.cmake BOARD=$BOARD
