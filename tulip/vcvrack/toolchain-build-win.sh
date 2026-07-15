#!/bin/bash
# Cross-build the Windows (MinGW) MP archive + console test harnesses inside
# the rack-plugin-toolchain image. W1 of the Windows port: no Rack plugin yet,
# just the shared MicroPython+AMY kernel, validated by running the .exes on a
# Windows machine. Run from the repo root ON THE HOST:
#   docker run --rm --user root -v $PWD:/work -w /work/tulip/vcvrack \
#     rack-plugin-toolchain:19 bash /work/tulip/vcvrack/toolchain-build-win.sh
set -ex
TC=/home/build/rack-plugin-toolchain
export PATH="$TC/local/x86_64-w64-mingw32/bin:$PATH"

# mpy-cross runs on the BUILD host — native compiler.
make -C /work/micropython/mpy-cross BUILD=build-native -j8

export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-g++
export STRIP=x86_64-w64-mingw32-strip
export OBJCOPY=x86_64-w64-mingw32-objcopy
export MICROPY_MPYCROSS=/work/micropython/mpy-cross/build-native/mpy-cross
export MICROPY_MPYCROSS_DEPENDENCY=$MICROPY_MPYCROSS

make -f Makefile.mp -j8 BUILD=build-win/tulip/obj

# Console harnesses (static mingw runtime so the exes run bare on Windows;
# winpthread provides the pthreads mp_embed and the harnesses use).
AR_LIB=build-win/tulip/obj/libamyboardmp.a
CFL="-I../../amy/src -I../shared -DAMY_NO_MINIAUDIO -DGAMMA9001 -D__USE_MINGW_ANSI_STDIO=1 -O2"
LDL="-static -lm -lpthread -lws2_32 -lbcrypt"
$CC $CFL -o test/board_smoke.exe test/board_smoke.c src/amy_stubs.c src/vcv_midi.c $AR_LIB $LDL
$CC $CFL -o test/sysex_smoke.exe test/sysex_smoke.c src/amy_stubs.c $AR_LIB $LDL
ls -la test/*.exe

# W2: the Rack plugin itself. vcv_midi.c is the stub backend on Windows for
# now (no OS virtual MIDI; the editor bridge will use Rack midi::Output +
# loopMIDI). plugin.mk's clean wipes build/ from other platforms' runs.
export RACK_DIR="$TC/Rack-SDK-win-x64"
make clean
make -j8 dist MPLIB=build-win/tulip/obj/libamyboardmp.a \
  EXTRA_LDFLAGS="-lws2_32 -lbcrypt -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic"
mkdir -p dist-win && cp dist/*.vcvplugin dist-win/
# leave the shared build/ and dist/ dirs clean for native (mac) builds —
# mixed-platform objects in build/ have broken the next native link 3x now
make clean
ls -la dist-win/*.vcvplugin
