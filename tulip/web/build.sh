#!/bin/bash
# build.sh
# Just builds web version locally
set -e

if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  SED_INPLACE=(-i '')
fi

# In CI the checkout already provides submodules (and Linux needs no mpy-cross
# patch); only bootstrap them for local builds.
if [ -z "${CI:-}" ]; then
  source ../shared/grab_submodules.sh
fi

timestamp=$(date +%Y%m%d%H%M%S)

# AMY for web (emscripten). Shared by both web apps — ../webdev.py builds it
# once and sets SKIP_AMY_WEB=1 so this run reuses amy/build/ instead of
# recompiling the identical WASM a second time.
if [ -z "${SKIP_AMY_WEB:-}" ]; then
  cd ../../amy
  make web
  #make docs/amy-audioin.js
  cd ../tulip/web
fi

# Ship editable copies of the built-in apps as /sys/ex/my_*.py (baked into
# micropython.data via the Makefile's --preload-file of ../fs/tulip). The
# frozen originals are read-only; the docs point users at these copies.
# Gitignored (tulip/fs/tulip/ex/my_*); fs_create.py does the same for ESP images.
for app in drums juno6 voices worldui; do
  cp ../shared/py/$app.py ../fs/tulip/ex/my_$app.py
done

make

# Now modify the static html and copy everything to a stage area 
rm -rf stage
mkdir stage
mkdir stage/run

cp static/* stage/run

# Assemble the full tulip.computer site root. The Tulip-specific landing + its
# webfonts live in site/; the marketing assets (img/css/fonts/js) are shared with
# AMYboard Web + the docs, so they live in repo-root assets/. The web app itself
# goes under stage/run (below).
cp site/index.html stage/
cp -Rf site/webfonts stage/
cp -Rf ../../assets/css ../../assets/fonts ../../assets/img ../../assets/js stage/

# vercel.json must sit at the deploy root: the release/preview workflows run
# `vercel deploy stage`, so a vercel.json left in tulip/web/ is ignored. Copy it
# in so its headers (COOP/COEP cross-origin isolation for /run, plus cache rules)
# actually get served. (amyboardweb's dev.py does the equivalent copy.)
#
# WHAT IS IN IT, since JSON cannot say so itself -- and that is not a
# figure of speech: Vercel VALIDATES vercel.json against a strict schema
# and refuses an unknown property, so the usual "//" comment key fails
# the deploy outright ("should NOT have additional property `//`"). The
# explanation lives here.
#
#   /run          Tulip 1, this repo's own web build, in stage/run.
#   /2            TULIP 5, which is a different repo and a different
#                 Vercel project (tulip2-web), REWRITTEN onto this
#                 domain. Vercel does not do path-based domain
#                 assignment, so a proxy is the only way to put two
#                 projects on one hostname -- and it is the right one
#                 anyway: a path serving a COPY would be a second deploy
#                 somebody has to remember, which is how a stale wasm
#                 has shipped before. This way /2 is whatever that
#                 project last built, with no step in between.
#   /2/admin      the Tulip 5 admin panel, on its world service.
#
# Four things in there are load-bearing and each was a way to get it
# wrong:
#
#   * /2 REDIRECTS to /2/ first. Tulip 5's index.html asks for its
#     loader and its wasm by RELATIVE name, so without the trailing
#     slash they resolve against this domain's root and 404. 307 and not
#     308, because a permanent redirect is cached by the browser for
#     ever.
#   * the rewrites use `:path(.*)` and NOT `:path*`. The starred form
#     does not match an EMPTY segment, so with it `/2/` and `/2/admin/`
#     fell through to a Vercel 404 while `/2/index.html` worked --
#     which is the shape of this that looks like the origin is broken.
#   * the /2/admin rules come BEFORE /2/:path(.*), which would otherwise
#     swallow them: Vercel takes the first matching rewrite.
#   * /2 is excluded from the catch-all no-cache header below. Tulip 5's
#     own vercel.json already answers that for its files (index.html
#     no-store, the content-stamped micropython-<hash>.{mjs,wasm}
#     immutable for a year) and those headers come through the proxy
#     intact. A no-cache stamped over the top would make every visit
#     revalidate 8 MB that is guaranteed by its own NAME never to
#     change, which is the whole point of stamping it.
cp vercel.json stage/

cp ../../amy/build/amy.js stage/run/amy-$timestamp.js
cp ../../amy/build/amy.wasm stage/run/amy-$timestamp.wasm
# emscripten >=4 inlines the AudioWorklet glue into amy.js, so newer amy no longer
# ships a separate amy.aw.js (and amy.js never references one). Copy it only if an
# older pinned amy still has it -- otherwise `set -e` aborts the whole build on the
# missing file. (amyboardweb's dev.py already guards the same copy with os.path.exists.)
if [ -f ../../amy/docs/amy.aw.js ]; then
  cp ../../amy/docs/amy.aw.js stage/run/amy-$timestamp.aw.js
fi

cp build-standard/tulip/obj/micropython.wasm stage/run/tulipcc-$timestamp.wasm
cp build-standard/tulip/obj/micropython.mjs stage/run/tulipcc-$timestamp.mjs
cp build-standard/tulip/obj/micropython.data stage/run/tulipcc-$timestamp.data

sed "${SED_INPLACE[@]}" -e "s/TULIPCCMJS/tulipcc\-${timestamp}.mjs/g" -e "s/AMYJS/amy\-${timestamp}.js/g" stage/run/index.html
sed "${SED_INPLACE[@]}" -e "s/amy.js/amy\-${timestamp}.js/g" -e "s/amy.aw.js/amy\-${timestamp}.aw.js/g" -e "s/amy.wasm/amy\-${timestamp}.wasm/g" stage/run/amy-$timestamp.js
#sed "${SED_INPLACE[@]}" -e "s/amy\-audioin.aw.js/amy\-audioin\-${timestamp}.aw.js/g" -e "s/amy\-audioin.wasm/amy\-audioin\-${timestamp}.wasm/g" stage/run/amy-audioin-$timestamp.js
sed "${SED_INPLACE[@]}" -e "s/micropython./tulipcc\-${timestamp}./g" stage/run/tulipcc-$timestamp.mjs

# User C DSP web plumbing rides inside amy.js so it runs both on the page and
# in AMY's AudioWorklet scope (which only ever loads amy.js). Appended after
# the filename seds so its comments aren't rewritten.
cat ../shared/user_c_dsp_web.js >> stage/run/amy-$timestamp.js
# Generated table-driven C API bridge (amy_c_api_bind + AMY_C_API_PY_INSTALL),
# used by spss.js. Regenerate in amy/ with `make c-api`.
cat ../../amy/src/amy_c_api.generated.js >> stage/run/amy-$timestamp.js

