#!/bin/bash
# build.sh
# Just builds web version locally
set -e

build_once() {
  source ../shared/grab_submodules.sh

  timestamp=$(date +%Y%m%d%H%M%S)

  python3 scripts/gen_amy_event_layout.py

  cd ../../amy
  make docs/amy.js
  cd ../tulip/amyboardweb
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

  

  cp ../../amy/docs/amy.js stage/amy-$timestamp.js
  cp ../../amy/docs/amy.wasm stage/amy-$timestamp.wasm
  cp ../../amy/docs/amy.aw.js stage/amy-$timestamp.aw.js

  cp build-standard/tulip/obj/micropython.wasm stage/amyboard-$timestamp.wasm
  cp build-standard/tulip/obj/micropython.mjs stage/amyboard-$timestamp.mjs
  cp build-standard/tulip/obj/micropython.data stage/amyboard-$timestamp.data

  sed -i '' -e "s/AMYBOARDMJS/amyboard\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/editor.html
  sed -i '' -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" -e "s/amy.js/amy\-${timestamp}.js/g" stage/amy-$timestamp.js
  sed -i '' -e "s/micropython./amyboard\-${timestamp}./g" stage/amyboard-$timestamp.mjs
}

snapshot_static() {
  python3 - <<'PY'
import hashlib
import os

root = "static"
ignore = {"amy_event_layout.js", "patches.js"}
h = hashlib.sha1()
for base, dirs, files in os.walk(root):
    dirs.sort()
    files.sort()
    for name in files:
        if name in ignore:
            continue
        path = os.path.join(base, name)
        try:
            st = os.stat(path)
        except FileNotFoundError:
            continue
        h.update(path.encode())
        h.update(str(st.st_mtime_ns).encode())
print(h.hexdigest())
PY
}

if [ "$1" = "--once" ]; then
  build_once
  python3 server.py
  exit 0
fi

build_once
python3 server.py &
server_pid=$!
trap 'kill $server_pid' EXIT

last_snapshot=$(snapshot_static)
while true; do
  sleep 1
  next_snapshot=$(snapshot_static)
  if [ "$next_snapshot" != "$last_snapshot" ]; then
    last_snapshot=$next_snapshot
    kill $server_pid
    build_once
    python3 server.py &
    server_pid=$!
  fi
done
