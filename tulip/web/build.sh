#!/bin/bash
# build.sh
# Just builds web version locally
set -e

source ../shared/grab_submodules.sh

cd ../../amy
make docs/amy-audioin.js
cd ../tulip/web
cp ../../amy/docs/amy-audioin.js ../../www/run/
cp ../../amy/docs/amy-audioin.wasm ../../www/run/
cp ../../amy/docs/amy-audioin.aw.js ../../www/run/

make
cp build-standard/tulip/obj/micropython.wasm ../../www/run/
cp build-standard/tulip/obj/micropython.mjs ../../www/run/
cp build-standard/tulip/obj/micropython.data ../../www/run/

