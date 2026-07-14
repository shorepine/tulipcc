#!/bin/bash
# Cross-build the lin-x64 plugin inside the rack-plugin-toolchain image.
# Run from the repo root ON THE HOST:
#   docker run --rm -v $PWD:/work -w /work/tulip/vcvrack rack-plugin-toolchain:19 \
#     bash /work/tulip/vcvrack/toolchain-build-lin.sh
set -ex
TC=/home/build/rack-plugin-toolchain

# ALSA headers for vcv_midi.c (arch-independent; we do not link libasound —
# symbols resolve from the Rack process at load time).
apt-get update -qq && apt-get install -y -qq libasound2-dev > /dev/null
mkdir -p /tmp/alsa-inc && cp -r /usr/include/alsa /tmp/alsa-inc/

export PATH="$TC/local/x86_64-ubuntu16.04-linux-gnu/bin:$PATH"
export RACK_DIR="$TC/Rack-SDK-lin-x64"

# mpy-cross runs on the BUILD host (container arch) — native compiler.
make -C /work/micropython/mpy-cross BUILD=build-lin -j8

export CC=x86_64-ubuntu16.04-linux-gnu-gcc
export CXX=x86_64-ubuntu16.04-linux-gnu-g++
export STRIP=x86_64-ubuntu16.04-linux-gnu-strip
export OBJCOPY=x86_64-ubuntu16.04-linux-gnu-objcopy
export MICROPY_MPYCROSS=/work/micropython/mpy-cross/build-lin/mpy-cross
export MICROPY_MPYCROSS_DEPENDENCY=$MICROPY_MPYCROSS

# Stage 1: MicroPython+AMY archive (cross), own build dir.
make -f Makefile.mp -j8 BUILD=build-lin/tulip/obj

# Stage 2: the plugin. Clean removes the mac objects in build/.
make clean
make -j8 dist MPLIB=build-lin/tulip/obj/libamyboardmp.a EXTRA_FLAGS="-I/tmp/alsa-inc"
mkdir -p dist-lin && cp dist/*.vcvplugin dist-lin/
# leave the shared build/ and dist/ dirs clean for native (mac) builds —
# mixed-platform objects in build/ have broken the next native link 3x now
make clean
ls -la dist-lin/*.vcvplugin
