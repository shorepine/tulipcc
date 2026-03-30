#!/usr/bin/env python3
"""Backup and restore AMYboard World + Tulip World from Railway.

Usage:
    WORLD_ADMIN_TOKEN=xxx python3 backup_world.py --backup [--url URL]
    WORLD_ADMIN_TOKEN=xxx python3 backup_world.py --restore <archive.tar.gz> [--url URL]

Requires the WORLD_ADMIN_TOKEN environment variable to be set.

Backup downloads all metadata and files via the public API and packs
them into a timestamped .tar.gz archive.  Restore re-uploads everything
from a previously created archive.

No external dependencies — uses only the Python standard library.
"""

import argparse
import io
import json
import os
import sys
import tarfile
import uuid
from datetime import datetime
from urllib.error import HTTPError
from urllib.parse import urlencode
from urllib.request import Request, urlopen

DEFAULT_URL = "https://tulipcc-production.up.railway.app"
MAX_LIMIT = 5000  # API max


# ── Helpers ───────────────────────────────────────────────────────────────────

def api_get(url, params=None):
    """GET request, return parsed JSON."""
    if params:
        url = url + "?" + urlencode(params)
    resp = urlopen(Request(url), timeout=30)
    return json.loads(resp.read())


def api_get_bytes(url):
    """GET request, return raw bytes."""
    return urlopen(Request(url), timeout=60).read()


def fetch_all(url, params=None):
    """Fetch all records from a list endpoint (limit=5000)."""
    if params is None:
        params = {}
    params["limit"] = MAX_LIMIT
    data = api_get(url, params)
    if isinstance(data, dict) and "files" in data:
        return data["files"]
    elif isinstance(data, list):
        return data
    return data.get("items", [])


def multipart_upload(url, fields, filename, file_data):
    """POST multipart/form-data with a file."""
    boundary = uuid.uuid4().hex
    body = b""
    for key, val in fields.items():
        body += f"--{boundary}\r\n".encode()
        body += f"Content-Disposition: form-data; name=\"{key}\"\r\n\r\n".encode()
        body += f"{val}\r\n".encode()
    body += f"--{boundary}\r\n".encode()
    body += f"Content-Disposition: form-data; name=\"file\"; filename=\"{filename}\"\r\n".encode()
    body += b"Content-Type: application/octet-stream\r\n\r\n"
    body += file_data
    body += f"\r\n--{boundary}--\r\n".encode()
    req = Request(url, data=body, method="POST")
    req.add_header("Content-Type", f"multipart/form-data; boundary={boundary}")
    return urlopen(req, timeout=60)


def json_post(url, payload):
    """POST JSON body."""
    data = json.dumps(payload).encode("utf-8")
    req = Request(url, data=data, method="POST")
    req.add_header("Content-Type", "application/json")
    return urlopen(req, timeout=30)


# ── Backup ────────────────────────────────────────────────────────────────────

def do_backup(base_url):
    ts = datetime.now().strftime("%Y%m%d-%H%M%S")
    archive_name = f"world-backup-{ts}.tar.gz"

    print(f"Backing up from {base_url}")
    print()

    # Fetch all metadata
    print("Fetching AMYboard World files list...")
    amyboard_files = fetch_all(
        f"{base_url}/api/amyboardworld/files",
        {"latest_per_user_env": "false"},
    )
    print(f"  {len(amyboard_files)} entries")

    print("Fetching Tulip World files list...")
    tulip_files = fetch_all(
        f"{base_url}/api/tulipworld/files",
        {"latest_per_user_env": "false"},
    )
    print(f"  {len(tulip_files)} entries")

    print("Fetching Tulip World messages...")
    tulip_messages = fetch_all(f"{base_url}/api/tulipworld/messages")
    print(f"  {len(tulip_messages)} entries")
    print()

    metadata = {
        "backed_up_at": ts,
        "source_url": base_url,
        "amyboard_files": amyboard_files,
        "tulip_files": tulip_files,
        "tulip_messages": tulip_messages,
    }

    with tarfile.open(archive_name, "w:gz") as tar:
        # Write metadata.json
        meta_bytes = json.dumps(metadata, indent=2).encode("utf-8")
        info = tarfile.TarInfo(name="metadata.json")
        info.size = len(meta_bytes)
        tar.addfile(info, io.BytesIO(meta_bytes))

        # Download and archive AMYboard files
        for i, f in enumerate(amyboard_files):
            fid = f["id"]
            fname = f.get("filename", f"file-{fid}")
            print(f"  [{i+1}/{len(amyboard_files)}] AMYboard: {fname} (id={fid})")
            try:
                data = api_get_bytes(f"{base_url}/api/amyboardworld/files/{fid}/download")
                arc_path = f"amyboardworld_files/{fid}-{fname}"
                info = tarfile.TarInfo(name=arc_path)
                info.size = len(data)
                tar.addfile(info, io.BytesIO(data))
            except Exception as e:
                print(f"    WARNING: failed to download id={fid}: {e}")

        # Download and archive Tulip World files
        for i, f in enumerate(tulip_files):
            fid = f["id"]
            fname = f.get("filename", f"file-{fid}")
            print(f"  [{i+1}/{len(tulip_files)}] Tulip: {fname} (id={fid})")
            try:
                data = api_get_bytes(f"{base_url}/api/tulipworld/files/{fid}/download")
                arc_path = f"tulipworld_files/{fid}-{fname}"
                info = tarfile.TarInfo(name=arc_path)
                info.size = len(data)
                tar.addfile(info, io.BytesIO(data))
            except Exception as e:
                print(f"    WARNING: failed to download id={fid}: {e}")

    size_mb = os.path.getsize(archive_name) / (1024 * 1024)
    print()
    print(f"Backup complete: {archive_name} ({size_mb:.1f} MB)")
    print(f"  {len(amyboard_files)} AMYboard files")
    print(f"  {len(tulip_files)} Tulip files")
    print(f"  {len(tulip_messages)} Tulip messages")


# ── Restore ───────────────────────────────────────────────────────────────────

def do_restore(archive_path, base_url):
    if not os.path.exists(archive_path):
        print(f"Error: {archive_path} not found")
        sys.exit(1)

    print(f"Restoring from {archive_path} to {base_url}")
    print()

    with tarfile.open(archive_path, "r:gz") as tar:
        meta_file = tar.extractfile("metadata.json")
        if not meta_file:
            print("Error: no metadata.json in archive")
            sys.exit(1)
        metadata = json.loads(meta_file.read())

        amyboard_files = metadata.get("amyboard_files", [])
        tulip_files = metadata.get("tulip_files", [])
        tulip_messages = metadata.get("tulip_messages", [])

        print(f"Archive contains:")
        print(f"  {len(amyboard_files)} AMYboard files")
        print(f"  {len(tulip_files)} Tulip files")
        print(f"  {len(tulip_messages)} Tulip messages")
        print(f"  Backed up at: {metadata.get('backed_up_at', 'unknown')}")
        print(f"  Source: {metadata.get('source_url', 'unknown')}")
        print()

        # Restore AMYboard files
        for i, f in enumerate(amyboard_files):
            fid = f["id"]
            fname = f.get("filename", f"file-{fid}")
            username = f.get("username", "unknown")
            description = f.get("description", "")
            created_at_ms = f.get("created_at_ms")
            print(f"  [{i+1}/{len(amyboard_files)}] AMYboard: {fname} by {username}")

            arc_path = f"amyboardworld_files/{fid}-{fname}"
            try:
                member = tar.extractfile(arc_path)
                if not member:
                    print(f"    WARNING: {arc_path} not found in archive, skipping")
                    continue
                file_data = member.read()
                fields = {"username": username, "description": description}
                if created_at_ms:
                    fields["created_at_ms"] = str(created_at_ms)
                multipart_upload(
                    f"{base_url}/api/amyboardworld/upload",
                    fields, fname, file_data,
                )
            except Exception as e:
                print(f"    WARNING: failed to restore id={fid}: {e}")

        # Restore Tulip World files
        for i, f in enumerate(tulip_files):
            fid = f["id"]
            fname = f.get("filename", f"file-{fid}")
            username = f.get("username", "unknown")
            description = f.get("description", "")
            created_at_ms = f.get("created_at_ms")
            print(f"  [{i+1}/{len(tulip_files)}] Tulip: {fname} by {username}")

            arc_path = f"tulipworld_files/{fid}-{fname}"
            try:
                member = tar.extractfile(arc_path)
                if not member:
                    print(f"    WARNING: {arc_path} not found in archive, skipping")
                    continue
                file_data = member.read()
                fields = {"username": username, "description": description}
                if created_at_ms:
                    fields["created_at_ms"] = str(created_at_ms)
                multipart_upload(
                    f"{base_url}/api/tulipworld/upload",
                    fields, fname, file_data,
                )
            except Exception as e:
                print(f"    WARNING: failed to restore id={fid}: {e}")

        # Restore Tulip World messages
        for i, m in enumerate(tulip_messages):
            username = m.get("username", "unknown")
            content = m.get("content", "")
            created_at_ms = m.get("created_at_ms")
            print(f"  [{i+1}/{len(tulip_messages)}] Message by {username}: {content[:50]}...")
            try:
                payload = {"username": username, "content": content}
                if created_at_ms:
                    payload["created_at_ms"] = created_at_ms
                json_post(f"{base_url}/api/tulipworld/messages", payload)
            except Exception as e:
                print(f"    WARNING: failed to restore message: {e}")

    print()
    print("Restore complete.")


# ── CLI ───────────────────────────────────────────────────────────────────────

def main():
    token = os.environ.get("WORLD_ADMIN_TOKEN", "")
    if not token:
        print("Error: WORLD_ADMIN_TOKEN environment variable must be set.")
        sys.exit(1)

    parser = argparse.ArgumentParser(
        description="Backup and restore AMYboard World + Tulip World"
    )
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--backup", action="store_true", help="Create a backup archive")
    group.add_argument("--restore", metavar="ARCHIVE", help="Restore from a backup archive")
    parser.add_argument(
        "--url", default=DEFAULT_URL,
        help=f"Server URL (default: {DEFAULT_URL})",
    )

    args = parser.parse_args()

    if args.backup:
        do_backup(args.url)
    elif args.restore:
        do_restore(args.restore, args.url)


if __name__ == "__main__":
    main()
