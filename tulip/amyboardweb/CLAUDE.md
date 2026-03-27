# amyboardweb

## Build and Serve

1. Run `./build.sh` to build everything (compiles micropython/JS/WASM for editor, copies static files and `www/img` into `stage/`).
2. Run `./serve.sh` to start a local dev server on port 8000 serving from `stage/`.

Always rebuild (`./build.sh`) after making changes — the dev server serves from `stage/`, not directly from `static/`.

## Image Sources

All marketing site images live in `www/img/` at the repo root. `build.sh` copies them into `stage/img/`.

## What Not to Touch

The following are separate from the marketing site and should not be modified during marketing site work:
- `editor/` — the online AMYboard editor
- Firmware flasher functionality
- "Send to AMYboard" functionality
- "AMYboard World" functionality
