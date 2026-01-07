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

cp static/* stage/
cp -Rf ../../www/img stage
cp -Rf ../../www/fonts stage
cp -Rf ../../www/css stage
cp -Rf ../../www/js stage

cp ../../amy/docs/amy.js stage/amy-$timestamp.js
cp ../../amy/docs/amy.wasm stage/amy-$timestamp.wasm
cp ../../amy/docs/amy.aw.js stage/amy-$timestamp.aw.js

cp build-standard/tulip/obj/micropython.wasm stage/amyboard-$timestamp.wasm
cp build-standard/tulip/obj/micropython.mjs stage/amyboard-$timestamp.mjs
cp build-standard/tulip/obj/micropython.data stage/amyboard-$timestamp.data

sed -i '' -e "s/AMYBOARDMJS/amyboard\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/your.html
sed -i '' -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" -e "s/amy.js/amy\-${timestamp}.js/g" stage/amy-$timestamp.js
sed -i '' -e "s/micropython./amyboard\-${timestamp}./g" stage/amyboard-$timestamp.mjs

python3 server.py
