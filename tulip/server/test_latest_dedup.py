#!/usr/bin/env python3
"""Tests for latest_per_user_env dedup when created_at_ms ties.

Uploads may carry an explicit created_at_ms (the Discord importer, backup
restores), so two rows for the same (username, filename) can tie exactly. The
listing must resolve that tie toward the newest row -- the highest id. Without
an `id DESC` tie-break SQLite orders ties by rowid ascending and the *oldest*
duplicate wins, which is what hid 78 Tulip World sketches behind dead rows
whose blobs were lost in the Feb 2026 Modal->Railway migration.

Run: python3 tulip/server/test_latest_dedup.py
(requires the server deps: fastapi, pydantic, requests).
"""
from __future__ import annotations

import os
import sys
import tempfile
from pathlib import Path

_TMP = Path(tempfile.mkdtemp(prefix="worlddedup-"))
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


def insert(table: str, item_id: int, username: str, filename: str, created_at_ms: int) -> None:
    cols = ("id, username, filename, description, tags_json, created_at_ms, "
            "size_bytes, sha256, blob_path, client_ip")
    vals = "?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
    if table == "environments":
        cols += ", item_type"
        vals += ", ?"
    row = [item_id, username, filename, "", "[]", created_at_ms, 1, "x",
           f"/blobs/{item_id:09d}-{filename}", ""]
    if table == "environments":
        row.append("environment")
    with m._open_db() as conn:
        conn.execute(f"INSERT INTO {table}({cols}) VALUES({vals})", row)
        conn.commit()


def latest_ids(lister, **extra) -> list[int]:
    # Called directly rather than through FastAPI, so every Query-defaulted
    # parameter has to be supplied explicitly.
    got = lister(limit=500, q="", tag="", username="", latest_per_user_env=True,
                 **extra)
    return [i["id"] for i in got["items"]]


def main() -> int:
    m._ensure_schema()

    # Exact tie: the Discord importer ran twice and backdated both runs, so the
    # dead run-1 row and the live run-2 row share a created_at_ms to the ms.
    tie = 1771171140000
    insert("tulip_files", 232, "bwhitman", "cooleditor.py", tie)
    insert("tulip_files", 467, "bwhitman", "cooleditor.py", tie)
    ids = latest_ids(m.list_tulip_files)
    check("tulip: exact created_at_ms tie -> highest id wins", ids == [467])

    # A genuinely newer row must still win even with a lower id, so the
    # tie-break must stay secondary to created_at_ms.
    insert("tulip_files", 12, "revaaron", "sm353.py", tie + 5000)
    insert("tulip_files", 900, "revaaron", "sm353.py", tie)
    ids = latest_ids(m.list_tulip_files)
    check("tulip: newer created_at_ms still beats a higher id",
          12 in ids and 900 not in ids)

    # Same guarantee on the AMYboard side (shared _list_file_rows code path).
    insert("environments", 40, "generator", "granular_synth.py", tie)
    insert("environments", 884, "generator", "granular_synth.py", tie)
    ids = latest_ids(m.list_amyboard_files, item_type="")
    check("amyboard: exact created_at_ms tie -> highest id wins", ids == [884])

    # Dedup must not drop or duplicate distinct sketches.
    insert("tulip_files", 500, "itd", "delay.py", tie)
    insert("tulip_files", 501, "itd", "euclid2.py", tie)
    ids = latest_ids(m.list_tulip_files)
    check("tulip: distinct (user, filename) pairs all survive dedup",
          len(ids) == len(set(ids)) == 4)

    print()
    if _failures:
        print(f"{_failures} FAILED")
        return 1
    print("all ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
