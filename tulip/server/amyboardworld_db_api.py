#!/usr/bin/env python3
"""DB-backed AMYboard World + Tulip World API.

This service stores uploaded files on disk and metadata in SQLite.
It provides separate storage for:
- AMYboard World files
- Tulip World files
- Tulip World text messages
"""

from __future__ import annotations

import ast
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


def _load_dotenv() -> None:
    """Best-effort load of a repo-root .env for local development.

    Only fills variables that are not already set, so real environment
    variables (e.g. those injected by the production host) always win. Does
    nothing if no .env file is present.
    """
    try:
        env_path = Path(__file__).resolve().parents[2] / ".env"
        if not env_path.is_file():
            return
        for raw_line in env_path.read_text().splitlines():
            line = raw_line.strip()
            if not line or line.startswith("#") or "=" not in line:
                continue
            key, _, value = line.partition("=")
            key = key.strip()
            value = value.strip().strip('"').strip("'")
            if key:
                os.environ.setdefault(key, value)
    except Exception:
        pass


_load_dotenv()

USERNAME_RE = re.compile(r"^[A-Za-z0-9]{1,20}$")
ENV_NAME_RE = re.compile(r"^[A-Za-z0-9_-]{1,20}$")
FILE_NAME_RE = re.compile(r"^[A-Za-z0-9._-]{1,80}$")
TAG_RE = re.compile(r"^[A-Za-z0-9_-]{1,32}$")
RESERVED_USERNAMES = {"shorepine"}
# Sketches uploaded by these usernames are surfaced with the OFFICIAL_TAG
# automatically — the tag is injected into API responses at serialization
# time and the ?tag=official filter includes them even if their stored
# tags_json doesn't literally contain "official". Use this when a user
# account is considered official/canonical and you want all of its
# historical uploads to show up under the #official chip without running
# a database migration to backfill the tag.
OFFICIAL_USERNAMES = {"shorepine"}
OFFICIAL_TAG = "official"
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

DISCORD_BOT_TOKEN = os.getenv("WORLD_DISCORD_BOT_TOKEN", os.getenv("AMYBOARDWORLD_DISCORD_BOT_TOKEN", ""))

# --- AMYboard sketch generation (Claude API) -------------------------------
# API key for the Anthropic API. Local dev reads CLAUDE_KEY from the repo-root
# .env (see _load_dotenv); a host may instead provide ANTHROPIC_API_KEY.
CLAUDE_API_KEY = os.getenv("CLAUDE_KEY", os.getenv("ANTHROPIC_API_KEY", ""))
GENERATE_MODEL = os.getenv("AMYBOARD_GENERATE_MODEL", "claude-sonnet-4-6")
GENERATE_MAX_TOKENS = int(os.getenv("AMYBOARD_GENERATE_MAX_TOKENS", "4096"))
# Abuse / cost controls, enforced per rolling 24h window.
GENERATE_PER_IP_PER_DAY = int(os.getenv("AMYBOARD_GENERATE_PER_IP_PER_DAY", "10"))
GENERATE_GLOBAL_PER_DAY = int(os.getenv("AMYBOARD_GENERATE_GLOBAL_PER_DAY", "200"))
GENERATE_MAX_PROMPT_CHARS = int(os.getenv("AMYBOARD_GENERATE_MAX_PROMPT_CHARS", "1000"))

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
            """
        )

        # Add client_ip column to all tables.
        for tbl in ("environments", "tulip_files", "tulip_messages"):
            try:
                conn.execute(f"ALTER TABLE {tbl} ADD COLUMN client_ip TEXT NOT NULL DEFAULT ''")
            except sqlite3.OperationalError:
                pass  # column already exists

        # Add item_type column to environments (for patch vs environment distinction).
        # SQLite has no ALTER TABLE ... ADD COLUMN IF NOT EXISTS, so catch the error.
        try:
            conn.execute("ALTER TABLE environments ADD COLUMN item_type TEXT NOT NULL DEFAULT 'environment'")
        except sqlite3.OperationalError:
            pass  # column already exists
        conn.execute("CREATE INDEX IF NOT EXISTS idx_env_item_type ON environments(item_type)")

        conn.executescript(
            """
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

        # Retry client_ip for tulip_files and tulip_messages — the first
        # ALTER TABLE pass (above) runs before these tables are created on a
        # fresh database, so the column may be missing.
        for tbl in ("tulip_files", "tulip_messages"):
            try:
                conn.execute(f"ALTER TABLE {tbl} ADD COLUMN client_ip TEXT NOT NULL DEFAULT ''")
            except sqlite3.OperationalError:
                pass

        # Log of AMYboard sketch generations (for rate limiting + audit).
        conn.executescript(
            """
            CREATE TABLE IF NOT EXISTS generations (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              created_at_ms INTEGER NOT NULL,
              client_ip TEXT NOT NULL DEFAULT '',
              prompt TEXT NOT NULL DEFAULT '',
              ok INTEGER NOT NULL DEFAULT 0,
              model TEXT NOT NULL DEFAULT '',
              input_tokens INTEGER NOT NULL DEFAULT 0,
              output_tokens INTEGER NOT NULL DEFAULT 0
            );
            CREATE INDEX IF NOT EXISTS idx_gen_created ON generations(created_at_ms DESC);
            CREATE INDEX IF NOT EXISTS idx_gen_ip ON generations(client_ip);
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


def _client_ip(request: Request) -> str:
    forwarded = request.headers.get("x-forwarded-for", "")
    if forwarded:
        return forwarded.split(",")[0].strip()
    if request.client:
        return request.client.host
    return ""


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


def _validate_amyboard_sketch_upload(filename: str, contents: bytes) -> str:
    clean_name = (filename or "").strip()
    if not clean_name.lower().endswith(".py"):
        raise HTTPException(status_code=400, detail="File must be a .py")
    env_name = clean_name[:-3]
    if not ENV_NAME_RE.match(env_name):
        raise HTTPException(status_code=400, detail="Environment name must be 1-20 chars: A-Z, a-z, 0-9, -, _")
    if len(contents) < 1:
        raise HTTPException(status_code=400, detail="Empty upload")
    if len(contents) > MAX_FILE_BYTES:
        raise HTTPException(status_code=413, detail=f"File too large (max {MAX_FILE_BYTES} bytes)")
    return clean_name


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
    # Inject the official tag for sketches uploaded by OFFICIAL_USERNAMES,
    # even if their stored tags_json doesn't contain it. See OFFICIAL_USERNAMES.
    if str(row["username"]).lower() in OFFICIAL_USERNAMES and OFFICIAL_TAG not in tags:
        tags = [OFFICIAL_TAG] + tags
    t = int(row["created_at_ms"])
    # item_type is only present in the environments table; default to "environment" for others.
    try:
        item_type = row["item_type"]
    except (IndexError, KeyError):
        item_type = "environment"
    result = {
        "id": file_id,
        "scope": scope,
        "kind": "file",
        "item_type": item_type,
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
    try:
        result["client_ip"] = row["client_ip"]
    except (IndexError, KeyError):
        pass
    return result


def _message_row_to_public(row: sqlite3.Row) -> dict[str, Any]:
    now_ms = _now_ms()
    t = int(row["created_at_ms"])
    result = {
        "id": int(row["id"]),
        "scope": "tulipworld",
        "kind": "message",
        "username": row["username"],
        "content": row["content"],
        "time": t,
        "age_ms": max(0, now_ms - t),
    }
    try:
        result["client_ip"] = row["client_ip"]
    except (IndexError, KeyError):
        pass
    return result


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
    item_type: str = "environment",
    client_ip: str = "",
) -> int:
    ts_ms = _normalize_created_at_ms(created_at_ms)
    digest = hashlib.sha256(contents).hexdigest()
    with _open_db() as conn:
        # Use item_type column only for the environments table.
        if table == "environments":
            cur = conn.execute(
                f"""
                INSERT INTO {table}(username, filename, description, tags_json, created_at_ms, size_bytes, sha256, blob_path, item_type, client_ip)
                VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (username, filename, description, "[]", ts_ms, len(contents), digest, "", item_type, client_ip),
            )
        else:
            cur = conn.execute(
                f"""
                INSERT INTO {table}(username, filename, description, tags_json, created_at_ms, size_bytes, sha256, blob_path, client_ip)
                VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (username, filename, description, "[]", ts_ms, len(contents), digest, "", client_ip),
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
    item_type: str = "",
    include_ip: bool = False,
) -> dict[str, Any]:
    clauses = ["deleted_at_ms IS NULL"]
    params: list[Any] = []

    # item_type filter (only meaningful for environments table)
    item_type_s = item_type.strip().lower()
    if item_type_s and table == "environments":
        clauses.append("item_type = ?")
        params.append(item_type_s)

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
        # For OFFICIAL_TAG, also match rows whose username is in
        # OFFICIAL_USERNAMES — mirrors the auto-injection in
        # _file_row_to_public so the #official filter chip returns
        # the same set of sketches that display the #official badge.
        if tag_s == OFFICIAL_TAG and OFFICIAL_USERNAMES:
            placeholders = ",".join(["?"] * len(OFFICIAL_USERNAMES))
            clauses.append(
                f"(lower(tags_json) LIKE ? OR lower(username) IN ({placeholders}))"
            )
            params.append(f'%"{tag_s}"%')
            params.extend(sorted(OFFICIAL_USERNAMES))
        else:
            clauses.append("lower(tags_json) LIKE ?")
            params.append(f'%"{tag_s}"%')

    where_sql = " AND ".join(clauses)

    # Include item_type in SELECT for environments table.
    extra_cols = ", item_type" if table == "environments" else ""
    if include_ip:
        extra_cols += ", client_ip"

    with _open_db() as conn:
        rows = conn.execute(
            f"""
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes{extra_cols}
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
    request: Request,
    username: str = Form(...),
    description: str = Form(...),
    file: UploadFile = File(...),
    created_at_ms: int | None = Form(default=None),
    x_admin_token: str | None = Header(default=None),
) -> dict[str, Any]:
    user = _normalize_username(username)
    if user.lower() in RESERVED_USERNAMES:
        if not ADMIN_TOKEN:
            raise HTTPException(status_code=503, detail="Admin token not configured")
        if not x_admin_token or x_admin_token != ADMIN_TOKEN:
            raise HTTPException(status_code=403, detail="Admin token required for reserved username")
    desc = _normalize_description(description)
    contents = await file.read()
    raw_filename = (file.filename or "").strip()

    # Accept .py (sketch) and .tar (legacy environment) uploads. .patch
    # support was removed in Apr 2026 — we no longer use AMYboard patches.
    if raw_filename.lower().endswith(".py"):
        filename = _validate_amyboard_sketch_upload(raw_filename, contents)
        upload_item_type = "environment"
    elif raw_filename.lower().endswith(".tar"):
        filename = _validate_amyboard_upload(raw_filename, contents)
        upload_item_type = "environment"
    else:
        raise HTTPException(status_code=400, detail="File must be a .py or .tar")

    item_id = _insert_file_row(
        "environments",
        AMYBOARD_FILES_DIR,
        user,
        desc,
        filename,
        contents,
        created_at_ms=created_at_ms,
        item_type=upload_item_type,
        client_ip=_client_ip(request),
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
    item_type: str = Query(default=""),
) -> dict[str, Any]:
    return _list_file_rows(
        "environments",
        "amyboardworld",
        limit=limit,
        q=q,
        tag=tag,
        username=username,
        latest_per_user_env=latest_per_user_env,
        item_type=item_type,
    )


@app.get("/api/amyboardworld/files/{item_id}")
def get_amyboard_file(item_id: int) -> dict[str, Any]:
    with _open_db() as conn:
        row = conn.execute(
            """
            SELECT id, username, filename, description, tags_json, created_at_ms, size_bytes, item_type
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
    fname = str(row["filename"])
    lower = fname.lower()
    if lower.endswith(".py"):
        mime = "text/x-python"
    elif lower.endswith(".tar"):
        mime = "application/x-tar"
    else:
        mime = "application/octet-stream"
    return FileResponse(str(blob_path), media_type=mime, filename=fname)


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
    request: Request,
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
        client_ip=_client_ip(request),
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

    ip = _client_ip(request)
    with _open_db() as conn:
        cur = conn.execute(
            "INSERT INTO tulip_messages(username, content, created_at_ms, client_ip) VALUES (?, ?, ?, ?)",
            (user, msg, ts_ms, ip),
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
                include_ip=True,
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
                include_ip=True,
            )["items"]
        )
    if scope_s in {"all", "tulip_messages"}:
        with _open_db() as conn:
            rows = conn.execute(
                """
                SELECT id, username, content, created_at_ms, client_ip
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


# ---------------------------------------------------------------------------
# AMYboard sketch generation via the Claude API
# ---------------------------------------------------------------------------

# Static, cacheable system prompt. Keep this byte-stable across requests so the
# Anthropic prompt cache stays warm (the user's description is the only part
# that varies, and it goes in the user turn). The examples below are trimmed
# real sketches from tulip/amyboardweb/sketches/.
_AMYBOARD_SYSTEM_PROMPT = """You are an expert sound designer and MicroPython programmer for the AMYboard, a small hardware + web synthesizer that runs the AMY audio-synthesis engine. Your ONLY job is to write AMYboard "sketches": short MicroPython programs (the contents of a sketch.py file) that make sound and music on the AMYboard or its in-browser simulator.

OUTPUT CONTRACT (strict):
- Respond with ONLY the contents of sketch.py as plain MicroPython source.
- No Markdown, no code fences, no explanation before or after the code.
- Begin with comment lines, including one line of the form: # DESCRIPTION: <short summary>
- You MUST define a top-level loop() function. It may just `pass`. loop() is called repeatedly (about every 32nd note) while the sketch runs; use it for sequencing, timing, and reading inputs.
- Top-level code runs once at boot (set up synths, effects, callbacks there).
- Only use the APIs documented below. Do NOT invent functions, modules, or parameters.
- Keep sketches self-contained and runnable in the web simulator: no network, no filesystem, no SD card, no long blocking loops, and never call time.sleep() inside loop().

SCOPE GUARD:
- You only produce AMYboard music/synthesis sketches. If the request is not about making sound, music, or a synth/instrument/effect on the AMYboard (for example it asks for an essay, a web page, general-purpose code, math help, or anything unrelated), do NOT comply. Instead output a minimal valid sketch whose # DESCRIPTION line politely states that the request is outside the scope of AMYboard sketch generation.

THE AMY ENGINE (import amy)
Everything is driven by amy.send(...) with keyword arguments. The most reliable way to make sound is the patch + synth model:
- amy.send(synth=N, patch=P, num_voices=V): configure synth N (N=1..16 also map to MIDI channels) with built-in patch P and V-voice polyphony.
    Patch ranges: 0-127 = Juno-6 analog, 128-255 = DX7 FM, 256 = grand piano, 257+ = PCM drum kits.
- amy.send(synth=N, note=MIDI, vel=V): play a note. note is a MIDI number 0-127. vel is 0.0-1.0 (velocity). vel=0 is note-off.
- amy.send(synth=N, vel=0): all notes off for that synth.
- amy.send(synth=N, grab_midi_notes=0): stop synth N from auto-playing incoming MIDI, so your sketch can play notes itself (use this whenever you handle MIDI in a callback).
Common per-synth parameters you may pass to amy.send(synth=N, ...): filter_freq, filter_type (amy.FILTER_LPF, amy.FILTER_LPF24, amy.FILTER_HPF, amy.FILTER_BPF), resonance, amp, pan, bend, portamento, chorus, reverb, echo.
Low-level oscillators (advanced): amy.send(osc=K, wave=W, freq=Hz, vel=V) where wave W is one of amy.SINE, amy.PULSE, amy.SAW_DOWN, amy.SAW_UP, amy.TRIANGLE, amy.NOISE, amy.PCM. amy.reset() clears all state.
Modulation routing: a parameter may be a dict to mix a constant with a control source, e.g. filter_freq={'const': 300, 'ext0': 0.25} routes CV1 (ext0) into cutoff; ext1 is CV2.
Global effects are strings, e.g. amy.send(reverb="0.5,0.3,0.05"), amy.send(chorus="0.6,2,0.3"), amy.send(echo="0.4,200,0.3,0.3").

THE amyboard MODULE (import amyboard) -- physical I/O (present on hardware; safe to call in the simulator)
- amyboard.cv_in(channel) -> volts (-10..10). channel 0 = CV1, 1 = CV2.
- amyboard.cv_out(volts, channel): write a CV output.
- amyboard.read_encoder(), amyboard.init_buttons(), amyboard.read_buttons(): rotary encoder + buttons.
- amyboard.init_display(); amyboard.display.fill(0); amyboard.display.text("hi", 0, 0, 255); amyboard.display_refresh(): optional OLED.

OTHER MODULES
- import midi: midi.add_callback(fn) registers fn(msg) for incoming MIDI. msg is a 3-byte sequence [status, data1, data2]; note-on is (status & 0xF0)==0x90 with vel>0, note-off is 0x80 (or 0x90 with vel==0).
- import tulip: tulip.amy_ticks_ms() returns a millisecond clock. Use it in loop() to schedule events instead of sleeping.
- from music import Chord, Key: Chord("C:maj").annotations is a list of semitone offsets; Key("A:min") for scales. Root note names are 'C','C#','D','D#','E','F','F#','G','G#','A','A#','B'.
- import sequencer: sequencer.tempo(bpm) sets the sketch tempo.
- Standard library available: random, math.

CONVENTIONS
- For onscreen-keyboard playing in the simulator, a synth with default grab_midi_notes will already respond to MIDI channel 1; sketches that play their own notes should set grab_midi_notes=0 first.
- The web editor may append a trailing block named _auto_generated_knobs; never write one yourself and never depend on it.

EXAMPLES (each is a complete, valid sketch.py)

# AMYboard Sketch
# DESCRIPTION: Play an additive-voiced grand piano on MIDI channel 1.
import amy
amy.send(synth=1, patch=256, num_voices=6)

def loop():
    pass

# AMYboard Sketch
# DESCRIPTION: Each MIDI key triggers a major chord rooted at that key.
import amy, midi
from music import Chord

amy.send(synth=1, grab_midi_notes=0)
ROOT_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
active = {}

def chord_for_root(root_midi):
    name = ROOT_NAMES[root_midi % 12]
    c = Chord(name + ":maj")
    return [root_midi + offset for offset in c.annotations]

def midi_cb(m):
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    note = m[1]
    vel = m[2]
    if status == 0x90 and vel > 0:
        notes = chord_for_root(note)
        active[note] = notes
        v = vel / 127.0
        for n in notes:
            amy.send(synth=1, note=n, vel=v)
    elif status == 0x80 or (status == 0x90 and vel == 0):
        for n in active.pop(note, ()):
            amy.send(synth=1, note=n, vel=0)

midi.add_callback(midi_cb)

def loop():
    pass

# AMYboard Sketch
# DESCRIPTION: Hold MIDI keys; plays them in order as 8th-note arpeggios.
import amy, midi, tulip

amy.send(synth=1, grab_midi_notes=0)
STEP_MS = 250  # 8th note at 120 BPM
held = set()
arp_idx = 0
last_played = None
last_step_ms = 0

def midi_cb(m):
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    note = m[1]
    vel = m[2]
    if status == 0x90 and vel > 0:
        held.add(note)
    elif status == 0x80 or (status == 0x90 and vel == 0):
        held.discard(note)

midi.add_callback(midi_cb)

def loop():
    global arp_idx, last_played, last_step_ms
    now = tulip.amy_ticks_ms()
    if now - last_step_ms < STEP_MS:
        return
    last_step_ms = now
    if last_played is not None:
        amy.send(synth=1, note=last_played, vel=0)
        last_played = None
    if not held:
        arp_idx = 0
        return
    notes = sorted(held)
    arp_idx %= len(notes)
    n = notes[arp_idx]
    amy.send(synth=1, note=n, vel=0.8)
    last_played = n
    arp_idx += 1

# AMYboard Sketch
# DESCRIPTION: CV1 controls filter cutoff, CV2 controls resonance.
import amy, amyboard

amy.send(synth=1, filter_freq={'const': 300, 'ext0': 0.25}, filter_type=amy.FILTER_LPF24)

def loop():
    r = (amyboard.cv_in(1) + 10.0) / 5.0  # map CV2 to roughly 0-4
    amy.send(synth=1, resonance=r)
"""


def _extract_sketch_code(text: str) -> str:
    """Strip Markdown fences the model may have added, returning bare source."""
    s = (text or "").strip()
    if s.startswith("```"):
        lines = s.split("\n")
        if lines and lines[0].startswith("```"):
            lines = lines[1:]
        if lines and lines[-1].strip().startswith("```"):
            lines = lines[:-1]
        s = "\n".join(lines).strip()
    return s


def _validate_sketch(code: str) -> tuple[bool, str]:
    """Structural check that the output is a real AMYboard sketch.

    This both guards quality and blocks "free Claude" abuse: the output must
    parse as Python, import amy/amyboard, and define a loop() function.
    """
    if not code or not code.strip():
        return False, "empty output"
    try:
        tree = ast.parse(code)
    except SyntaxError as exc:
        return False, f"syntax error: {exc}"
    imports_amy = False
    defines_loop = False
    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            if any(alias.name.split(".")[0] in ("amy", "amyboard") for alias in node.names):
                imports_amy = True
        elif isinstance(node, ast.ImportFrom):
            if (node.module or "").split(".")[0] in ("amy", "amyboard"):
                imports_amy = True
        elif isinstance(node, ast.FunctionDef) and node.name == "loop":
            defines_loop = True
    if not imports_amy:
        return False, "does not import amy or amyboard"
    if not defines_loop:
        return False, "does not define a loop() function"
    return True, ""


def _count_generations_since(ip: str, since_ms: int) -> tuple[int, int]:
    """Return (count for this ip, total count) in the window since since_ms."""
    with _open_db() as conn:
        ip_row = conn.execute(
            "SELECT COUNT(*) AS n FROM generations WHERE created_at_ms >= ? AND client_ip = ?",
            (since_ms, ip),
        ).fetchone()
        all_row = conn.execute(
            "SELECT COUNT(*) AS n FROM generations WHERE created_at_ms >= ?",
            (since_ms,),
        ).fetchone()
    return (int(ip_row["n"]) if ip_row else 0, int(all_row["n"]) if all_row else 0)


def _log_generation(ip: str, prompt: str, result: dict[str, Any]) -> None:
    with _open_db() as conn:
        conn.execute(
            """
            INSERT INTO generations(created_at_ms, client_ip, prompt, ok, model, input_tokens, output_tokens)
            VALUES(?, ?, ?, ?, ?, ?, ?)
            """,
            (
                _now_ms(),
                ip,
                prompt[:MAX_DESCRIPTION],
                1 if result.get("ok") else 0,
                GENERATE_MODEL,
                int(result.get("input_tokens", 0)),
                int(result.get("output_tokens", 0)),
            ),
        )
        conn.commit()


def _generate_sketch_via_claude(description: str, current_code: str | None) -> dict[str, Any]:
    """Call the Anthropic API once (with one corrective retry) and return the
    extracted, validated sketch plus token usage. Raises on API errors."""
    import anthropic  # lazy: server still imports if the dep is absent

    client = anthropic.Anthropic(api_key=CLAUDE_API_KEY)
    system = [{"type": "text", "text": _AMYBOARD_SYSTEM_PROMPT, "cache_control": {"type": "ephemeral"}}]

    frame = (
        "Write a complete AMYboard sketch.py for this request.\n\n"
        "<request>\n" + description.strip() + "\n</request>"
    )
    if current_code and current_code.strip():
        frame += (
            "\n\nThe user is currently editing this sketch; modify it to satisfy the "
            "request rather than starting over:\n<current_sketch>\n"
            + current_code.strip()[:8000]
            + "\n</current_sketch>"
        )
    frame += "\n\nOutput only the contents of sketch.py."
    messages: list[dict[str, Any]] = [{"role": "user", "content": frame}]

    def _call() -> Any:
        return client.messages.create(
            model=GENERATE_MODEL,
            max_tokens=GENERATE_MAX_TOKENS,
            system=system,
            messages=messages,
        )

    resp = _call()
    raw = "".join(b.text for b in resp.content if getattr(b, "type", "") == "text")
    code = _extract_sketch_code(raw)
    ok, reason = _validate_sketch(code)

    if not ok:
        # One corrective retry — cheaper than failing the user outright.
        messages.append({"role": "assistant", "content": raw})
        messages.append(
            {
                "role": "user",
                "content": (
                    f"That was not a valid AMYboard sketch ({reason}). Output ONLY a "
                    "corrected, complete sketch.py that imports amy (or amyboard) and "
                    "defines a loop() function. No markdown, no commentary."
                ),
            }
        )
        resp = _call()
        raw = "".join(b.text for b in resp.content if getattr(b, "type", "") == "text")
        code = _extract_sketch_code(raw)
        ok, reason = _validate_sketch(code)

    usage = getattr(resp, "usage", None)
    return {
        "ok": ok,
        "reason": reason,
        "code": code,
        "input_tokens": getattr(usage, "input_tokens", 0) or 0,
        "output_tokens": getattr(usage, "output_tokens", 0) or 0,
        "cache_read_input_tokens": getattr(usage, "cache_read_input_tokens", 0) or 0,
    }


class GenerateRequest(BaseModel):
    description: str
    current_code: str | None = None


@app.post("/api/amyboardworld/generate")
def generate_amyboard_sketch(body: GenerateRequest, request: Request) -> dict[str, Any]:
    if not CLAUDE_API_KEY:
        raise HTTPException(status_code=503, detail="Sketch generation is not configured on this server")

    description = " ".join((body.description or "").split()).strip()
    if len(description) < 3:
        raise HTTPException(status_code=400, detail="Describe what you want the sketch to do (a few words at least)")
    if len(description) > GENERATE_MAX_PROMPT_CHARS:
        raise HTTPException(status_code=400, detail=f"Description too long (max {GENERATE_MAX_PROMPT_CHARS} characters)")

    ip = _client_ip(request)
    since_ms = _now_ms() - 24 * 60 * 60 * 1000
    ip_count, global_count = _count_generations_since(ip, since_ms)
    if global_count >= GENERATE_GLOBAL_PER_DAY:
        raise HTTPException(status_code=429, detail="Sketch generation is busy today — please try again tomorrow")
    if ip_count >= GENERATE_PER_IP_PER_DAY:
        raise HTTPException(
            status_code=429,
            detail=f"Daily limit reached ({GENERATE_PER_IP_PER_DAY} sketches/day). Try again tomorrow.",
        )

    try:
        result = _generate_sketch_via_claude(description, body.current_code)
    except Exception as exc:  # noqa: BLE001 — map any API failure to an HTTP error
        status = 502
        try:
            import anthropic

            if isinstance(exc, anthropic.RateLimitError):
                status = 429
            elif isinstance(exc, anthropic.AuthenticationError):
                status = 503
        except Exception:
            pass
        raise HTTPException(status_code=status, detail=f"Sketch generation failed: {exc}") from exc

    # Every billed attempt (success or validation failure) counts toward limits.
    _log_generation(ip, description, result)

    if not result["ok"]:
        raise HTTPException(
            status_code=422,
            detail="Could not produce a valid sketch for that request. Try rephrasing it as a musical or synth idea.",
        )

    return {
        "ok": True,
        "code": result["code"],
        "remaining_today": max(0, GENERATE_PER_IP_PER_DAY - (ip_count + 1)),
        "model": GENERATE_MODEL,
    }
