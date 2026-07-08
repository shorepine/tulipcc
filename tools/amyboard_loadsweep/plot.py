#!/usr/bin/env python3
"""Graph a render-load sweep produced by sweep.py.

Three views: all days overlaid, per-day small multiples, and a steady-state
mean/max bar chart (the discontinuity view) with amy submodule pin changes
marked. Wedged days (board stopped answering MIDI) are flagged.
"""
import argparse
import csv
import glob
import json
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt          # noqa: E402
import matplotlib.cm as cm               # noqa: E402
import numpy as np                       # noqa: E402


def load_days(results):
    days = []
    for d in sorted(glob.glob(os.path.join(results, "*/"))):
        csvp = os.path.join(d, "load.csv")
        if not os.path.exists(csvp):
            continue
        rows = [(float(r["t_s"]), float(r["load"]))
                for r in csv.DictReader(open(csvp)) if float(r["t_s"]) >= -1]
        meta = {}
        mp = os.path.join(d, "meta.json")
        if os.path.exists(mp):
            meta = json.load(open(mp))
        amy = ""
        sp = os.path.join(d, "shas.txt")
        if os.path.exists(sp):
            for line in open(sp):
                if line.startswith("amy "):
                    amy = line.split()[1][:8]
        days.append({"label": os.path.basename(d.rstrip("/")),
                     "rows": rows, "meta": meta, "amy": amy})
    return days


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--results", default=os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "results"))
    ap.add_argument("--out", default=None, help="output PNG "
                    "(default <results>/render_load_sweep.png)")
    ap.add_argument("--title", default="AMYboard render load by day of tulipcc main")
    args = ap.parse_args()
    out = args.out or os.path.join(args.results, "render_load_sweep.png")

    days = load_days(args.results)
    if not days:
        raise SystemExit(f"no load.csv files under {args.results}")
    n = len(days)
    tmax = max((r[0] for d in days for r in d["rows"]), default=60) + 2
    colors = cm.viridis(np.linspace(0.05, 0.95, n))

    fig = plt.figure(figsize=(14, 16))
    gs = fig.add_gridspec(3, 1, height_ratios=[1.1, 1.6, 0.8], hspace=0.35)

    # overlay
    ax = fig.add_subplot(gs[0])
    for day, c in zip(days, colors):
        t = [r[0] for r in day["rows"]]
        v = [r[1] for r in day["rows"]]
        wedge = " (WEDGED)" if day["meta"].get("wedged") else ""
        ax.plot(t, v, color=c, lw=1.8, label=day["label"] + wedge)
    ax.axhline(0.98, color="red", ls=":", lw=1, alpha=0.6)
    ax.text(0.2, 0.985, "overload threshold", color="red", fontsize=8, va="bottom")
    ax.set_xlim(-1, tmax)
    ax.set_ylim(0, 1.05)
    ax.set_xlabel("seconds since sketch start")
    ax.set_ylabel("AMY render load (0..1)")
    ax.set_title(args.title)
    ax.legend(fontsize=7.5, ncol=2, loc="lower right")
    ax.grid(alpha=0.25)

    # small multiples
    cols = 5
    rows_n = int(np.ceil(n / cols))
    sub = gs[1].subgridspec(rows_n, cols, hspace=0.45, wspace=0.25)
    for i, (day, c) in enumerate(zip(days, colors)):
        axs = fig.add_subplot(sub[i // cols, i % cols])
        t = [r[0] for r in day["rows"]]
        v = [r[1] for r in day["rows"]]
        axs.plot(t, v, color=c, lw=1.5)
        axs.fill_between(t, v, color=c, alpha=0.25)
        axs.set_xlim(-1, tmax)
        axs.set_ylim(0, 1.05)
        axs.axhline(0.98, color="red", ls=":", lw=0.7, alpha=0.6)
        title = day["label"][5:] + (" ⚠wedged" if day["meta"].get("wedged") else "")
        axs.set_title(f"{title}\namy {day['amy']}", fontsize=8)
        axs.tick_params(labelsize=7)
        if i % cols:
            axs.set_yticklabels([])

    # steady-state summary
    axb = fig.add_subplot(gs[2])
    labels = [d["label"][5:] for d in days]
    means, maxes, wedged = [], [], []
    for d in days:
        ss = [v for t, v in d["rows"] if t >= 5.0]
        means.append(np.mean(ss) if ss else 0)
        maxes.append(max(ss) if ss else 0)
        wedged.append(d["meta"].get("wedged", False))
    x = np.arange(n)
    axb.bar(x - 0.2, means, 0.4, color=list(colors), label="mean load (t≥5s)")
    axb.bar(x + 0.2, maxes, 0.4, color=list(colors), alpha=0.45,
            label="max load (t≥5s)")
    for i, w in enumerate(wedged):
        if w:
            axb.text(i, maxes[i] + 0.02, "wedged", ha="center", fontsize=7,
                     color="red", rotation=90)
    axb.axhline(0.98, color="red", ls=":", lw=1, alpha=0.6)
    axb.set_xticks(x)
    axb.set_xticklabels(labels, rotation=45, ha="right", fontsize=8)
    axb.set_ylim(0, 1.1)
    axb.set_ylabel("render load")
    axb.set_title("steady-state render load by day (discontinuity view)")
    axb.legend(fontsize=8)
    axb.grid(axis="y", alpha=0.25)

    prev = None
    for i, d in enumerate(days):
        if prev is not None and d["amy"] != prev:
            axb.axvline(i - 0.5, color="k", ls="--", lw=0.8, alpha=0.5)
            axb.text(i - 0.45, 1.04, f"amy→{d['amy']}", fontsize=6.5)
        prev = d["amy"]

    fig.savefig(out, dpi=130, bbox_inches="tight")
    print("wrote", out)
    for d, m, mx in zip(days, means, maxes):
        print(f"{d['label']}  amy={d['amy']}  mean={m:.3f}  max={mx:.3f}"
              f"{'  WEDGED' if d['meta'].get('wedged') else ''}")


if __name__ == "__main__":
    main()
