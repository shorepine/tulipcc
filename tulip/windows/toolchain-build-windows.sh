#!/bin/bash
# Cross-build Tulip Desktop for Windows (tulip.exe) inside the
# rack-plugin-toolchain image (has x86_64-w64-mingw32 gcc 15). Phase A of the
# Windows port: get MicroPython + AMY + the Tulip display/LVGL/ulab modules to
# compile, link, and boot to a REPL. Run from the repo root ON THE HOST:
#
#   docker run --rm --user root \
#     -v $PWD:/work -w /work/tulip/windows \
#     rack-plugin-toolchain:19 bash /work/tulip/windows/toolchain-build-windows.sh
#
# Build output is teed to build-standard/build.log (huge — grep it, don't cat).
set -ex

TC=/home/build/rack-plugin-toolchain
export PATH="$TC/local/x86_64-w64-mingw32/bin:$PATH"

cd /work/tulip/windows

# --- SDL2 (MinGW dev package), cached under _sdl ---------------------------
SDL_VER=2.30.9
SDL_ROOT="/work/tulip/windows/_sdl/SDL2-${SDL_VER}/x86_64-w64-mingw32"
if [ ! -d "$SDL_ROOT" ]; then
    mkdir -p /work/tulip/windows/_sdl
    curl -sL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VER}/SDL2-devel-${SDL_VER}-mingw.tar.gz" \
        | tar xz -C /work/tulip/windows/_sdl
fi
export SDL_INC="-I${SDL_ROOT}/include/SDL2 -I${SDL_ROOT}/include"
export SDL_LIB="-L${SDL_ROOT}/lib -lmingw32 -lSDL2main -lSDL2"

# --- mpy-cross runs on the BUILD host (native compiler) --------------------
make -C /work/micropython/mpy-cross BUILD=build-native -j8

# --- cross toolchain env ---------------------------------------------------
export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
export STRIP=x86_64-w64-mingw32-strip
export OBJCOPY=x86_64-w64-mingw32-objcopy
export MICROPY_MPYCROSS=/work/micropython/mpy-cross/build-native/mpy-cross
export MICROPY_MPYCROSS_DEPENDENCY=$MICROPY_MPYCROSS

# The generated AMY data (drums_bin.c) and patches.h are committed; mark them
# up to date so make doesn't try to regenerate them (needs numpy, not present
# in this image).
touch /work/amy/build/drums_bin.c /work/amy/src/patches.h 2>/dev/null || true

mkdir -p build-standard
make -f Makefile -j8 DEBUG=1 2>&1 | tee build-standard/build.log

EXE=build-standard/tulip/obj/tulip.exe
if [ -f "$EXE" ]; then
    x86_64-w64-mingw32-strip -s -o build-standard/tulip.exe "$EXE" || cp "$EXE" build-standard/tulip.exe
    cp "${SDL_ROOT}/bin/SDL2.dll" build-standard/ || true
    ls -la build-standard/tulip.exe build-standard/SDL2.dll
    echo "BUILD OK: build-standard/tulip.exe"
else
    echo "BUILD FAILED: no tulip.exe (see build-standard/build.log)"
    exit 1
fi
