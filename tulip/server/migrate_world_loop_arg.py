#!/usr/bin/env python3
"""Migrate Tulip World / AMYboard World sketches to the mandatory loop(tick).

AMYboard's sketch runner used to accept either `def loop():` or `def loop(step):`,
deciding which by calling loop(step) and catching TypeError. That guess is not
decidable -- MicroPython binds arguments before running the body, so an arity
error and a TypeError raised *inside* a one-argument loop look identical at the
call site, and a sketch whose first loop() happened to raise was permanently
misfiled as zero-argument. The runner now always calls loop(tick), and the
argument is AMY's absolute sequencer tick rather than a 32nd-note step index.

So two things changed, and a sketch can need either or both:

  1. the argument is now REQUIRED   -- `def loop():` no longer runs at all
  2. the argument is now TICKS      -- a sketch that used the old `step` value
                                       must divide by amyboard.TICKS_PER_STEP

Usage:
    python3 migrate_world_loop_arg.py --report              # scan only, no writes
    python3 migrate_world_loop_arg.py --report --verbose    # + per-sketch detail
    python3 migrate_world_loop_arg.py --write-diffs DIR     # dump before/after files
    WORLD_ADMIN_TOKEN=xxx python3 migrate_world_loop_arg.py --apply

`--apply` re-uploads each rewritten sketch as a new version under the original
username/filename/description. Both worlds are versioned, so the previous
version stays in history.

Only the latest version of each (scope, username, filename) is considered.

No external dependencies -- standard library only.
"""

import argparse
import ast
import io
import json
import os
import sys
import uuid
from urllib.error import HTTPError
from urllib.request import Request, urlopen

DEFAULT_URL = "https://tulipcc-production.up.railway.app"
MAX_LIMIT = 5000
SCOPES = ("amyboardworld", "tulipworld")

# Preferred name for the new parameter, and the constant a rescaled sketch uses.
TICK_NAME = "tick"
STEP_EXPR = "%s // amyboard.TICKS_PER_STEP"

# ── Classification ────────────────────────────────────────────────────────────
#
# ADD_ARG   `def loop():`  ->  `def loop(tick):`
#           Body untouched: it never had the argument, so nothing to rescale.
#
# RENAME    `def loop(step):` where the parameter is never read.
#           Only the name is a lie now (it receives ticks), so rename it.
#           Body untouched.
#
# RESCALE   `def loop(step):` where the parameter IS read. The value it gets is
#           now 6x larger (ticks, not 32nd-note steps), so every use would be
#           wrong. Rename the parameter to `tick` and insert
#           `step = tick // amyboard.TICKS_PER_STEP` as the first statement,
#           which leaves the body's arithmetic exactly as the author wrote it.
#           Adds `import amyboard` if the sketch doesn't already have it.
#
# SKIP      No top-level `def loop`, or already migrated, or unparseable.
CLASSES = ("ADD_ARG", "RENAME", "RESCALE", "SKIP")


# ── API helpers (same shapes as migrate_world_ticks.py) ───────────────────────

def api_get(url, binary=False, timeout=90):
    with urlopen(Request(url), timeout=timeout) as r:
        return r.read() if binary else json.load(r)


def list_items(api_url, scope):
    url = (f"{api_url}/api/{scope}/files?limit={MAX_LIMIT}"
           f"&latest_per_user_env=true")
    d = api_get(url)
    return d.get("items", d.get("files", []))


def download(api_url, scope, item_id):
    return api_get(f"{api_url}/api/{scope}/files/{item_id}/download", binary=True)


def upload(api_url, scope, username, filename, description, blob, admin_token=None):
    boundary = uuid.uuid4().hex
    body = b""
    for key, val in (("username", username), ("description", description)):
        body += f"--{boundary}\r\n".encode()
        body += f'Content-Disposition: form-data; name="{key}"\r\n\r\n'.encode()
        body += f"{val}\r\n".encode()
    body += f"--{boundary}\r\n".encode()
    body += (f'Content-Disposition: form-data; name="file"; '
             f'filename="{filename}"\r\n').encode()
    body += b"Content-Type: application/octet-stream\r\n\r\n"
    body += blob + f"\r\n--{boundary}--\r\n".encode()
    req = Request(f"{api_url}/api/{scope}/upload", data=body, method="POST")
    req.add_header("Content-Type", f"multipart/form-data; boundary={boundary}")
    if admin_token:
        req.add_header("X-Admin-Token", admin_token)
    with urlopen(req, timeout=60) as r:
        return json.load(r)


# ── Source analysis and rewriting ─────────────────────────────────────────────

def _loop_def(tree):
    """The last top-level `def loop`, matching Python's own rebinding."""
    fns = [n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == "loop"]
    return fns[-1] if fns else None


def _free_name(src, preferred=TICK_NAME):
    """A parameter name not already used anywhere in the sketch.

    A blind rename to `tick` is not safe: a sketch can already have its own
    module-level `tick` (universal_hair.py did), and shadowing it inside loop
    would silently change behaviour -- or, with a `global tick`, not even
    compile.
    """
    names = set()
    try:
        for node in ast.walk(ast.parse(src)):
            if isinstance(node, ast.Name):
                names.add(node.id)
            elif isinstance(node, ast.arg):
                names.add(node.arg)
            elif isinstance(node, (ast.FunctionDef, ast.ClassDef)):
                names.add(node.name)
    except SyntaxError:
        return preferred
    if preferred not in names:
        return preferred
    cand = "_" + preferred
    i = 2
    while cand in names:
        cand = "_%s%d" % (preferred, i)
        i += 1
    return cand


def _imports_amyboard(tree):
    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            if any(a.name.split(".")[0] == "amyboard" for a in node.names):
                return True
        elif isinstance(node, ast.ImportFrom):
            if (node.module or "").split(".")[0] == "amyboard":
                return True
    return False


def _first_body_line(fn, lines):
    """Line index (0-based) to insert at: after any docstring and any leading
    global/nonlocal declarations, which must keep preceding their own uses."""
    idx = 0
    for stmt in fn.body:
        is_doc = (idx == 0 and isinstance(stmt, ast.Expr)
                  and isinstance(stmt.value, ast.Constant)
                  and isinstance(stmt.value.value, str))
        if is_doc or isinstance(stmt, (ast.Global, ast.Nonlocal)):
            idx += 1
            continue
        return stmt.lineno - 1
    # Body was nothing but docstring/global declarations.
    return fn.body[-1].end_lineno if fn.body else fn.lineno


def classify_and_rewrite(src):
    """Return (cls, new_src, note). new_src is None when nothing changes."""
    try:
        tree = ast.parse(src)
    except SyntaxError as e:
        return "SKIP", None, "unparseable: %s" % e
    fn = _loop_def(tree)
    if fn is None:
        return "SKIP", None, "no top-level def loop"
    args = fn.args
    if args.vararg is not None:
        return "SKIP", None, "loop(*args) already accepts the tick"
    if len(args.args) > 1:
        return "SKIP", None, "loop() takes %d args -- needs a human" % len(args.args)

    lines = src.split("\n")
    def_i = fn.lineno - 1
    def_line = lines[def_i]

    if not args.args:
        # ADD_ARG: `def loop():` -> `def loop(tick):`
        name = _free_name(src)
        if "loop()" not in def_line:
            return "SKIP", None, "unexpected def line: %r" % def_line
        lines[def_i] = def_line.replace("loop()", "loop(%s)" % name, 1)
        return "ADD_ARG", "\n".join(lines), "added parameter %r" % name

    param = args.args[0].arg
    reads = [n for n in ast.walk(fn)
             if isinstance(n, ast.Name) and n.id == param and isinstance(n.ctx, ast.Load)]
    name = _free_name(src)

    # Rewrite the parameter in the def line only (the body keeps `param`).
    old_sig = "loop(%s)" % param
    if old_sig not in def_line:
        return "SKIP", None, "unexpected def line: %r" % def_line

    if not reads:
        # RENAME: the parameter is never read, so only the name was misleading.
        lines[def_i] = def_line.replace(old_sig, "loop(%s)" % name, 1)
        return "RENAME", "\n".join(lines), "%r -> %r (unused)" % (param, name)

    # RESCALE: keep the body verbatim; recreate `param` from the tick.
    lines[def_i] = def_line.replace(old_sig, "loop(%s)" % name, 1)
    ins_i = _first_body_line(fn, lines)
    indent = " " * (len(lines[ins_i]) - len(lines[ins_i].lstrip())) if ins_i < len(lines) else "    "
    if not indent:
        indent = "    "
    lines.insert(ins_i, "%s%s = %s" % (indent, param, STEP_EXPR % name))
    new_src = "\n".join(lines)

    if not _imports_amyboard(tree):
        new_src = _add_amyboard_import(new_src)
    return "RESCALE", new_src, "%r now derived from %r (%d use%s)" % (
        param, name, len(reads), "" if len(reads) == 1 else "s")


def _add_amyboard_import(src):
    """Insert `import amyboard` before the first import, else after the leading
    comment block (sketches start with `# AMYboard Sketch` / `# DESCRIPTION:`)."""
    lines = src.split("\n")
    try:
        tree = ast.parse(src)
        firsts = [n.lineno - 1 for n in tree.body if isinstance(n, (ast.Import, ast.ImportFrom))]
        if firsts:
            lines.insert(min(firsts), "import amyboard")
            return "\n".join(lines)
    except SyntaxError:
        pass
    i = 0
    while i < len(lines) and (not lines[i].strip() or lines[i].lstrip().startswith("#")):
        i += 1
    lines.insert(i, "import amyboard")
    return "\n".join(lines)


def verify(new_src):
    """The rewrite must parse and leave loop taking exactly one argument."""
    try:
        tree = ast.parse(new_src)
    except SyntaxError as e:
        return False, "rewrite does not parse: %s" % e
    fn = _loop_def(tree)
    if fn is None:
        return False, "rewrite lost def loop"
    if len(fn.args.args) != 1 and fn.args.vararg is None:
        return False, "rewrite left arity %d" % len(fn.args.args)
    return True, ""


# ── Driver ────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--url", default=DEFAULT_URL)
    ap.add_argument("--report", action="store_true", help="scan only, no writes")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--write-diffs", metavar="DIR", help="dump before/after files")
    ap.add_argument("--apply", action="store_true", help="re-upload rewritten sketches")
    ap.add_argument("--scope", choices=SCOPES, help="limit to one world")
    opts = ap.parse_args()
    if not (opts.report or opts.write_diffs or opts.apply):
        opts.report = True

    token = os.environ.get("WORLD_ADMIN_TOKEN")
    if opts.apply and not token:
        sys.exit("--apply needs WORLD_ADMIN_TOKEN in the environment")

    counts = dict.fromkeys(CLASSES, 0)
    failures = []
    rewritten = []

    for scope in (opts.scope,) if opts.scope else SCOPES:
        for item in list_items(opts.url, scope):
            fname = item.get("filename") or ""
            if not fname.endswith(".py"):
                continue
            try:
                src = download(opts.url, scope, item["id"]).decode("utf-8", "replace")
            except Exception as e:
                failures.append((scope, item, "download failed: %s" % e))
                continue
            cls, new_src, note = classify_and_rewrite(src)
            counts[cls] += 1
            if new_src is None:
                if opts.verbose and cls == "SKIP":
                    print("  SKIP     %s/%s: %s" % (item["username"], fname, note))
                continue
            ok, why = verify(new_src)
            if not ok:
                failures.append((scope, item, why))
                continue
            rewritten.append((scope, item, src, new_src))
            if opts.verbose:
                print("  %-8s %s/%s: %s" % (cls, item["username"], fname, note))

    print("\n%-9s %s" % ("CLASS", "COUNT"))
    for c in CLASSES:
        print("%-9s %d" % (c, counts[c]))
    print("%-9s %d" % ("TO WRITE", len(rewritten)))
    if failures:
        print("\n%d FAILURE(S) -- not rewritten:" % len(failures))
        for scope, item, why in failures:
            print("  %s %s/%s: %s" % (scope, item["username"], item["filename"], why))

    if opts.write_diffs:
        os.makedirs(opts.write_diffs, exist_ok=True)
        for scope, item, old, new in rewritten:
            base = "%s_%s_%s" % (scope, item["username"], item["filename"][:-3])
            with open(os.path.join(opts.write_diffs, base + ".before.py"), "w") as f:
                f.write(old)
            with open(os.path.join(opts.write_diffs, base + ".after.py"), "w") as f:
                f.write(new)
        print("\nwrote %d before/after pairs to %s" % (len(rewritten), opts.write_diffs))

    if opts.apply:
        done = 0
        for scope, item, _old, new in rewritten:
            try:
                upload(opts.url, scope, item["username"], item["filename"],
                       item.get("description") or "", new.encode("utf-8"), token)
                done += 1
            except HTTPError as e:
                print("  upload failed %s/%s: HTTP %s %s"
                      % (item["username"], item["filename"], e.code, e.read()[:200]))
            except Exception as e:
                print("  upload failed %s/%s: %s" % (item["username"], item["filename"], e))
        print("\nre-uploaded %d/%d sketches" % (done, len(rewritten)))

    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
