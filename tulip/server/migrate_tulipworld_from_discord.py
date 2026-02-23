#!/usr/bin/env python3
"""Migrate legacy Tulip World data from Discord into the DB API.

Imports:
- Tulip World files channel attachments (all by default)
- Latest N Tulip World text messages (default: 100)
"""

from __future__ import annotations

import argparse
import os
import re
from dataclasses import dataclass
from typing import Any, Iterable

import requests

DEFAULT_API_BASE = "https://tulipcc-production.up.railway.app"
DEFAULT_TEXT_CHANNEL = "1239226672046407824"
DEFAULT_FILES_CHANNEL = "1239512482025050204"
DISCORD_EPOCH_MS = 1420070400000

# Keep compatibility with the historical Tulip World Discord token.
_DISCORD_TOKEN_A = "MTIzOTIyNTc4NDU3NzgxODc0NQ.GjGMum"
_DISCORD_TOKEN_B = "KvPGzKZDr1phrId9iY7LMtIDgMNtI0om8MsWsA"
DEFAULT_DISCORD_BOT_TOKEN = f"{_DISCORD_TOKEN_A}.{_DISCORD_TOKEN_B}"


@dataclass
class ParsedDiscordMessage:
    username: str
    content: str
    raw_id: str


def normalize_username(raw: str) -> str:
    cleaned = re.sub(r"[^A-Za-z0-9]", "", str(raw or ""))
    return cleaned[:20]


def parse_tulip_content(content: str) -> ParsedDiscordMessage | None:
    if not isinstance(content, str):
        return None
    text = content
    # Legacy code appended a trailing space workaround for Discord trim behavior.
    if text.endswith("#"):
        text = text + " "
    if " ### " not in text:
        return None
    username, body = text.split(" ### ", 1)
    username = normalize_username(username.strip())
    body = body.strip()
    if not username or not body:
        return None
    return ParsedDiscordMessage(username=username, content=body, raw_id="")


def discord_id_to_ms(discord_id: str | int) -> int:
    return (int(discord_id) >> 22) + DISCORD_EPOCH_MS


def discord_headers(token: str) -> dict[str, str]:
    return {
        "Authorization": f"Bot {token}",
        "User-Agent": "TulipCC/4.0 (https://tulip.computer, migration)",
    }


def fetch_channel_messages(
    session: requests.Session,
    token: str,
    channel_id: str,
    *,
    limit: int,
) -> list[dict[str, Any]]:
    url = f"https://discordapp.com/api/channels/{channel_id}/messages"
    out: list[dict[str, Any]] = []
    before: str | None = None

    while len(out) < limit:
        chunk = min(100, limit - len(out))
        params: dict[str, str | int] = {"limit": chunk}
        if before:
            params["before"] = before
        resp = session.get(url, headers=discord_headers(token), params=params, timeout=20)
        resp.raise_for_status()
        payload = resp.json()
        if not payload:
            break
        out.extend(payload)
        before = str(payload[-1]["id"])

    return out


def iter_all_channel_messages(
    session: requests.Session,
    token: str,
    channel_id: str,
) -> Iterable[dict[str, Any]]:
    url = f"https://discordapp.com/api/channels/{channel_id}/messages"
    before: str | None = None

    while True:
        params: dict[str, str | int] = {"limit": 100}
        if before:
            params["before"] = before
        resp = session.get(url, headers=discord_headers(token), params=params, timeout=20)
        resp.raise_for_status()
        payload = resp.json()
        if not payload:
            return
        for item in payload:
            yield item
        before = str(payload[-1]["id"])


def post_world_message(
    session: requests.Session,
    api_base: str,
    username: str,
    content: str,
    *,
    created_at_ms: int,
    dry_run: bool,
) -> None:
    content = str(content or "")[:800]
    if dry_run:
        print(f"DRYRUN message: {username}: {content[:80]}")
        return
    resp = session.post(
        f"{api_base}/api/tulipworld/messages",
        json={"username": username, "content": content, "created_at_ms": created_at_ms},
        timeout=20,
    )
    if resp.status_code >= 400:
        raise RuntimeError(f"message post failed ({resp.status_code}): {resp.text[:240]}")


def upload_world_file(
    session: requests.Session,
    api_base: str,
    *,
    username: str,
    description: str,
    filename: str,
    file_bytes: bytes,
    created_at_ms: int,
    dry_run: bool,
) -> None:
    if dry_run:
        print(f"DRYRUN file: {username} {filename} ({len(file_bytes)} bytes)")
        return
    files = {"file": (filename, file_bytes, "application/x-tar")}
    data = {"username": username, "description": description, "created_at_ms": str(created_at_ms)}
    resp = session.post(f"{api_base}/api/tulipworld/upload", data=data, files=files, timeout=60)
    if resp.status_code >= 400:
        raise RuntimeError(f"file upload failed ({resp.status_code}): {resp.text[:240]}")


def migrate_messages(
    session: requests.Session,
    *,
    api_base: str,
    token: str,
    text_channel_id: str,
    max_messages: int,
    dry_run: bool,
) -> int:
    source = fetch_channel_messages(session, token, text_channel_id, limit=max_messages)
    parsed: list[tuple[str, str, int]] = []
    for item in reversed(source):
        msg = parse_tulip_content(item.get("content", ""))
        if not msg:
            continue
        parsed.append((msg.username, msg.content, discord_id_to_ms(item.get("id"))))

    for user, text, created_at_ms in parsed:
        post_world_message(session, api_base, user, text, created_at_ms=created_at_ms, dry_run=dry_run)
    return len(parsed)


def migrate_files(
    session: requests.Session,
    *,
    api_base: str,
    token: str,
    files_channel_id: str,
    max_files: int,
    dry_run: bool,
) -> int:
    migrated = 0
    seen_keys: set[tuple[str, str, str]] = set()

    for item in iter_all_channel_messages(session, token, files_channel_id):
        attachments = item.get("attachments") or []
        if not attachments:
            continue

        msg = parse_tulip_content(item.get("content", ""))
        if not msg:
            continue

        for att in attachments:
            filename = str(att.get("filename") or "").strip()
            url = str(att.get("url") or "").strip()
            if not filename or not url:
                continue

            key = (msg.username, filename, url)
            if key in seen_keys:
                continue
            seen_keys.add(key)

            file_resp = session.get(url, timeout=60)
            file_resp.raise_for_status()
            upload_world_file(
                session,
                api_base,
                username=msg.username,
                description=msg.content,
                filename=filename,
                file_bytes=file_resp.content,
                created_at_ms=discord_id_to_ms(item.get("id")),
                dry_run=dry_run,
            )
            migrated += 1
            print(f"uploaded {migrated}: {msg.username} {filename}")

            if max_files > 0 and migrated >= max_files:
                return migrated

    return migrated


def main() -> int:
    parser = argparse.ArgumentParser(description="Migrate legacy Tulip World Discord data into Railway DB API")
    parser.add_argument("--api-base", default=os.getenv("WORLD_API_BASE", DEFAULT_API_BASE))
    parser.add_argument("--discord-bot-token", default=os.getenv("WORLD_DISCORD_BOT_TOKEN", DEFAULT_DISCORD_BOT_TOKEN))
    parser.add_argument("--text-channel-id", default=os.getenv("TULIPWORLD_DISCORD_TEXT_CHANNEL_ID", DEFAULT_TEXT_CHANNEL))
    parser.add_argument("--files-channel-id", default=os.getenv("TULIPWORLD_DISCORD_FILES_CHANNEL_ID", DEFAULT_FILES_CHANNEL))
    parser.add_argument("--max-messages", type=int, default=100, help="How many latest text messages to import")
    parser.add_argument("--max-files", type=int, default=0, help="0 = all files, otherwise stop after this many")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    if not args.discord_bot_token:
        raise SystemExit("Missing discord bot token")

    with requests.Session() as session:
        print("migrating tulip messages...")
        msg_count = migrate_messages(
            session,
            api_base=args.api_base.rstrip("/"),
            token=args.discord_bot_token,
            text_channel_id=args.text_channel_id,
            max_messages=max(1, args.max_messages),
            dry_run=args.dry_run,
        )
        print(f"imported messages: {msg_count}")

        print("migrating tulip files...")
        file_count = migrate_files(
            session,
            api_base=args.api_base.rstrip("/"),
            token=args.discord_bot_token,
            files_channel_id=args.files_channel_id,
            max_files=max(0, args.max_files),
            dry_run=args.dry_run,
        )
        print(f"imported files: {file_count}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
