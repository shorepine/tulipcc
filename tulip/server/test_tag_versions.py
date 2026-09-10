#!/usr/bin/env python3
"""Tags follow the sketch, not one upload of it.

Regression tests for the Sep 2026 #featured bug: the loop(tick) migration
re-uploaded every World sketch as a new version without its tags, and the
listing applied ?tag= *before* resolving the latest version per
(username, filename). The #featured chip therefore served the old, tagged
row (`def loop():`) while the plain listing served the migrated one.

Two guarantees:
  1. A tag filter can only ever match the latest live version of a sketch.
  2. A new version uploaded through _insert_file_row inherits the previous
     version's tags (hardware tags are replaced when the caller detects them).

Run: python3 tulip/server/test_tag_versions.py
(requires the server deps: fastapi, pydantic, requests).
"""
from __future__ import annotations

import json
import os
import sys
import tempfile
from pathlib import Path

_TMP = Path(tempfile.mkdtemp(prefix="worldtags-"))
os.environ["AMYBOARDWORLD_DB_PATH"] = str(_TMP / "world.db")
os.environ["AMYBOARDWORLD_FILES_DIR"] = str(_TMP / "abw")
os.environ["TULIPWORLD_FILES_DIR"] = str(_TMP / "tw")

sys.path.insert(0, str(Path(__file__).resolve().parent))
import amyboardworld_db_api as m  # noqa: E402

_failures = 0


def check(name: str, cond: bool) -> None:
    global _failures
    print(f"[{'ok  ' if cond else 'FAIL'}] {name}")
    if not cond:
        _failures += 1


def insert(item_id: int, username: str, filename: str, created_at_ms: int,
           tags: list[str] = ()) -> None:
    with m._open_db() as conn:
        conn.execute(
            "INSERT INTO environments(id, username, filename, description, tags_json, "
            "created_at_ms, size_bytes, sha256, blob_path, client_ip, item_type) "
            "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            [item_id, username, filename, "", json.dumps(list(tags)), created_at_ms, 1,
             "x", f"/blobs/{item_id:09d}-{filename}", "", "environment"])
        conn.commit()


def ids(**kw) -> list[int]:
    args = dict(limit=500, q="", tag="", username="", latest_per_user_env=True,
                item_type="")
    args.update(kw)
    return [i["id"] for i in m.list_amyboard_files(**args)["items"]]


def tags_of(item_id: int) -> list[str]:
    with m._open_db() as conn:
        row = conn.execute("SELECT tags_json FROM environments WHERE id = ?",
                           (item_id,)).fetchone()
    return json.loads(row["tags_json"])


def main() -> int:
    m._ensure_schema()
    t0 = 1783515468941

    # --- 1. listing: the tag filter must not resurrect a superseded version.
    insert(920, "generator", "dx7_groove_128_bpm.py", t0, ["featured"])
    insert(1187, "generator", "dx7_groove_128_bpm.py", t0 + 10**9)  # migrated, untagged
    check("plain listing serves the latest version", ids() == [1187])
    check("?tag=featured does not surface the stale tagged version",
          920 not in ids(tag="featured"))
    check("?tag=featured on an untagged latest version finds nothing",
          ids(tag="featured") == [])
    check("?q= cannot match a stale version either",
          ids(q="dx7_groove") == [1187])

    # Tagging the latest version makes it show up under the chip.
    with m._open_db() as conn:
        conn.execute("UPDATE environments SET tags_json = ? WHERE id = 1187",
                     (json.dumps(["featured"]),))
        conn.commit()
    check("?tag=featured serves the latest version once it carries the tag",
          ids(tag="featured") == [1187])

    # Deleting the latest version falls back to the previous live one.
    with m._open_db() as conn:
        conn.execute("UPDATE environments SET deleted_at_ms = 1 WHERE id = 1187")
        conn.commit()
    check("soft-deleting the latest version reveals the previous one",
          ids() == [920] and ids(tag="featured") == [920])

    # latest_per_user_env=False still returns every live version.
    with m._open_db() as conn:
        conn.execute("UPDATE environments SET deleted_at_ms = NULL WHERE id = 1187")
        conn.commit()
    check("latest_per_user_env=False lists all live versions",
          sorted(ids(latest_per_user_env=False)) == [920, 1187])

    # --- 2. upload: a new version inherits the previous version's tags.
    insert(422, "FlowRain", "SuperSaw.py", t0, ["featured", "display"])
    new_id = m._insert_file_row("environments", m.AMYBOARD_FILES_DIR, "FlowRain",
                                "v2", "SuperSaw.py", b"def loop(tick):\n    pass\n")
    check("re-uploaded sketch inherits featured + hardware tags",
          tags_of(new_id) == ["featured", "display"])
    check("...and the new version is what ?tag=featured now serves",
          ids(tag="featured", username="flowrain") == [new_id])

    # Username lookup is case-insensitive, like the listing's dedup key.
    newer_id = m._insert_file_row("environments", m.AMYBOARD_FILES_DIR, "flowrain",
                                  "v3", "SuperSaw.py", b"x = 1\n")
    check("inheritance matches username case-insensitively",
          tags_of(newer_id) == ["featured", "display"])

    # Caller-supplied hardware tags replace inherited ones; other tags are kept.
    gen_id = m._insert_file_row("environments", m.AMYBOARD_FILES_DIR, "FlowRain",
                                "v4", "SuperSaw.py", b"x = 2\n", tags=["cv"])
    check("detected hardware tags replace inherited hardware tags, featured survives",
          tags_of(gen_id) == ["featured", "cv"])

    # A brand-new sketch with no prior version gets exactly the caller's tags.
    fresh = m._insert_file_row("environments", m.AMYBOARD_FILES_DIR, "generator",
                               "new", "brand_new.py", b"x = 3\n", tags=["encoder"])
    check("first version of a sketch carries only the supplied tags",
          tags_of(fresh) == ["encoder"])
    fresh2 = m._insert_file_row("environments", m.AMYBOARD_FILES_DIR, "someone",
                                "new", "plain.py", b"x = 4\n")
    check("first version with no tags is untagged", tags_of(fresh2) == [])

    print()
    if _failures:
        print(f"{_failures} FAILED")
        return 1
    print("all ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
