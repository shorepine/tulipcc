#!/usr/bin/env python3
"""Carry tags forward from superseded World sketch versions to the latest one.

The migration scripts (migrate_world_loop_arg.py, migrate_world_ticks.py,
migrate_world_drum_amp.py) historically re-uploaded each rewritten sketch as a
brand-new version without copying the old row's tags. Tag filters like
``?tag=featured`` then matched only the stale row, so the #featured chip served
the pre-migration source (``def loop():``) while the plain listing served the
migrated one. This script finds every (scope, username, filename) group with more
than one live version and PATCHes the union of the older rows' tags onto the
newest row.

    python3 backfill_world_tags.py            # report only
    python3 backfill_world_tags.py --apply    # write via the admin tags endpoint

The admin token is read from AMYBOARDWORLD_ADMIN_TOKEN (or WORLD_ADMIN_TOKEN);
run under ``railway run`` to have Railway inject it.
"""
from __future__ import annotations

import argparse
import json
import os
import sys
from collections import defaultdict
from urllib.request import Request, urlopen

DEFAULT_URL = "https://tulipcc-production.up.railway.app"
SCOPES = ("amyboardworld", "tulipworld")
# Injected at serialization time for OFFICIAL_USERNAMES; never stored.
VIRTUAL_TAGS = {"official"}


def fetch_all(api_url: str, scope: str) -> list[dict]:
    url = f"{api_url}/api/{scope}/files?limit=5000&latest_per_user_env=false"
    with urlopen(url, timeout=60) as r:
        return json.load(r)["items"]


def plan(items: list[dict]) -> list[dict]:
    groups: dict[tuple[str, str], list[dict]] = defaultdict(list)
    for it in items:
        groups[(it["username"].lower(), it["filename"].lower())].append(it)
    out = []
    for key, rows in groups.items():
        if len(rows) < 2:
            continue
        rows.sort(key=lambda r: (r["time"], r["id"]), reverse=True)
        latest, older = rows[0], rows[1:]
        have = [t for t in latest.get("tags", []) if t not in VIRTUAL_TAGS]
        want = list(have)
        for r in older:
            for t in r.get("tags", []):
                if t not in VIRTUAL_TAGS and t not in want:
                    want.append(t)
        if want != have:
            out.append({"id": latest["id"], "filename": latest["filename"],
                        "username": latest["username"], "from": [r["id"] for r in older],
                        "was": have, "tags": want})
    return out


def patch_tags(api_url: str, scope: str, item_id: int, tags: list[str], token: str) -> None:
    body = json.dumps({"tags": tags}).encode()
    req = Request(f"{api_url}/api/{scope}/files/{item_id}/tags", data=body, method="PATCH")
    req.add_header("Content-Type", "application/json")
    req.add_header("X-Admin-Token", token)
    with urlopen(req, timeout=60) as r:
        r.read()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--url", default=DEFAULT_URL)
    ap.add_argument("--scope", choices=SCOPES)
    ap.add_argument("--apply", action="store_true")
    opts = ap.parse_args()

    token = os.environ.get("AMYBOARDWORLD_ADMIN_TOKEN") or os.environ.get("WORLD_ADMIN_TOKEN")
    if opts.apply and not token:
        print("--apply needs AMYBOARDWORLD_ADMIN_TOKEN in the environment", file=sys.stderr)
        return 2

    total = 0
    for scope in (opts.scope,) if opts.scope else SCOPES:
        changes = plan(fetch_all(opts.url, scope))
        print(f"[{scope}] {len(changes)} latest rows missing tags from older versions")
        for c in changes:
            print(f"  #{c['id']} {c['username']}/{c['filename']}: {c['was']} -> {c['tags']} (from {c['from']})")
            if opts.apply:
                patch_tags(opts.url, scope, c["id"], c["tags"], token)
        total += len(changes)
    print(("applied" if opts.apply else "would apply") + f" {total} tag updates")
    return 0


if __name__ == "__main__":
    sys.exit(main())
