# Community digest bot

A scheduled Claude Code agent that, every ~12 hours, scans GitHub
(shorepine/tulipcc + shorepine/amy issues/PRs/discussions) and the community
Discord channels, then posts a triaged digest to the private **#admin-actions**
channel: a short "what's going on", a list of **simple actions it recommends**
(you approve), and a list of **things that need your call**.

- Helper / data layer: [`community_digest.py`](community_digest.py)
- Routine prompt (the agent's instructions): [`community_digest_routine.md`](community_digest_routine.md)

## Token

The Discord bot token (`WORLD_DISCORD_BOT_TOKEN`, the same one the world DB API
uses) is read from the environment, or from a local gitignored file:

```
~/.config/tulip-digest.env   (chmod 600)
    WORLD_DISCORD_BOT_TOKEN=...
```

The helper is stdlib-only and **never** prints the token or reads Railway. To
provision the file from Railway (run in your own terminal):

```bash
mkdir -p ~/.config && railway variables --json | python3 -c '
import json, os, sys
d = json.load(sys.stdin)
t = d.get("WORLD_DISCORD_BOT_TOKEN") or d.get("AMYBOARDWORLD_DISCORD_BOT_TOKEN")
assert t, "no Discord bot-token var found"
p = os.path.expanduser("~/.config/tulip-digest.env")
open(p, "w").write("WORLD_DISCORD_BOT_TOKEN=" + t + "\n"); os.chmod(p, 0o600)
print("wrote", p)'
```

## CLI

```
python3 tulip/server/community_digest.py gather [--hours N]   # GitHub + Discord activity → JSON
python3 tulip/server/community_digest.py read-channel <id> [--limit N] [--after <msg_id>]
python3 tulip/server/community_digest.py post <id> [--text T] # or pipe body on stdin
python3 tulip/server/community_digest.py delete <id> <msg_id>
python3 tulip/server/community_digest.py resolve-channel <name>
```

Config lives at the top of `community_digest.py`: `GH_REPOS`, `SCAN_CHANNELS`
(the 6 human Discord channels scanned — edit freely), and the #admin-actions
channel id used by the routine. Discord history is fetched one page deep
(100 msgs/channel); ample for a 12–24h window.

## Scheduling (local Desktop task — recommended)

The routine needs your local `gh` auth and the local token file, so it runs
**on your Mac**, not in a cloud routine. (Cloud routines can't see local files /
`gh` auth; they'd need the token as a cloud env var + the Claude GitHub App.)

1. **The code must be in the checkout the task runs from** — merge this to
   `main` (or wherever the task's working dir points).
2. **Grant the routine its one command** in `~/.claude/settings.json` (you must
   add this yourself — agents can't self-grant permissions). Merge into the
   existing `permissions.allow` array:

   ```json
   {
     "permissions": {
       "allow": [
         "Bash(python3 tulip/server/community_digest.py:*)"
       ]
     }
   }
   ```

   That single rule is all v1 needs: the helper invokes `gh` and Discord as
   subprocesses, so they aren't separately gated, and v1 never runs `gh`-write
   commands.
3. **Create the task:** Desktop app → Routines → New → **Local**. Working dir =
   the repo root. Instructions: *"Run the community digest routine exactly as
   described in tulip/server/community_digest_routine.md."* Schedule: every 12h.
   Permission mode: **`dontAsk`** (only pre-approved commands run; no prompts).

The task fires when your Mac is powered on (it catches up on wake).

### launchd fallback (if you don't want the desktop app to manage it)

```bash
claude -p "$(cat /Users/bwhitman/outside/tulipcc/tulip/server/community_digest_routine.md)" \
  --permission-mode dontAsk --output-format json >> ~/.config/tulip-digest.log 2>&1
```

Wrap that in a `launchd` plist with `StartInterval 43200` (12h).

## v1 vs v2

- **v1 (this):** the agent gathers, triages, and **posts** the digest. It only
  *recommends* the simple actions — **you** run them by asking Claude Code,
  referencing the digest's item numbers (e.g. "do 1 and 4"). The digest does not
  invite Discord replies. Minimal powers: read GitHub, read Discord, post to
  #admin-actions.
- **v2 (later):** reply `do 1 4 5` in #admin-actions and the next run executes
  those approved items (file the bug, add the FAQ line, comment on the issue).
  That needs (a) the agent granted scoped `gh`-write + post permissions, and
  (b) a small state file mapping the digest's action numbers → concrete
  commands so the next run knows what "1, 4, 5" mean.
