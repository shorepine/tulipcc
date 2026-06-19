#!/bin/bash
# Deploys the built stage/ to the 'tulip' Vercel project (tulip.computer).
# Run ./build.sh first to (re)generate stage/. Mirrors amyboardweb/deploy.sh.

set -euo pipefail

VERCEL_SCOPE="bwhitmans-projects"
VERCEL_PROJECT="tulip"
STAGE_DIR="stage"

current_project=""
project_json="$STAGE_DIR/.vercel/project.json"
if [ -f "$project_json" ]; then
  current_project=$(python3 - <<PY
import json
try:
    with open("$project_json", "r", encoding="utf-8") as fh:
        data = json.load(fh)
    print(data.get("projectName", ""))
except Exception:
    print("")
PY
  )
fi

if [ "$current_project" != "$VERCEL_PROJECT" ]; then
  vercel link --yes --scope "$VERCEL_SCOPE" --project "$VERCEL_PROJECT" --cwd "$STAGE_DIR"
fi

vercel deploy "$STAGE_DIR" --prod --yes --scope "$VERCEL_SCOPE"
