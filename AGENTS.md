# Agent Instructions for `tulipcc`

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

If an agent makes changes to `amy`, the agent must follow this exact sequence:

1. Open a PR in the `amy` repository for those `amy` changes.
2. Run `make test` in `amy`.
3. In `tulipcc`, point the `amy` submodule to the AMY PR commit and test `tulipcc`.
4. If both `amy` tests and `tulipcc` tests pass, merge the `amy` PR.
5. Pin the `amy` submodule in `tulipcc` to that merged `amy` commit.
6. Commit the submodule pointer update in `tulipcc`.
7. Merge/push `tulipcc` to `main`.

## Testing `tulipcc`

The easiest ways to test `tulipcc` are:

- `./build.sh` in `tulip/macos`
- `./build.sh` in `tulip/web`
- `./build.sh` in `tulip/amyboardweb`

Note:

- `./build.sh` in `tulip/web` and `tulip/amyboardweb` keep a server running.

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

Release steps:

1. Activate the Espressif virtualenv:
   - `source ~/.espressif/python_env/idf5.4_py3.13_env/bin/activate`
2. Change into the `tulip/` directory.
3. Ensure release tag `v-XXX-2026` exists (where `XXX` is `jan`, `feb`, `mar`, `apr`, etc.).
   - If the tag does not exist, create it first.
4. Run:
   - `./release.sh v-XXX-2026 upload`

Web release steps (run when the user asks to release/deploy web targets):

1. For Tulip Web:
   - `cd tulip/web`
   - `./build.sh`
   - Ensure compile/build succeeded.
   - Kill the `./build.sh` server process.
   - `./deploy.sh`
   - Merge the deploy branch to `main` so GitHub Pages publishes updated `www/run` artifacts.
2. For Amyboard Web:
   - `cd tulip/amyboardweb`
   - `./build.sh`
   - Ensure compile/build succeeded.
   - Kill the `./build.sh` server process.
   - `./deploy.sh`

Notes:

- This command cleans and compiles all Tulip targets.
- Typical runtime is about 5 to 10 minutes.

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
