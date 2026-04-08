#!/usr/bin/env python3
"""
dev.py — AMYboard Web dev server + file watcher.

Run once from tulip/amyboardweb/:
    python3 dev.py

On startup:
  - Rebuilds stage/ from scratch (same logic as build.sh copy+sed steps).
  - Picks a timestamp once and keeps it for the whole session (WASM names stay stable).

While running:
  - Polls static/, www/img, www/fonts, www/css, www/js, api/ every second.
  - On any change, syncs just the changed files to stage/ and re-applies the
    sed substitution to editor/index.html if that file changed.
  - HTTP server serves stage/ via absolute path, so it never breaks when files
    are updated in place.

Do NOT kill this process while developing.  Only stop it when you're done or
the user asks you to.
"""

import os
import shutil
import subprocess
import threading
import time
from datetime import datetime
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import parse_qs, urlparse
from urllib.request import Request, urlopen

# ── Paths ──────────────────────────────────────────────────────────────────────

SCRIPT_DIR      = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT       = os.path.abspath(os.path.join(SCRIPT_DIR, "../.."))

STAGE_DIR       = os.path.join(SCRIPT_DIR, "stage")
STATIC_DIR      = os.path.join(SCRIPT_DIR, "static")
API_DIR         = os.path.join(SCRIPT_DIR, "api")
VERCEL_JSON     = os.path.join(SCRIPT_DIR, "vercel.json")

WWW_IMG         = os.path.join(REPO_ROOT, "www", "img")
WWW_FONTS       = os.path.join(REPO_ROOT, "www", "fonts")
WWW_CSS         = os.path.join(REPO_ROOT, "www", "css")
WWW_JS          = os.path.join(REPO_ROOT, "www", "js")

AMY_DIR         = os.path.join(REPO_ROOT, "amy")
AMY_BUILD       = os.path.join(REPO_ROOT, "amy", "build")
AMY_DOCS        = os.path.join(REPO_ROOT, "amy", "docs")
MICROPYTHON_DIR = os.path.join(SCRIPT_DIR, "build-standard", "tulip", "obj")

PORT = int(os.environ.get("PORT", 8000))

# Fixed for the whole dev session — WASM filenames stay stable.
SESSION_TS = datetime.now().strftime("%Y%m%d%H%M%S")

# ── Helpers ────────────────────────────────────────────────────────────────────

def replace_in_file(path, replacements):
    """Apply [(old, new), …] plain-string replacements to a file in-place."""
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        content = f.read()
    for old, new in replacements:
        content = content.replace(old, new)
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)


def apply_editor_subs(html_path):
    ts = SESSION_TS
    replace_in_file(html_path, [
        ("AMYBOARDMJS", f"amyboard-{ts}.mjs"),
        ("AMYJS",       f"amy-{ts}.js"),
    ])


def copy_to_stage(src, stage_rel):
    """Copy src → stage/<stage_rel>, creating dirs as needed."""
    dst = os.path.join(STAGE_DIR, stage_rel)
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    shutil.copy2(src, dst)

# ── Initial full build ─────────────────────────────────────────────────────────

def full_build():
    ts = SESSION_TS
    print(f"[dev] Building stage/ (session timestamp {ts}) …")

    # Compile AMY for web (emscripten)
    print("[dev] Building AMY (make web) …")
    subprocess.run(["make", "web"], cwd=AMY_DIR, check=True)

    # Copy generated JS files from AMY build into static/
    for name in ("amy_api.generated.js", "patches.generated.js"):
        src = os.path.join(AMY_BUILD, name)
        if os.path.exists(src):
            shutil.copy2(src, os.path.join(STATIC_DIR, name))

    # Compile MicroPython/AMYboard for web (emscripten)
    print("[dev] Building AMYboard (make) …")
    subprocess.run(["make"], cwd=SCRIPT_DIR, check=True)

    if os.path.exists(STAGE_DIR):
        shutil.rmtree(STAGE_DIR)
    os.makedirs(STAGE_DIR)

    # static files
    shutil.copytree(STATIC_DIR, STAGE_DIR, dirs_exist_ok=True)

    # www assets
    for src, name in [(WWW_IMG, "img"), (WWW_FONTS, "fonts"),
                      (WWW_CSS, "css"),  (WWW_JS,    "js")]:
        if os.path.exists(src):
            shutil.copytree(src, os.path.join(STAGE_DIR, name), dirs_exist_ok=True)

    # api + vercel
    if os.path.exists(API_DIR):
        shutil.copytree(API_DIR, os.path.join(STAGE_DIR, "api"), dirs_exist_ok=True)
    if os.path.exists(VERCEL_JSON):
        shutil.copy2(VERCEL_JSON, os.path.join(STAGE_DIR, "vercel.json"))

    # Timestamped WASM / JS blobs
    blobs = [
        (os.path.join(AMY_BUILD,       "amy.js"),            f"amy-{ts}.js"),
        (os.path.join(AMY_BUILD,       "amy.wasm"),          f"amy-{ts}.wasm"),
        (os.path.join(AMY_DOCS,        "amy.aw.js"),         f"amy-{ts}.aw.js"),
        (os.path.join(MICROPYTHON_DIR, "micropython.wasm"),  f"amyboard-{ts}.wasm"),
        (os.path.join(MICROPYTHON_DIR, "micropython.mjs"),   f"amyboard-{ts}.mjs"),
        (os.path.join(MICROPYTHON_DIR, "micropython.data"),  f"amyboard-{ts}.data"),
    ]
    for src, dst_name in blobs:
        if os.path.exists(src):
            shutil.copy2(src, os.path.join(STAGE_DIR, dst_name))

    # Substitute placeholders
    editor_html = os.path.join(STAGE_DIR, "editor", "index.html")
    if os.path.exists(editor_html):
        apply_editor_subs(editor_html)

    amy_js = os.path.join(STAGE_DIR, f"amy-{ts}.js")
    if os.path.exists(amy_js):
        replace_in_file(amy_js, [
            ("amy.aw.js",  f"amy-{ts}.aw.js"),
            ("amy.wasm",   f"amy-{ts}.wasm"),
            ("amy.js",     f"amy-{ts}.js"),
        ])

    amy_mjs = os.path.join(STAGE_DIR, f"amyboard-{ts}.mjs")
    if os.path.exists(amy_mjs):
        replace_in_file(amy_mjs, [("micropython.", f"amyboard-{ts}.")])

    print("[dev] Build done.")

# ── Incremental sync ───────────────────────────────────────────────────────────

# Map each watch-root to its stage sub-directory ("" means stage root)
WATCH_ROOTS = [
    (STATIC_DIR, ""),
    (WWW_IMG,    "img"),
    (WWW_FONTS,  "fonts"),
    (WWW_CSS,    "css"),
    (WWW_JS,     "js"),
    (API_DIR,    "api"),
]


def src_to_stage_rel(src_path):
    for root, stage_subdir in WATCH_ROOTS:
        if src_path.startswith(root + os.sep) or src_path.startswith(root + "/"):
            rel = os.path.relpath(src_path, root)
            return os.path.join(stage_subdir, rel) if stage_subdir else rel
    return None


def incremental_sync(changed):
    for src in changed:
        stage_rel = src_to_stage_rel(src)
        if stage_rel is None:
            continue
        if not os.path.exists(src):
            continue  # deleted — leave stage alone for now
        dst = os.path.join(STAGE_DIR, stage_rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)
        print(f"[dev] synced  {stage_rel}")
        # Re-apply subs if it's the editor HTML
        if os.path.normpath(dst) == os.path.normpath(
                os.path.join(STAGE_DIR, "editor", "index.html")):
            apply_editor_subs(dst)
            print("[dev]   (re-applied timestamp subs to editor/index.html)")

# ── File watcher ───────────────────────────────────────────────────────────────

def collect_mtimes():
    mtimes = {}
    for root, _ in WATCH_ROOTS:
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


def watcher_loop():
    known = collect_mtimes()
    print("[dev] Watching for changes (polling every 1 s) …")
    while True:
        time.sleep(1)
        current = collect_mtimes()
        changed = [p for p, mt in current.items() if known.get(p) != mt]
        if changed:
            labels = [os.path.basename(p) for p in changed]
            print(f"[dev] changed: {labels}")
            incremental_sync(changed)
        known = current

# ── HTTP server ────────────────────────────────────────────────────────────────

class DevHandler(SimpleHTTPRequestHandler):
    """Serves from STAGE_DIR (absolute path — never breaks on rebuilds)."""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=STAGE_DIR, **kwargs)

    def end_headers(self):
        self.send_header("Access-Control-Allow-Origin",  "*")
        self.send_header("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cross-Origin-Opener-Policy",   "same-origin")
        self.send_header("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0")
        self.send_header("Pragma",  "no-cache")
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
        url   = query.get("url", [None])[0]
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
                self.send_header("Content-Type",   resp.headers.get("Content-Type", "application/octet-stream"))
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)
        except Exception as exc:
            self.send_error(502, f"Proxy fetch failed: {exc}")

    def log_message(self, fmt, *args):
        pass  # keep terminal clean; un-comment for request logs
        # print(f"[http] {self.address_string()} {fmt % args}")


# ── Entry point ────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    os.chdir(SCRIPT_DIR)

    full_build()

    watcher = threading.Thread(target=watcher_loop, daemon=True)
    watcher.start()

    httpd = HTTPServer(("", PORT), DevHandler)
    print(f"[dev] Serving http://localhost:{PORT}/  (Ctrl-C to stop)")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n[dev] Stopped.")
