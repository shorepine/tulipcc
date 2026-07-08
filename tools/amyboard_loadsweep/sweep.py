#!/usr/bin/env python3
"""Day-by-day AMYboard render-load sweep over tulipcc main history.

For each date in the range: resolve `main` as of that midnight UTC, build the
AMYboard firmware from that tree in a throwaway clone (with amy patched to
print its render load — see patch_render_load.py), flash a connected board
with the full merged image, run a sketch for N seconds, and record the load
trace. Then plot.py graphs the traces to spot the day a regression landed.

    # 14 days ending today, dpwe's eno_ambient from AMYboard World:
    python3 sweep.py --days 14 --sketch-world dpwe/eno_ambient

    # then:
    python3 plot.py --results results/

Builds are resumable and deduplicated: a date whose load.csv exists is
skipped, and dates that resolve to the same tulipcc SHA share one firmware.
A wedged board (the point of the exercise) is recovered by the flash's
DTR/RTS reset lines — see measure.py for the bench wiring this needs.

Run this with the ESP-IDF v5.4.1 environment available: either already
exported, or at the default install paths (see --idf-*). The IDF python env
needs littlefs-python (for fs_create.py).
"""
import argparse
import datetime
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, "..", ".."))

MICROPYTHON_LIBS = ["lib/axtls", "lib/libffi", "lib/mbedtls",
                    "lib/berkeley-db-1.xx", "lib/micropython-lib", "lib/tinyusb"]


def run(cmd, cwd=None, check=True, quiet=False, env=None):
    r = subprocess.run(cmd, cwd=cwd, text=True, env=env,
                       capture_output=quiet)
    if check and r.returncode != 0:
        if quiet:
            sys.stderr.write((r.stdout or "")[-2000:] + (r.stderr or "")[-2000:])
        raise RuntimeError(f"command failed: {cmd}")
    return r


def git(repo, *args, capture=True):
    r = subprocess.run(["git", "-C", repo] + list(args), text=True,
                       capture_output=capture)
    if r.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)}: {r.stderr.strip()[:500]}")
    return (r.stdout or "").strip()


def resolve_days(args):
    """[(label, sha)] for each midnight UTC in the range, oldest first."""
    git(REPO, "fetch", "origin", "main")
    end = (datetime.date.fromisoformat(args.end) if args.end
           else datetime.datetime.now(datetime.timezone.utc).date())
    days = []
    for i in range(args.days - 1, -1, -1):
        d = end - datetime.timedelta(days=i)
        # --first-parent: walk main's merge spine only, so a PR commit authored
        # before midnight but merged after doesn't masquerade as that day's main
        sha = git(REPO, "rev-list", "-1", "--first-parent",
                  f"--before={d}T00:00:00Z", "origin/main")
        if not sha:
            print(f"[days] {d}: no commit before this date on origin/main, skipping")
            continue
        days.append((str(d), sha))
    return days


def init_workspace(ws):
    if os.path.exists(os.path.join(ws, ".git")):
        # pick up any new commits from the source repo
        git(ws, "fetch", REPO,
            "+refs/remotes/origin/main:refs/heads/upstream-main")
        return
    print(f"[ws] cloning {REPO} -> {ws}")
    run(["git", "clone", REPO, ws], quiet=True)
    git(ws, "fetch", REPO, "+refs/remotes/origin/main:refs/heads/upstream-main")
    # submodules fetch from the local checkouts when present (fast, offline);
    # a worktree without them falls back to the .gitmodules URLs
    for sub in ("amy", "micropython"):
        local = os.path.join(REPO, sub)
        if os.path.exists(os.path.join(local, ".git")):
            git(ws, "config", f"submodule.{sub}.url", local)


def build_day(ws, sha, label, outdir, args):
    fw = os.path.join(outdir, "firmware.bin")
    if os.path.exists(fw):
        print(f"== {label}: firmware already built")
        return fw
    print(f"== {label}: building {sha[:12]}")
    git(ws, "checkout", "-q", "-f", sha)
    git(ws, "-c", "protocol.file.allow=always", "submodule", "update",
        "--init", "--force", "amy", "micropython")
    git(os.path.join(ws, "micropython"), "submodule", "update", "--init",
        *MICROPYTHON_LIBS)
    # keep .submodules_ok so grab_submodules only (re)applies its mpy-cross
    # Makefile warning patches (needed whenever the micropython pin moved)
    open(os.path.join(ws, ".submodules_ok"), "w").close()
    run(["./grab_submodules.sh"], cwd=os.path.join(ws, "tulip", "shared"),
        quiet=True)

    if not args.no_patch:
        run([sys.executable, os.path.join(HERE, "patch_render_load.py"),
             os.path.join(ws, "amy", "src", "i2s.c")])

    # build inside the IDF environment; ccache makes repeated daily builds fast
    build_sh = (
        f'[ -n "${{IDF_PATH:-}}" ] || {{ source {args.idf_venv}/bin/activate && '
        f'source {args.idf_export} > /dev/null; }} && '
        f'export IDF_CCACHE_ENABLE=1 && '
        f'cd "{ws}/tulip/amyboard" && rm -rf build && '
        f'idf.py -DMICROPY_BOARD=AMYBOARD build && '
        f'cd .. && python fs_create.py amyboard'
    )
    log = os.path.join(outdir, "build.log")
    with open(log, "w") as lf:
        r = subprocess.run(["bash", "-c", build_sh], stdout=lf,
                           stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0:
        print(f"== {label}: BUILD FAILED (see {log})")
        return None

    dist = os.path.join(ws, "tulip", "amyboard", "dist")
    full = [f for f in sorted(os.listdir(dist)) if f.startswith("amyboard-full")]
    if not full:
        print(f"== {label}: no amyboard-full*.bin in {dist}")
        return None
    shutil.copy(os.path.join(dist, full[0]), fw)
    amy_sha = git(os.path.join(ws, "amy"), "rev-parse", "HEAD")
    with open(os.path.join(outdir, "shas.txt"), "w") as f:
        f.write(f"tulipcc {sha}\namy {amy_sha}\n")
    print(f"== {label}: built -> {fw}")
    return fw


def find_built(results, sha):
    """A results dir whose firmware was already built from this tulipcc SHA."""
    for label in sorted(os.listdir(results)):
        d = os.path.join(results, label)
        shas = os.path.join(d, "shas.txt")
        if os.path.exists(os.path.join(d, "firmware.bin")) and os.path.exists(shas):
            if f"tulipcc {sha}" in open(shas).read():
                return d
    return None


def fetch_world_sketch(spec, dest):
    sys.path.insert(0, os.path.join(REPO, "tools", "amyboardctl"))
    from amyboardctl import world
    author, name = spec.split("/", 1)
    name = name.removesuffix(".py")
    text = world.fetch_sketch(name, author)
    open(dest, "w").write(text)
    print(f"[sketch] fetched {spec} ({len(text)} bytes)")
    return dest


def main():
    ap = argparse.ArgumentParser(
        description=__doc__.splitlines()[0],
        formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    ap.add_argument("--days", type=int, default=14, help="number of daily samples")
    ap.add_argument("--end", default=None,
                    help="last date (YYYY-MM-DD, default today UTC)")
    sk = ap.add_mutually_exclusive_group(required=True)
    sk.add_argument("--sketch-world", help="AMYboard World sketch author/name, "
                    "e.g. dpwe/eno_ambient")
    sk.add_argument("--sketch-file", help="local sketch .py to run")
    ap.add_argument("--results", default=os.path.join(HERE, "results"))
    ap.add_argument("--workspace",
                    default=os.path.expanduser("~/.cache/amyboard_loadsweep_ws"),
                    help="throwaway tulipcc clone used for historical builds")
    ap.add_argument("--seconds", type=float, default=60.0,
                    help="capture window per day")
    ap.add_argument("--port", default="/dev/cu.usbserial-A5069RR4",
                    help="debug-UART dongle (flash + console)")
    ap.add_argument("--native-port", default="/dev/cu.usbmodem22234101",
                    help="board native USB (bootloader fallback)")
    ap.add_argument("--no-patch", action="store_true",
                    help="don't patch amy (its render-load print already landed)")
    ap.add_argument("--build-only", action="store_true",
                    help="build all firmwares, skip the hardware runs")
    ap.add_argument("--idf-venv", default=os.path.expanduser(
        "~/.espressif/python_env/idf5.4_py3.13_env"))
    ap.add_argument("--idf-export", default=os.path.expanduser(
        "~/esp/esp-idf-v5.4.1/export.sh"))
    args = ap.parse_args()

    os.makedirs(args.results, exist_ok=True)
    days = resolve_days(args)
    uniq = len({sha for _, sha in days})
    print(f"[days] {len(days)} dates, {uniq} unique builds")

    sketch = args.sketch_file
    if args.sketch_world:
        sketch = os.path.join(args.results, "sketch.py")
        fetch_world_sketch(args.sketch_world, sketch)

    init_workspace(args.workspace)

    failed = []
    for label, sha in days:
        outdir = os.path.join(args.results, label)
        os.makedirs(outdir, exist_ok=True)
        if os.path.exists(os.path.join(outdir, "load.csv")):
            print(f"== {label}: already measured, skipping")
            continue
        fw = os.path.join(outdir, "firmware.bin")
        if not os.path.exists(fw):
            prev = find_built(args.results, sha)
            if prev:
                print(f"== {label}: reusing firmware from {os.path.basename(prev)}")
                shutil.copy(os.path.join(prev, "firmware.bin"), fw)
                shutil.copy(os.path.join(prev, "shas.txt"),
                            os.path.join(outdir, "shas.txt"))
            elif not build_day(args.workspace, sha, label, outdir, args):
                failed.append(label)
                continue
        if args.build_only:
            continue
        r = subprocess.run([sys.executable, os.path.join(HERE, "measure.py"),
                            fw, "--sketch", sketch, "--out", outdir,
                            "--port", args.port, "--native-port", args.native_port,
                            "--seconds", str(args.seconds), "--label", label])
        if r.returncode != 0:
            print(f"== {label}: MEASURE FAILED")
            failed.append(label)
    print(f"== sweep done ({len(failed)} failed: {failed})" if failed
          else "== sweep done")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
