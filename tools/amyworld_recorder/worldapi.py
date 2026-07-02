"""Thin client for the AMYboard World sketch API (read-only, public).

Backed by tulip/server/amyboardworld_db_api.py; production base is
https://tulipcc-production.up.railway.app
"""

import requests

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
    r = requests.get(base + "/api/amyboardworld/files", params=params, timeout=timeout)
    r.raise_for_status()
    items = r.json().get("items", [])
    # only real environment .py sketches
    return [it for it in items if str(it.get("filename", "")).endswith(".py")]


def download_sketch(item, base=DEFAULT_BASE, timeout=30):
    """Return the sketch's python source text for a list item."""
    url = item.get("download_url") or ("/api/amyboardworld/files/%s/download" % item["id"])
    if not url.startswith("http"):
        url = base + url
    r = requests.get(url, timeout=timeout)
    r.raise_for_status()
    return r.text
