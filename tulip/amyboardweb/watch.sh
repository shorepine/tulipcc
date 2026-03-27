#!/bin/bash
# watch.sh - Full build once, then watch for static file changes and hot-copy to stage/.
# Runs a dev server on port 8000.
# Usage: ./watch.sh
#   First run: does full build.sh (compiles AMY + MicroPython WASM)
#   Then watches static/, www/img, www/css, www/js, www/fonts for changes
#   On change: re-copies only static assets to stage/ (instant)
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Full build first (compiles everything)
echo "=== Running full build... ==="
./build.sh
echo "=== Full build done ==="

# Kill any existing server on 8000
kill $(lsof -ti :8000) 2>/dev/null || true
sleep 0.5

# Start server in background
cd stage
python3 -m http.server 8000 &
SERVER_PID=$!
cd "$SCRIPT_DIR"

echo "=== Dev server running on http://localhost:8000 ==="
echo "=== Watching for changes... (Ctrl-C to stop) ==="

# Detect the timestamp from the initial build (from the amy-*.js file in stage)
BUILD_TS=$(ls stage/amy-*.wasm 2>/dev/null | head -1 | sed 's/.*amy-\(.*\)\.wasm/\1/')
echo "=== Build timestamp: $BUILD_TS ==="

if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  SED_INPLACE=(-i '')
fi

# Quick re-copy function (no recompilation)
# Copies marketing site files + shared assets, and re-patches editor/index.html
# with the original build timestamp so the WASM/MJS/JS references stay valid.
sync_static() {
  echo "[$(date +%H:%M:%S)] Change detected, syncing..."

  # Copy marketing site HTML (top-level index.html, not editor/)
  cp static/index.html stage/index.html
  # Copy editor/index.html and re-apply timestamp substitutions
  cp static/editor/index.html stage/editor/index.html
  sed "${SED_INPLACE[@]}" \
    -e "s/AMYBOARDMJS/amyboard-${BUILD_TS}.mjs/g" \
    -e "s/AMYJS/amy-${BUILD_TS}.js/g" \
    stage/editor/index.html
  # Copy shared JS (spss.js etc) but not editor/
  for f in static/*.js static/*.css; do
    [ -f "$f" ] && cp "$f" stage/
  done
  # Copy shared assets
  cp -Rf ../../www/img stage/
  cp -Rf ../../www/fonts stage/
  cp -Rf ../../www/css stage/
  cp -Rf ../../www/js stage/

  echo "[$(date +%H:%M:%S)] Done. Reload your browser."
}

cleanup() {
  echo ""
  echo "Stopping server..."
  kill $SERVER_PID 2>/dev/null || true
  kill $WATCH_PID 2>/dev/null || true
  exit 0
}
trap cleanup INT TERM

# Use fswatch if available (better), otherwise fall back to polling
if command -v fswatch &>/dev/null; then
  fswatch -o -r -l 1 \
    --exclude='stage/' --exclude='\.git/' --exclude='build-standard/' \
    static/ ../../www/img/ ../../www/css/ ../../www/js/ ../../www/fonts/ \
  | while read -r _; do
    sync_static
  done &
  WATCH_PID=$!
else
  echo "(fswatch not found, using polling every 2s)"
  while true; do
    sleep 2
    # Check if any source file is newer than a sentinel
    SENTINEL="stage/.last_sync"
    touch -r stage/index.html "$SENTINEL" 2>/dev/null || touch "$SENTINEL"
    CHANGED=$(find static/ ../../www/img/ ../../www/css/ ../../www/js/ ../../www/fonts/ \
      -newer "$SENTINEL" -type f 2>/dev/null | head -1)
    if [ -n "$CHANGED" ]; then
      sync_static
      touch "$SENTINEL"
    fi
  done &
  WATCH_PID=$!
fi

wait $SERVER_PID
