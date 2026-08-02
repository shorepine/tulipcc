#!/usr/bin/env python3
"""Migrate Tulip World / AMYboard World user code off AMY's old `time=` and
`sequence=` scheduling kwargs onto tick-based `ticks=`.

AMY 1.2.110 (tulipcc PR #1266) removed the millisecond `time=` wire parameter
and the `sequence=` parameter; both are now the single `ticks=` parameter.
The rename is NOT uniform -- see CLASSES below -- so this script parses each
sketch with `ast`, classifies every affected call site, and only rewrites the
ones whose translation is unambiguous.

Usage:
    python3 migrate_world_ticks.py --report              # scan only, no writes
    python3 migrate_world_ticks.py --report --verbose    # + per-call-site detail
    python3 migrate_world_ticks.py --write-diffs DIR     # dump before/after files
    WORLD_ADMIN_TOKEN=xxx python3 migrate_world_ticks.py --apply

`--apply` re-uploads each rewritten sketch as a new version under the original
username/filename/description, with created_at_ms set to the original + 1ms so
it sorts as the latest version and stays next to the original in the history.
The original row is left in place (both worlds are already versioned).

Only the latest version of each (scope, username, filename) is considered --
re-uploading superseded history would be pointless.

No external dependencies -- standard library only.
"""

import argparse
import ast
import io
import json
import os
import re
import sys
import tarfile
import uuid
from urllib.error import HTTPError
from urllib.request import Request, urlopen

DEFAULT_URL = "https://tulipcc-production.up.railway.app"
MAX_LIMIT = 5000
SCOPES = ("amyboardworld", "tulipworld")

# AMY's default tempo: 108 BPM at 48 PPQ.
TICKS_PER_MS_EXPR = "TICKS_PER_MS"
TICKS_PER_MS_DEF = "TICKS_PER_MS = 108 * 48 / 60000.0  # AMY default tempo (108 BPM, 48 PPQ)"

# Calls whose `time=` / `sequence=` kwarg is an AMY scheduling parameter.
# `amy.send`/`amy.message` are the wire API; the rest are the synth.py /
# sequencer.py wrappers that forward straight to it.
AMY_FUNCS = {"send", "message", "note_on", "note_off", "amy_send", "play"}

# ── Classification ────────────────────────────────────────────────────────────
#
# SEQUENCE_RENAME  sequence="0,384,1"  -> ticks="0,384,1"
#                  Pure rename: the tick/period/tag string format is unchanged.
#
# CALLBACK_DROP    time=t inside a sequencer callback def f(t).
#                  Old: `t` was a tick count handed to a *milliseconds*
#                  parameter, so it was always in the past and AMY played it
#                  immediately. New: AMY *drops* a one-off `ticks=` already in
#                  the past, so `ticks=t` would be silence. Correct migration
#                  is to drop the kwarg and play now -- exactly the fix applied
#                  to arpegg.py and the web examples in commit fa252530.
#
# PAST_LITERAL     time=1000 (a bare number, i.e. absolute ms 1000 since AMY
#                  boot -- always in the past). Same reasoning: drop the kwarg.
#
# MS_CONVERT       time=tulip.amy_ticks_ms() + 250, or time=now + 250 where
#                  `now` came from amy_ticks_ms(). A genuine future ms offset;
#                  needs a real ms->tick conversion anchored on seq_ticks().
#
# REVIEW           anything else -- reported, never auto-rewritten.
CLASSES = ("SEQUENCE_RENAME", "CALLBACK_DROP", "PAST_LITERAL", "MS_CONVERT", "REVIEW")
AUTO_CLASSES = ("SEQUENCE_RENAME", "CALLBACK_DROP", "PAST_LITERAL", "MS_CONVERT")


# ── API helpers ───────────────────────────────────────────────────────────────

def api_get(url, binary=False, timeout=90):
    with urlopen(Request(url), timeout=timeout) as r:
        return r.read() if binary else json.load(r)


def list_items(api_url, scope, latest_only=True):
    url = (f"{api_url}/api/{scope}/files?limit={MAX_LIMIT}"
           f"&latest_per_user_env={'true' if latest_only else 'false'}")
    d = api_get(url)
    return d.get("items", d.get("files", []))


def download(api_url, scope, item_id):
    return api_get(f"{api_url}/api/{scope}/files/{item_id}/download", binary=True)


def upload(api_url, scope, username, filename, description, blob, created_at_ms,
           admin_token=None):
    boundary = uuid.uuid4().hex
    body = b""
    fields = [("username", username), ("description", description),
              ("created_at_ms", str(created_at_ms))]
    for key, val in fields:
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


# ── Source analysis ───────────────────────────────────────────────────────────

class Finding:
    def __init__(self, cls, kw, lineno, col, end_col, snippet, note=""):
        self.cls, self.kw = cls, kw
        self.lineno, self.col, self.end_col = lineno, col, end_col
        self.snippet, self.note = snippet, note

    def __repr__(self):
        return f"<{self.cls} line {self.lineno}: {self.snippet}>"


def _call_name(node):
    """Return the trailing attribute/name of a Call's func, or ''."""
    f = node.func
    if isinstance(f, ast.Attribute):
        return f.attr
    if isinstance(f, ast.Name):
        return f.id
    return ""


def _is_amy_call(node):
    return _call_name(node) in AMY_FUNCS


MS_CLOCKS = ("amy_ticks_ms", "ticks_ms")


def _direct_ms_call(node):
    """True if `node` contains a direct call to AMY's millisecond clock.

    Matches `tulip.amy_ticks_ms()`, `amy.ticks_ms()` and bare `amy_ticks_ms()`.
    Deliberately does NOT match `tulip.ticks_ms()` -- that's the system clock,
    a different timebase, and any use of it with `time=` needs a human.
    """
    for n in ast.walk(node):
        if not isinstance(n, ast.Call):
            continue
        f = n.func
        if isinstance(f, ast.Attribute):
            if f.attr == "amy_ticks_ms":
                return True
            if (f.attr == "ticks_ms" and isinstance(f.value, ast.Name)
                    and f.value.id == "amy"):
                return True
        elif isinstance(f, ast.Name) and f.id == "amy_ticks_ms":
            return True
    return False


def _target_names(targets):
    for t in targets:
        if isinstance(t, ast.Name):
            yield t.id
        elif isinstance(t, ast.Attribute):
            yield t.attr


def _ms_taint(tree):
    """Names (and function-parameter names) that carry a millisecond value.

    Iterates to a fixpoint over three propagation rules:
      1. `now = tulip.amy_ticks_ms()`      -> `now` is ms
      2. `def _clock(): return amy_ticks_ms()` -> calls to `_clock` are ms
      3. `f(when=now)` where `now` is ms   -> f's `when` parameter is ms
    """
    funcs = {n.name: n for n in ast.walk(tree)
             if isinstance(n, (ast.FunctionDef, ast.AsyncFunctionDef))}
    ms_names, ms_funcs = set(), set()

    def expr_is_ms(node):
        if _direct_ms_call(node):
            return True
        for n in ast.walk(node):
            if isinstance(n, ast.Name) and n.id in ms_names:
                return True
            if isinstance(n, ast.Call):
                f = n.func
                name = f.attr if isinstance(f, ast.Attribute) else getattr(f, "id", None)
                if name in ms_funcs:
                    return True
        return False

    for _ in range(6):
        before = (len(ms_names), len(ms_funcs))

        for n in ast.walk(tree):
            if isinstance(n, ast.Assign) and expr_is_ms(n.value):
                ms_names.update(_target_names(n.targets))
            elif isinstance(n, ast.AnnAssign) and n.value and expr_is_ms(n.value):
                ms_names.update(_target_names([n.target]))
            elif isinstance(n, ast.AugAssign) and expr_is_ms(n.value):
                ms_names.update(_target_names([n.target]))

        # Rule 2: a local function that returns a millisecond expression.
        for name, fn in funcs.items():
            for n in ast.walk(fn):
                if isinstance(n, ast.Return) and n.value is not None and expr_is_ms(n.value):
                    ms_funcs.add(name)
                    break

        # Rule 3: an ms argument taints the callee's matching parameter.
        for n in ast.walk(tree):
            if not isinstance(n, ast.Call):
                continue
            f = n.func
            cname = f.attr if isinstance(f, ast.Attribute) else getattr(f, "id", None)
            fn = funcs.get(cname)
            if fn is None:
                continue
            params = [a.arg for a in fn.args.args]
            for i, arg in enumerate(n.args):
                if i < len(params) and expr_is_ms(arg):
                    ms_names.add(params[i])
            for kw in n.keywords:
                if kw.arg and expr_is_ms(kw.value):
                    ms_names.add(kw.arg)

        if (len(ms_names), len(ms_funcs)) == before:
            break
    return ms_names, ms_funcs


# Calls that register a python function as a sequencer callback. The
# registered function's first non-self parameter is the AMY tick.
SEQ_REGISTRARS = {"seq_add_callback", "TulipSequence", "AMYSequence", "add",
                  "add_callback", "Sequence"}


def _func_ref_name(node):
    """Name of a function referenced (not called) as an argument."""
    if isinstance(node, ast.Name):
        return node.id
    if isinstance(node, ast.Attribute):
        return node.attr
    return None


def _tick_taint(tree, ms_names):
    """Parameter names that carry an AMY sequencer tick.

    Seeded only from real evidence of callback registration -- either a
    `seq.add(0, fn)` / `TulipSequence(8, fn)` style registrar call, or an
    assignment to a `*_action` / `*_callback` attribute -- then propagated
    through local calls. Anything we can't tie back to a registration stays
    unclassified and gets reported for manual review, because a bare `time=x`
    parameter is just as likely to be a millisecond offset (see xanadu.py,
    whose `timestamp=` param is ms, not ticks).
    """
    funcs = {n.name: n for n in ast.walk(tree)
             if isinstance(n, (ast.FunctionDef, ast.AsyncFunctionDef))}

    def first_param(fn):
        args = [a.arg for a in fn.args.args if a.arg not in ("self", "cls")]
        return args[0] if args else None

    registered = set()
    for n in ast.walk(tree):
        if isinstance(n, ast.Call):
            cname = (n.func.attr if isinstance(n.func, ast.Attribute)
                     else getattr(n.func, "id", None))
            if cname in SEQ_REGISTRARS:
                for a in list(n.args) + [k.value for k in n.keywords]:
                    if isinstance(a, (ast.Name, ast.Attribute)):
                        registered.add(_func_ref_name(a))
        elif isinstance(n, ast.Assign):
            for t in n.targets:
                if (isinstance(t, ast.Attribute)
                        and (t.attr.endswith("_action") or t.attr.endswith("_callback"))
                        and isinstance(n.value, (ast.Name, ast.Attribute))):
                    registered.add(_func_ref_name(n.value))

    tick_names = set()
    for name in registered:
        fn = funcs.get(name)
        if fn is not None:
            p = first_param(fn)
            if p and p not in ms_names:
                tick_names.add(p)

    # Propagate: a tick argument taints the callee's matching parameter.
    for _ in range(5):
        before = len(tick_names)
        for n in ast.walk(tree):
            if not isinstance(n, ast.Call):
                continue
            cname = (n.func.attr if isinstance(n.func, ast.Attribute)
                     else getattr(n.func, "id", None))
            fn = funcs.get(cname)
            if fn is None:
                continue
            params = [a.arg for a in fn.args.args]
            for i, arg in enumerate(n.args):
                if (i < len(params) and isinstance(arg, ast.Name)
                        and arg.id in tick_names and params[i] not in ms_names):
                    tick_names.add(params[i])
            for kw in n.keywords:
                if (kw.arg and isinstance(kw.value, ast.Name)
                        and kw.value.id in tick_names and kw.arg not in ms_names):
                    tick_names.add(kw.arg)
        if len(tick_names) == before:
            break
    return tick_names


def _is_past_tick_expr(v, tick_names, ms_names):
    """True for `t` or `t + 50` where `t` is a known sequencer tick.

    Under the old API such a value was a tick (a small integer) fed to a
    *milliseconds* parameter, so it was always in the past and AMY played it
    immediately. `ticks=` would drop it instead, so the kwarg must go.
    """
    names = [n for n in ast.walk(v) if isinstance(n, ast.Name)]
    if not names or any(n.id in ms_names for n in names):
        return False
    if not all(n.id in tick_names for n in names):
        return False
    # Only bare names or name +/- numeric literals.
    for n in ast.walk(v):
        if isinstance(n, (ast.Name, ast.Load, ast.BinOp, ast.Add, ast.Sub,
                          ast.Mult, ast.USub, ast.UnaryOp)):
            continue
        if isinstance(n, ast.Constant) and isinstance(n.value, (int, float)):
            continue
        return False
    return True


def analyze(src, filename="<sketch>"):
    """Parse `src`, return (list[Finding], parse_error_or_None)."""
    try:
        tree = ast.parse(src)
    except SyntaxError as e:
        return [], f"SyntaxError line {e.lineno}: {e.msg}"

    lines = src.split("\n")
    ms_names, _ = _ms_taint(tree)
    tick_names = _tick_taint(tree, ms_names)

    findings = []
    for n in ast.walk(tree):
        if not isinstance(n, ast.Call) or not _is_amy_call(n):
            continue
        for kw in n.keywords:
            if kw.arg not in ("time", "sequence"):
                continue
            v = kw.value
            snippet = lines[n.lineno - 1].strip() if n.lineno <= len(lines) else ""
            # Locate the exact `time=`/`sequence=` kwarg source span.
            col = getattr(kw, "col_offset", v.col_offset)
            end_col = v.end_col_offset
            lineno = getattr(kw, "lineno", v.lineno)

            if kw.arg == "sequence":
                cls, note = "SEQUENCE_RENAME", "sequence= -> ticks= (values unchanged)"
            elif isinstance(v, ast.Constant) and v.value is None:
                cls, note = "SEQUENCE_RENAME", "time=None -> ticks=None (no-op default)"
            elif isinstance(v, ast.Constant) and isinstance(v.value, (int, float)):
                cls, note = "PAST_LITERAL", f"absolute ms {v.value!r} -- already past, drop kwarg"
            elif _direct_ms_call(v):
                cls, note = "MS_CONVERT", "ms offset from AMY's ms clock -> tick conversion"
            elif any(isinstance(x, ast.Name) and x.id in ms_names for x in ast.walk(v)):
                cls, note = "MS_CONVERT", "derived from AMY's ms clock -> tick conversion"
            elif _is_past_tick_expr(v, tick_names, ms_names):
                nm = next(x.id for x in ast.walk(v) if isinstance(x, ast.Name))
                cls, note = "CALLBACK_DROP", f"sequencer callback tick `{nm}` -- drop kwarg, play now"
            else:
                cls, note = "REVIEW", "cannot determine ms-vs-tick intent"
            findings.append(Finding(cls, kw.arg, lineno, col, end_col, snippet, note))

    return findings, None


def _tick_helpers(src, scope):
    """Pick the tick-clock and ms-clock spellings to emit for this sketch.

    Sketches can't be judged by their imports -- both firmwares preload `tulip`
    and `amy` into the REPL, so plenty of world sketches call `tulip.x()` with
    no import line at all. Follow the shipped precedent per world instead:
    AMYboard's bundled sketches use `amy.sequencer_ticks()`, Tulip's bundled
    examples and docs use `tulip.seq_ticks()`. For the millisecond clock, reuse
    whatever spelling the sketch itself already uses, so we never introduce a
    name the sketch's environment might not have.
    """
    seq = ("amy.sequencer_ticks()" if scope == "amyboardworld"
           else "tulip.seq_ticks()")
    m = re.search(r"\b(?:tulip\.amy_ticks_ms|amy\.ticks_ms|amy_ticks_ms)\(\)", src)
    ms = m.group(0) if m else (
        "amy.ticks_ms()" if scope == "amyboardworld" else "tulip.amy_ticks_ms()")
    return seq, ms


def rewrite(src, findings, scope="tulipworld"):
    """Apply auto-migratable findings. Returns (new_src, applied, skipped)."""
    lines = src.split("\n")
    # Apply bottom-up so earlier spans keep their offsets.
    todo = sorted((f for f in findings if f.cls in AUTO_CLASSES),
                  key=lambda f: (f.lineno, f.col), reverse=True)
    skipped = [f for f in findings if f.cls not in AUTO_CLASSES]
    applied = []
    needs_const = False
    blanked = set()
    seq_ticks, ticks_ms = _tick_helpers(src, scope)

    for f in todo:
        line = lines[f.lineno - 1]
        old = line[f.col:f.end_col]
        if f.cls == "SEQUENCE_RENAME":
            new = re.sub(r"^\s*(time|sequence)\s*=", "ticks=", old)
        elif f.cls == "MS_CONVERT":
            expr = re.sub(r"^\s*time\s*=\s*", "", old)
            expr = re.sub(r"\b(tulip\.|amy\.)?amy_ticks_ms\(\)|\bamy\.ticks_ms\(\)",
                          "__MSBASE__", expr)
            if "__MSBASE__" in expr:
                # `amy_ticks_ms() + N` is "N ms from now" -> N ms worth of ticks
                # from now. Substituting 0 for the ms clock leaves just the offset.
                expr = expr.replace("__MSBASE__", "0")
                new = f"ticks=round({seq_ticks} + ({expr}) * {TICKS_PER_MS_EXPR})"
            else:
                # A variable captured from the ms clock earlier: convert the
                # ms still remaining between now and the target into ticks.
                new = (f"ticks=round({seq_ticks} + "
                       f"(({expr}) - {ticks_ms}) * {TICKS_PER_MS_EXPR})")
            needs_const = True
        else:  # CALLBACK_DROP / PAST_LITERAL -- remove the kwarg entirely.
            before, after = line[:f.col], line[f.end_col:]
            # Also swallow a trailing or leading comma so the call stays valid.
            m = re.match(r"\s*,\s*", after)
            if m:
                after = after[m.end():]
            else:
                before = re.sub(r",\s*$", "", before)
            lines[f.lineno - 1] = before + after
            # A kwarg that had a whole line to itself leaves a blank line in
            # the middle of the call; drop it rather than leave the litter.
            if not lines[f.lineno - 1].strip():
                blanked.add(f.lineno - 1)
            applied.append(f)
            continue
        lines[f.lineno - 1] = line[:f.col] + new + line[f.end_col:]
        applied.append(f)

    if blanked:
        lines = [l for i, l in enumerate(lines) if i not in blanked]
    out = "\n".join(lines)
    if needs_const and TICKS_PER_MS_EXPR not in src:
        out = _insert_const(out)

    # Never hand back source we just broke.
    try:
        ast.parse(out)
    except SyntaxError as e:
        raise RewriteError(f"rewrite produced invalid python: line {e.lineno}: {e.msg}")
    return out, applied, skipped


class RewriteError(Exception):
    pass


def _insert_const(src):
    """Insert the TICKS_PER_MS definition after the last top-level import."""
    lines = src.split("\n")
    last_import = 0
    try:
        for n in ast.parse(src).body:
            if isinstance(n, (ast.Import, ast.ImportFrom)):
                last_import = n.end_lineno
    except SyntaxError:
        pass
    lines.insert(last_import, TICKS_PER_MS_DEF)
    return "\n".join(lines)


# ── Item handling (a world item is a .py, or a .tar of .py) ───────────────────

def sources_from_blob(filename, blob):
    """Yield (member_name_or_None, text) for every python source in a blob."""
    low = filename.lower()
    if low.endswith(".py"):
        yield None, blob.decode("utf-8", "replace")
    elif low.endswith(".tar"):
        with tarfile.open(fileobj=io.BytesIO(blob)) as t:
            for m in t.getmembers():
                if m.isfile() and m.name.lower().endswith(".py"):
                    yield m.name, t.extractfile(m).read().decode("utf-8", "replace")


def repack_tar(blob, replacements):
    """Rebuild a .tar with `replacements` {member_name: new_text} applied."""
    out = io.BytesIO()
    with tarfile.open(fileobj=io.BytesIO(blob)) as src, \
            tarfile.open(fileobj=out, mode="w") as dst:
        for m in src.getmembers():
            data = src.extractfile(m).read() if m.isfile() else None
            if m.name in replacements:
                data = replacements[m.name].encode("utf-8")
                m.size = len(data)
            dst.addfile(m, io.BytesIO(data) if data is not None else None)
    return out.getvalue()


def process_item(api_url, item, scope, source_item=None):
    """Download + analyze one world item. Returns a result dict or None.

    `source_item` lets the content come from a different (older) row than the
    one we'd re-upload against -- used by --recover-missing for items whose
    latest blob is gone from the volume.
    """
    src_item = source_item or item
    try:
        blob = download(api_url, scope, src_item["id"])
    except HTTPError as e:
        kind = "missing_blob" if e.code == 404 else "download_error"
        return {"item": item, "scope": scope, "error": f"download {e.code}",
                "error_kind": kind}

    per_member, parse_errors = {}, []
    try:
        srcs = list(sources_from_blob(src_item["filename"], blob))
    except Exception as e:
        return {"item": item, "scope": scope, "error": f"unpack: {e}",
                "error_kind": "unpack_error"}

    for name, text in srcs:
        findings, err = analyze(text, name or item["filename"])
        if err:
            # CPython rejects source MicroPython accepts (mixed tabs/spaces is
            # the common one). We can't classify it, but we must not silently
            # call it clean -- raw-grep it so a real hit still gets reported.
            raw = len(re.findall(r"\b(?:time|sequence)\s*=", text))
            parse_errors.append(
                f"{name or item['filename']}: {err}"
                + (f"  [{raw} raw time=/sequence= hit(s) -- NEEDS MANUAL CHECK]"
                   if raw else "  [no time=/sequence= text present -- nothing to migrate]"))
            continue
        if findings:
            per_member[name] = (text, findings)

    if not per_member and not parse_errors:
        return None
    return {"item": item, "scope": scope, "blob": blob, "members": per_member,
            "parse_errors": parse_errors,
            "recovered_from": src_item["id"] if source_item else None}


def build_migration(result):
    """Compute rewritten content for a result. Adds 'new_blob'/'applied'/'skipped'."""
    applied, skipped, new_texts = [], [], {}
    for name, (text, findings) in result["members"].items():
        try:
            new_text, ap, sk = rewrite(text, findings, result["scope"])
        except RewriteError as e:
            result["error"] = str(e)
            result["error_kind"] = "rewrite_error"
            result["applied"], result["skipped"] = [], findings
            result["new_blob"] = None
            return result
        applied += ap
        skipped += sk
        if new_text != text:
            new_texts[name] = new_text
    result["applied"], result["skipped"] = applied, skipped
    if not new_texts:
        result["new_blob"] = None
        return result
    if result["item"]["filename"].lower().endswith(".py"):
        result["new_blob"] = new_texts[None].encode("utf-8")
    else:
        result["new_blob"] = repack_tar(result["blob"], new_texts)
    return result


# ── Reporting ─────────────────────────────────────────────────────────────────

def print_report(results, verbose=False, scanned=None):
    from collections import Counter
    cls_counts, scope_counts = Counter(), Counter()
    changed, review_only, errored, parse_failed = [], [], [], []

    for r in results:
        if r.get("parse_errors"):
            parse_failed.append(r)
        if r.get("error"):
            errored.append(r)
            continue
        for f in r.get("applied", []) + r.get("skipped", []):
            cls_counts[f.cls] += 1
        if r.get("new_blob"):
            changed.append(r)
            scope_counts[r["scope"]] += 1
        elif r.get("skipped"):
            review_only.append(r)

    print("=" * 78)
    print("TICKS MIGRATION REPORT -- Tulip World + AMYboard World")
    print("=" * 78)
    if scanned:
        print("\nScanned (latest version of each user/filename):")
        for s, n in scanned.items():
            print(f"    {s:16s} {n} code items")
    print(f"\nItems that would be REWRITTEN and re-uploaded:  {len(changed)}")
    for s in SCOPES:
        print(f"    {s:16s} {scope_counts[s]}")
    print(f"Items needing MANUAL review (no safe auto-fix): {len(review_only)}")
    print(f"Items whose blob is missing / undownloadable:   "
          f"{sum(1 for r in errored if r.get('error_kind') == 'missing_blob')}")
    print(f"Items that failed to parse as python:           {len(parse_failed)}")
    other = [r for r in errored if r.get("error_kind") != "missing_blob"]
    if other:
        print(f"Other errors:                                  {len(other)}")

    print("\nAffected call sites by class:")
    for c in CLASSES:
        if cls_counts[c]:
            auto = "auto-migrated" if c in AUTO_CLASSES else "MANUAL ONLY"
            print(f"    {c:16s} {cls_counts[c]:4d}   [{auto}]")

    print("\n--- Items to be rewritten " + "-" * 51)
    if not changed:
        print("  (none)")
    for r in sorted(changed, key=lambda r: (r["scope"], r["item"]["username"].lower())):
        it = r["item"]
        by = Counter(f.cls for f in r["applied"])
        extra = f"  (+{len(r['skipped'])} REVIEW)" if r["skipped"] else ""
        rec = f"  [recovered from id={r['recovered_from']}]" if r.get("recovered_from") else ""
        print(f"  [{r['scope'][:3]}] {it['username']}/{it['filename']} "
              f"id={it['id']}  {dict(by)}{extra}{rec}")
        if verbose:
            for f in r["applied"]:
                print(f"        L{f.lineno:<4d} {f.cls:15s} {f.snippet[:88]}")
                print(f"              -> {f.note}")

    if review_only:
        print("\n--- Manual review only " + "-" * 54)
        for r in sorted(review_only, key=lambda r: r["item"]["username"].lower()):
            it = r["item"]
            print(f"  [{r['scope'][:3]}] {it['username']}/{it['filename']} id={it['id']}")
            for f in r["skipped"]:
                print(f"        L{f.lineno:<4d} {f.snippet[:88]}")
                print(f"              {f.note}")

    if parse_failed:
        print("\n--- Failed to parse " + "-" * 57)
        for r in parse_failed:
            it = r["item"]
            for e in r["parse_errors"]:
                print(f"  [{r['scope'][:3]}] {it['username']}/{it['filename']} "
                      f"id={it['id']}: {e}")

    missing = [r for r in errored if r.get("error_kind") == "missing_blob"]
    if missing:
        print("\n--- Missing blobs (cannot scan; pre-existing data loss) " + "-" * 21)
        for r in sorted(missing, key=lambda r: r["item"]["id"]):
            it = r["item"]
            print(f"  [{r['scope'][:3]}] {it['username']}/{it['filename']} id={it['id']}")
    if other:
        print("\n--- Other errors " + "-" * 60)
        for r in other:
            it = r["item"]
            print(f"  [{r['scope'][:3]}] {it['username']}/{it['filename']} "
                  f"id={it['id']}: {r['error']}")
    print()
    return changed


# ── Main ──────────────────────────────────────────────────────────────────────

def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--url", default=os.environ.get("WORLD_API_URL", DEFAULT_URL))
    ap.add_argument("--report", action="store_true", help="scan and report only")
    ap.add_argument("--apply", action="store_true", help="re-upload rewritten sketches")
    ap.add_argument("--verbose", action="store_true", help="per-call-site detail")
    ap.add_argument("--scope", choices=SCOPES, help="limit to one world")
    ap.add_argument("--write-diffs", metavar="DIR", help="dump before/after sources")
    ap.add_argument("--limit", type=int, help="stop after N changed items (testing)")
    ap.add_argument("--exclude-user", action="append", default=[], metavar="NAME",
                    help="skip this username (repeatable). Use for `shorepine`, "
                         "whose official sketches are published from the repo by "
                         "amyboardweb/sketches/deploy_sketches.py instead.")
    ap.add_argument("--recover-missing", action="store_true",
                    help="for items whose latest blob is missing from the volume, "
                         "scan the newest readable older version instead. This "
                         "RESTORES content, not just migrates it -- opt in knowingly.")
    args = ap.parse_args()

    if not args.report and not args.apply:
        ap.error("pass --report or --apply")

    scopes = [args.scope] if args.scope else list(SCOPES)
    excluded = {u.lower() for u in args.exclude_user}
    results, scanned = [], {}
    for scope in scopes:
        items = list_items(args.url, scope, latest_only=True)
        code = [i for i in items if i["filename"].lower().endswith((".py", ".tar"))
                and i["username"].lower() not in excluded]
        if excluded:
            skipped_n = sum(1 for i in items
                            if i["filename"].lower().endswith((".py", ".tar"))
                            and i["username"].lower() in excluded)
            if skipped_n:
                print(f"  skipping {skipped_n} {scope} items from "
                      f"{sorted(excluded)}", file=sys.stderr)
        scanned[scope] = len(code)
        print(f"scanning {scope}: {len(code)} latest code items", file=sys.stderr)

        siblings = {}
        if args.recover_missing:
            allv = list_items(args.url, scope, latest_only=False)
            for v in allv:
                siblings.setdefault(
                    (v["username"].lower(), v["filename"].lower()), []).append(v)

        for i, it in enumerate(code):
            r = process_item(args.url, it, scope)
            if r and r.get("error_kind") == "missing_blob" and args.recover_missing:
                key = (it["username"].lower(), it["filename"].lower())
                older = sorted((v for v in siblings.get(key, []) if v["id"] != it["id"]),
                               key=lambda v: v["time"], reverse=True)
                for cand in older:
                    r2 = process_item(args.url, it, scope, source_item=cand)
                    if r2 is None or r2.get("error_kind") != "missing_blob":
                        r = r2
                        break
            if r:
                results.append(build_migration(r) if not r.get("error") else r)
            if i and i % 50 == 0:
                print(f"  {i}/{len(code)}", file=sys.stderr)

    changed = print_report(results, verbose=args.verbose, scanned=scanned)

    if args.write_diffs:
        os.makedirs(args.write_diffs, exist_ok=True)
        for r in changed:
            it = r["item"]
            stem = f"{r['scope']}-{it['id']}-{it['username']}-{it['filename']}"
            for name, (text, _) in r["members"].items():
                suffix = ("-" + name.replace("/", "_")) if name else ""
                with open(os.path.join(args.write_diffs, stem + suffix + ".before"), "w") as f:
                    f.write(text)
            nb = r["new_blob"]
            if it["filename"].lower().endswith(".py"):
                with open(os.path.join(args.write_diffs, stem + ".after"), "wb") as f:
                    f.write(nb)
            else:
                for name, text in [(n, t) for n, t in
                                   sources_from_blob(it["filename"], nb)]:
                    if name in r["members"]:
                        p = stem + "-" + name.replace("/", "_") + ".after"
                        with open(os.path.join(args.write_diffs, p), "w") as f:
                            f.write(text)
        print(f"wrote before/after sources to {args.write_diffs}")

    if not args.apply:
        return 0

    token = os.environ.get("WORLD_ADMIN_TOKEN")
    ok = fail = 0
    for r in (changed[:args.limit] if args.limit else changed):
        it = r["item"]
        try:
            resp = upload(args.url, r["scope"], it["username"], it["filename"],
                          it.get("description", ""), r["new_blob"],
                          int(it["time"]) + 1, admin_token=token)
            print(f"  uploaded {r['scope']} {it['username']}/{it['filename']} "
                  f"-> id={resp.get('id')}")
            ok += 1
        except HTTPError as e:
            print(f"  FAILED {r['scope']} {it['username']}/{it['filename']}: "
                  f"{e.code} {e.read()[:200]!r}", file=sys.stderr)
            fail += 1
    print(f"\nuploaded {ok}, failed {fail}")
    return 1 if fail else 0


if __name__ == "__main__":
    sys.exit(main())
