#!/usr/bin/env python3
"""Migrate shorepine AMYboard World .tar files to .py sketches.

Downloads uploads from the source server, converts any legacy .tar/.patch
archives into merged .py sketches, and uploads the resulting .py to the
destination server. Preserves username, description, and created_at_ms.

Default direction: api-production-4bfd (temporary new server) → tulipcc-production
(the original "zestful-love" server). Pass --source/--dest to override.

Usage:
    python3 migrate_amyboard_sketches.py \
        [--source URL] [--dest URL] \
        [--username shorepine] [--all-users] [--dry-run]
"""

import argparse
import io
import json
import os
import sys
import tarfile
import uuid
from urllib.error import HTTPError
from urllib.parse import urlencode
from urllib.request import Request, urlopen


def api_get(url, params=None):
    if params:
        url = url + "?" + urlencode(params)
    resp = urlopen(Request(url), timeout=30)
    return json.loads(resp.read())


def api_get_bytes(url):
    return urlopen(Request(url), timeout=60).read()


def multipart_upload(url, fields, filename, file_data, admin_token=None):
    boundary = uuid.uuid4().hex
    body = b""
    for key, val in fields.items():
        body += f"--{boundary}\r\n".encode()
        body += f'Content-Disposition: form-data; name="{key}"\r\n\r\n'.encode()
        body += f"{val}\r\n".encode()
    body += f"--{boundary}\r\n".encode()
    body += f'Content-Disposition: form-data; name="file"; filename="{filename}"\r\n'.encode()
    body += b"Content-Type: text/x-python\r\n\r\n"
    body += file_data
    body += f"\r\n--{boundary}--\r\n".encode()
    req = Request(url, data=body, method="POST")
    req.add_header("Content-Type", f"multipart/form-data; boundary={boundary}")
    if admin_token:
        req.add_header("X-Admin-Token", admin_token)
    return urlopen(req, timeout=60)


def editor_state_to_knobs_text(editor_state):
    """Convert editor_state.json content into wire-code lines for _auto_generated_knobs."""
    if not editor_state:
        return ""
    try:
        state = json.loads(editor_state) if isinstance(editor_state, str) else editor_state
    except (json.JSONDecodeError, TypeError):
        return ""

    lines = []
    # editor_state.json stores per-channel synth state as wire code lines
    # Format varies but typically has channel_states with wire code arrays
    channel_states = state.get("channel_states", state.get("channels", {}))
    if isinstance(channel_states, dict):
        for ch_str, ch_data in sorted(channel_states.items()):
            ch = int(ch_str) if ch_str.isdigit() else ch_str
            if isinstance(ch_data, list):
                # List of wire code strings
                for wire in ch_data:
                    wire = str(wire).strip()
                    if wire:
                        lines.append(f"i{ch}{wire}")
            elif isinstance(ch_data, dict):
                wire_lines = ch_data.get("wire_code", ch_data.get("wire", []))
                if isinstance(wire_lines, list):
                    for wire in wire_lines:
                        wire = str(wire).strip()
                        if wire:
                            lines.append(f"i{ch}{wire}")
    elif isinstance(channel_states, list):
        for ch, ch_data in enumerate(channel_states):
            if ch == 0:
                continue  # skip index 0
            if isinstance(ch_data, list):
                for wire in ch_data:
                    wire = str(wire).strip()
                    if wire:
                        lines.append(f"i{ch}{wire}")

    # Also check for global effects
    for key in ("effects", "global_effects", "fx"):
        fx = state.get(key, [])
        if isinstance(fx, list):
            for wire in fx:
                wire = str(wire).strip()
                if wire:
                    lines.append(wire)

    return "\n".join(lines)


def patch_file_to_knobs_text(patch_content, channel=1):
    """Convert a .patch file's wire code lines into channel-prefixed knobs text."""
    lines = []
    for line in patch_content.decode("utf-8", errors="replace").splitlines():
        line = line.strip()
        if line:
            lines.append(f"i{channel}{line}")
    return "\n".join(lines)


def inject_knobs_into_sketch(sketch_text, knobs_text):
    """Append _auto_generated_knobs block to sketch.py if not already present."""
    if not knobs_text:
        return sketch_text

    marker = '_auto_generated_knobs = """'
    # Strip any existing block
    if marker in sketch_text:
        start = sketch_text.find(marker)
        end_marker = '"""'
        end = sketch_text.find(end_marker, start + len(marker))
        if end >= 0:
            # Also strip preceding comment line
            pre = sketch_text[:start]
            if pre.rstrip().endswith("# Do not edit. Set automatically by the knobs on AMYboard Online."):
                pre = pre[:pre.rstrip().rfind("# Do not edit")]
            sketch_text = pre.rstrip("\n") + sketch_text[end + len(end_marker):]

    sketch_text = sketch_text.rstrip("\n") + "\n\n"
    sketch_text += "# Do not edit. Set automatically by the knobs on AMYboard Online.\n"
    sketch_text += f'{marker}\n{knobs_text}\n"""\n'
    return sketch_text


def extract_and_convert_tar(tar_bytes, env_name):
    """Extract a .tar file and produce a merged sketch.py with knobs."""
    try:
        tar = tarfile.open(fileobj=io.BytesIO(tar_bytes))
    except tarfile.TarError as e:
        print(f"    WARNING: not a valid tar: {e}")
        return None

    sketch_text = None
    editor_state = None
    patch_files = {}

    for member in tar.getmembers():
        name = member.name
        # Strip leading directory (e.g. "current/sketch.py" -> "sketch.py")
        basename = name.split("/")[-1] if "/" in name else name

        if basename == "sketch.py":
            f = tar.extractfile(member)
            if f:
                sketch_text = f.read().decode("utf-8", errors="replace")
        elif basename == "editor_state.json":
            f = tar.extractfile(member)
            if f:
                editor_state = f.read().decode("utf-8", errors="replace")
        elif basename.endswith(".patch"):
            f = tar.extractfile(member)
            if f:
                patch_files[basename] = f.read()

    if sketch_text is None:
        # No sketch.py — might just be a .patch file upload
        if patch_files:
            # Use the first patch file as the knobs source
            patch_name, patch_content = next(iter(patch_files.items()))
            sketch_text = "# Migrated from patch file\n"
            knobs_text = patch_file_to_knobs_text(patch_content, channel=1)
            return inject_knobs_into_sketch(sketch_text, knobs_text)
        return None

    # Build knobs text from editor_state.json or patch files
    knobs_text = ""
    if editor_state:
        knobs_text = editor_state_to_knobs_text(editor_state)
    if not knobs_text and patch_files:
        # Fall back to first patch file
        patch_name, patch_content = next(iter(patch_files.items()))
        knobs_text = patch_file_to_knobs_text(patch_content, channel=1)

    return inject_knobs_into_sketch(sketch_text, knobs_text)


def main():
    parser = argparse.ArgumentParser(description="Migrate AMYboard World .tar to .py")
    parser.add_argument("--source", default="https://api-production-4bfd.up.railway.app",
                        help="Source server URL")
    parser.add_argument("--dest", default="https://tulipcc-production.up.railway.app",
                        help="Destination server URL")
    parser.add_argument("--username", default="shorepine",
                        help="Only migrate files from this username (default: shorepine)")
    parser.add_argument("--all-users", action="store_true",
                        help="Migrate all users, not just --username")
    parser.add_argument("--dry-run", action="store_true",
                        help="Download and convert but don't upload")
    parser.add_argument("--admin-token", default=os.getenv("WORLD_ADMIN_TOKEN", ""),
                        help="Admin token for reserved usernames (or set WORLD_ADMIN_TOKEN)")
    args = parser.parse_args()

    print(f"Source: {args.source}")
    print(f"Dest:   {args.dest}")
    print()

    # Fetch file list from source
    params = {"limit": "5000", "latest_per_user_env": "false"}
    data = api_get(f"{args.source}/api/amyboardworld/files", params)
    items = data.get("items", data.get("files", []))
    print(f"Found {len(items)} total files on source")

    # Filter
    if not args.all_users:
        items = [i for i in items if i.get("username", "").lower() == args.username.lower()]
        print(f"Filtered to {len(items)} files from {args.username}")

    # Deduplicate: keep only the latest version of each username/filename pair
    seen = {}
    for item in items:
        key = (item.get("username", "").lower(), item.get("filename", "").lower())
        existing = seen.get(key)
        if existing is None or item.get("time", 0) > existing.get("time", 0):
            seen[key] = item
    items = list(seen.values())
    print(f"After dedup: {len(items)} unique files")
    print()

    uploaded = 0
    skipped = 0
    failed = 0

    for i, item in enumerate(items):
        fid = item["id"]
        fname = item.get("filename", f"file-{fid}")
        username = item.get("username", "unknown")
        description = item.get("description", "")
        print(f"[{i+1}/{len(items)}] {username}/{fname} (id={fid})")

        # Download
        try:
            file_bytes = api_get_bytes(f"{args.source}/api/amyboardworld/files/{fid}/download")
        except Exception as e:
            print(f"    FAILED to download: {e}")
            failed += 1
            continue

        # Determine file type
        if fname.endswith(".tar"):
            # Convert .tar to .py
            sketch_py = extract_and_convert_tar(file_bytes, fname.replace(".tar", ""))
            if sketch_py is None:
                print(f"    SKIP: no sketch.py found in tar")
                skipped += 1
                continue
            upload_name = fname.replace(".tar", ".py")
            upload_bytes = sketch_py.encode("utf-8")
            print(f"    Converted .tar -> .py ({len(upload_bytes)} bytes)")
        elif fname.endswith(".py"):
            # Already a .py, upload as-is
            upload_name = fname
            upload_bytes = file_bytes
            print(f"    Already .py ({len(upload_bytes)} bytes)")
        elif fname.endswith(".patch"):
            # Convert .patch to .py with knobs
            knobs_text = patch_file_to_knobs_text(file_bytes, channel=1)
            sketch_py = "# Migrated from patch file\n"
            sketch_py = inject_knobs_into_sketch(sketch_py, knobs_text)
            upload_name = fname.replace(".patch", ".py")
            upload_bytes = sketch_py.encode("utf-8")
            print(f"    Converted .patch -> .py ({len(upload_bytes)} bytes)")
        else:
            print(f"    SKIP: unknown format {fname}")
            skipped += 1
            continue

        if args.dry_run:
            print(f"    DRY RUN: would upload {upload_name}")
            uploaded += 1
            continue

        # Upload to destination — preserve the original created_at_ms so the
        # timestamps on the destination match the source instead of bunching
        # up at "now".
        try:
            fields = {"username": username, "description": description}
            created_at_ms = item.get("time") or item.get("created_at_ms")
            if created_at_ms:
                fields["created_at_ms"] = str(int(created_at_ms))
            multipart_upload(
                f"{args.dest}/api/amyboardworld/upload",
                fields, upload_name, upload_bytes,
                admin_token=args.admin_token,
            )
            print(f"    Uploaded {upload_name}")
            uploaded += 1
        except HTTPError as e:
            print(f"    FAILED to upload: {e.code} {e.read().decode('utf-8', errors='replace')[:200]}")
            failed += 1
        except Exception as e:
            print(f"    FAILED to upload: {e}")
            failed += 1

    print()
    print(f"Done: {uploaded} uploaded, {skipped} skipped, {failed} failed")


if __name__ == "__main__":
    main()
