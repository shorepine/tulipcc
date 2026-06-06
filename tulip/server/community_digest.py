#!/usr/bin/env python3
"""Community digest helper for the tulip/amy/amyboard scheduled routine.

Stdlib only (no pip deps) so it runs anywhere python3 is available, including a
laptop Claude Code routine without a venv. GitHub activity is read via the `gh`
CLI; Discord via the REST API.

Token handling: the Discord bot token is read from the environment
(WORLD_DISCORD_BOT_TOKEN, matching the server). If it is not already in the
environment, it is loaded from a local, gitignored env file -- by default
~/.config/tulip-digest.env, overridable with $TULIP_DIGEST_ENV. The token is
never printed, and this helper never touches Railway or any other secret store.

  ~/.config/tulip-digest.env  (chmod 600):
      WORLD_DISCORD_BOT_TOKEN=...

Subcommands:
  gather                   Emit recent GitHub + Discord activity as JSON.
  resolve-channel <name>   Find a Discord channel id by name across the bot's guilds.
  read-channel <id>        Print recent messages in a channel (activity / approvals).
  post <id>                Post a message to a channel (body from --text or stdin).
  delete <id> <msg>        Delete a message (used to clean up test posts).
  edit <id> <msg>          Replace a message's content (body from --text or stdin).
"""
import argparse
import datetime
import json
import os
import subprocess
import sys
import urllib.error
import urllib.request

DISCORD_API = "https://discord.com/api/v10"

# Env var names to check (in order). Matches amyboardworld_db_api.py:90.
TOKEN_ENV_CANDIDATES = (
    "WORLD_DISCORD_BOT_TOKEN",
    "AMYBOARDWORLD_DISCORD_BOT_TOKEN",
)

# GitHub repos to watch. amyboard has no separate repo -- its issues live in
# tulipcc.
GH_REPOS = ("shorepine/tulipcc", "shorepine/amy")

# Discord channels to scan for people having issues. Bot-feed / automated
# channels (#github, #tulip-world*, #amyboard-world-files) are intentionally
# excluded -- GitHub activity is gathered directly via gh. Adjust freely.
SCAN_CHANNELS = (
    ("general", "1126544387942400106"),
    ("tulip", "1126572351618822338"),
    ("amy", "1126572379930361896"),
    ("amyboard", "1332047647120424993"),
    ("tulip-app-dev", "1324829560243621919"),
    ("your-projects", "1510990549040107653"),
)

# Discord history is fetched one page deep (100 messages/channel). For a 12-24h
# window in this community that is ample; a busier window could miss older
# messages. Bump to pagination if that ever becomes real.
DISCORD_PAGE = 100


# --------------------------------------------------------------------------
# Token / env
# --------------------------------------------------------------------------
def _env_file_path():
    return os.environ.get("TULIP_DIGEST_ENV") or os.path.expanduser(
        "~/.config/tulip-digest.env"
    )


def _load_env_file():
    """Populate os.environ from a simple KEY=VALUE file, without overwriting."""
    path = _env_file_path()
    try:
        with open(path, encoding="utf-8") as fh:
            lines = fh.readlines()
    except OSError:
        return
    for raw in lines:
        line = raw.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, val = line.split("=", 1)
        key = key.strip()
        val = val.strip().strip('"').strip("'")
        if key and key not in os.environ:
            os.environ[key] = val


def get_token():
    for env in TOKEN_ENV_CANDIDATES:
        if os.environ.get(env):
            return os.environ[env]
    _load_env_file()
    for env in TOKEN_ENV_CANDIDATES:
        if os.environ.get(env):
            return os.environ[env]
    raise SystemExit(
        f"Discord bot token not found. Set WORLD_DISCORD_BOT_TOKEN in the "
        f"environment or in {_env_file_path()} (KEY=VALUE, chmod 600)."
    )


# --------------------------------------------------------------------------
# Discord REST
# --------------------------------------------------------------------------
def discord_request(method, path, token, body=None):
    url = f"{DISCORD_API}{path}"
    data = json.dumps(body).encode() if body is not None else None
    req = urllib.request.Request(url, data=data, method=method)
    req.add_header("Authorization", f"Bot {token}")
    req.add_header("User-Agent",
                   "TulipCC-CommunityDigest/0.1 (https://tulip.computer)")
    if data is not None:
        req.add_header("Content-Type", "application/json")
    try:
        with urllib.request.urlopen(req, timeout=20) as resp:
            raw = resp.read().decode()
            return json.loads(raw) if raw else None
    except urllib.error.HTTPError as e:
        detail = e.read().decode(errors="replace")
        raise SystemExit(f"Discord {method} {path} -> HTTP {e.code}: {detail}")
    except urllib.error.URLError as e:
        raise SystemExit(f"Discord {method} {path} -> {e}")


# --------------------------------------------------------------------------
# Gather: GitHub (via gh) + Discord activity since a cutoff
# --------------------------------------------------------------------------
def _parse_iso(s):
    if not s:
        return None
    try:
        return datetime.datetime.fromisoformat(s.replace("Z", "+00:00"))
    except ValueError:
        return None


def _gh(args):
    """Run a gh command, returning parsed JSON or None on any failure."""
    try:
        out = subprocess.run(["gh"] + args, capture_output=True, text=True,
                             timeout=60)
    except (OSError, subprocess.SubprocessError):
        return None
    if out.returncode != 0:
        return None
    try:
        return json.loads(out.stdout or "null")
    except json.JSONDecodeError:
        return None


def _gh_items(repo, kind, cutoff_dt):
    """Open issues or PRs updated since cutoff. kind in {'issue','pr'}."""
    fields = "number,title,author,updatedAt,createdAt,labels,url"
    if kind == "pr":
        fields += ",isDraft"
    data = _gh([kind, "list", "--repo", repo, "--state", "open",
                "--json", fields, "--limit", "100"]) or []
    out = []
    for it in data:
        dt = _parse_iso(it.get("updatedAt"))
        if not dt or dt < cutoff_dt:
            continue
        row = {
            "number": it.get("number"),
            "title": it.get("title"),
            "author": (it.get("author") or {}).get("login"),
            "updatedAt": it.get("updatedAt"),
            "createdAt": it.get("createdAt"),
            "labels": [l.get("name") for l in it.get("labels", [])],
            "url": it.get("url"),
        }
        if kind == "pr":
            row["isDraft"] = it.get("isDraft")
        out.append(row)
    return out


def _gh_discussions(repo, cutoff_dt):
    owner, name = repo.split("/")
    query = (
        "query($owner:String!,$name:String!){"
        "repository(owner:$owner,name:$name){"
        "discussions(first:30,orderBy:{field:UPDATED_AT,direction:DESC}){"
        "nodes{number title url updatedAt createdAt author{login}"
        " category{name} comments{totalCount} answerChosenAt}}}}"
    )
    data = _gh(["api", "graphql", "-f", f"query={query}",
                "-f", f"owner={owner}", "-f", f"name={name}"])
    nodes = (((data or {}).get("data") or {}).get("repository") or {})
    nodes = (nodes.get("discussions") or {}).get("nodes") or []
    out = []
    for d in nodes:
        dt = _parse_iso(d.get("updatedAt"))
        if not dt or dt < cutoff_dt:
            continue
        out.append({
            "number": d.get("number"),
            "title": d.get("title"),
            "author": (d.get("author") or {}).get("login"),
            "category": (d.get("category") or {}).get("name"),
            "updatedAt": d.get("updatedAt"),
            "createdAt": d.get("createdAt"),
            "comments": (d.get("comments") or {}).get("totalCount"),
            "answered": bool(d.get("answerChosenAt")),
            "url": d.get("url"),
        })
    return out


def _discord_recent(channel_id, token, cutoff_dt):
    msgs = discord_request(
        "GET", f"/channels/{channel_id}/messages?limit={DISCORD_PAGE}", token
    ) or []
    out = []
    for m in msgs:
        dt = _parse_iso(m.get("timestamp"))
        if not dt or dt < cutoff_dt:
            continue
        author = m.get("author", {})
        if author.get("bot"):
            continue  # skip webhook feeds and the bot's own posts
        out.append({
            "id": m.get("id"),
            "author": author.get("username"),
            "timestamp": m.get("timestamp"),
            "content": m.get("content", ""),
            "reply_to": (m.get("referenced_message") or {}).get("id"),
        })
    return list(reversed(out))  # oldest first


def cmd_gather(args):
    cutoff_dt = (datetime.datetime.now(datetime.timezone.utc)
                 - datetime.timedelta(hours=args.hours))
    result = {
        "generated_since": cutoff_dt.strftime("%Y-%m-%dT%H:%M:%SZ"),
        "window_hours": args.hours,
        "github": {},
        "discord": {},
    }
    for repo in GH_REPOS:
        result["github"][repo] = {
            "issues": _gh_items(repo, "issue", cutoff_dt),
            "prs": _gh_items(repo, "pr", cutoff_dt),
            "discussions": _gh_discussions(repo, cutoff_dt),
        }
    token = get_token()
    for name, cid in SCAN_CHANNELS:
        result["discord"][name] = _discord_recent(cid, token, cutoff_dt)
    print(json.dumps(result, indent=2, ensure_ascii=False))


# --------------------------------------------------------------------------
# Discord one-shot helpers (resolve / read / post / delete)
# --------------------------------------------------------------------------
def cmd_resolve_channel(args):
    token = get_token()
    name = args.name.lstrip("#").lower()
    guilds = discord_request("GET", "/users/@me/guilds", token) or []
    found, allch = [], []
    for g in guilds:
        gid, gname = g["id"], g.get("name", "?")
        channels = discord_request("GET", f"/guilds/{gid}/channels", token) or []
        for c in channels:
            entry = (gname, gid, c.get("name", "?"), c["id"], c.get("type"))
            allch.append(entry)
            if c.get("name", "").lower() == name:
                found.append(entry)
    if found:
        print("MATCHES:")
        for gname, gid, cname, cid, ctype in found:
            print(f"  guild={gname} ({gid})  #{cname}  channel_id={cid}  type={ctype}")
    else:
        print(f"No channel named '{name}'. Channels the bot can see:")
        for gname, gid, cname, cid, ctype in sorted(allch):
            print(f"  guild={gname} ({gid})  #{cname}  id={cid}  type={ctype}")


def cmd_read_channel(args):
    token = get_token()
    path = f"/channels/{args.channel}/messages?limit={args.limit}"
    if args.after:
        path += f"&after={args.after}"
    msgs = discord_request("GET", path, token) or []
    for m in reversed(msgs):  # oldest first
        author = m.get("author", {})
        uname = author.get("username", "?")
        bot = " (bot)" if author.get("bot") else ""
        ts = m.get("timestamp", "")
        content = (m.get("content") or "").replace("\n", " ")
        print(f"[{ts}] {uname}{bot} {m['id']}: {content[:200]}")


def cmd_post(args):
    token = get_token()
    text = args.text if args.text is not None else sys.stdin.read()
    msg = discord_request("POST", f"/channels/{args.channel}/messages",
                          token, {"content": text})
    print(msg["id"] if msg else "")


def cmd_delete(args):
    token = get_token()
    discord_request("DELETE",
                    f"/channels/{args.channel}/messages/{args.message}", token)
    print(f"deleted {args.message}")


def cmd_edit(args):
    token = get_token()
    text = args.text if args.text is not None else sys.stdin.read()
    msg = discord_request("PATCH",
                          f"/channels/{args.channel}/messages/{args.message}",
                          token, {"content": text})
    print(msg["id"] if msg else "")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    sub = p.add_subparsers(dest="cmd", required=True)

    ga = sub.add_parser("gather")
    ga.add_argument("--hours", type=int, default=24,
                    help="look-back window in hours (default 24)")
    ga.set_defaults(func=cmd_gather)

    rc = sub.add_parser("resolve-channel")
    rc.add_argument("name")
    rc.set_defaults(func=cmd_resolve_channel)

    rd = sub.add_parser("read-channel")
    rd.add_argument("channel")
    rd.add_argument("--limit", type=int, default=30)
    rd.add_argument("--after", default=None)
    rd.set_defaults(func=cmd_read_channel)

    po = sub.add_parser("post")
    po.add_argument("channel")
    po.add_argument("--text", default=None)
    po.set_defaults(func=cmd_post)

    dl = sub.add_parser("delete")
    dl.add_argument("channel")
    dl.add_argument("message")
    dl.set_defaults(func=cmd_delete)

    ed = sub.add_parser("edit")
    ed.add_argument("channel")
    ed.add_argument("message")
    ed.add_argument("--text", default=None)
    ed.set_defaults(func=cmd_edit)

    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
