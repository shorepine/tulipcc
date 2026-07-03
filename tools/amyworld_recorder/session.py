"""Session state for a recording run.

Stored as <out_dir>/session.json. Tracks the disposition of every sketch we've
touched so a later run can skip the ones already handled, and so the stitcher
knows which clips are good and what text to overlay.
"""

import json
import os
import time

import _bootstrap  # noqa: F401  (makes the in-repo amyboardctl importable)
from amyboardctl import world as worldapi

# Statuses that count as "handled" — skipped on the next run unless --redo.
DONE = {"good", "not_interesting", "not_working"}
ALL_STATUSES = DONE | {"pending"}


class Session:
    def __init__(self, out_dir):
        self.out_dir = out_dir
        self.path = os.path.join(out_dir, "session.json")
        self.denylist_path = os.path.join(out_dir, "denylist.json")
        self.data = {"created_at": time.time(), "sketches": {}}
        self.denylist = {}  # {str(id): {reason, filename, username, added_at}}
        os.makedirs(out_dir, exist_ok=True)
        if os.path.exists(self.path):
            self.load()
        if os.path.exists(self.denylist_path):
            with open(self.denylist_path) as f:
                self.denylist = json.load(f)

    def load(self):
        with open(self.path) as f:
            self.data = json.load(f)
        self.data.setdefault("sketches", {})

    def save(self):
        tmp = self.path + ".tmp"
        with open(tmp, "w") as f:
            json.dump(self.data, f, indent=2)
        os.replace(tmp, self.path)

    # -- queries ------------------------------------------------------------
    def get(self, sketch_id):
        return self.data["sketches"].get(str(sketch_id))

    def status(self, sketch_id):
        rec = self.get(sketch_id)
        return rec["status"] if rec else None

    def is_done(self, sketch_id):
        return self.status(sketch_id) in DONE

    def next_beep_index(self, sketch_id):
        """Record a sync beep for `sketch_id` and return its index. The Nth beep
        found in the phone recording maps to self.data['beeps'][N] (external-video
        mode) — so beeps stay aligned to sketches regardless of how each was
        finally marked."""
        beeps = self.data.setdefault("beeps", [])
        idx = len(beeps)
        beeps.append(str(sketch_id))
        self.save()
        return idx

    # -- denylist (sketches to never load again, e.g. they hang the board) ---
    def is_denied(self, sketch_id):
        return str(sketch_id) in self.denylist

    def deny(self, item, reason=""):
        """Add a sketch to the persistent denylist; it's skipped on every run."""
        sid = str(item["id"])
        self.denylist[sid] = {
            "id": item["id"],
            "filename": item.get("filename", ""),
            "username": worldapi.display_username(item.get("username", "")),
            "reason": reason,
            "added_at": time.time(),
        }
        self._save_denylist()
        return self.denylist[sid]

    def undeny(self, sketch_id):
        if str(sketch_id) in self.denylist:
            del self.denylist[str(sketch_id)]
            self._save_denylist()
            return True
        return False

    def _save_denylist(self):
        tmp = self.denylist_path + ".tmp"
        with open(tmp, "w") as f:
            json.dump(self.denylist, f, indent=2)
        os.replace(tmp, self.denylist_path)

    def by_status(self, status):
        return [r for r in self.data["sketches"].values() if r.get("status") == status]

    def counts(self):
        c = {s: 0 for s in ALL_STATUSES}
        for r in self.data["sketches"].values():
            c[r.get("status", "pending")] = c.get(r.get("status", "pending"), 0) + 1
        return c

    # -- writes -------------------------------------------------------------
    def record(self, item, status, clip=None, errors=None, pattern=None,
               started_at=None, ended_at=None, audition_in=None, transfer=None,
               beep_index=None):
        """Upsert the result for one sketch."""
        sid = str(item["id"])
        rec = self.data["sketches"].get(sid, {})
        rec.update({
            "id": item["id"],
            "filename": item.get("filename", ""),
            "title": str(item.get("filename", "")).rsplit(".", 1)[0],
            "username": worldapi.display_username(item.get("username", "")),
            "description": item.get("description", "") or item.get("content", ""),
            "tags": item.get("tags", []),
            "status": status,
            "updated_at": time.time(),
        })
        if clip is not None:
            rec["clip"] = clip
        if errors is not None:
            rec["errors"] = errors
        if pattern is not None:
            rec["pattern"] = pattern
        if started_at is not None:
            rec["started_at"] = started_at
        if ended_at is not None:
            rec["ended_at"] = ended_at
        if audition_in is not None:
            rec["audition_in"] = audition_in
        if transfer is not None:
            rec["transfer"] = transfer
        if beep_index is not None:
            rec["beep_index"] = beep_index
        self.data["sketches"][sid] = rec
        self.save()
        return rec
