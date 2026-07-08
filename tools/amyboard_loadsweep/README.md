# amyboard_loadsweep

Answer "**when did this sketch start killing the AMYboard's CPU?**" by
building the AMYboard firmware from `main` as of each midnight UTC over a
date range, running the sketch on a physically connected board, and graphing
AMY's render load day by day. A discontinuity in the graph is the day to
bisect.

This is how we established that dpwe's `eno_ambient` saturates the synth
(render load pinned at ~0.99, board wedged on 11 of 14 days) on *every* build
from 2026-06-25 through 2026-07-08 — i.e. that regression predates the
window, and isn't any of the six amy pin bumps inside it.

## How it works

For each date in the range, [`sweep.py`](sweep.py):

1. resolves `origin/main` as of that midnight UTC (`git rev-list -1 --before`),
   deduplicating dates that share a commit;
2. checks the tree out in a throwaway clone (default
   `~/.cache/amyboard_loadsweep_ws`), syncs `amy`/`micropython` to that day's
   pins, and applies [`patch_render_load.py`](patch_render_load.py) — a
   self-contained backport of the render-load measurement from
   [amy#826](https://github.com/shorepine/amy/pull/826) that makes the
   fill-buffer task `fprintf` a smoothed `RENDER_LOAD ms=… load=…` line to
   stderr once per second (no failsafe, no API changes, applies cleanly to
   months of amy history);
3. builds with `idf.py -DMICROPY_BOARD=AMYBOARD` + `fs_create.py`
   (`IDF_CCACHE_ENABLE=1`, so 13 near-identical daily builds take minutes,
   not hours);
4. hands the **full merged image** to [`measure.py`](measure.py), which
   flashes it at `0x0` (always the full image — the partition table has
   changed over time, so app-only flashes onto an old table don't boot),
   uploads the sketch over USB-MIDI SysEx (via
   [`tools/amyboardctl`](../amyboardctl/)), and tails the debug UART for the
   `RENDER_LOAD` lines for N seconds (default 60);
5. writes `results/<date>/{firmware.bin,shas.txt,load.csv,serial.log,meta.json}`.

The sweep is resumable: re-running skips any date that already has a
`load.csv`, and reuses firmware for dates that resolve to an already-built
SHA. To re-measure a day, delete its `load.csv`.

[`plot.py`](plot.py) then renders one PNG: all days overlaid, per-day small
multiples, and a steady-state mean/max bar chart with amy pin changes marked
and wedged days flagged.

## Bench requirements

* An AMYboard on USB (enumerates as the `AMYboard` MIDI device), **plus** a
  USB-serial dongle on the debug header whose **DTR/RTS drive the board's
  auto-reset circuit** (DTR→IO0, RTS→EN — same wiring as
  [`tulip/amyboard/hwci`](../../tulip/amyboard/hwci/)). esptool needs it to
  enter the ROM bootloader unattended, and — the important part for a tool
  whose test case *wedges the board on purpose* — RTS recovers a dead board
  between days. Fallback when the handshake fails: `zP machine.bootloader()`
  over USB-MIDI, then flashing over the board's native USB; that only works
  while the board still answers MIDI.
* ESP-IDF v5.4.1 (the tulipcc build env; see repo `CLAUDE.md`/docs), with
  `littlefs-python` in its python env. Defaults assume
  `~/.espressif/python_env/idf5.4_py3.13_env` + `~/esp/esp-idf-v5.4.1`;
  override with `--idf-venv`/`--idf-export`, or pre-export the env.
  `ccache` recommended.
* Host python: `pip install ./tools/amyboardctl pyserial esptool matplotlib`.

## Usage

```sh
cd tools/amyboard_loadsweep

# 14 days ending today, sketch fetched from AMYboard World:
python3 sweep.py --days 14 --sketch-world dpwe/eno_ambient \
    --port /dev/cu.usbserial-A5069RR4 --native-port /dev/cu.usbmodem22234101

# graph it:
python3 plot.py --results results/

# extend the search further back (already-measured days are skipped):
python3 sweep.py --days 42 --sketch-world dpwe/eno_ambient

# one-off: flash any firmware and measure any local sketch for 90s
python3 measure.py results/2026-07-08/firmware.bin \
    --sketch mysketch.py --out /tmp/oneoff --seconds 90
```

Other useful flags: `--end YYYY-MM-DD` (sweep a historical window),
`--build-only` (prebuild all firmwares without touching the board),
`--seconds`, `--no-patch` (once amy's own render-load print has landed at the
pins being tested).

## Reading the results

* Load is `busy/(busy+blocked)` per 256-sample block, smoothed 0.05/block —
  the fraction of real time the fill-buffer task spends rendering vs parked
  in the i2s DMA write. ~1.0 means rendering can't keep up (audio underruns).
* `meta.json`: `wedged` = board stopped answering MIDI after the run;
  `serial_died_early` = the RENDER_LOAD prints themselves stopped (hard
  crash); `load_max`/`load_last` for quick triage.
* Generative sketches make survival-within-a-window partly luck — compare
  the steady-state mean (bottom panel of the plot), not just who wedged.
