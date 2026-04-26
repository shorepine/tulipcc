#!/usr/bin/env python3
"""Upload or verify official sketches against AMYboard World.

Usage:
    WORLD_ADMIN_TOKEN=xxx python3 deploy_sketches.py
    WORLD_ADMIN_TOKEN=xxx WORLD_API_URL=https://... python3 deploy_sketches.py
    python3 deploy_sketches.py --verify

Uploads every .py file in this directory (except this script) to
AMYboard World as username "shorepine". Parses "# DESCRIPTION: ..."
from each file for the description field. Uses the filename stem as
the sketch name.

With --verify, fetches each shorepine sketch from ABW and compares
content + description to the local copy. Reports mismatches and exits
non-zero if any are found. No admin token is required for verify.

Requires WORLD_ADMIN_TOKEN for the reserved "shorepine" username (upload only).
"""

import json
import os
import re
import sys
import uuid
from urllib.error import HTTPError
from urllib.request import Request, urlopen

DEFAULT_URL = "https://tulipcc-production.up.railway.app"
USERNAME = "shorepine"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def extract_description(text):
    """Extract DESCRIPTION from '# DESCRIPTION: ...' comment line."""
    for line in text.split("\n"):
        m = re.match(r"^#\s*DESCRIPTION:\s*(.+)", line, re.IGNORECASE)
        if m:
            return m.group(1).strip()
    return ""


def upload(url, username, filename, description, file_data, admin_token):
    """POST multipart/form-data upload."""
    boundary = uuid.uuid4().hex
    body = b""
    for key, val in [("username", username), ("description", description)]:
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
    return urlopen(req, timeout=30)


def collect_local_sketches():
    """Return sorted list of local .py sketch filenames (excluding this script)."""
    script_name = os.path.basename(__file__)
    return sorted(
        f for f in os.listdir(SCRIPT_DIR)
        if f.endswith(".py") and f != script_name
    )


def fetch_remote_shorepine(api_url):
    """Return dict mapping filename -> item metadata for shorepine ABW sketches."""
    list_url = api_url + "/api/amyboardworld/files?limit=1000"
    with urlopen(list_url, timeout=30) as resp:
        data = json.loads(resp.read().decode("utf-8"))
    items = data.get("items", [])
    return {
        i["filename"]: i
        for i in items
        if i.get("username") == USERNAME and i.get("item_type") == "environment"
    }


def fetch_remote_content(api_url, item):
    """Return raw bytes of an ABW item's file content."""
    url = api_url + item["download_url"]
    with urlopen(url, timeout=30) as resp:
        return resp.read()


def verify(api_url):
    """Compare each local sketch to its ABW shorepine counterpart.

    Returns (mismatches, missing_remote, extra_remote) where each is a list.
    """
    local_files = collect_local_sketches()
    remote = fetch_remote_shorepine(api_url)

    print(f"Verifying {len(local_files)} local sketches against {api_url}")
    print(f"Username: {USERNAME}")
    print(f"Remote shorepine sketches: {len(remote)}")
    print()

    mismatches = []
    missing_remote = []
    ok = 0

    for filename in local_files:
        filepath = os.path.join(SCRIPT_DIR, filename)
        with open(filepath, "rb") as f:
            local_bytes = f.read()
        local_desc = extract_description(local_bytes.decode("utf-8", errors="replace"))

        item = remote.get(filename)
        if item is None:
            print(f"  {filename}: MISSING on ABW")
            missing_remote.append(filename)
            continue

        try:
            remote_bytes = fetch_remote_content(api_url, item)
        except Exception as e:
            print(f"  {filename}: FAILED to fetch remote: {e}")
            mismatches.append((filename, f"fetch error: {e}"))
            continue

        remote_desc = item.get("description", "") or ""
        problems = []
        if remote_bytes != local_bytes:
            problems.append(
                f"content differs (local {len(local_bytes)}b, remote {len(remote_bytes)}b)"
            )
        if remote_desc != local_desc:
            problems.append(
                f"description differs (local={local_desc!r}, remote={remote_desc!r})"
            )

        if problems:
            print(f"  {filename}: MISMATCH")
            for p in problems:
                print(f"    - {p}")
            mismatches.append((filename, "; ".join(problems)))
        else:
            print(f"  {filename}: OK")
            ok += 1

    extra_remote = sorted(set(remote) - set(local_files))
    if extra_remote:
        print()
        print("Remote shorepine sketches with no local file:")
        for f in extra_remote:
            print(f"  + {f}")

    print()
    print(
        f"Summary: {ok} OK, {len(mismatches)} mismatched, "
        f"{len(missing_remote)} missing on ABW, {len(extra_remote)} extra on ABW"
    )
    return mismatches, missing_remote, extra_remote


def main():
    args = sys.argv[1:]
    api_url = os.environ.get("WORLD_API_URL", DEFAULT_URL)

    if "--verify" in args:
        mismatches, missing_remote, extra_remote = verify(api_url)
        if mismatches or missing_remote or extra_remote:
            sys.exit(1)
        sys.exit(0)

    token = os.environ.get("WORLD_ADMIN_TOKEN", "")
    if not token:
        print("Error: WORLD_ADMIN_TOKEN environment variable must be set.")
        sys.exit(1)

    upload_url = api_url + "/api/amyboardworld/upload"

    sketch_files = collect_local_sketches()

    if not sketch_files:
        print("No sketch files found.")
        return

    print(f"Uploading {len(sketch_files)} sketches to {api_url}")
    print(f"Username: {USERNAME}")
    print()

    uploaded = 0
    failed = 0

    for filename in sketch_files:
        filepath = os.path.join(SCRIPT_DIR, filename)
        with open(filepath, "rb") as f:
            file_data = f.read()

        text = file_data.decode("utf-8", errors="replace")
        description = extract_description(text)
        stem = filename.replace(".py", "")

        print(f"  {filename} ({len(file_data)} bytes)")
        if description:
            print(f"    desc: {description}")
        else:
            print(f"    desc: (none)")

        try:
            upload(upload_url, USERNAME, filename, description, file_data, token)
            print(f"    OK")
            uploaded += 1
        except HTTPError as e:
            body = e.read().decode("utf-8", errors="replace")[:200]
            print(f"    FAILED: {e.code} {body}")
            failed += 1
        except Exception as e:
            print(f"    FAILED: {e}")
            failed += 1

    print()
    print(f"Done: {uploaded} uploaded, {failed} failed")


if __name__ == "__main__":
    main()
