#!/usr/bin/env python3
"""Tests for the hardware-tag detector that auto-tags published generator
sketches (#display, #encoder, #quad-encoder, #8encoder, #8angle). Pure local
string checks — no network, DB, or Anthropic API key required.
Run: python3 tulip/server/test_hardware_tags.py
(requires the server deps: fastapi, pydantic, requests)."""
from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import amyboardworld_db_api as m  # noqa: E402

_failures = 0


def check(name: str, cond: bool) -> None:
    global _failures
    status = "ok  " if cond else "FAIL"
    if not cond:
        _failures += 1
    print(f"[{status}] {name}")


def tags(code: str) -> list[str]:
    return m._hardware_tags_for_sketch(code)


# --- nothing hardware-y ---
check("plain synth sketch has no tags", tags("import amy\namy.send(synth=1, patch=256)\ndef loop():\n    pass\n") == [])
check("empty code has no tags", tags("") == [])
check("comment mentions don't count", tags("# use the display and encoder for this\nimport amy\ndef loop():\n    pass\n") == [])
check("description line doesn't count", tags("# DESCRIPTION: encoder controls the display brightness\nimport amy\ndef loop():\n    pass\n") == [])

# --- display ---
check("init_display", tags("import amyboard\namyboard.init_display()\namyboard.display.text('hi',0,0,255)\namyboard.display_refresh()") == ["display"])
check("display.fill_rect", tags("import amyboard\namyboard.display.fill_rect(0,0,10,10,255)\namyboard.display.show()") == ["display"])
check("draw_waveform helper", tags("import amyboard\ndef loop():\n    amyboard.draw_waveform()") == ["display"])

# --- encoder (unified API) ---
check("amyboard.encoder()", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    v = enc.read(0)") == ["encoder"])
check("legacy read_encoder", tags("import amyboard\ndef loop():\n    v = amyboard.read_encoder()") == ["encoder"])
check("legacy m5_8encoder module", "8encoder" in tags("import m5_8encoder\ndef loop():\n    pass"))

# --- encoder count evidence ---
check("index 1-3 means quad", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    a = enc.read(0)\n    b = enc.read(3)") == ["encoder", "quad-encoder"])
check("range(4) loop means quad", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    for i in range(4):\n        enc.read(i)") == ["encoder", "quad-encoder"])
check("forced adafruit_quad means quad", tags("import amyboard\nenc = amyboard.encoder(type='adafruit_quad')\ndef loop():\n    pass") == ["encoder", "quad-encoder"])
check("index 4-7 means 8encoder", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    enc.led(7, 255, 0, 0)") == ["encoder", "8encoder"])
check("range(8) loop means 8encoder", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    for i in range(8):\n        enc.read(i)") == ["encoder", "8encoder"])
check("forced m5stack means 8encoder", tags("import amyboard\nenc = amyboard.encoder(type=\"m5stack\")\ndef loop():\n    pass") == ["encoder", "8encoder"])
check("switch() means 8encoder", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    if enc.switch():\n        pass") == ["encoder", "8encoder"])
check("8 evidence beats 4 evidence", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    enc.read(2)\n    enc.read(6)") == ["encoder", "8encoder"])
check("adaptive enc.encoders loop stays plain #encoder", tags("import amyboard\nenc = amyboard.encoder()\ndef loop():\n    for i in range(enc.encoders):\n        enc.read(i)") == ["encoder"])

# --- 8angle ---
check("m58angle module", tags("import m58angle\ndef loop():\n    v = m58angle.get(0)") == ["8angle"])

# --- combinations ---
check("display + encoder", tags("import amyboard\namyboard.init_display()\nenc = amyboard.encoder()\ndef loop():\n    amyboard.display.text(str(enc.read(0)),0,0,255)\n    amyboard.display.show()") == ["display", "encoder"])
check("patch_selector implies display + encoder", tags("import amyboard\namyboard.patch_selector()\ndef loop():\n    pass") == ["display", "encoder"])

# --- against the bundled example sketches ---
SKETCHES = Path(__file__).resolve().parents[1] / "amyboardweb" / "sketches"


def sketch_tags(name: str) -> list[str]:
    return tags((SKETCHES / name).read_text(encoding="utf-8"))


check("preset_selector.py -> display + encoder", sketch_tags("preset_selector.py") == ["display", "encoder"])
check("acid_generator.py -> display", sketch_tags("acid_generator.py") == ["display"])
check("woodpiano.py -> display", sketch_tags("woodpiano.py") == ["display"])
check("arp.py -> no hardware tags", sketch_tags("arp.py") == [])
check("cvfilter.py -> no hardware tags", sketch_tags("cvfilter.py") == [])

# --- merge preserves non-hardware tags ---
check("merge keeps featured", m._merge_hardware_tags(["featured", "display"], ["encoder"]) == ["featured", "encoder"])
check("merge removes stale hw tags", m._merge_hardware_tags(["display", "encoder", "8encoder"], []) == [])
check("merge is idempotent", m._merge_hardware_tags(["featured", "display"], ["display"]) == ["featured", "display"])

# --- every hardware tag passes the API tag validator ---
for t in m.HARDWARE_TAGS:
    check(f"tag '{t}' is valid per TAG_RE", bool(m.TAG_RE.match(t)))

print()
if _failures:
    print(f"{_failures} FAILURES")
    sys.exit(1)
print("all ok")
