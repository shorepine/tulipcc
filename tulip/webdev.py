#!/usr/bin/env python3
"""
webdev.py — one command to build + serve BOTH Tulip web apps locally at once.

Run from tulip/:

    python3 webdev.py

What it does:

  1. Builds AMY for web (emscripten) ONCE and shares it between both apps.
     Previously tulip/web/build.sh and amyboardweb/dev.py each ran `make web`
     in amy/ separately, compiling the identical AMY WASM twice.
  2. Builds each app's own MicroPython WASM variant — AMYBOARD vs TULIP4_R11.
     These genuinely differ, so they are NOT shared.
  3. Serves each app on its own port, at its own document root, so absolute
     asset paths (/img, /css, /editor/, /run/, /api/…) resolve exactly like
     production (amyboard.com / tulip.computer) — no path rewriting needed:

         AMYboard Web : http://localhost:8000/        (editor at /editor/)
         Tulip Web    : http://localhost:8001/run/    (landing at /)

  4. Watches each app's static/, site/ and the shared repo-root assets/ and
     rebuilds the affected app(s) when files change. (Editing assets/, which is
     shared, rebuilds both.)

The build logic itself is reused from the existing per-app scripts
(amyboardweb/dev.py --build-only and tulip/web/build.sh, both with their AMY
step skipped) so there is a single source of truth — those are also what CI
uses. This script only builds AMY once, orchestrates them, and serves/watches.

Flags:
    --app {both,amyboard,tulip}   which app(s) to build/serve   (default: both)
    --build-only                  build stage/(s) and exit (no server, no watch)
    --no-watch                    serve but don't watch for changes
    --bootstrap                   force submodule bootstrap before building
    --amyboard-port N             AMYboard Web port             (default: 8000)
    --tulip-port N                Tulip Web port                (default: 8001)
"""

import argparse
import os
import subprocess
import sys
import threading
import time
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import parse_qs, urlparse
from urllib.request import Request, urlopen

# ── Paths ────────────────────────────────────────────────────────────────────

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))   # tulip/
REPO_ROOT  = os.path.dirname(SCRIPT_DIR)                   # repo root
AMY_DIR    = os.path.join(REPO_ROOT, "amy")
ASSETS_DIR = os.path.join(REPO_ROOT, "assets")            # shared site assets
SHARED_DIR = os.path.join(SCRIPT_DIR, "shared")           # has grab_submodules.sh

AMYBOARD_DIR = os.path.join(SCRIPT_DIR, "amyboardweb")
TULIP_DIR    = os.path.join(SCRIPT_DIR, "web")

# Per-app config. `build` is run in `dir`; its AMY step is skipped because we
# build AMY once up front (see build_amy_once).
APPS = {
    "amyboard": {
        "name":  "AMYboard Web",
        "dir":   AMYBOARD_DIR,
        "stage": os.path.join(AMYBOARD_DIR, "stage"),
        "url":   "/editor/",
        "build": [sys.executable, "dev.py", "--build-only", "--skip-amy-build"],
        "env":   {},
        "watch": [os.path.join(AMYBOARD_DIR, "static"), ASSETS_DIR],
    },
    "tulip": {
        "name":  "Tulip Web",
        "dir":   TULIP_DIR,
        "stage": os.path.join(TULIP_DIR, "stage"),
        "url":   "/run/",
        "build": ["bash", "build.sh"],
        "env":   {"SKIP_AMY_WEB": "1"},
        "watch": [os.path.join(TULIP_DIR, "static"),
                  os.path.join(TULIP_DIR, "site"), ASSETS_DIR],
    },
}

# ── Build ────────────────────────────────────────────────────────────────────

def bootstrap_submodules():
    """Initialise/patch the amy + micropython submodules (idempotent)."""
    print("[webdev] Bootstrapping submodules (tulip/shared/grab_submodules.sh) …")
    subprocess.run(["bash", "grab_submodules.sh"], cwd=SHARED_DIR, check=True)


def build_amy_once():
    """Compile AMY for web a single time; both apps reuse amy/build/."""
    print("[webdev] Building AMY for web once (make web) …")
    subprocess.run(["make", "web"], cwd=AMY_DIR, check=True)


def build_app(key):
    """(Re)build one app's stage/ — its AMY step is skipped (already built)."""
    app = APPS[key]
    print(f"[webdev] Building {app['name']} …")
    env = {**os.environ, **app["env"]}
    subprocess.run(app["build"], cwd=app["dir"], env=env, check=True)
    print(f"[webdev] {app['name']} stage/ ready.")

# ── HTTP server ──────────────────────────────────────────────────────────────

class DevHandler(SimpleHTTPRequestHandler):
    """Static handler with WASM-friendly CORS/COOP-COEP headers + a small
    same-origin proxy for GitHub firmware fetches (mirrors the per-app
    server.py these replaced)."""

    def end_headers(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        # Dev server: never serve stale JS/CSS/WASM after a rebuild.
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0")
        self.send_header("Pragma", "no-cache")
        self.send_header("Expires", "0")
        super().end_headers()

    def do_OPTIONS(self):
        self.send_response(204)
        self.end_headers()

    def do_GET(self):
        if self.path.startswith("/proxy") or self.path.startswith("/api/firmware"):
            self._handle_proxy()
            return
        super().do_GET()

    def _handle_proxy(self):
        query = parse_qs(urlparse(self.path).query)
        url = query.get("url", [None])[0]
        if not url:
            self.send_error(400, "Missing url parameter")
            return
        parsed = urlparse(url)
        allowed = {"github.com", "objects.githubusercontent.com", "raw.githubusercontent.com"}
        if parsed.scheme not in ("http", "https") or parsed.hostname not in allowed:
            self.send_error(403, "URL not allowed")
            return
        try:
            req = Request(url, headers={"User-Agent": "tulipcc-local-proxy"})
            with urlopen(req) as resp:
                data = resp.read()
                self.send_response(getattr(resp, "status", 200))
                self.send_header("Content-Type", resp.headers.get("Content-Type", "application/octet-stream"))
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)
        except Exception as exc:
            self.send_error(502, f"Proxy fetch failed: {exc}")

    def log_message(self, *args):
        pass  # keep the terminal clean


def serve_app(key, port):
    app = APPS[key]
    handler = partial(DevHandler, directory=app["stage"])
    httpd = ThreadingHTTPServer(("", port), handler)
    httpd.serve_forever()

# ── File watcher ─────────────────────────────────────────────────────────────

def collect_mtimes(roots):
    mtimes = {}
    for root in roots:
        if not os.path.exists(root):
            continue
        for dirpath, _, files in os.walk(root):
            for fname in files:
                p = os.path.join(dirpath, fname)
                try:
                    mtimes[p] = os.path.getmtime(p)
                except OSError:
                    pass
    return mtimes


def watcher_loop(selected):
    # Map each watch-root to the set of apps it affects (assets/ → both).
    root_apps = {}
    for key in selected:
        for root in APPS[key]["watch"]:
            root_apps.setdefault(root, set()).add(key)

    known = collect_mtimes(root_apps)
    print("[webdev] Watching static/, site/ and assets/ for changes …")
    while True:
        time.sleep(1)
        current = collect_mtimes(root_apps)
        changed = [p for p, mt in current.items() if known.get(p) != mt]
        if changed:
            affected = set()
            for p in changed:
                for root, keys in root_apps.items():
                    if p == root or p.startswith(root + os.sep):
                        affected |= keys
            names = ", ".join(os.path.basename(p) for p in changed[:5])
            more = "" if len(changed) <= 5 else f" (+{len(changed) - 5} more)"
            print(f"[webdev] changed: {names}{more} → rebuilding {', '.join(sorted(affected))}")
            for key in sorted(affected):
                try:
                    build_app(key)
                except subprocess.CalledProcessError as exc:
                    print(f"[webdev] rebuild of {key} failed: {exc}")
        known = current

# ── Entry point ──────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(description="Unified local dev server for both Tulip web apps")
    parser.add_argument("--app", choices=["both", "amyboard", "tulip"], default="both",
                        help="Which app(s) to build/serve (default: both)")
    parser.add_argument("--build-only", action="store_true",
                        help="Build stage/(s) and exit (no server, no watch)")
    parser.add_argument("--no-watch", action="store_true",
                        help="Serve but don't watch for file changes")
    parser.add_argument("--bootstrap", action="store_true",
                        help="Force submodule bootstrap before building")
    parser.add_argument("--amyboard-port", type=int, default=8000)
    parser.add_argument("--tulip-port", type=int, default=8001)
    args = parser.parse_args()

    selected = ["amyboard", "tulip"] if args.app == "both" else [args.app]
    ports = {"amyboard": args.amyboard_port, "tulip": args.tulip_port}

    # Submodules: build.sh bootstraps on its own for local runs, but dev.py
    # assumes they're present, so make sure they exist before we start.
    if args.bootstrap or not os.path.exists(os.path.join(AMY_DIR, "Makefile")):
        bootstrap_submodules()

    # AMY once, then each app (which skips its own AMY build).
    build_amy_once()
    for key in selected:
        build_app(key)

    if args.build_only:
        print("[webdev] --build-only: stage(s) built, exiting.")
        return

    for key in selected:
        t = threading.Thread(target=serve_app, args=(key, ports[key]), daemon=True)
        t.start()

    print("\n[webdev] Serving:")
    for key in selected:
        app = APPS[key]
        print(f"          {app['name']:<13} http://localhost:{ports[key]}{app['url']}")
    print("         (Ctrl-C to stop)\n")

    if not args.no_watch:
        threading.Thread(target=watcher_loop, args=(selected,), daemon=True).start()

    try:
        while True:
            time.sleep(3600)
    except KeyboardInterrupt:
        print("\n[webdev] Stopped.")


if __name__ == "__main__":
    main()
