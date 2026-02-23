#!/usr/bin/env python3
"""DB-backed AMYboard World + Tulip World API.

This service stores uploaded files on disk and metadata in SQLite.
It provides separate storage for:
- AMYboard World files
- Tulip World files
- Tulip World text messages
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

import requests
from fastapi import Depends, FastAPI, File, Form, Header, HTTPException, Query, Request, UploadFile
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse
from pydantic import BaseModel

USERNAME_RE = re.compile(r"^[A-Za-z0-9]{1,20}$")
ENV_NAME_RE = re.compile(r"^[A-Za-z0-9_-]{1,20}$")
FILE_NAME_RE = re.compile(r"^[A-Za-z0-9._-]{1,80}$")
TAG_RE = re.compile(r"^[A-Za-z0-9_-]{1,32}$")
MAX_DESCRIPTION = 400
MAX_MESSAGE = 800
MAX_FILE_BYTES = 10 * 1024 * 1024

DB_PATH = Path(os.getenv("WORLD_DB_PATH", os.getenv("AMYBOARDWORLD_DB_PATH", "./amyboardworld.db"))).expanduser().resolve()
AMYBOARD_FILES_DIR = Path(
    os.getenv("AMYBOARDWORLD_FILES_DIR", "./amyboardworld_files")
).expanduser().resolve()
TULIP_FILES_DIR = Path(
    os.getenv("TULIPWORLD_FILES_DIR", "./tulipworld_files")
).expanduser().resolve()
ADMIN_TOKEN = os.getenv("WORLD_ADMIN_TOKEN", os.getenv("AMYBOARDWORLD_ADMIN_TOKEN", ""))
ADMIN_UI_PATH = Path(__file__).with_name("amyboardworld_admin.html")

DISCORD_AMYBOARD_CHANNEL_ID = os.getenv("AMYBOARDWORLD_DISCORD_CHANNEL_ID", "1353327346924388422")
DISCORD_TULIP_TEXT_CHANNEL_ID = os.getenv("TULIPWORLD_DISCORD_TEXT_CHANNEL_ID", "1239226672046407824")
DISCORD_TULIP_FILES_CHANNEL_ID = os.getenv("TULIPWORLD_DISCORD_FILES_CHANNEL_ID", "1239512482025050204")

# Keep compatibility with historical Tulip World Discord bot token usage.
_DISCORD_TOKEN_A = "MTIzOTIyNTc4NDU3NzgxODc0NQ.GjGMum"
_DISCORD_TOKEN_B = "KvPGzKZDr1phrId9iY7LMtIDgMNtI0om8MsWsA"
DISCORD_BOT_TOKEN = os.getenv("WORLD_DISCORD_BOT_TOKEN", os.getenv("AMYBOARDWORLD_DISCORD_BOT_TOKEN", f"{_DISCORD_TOKEN_A}.{_DISCORD_TOKEN_B}"))

app = FastAPI(title="World DB API", version="0.2.0")
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
    AMYBOARD_FILES_DIR.mkdir(parents=True, exist_ok=True)
    TULIP_FILES_DIR.mkdir(parents=True, exist_ok=True)
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

            CREATE TABLE IF NOT EXISTS tulip_files (
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

            CREATE INDEX IF NOT EXISTS idx_twf_created_at ON tulip_files(created_at_ms DESC);
            CREATE INDEX IF NOT EXISTS idx_twf_username ON tulip_files(username);
            CREATE INDEX IF NOT EXISTS idx_twf_filename ON tulip_files(filename);
            CREATE INDEX IF NOT EXISTS idx_twf_deleted ON tulip_files(deleted_at_ms);

            CREATE TABLE IF NOT EXISTS tulip_messages (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              username TEXT NOT NULL,
              content TEXT NOT NULL,
              created_at_ms INTEGER NOT NULL,
              deleted_at_ms INTEGER
            );

            CREATE INDEX IF NOT EXISTS idx_twm_created_at ON tulip_messages(created_at_ms DESC);
            CREATE INDEX IF NOT EXISTS idx_twm_username ON tulip_messages(username);
            CREATE INDEX IF NOT EXISTS idx_twm_deleted ON tulip_messages(deleted_at_ms);
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


def _normalize_username(raw: str) -> str:
    username = (raw or "").strip()
    if not USERNAME_RE.match(username):
        raise HTTPException(status_code=400, detail="Username must be 1-20 chars: A-Z, a-z, 0-9")
    return username


def _normalize_description(raw: str) -> str:
    description = " ".join((raw or "").split()).strip()
    if len(description) < 1 or len(description) > MAX_DESCRIPTION:
        raise HTTPException(status_code=400, detail="Description must be 1-400 chars")
    return description


def _normalize_message(raw: str) -> str:
    content = " ".join((raw or "").split()).strip()
    if len(content) < 1 or len(content) > MAX_MESSAGE:
        raise HTTPException(status_code=400, detail=f"Message must be 1-{MAX_MESSAGE} chars")
    return content


def _normalize_created_at_ms(raw: Any) -> int:
    if raw is None or raw == "":
        return _now_ms()
    try:
        value = int(raw)
    except (TypeError, ValueError):
        raise HTTPException(status_code=400, detail="created_at_ms must be an integer timestamp in milliseconds")
    if value < 1420070400000 or value > (_now_ms() + 60 * 60 * 1000):
        raise HTTPException(status_code=400, detail="created_at_ms out of allowed range")
    return value


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


def _validate_amyboard_upload(filename: str, contents: bytes) -> str:
    clean_name = (filename or "").strip()
    if not clean_name.lower().endswith(".tar"):
        raise HTTPException(status_code=400, detail="File must be a .tar")
    env_name = clean_name[:-4]
    if not ENV_NAME_RE.match(env_name):
        raise HTTPException(status_code=400, detail="Environment name must be 1-20 chars: A-Z, a-z, 0-9, -, _")
    if len(contents) < 1:
        raise HTTPException(status_code=400, detail="Empty upload")
    if len(contents) > MAX_FILE_BYTES:
        raise HTTPException(status_code=413, detail=f"File too large (max {MAX_FILE_BYTES} bytes)")
    return clean_name


def _validate_tulip_upload(filename: str, contents: bytes) -> str:
    clean_name = (filename or "").strip().split("/")[-1].split("\\")[-1]
    if not FILE_NAME_RE.match(clean_name):
        raise HTTPException(status_code=400, detail="Filename must be 1-80 chars: A-Z, a-z, 0-9, ., -, _")
    if len(contents) < 1:
        raise HTTPException(status_code=400, detail="Empty upload")
    if len(contents) > MAX_FILE_BYTES:
        raise HTTPException(status_code=413, detail=f"File too large (max {MAX_FILE_BYTES} bytes)")
    return clean_name


def _file_row_to_public(row: sqlite3.Row, scope: str) -> dict[str, Any]:
    file_id = int(row["id"])
    tags = _parse_tags(row["tags_json"])
    t = int(row["created_at_ms"])
    return {
        "id": file_id,
        "scope": scope,
        "kind": "file",
        "username": row["username"],
        "filename": row["filename"],
        "description": row["description"],
        # keep compatibility with legacy tulip world field name
        "content": row["description"],
        "tags": tags,
        "time": t,
        "age_ms": max(0, _now_ms() - t),
        "size": int(row["size_bytes"]),
        "download_url": f"/api/{scope}/files/{file_id}/download",
    }


def _message_row_to_public(row: sqlite3.Row) -> dict[str, Any]:
    now_ms = _now_ms()
    t = int(row["created_at_ms"])
    return {
        "id": int(row["id"]),
        "scope": "tulipworld",
        "kind": "message",
        "username": row["username"],
        "content": row["content"],
        "time": t,
        "age_ms": max(0, now_ms - t),
    }


def _discord_notify(channel_id: str, content: str) -> None:
    if not DISCORD_BOT_TOKEN or not channel_id:
        return
    url = f"https://discordapp.com/api/channels/{channel_id}/messages"
    headers = {
        "Authorization": f"Bot {DISCORD_BOT_TOKEN}",
        "User-Agent": "TulipCC/4.0 (https://tulip.computer, v0.1)",
        "Content-Type": "application/json",
    }
    try:
        requests.post(url, headers=headers, data=json.dumps({"content": content}), timeout=8)
    except requests.RequestException:
        return


def _insert_file_row(
    table: str,
    files_dir: Path,
    username: str,
    description: str,
    filename: str,
    contents: bytes,
    *,
    created_at_ms: int | None = None,
) -> int:
    ts_ms = _normalize_created_at_ms(created_at_ms)
    digest = hashlib.sha256(contents).hexdigest()
    with _open_db() as conn:
        cur = conn.execute(
            f"""
            INSERT INTO {table}(username, filename, description, tags_json, created_at_ms, size_bytes, sha256, blob_path)
            VALUES(?, ?, ?, ?, ?, ?, ?, ?)
            """,
            (username, filename, description, "[]", ts_ms, len(contents), digest, ""),
        )
        item_id = int(cur.lastrowid)
        safe_filename = f"{item_id:09d}-{filename}"
        blob_path = files_dir / safe_filename
        blob_path.write_bytes(contents)
        conn.execute(f"UPDATE {table} SET blob_path = ? WHERE id = ?", (str(blob_path), item_id))
        conn.commit()
    return item_id


def _list_file_rows(
    table: str,
    scope: str,
    *,
    limit: int,
    q: str,
    tag: str,
    username: str,
    latest_per_user_env: bool,
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
            FROM {table}
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

    return {"items": [_file_row_to_public(row, scope) for row in rows]}


class TagsPatch(BaseModel):
    tags: list[str]


@app.get("/health")
def health() -> dict[str, Any]:
    return {"ok": True, "ts": _now_ms()}


@app.get("/admin")
def admin_ui() -> FileResponse:
    if not ADMIN_UI_PATH.exists():
        raise HTTPException(status_code=404, detail="Admin UI not found")
    return FileResponse(str(ADMIN_UI_PATH), media_type="text/html; charset=utf-8")


@app.get("/api/world/debug/storage", dependencies=[Depends(_require_admin)])
def debug_storage() -> dict[str, Any]:
    db_exists = DB_PATH.exists()
    with _open_db() as conn:
        amy_count = int(conn.execute("SELECT COUNT(*) AS n FROM environments").fetchone()["n"])
        tulip_file_count = int(conn.execute("SELECT COUNT(*) AS n FROM tulip_files").fetchone()["n"])
        tulip_msg_count = int(conn.execute("SELECT COUNT(*) AS n FROM tulip_messages").fetchone()["n"])
    return {
        "db_path": str(DB_PATH),
        "db_exists": db_exists,
        "db_size_bytes": DB_PATH.stat().st_size if db_exists else 0,
        "amyboard_files_dir": str(AMYBOARD_FILES_DIR),
        "tulip_files_dir": str(TULIP_FILES_DIR),
        "amyboard_file_rows": amy_count,
        "tulip_file_rows": tulip_file_count,
        "tulip_message_rows": tulip_msg_count,
        "cwd": os.getcwd(),
    }


# Back-compat alias for existing amyb admin tooling.
@app.get("/api/amyboardworld/debug/storage", dependencies=[Depends(_require_admin)])
def debug_storage_legacy() -> dict[str, Any]:
    return debug_storage()


@app.post("/api/amyboardworld/upload")
async def upload_amyboard_environment(
    username: str = Form(...),
    description: str = Form(...),
    file: UploadFile = File(...),
    created_at_ms: int | None = Form(default=None),
) -> dict[str, Any]:
    user = _normalize_username(username)
    desc = _normalize_description(description)
    contents = await file.read()
    filename = _validate_amyboard_upload(file.filename or "", contents)
    item_id = _insert_file_row(
        "environments",
        AMYBOARD_FILES_DIR,
        user,
        desc,
        filename,
        contents,
        created_at_ms=created_at_ms,
    )
    _discord_notify(DISCORD_AMYBOARD_CHANNEL_ID, f"{user} ### {desc[:MAX_DESCRIPTION]} ## {filename} ({len(contents)} bytes)")
    return {"ok": True, "id": item_id}


@app.get("/api/amyboardworld/files")
def list_amyboard_files(
    limit: int = Query(default=100, ge=1, le=5000),
    q: str = Query(default=""),
    tag: str = Query(default=""),
    username: str = Query(default=""),
    latest_per_user_env: bool = Query(default=True),
) -> dict[str, Any]:
    return _list_file_rows(
        "environments",
        "amyboardworld",
        limit=limit,
        q=q,
        tag=tag,
        username=username,
        latest_per_user_env=latest_per_user_env,
    )


@app.get("/api/amyboardworld/files/{item_id}")
def get_amyboard_file(item_id: int) -> dict[str, Any]:
    with _open_db() as conn:
        row = conn.execute(
            """
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes
            FROM environments
            WHERE id = ? AND deleted_at_ms IS NULL
            """,
            (item_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    return _file_row_to_public(row, "amyboardworld")


@app.get("/api/amyboardworld/files/{item_id}/download")
def download_amyboard_file(item_id: int) -> FileResponse:
    with _open_db() as conn:
        row = conn.execute(
            "SELECT filename, blob_path FROM environments WHERE id = ? AND deleted_at_ms IS NULL",
            (item_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    blob_path = Path(str(row["blob_path"]))
    if not blob_path.exists():
        raise HTTPException(status_code=404, detail="File missing")
    return FileResponse(str(blob_path), media_type="application/x-tar", filename=str(row["filename"]))


@app.patch("/api/amyboardworld/files/{item_id}/tags", dependencies=[Depends(_require_admin)])
def patch_amyboard_tags(item_id: int, body: TagsPatch) -> dict[str, Any]:
    tags = _parse_tags(body.tags)
    with _open_db() as conn:
        cur = conn.execute(
            "UPDATE environments SET tags_json = ? WHERE id = ? AND deleted_at_ms IS NULL",
            (json.dumps(tags), item_id),
        )
        conn.commit()
        if cur.rowcount < 1:
            raise HTTPException(status_code=404, detail="Not found")
    return {"ok": True, "id": item_id, "tags": tags}


@app.delete("/api/amyboardworld/files/{item_id}", dependencies=[Depends(_require_admin)])
def delete_amyboard_file(item_id: int) -> dict[str, Any]:
    return _delete_file_row("environments", item_id)


@app.post("/api/tulipworld/upload")
async def upload_tulip_file(
    username: str = Form(...),
    description: str = Form(...),
    file: UploadFile = File(...),
    created_at_ms: int | None = Form(default=None),
) -> dict[str, Any]:
    user = _normalize_username(username)
    desc = _normalize_description(description)
    contents = await file.read()
    filename = _validate_tulip_upload(file.filename or "", contents)
    item_id = _insert_file_row(
        "tulip_files",
        TULIP_FILES_DIR,
        user,
        desc,
        filename,
        contents,
        created_at_ms=created_at_ms,
    )
    _discord_notify(DISCORD_TULIP_FILES_CHANNEL_ID, f"{user} ### {desc[:MAX_DESCRIPTION]} ## {filename} ({len(contents)} bytes)")
    return {"ok": True, "id": item_id}


@app.get("/api/tulipworld/files")
def list_tulip_files(
    limit: int = Query(default=100, ge=1, le=5000),
    q: str = Query(default=""),
    tag: str = Query(default=""),
    username: str = Query(default=""),
    latest_per_user_env: bool = Query(default=True),
) -> dict[str, Any]:
    return _list_file_rows(
        "tulip_files",
        "tulipworld",
        limit=limit,
        q=q,
        tag=tag,
        username=username,
        latest_per_user_env=latest_per_user_env,
    )


@app.get("/api/tulipworld/files/{item_id}")
def get_tulip_file(item_id: int) -> dict[str, Any]:
    with _open_db() as conn:
        row = conn.execute(
            """
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes
            FROM tulip_files
            WHERE id = ? AND deleted_at_ms IS NULL
            """,
            (item_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    return _file_row_to_public(row, "tulipworld")


@app.get("/api/tulipworld/files_latest")
def get_latest_tulip_file(
    filename: str = Query(...),
    username: str = Query(default=""),
) -> dict[str, Any]:
    filename_s = (filename or "").strip()
    if not filename_s:
        raise HTTPException(status_code=400, detail="filename is required")

    params: list[Any] = [filename_s.lower()]
    where = "deleted_at_ms IS NULL AND lower(filename) = ?"
    username_s = (username or "").strip().lower()
    if username_s:
        where += " AND lower(username) = ?"
        params.append(username_s)

    with _open_db() as conn:
        row = conn.execute(
            f"""
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes
            FROM tulip_files
            WHERE {where}
            ORDER BY created_at_ms DESC, id DESC
            LIMIT 1
            """,
            params,
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    return _file_row_to_public(row, "tulipworld")


@app.get("/api/tulipworld/files/{item_id}/download")
def download_tulip_file(item_id: int) -> FileResponse:
    with _open_db() as conn:
        row = conn.execute(
            "SELECT filename, blob_path FROM tulip_files WHERE id = ? AND deleted_at_ms IS NULL",
            (item_id,),
        ).fetchone()
    if not row:
        raise HTTPException(status_code=404, detail="Not found")
    blob_path = Path(str(row["blob_path"]))
    if not blob_path.exists():
        raise HTTPException(status_code=404, detail="File missing")
    return FileResponse(str(blob_path), media_type="application/x-tar", filename=str(row["filename"]))


@app.patch("/api/tulipworld/files/{item_id}/tags", dependencies=[Depends(_require_admin)])
def patch_tulip_file_tags(item_id: int, body: TagsPatch) -> dict[str, Any]:
    tags = _parse_tags(body.tags)
    with _open_db() as conn:
        cur = conn.execute(
            "UPDATE tulip_files SET tags_json = ? WHERE id = ? AND deleted_at_ms IS NULL",
            (json.dumps(tags), item_id),
        )
        conn.commit()
        if cur.rowcount < 1:
            raise HTTPException(status_code=404, detail="Not found")
    return {"ok": True, "id": item_id, "tags": tags}


@app.delete("/api/tulipworld/files/{item_id}", dependencies=[Depends(_require_admin)])
def delete_tulip_file(item_id: int) -> dict[str, Any]:
    return _delete_file_row("tulip_files", item_id)


def _delete_file_row(table: str, item_id: int) -> dict[str, Any]:
    deleted_at_ms = _now_ms()
    with _open_db() as conn:
        row = conn.execute(
            f"SELECT blob_path FROM {table} WHERE id = ? AND deleted_at_ms IS NULL",
            (item_id,),
        ).fetchone()
        if not row:
            raise HTTPException(status_code=404, detail="Not found")
        conn.execute(f"UPDATE {table} SET deleted_at_ms = ? WHERE id = ?", (deleted_at_ms, item_id))
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
    return {"ok": True, "id": item_id}


@app.post("/api/tulipworld/messages")
async def post_tulip_message(
    request: Request,
    username: str | None = Form(default=None),
    content: str | None = Form(default=None),
    created_at_ms: int | None = Form(default=None),
) -> dict[str, Any]:
    user_raw = username
    content_raw = content
    if user_raw is None or content_raw is None:
        try:
            payload = await request.json()
        except Exception:
            payload = {}
        if user_raw is None:
            user_raw = payload.get("username", "")
        if content_raw is None:
            content_raw = payload.get("content", "")
        if created_at_ms is None:
            created_at_ms = payload.get("created_at_ms")
    user = _normalize_username(user_raw)
    msg = _normalize_message(content_raw)
    ts_ms = _normalize_created_at_ms(created_at_ms)

    with _open_db() as conn:
        cur = conn.execute(
            "INSERT INTO tulip_messages(username, content, created_at_ms) VALUES (?, ?, ?)",
            (user, msg, ts_ms),
        )
        msg_id = int(cur.lastrowid)
        conn.commit()

    _discord_notify(DISCORD_TULIP_TEXT_CHANNEL_ID, f"{user} ### {msg}")
    return {"ok": True, "id": msg_id, "time": ts_ms}


@app.get("/api/tulipworld/messages")
def list_tulip_messages(
    limit: int = Query(default=100, ge=1, le=5000),
    q: str = Query(default=""),
    username: str = Query(default=""),
) -> list[dict[str, Any]]:
    clauses = ["deleted_at_ms IS NULL"]
    params: list[Any] = []

    q_s = q.strip().lower()
    if q_s:
        like = f"%{q_s}%"
        clauses.append("(lower(username) LIKE ? OR lower(content) LIKE ?)")
        params.extend([like, like])

    username_s = username.strip().lower()
    if username_s:
        clauses.append("lower(username) = ?")
        params.append(username_s)

    with _open_db() as conn:
        rows = conn.execute(
            f"""
            SELECT id, username, content, created_at_ms
            FROM tulip_messages
            WHERE {' AND '.join(clauses)}
            ORDER BY created_at_ms DESC
            LIMIT ?
            """,
            [*params, limit],
        ).fetchall()

    return [_message_row_to_public(row) for row in rows]


@app.delete("/api/tulipworld/messages/{message_id}", dependencies=[Depends(_require_admin)])
def delete_tulip_message(message_id: int) -> dict[str, Any]:
    with _open_db() as conn:
        cur = conn.execute(
            "UPDATE tulip_messages SET deleted_at_ms = ? WHERE id = ? AND deleted_at_ms IS NULL",
            (_now_ms(), message_id),
        )
        conn.commit()
        if cur.rowcount < 1:
            raise HTTPException(status_code=404, detail="Not found")
    return {"ok": True, "id": message_id}


@app.get("/api/admin/items", dependencies=[Depends(_require_admin)])
def list_admin_items(
    scope: str = Query(default="all"),
    limit: int = Query(default=200, ge=1, le=500),
    q: str = Query(default=""),
) -> dict[str, Any]:
    scope_s = scope.strip().lower()
    valid_scopes = {"all", "amyboard_files", "tulip_files", "tulip_messages"}
    if scope_s not in valid_scopes:
        raise HTTPException(status_code=400, detail=f"scope must be one of: {', '.join(sorted(valid_scopes))}")

    items: list[dict[str, Any]] = []
    if scope_s in {"all", "amyboard_files"}:
        items.extend(
            _list_file_rows(
                "environments",
                "amyboardworld",
                limit=limit,
                q=q,
                tag="",
                username="",
                latest_per_user_env=False,
            )["items"]
        )
    if scope_s in {"all", "tulip_files"}:
        items.extend(
            _list_file_rows(
                "tulip_files",
                "tulipworld",
                limit=limit,
                q=q,
                tag="",
                username="",
                latest_per_user_env=False,
            )["items"]
        )
    if scope_s in {"all", "tulip_messages"}:
        with _open_db() as conn:
            rows = conn.execute(
                """
                SELECT id, username, content, created_at_ms
                FROM tulip_messages
                WHERE deleted_at_ms IS NULL
                ORDER BY created_at_ms DESC
                LIMIT ?
                """,
                (limit,),
            ).fetchall()
        items.extend(_message_row_to_public(row) for row in rows)

    if q.strip():
        q_l = q.strip().lower()
        items = [
            i for i in items
            if q_l in str(i.get("username", "")).lower()
            or q_l in str(i.get("filename", "")).lower()
            or q_l in str(i.get("description", "")).lower()
            or q_l in str(i.get("content", "")).lower()
        ]

    items.sort(key=lambda i: int(i.get("time", 0)), reverse=True)
    return {"items": items[:limit]}
