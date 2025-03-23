#!/bin/bash
# build.sh
# Just builds web version locally
set -e

source ../shared/grab_submodules.sh

timestamp=$(date +%Y%m%d%H%M%S)

cd ../../amy
make docs/amy.js
make docs/amy-audioin.js
cd ../tulip/amyboardweb
make

# Now modify the static html and copy everything to a stage area 
rm -rf stage
mkdir stage
mkdir stage/run

cp static/* stage/run
cp -Rf ../../www/img stage
cp -Rf ../../www/webfonts stage
cp -Rf ../../www/css stage

cp ../../amy/docs/amy.js stage/run/amy-$timestamp.js
cp ../../amy/docs/amy.wasm stage/run/amy-$timestamp.wasm
cp ../../amy/docs/amy.aw.js stage/run/amy-$timestamp.aw.js

cp ../../amy/docs/amy-audioin.js stage/run/amy-audioin-$timestamp.js
cp ../../amy/docs/amy-audioin.wasm stage/run/amy-audioin-$timestamp.wasm
cp ../../amy/docs/amy-audioin.aw.js stage/run/amy-audioin-$timestamp.aw.js

cp build-standard/tulip/obj/micropython.wasm stage/run/amyboard-$timestamp.wasm
cp build-standard/tulip/obj/micropython.mjs stage/run/amyboard-$timestamp.mjs
cp build-standard/tulip/obj/micropython.data stage/run/amyboard-$timestamp.data

sed -i '' -e "s/AMYBOARDMJS/amyboard\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/run/index.html
sed -i '' -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" stage/run/amy-$timestamp.js
sed -i '' -e "s/amy\-audioin.aw.js/amy\-audioin\-${timestamp}.aw.js/g" -e "s/amy\-audioin.wasm/amy\-audioin\-${timestamp}.wasm/g" stage/run/amy-audioin-$timestamp.js
sed -i '' -e "s/micropython./amyboard\-${timestamp}./g" stage/run/amyboard-$timestamp.mjs

python3 server.py
