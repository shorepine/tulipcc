# AMYboard World DB API

This is a DB-backed replacement for the current Discord-backed AMYboard World proxy.

## Run locally

```bash
cd tulip/server
python3 -m venv .venv
source .venv/bin/activate
pip install fastapi "uvicorn[standard]" python-multipart
uvicorn amyboardworld_db_api:app --reload --port 8090
```

## Configure storage

Optional environment variables:

- `AMYBOARDWORLD_DB_PATH` (default: `./amyboardworld.db`)
- `AMYBOARDWORLD_FILES_DIR` (default: `./amyboardworld_files`)
- `AMYBOARDWORLD_ADMIN_TOKEN` (required for moderation endpoints)
- `AMYBOARDWORLD_PUBLIC_BASE_URL` (optional base URL used in Discord upload messages)
- `AMYBOARDWORLD_DISCORD_BOT_TOKEN` (optional override for bot token used to post upload notifications)
- `AMYBOARDWORLD_DISCORD_CHANNEL_ID` (default: `1353327346924388422`, `#amyboard-world-files`)

## API surface

- `POST /api/amyboardworld/upload` multipart upload (`username`, `description`, `file`)
- `GET /api/amyboardworld/files` list/search (`q`, `tag`, `username`, `limit`)
- `GET /api/amyboardworld/files/{id}` metadata for one file
- `GET /api/amyboardworld/files/{id}/download` file bytes
- `PATCH /api/amyboardworld/files/{id}/tags` admin only (`x-admin-token`)
- `DELETE /api/amyboardworld/files/{id}` admin only (`x-admin-token`)

## Admin UI

- `GET /admin` serves a simple moderation interface.
- Supports: list/search, upload, download, tag updates, delete.
- Enter your `AMYBOARDWORLD_ADMIN_TOKEN` in the page to perform admin actions.

## Amyboardweb integration

`amyboardweb` can use this API by setting:

```html
<script>
  window.AMYBOARD_WORLD_API_BASE = "https://your-host";
</script>
```

When unset, the app falls back to the current Modal/Discord endpoints.
