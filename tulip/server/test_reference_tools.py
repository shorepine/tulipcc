#!/usr/bin/env python3
"""Tests for the sketch-generator reference tools (search/read/list + the path
allowlist). No network or Anthropic API key required — these exercise only the
local tool handlers. Run: python3 tulip/server/test_reference_tools.py
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


# --- categories ---
check("4 categories", set(m._REFERENCE_CATEGORIES) == {"sketches", "amy_docs", "amyboard_docs", "py_libs"})

# --- list ---
all_list = m._ref_list(None)["content"]
check("list(all) includes a sketch", "tulip/amyboardweb/sketches/" in all_list)
check("list(all) includes vendored amy synth.md", "refdocs/amy/synth.md" in all_list)
check("list(amy_docs) only amy docs", "synth.md" in m._ref_list("amy_docs")["content"])
check("list(py_libs) includes sequencer.py", "tulip/shared/py/sequencer.py" in m._ref_list("py_libs")["content"])
check("list(bogus) is error", m._ref_list("bogus").get("is_error") is True)

# --- search ---
s = m._ref_search("mod_source", "sketches")
check("search mod_source finds sketches", s.get("is_error") is None and "mod_source" in s["content"])
check("search mod_source hits known sketch", ("xanadu.py" in s["content"]) or ("universal_hair.py" in s["content"]))
sa = m._ref_search("ctrl", "amy_docs")
check("search in amy_docs finds synth.md", "synth.md" in sa["content"])
check("search empty query is error", m._ref_search("", None).get("is_error") is True)
check("search bogus category is error", m._ref_search("x", "bogus").get("is_error") is True)
check("search no-match lists files to read", "Files you can read_reference" in m._ref_search("zzqqxx_nope", "sketches")["content"])

# --- read (happy paths) ---
r1 = m._ref_read("tulip/amyboardweb/sketches/house_generator.py")
check("read sketch returns content", r1.get("is_error") is None and "sequencer.tempo" in r1["content"])
r2 = m._ref_read("tulip/server/refdocs/amy/synth.md")
check("read vendored amy doc returns content", r2.get("is_error") is None and len(r2["content"]) > 1000)
r3 = m._ref_read("tulip/shared/py/sequencer.py")
check("read py lib returns content", r3.get("is_error") is None and "def " in r3["content"])

# --- read (security: everything off-list must be blocked) ---
blocked = [
    ("server source", "tulip/server/amyboardworld_db_api.py"),
    ("env file", ".env"),
    ("abs path", "/etc/passwd"),
    ("traversal", "../../../../etc/passwd"),
    ("traversal into server", "tulip/server/../server/amyboardworld_db_api.py"),
    ("non-md in amy refdocs", "tulip/server/refdocs/amy/_VENDORED_FROM.txt"),
    ("backslash", "tulip\\server\\amyboardworld_db_api.py"),
    ("empty", ""),
]
for label, p in blocked:
    res = m._ref_read(p)
    check(f"BLOCKED: {label}", res.get("is_error") is True)
    check(f"resolve None: {label}", m._resolve_reference_path(p) is None)

# allowed resolves to a real file
check("resolve allows a sketch", m._resolve_reference_path("tulip/amyboardweb/sketches/arp.py") is not None)

# --- dispatch ---
check("dispatch list", "reference file" in m._run_reference_tool("list_reference", {})["content"])
check("dispatch search", m._run_reference_tool("search_reference", {"query": "reverb"}).get("content") is not None)
check("dispatch unknown is error", m._run_reference_tool("nope", {}).get("is_error") is True)
check("dispatch bad input type tolerated", m._run_reference_tool("list_reference", None).get("content") is not None)

# --- agentic loop control flow (mocked Anthropic client, no network) ---
import types  # noqa: E402

_VALID_SKETCH = "import amy\namy.send(synth=1, patch=0, num_voices=1)\ndef loop():\n    pass\n"


def _ns(**kw):
    return types.SimpleNamespace(**kw)


def _usage():
    return _ns(input_tokens=10, output_tokens=5, cache_read_input_tokens=2)


def _tool_use(name="search_reference", inp=None, _id="t1"):
    return _ns(content=[_ns(type="tool_use", name=name, input=inp or {"query": "reverb"}, id=_id)],
               stop_reason="tool_use", usage=_usage())


def _final(text=_VALID_SKETCH):
    return _ns(content=[_ns(type="text", text=text)], stop_reason="end_turn", usage=_usage())


def _install_fake_anthropic(script):
    """Inject a fake `anthropic` module whose client replays `script` and records
    each create() kwargs into .calls. Returns the recorder client."""
    recorder = {"calls": []}

    class FakeClient:
        def __init__(self, *a, **k):
            self._script = list(script)
            self.messages = _ns(create=self._create)

        def _create(self, **kwargs):
            recorder["calls"].append(kwargs)
            return self._script.pop(0)

    mod = types.ModuleType("anthropic")
    mod.Anthropic = FakeClient
    mod.RateLimitError = type("RateLimitError", (Exception,), {})
    mod.AuthenticationError = type("AuthenticationError", (Exception,), {})
    sys.modules["anthropic"] = mod
    return recorder


_saved_anthropic = sys.modules.get("anthropic")
try:
    # Happy path: model searches, reads, then emits a valid sketch.
    rec = _install_fake_anthropic([_tool_use("search_reference"), _tool_use("read_reference", {"path": "x"}), _final()])
    res = m._generate_sketch_via_claude("a reverb pad", None)
    check("loop: ok", res["ok"] is True)
    check("loop: 2 tool calls serviced", res["tool_calls"] == 2)
    check("loop: returns the sketch", "import amy" in res["code"])
    check("loop: tokens accumulated across turns", res["input_tokens"] == 30 and res["output_tokens"] == 15)
    check("loop: first call sent tools", "tools" in rec["calls"][0])

    # Turn cap: model keeps calling tools forever; loop must stop and force a final answer.
    cap = m.GENERATE_MAX_TOOL_TURNS
    rec2 = _install_fake_anthropic([_tool_use() for _ in range(cap)] + [_final()])
    res2 = m._generate_sketch_via_claude("loop forever", None)
    check("cap: stopped at MAX_TOOL_TURNS", res2["tool_calls"] == cap)
    check("cap: still produced a valid sketch", res2["ok"] is True)
    check("cap: final call dropped tools", "tools" not in rec2["calls"][-1])

    # Corrective retry: first final is invalid, retry yields a valid sketch.
    rec3 = _install_fake_anthropic([_final("print('not a sketch')"), _final()])
    res3 = m._generate_sketch_via_claude("oops", None)
    check("retry: recovered to ok", res3["ok"] is True)
    check("retry: made a second (corrective) call", len(rec3["calls"]) == 2)
finally:
    if _saved_anthropic is not None:
        sys.modules["anthropic"] = _saved_anthropic
    else:
        sys.modules.pop("anthropic", None)

print()
if _failures:
    print(f"{_failures} FAILURE(S)")
    sys.exit(1)
print("ALL PASSED")
