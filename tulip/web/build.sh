#!/bin/bash
# build.sh
# Just builds web version locally
set -e

if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  SED_INPLACE=(-i '')
fi

# In CI the checkout already provides submodules (and Linux needs no mpy-cross
# patch); only bootstrap them for local builds.
if [ -z "${CI:-}" ]; then
  source ../shared/grab_submodules.sh
fi

timestamp=$(date +%Y%m%d%H%M%S)

# AMY for web (emscripten). Shared by both web apps — ../webdev.py builds it
# once and sets SKIP_AMY_WEB=1 so this run reuses amy/build/ instead of
# recompiling the identical WASM a second time.
if [ -z "${SKIP_AMY_WEB:-}" ]; then
  cd ../../amy
  make web
  #make docs/amy-audioin.js
  cd ../tulip/web
fi
make

# Now modify the static html and copy everything to a stage area 
rm -rf stage
mkdir stage
mkdir stage/run

cp static/* stage/run

# Assemble the full tulip.computer site root. The Tulip-specific landing + its
# webfonts live in site/; the marketing assets (img/css/fonts/js) are shared with
# AMYboard Web + the docs, so they live in repo-root assets/. The web app itself
# goes under stage/run (below).
cp site/index.html stage/
cp -Rf site/webfonts stage/
cp -Rf ../../assets/css ../../assets/fonts ../../assets/img ../../assets/js stage/

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

