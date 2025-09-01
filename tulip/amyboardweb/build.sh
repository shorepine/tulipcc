#!/bin/bash
# build.sh
# Just builds web version locally
set -e

source ../shared/grab_submodules.sh

timestamp=$(date +%Y%m%d%H%M%S)

cd ../../amy
make docs/amy.js
cd ../tulip/amyboardweb
make

# Now modify the static html and copy everything to a stage area 
rm -rf stage
mkdir stage
mkdir stage/amy

cp static/* stage/amy
cp -Rf ../../www/img stage
cp -Rf ../../www/webfonts stage
cp -Rf ../../www/css stage

cp ../../amy/docs/amy.js stage/amy/amy-$timestamp.js
cp ../../amy/docs/amy.wasm stage/amy/amy-$timestamp.wasm
cp ../../amy/docs/amy.aw.js stage/amy/amy-$timestamp.aw.js

cp build-standard/tulip/obj/micropython.wasm stage/amy/amyboard-$timestamp.wasm
cp build-standard/tulip/obj/micropython.mjs stage/amy/amyboard-$timestamp.mjs
cp build-standard/tulip/obj/micropython.data stage/amy/amyboard-$timestamp.data

sed -i '' -e "s/AMYBOARDMJS/amyboard\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/amy/index.html
sed -i '' -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" stage/amy/amy-$timestamp.js
sed -i '' -e "s/micropython./amyboard\-${timestamp}./g" stage/amy/amyboard-$timestamp.mjs

python3 server.py
