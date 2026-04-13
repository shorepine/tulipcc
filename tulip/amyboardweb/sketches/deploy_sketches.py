#!/usr/bin/env python3
"""Upload all official sketches to AMYboard World.

Usage:
    WORLD_ADMIN_TOKEN=xxx python3 deploy_sketches.py
    WORLD_ADMIN_TOKEN=xxx WORLD_API_URL=https://... python3 deploy_sketches.py

Uploads every .py file in this directory (except this script) to
AMYboard World as username "shorepine". Parses "# DESCRIPTION: ..."
from each file for the description field. Uses the filename stem as
the sketch name.

Requires WORLD_ADMIN_TOKEN for the reserved "shorepine" username.
"""

import os
import re
import sys
import uuid
from urllib.error import HTTPError
from urllib.request import Request, urlopen

DEFAULT_URL = "https://api-production-4bfd.up.railway.app"
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


def main():
    token = os.environ.get("WORLD_ADMIN_TOKEN", "")
    if not token:
        print("Error: WORLD_ADMIN_TOKEN environment variable must be set.")
        sys.exit(1)

    api_url = os.environ.get("WORLD_API_URL", DEFAULT_URL)
    upload_url = api_url + "/api/amyboardworld/upload"

    # Collect all .py files except this script
    script_name = os.path.basename(__file__)
    sketch_files = sorted(
        f for f in os.listdir(SCRIPT_DIR)
        if f.endswith(".py") and f != script_name
    )

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
