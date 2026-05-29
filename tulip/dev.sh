#!/bin/bash
# dev.sh — build + publish AMYboard DEV builds (firmware + web).
#
# This is the DEV counterpart to the release flow:
#   - release.sh v-XXX-2026 upload     # promotes firmware to the real release
#   - amyboardweb/deploy.sh            # promotes web to amyboard.com
#
# dev.sh publishes test builds instead:
#   - firmware -> a rolling GitHub PRERELEASE tagged 'dev'
#                 (assets at .../releases/download/dev/...; never becomes 'latest')
#   - web      -> the 'amyboard-dev' Vercel project (https://amyboard-dev.vercel.app)
#
# The flasher picks its channel from the site it's served on: amyboard-dev.*
# only ever flashes the 'dev' firmware, amyboard.com only the latest release.
#
# Run from the tulip/ directory:
#   ./dev.sh            # firmware + web
#   ./dev.sh firmware   # firmware only
#   ./dev.sh web        # web only
set -e

die() { echo "$*" 1>&2 ; exit 1; }

WHAT="${1:-all}"

DEV_TAG="dev"
GH_REPO="shorepine/tulipcc"
VERCEL_SCOPE="bwhitmans-projects"
VERCEL_DEV_PROJECT="amyboard-dev"

[ -d amyboard ] && [ -d amyboardweb ] || die "Run ./dev.sh from the tulip/ directory."

ensure_idf() {
    if command -v idf.py >/dev/null 2>&1; then return; fi
    [ -f "$HOME/.espressif/python_env/idf5.4_py3.13_env/bin/activate" ] && \
        source "$HOME/.espressif/python_env/idf5.4_py3.13_env/bin/activate"
    [ -f "$HOME/esp/esp-idf-v5.4.1/export.sh" ] && \
        source "$HOME/esp/esp-idf-v5.4.1/export.sh"
    command -v idf.py >/dev/null 2>&1 || \
        die "idf.py not found — activate the ESP-IDF env first (see CLAUDE.md)."
}

do_firmware() {
    echo "[dev] Building AMYboard firmware …"
    ensure_idf
    cd amyboard
    rm -rf build
    idf.py -DMICROPY_BOARD=AMYBOARD build
    cd ..
    python fs_create.py amyboard

    echo "[dev] Publishing firmware to GitHub '$DEV_TAG' prerelease …"
    if ! gh release view "$DEV_TAG" --repo "$GH_REPO" >/dev/null 2>&1; then
        gh release create "$DEV_TAG" --repo "$GH_REPO" --prerelease \
            --title "AMYboard dev (rolling)" \
            --notes "Rolling AMYboard development build. Flash from https://amyboard-dev.vercel.app — this is NOT a stable release."
    fi
    gh release upload --clobber --repo "$GH_REPO" "$DEV_TAG" \
        amyboard/dist/amyboard-firmware-AMYBOARD.bin \
        amyboard/dist/amyboard-full-AMYBOARD.bin \
        amyboard/dist/amyboard-sys.bin \
        amyboard/dist/amyboard-version.json
    echo "[dev] Firmware uploaded to '$DEV_TAG' prerelease."
}

do_web() {
    echo "[dev] Building amyboardweb stage/ …"
    cd amyboardweb
    python3 dev.py --build-only

    # Link stage/ to the dev Vercel project (creates it on first run).
    project_json="stage/.vercel/project.json"
    current_project=""
    if [ -f "$project_json" ]; then
        current_project=$(python3 - "$project_json" <<'PY'
import json, sys
try:
    print(json.load(open(sys.argv[1])).get("projectName", ""))
except Exception:
    print("")
PY
)
    fi
    if [ "$current_project" != "$VERCEL_DEV_PROJECT" ]; then
        vercel link --yes --scope "$VERCEL_SCOPE" --project "$VERCEL_DEV_PROJECT" --cwd stage
    fi

    echo "[dev] Deploying web to '$VERCEL_DEV_PROJECT' …"
    vercel deploy stage --prod --yes --scope "$VERCEL_SCOPE"
    cd ..
    echo "[dev] Web deployed. Test it at https://amyboard-dev.vercel.app"
}

case "$WHAT" in
    all)      do_firmware; do_web ;;
    firmware) do_firmware ;;
    web)      do_web ;;
    *)        die "usage: ./dev.sh [all|firmware|web]" ;;
esac

echo "[dev] Done. Promote to release with: ./release.sh v-XXX-2026 upload  &&  (cd amyboardweb && ./deploy.sh)"
