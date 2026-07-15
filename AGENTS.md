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

### Worktrees start with empty submodules (read this BEFORE searching `amy/`)

Git worktrees do **not** inherit the main checkout's submodules. In a fresh worktree
(e.g. anything under `.claude/worktrees/*`) the submodules are uninitialized:
`git submodule status` lists `amy` and `micropython` with a leading `-`, and
`amy/src/`, `micropython/`, etc. are empty. **This is expected** — do not burn a turn
"discovering" it by grepping `amy/`, finding nothing, and then reasoning your way to the
fix. When you need amy (or micropython) content, go straight to the right resolution:

- **Building anything (`amyboard`, `tulip/web`, `tulip/amyboardweb`), or you need the
  worktree's exact pinned commit:** bootstrap *in the worktree* —
  `cd tulip/shared && ./grab_submodules.sh`. A fresh worktree has no `.submodules_ok`, so
  it runs a full init. (Web/WASM builds require this first — see "Testing `tulipcc`".)
- **Read-only source inspection only** (e.g. "where is `yield_synth_commands` defined?"):
  the main checkout at `/Users/bwhitman/outside/tulipcc/amy` already has `amy/` checked
  out — just read it there. Caveat: it may be at a **different commit** than this
  worktree's pin. Verify with `git ls-tree HEAD amy` in the worktree and compare the SHA;
  if exact-pin fidelity matters, bootstrap instead. **Read-only only — never edit the
  main checkout** (see "Worktree File Editing").

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
- `python3 webdev.py` in `tulip/` — builds and serves **both** web apps at once: Tulip Web on `:8001/run/` and AMYboard Web on `:8000/editor/`, sharing a single AMY build.

**Web builds require submodules first.** `webdev.py` (and the per-app `tulip/web/build.sh` and `tulip/amyboardweb/dev.py` it wraps) compile the `amy` and `micropython` submodules to WASM, so the submodules must be bootstrapped before building — run `tulip/shared/grab_submodules.sh` (see Submodule Setup). If `amy/` is empty, the `make web` step fails with `make: *** No rule to make target 'web'`.

`webdev.py` builds AMY once, builds each app's `stage/`, serves each at its own document root (so absolute `/img`, `/editor/`, `/run/` paths match production), and rebuilds an app when its `static/`, `site/` or the shared `assets/` change. To work on a single app instead: `python3 dev.py` in `tulip/amyboardweb` (AMYboard Web, live file-watching — see its `CLAUDE.md`), or `./build.sh` in `tulip/web` to build Tulip Web's `stage/` (what CI runs).

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

Tulip firmware is **released continuously** by `.github/workflows/tulip-release.yml`
(push to `main` on firmware-affecting paths; also `workflow_dispatch`): it builds
TULIP4_R11 and publishes to the rolling **`tulip`** GitHub release, which is kept
marked **Latest** — it IS `releases/latest`. That matters because firmware from the
old monthly releases (≤ `v-jun-2026`) OTAs from `releases/latest`; current firmware
reads `releases/tags/tulip` directly. The same workflow also builds **Tulip Desktop
for macOS** (universal) and uploads `Tulip_Desktop.zip` to the rolling release —
signing/notarization requires the `MACOS_SIGNING_CERT_P12`/`MACOS_SIGNING_CERT_PASSWORD`
and `MACOS_NOTARY_API_KEY`/`MACOS_NOTARY_API_KEY_ID`/`MACOS_NOTARY_API_ISSUER_ID` repo
secrets; without them the zip is built as a CI artifact but not published.

The same workflow also builds **Tulip Desktop for Windows** (x64) in a
`desktop-windows` job: MinGW cross-build inside the prebuilt public
`ghcr.io/shorepine/rack-plugin-toolchain:19` docker image (pushed once from a
local `make docker-build` of VCVRack/rack-plugin-toolchain — never rebuild it
in CI, that takes ~1 hr/platform), publishing `Tulip_Desktop_Windows.zip`
(unsigned `tulip.exe` + `SDL2.dll`) to the rolling `tulip` release.

Both rolling releases also carry a **date-coded copy of the full flash image**
(`tulip-full-TULIP4_R11-YYYYMMDD.bin`, `amyboard-full-AMYBOARD-YYYYMMDD.bin`) for the
manufacturing partner; the stable-named bins must never be renamed (the web flasher,
docs links, and `upgrade()` fetch them by exact name). Only the newest date code is
kept on the release.

### Monthly release.sh (legacy)

The monthly `release.sh` flow below is legacy — only perform it when the user
explicitly asks for it. **If a monthly release is ever cut again, create it with
`--latest=false`**: it must not take the Latest mark back from the rolling `tulip`
release, or legacy devices and the releases page would point at a stale build again
(this is exactly why `v-jun-2026` was retired).

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

Web release steps:

Both web apps are released automatically on push to `main` (no gh-pages, no manual
deploy step) and get per-PR previews:

1. Tulip Web — `tulip-web-release.yml` builds `stage/` and deploys it `--prod` to the
   `tulip` Vercel project (tulip.computer); PRs get a `tulip-pr-<N>.vercel.app` preview
   via `tulip-pr-preview.yml`.
2. AMYboard Web — `amyboard-release.yml` deploys to the `amyboard` Vercel project
   (amyboard.com); PRs get an `amyboard-pr-<N>.vercel.app` preview.

Shared site assets live in repo-root `assets/`. For local development of both apps at
once, run `python3 webdev.py` in `tulip/`.

Notes:

- This command cleans and compiles all Tulip targets.
- Typical runtime is about 5 to 10 minutes.

## AMYboard Release (auto on push to main)

AMYboard is **released continuously**: every push to `main` builds the firmware and
web editor and publishes them as the real release. Now that each PR gets its own
preview site + firmware (`amyboard-pr-<N>.vercel.app`) and HW CI before merge, `main`
is always treated as releasable. **There is no longer a separate `dev` channel** — PR
previews replaced it.

The workflow is `.github/workflows/amyboard-release.yml` (push to `main` on
AMYboard-affecting paths; also `workflow_dispatch`):

1. **firmware** → built and uploaded to the rolling **`amyboard`** GitHub release. That
   release is kept **non-latest** — the rolling `tulip` release owns the Latest mark
   (`releases/latest`). amyboard.com's flasher reads `releases/download/amyboard`, and
   on-device `amyboard.upgrade()` reads `releases/tags/amyboard` (see
   `get_latest_release()` in `tulip/shared/py/tulip.py`). The firmware + sys bins are
   also **mirrored onto the rolling `tulip` release** so legacy AMYboards (monthly
   firmware ≤ `v-jun-2026`, which OTA from `releases/latest`) can still upgrade.
2. **web** → `amyboardweb/stage/` is built and deployed `--prod` to the **`amyboard`**
   Vercel project (amyboard.com). Requires repo secret `VERCEL_TOKEN`
   (scope `bwhitmans-projects`), same as the PR-preview workflow.
3. **vcv-lin-win / vcv-mac** → the AMYboard VCV Rack plugin
   (`AMYboard-<version>-{lin-x64,win-x64,mac-arm64}.vcvplugin`) uploaded to the
   rolling **`amyboard`** release for sideloading (drop into Rack's
   `plugins-<os>-<cpu>/` folder). lin/win cross-build inside the public
   `ghcr.io/shorepine/rack-plugin-toolchain:19` image; mac-arm64 builds
   natively on the macos runner against the official Rack SDK (mac-x64 is a
   known gap — `Makefile.mp` has no cross `-arch` support yet). The **official
   VCV Library is a separate, manual flow**: bump `version` in
   `tulip/vcvrack/plugin.json`, push to main, then comment the new version +
   commit hash on our plugin's issue thread in `github.com/VCVRack/library`;
   VCV's build farm builds from source (~1–2 week turnaround). Do not try to
   automate that from CI.

Tulip firmware is **not** built here — it has its own continuous release,
`tulip-release.yml` (see "Tulip Release"). `releases/latest` is the rolling `tulip`
release; the monthly `v-XXX-2026` releases are retired (the misleading
"Latest"-badged `v-jun-2026` was demoted in July 2026).

### How the flasher chooses a channel

`amyboardweb/static/esptool/js/script.js` resolves the channel at runtime: any host
containing `amyboard-pr` → **pr** (flashes that PR's bundled firmware), everything else
(amyboard.com, localhost) → **release** (the rolling `amyboard` GitHub release). A
`?channel=pr|release` query param overrides this for testing.

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
