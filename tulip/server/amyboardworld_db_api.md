# World DB API (Railway)

DB-backed replacement for legacy Discord/Modal Tulip World and AMYboard World flows.

## Run locally

```bash
cd tulip/server
python3 -m venv .venv
source .venv/bin/activate
pip install fastapi "uvicorn[standard]" python-multipart requests
uvicorn amyboardworld_db_api:app --reload --port 8090
```

## Configure storage

Optional environment variables:

- `WORLD_DB_PATH` (default: `./amyboardworld.db`)
- `AMYBOARDWORLD_FILES_DIR` (default: `./amyboardworld_files`)
- `TULIPWORLD_FILES_DIR` (default: `./tulipworld_files`)
- `WORLD_ADMIN_TOKEN` (shared admin token for moderation routes)
- `WORLD_DISCORD_BOT_TOKEN` (optional Discord mirror-post token)
- `AMYBOARDWORLD_DISCORD_CHANNEL_ID` (default: `#amyboard-world-files`)
- `TULIPWORLD_DISCORD_TEXT_CHANNEL_ID` (default: `#tulip-world`)
- `TULIPWORLD_DISCORD_FILES_CHANNEL_ID` (default: `#tulip-world-files`)

## API surface

AMYboard World file API (existing):

- `POST /api/amyboardworld/upload` (optional `created_at_ms` for backfills)
- `GET /api/amyboardworld/files`
- `GET /api/amyboardworld/files/{id}`
- `GET /api/amyboardworld/files/{id}/download`
- `PATCH /api/amyboardworld/files/{id}/tags` (admin)
- `DELETE /api/amyboardworld/files/{id}` (admin)

Tulip World file API (new):

- `POST /api/tulipworld/upload` (optional `created_at_ms` for backfills)
- `GET /api/tulipworld/files`
- `GET /api/tulipworld/files/{id}`
- `GET /api/tulipworld/files_latest?filename=...&username=...` (optional `username`; returns latest match)
- `GET /api/tulipworld/files/{id}/download`
- `PATCH /api/tulipworld/files/{id}/tags` (admin)
- `DELETE /api/tulipworld/files/{id}` (admin)

Tulip World text API (new):

- `POST /api/tulipworld/messages` (JSON or form, optional `created_at_ms` for backfills)
- `GET /api/tulipworld/messages`
- `DELETE /api/tulipworld/messages/{id}` (admin)

Unified admin API (new):

- `GET /api/admin/items` (admin; scopes: `all`, `amyboard_files`, `tulip_files`, `tulip_messages`)

## Admin UI

- `GET /admin` serves a unified moderation interface.
- One admin token can moderate AMY files, Tulip files, and Tulip messages.

## Migration script

Use `tulip/server/migrate_tulipworld_from_discord.py` to import legacy Tulip data:

```bash
cd tulip/server
python3 migrate_tulipworld_from_discord.py \
  --api-base https://tulipcc-production.up.railway.app \
  --max-messages 100
```

By default it imports all file attachments from legacy Tulip files channel and latest 100 Tulip text messages.
Use `--dry-run` first to verify parsing and counts.
