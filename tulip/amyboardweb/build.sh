#!/bin/bash
# build.sh
# Just builds web version locally
set -e

if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  SED_INPLACE=(-i '')
fi

build_once() {
  source ../shared/grab_submodules.sh

  timestamp=$(date +%Y%m%d%H%M%S)

  python3 scripts/gen_amy_event_layout.py
  cd ../../amy
  make web
  cd ../tulip/amyboardweb
  cp ../../amy/build/amy_api.generated.js static/amy_api.generated.js
  make

  # Now modify the static html and copy everything to a stage area 
  rm -rf stage
  mkdir stage

  cp -R static/. stage/
  cp -Rf ../../www/img stage
  cp -Rf ../../www/fonts stage
  cp -Rf ../../www/css stage
  cp -Rf ../../www/js stage
  cp -Rf api stage
  cp vercel.json stage/vercel.json

  

  cp ../../amy/build/amy.js stage/amy-$timestamp.js
  cp ../../amy/build/amy.wasm stage/amy-$timestamp.wasm
  cp ../../amy/docs/amy.aw.js stage/amy-$timestamp.aw.js

  cp build-standard/tulip/obj/micropython.wasm stage/amyboard-$timestamp.wasm
  cp build-standard/tulip/obj/micropython.mjs stage/amyboard-$timestamp.mjs
  cp build-standard/tulip/obj/micropython.data stage/amyboard-$timestamp.data

  sed "${SED_INPLACE[@]}" -e "s/AMYBOARDMJS/amyboard\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/editor/index.html
  sed "${SED_INPLACE[@]}" -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" -e "s/amy.js/amy\-${timestamp}.js/g" stage/amy-$timestamp.js
  sed "${SED_INPLACE[@]}" -e "s/micropython./amyboard\-${timestamp}./g" stage/amyboard-$timestamp.mjs
}

build_once
python3 server.py
