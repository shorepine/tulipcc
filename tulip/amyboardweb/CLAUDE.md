# amyboardweb

## Dev Server (`dev.py`)

`dev.py` is all you need to develop AMYboard Web on its own — to run it alongside Tulip Web in one command, use `python3 tulip/webdev.py` instead (see the top-level `AGENTS.md`). `dev.py` does everything:

1. On startup: compiles AMY (`make web` in `amy/`) and AMYboard MicroPython (`make` in `tulip/amyboardweb/`), then builds `stage/` — copies `static/` and `assets/img/fonts/css/js`, substitutes timestamped WASM/JS filenames into `editor/index.html`.
2. While running: polls `static/` and `assets/` every second and incrementally syncs any changed files to `stage/`. Re-applies timestamp substitutions automatically if `editor/index.html` changes.
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

### Deploying

No manual deploy step: pushing to `main` builds and deploys AMYboard Web to the
`amyboard` Vercel project (amyboard.com) via `.github/workflows/amyboard-release.yml`,
and every PR gets an `amyboard-pr-<N>.vercel.app` preview. Just open a PR.

## Workflow Notes

- `static/` is where you and Claude edit source files.
- `static/*.generated.js` are build products, not sources: `dev.py` copies
  `amy_api` / `patches` / `pcm_presets` from the amy submodule's committed
  `amy/src/*.generated.js` and derives `drum_presets` from amy's gamma9001
  manifest. They are gitignored — to change them, change amy and run
  `make c-api` there.
- `stage/` is the built output — served locally and deployed to Vercel. Never edit files in `stage/` directly.
- All marketing site images live in `assets/img/` at the repo root. `dev.py` copies them into `stage/img/`.
- The WASM/JS timestamp is fixed for the entire dev session — WASM filenames stay stable even as you edit HTML/JS.

## What Not to Touch

The following are separate from the marketing site and should not be modified during marketing site work:

- `editor/` — the online AMYboard editor
- Firmware flasher functionality
- "Send to AMYboard" functionality
- "AMYboard World" functionality
