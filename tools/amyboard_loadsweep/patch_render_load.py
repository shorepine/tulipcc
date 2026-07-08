#!/usr/bin/env python3
"""Inject a 1 Hz render-load stderr print into amy/src/i2s.c.

Self-contained minimal version of the measurement from shorepine/amy#826:
in esp_fill_audio_buffer_task, time the render work (busy) vs the i2s DMA
write / update-sync wait (blocked), keep a 0.05-per-block smoothed load =
busy/(busy+blocked), and fprintf it to stderr once per second as:

    RENDER_LOAD ms=<board_ms> load=<0..1>

No failsafe, no header/API changes -- the anchors are stable, so it applies
unmodified to months of amy history (the function was byte-identical across
every pin from 2026-06-25 to 2026-07-08). Idempotent; fails loudly if an
anchor is missing (e.g. a future amy where the measurement landed upstream --
use sweep.py --no-patch there and print amy's own render load instead).

Usage: patch_render_load.py <path-to-amy>/src/i2s.c
"""
import re
import sys

MARK = "/* rl-patch */"

DECLS = """
%s
#include "esp_timer.h"
#include <stdio.h>
static float _rl_load = 0.0f;
static int64_t _rl_last_print = 0;
""" % MARK


def main(path):
    src = open(path).read()
    if MARK in src:
        print(f"[patch] {path} already patched")
        return 0
    orig = src
    subs = []

    def sub(pattern, repl, label):
        # repl is a template using \1 \2 ... group refs; expand it via a
        # function so other backslashes (e.g. \n inside C strings) survive.
        nonlocal src

        def expand(m):
            return re.sub(r"\\(\d)", lambda g: m.group(int(g.group(1))) or "", repl)

        new, n = re.subn(pattern, expand, src, count=1)
        if n != 1:
            sys.exit(f"[patch] FAILED: anchor not found: {label}")
        src = new
        subs.append(label)

    # 0) statics + includes before the task function
    sub(r"(?m)^(// Make AMY's FABT run forever.*\n)?^void esp_fill_audio_buffer_task\(\) \{",
        DECLS + "void esp_fill_audio_buffer_task() {",
        "function head")

    # 1) locals at top of the while loop + busy timer start after the
    #    (blocked-time-tracked) audio-in read
    sub(r"(void esp_fill_audio_buffer_task\(\) \{\n    while\(1\) \{\n)",
        "\\1        int64_t _rl_t; uint32_t _rl_busy = 0, _rl_blocked = 0;\n",
        "loop head")

    sub(r"( *)if\(AMY_HAS_I2S && AMY_HAS_AUDIO_IN\) \{\n( *)esp_read_i2s_input\(\);\n(\s*)\}",
        "\\1if(AMY_HAS_I2S && AMY_HAS_AUDIO_IN) {\n"
        "\\2_rl_t = esp_timer_get_time();\n"
        "\\2esp_read_i2s_input();\n"
        "\\2_rl_blocked += (uint32_t)(esp_timer_get_time() - _rl_t);\n"
        "\\3}\n"
        "        _rl_t = esp_timer_get_time();",
        "audio-in block")

    # 2) busy time ends after amy_fill_buffer
    sub(r"( *)(output_sample_type \*block = amy_fill_buffer\(\);)",
        "\\1\\2\n\\1_rl_busy = (uint32_t)(esp_timer_get_time() - _rl_t);",
        "fill buffer")

    # 3) blocked time around the i2s write / sync wait, then smooth + 1 Hz print
    sub(r"( *)(if \(AMY_HAS_I2S\) \{\n"
        r" *amy_i2s_write\(\(uint8_t \*\)block, AMY_BLOCK_SIZE \* AMY_NCHANS \* sizeof\(int16_t\)\);\n"
        r" *\} else \{\n"
        r" *// Wait for update sync\.\n"
        r" *ulTaskNotifyTake\(pdTRUE, portMAX_DELAY\);.*\n"
        r" *\})\n",
        "\\1_rl_t = esp_timer_get_time();\n"
        "\\1\\2\n"
        "\\1_rl_blocked += (uint32_t)(esp_timer_get_time() - _rl_t);\n"
        "\\1if (_rl_busy + _rl_blocked > 0)\n"
        "\\1    _rl_load += 0.05f * ((float)_rl_busy / (float)(_rl_busy + _rl_blocked) - _rl_load);\n"
        "\\1{ int64_t _rl_now = esp_timer_get_time();\n"
        "\\1  if (_rl_now - _rl_last_print > 1000000) {\n"
        "\\1      _rl_last_print = _rl_now;\n"
        "\\1      fprintf(stderr, \"RENDER_LOAD ms=%lu load=%.4f\\n\", (unsigned long)(_rl_now/1000), _rl_load);\n"
        "\\1      fflush(stderr);\n"
        "\\1  } }\n",
        "i2s write block")

    open(path, "w").write(src)
    print(f"[patch] OK: {', '.join(subs)} -> {path}")
    assert src != orig
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
