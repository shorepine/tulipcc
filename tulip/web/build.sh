#!/bin/bash
# build.sh
# Just builds web version locally
set -e

source ../shared/grab_submodules.sh

cd ../../amy
make docs/amy.js
cd ../tulip/web
cp ../../amy/docs/amy.js dist/
cp ../../amy/docs/amy.wasm dist/
cp ../../amy/docs/amy.aw.js dist/

make
cp build-standard/tulip/obj/micropython.wasm dist/
cp build-standard/tulip/obj/micropython.mjs dist/

