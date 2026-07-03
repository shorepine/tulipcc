"""Thin client for the AMYboard World sketch API (read-only, public).

Backed by tulip/server/amyboardworld_db_api.py; production base is
https://tulipcc-production.up.railway.app

Stdlib-only (urllib) so environments without `requests` — like the Pi HW-CI
venv — can use it.
"""

import json
import urllib.parse
import urllib.request

DEFAULT_BASE = "https://tulipcc-production.up.railway.app"

# How internal World usernames are credited to viewers. The 'generator' account
# holds sketches made from community-entered text prompts — that's real
# authorship — so we credit them as "anonymous" rather than exposing the backend
# account name.
USERNAME_ALIASES = {"generator": "anonymous"}


def display_username(name):
    """Map an internal World username to how it should be shown to viewers."""
    if not name:
        return name
    return USERNAME_ALIASES.get(name.strip().lower(), name)


def list_sketches(base=DEFAULT_BASE, limit=100, tag="", q="", username="",
                  item_type="environment", latest_per_user_env=True, timeout=30):
    params = {"limit": limit, "item_type": item_type,
              "latest_per_user_env": "true" if latest_per_user_env else "false"}
    if tag:
        params["tag"] = tag
    if q:
        params["q"] = q
    if username:
        params["username"] = username
    url = "%s/api/amyboardworld/files?%s" % (base, urllib.parse.urlencode(params))
    with urllib.request.urlopen(url, timeout=timeout) as r:
        items = json.load(r).get("items", [])
    # only real environment .py sketches
    return [it for it in items if str(it.get("filename", "")).endswith(".py")]


def download_sketch(item, base=DEFAULT_BASE, timeout=30):
    """Return the sketch's python source text for a list item."""
    url = item.get("download_url") or ("/api/amyboardworld/files/%s/download" % item["id"])
    if not url.startswith("http"):
        url = base + url
    with urllib.request.urlopen(url, timeout=timeout) as r:
        return r.read().decode("utf-8")


def fetch_sketch(name, author, base=DEFAULT_BASE, timeout=30):
    """Resolve `author`/`name` to a World sketch and return its python source.

    Resolves by name+author at run time (rather than a hard-coded id) so a
    re-upload doesn't silently fetch the wrong file."""
    items = list_sketches(base=base, limit=20, q=name, username=author, timeout=timeout)
    want = name + ".py"
    match = next((it for it in items if str(it.get("filename", "")) == want
                  and str(it.get("username", "")).lower() == author.lower()), None)
    match = match or next((it for it in items if str(it.get("filename", "")) == want), None)
    if not match:
        raise RuntimeError("World sketch %s/%s not found" % (author, name))
    return download_sketch(match, base=base, timeout=timeout)
