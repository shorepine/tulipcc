#!/usr/bin/env python3
"""Interactive AMYboard World capture session.

For each AMYboard World sketch: download it, push it onto a USB-connected
AMYboard (same SysEx "write to sketch" flow as the web editor), start an ffmpeg
recording of the iPhone (Continuity Camera) + a Mac audio input, then let you
audition it and mark it good / not interesting / not working. State is saved so a
later run skips what you've already done.

Run `./amyworld record --list-devices` first to see your camera/audio indices
(or `./.venv/bin/python record.py --list-devices`).
"""

import argparse
import json
import os
import sys
import termios
import time
import tty

import capture
import patterns
import worldapi
from amyboard_link import AMYboardLink
from session import Session


# ----------------------------------------------------------------------------
# single-key input
# ----------------------------------------------------------------------------
def getch():
    """Read one keypress (no Enter). Falls back to line input if not a tty."""
    if not sys.stdin.isatty():
        line = sys.stdin.readline()
        return line[:1] if line else "q"
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setcbreak(fd)
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)
    return ch


# ----------------------------------------------------------------------------
# config
# ----------------------------------------------------------------------------
DEFAULTS = {
    "api_base": worldapi.DEFAULT_BASE,
    "out_dir": "session_out",
    "capture_video": True,   # False = external-video mode (phone films continuously; we sync by beep)
    "video_device": None,    # name substring or index; None = auto-detect iPhone
    "audio_device": None,    # name substring or index; None = no audio
    "audio_channels": None,  # 1-indexed device channels to record, e.g. [13, 14]
    "midi_match": "amyboard",
    "framerate": 30,
    "video_size": None,
    "reset": "amy",          # amy | reboot | none
    "oled_reset": "logo",    # reset OLED between sketches: logo | clear | none
    "denylist_on_wedge": True,  # auto-denylist a sketch that hangs the board (watchdog)
    "amy_volume": None,      # if set, amy.send(volume=N) after each sketch starts (recording boost)
    "channel": 0,            # MIDI channel (0 == channel 1)
    "limit": 1000,           # World currently has ~296 latest-per-user sketches; this gets them all
    "tag": "",
    "q": "",
    "autoplay": False,
}


def load_config(path):
    cfg = dict(DEFAULTS)
    if path and os.path.exists(path):
        with open(path) as f:
            cfg.update(json.load(f))
    return cfg


# ----------------------------------------------------------------------------
# device setup
# ----------------------------------------------------------------------------
def print_devices():
    video, _ = capture.list_avfoundation_devices()
    print("Video devices (ffmpeg avfoundation):")
    for i, n in sorted(video.items()):
        print("  [%d] %s" % (i, n))
    print("Audio input devices (sounddevice / CoreAudio):")
    for i, n in sorted(capture.list_sd_audio_devices().items()):
        print("  [%d] %s" % (i, n))
    import mido
    print("MIDI inputs: ", mido.get_input_names())
    print("MIDI outputs:", mido.get_output_names())


def resolve_devices(cfg):
    # video via ffmpeg/avfoundation; audio via sounddevice (DIFFERENT numbering)
    vidx = None
    if cfg["capture_video"]:
        video, _ = capture.list_avfoundation_devices()
        vidx = capture.resolve_device(video, cfg["video_device"]) if cfg["video_device"] else capture.find_iphone(video)
        if vidx is None:
            raise SystemExit(
                "Could not resolve a video device. Connect your iPhone (Continuity Camera) "
                "or pass --video-device <name|index>. Run --list-devices to see options.")
    aidx = capture.resolve_sd_audio(cfg["audio_device"]) if cfg["audio_device"] else None
    if cfg["audio_device"] and aidx is None:
        raise SystemExit("Could not resolve audio device %r (see --list-devices)." % cfg["audio_device"])
    aname = capture.list_sd_audio_devices().get(aidx, "?") if aidx is not None else None
    vdesc = ("[%d] %s" % (vidx, capture.list_avfoundation_devices()[0].get(vidx, "?"))
             if vidx is not None else "(external — phone films the whole session)")
    print("Recording video %s  +  audio %s" % (
        vdesc, ("[%d] %s" % (aidx, aname)) if aidx is not None else "(none)"))
    return vidx, aidx


# ----------------------------------------------------------------------------
# one sketch
# ----------------------------------------------------------------------------
MENU = ("  [g]ood  [n]ot interesting  [x] not working  [d]enylist  |  "
        "[p]lay pattern  [s]top  [r]e-transfer  [i]nfo  [q]uit & save")


def handle_sketch(item, link, rec_factory, sess, cfg, index, total):
    title = str(item.get("filename", "")).rsplit(".", 1)[0]
    desc = item.get("description", "") or item.get("content", "")
    print("\n" + "=" * 72)
    print("[%d/%d] id=%s  %s  by %s" % (index, total, item["id"], item.get("filename"),
                                        worldapi.display_username(item.get("username"))))
    if desc:
        print("  " + desc)
    if item.get("tags"):
        print("  tags: " + ", ".join(item["tags"]))

    # download
    try:
        text = worldapi.download_sketch(item, cfg["api_base"])
    except Exception as e:
        print("  ! download failed: %s — marking not working" % e)
        sess.record(item, "not_working", errors=["download failed: %s" % e])
        return "next"

    # start recording (audio-only .m4a in external-video mode, else muxed .mov)
    clip_name = "%05d_%s.%s" % (item["id"], _safe(title), "mov" if cfg["capture_video"] else "m4a")
    clip_path = os.path.join(cfg["out_dir"], "clips", clip_name)
    recorder = None
    started = 0.0

    def start_recording():
        nonlocal recorder, started
        recorder = rec_factory()
        started = time.time()
        try:
            recorder.start(clip_path)
            print("  ● recording -> %s" % os.path.relpath(clip_path))
        except Exception as e:
            print("  ! recording failed to start: %s" % e)
            recorder = None

    def prog(i, n):
        sys.stdout.write("\r  transferring %d/%d chunks" % (i, n))
        sys.stdout.flush()

    def do_load():
        print("  pushing sketch (reset=%s%s)..." % (
            cfg["reset"], ", volume=%g" % cfg["amy_volume"] if cfg["amy_volume"] else ""))
        r = link.load_sketch(text, reset=cfg["reset"], progress=prog,
                             volume=cfg["amy_volume"], display_reset=cfg["oled_reset"])
        sys.stdout.write("\r" + " " * 40 + "\r")
        print("  transfer: %d/%d frames acked%s" % (
            r["acked"], r["sent"], "" if r["ready"] else "  (board did not re-enumerate cleanly)"))
        return r

    start_recording()
    result = do_load()

    # Watchdog with correct attribution. If the transfer barely ACKed (<=1), the
    # board was ALREADY wedged when this load began — i.e. a PREVIOUS sketch's
    # delayed hang, not this one's fault — so recover and retry this sketch once on
    # the clean board. If instead it loaded and then went unresponsive, this sketch
    # is the culprit; denylist it. (A hang that only shows up mid-audition is caught
    # by the post-mark check below.)
    retried = False
    while result.get("wedged"):
        if recorder:
            recorder.stop(discard=True)
        pre_existing = result.get("acked", 0) <= 1
        print("  ⛔ board wedged (%s) — recovering..." %
              ("already hung before this sketch" if pre_existing else "this sketch hung on load"))
        if not link.recover(factory_reset=True):
            print("  ✗ board did not recover — power-cycle USB and re-run.")
            sess.record(item, "not_working", errors=["wedged the board; recovery failed"])
            return "quit"
        print("  ✓ board recovered.")
        if pre_existing and not retried:
            print("  (a previous sketch hung the board, not this one — retrying it)")
            retried = True
            start_recording()
            result = do_load()
            continue
        if cfg.get("denylist_on_wedge", True):
            sess.deny(item, reason="wedged the board (CPU hang)")
            print("  ⛔ denylisted id=%s — it won't be loaded again." % item["id"])
        sess.record(item, "not_working", errors=["wedged the board (CPU hang; no traceback)"])
        return "next"

    # external-video mode: play a sync beep now (in the clean clip + the phone
    # video's audio) so the stitcher can locate this sketch on the phone timeline.
    beep_index = None
    if not cfg["capture_video"]:
        beep_index = sess.next_beep_index(item["id"])
        link.beep()
        print("  ♪ sync beep #%d" % beep_index)

    suggested_status = None
    if result["errors"]:
        print("  ⚠ SKETCH ERROR (suggest [x] not working):")
        for tb in result["errors"]:
            print(_indent(tb, "      "))
        suggested_status = "not_working"

    # audition loop
    player = [None]
    audition_in = [None]

    def start_pattern():
        stop_pattern()
        pat = patterns.random_pattern()
        if audition_in[0] is None and recorder:
            audition_in[0] = max(0.0, recorder.elapsed() - 0.3)
        player[0] = patterns.PatternPlayer(link, pat, channel=cfg["channel"])
        player[0].start()
        print("  ♪ %s" % pat.describe())
        return pat

    def stop_pattern():
        if player[0]:
            player[0].stop()
            player[0].join(timeout=2)
            player[0] = None
            link.all_notes_off(cfg["channel"])

    last_pattern = [None]
    if cfg["autoplay"]:
        last_pattern[0] = start_pattern()

    print(MENU)
    if suggested_status:
        print("  > suggested: x")
    while True:
        ch = getch().lower()
        if ch == "p":
            last_pattern[0] = start_pattern()
        elif ch == "s":
            stop_pattern()
            print("  ♪ stopped")
        elif ch == "i":
            print(_indent(text, "    ")[:1500])
        elif ch == "r":
            print("  re-transferring...")
            result = link.load_sketch(text, reset=cfg["reset"], progress=prog, volume=cfg["amy_volume"], display_reset=cfg["oled_reset"])
            sys.stdout.write("\r" + " " * 40 + "\r")
            if result["errors"]:
                print("  ⚠ sketch error on re-transfer")
                for tb in result["errors"]:
                    print(_indent(tb, "      "))
        elif ch in ("g", "n", "x"):
            status = {"g": "good", "n": "not_interesting", "x": "not_working"}[ch]
            stop_pattern()
            clip = recorder.stop() if recorder else None
            sess.record(item, status, clip=clip,
                        errors=result["errors"] or None,
                        pattern=(last_pattern[0].as_dict() if last_pattern[0] else None),
                        started_at=started, ended_at=time.time(),
                        audition_in=audition_in[0], beep_index=beep_index,
                        transfer={"acked": result["acked"], "sent": result["sent"]})
            print("  → %s" % status)
            # delayed-hang check: some sketches only wedge the board mid-audition
            # (e.g. once MIDI/heavy processing kicks in). Catch it HERE so it's
            # blamed on this sketch, not the next one — and so the next sketch
            # loads on a clean board. The clip is kept (it recorded fine).
            if not link.ping(timeout=2.5):
                print("  ⛔ board wedged during this sketch (delayed hang) — recovering (clip kept).")
                if link.recover(factory_reset=True):
                    print("  ✓ board recovered.")
                if cfg.get("denylist_on_wedge", True):
                    sess.deny(item, reason="wedged the board (delayed hang during audition)")
                    print("  ⛔ denylisted id=%s so it won't be loaded again." % item["id"])
            return "next"
        elif ch == "d":
            stop_pattern()
            if recorder:
                recorder.stop(discard=True)
            sess.deny(item, reason="denied during session")
            print("  ⛔ denylisted id=%s — will be skipped on every future run." % item["id"])
            return "next"
        elif ch == "q":
            stop_pattern()
            if recorder:
                recorder.stop()
            print("  stopping for now (this sketch stays pending).")
            return "quit"
        else:
            print(MENU)


def _safe(name):
    return "".join(c if c.isalnum() or c in "-_" else "_" for c in name)[:48]


def _indent(text, pad):
    return "\n".join(pad + line for line in str(text).splitlines())


# ----------------------------------------------------------------------------
# main
# ----------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--config", help="path to a JSON config file")
    ap.add_argument("--list-devices", action="store_true", help="list camera/audio/MIDI devices and exit")
    ap.add_argument("--out", dest="out_dir", help="output directory (default session_out)")
    ap.add_argument("--api-base")
    ap.add_argument("--video-device", help="iPhone video device name substring or index")
    ap.add_argument("--audio-device", help="Mac audio input device name substring or index")
    ap.add_argument("--audio-channels", help="1-indexed device channels to record, e.g. 13,14")
    ap.add_argument("--external-video", action="store_true",
                    help="don't capture video; you film the whole session on the phone and we sync by beep")
    ap.add_argument("--midi-match", help="MIDI port name substring (default amyboard)")
    ap.add_argument("--reset", choices=["amy", "reboot", "none"], help="clean-slate strategy between sketches")
    ap.add_argument("--oled-reset", choices=["logo", "clear", "none"], help="reset the OLED between sketches (default logo)")
    ap.add_argument("--amy-volume", type=float, help="amy.send(volume=N) after each sketch starts (recording level boost; e.g. 5)")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--tag", help="only this World tag (e.g. official)")
    ap.add_argument("--q", help="World search query")
    ap.add_argument("--framerate", type=int)
    ap.add_argument("--video-size", help="capture size e.g. 1280x720")
    ap.add_argument("--autoplay", action="store_true", help="auto-start a pattern after each transfer")
    ap.add_argument("--redo", action="store_true", help="don't skip already-handled sketches")
    ap.add_argument("--redo-status", help="only re-do sketches with this status (e.g. not_working)")
    ap.add_argument("--deny", help="add sketch id(s) to the denylist (comma list), e.g. 574; they're skipped forever")
    ap.add_argument("--undeny", help="remove sketch id(s) from the denylist (comma list)")
    ap.add_argument("--list-denied", action="store_true", help="show the denylist and exit")
    ap.add_argument("--recover", action="store_true",
                    help="unstick a board wedged by a runaway sketch (zB reboot + factory reset) and exit")
    ap.add_argument("--no-factory-reset", action="store_true",
                    help="with --recover, just free the board; don't overwrite sketch.py with the default")
    ap.add_argument("--no-denylist-on-wedge", action="store_true",
                    help="if a sketch hangs the board, recover but don't auto-denylist it")
    ap.add_argument("--deny-reason", default="manually denied", help="reason stored with --deny")
    args = ap.parse_args()

    if args.list_devices:
        print_devices()
        return

    cfg = load_config(args.config)
    for k in ("out_dir", "api_base", "video_device", "audio_device", "midi_match",
              "reset", "oled_reset", "limit", "tag", "q", "framerate", "video_size"):
        v = getattr(args, k, None)
        if v is not None:
            cfg[k] = v
    if args.autoplay:
        cfg["autoplay"] = True
    if args.audio_channels:
        cfg["audio_channels"] = [int(x) for x in args.audio_channels.replace(" ", "").split(",")]
    if args.amy_volume is not None:
        cfg["amy_volume"] = args.amy_volume
    if args.no_denylist_on_wedge:
        cfg["denylist_on_wedge"] = False
    if args.external_video:
        cfg["capture_video"] = False

    # --- recover a wedged board (run & exit; no camera needed) -------------
    if args.recover:
        link = AMYboardLink(cfg["midi_match"]).open()
        print("sending zB reboot (pure-C; bypasses a hung sketch loop)...")
        ok = link.recover(factory_reset=not args.no_factory_reset)
        link.close()
        if ok:
            print("✓ board recovered%s." % ("" if args.no_factory_reset
                                            else " and reset to the default sketch"))
        else:
            print("✗ board did not come back — power-cycle it (unplug/replug USB), then retry.")
        return

    sess = Session(cfg["out_dir"])

    # --- denylist management (run & exit; no board/camera needed) ----------
    if args.deny or args.undeny or args.list_denied:
        if args.undeny:
            for sid in args.undeny.replace(" ", "").split(","):
                print(("removed %s from denylist" % sid) if sess.undeny(sid) else ("%s not on denylist" % sid))
        if args.deny:
            # enrich with metadata from the API listing when we can
            by_id = {}
            try:
                by_id = {str(it["id"]): it for it in
                         worldapi.list_sketches(cfg["api_base"], limit=cfg["limit"])}
            except Exception:
                pass
            for sid in args.deny.replace(" ", "").split(","):
                item = by_id.get(sid, {"id": int(sid) if sid.isdigit() else sid})
                d = sess.deny(item, reason=args.deny_reason)
                print("⛔ denylisted id=%s %s" % (sid, d.get("filename", "")))
        if args.list_denied or args.deny or args.undeny:
            print("\ndenylist (%d) — %s:" % (len(sess.denylist), sess.denylist_path))
            for sid, d in sorted(sess.denylist.items(), key=lambda kv: int(kv[0])):
                print("  id=%-5s %-30s by %-14s — %s" % (
                    sid, d.get("filename", ""), d.get("username", ""), d.get("reason", "")))
        return

    print("session: %s" % sess.path)
    c = sess.counts()
    print("so far: good=%d  not_interesting=%d  not_working=%d  pending=%d  | denylisted=%d" % (
        c["good"], c["not_interesting"], c["not_working"], c["pending"], len(sess.denylist)))

    print("fetching sketch list...")
    items = worldapi.list_sketches(cfg["api_base"], limit=cfg["limit"], tag=cfg["tag"], q=cfg["q"])
    print("got %d sketches" % len(items))

    # filter to what we still need to do (denylisted are always skipped)
    todo = []
    for it in items:
        if sess.is_denied(it["id"]):
            continue
        if args.redo_status:
            if sess.status(it["id"]) == args.redo_status:
                todo.append(it)
        elif args.redo or not sess.is_done(it["id"]):
            todo.append(it)
    if not todo:
        print("nothing to do — all fetched sketches are already handled or denylisted (use --redo to repeat).")
        return
    print("%d sketches to record this run (skipping %d denylisted).\n" % (len(todo), len(sess.denylist)))

    vidx, aidx = resolve_devices(cfg)

    if cfg["audio_channels"]:
        print("audio channels: %s (extracted from the device)" % cfg["audio_channels"])

    def rec_factory():
        return capture.ClipRecorder(vidx, aidx, framerate=cfg["framerate"],
                                    video_size=cfg["video_size"],
                                    audio_channels=cfg["audio_channels"],
                                    log_dir=os.path.join(cfg["out_dir"], "logs"))

    if not cfg["capture_video"]:
        print("\n" + "!" * 72)
        print("EXTERNAL-VIDEO MODE: start your phone filming NOW and let it run for the")
        print("WHOLE session (don't stop/restart it). A sync beep plays at each sketch so")
        print("the stitcher can line the video up. Keep the AMYboard audible to the phone.")
        print("!" * 72)
        try:
            input("Press Enter once the phone is recording... ")
        except EOFError:
            pass

    with AMYboardLink(cfg["midi_match"]) as link:
        for i, item in enumerate(todo, 1):
            action = handle_sketch(item, link, rec_factory, sess, cfg, i, len(todo))
            if action == "quit":
                break

    c = sess.counts()
    print("\ndone. good=%d  not_interesting=%d  not_working=%d  pending=%d" % (
        c["good"], c["not_interesting"], c["not_working"], c["pending"]))
    if cfg["capture_video"]:
        print("next: ./amyworld stitch plan --session %s" % cfg["out_dir"])
    else:
        print("next: ./amyworld stitch sync --session %s --phone-video <your_video.mov> --out montage.mp4"
              % cfg["out_dir"])


if __name__ == "__main__":
    main()
