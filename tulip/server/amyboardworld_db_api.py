#!/usr/bin/env python3
"""DB-backed AMYboard World API.

This service is designed as a migration target away from Discord-backed storage.
It stores uploaded tar files on disk and metadata in SQLite.
"""

from __future__ import annotations

import hashlib
import json
import os
import re
import shutil
import sqlite3
import time
from pathlib import Path
from typing import Any

from fastapi import Depends, FastAPI, File, Form, Header, HTTPException, Query, UploadFile
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from pydantic import BaseModel

USERNAME_RE = re.compile(r"^[A-Za-z0-9]{1,20}$")
ENV_NAME_RE = re.compile(r"^[A-Za-z0-9_-]{1,20}$")
TAG_RE = re.compile(r"^[A-Za-z0-9_-]{1,32}$")
MAX_DESCRIPTION = 400
MAX_FILE_BYTES = 10 * 1024 * 1024

DB_PATH = Path(os.getenv("AMYBOARDWORLD_DB_PATH", "./amyboardworld.db")).expanduser().resolve()
FILES_DIR = Path(os.getenv("AMYBOARDWORLD_FILES_DIR", "./amyboardworld_files")).expanduser().resolve()
ADMIN_TOKEN = os.getenv("AMYBOARDWORLD_ADMIN_TOKEN", "")
ADMIN_UI_PATH = Path(__file__).with_name("amyboardworld_admin.html")

app = FastAPI(title="AMYboard World DB API", version="0.1.0")
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)


def _now_ms() -> int:
    return int(time.time() * 1000)


def _open_db() -> sqlite3.Connection:
    conn = sqlite3.connect(str(DB_PATH))
    conn.row_factory = sqlite3.Row
    return conn


def _ensure_schema() -> None:
    FILES_DIR.mkdir(parents=True, exist_ok=True)
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)
    with _open_db() as conn:
        conn.executescript(
            """
            PRAGMA journal_mode=WAL;

            CREATE TABLE IF NOT EXISTS environments (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              username TEXT NOT NULL,
              filename TEXT NOT NULL,
              description TEXT NOT NULL,
              tags_json TEXT NOT NULL DEFAULT '[]',
              created_at_ms INTEGER NOT NULL,
              deleted_at_ms INTEGER,
              size_bytes INTEGER NOT NULL,
              sha256 TEXT NOT NULL,
              blob_path TEXT NOT NULL
            );

            CREATE INDEX IF NOT EXISTS idx_env_created_at ON environments(created_at_ms DESC);
            CREATE INDEX IF NOT EXISTS idx_env_username ON environments(username);
            CREATE INDEX IF NOT EXISTS idx_env_filename ON environments(filename);
            CREATE INDEX IF NOT EXISTS idx_env_deleted ON environments(deleted_at_ms);
            """
        )


@app.on_event("startup")
def _startup() -> None:
    _ensure_schema()


def _require_admin(x_admin_token: str | None = Header(default=None)) -> None:
    if not ADMIN_TOKEN:
        raise HTTPException(status_code=503, detail="Admin moderation token not configured")
    if not x_admin_token or x_admin_token != ADMIN_TOKEN:
        raise HTTPException(status_code=403, detail="Admin token required")


def _parse_tags(raw: Any) -> list[str]:
    if raw is None:
        return []
    if isinstance(raw, str):
        if not raw.strip():
            return []
        try:
            parsed = json.loads(raw)
        except json.JSONDecodeError:
            parsed = [part.strip() for part in raw.split(",") if part.strip()]
    elif isinstance(raw, list):
        parsed = raw
    else:
        parsed = []

    out: list[str] = []
    seen: set[str] = set()
    for tag in parsed:
        tag_s = str(tag).strip().lower()
        if not tag_s:
            continue
        if not TAG_RE.match(tag_s):
            raise HTTPException(status_code=400, detail=f"Invalid tag: {tag_s}")
        if tag_s in seen:
            continue
        seen.add(tag_s)
        out.append(tag_s)
    return out


def _public_row(row: sqlite3.Row) -> dict[str, Any]:
    tags = _parse_tags(row["tags_json"])
    env_id = int(row["id"])
    return {
        "id": env_id,
        "username": row["username"],
        "filename": row["filename"],
        "description": row["description"],
        "tags": tags,
        "time": int(row["created_at_ms"]),
        "size": int(row["size_bytes"]),
        "download_url": f"/api/amyboardworld/files/{env_id}/download",
    }


@app.get("/health")
def health() -> dict[str, Any]:
    return {"ok": True, "ts": _now_ms()}


@app.get("/admin")
def admin_ui() -> FileResponse:
    if not ADMIN_UI_PATH.exists():
        raise HTTPException(status_code=404, detail="Admin UI not found")
    return FileResponse(str(ADMIN_UI_PATH), media_type="text/html; charset=utf-8")


@app.post("/api/amyboardworld/upload")
async def upload_environment(
    username: str = Form(...),
    description: str = Form(...),
    file: UploadFile = File(...),
) -> dict[str, Any]:
    username = (username or "").strip()
    if not USERNAME_RE.match(username):
        raise HTTPException(status_code=400, detail="Username must be 1-20 chars: A-Z, a-z, 0-9")

    description = " ".join((description or "").split()).strip()
    if len(description) < 1 or len(description) > MAX_DESCRIPTION:
        raise HTTPException(status_code=400, detail="Description must be 1-400 chars")

    filename = (file.filename or "").strip()
    if not filename.lower().endswith(".tar"):
        raise HTTPException(status_code=400, detail="File must be a .tar")

    env_name = filename[:-4]
    if not ENV_NAME_RE.match(env_name):
        raise HTTPException(status_code=400, detail="Environment name must be 1-20 chars: A-Z, a-z, 0-9, -, _")

    contents = await file.read()
    if len(contents) < 1:
        raise HTTPException(status_code=400, detail="Empty upload")
    if len(contents) > MAX_FILE_BYTES:
        raise HTTPException(status_code=413, detail=f"File too large (max {MAX_FILE_BYTES} bytes)")

    created_at_ms = _now_ms()
    digest = hashlib.sha256(contents).hexdigest()

    with _open_db() as conn:
        cur = conn.execute(
            """
            INSERT INTO environments(username, filename, description, tags_json, created_at_ms, size_bytes, sha256, blob_path)
            VALUES(?, ?, ?, ?, ?, ?, ?, ?)
            """,
            (username, filename, description, "[]", created_at_ms, len(contents), digest, ""),
        )
        env_id = int(cur.lastrowid)
        safe_filename = f"{env_id:09d}-{filename}"
        blob_path = FILES_DIR / safe_filename
        blob_path.write_bytes(contents)
        conn.execute("UPDATE environments SET blob_path = ? WHERE id = ?", (str(blob_path), env_id))
        conn.commit()

    return {"ok": True, "id": env_id}


@app.get("/api/amyboardworld/files")
def list_files(
    limit: int = Query(default=100, ge=1, le=500),
    q: str = Query(default=""),
    tag: str = Query(default=""),
    username: str = Query(default=""),
    latest_per_user_env: bool = Query(default=True),
) -> dict[str, Any]:
    clauses = ["deleted_at_ms IS NULL"]
    params: list[Any] = []

    q_s = q.strip().lower()
    if q_s:
        like = f"%{q_s}%"
        clauses.append("(lower(username) LIKE ? OR lower(filename) LIKE ? OR lower(description) LIKE ? OR lower(tags_json) LIKE ?)")
        params.extend([like, like, like, like])

    username_s = username.strip().lower()
    if username_s:
        clauses.append("lower(username) = ?")
        params.append(username_s)

    tag_s = tag.strip().lower()
    if tag_s:
        if not TAG_RE.match(tag_s):
            raise HTTPException(status_code=400, detail="Invalid tag")
        clauses.append("lower(tags_json) LIKE ?")
        params.append(f'%"{tag_s}"%')

    where_sql = " AND ".join(clauses)

    with _open_db() as conn:
        rows = conn.execute(
            f"""
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes
            FROM environments
            WHERE {where_sql}
            ORDER BY created_at_ms DESC
            LIMIT ?
            """,
            [*params, max(limit * 4, limit)],
        ).fetchall()

    if latest_per_user_env:
        deduped: list[sqlite3.Row] = []
        seen: set[tuple[str, str]] = set()
        for row in rows:
            key = (str(row["username"]).lower(), str(row["filename"]).lower())
            if key in seen:
                continue
            seen.add(key)
            deduped.append(row)
            if len(deduped) >= limit:
                break
        rows = deduped
    else:
        rows = rows[:limit]

    return {"items": [_public_row(row) for row in rows]}


@app.get("/api/amyboardworld/files/{env_id}")
def get_file(env_id: int) -> dict[str, Any]:
    with _open_db() as conn:
        row = conn.execute(
            """
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes
            FROM environments
            WHERE id = ? AND deleted_at_ms IS NULL
            """,
            (env_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    return _public_row(row)


@app.get("/api/amyboardworld/files/{env_id}/download")
def download_file(env_id: int) -> FileResponse:
    with _open_db() as conn:
        row = conn.execute(
            "SELECT filename, blob_path FROM environments WHERE id = ? AND deleted_at_ms IS NULL",
            (env_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")

    blob_path = Path(str(row["blob_path"]))
    if not blob_path.exists():
        raise HTTPException(status_code=404, detail="File missing")
    return FileResponse(str(blob_path), media_type="application/x-tar", filename=str(row["filename"]))


class TagsPatch(BaseModel):
    tags: list[str]


@app.patch("/api/amyboardworld/files/{env_id}/tags", dependencies=[Depends(_require_admin)])
def patch_tags(env_id: int, body: TagsPatch) -> dict[str, Any]:
    tags = _parse_tags(body.tags)
    with _open_db() as conn:
        cur = conn.execute(
            "UPDATE environments SET tags_json = ? WHERE id = ? AND deleted_at_ms IS NULL",
            (json.dumps(tags), env_id),
        )
        conn.commit()
        if cur.rowcount < 1:
            raise HTTPException(status_code=404, detail="Not found")
    return {"ok": True, "id": env_id, "tags": tags}


@app.delete("/api/amyboardworld/files/{env_id}", dependencies=[Depends(_require_admin)])
def delete_file(env_id: int) -> dict[str, Any]:
    deleted_at_ms = _now_ms()
    with _open_db() as conn:
        row = conn.execute("SELECT blob_path FROM environments WHERE id = ? AND deleted_at_ms IS NULL", (env_id,)).fetchone()
        if not row:
            raise HTTPException(status_code=404, detail="Not found")
        conn.execute("UPDATE environments SET deleted_at_ms = ? WHERE id = ?", (deleted_at_ms, env_id))
        conn.commit()

    blob_path = Path(str(row["blob_path"]))
    if blob_path.exists():
        try:
            if blob_path.is_file():
                blob_path.unlink()
            elif blob_path.is_dir():
                shutil.rmtree(blob_path)
        except OSError:
            pass
    return {"ok": True, "id": env_id}
