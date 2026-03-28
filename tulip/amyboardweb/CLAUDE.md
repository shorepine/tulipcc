# amyboardweb

## Dev Server (`dev.py`)

`dev.py` is the only script needed for local development. It does everything:

1. On startup: does a full build — wipes and recreates `stage/`, copies `static/` and `www/img/fonts/css/js`, substitutes timestamped WASM/JS filenames into `editor/index.html`.
2. While running: polls `static/` and `www/` every second and incrementally syncs any changed files to `stage/`. Re-applies timestamp substitutions automatically if `editor/index.html` changes.
3. Serves `stage/` on port 8000 with correct CORS headers for WASM.

### Starting the dev server

Run **once** from `tulip/amyboardweb/`:

```
PYTHONUNBUFFERED=1 nohup python3 -u dev.py > /tmp/amyboard-dev.log 2>&1 &
```

**Do not kill this process while developing.** Only stop it if the user asks or you are done for the session.

Check the log with: `cat /tmp/amyboard-dev.log`

### Editing files

- Edit files in `static/` (HTML, JS, CSS for the marketing site and editor).
- `dev.py` detects changes within 1 second and syncs them to `stage/` automatically.
- No manual rebuild or server restart needed after edits.

### Deploying to Vercel

`dev.py` keeps `stage/` up to date, so deploying is always safe:

```
cd tulip/amyboardweb && ./deploy.sh
```

Vercel deploys from `stage/` which always reflects the latest build.

## Workflow Notes

- `static/` is where you and Claude edit source files.
- `stage/` is the built output — served locally and deployed to Vercel. Never edit files in `stage/` directly.
- All marketing site images live in `www/img/` at the repo root. `dev.py` copies them into `stage/img/`.
- The WASM/JS timestamp is fixed for the entire dev session — WASM filenames stay stable even as you edit HTML/JS.

## What Not to Touch

The following are separate from the marketing site and should not be modified during marketing site work:

- `editor/` — the online AMYboard editor
- Firmware flasher functionality
- "Send to AMYboard" functionality
- "AMYboard World" functionality
