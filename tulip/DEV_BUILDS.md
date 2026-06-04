# Publishing AMYboard dev builds (firmware + web)

A guide for collaborators on pushing **test** builds to the AMYboard **dev**
channel. This does *not* touch the real release (amyboard.com / firmware
`releases/latest`) — promoting a dev build to release is a separate, maintainer-only
step (see the bottom).

## TL;DR

```bash
cd tulip
# activate your ESP-IDF env (for the firmware half) and set your Vercel token (for web):
source <esp-idf>/export.sh
export VERCEL_TOKEN=<token-from-brian>

./dev.sh            # builds + publishes BOTH firmware and web
```

Then test/flash at **https://amyboard-dev.vercel.app**.

> `./dev.sh` does firmware + web in one shell. You can also run the halves
> separately: `./dev.sh firmware` and `./dev.sh web`.

## What the "dev" channel is

There are two channels:

| Channel | Web (flasher) | Firmware source |
|---------|---------------|-----------------|
| **release** | `amyboard.com` | GitHub `releases/latest` |
| **dev** | `amyboard-dev.vercel.app` | GitHub **`dev` prerelease** (rolling, never becomes `latest`) |

`tulip/dev.sh` publishes the dev builds:

- **firmware** → uploaded to a GitHub **prerelease tagged `dev`** on `shorepine/tulipcc`
  (created on first run, `--clobber`ed afterward). Because it's a prerelease it can
  never become `releases/latest`.
- **web** → deployed to the **`amyboard-dev`** Vercel project (`https://amyboard-dev.vercel.app`).

The flasher page picks its channel **from the hostname it's served on**
(`static/esptool/js/script.js`): `amyboard.com` → release, any host containing
`amyboard-dev` → dev, `localhost` → dev, anything else → release. You can force a
channel with a `?channel=dev` (or `?channel=release`) query param on the flasher or
parent editor page. So the dev site only ever flashes dev firmware, and amyboard.com
only ever flashes the latest release — the deployed web artifact is the same; only
the hostname differs.

## Access you'll need (ask Brian for these)

1. **GitHub write (collaborator) access to `shorepine/tulipcc`.** Uploading firmware
   to the `dev` prerelease uses `gh release upload`, which needs push access. A fork
   or read-only access is **not** enough.
2. **A Vercel deploy token (no team needed).** Brian generates a personal **access
   token** (Vercel → Settings → Tokens) on the `bwhitmans-projects` account and shares
   it with you. The token carries Brian's access, so your deploys land in the shared
   `amyboard-dev` project at the canonical URL — you do **not** need to be on a Vercel
   team. (Brian: set an expiration on the token, and revoke it from the same page when
   the collaboration ends.)

## One-time setup

1. **Clone + submodules.** From the repo root:
   ```bash
   git clone git@github.com:shorepine/tulipcc.git
   cd tulipcc/tulip/shared && ./grab_submodules.sh && cd ../..
   ```
   (The `micropython` submodule will show as dirty afterward — that's expected;
   `grab_submodules.sh` patches a compiler flag.)

2. **ESP-IDF v5.4.1** (for firmware). Install per Espressif's docs. Easiest is to let
   the installer use the default `~/esp/esp-idf-v5.4.1` location — but the simplest
   reliable approach is to **activate your own ESP-IDF env** (so `idf.py` is on your
   PATH) before running `./dev.sh firmware`; `dev.sh` will use whatever's already
   active. The hardcoded fallback paths inside `dev.sh` are Brian's and may not match
   your install.

3. **Emscripten** (for web). Install and activate the `emsdk` so `emcc` is on your
   PATH — the web build compiles AMY and MicroPython to WebAssembly.

4. **Python 3 with `numpy`** (for web). The web build runs `make web`, which generates
   C headers via `python3 -m amy.headers` (needs `numpy`). The ESP-IDF venv already has
   numpy, so `./dev.sh` works straight from your IDF shell; if you build web from a
   plain shell instead, `pip3 install numpy`. (`soundfile` is **not** required — it's
   only used for optional PCM regeneration, which dev builds skip.)

5. **CLIs:**
   ```bash
   gh auth login                          # account with write access to shorepine/tulipcc
   gh repo view shorepine/tulipcc         # sanity check it resolves

   npm i -g vercel
   # No `vercel login` needed — use Brian's token instead:
   export VERCEL_TOKEN=<token-from-brian>  # add to your shell profile or a local, uncommitted env file
   ```
   `dev.sh` already passes `--scope bwhitmans-projects`, and the Vercel CLI picks up
   `VERCEL_TOKEN` from the environment automatically — so `./dev.sh web` deploys to the
   shared dev project with no interactive login.

## Publishing a dev build

Always run from the `tulip/` directory.

### Firmware — in a shell WITH ESP-IDF active

```bash
# activate your ESP-IDF environment, e.g.:
source ~/.espressif/python_env/idf5.4_py3.13_env/bin/activate   # your venv name may differ
source ~/esp/esp-idf-v5.4.1/export.sh                           # your idf path may differ

cd tulip
./dev.sh firmware
```

This builds the AMYboard firmware (`idf.py -DMICROPY_BOARD=AMYBOARD build`), packs the
filesystem, and uploads three images to the `dev` prerelease:
`amyboard-firmware-AMYBOARD.bin`, `amyboard-full-AMYBOARD.bin`, `amyboard-sys.bin`.

### Web

```bash
cd tulip
./dev.sh web          # VERCEL_TOKEN must be set in this shell (see setup)
```

This runs `python3 dev.py --build-only` (builds `stage/`, including `make web`), links
`stage/` to the `amyboard-dev` Vercel project on first run, then
`vercel deploy stage --prod`. It prints the dev URL when done.

> **Note:** older instructions warned that the web build had to run in a non-IDF shell
> because `make web` died with `ModuleNotFoundError: No module named 'soundfile'`. That
> is fixed — `amy` now imports `soundfile` lazily, so `make web` only needs `numpy` and
> runs whether or not ESP-IDF is active. That's why `./dev.sh` (firmware + web in one
> go) is fine.

## Testing

- Open **https://amyboard-dev.vercel.app**. It shows a "DEV channel" badge and flashes
  the rolling `dev` firmware.
- Connect the AMYboard over USB, flash, and verify your change.

## Promoting dev → release (maintainer only — not for testing)

Once a dev build is validated, the maintainer promotes it with the existing scripts:

```bash
./release.sh v-XXX-2026 upload          # firmware → version tag (becomes releases/latest)
cd amyboardweb && ./deploy.sh           # web → the 'amyboard' project / amyboard.com
```

Don't run these for routine testing — `dev.sh` is the whole loop for that.
