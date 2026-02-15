#!/bin/bash
# build.sh
# Just builds web version locally
set -e

if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  SED_INPLACE=(-i '')
fi

source ../shared/grab_submodules.sh

timestamp=$(date +%Y%m%d%H%M%S)

cd ../../amy
make web
#make docs/amy-audioin.js
cd ../tulip/web
make

# Now modify the static html and copy everything to a stage area 
rm -rf stage
mkdir stage
mkdir stage/run

cp static/* stage/run
cp -Rf ../../www/img stage
cp -Rf ../../www/webfonts stage
cp -Rf ../../www/css stage

cp ../../amy/build/amy.js stage/run/amy-$timestamp.js
cp ../../amy/build/amy.wasm stage/run/amy-$timestamp.wasm
cp ../../amy/docs/amy.aw.js stage/run/amy-$timestamp.aw.js

cp build-standard/tulip/obj/micropython.wasm stage/run/tulipcc-$timestamp.wasm
cp build-standard/tulip/obj/micropython.mjs stage/run/tulipcc-$timestamp.mjs
cp build-standard/tulip/obj/micropython.data stage/run/tulipcc-$timestamp.data

sed "${SED_INPLACE[@]}" -e "s/TULIPCCMJS/tulipcc\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/run/index.html
sed "${SED_INPLACE[@]}" -e "s/amy.js/amy\-${timestamp}.js/g" -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" stage/run/amy-$timestamp.js
#sed "${SED_INPLACE[@]}" -e "s/amy\-audioin.aw.js/amy\-audioin\-${timestamp}.aw.js/g" -e "s/amy\-audioin.wasm/amy\-audioin\-${timestamp}.wasm/g" stage/run/amy-audioin-$timestamp.js
sed "${SED_INPLACE[@]}" -e "s/micropython./tulipcc\-${timestamp}./g" stage/run/tulipcc-$timestamp.mjs

python3 server.py
