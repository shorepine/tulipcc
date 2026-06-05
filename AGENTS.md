# Agent Instructions for `tulipcc`

## Worktree File Editing

When working in a worktree, **always use relative paths** or the full worktree path for file edits. Never use `/Users/bwhitman/outside/tulipcc/tulip/...` — that's the main repo, not the worktree. The worktree is at the current working directory. Using main repo paths will silently edit the wrong copy and the changes won't appear in your build or in VS Code. This applies to all files including submodules like `amy/`.

## Submodule Setup

To initialize and update submodules for this repo, use:

- `tulip/shared/grab_submodules.sh`

Do not use ad-hoc/manual recursive submodule commands when this script is available.

Important:

- Run this script from `tulip/shared` (for example: `cd tulip/shared && ./grab_submodules.sh`). The script uses relative paths and will fail or partially run from other directories.
- The script creates `.submodules_ok` at repo root after first successful setup. If submodule pointers change and you need to force a resync, remove `.submodules_ok` and rerun the script from `tulip/shared`.
- `micropython` is expected to appear dirty after running submodule setup. `grab_submodules.sh` intentionally patches `micropython/mpy-cross/Makefile` (`-Wno-gnu-folding-constant`) so it compiles on newer macOS clang versions.

## Micropython Submodule Status

The `micropython` submodule is expected to appear dirty after bootstrap/build steps (for example, local warning-flag patching done by `tulip/shared/grab_submodules.sh`).

Treat this as expected local state and ignore `micropython` dirtiness in `git status` unless you are intentionally working on `micropython`.

## AMY Submodule Policy

We constantly update the `amy` submodule to work with `tulipcc`.

Before pushing anything in `tulipcc` to `main`, the `amy` submodule must be pinned to the latest known working git version.

### Checking if AMY is up to date

When asked to update to the latest commits, always fetch the AMY remote and compare against its remote main — do not assume the pinned commit is current:

1. `cd amy && git fetch amy-origin main`
2. Compare `git rev-parse amy-origin/main` vs `git rev-parse HEAD`
3. If they differ, checkout the latest and update the pin in `tulipcc`

### Making changes to AMY

If an agent makes changes to `amy`, the agent must follow this exact sequence:

1. Open a PR in the `amy` repository for those `amy` changes.
2. Run `make test` in `amy`.
3. In `tulipcc`, point the `amy` submodule to the AMY PR commit and test `tulipcc`.
4. If both `amy` tests and `tulipcc` tests pass, merge the `amy` PR.
5. Pin the `amy` submodule in `tulipcc` to that merged `amy` commit.
6. Commit the submodule pointer update in `tulipcc`.
7. Merge/push `tulipcc` to `main`.

### Refresh the generator's vendored AMY docs when the pin changes

The AMYboard sketch generator (`tulip/server/amyboardworld_db_api.py`) reads a
**committed snapshot** of `amy/docs/*.md` from `tulip/server/refdocs/amy/`, because
the `amy` submodule is not checked out in the Railway deploy. Whenever you bump the
`amy` pin (either flow above), refresh that snapshot so the generator's docs match the
pinned commit:

1. `python3 tulip/server/sync_amy_docs.py` (sources from the pinned commit — the local
   submodule if checked out, else GitHub raw at the pinned SHA).
2. Commit the updated `tulip/server/refdocs/amy/`.

## Testing `tulipcc`

The easiest ways to test `tulipcc` are:

- `./build.sh` in `tulip/macos`
- `./build.sh` in `tulip/web`
- `python3 dev.py` in `tulip/amyboardweb`

**Web builds require submodules first.** `tulip/web` and `tulip/amyboardweb/dev.py` compile the `amy` and `micropython` submodules to WASM, so the submodules must be bootstrapped before building — run `tulip/shared/grab_submodules.sh` (see Submodule Setup). If `amy/` is empty, `dev.py`'s `make web` step fails with `make: *** No rule to make target 'web'`.

The `build.sh` scripts exit after building. To start a local dev server on port 8000 after building `tulip/web`, run `./serve.sh` there. For `tulip/amyboardweb`, `dev.py` builds `stage/` on startup, serves it on port 8000, and incrementally re-syncs files as you edit — leave it running during the session. See `tulip/amyboardweb/CLAUDE.md` for the exact invocation.

## ESP-IDF Build Commands (Required)

If the user asks to build `amyboard`, or asks to build `tulip cc` / `tulip esp32s3`, first run:

- `source ~/.espressif/python_env/idf5.4_py3.13_env/bin/activate`
- `source ~/esp/esp-idf-v5.4.1/export.sh`

For `amyboard` build:

1. `cd tulip/amyboard`
2. `idf.py -DMICROPY_BOARD=AMYBOARD build`

For `tulip cc` or `tulip esp32s3` build:

1. `cd tulip/esp32s3`
2. `idf.py -DMICROPY_BOARD=TULIP4_R11 build`

## Tulip Release

Only perform a Tulip release when the user explicitly asks for it.

**Important:** Releases must always be built from the `main` branch (or a worktree reset to `origin/main`). Before starting a release, ensure the working tree is on `main` with all intended changes merged. Never release from an unmerged feature branch.

Release steps:

1. Ensure you are on the `main` branch (or reset to `origin/main`).
2. Activate the Espressif virtualenv:
   - `source ~/.espressif/python_env/idf5.4_py3.13_env/bin/activate`
3. Change into the `tulip/` directory.
4. Ensure release tag `v-XXX-2026` exists (where `XXX` is `jan`, `feb`, `mar`, `apr`, etc.).
   - If the tag does not exist, create it first.
5. Run:
   - `./release.sh v-XXX-2026 upload`

Web release steps (run when the user asks to release/deploy web targets):

1. For Tulip Web:
   - `cd tulip/web`
   - `./build.sh`
   - Ensure compile/build succeeded.
   - `./deploy.sh`
   - Merge the deploy branch to `main` so GitHub Pages publishes updated `www/run` artifacts.
2. For Amyboard Web:
   - `cd tulip/amyboardweb`
   - `python3 dev.py` (builds `stage/` on startup and serves it; leave running)
   - Ensure compile/build succeeded.
   - `./deploy.sh` (deploys from `stage/`)

Notes:

- This command cleans and compiles all Tulip targets.
- Typical runtime is about 5 to 10 minutes.

## AMYboard Dev Channel (`dev.sh`)

AMYboard has two firmware/web channels:

- **release** — the real GitHub release (`releases/latest`), flashed from
  `amyboard.com`. Promoted via `release.sh` (firmware) + `amyboardweb/deploy.sh` (web).
- **dev** — rolling test builds, flashed from `https://amyboard-dev.vercel.app`.
  Published via `tulip/dev.sh`.

### Publishing dev builds

Run from `tulip/` (ESP-IDF env active, see above):

- `./dev.sh` — firmware + web
- `./dev.sh firmware` — firmware only
- `./dev.sh web` — web only

`dev.sh`:

1. Builds AMYboard firmware and uploads `amyboard-{firmware,full,sys}-AMYBOARD.bin`
   to a GitHub **prerelease tagged `dev`** (created on first run, `--clobber`ed after).
   Because it's a prerelease, it never becomes `releases/latest`.
2. Builds `amyboardweb/stage/` via `python3 dev.py --build-only` and deploys it to
   the **`amyboard-dev`** Vercel project (scope `bwhitmans-projects`, `--prod`).

### How the flasher chooses a channel

`amyboardweb/static/esptool/js/script.js` resolves the channel at runtime:
`amyboard.com` → release, any host containing `amyboard-dev` → dev, localhost → dev,
anything else → release. A `?channel=dev|release` query param (on the flasher page or
the parent editor page) overrides this for testing. So the **dev site only ever flashes
dev firmware and amyboard.com only ever flashes the latest release** — the same web
artifact is deployed to both; only the hostname differs.

### Promotion (dev → release)

Once a dev build is tested, promote with the existing scripts:

1. Firmware: `./release.sh v-XXX-2026 upload` (rebuilds from `main`, uploads to the
   version tag, which becomes `releases/latest`).
2. Web: `cd amyboardweb && ./deploy.sh` (deploys `stage/` to the `amyboard` Vercel
   project / amyboard.com).

## World DB API Server (Railway)

The FastAPI server in `tulip/server/amyboardworld_db_api.py` backs all
`/api/amyboardworld/*` and `/api/tulipworld/*` endpoints (sketches, tags,
admin, Tulip World messages). It runs on Railway, not Vercel — a Vercel
deploy of `tulip/amyboardweb/` does NOT touch it.

### Railway configuration

- Project: `zestful-love` (Brian Whitman's Projects workspace)
- Service: `tulipcc`
- Environment: `production`
- Config file: `/railway.json` (repo root, NOT `tulip/server/railway.json`
  — the root one takes precedence and uses `--app-dir tulip/server`)
- Start command: `uvicorn amyboardworld_db_api:app --host 0.0.0.0 --port $PORT --app-dir tulip/server`
- Production URL: `https://tulipcc-production.up.railway.app`
- Dependencies: the **root** `/requirements.txt` is the file Railway/Nixpacks
  actually installs (auto-detected at the repo root). Keep it in sync with
  `tulip/server/requirements.txt` (the local-dev copy). Currently: fastapi,
  uvicorn[standard], python-multipart, requests, anthropic. **Adding a dep to
  only `tulip/server/requirements.txt` will NOT install it on Railway** — it
  must also go in the root `/requirements.txt`.

### Auto-deploy on push to main

Railway watches `main` and auto-deploys on every push. **No manual deploy
step is needed for server changes merged to main.** To verify a deploy
picked up a specific commit, run:

```
railway status --json | python3 -c "import json,sys;d=json.load(sys.stdin); \
  svc=d['environments']['edges'][0]['node']['serviceInstances']['edges'][0]['node']; \
  print(svc['latestDeployment']['meta']['commitHash'][:12], svc['latestDeployment']['meta']['commitMessage'].splitlines()[0])"
```

Or hit the API directly to confirm expected behavior:

```
curl -s "https://tulipcc-production.up.railway.app/api/amyboardworld/files?limit=1"
```

### Manual operations

From the repo root (or any worktree):

- `railway status` — current project/service/environment linkage.
- `railway status --json` — full deploy metadata including latest commit.
- `railway variables` — list env vars (DB path, blob path, admin token).
- `railway logs` — stream recent service logs.
- `railway domain` — show the public URL.
- `railway up` — manual deploy from local working tree (rarely needed).
- `railway redeploy` — redeploy the last successful deployment.

### Environment variables (set on Railway, not in the repo)

- `AMYBOARDWORLD_DB_PATH` — currently `/data/files/amyboardworld.db`
- `AMYBOARDWORLD_FILES_DIR` — currently `/data/files/blobs`
- `AMYBOARDWORLD_ADMIN_TOKEN` — shared admin bearer for moderation endpoints
- (Plus `RAILWAY_*` platform-injected vars)

The DB and blob store live on a Railway-attached persistent volume under
`/data` — do NOT delete or recreate that volume without backing up first.

### Local development

See `tulip/server/amyboardworld_db_api.md` for local-run instructions
(`python3 -m venv .venv && pip install -r requirements.txt && uvicorn amyboardworld_db_api:app --reload --port 8090`).

## Safe Git Merge Procedure (Required)

When a user asks to commit/push/merge PR changes, treat it as a strict ordered workflow.

### Ordering Rules

- Do not run `git push` and PR merge commands in parallel.
- Do not run any dependent git steps in parallel.
- Required order is:
  1. `git add` / `git commit`
  2. `git push`
  3. confirm remote branch tip equals local `HEAD`
  4. merge PR
  5. confirm `origin/main` contains expected commit(s)

### Pre-Merge Verification (Required)

Before merging:
1. Ensure local branch is clean except expected submodule dirtiness.
2. Ensure all intended files are committed (`git diff --name-only HEAD` should be empty for tracked files intended for merge).
3. Ensure remote branch includes latest commit:
   - `git rev-parse HEAD`
   - `git rev-parse origin/<current-branch>`
   - These must match before merge.

### Post-Merge Verification (Required)

After merging:
1. `git fetch origin main`
2. Verify merged commit SHA is reachable from `origin/main`.
3. Verify key changed files match expectations (for example with `git diff origin/main -- <path>`).
4. Report in final response:
   - merged PR number
   - merge commit SHA
   - confirmation that `origin/main` includes the intended changes

### Failure Handling

- If any verification fails, do not claim success.
- Continue until the intended commit(s) are in `origin/main` (or explain the blocking protection explicitly).
