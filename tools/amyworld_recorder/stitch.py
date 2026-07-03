#!/usr/bin/env python3
"""Build the final montage from a recording session, with ffmpeg.

Run via the wrapper (uses the bundled venv): `./amyworld stitch <cmd> …`
(or directly: `./.venv/bin/python stitch.py <cmd> …`).

Workflow:
  1. ./amyworld stitch plan   --session session_out          # writes clips.json
  2. (edit clips.json: reorder, set enabled:false, set in/out trim seconds)
  3. ./amyworld stitch render --session session_out --out montage.mp4

Other commands:
  ./amyworld stitch list   --session session_out [--status good]
  ./amyworld stitch broken --session session_out             # show errors for failed sketches
"""

import argparse
import json
import os
import subprocess
import sys
import tempfile
import textwrap

import _bootstrap  # noqa: F401  (makes the in-repo amyboardctl importable)
from amyboardctl.world import display_username
from session import Session

FONT_CANDIDATES = [
    "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "/System/Library/Fonts/Helvetica.ttc",
    "/System/Library/Fonts/SFNS.ttf",
]


def find_font():
    for f in FONT_CANDIDATES:
        if os.path.exists(f):
            return f
    return None


def ffprobe_has_audio(path):
    try:
        out = subprocess.run(
            ["ffprobe", "-v", "error", "-select_streams", "a",
             "-show_entries", "stream=index", "-of", "csv=p=0", path],
            capture_output=True, text=True).stdout.strip()
        return bool(out)
    except Exception:
        return False


def ffprobe_duration(path):
    try:
        out = subprocess.run(
            ["ffprobe", "-v", "error", "-show_entries", "format=duration",
             "-of", "csv=p=0", path], capture_output=True, text=True).stdout.strip()
        return float(out)
    except Exception:
        return 0.0


# ---------------------------------------------------------------------------
# plan
# ---------------------------------------------------------------------------
def cmd_plan(args):
    sess = Session(args.session)
    good = sess.by_status("good")
    good.sort(key=lambda r: r.get("updated_at", 0))
    clips = []
    for r in good:
        clip = r.get("clip")
        if not clip or not os.path.exists(clip):
            print("  ! skipping id=%s (clip missing: %s)" % (r["id"], clip))
            continue
        clips.append({
            "id": r["id"],
            "title": r.get("title", ""),
            "username": display_username(r.get("username", "")),
            "description": r.get("description", ""),
            "clip": clip,
            "enabled": True,
            "in": round(r.get("audition_in") or 0.0, 2),
            "out": None,  # null = to end
        })
    out = args.list or os.path.join(args.session, "clips.json")
    with open(out, "w") as f:
        json.dump({"clips": clips}, f, indent=2)
    print("wrote %d good clips to %s" % (len(clips), out))
    print("edit it (reorder / enabled:false / in/out trim), then: ./amyworld stitch render --session %s" % args.session)


# ---------------------------------------------------------------------------
# render
# ---------------------------------------------------------------------------
def cmd_render(args):
    listpath = args.list or os.path.join(args.session, "clips.json")
    if not os.path.exists(listpath):
        raise SystemExit("no clip list at %s — run `stitch.py plan` first." % listpath)
    with open(listpath) as f:
        clips = [c for c in json.load(f)["clips"] if c.get("enabled", True)]
    if not clips:
        raise SystemExit("no enabled clips in %s" % listpath)

    font = args.font or find_font()
    if not font:
        print("warning: no font found; text overlays disabled.", file=sys.stderr)

    W, H = (1080, 1920) if args.portrait else (args.width, args.height)
    workdir = tempfile.mkdtemp(prefix="amyworld_stitch_")
    segments = []
    print("rendering %d clips at %dx%d -> %s" % (len(clips), W, H, args.out))
    for i, c in enumerate(clips):
        # Segments carry LOSSLESS PCM audio. We must not encode AAC per segment:
        # AAC's ~2100-sample encoder priming/padding would leave a click + timing
        # wobble at every clip join once concatenated. Instead we copy the gapless
        # PCM through the concat demuxer and encode AAC exactly once, below.
        seg = os.path.join(workdir, "seg_%03d.mov" % i)
        _render_segment(c, seg, W, H, font, args)
        segments.append(seg)
        print("  [%d/%d] %s" % (i + 1, len(clips), c.get("title")))

    _concat_segments(segments, workdir, args.out)
    print("done -> %s" % args.out)


def _concat_segments(segments, workdir, out):
    """Concat PCM-audio segments, encoding AAC exactly once (no per-segment AAC
    boundaries), video stream-copied."""
    listfile = os.path.join(workdir, "concat.txt")
    with open(listfile, "w") as f:
        for s in segments:
            f.write("file '%s'\n" % s)
    subprocess.run(["ffmpeg", "-hide_banner", "-y", "-f", "concat", "-safe", "0",
                    "-i", listfile, "-c:v", "copy",
                    "-c:a", "aac", "-b:a", "256k", "-ar", "48000", "-ac", "2",
                    "-movflags", "+faststart", out], check=True)


# ---------------------------------------------------------------------------
# sync: build the montage from an externally-recorded phone video, locating each
# sketch by its sync beep and using the clean Model 16 audio the Mac recorded.
# ---------------------------------------------------------------------------
def cmd_sync(args):
    import beepsync
    sess = Session(args.session)
    beeps = sess.data.get("beeps", [])
    if not beeps:
        raise SystemExit("no sync beeps recorded — record the session with --external-video first.")

    # One phone video per run; the beep counter climbs across runs. Detect beeps
    # in each video (given in RUN ORDER) and lay them end to end — that flat list
    # lines up 1:1 with the recorded beeps, so each sketch is found in the right
    # video at the right time.
    flat = []  # (video_path, local_time) in recorded order
    for v in args.phone_video:
        pa, sr = beepsync.load_audio_mono(v, sr=8000)
        if pa.size == 0:
            raise SystemExit("no decodable audio in %s — can't find the beeps." % v)
        bts = beepsync.detect_beeps(pa, sr)
        print("  %-40s %d beeps" % (os.path.basename(v), len(bts)))
        flat.extend((v, t) for t in bts)
    print("total beeps across %d video(s): %d;  recorded: %d" % (len(args.phone_video), len(flat), len(beeps)))
    if len(flat) != len(beeps):
        print("  ! count mismatch: alignment proceeds by index, so a missed/extra beep shifts")
        print("    everything after it. Give the videos in RUN ORDER; check the result.")
    n = min(len(flat), len(beeps))

    # good sketches, in beep order
    good = []
    for i in range(n):
        rec = sess.get(beeps[i])
        if rec and rec.get("status") == "good" and rec.get("clip") and os.path.exists(rec["clip"]):
            good.append((i, rec))
    if not good:
        raise SystemExit("no good clips with audio to stitch (checked %d beeps)." % n)

    font = args.font or find_font()
    W, H = (1080, 1920) if args.portrait else (args.width, args.height)
    workdir = tempfile.mkdtemp(prefix="amyworld_sync_")
    segments = []
    print("rendering %d good clips synced to the phone video ..." % len(good))
    for k, (bi, rec) in enumerate(good):
        clip = rec["clip"]
        # locate the beep inside the clean clip so we can align it to the phone beep
        ca, csr = beepsync.load_audio_mono(clip, sr=8000)
        cb = beepsync.detect_beeps(ca, csr)
        clip_beep = cb[0] if cb else 0.0
        lead = 0.4                       # start just past the ~0.28s double-beep
        a_ss = clip_beep + lead
        clip_dur = ffprobe_duration(clip)
        seg_dur = max(0.2, clip_dur - a_ss - max(0.0, args.tail))
        video, beep_t = flat[bi]             # which phone video, and where in it
        phone_ss = beep_t + lead             # beep-to-beep alignment
        seg = os.path.join(workdir, "seg_%03d.mov" % k)
        _render_synced_segment(rec, video, phone_ss, seg_dur, clip, a_ss, seg, W, H, font, args)
        segments.append(seg)
        print("  [%d/%d] %-28s %s@%.1fs  %.1fs%s" % (
            k + 1, len(good), rec.get("title", "")[:28], os.path.basename(video), phone_ss, seg_dur,
            "" if cb else "  (no beep found in clip — used start)"))

    _concat_segments(segments, workdir, args.out)
    print("done -> %s" % args.out)


def _render_synced_segment(rec, phone_video, phone_ss, dur, audio_path, audio_ss, out, W, H, font, args):
    clip = {"title": rec.get("title", ""), "username": display_username(rec.get("username", "")),
            "description": rec.get("description", "")}
    vf = ("scale=%d:%d:force_original_aspect_ratio=decrease,"
          "pad=%d:%d:(ow-iw)/2:(oh-ih)/2,setsar=1,fps=%d" % (W, H, W, H, args.fps))
    for filt in _overlay_filters(clip, out, W, H, font, args, 0.0):
        vf += "," + filt
    cmd = ["ffmpeg", "-hide_banner", "-y",
           "-ss", "%.3f" % phone_ss, "-i", phone_video,      # video from phone
           "-ss", "%.3f" % audio_ss, "-i", audio_path,       # clean Model 16 audio
           "-t", "%.3f" % dur,
           "-map", "0:v:0", "-map", "1:a:0", "-vf", vf,
           "-c:v", "libx264", "-preset", "medium", "-crf", "18", "-pix_fmt", "yuv420p",
           "-c:a", "pcm_s16le", "-ar", "48000", "-ac", "2", "-r", str(args.fps), out]
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def _overlay_filters(clip, out, W, H, font, args, ss):
    """A lower-third: one translucent panel (drawbox) with the title + 'by user'
    + wrapped description stacked as one drawtext per line. Each line is its own
    drawtext (no embedded newlines) because ffmpeg 8's harfbuzz shaping renders
    an embedded \\n as a tofu glyph. Text comes from files to avoid escaping."""
    if not font:
        return []
    pad, gap, margin_x = 26, 12, 64

    lines = []  # (text, size)
    if clip.get("title"):
        lines.append((clip["title"], args.title_size))
    if clip.get("username"):
        lines.append(("by %s" % clip["username"], args.meta_size))

    # Wrap the description to the real frame width (drawtext doesn't wrap), then
    # show as many lines as fit in a panel up to overlay_max_frac of the height;
    # ellipsize only if it's longer than that.
    usable_w = W - 2 * margin_x
    wrap_w = max(20, int(usable_w / (args.meta_size * 0.60)))  # ~chars per line
    desc_lines = textwrap.wrap(clip.get("description", "") or "", width=wrap_w)
    desc_h = int(args.meta_size * 1.32) + gap
    used_h = pad * 2 + sum(int(sz * 1.32) + gap for _, sz in lines)
    budget = args.overlay_max_frac * H
    max_desc = max(1, int((budget - used_h) / desc_h))
    if len(desc_lines) > max_desc:
        desc_lines = desc_lines[:max_desc]
        desc_lines[-1] = desc_lines[-1].rstrip()[:wrap_w - 1] + "…"  # ellipsis
    for dl in desc_lines:
        lines.append((dl, args.meta_size))
    if not lines:
        return []

    heights = [int(sz * 1.32) for _, sz in lines]
    panel_h = sum(heights) + gap * (len(lines) - 1) + pad * 2
    y0 = H - args.bottom_margin - panel_h
    enable = (":enable='lt(t-%g\\,%g)'" % (ss, args.title_seconds)) if args.title_seconds else ""

    filters = ["drawbox=x=0:y=%d:w=%d:h=%d:color=black@0.5:t=fill%s" % (y0, W, panel_h, enable)]
    y = y0 + pad
    for i, (txt, sz) in enumerate(lines):
        tf = os.path.join(os.path.dirname(out),
                          "%s.l%d.txt" % (os.path.basename(out), i))
        with open(tf, "w") as f:
            f.write(txt)
        filters.append(
            "drawtext=fontfile=%s:textfile=%s:fontsize=%d:fontcolor=white:"
            "x=%d:y=%d:shadowcolor=black@0.8:shadowx=2:shadowy=2%s"
            % (font, tf, sz, margin_x, y, enable))
        y += heights[i] + gap
    return filters


def _render_segment(clip, out, W, H, font, args):
    path = clip["clip"]
    ss = float(clip.get("in") or 0.0)
    to = clip.get("out")
    seek = ["-ss", str(ss)]
    trim = ["-to", str(float(to))] if to else []
    has_audio = ffprobe_has_audio(path)

    vf = ("scale=%d:%d:force_original_aspect_ratio=decrease,"
          "pad=%d:%d:(ow-iw)/2:(oh-ih)/2,setsar=1,fps=%d"
          % (W, H, W, H, args.fps))
    for filt in _overlay_filters(clip, out, W, H, font, args, ss):
        vf += "," + filt

    cmd = ["ffmpeg", "-hide_banner", "-y"] + seek + ["-i", path] + trim
    if not has_audio:
        cmd += ["-f", "lavfi", "-i", "anullsrc=channel_layout=stereo:sample_rate=48000"]
    cmd += ["-vf", vf, "-map", "0:v:0", "-map", ("1:a:0" if not has_audio else "0:a:0?")]
    # PCM audio so the join is gapless; AAC is encoded once at concat time.
    cmd += ["-c:v", "libx264", "-preset", "medium", "-crf", "18", "-pix_fmt", "yuv420p",
            "-c:a", "pcm_s16le", "-ar", "48000", "-ac", "2",
            "-shortest", "-r", str(args.fps), out]
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


# ---------------------------------------------------------------------------
# list / broken
# ---------------------------------------------------------------------------
def cmd_list(args):
    sess = Session(args.session)
    recs = sess.by_status(args.status) if args.status else list(sess.data["sketches"].values())
    recs.sort(key=lambda r: r.get("updated_at", 0))
    for r in recs:
        print("%-14s id=%-5s %-32s by %-14s %s" % (
            r.get("status"), r["id"], r.get("filename", "")[:32],
            display_username(r.get("username", "")), os.path.basename(r.get("clip") or "")))
    print("\n%d shown" % len(recs))


def cmd_broken(args):
    sess = Session(args.session)
    recs = sess.by_status("not_working")
    if not recs:
        print("no sketches marked not_working.")
        return
    for r in recs:
        print("=" * 72)
        print("id=%s  %s  by %s" % (r["id"], r.get("filename"), display_username(r.get("username"))))
        for tb in (r.get("errors") or ["(no captured python error — likely silent / no sound)"]):
            print("\n".join("    " + ln for ln in str(tb).splitlines()))
    print("\n%d broken" % len(recs))


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("plan", help="write clips.json from good sketches")
    p.add_argument("--session", required=True)
    p.add_argument("--list", help="output path (default <session>/clips.json)")
    p.set_defaults(func=cmd_plan)

    p = sub.add_parser("render", help="render the montage from clips.json")
    p.add_argument("--session", required=True)
    p.add_argument("--list", help="clip list (default <session>/clips.json)")
    p.add_argument("--out", default="montage.mp4")
    p.add_argument("--font")
    p.add_argument("--width", type=int, default=1920)
    p.add_argument("--height", type=int, default=1080)
    p.add_argument("--portrait", action="store_true", help="1080x1920 vertical")
    p.add_argument("--fps", type=int, default=30)
    p.add_argument("--title-size", type=int, default=64)
    p.add_argument("--meta-size", type=int, default=38)
    p.add_argument("--bottom-margin", type=int, default=80)
    p.add_argument("--overlay-max-frac", type=float, default=0.6,
                   help="max fraction of frame height the title/description panel may use (default 0.6)")
    p.add_argument("--title-seconds", type=float, default=0.0,
                   help="show overlays only for the first N seconds (0 = whole clip)")
    p.set_defaults(func=cmd_render)

    p = sub.add_parser("sync", help="build the montage from an external phone video, synced by beep")
    p.add_argument("--session", required=True)
    p.add_argument("--phone-video", required=True, nargs="+",
                   help="the phone video(s), one per run, IN RUN ORDER (e.g. run1.mov run2.mov)")
    p.add_argument("--out", default="montage.mp4")
    p.add_argument("--tail", type=float, default=0.0, help="trim N seconds off each clip's end")
    p.add_argument("--font")
    p.add_argument("--width", type=int, default=1920)
    p.add_argument("--height", type=int, default=1080)
    p.add_argument("--portrait", action="store_true", help="1080x1920 vertical")
    p.add_argument("--fps", type=int, default=30)
    p.add_argument("--title-size", type=int, default=64)
    p.add_argument("--meta-size", type=int, default=38)
    p.add_argument("--bottom-margin", type=int, default=80)
    p.add_argument("--overlay-max-frac", type=float, default=0.6,
                   help="max fraction of frame height the title/description panel may use (default 0.6)")
    p.add_argument("--title-seconds", type=float, default=0.0,
                   help="show overlays only for the first N seconds (0 = whole clip)")
    p.set_defaults(func=cmd_sync)

    p = sub.add_parser("list", help="list recorded sketches")
    p.add_argument("--session", required=True)
    p.add_argument("--status", help="good | not_interesting | not_working | pending")
    p.set_defaults(func=cmd_list)

    p = sub.add_parser("broken", help="show not_working sketches and their python errors")
    p.add_argument("--session", required=True)
    p.set_defaults(func=cmd_broken)

    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
