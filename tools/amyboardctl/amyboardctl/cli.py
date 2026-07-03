"""amyboardctl — command-line control of an AMYboard over USB-MIDI SysEx.

Examples:
    amyboardctl ports
    amyboardctl ping
    amyboardctl version
    amyboardctl upload_sketch sketch.py
    amyboardctl download_sketch -o sketch.py
    amyboardctl download_file /user/current/other.py -o other.py
    amyboardctl download_state
    amyboardctl save_state
    amyboardctl run "import amy; amy.send(volume=0.5)"
    amyboardctl reset
    amyboardctl reboot
    amyboardctl seq start
    amyboardctl note 60 --duration 0.5
    amyboardctl world list --tag drums
    amyboardctl world get shorepine/woodpiano -o woodpiano.py

See docs/amyboard/control_api.md for the underlying SysEx protocol.
"""

import argparse
import sys
import time

from . import __version__
from .link import AMYboardLink, SKETCH_PATH
from .transport import DEFAULT_PORT_MATCH, list_ports
from . import world


def _open_link(args):
    return AMYboardLink(port_match=args.port, backend=args.backend,
                        verbose=not args.quiet).open()


def _write_out(data, out):
    """Write bytes/str to a file, or stdout if out is None or '-'."""
    if isinstance(data, str):
        data = data.encode("utf-8")
    if out and out != "-":
        with open(out, "wb") as f:
            f.write(data)
        print("wrote %d bytes -> %s" % (len(data), out), file=sys.stderr)
    else:
        sys.stdout.buffer.write(data)
        if data and not data.endswith(b"\n"):
            sys.stdout.buffer.write(b"\n")


def _report_errors(link, wait=1.0):
    """Give the board a beat to push any 'X' (sketch error) frames, then print
    them. Returns True if any arrived."""
    time.sleep(wait)
    errs = link.drain_errors()
    for e in errs:
        print("---- board error " + "-" * 44, file=sys.stderr)
        print(e, file=sys.stderr)
    return bool(errs)


# ── commands ────────────────────────────────────────────────────────────────
def cmd_ports(args):
    p = list_ports(args.backend)
    if p["inputs"] == p["outputs"]:
        for n in p["inputs"]:
            print(n)
    else:
        for n in p["inputs"]:
            print("in:  %s" % n)
        for n in p["outputs"]:
            print("out: %s" % n)
    return 0


def cmd_ping(args):
    with _open_link(args) as link:
        if link.ping_ok(timeout=args.timeout):
            print("OK")
            return 0
        print("no reply", file=sys.stderr)
        return 1


def cmd_version(args):
    with _open_link(args) as link:
        v = link.report_version(timeout=args.timeout)
        if v:
            print(v)
            return 0
        print("no version reply", file=sys.stderr)
        return 1


def cmd_upload_sketch(args):
    with open(args.file, "r", encoding="utf-8") as f:
        text = f.read()
    with _open_link(args) as link:
        if args.no_start or args.path != SKETCH_PATH:
            # plain file write — no sketch restart (a non-sketch path can't be
            # "started" anyway; use `run` to act on it)
            if args.reset == "amy":
                link.reset_amy()
            sent, acked = link.transfer_file(text, path=args.path)
            print("transferred %s: %d/%d frames acked" % (args.path, acked, sent),
                  file=sys.stderr)
            return 0 if acked == sent else 1
        r = link.load_sketch(text, reset=args.reset, volume=args.volume,
                             display_reset=args.display_reset)
        print("transferred %s: %d/%d frames acked" % (args.path, r["acked"], r["sent"]),
              file=sys.stderr)
        for e in r["errors"]:
            print("---- board error " + "-" * 44, file=sys.stderr)
            print(e, file=sys.stderr)
        if r["wedged"]:
            print("board is unresponsive after starting the sketch (runaway "
                  "loop()?) — try `amyboardctl reboot` then `factory_reset`",
                  file=sys.stderr)
        ok = (r["acked"] == r["sent"]) and not r["errors"] and not r["wedged"]
        return 0 if ok else 1


def cmd_download_sketch(args):
    with _open_link(args) as link:
        data = link.read_file(args.path, timeout=args.timeout)
    _write_out(data, args.out)
    return 0


def cmd_download_file(args):
    with _open_link(args) as link:
        data = link.read_file(args.boardpath, timeout=args.timeout)
    _write_out(data, args.out)
    return 0


def cmd_download_state(args):
    with _open_link(args) as link:
        text = link.dump_state(timeout=args.timeout)
    _write_out(text, args.out)
    return 0


def cmd_save_state(args):
    with _open_link(args) as link:
        ok = link.save_state_to_sketch(args.path)
    return 0 if ok else 1


def cmd_run(args):
    with _open_link(args) as link:
        ok = link.send_python(args.code)
        had_errors = _report_errors(link, wait=args.error_wait)
    return 0 if ok and not had_errors else 1


def cmd_reset(args):
    with _open_link(args) as link:
        return 0 if link.reset_amy() else 1


def cmd_restart(args):
    with _open_link(args) as link:
        ok = link.restart_sketch()
        had_errors = _report_errors(link)
    return 0 if ok and not had_errors else 1


def cmd_factory_reset(args):
    with _open_link(args) as link:
        ok = link.factory_reset()
        had_errors = _report_errors(link)
    return 0 if ok and not had_errors else 1


def cmd_reboot(args):
    with _open_link(args) as link:
        link.reboot(args.mode)
        if args.mode == "rom" or args.no_wait:
            print("reboot sent", file=sys.stderr)
            return 0
        print("reboot sent; waiting for the board to re-enumerate ...", file=sys.stderr)
        if link.wait_for_ready(timeout=args.timeout):
            print("board is back", file=sys.stderr)
            return 0
        print("board did not re-enumerate within %.0fs" % args.timeout, file=sys.stderr)
        return 1


def cmd_seq(args):
    with _open_link(args) as link:
        return 0 if link.sequencer(args.action == "start") else 1


def cmd_note(args):
    with _open_link(args) as link:
        link.note(args.note, args.velocity, args.duration, args.channel - 1)
    return 0


def cmd_world_list(args):
    items = world.list_sketches(base=args.base, limit=args.limit, tag=args.tag,
                                q=args.q, username=args.username)
    for it in items:
        print("%-24s %-16s %s" % (it.get("filename", ""),
                                  world.display_username(it.get("username", "")),
                                  it.get("description", "") or ""))
    return 0


def cmd_world_get(args):
    author, _, name = args.sketch.partition("/")
    if not name:
        print("expected AUTHOR/NAME (e.g. shorepine/woodpiano)", file=sys.stderr)
        return 2
    if name.endswith(".py"):
        name = name[:-3]
    text = world.fetch_sketch(name, author, base=args.base)
    _write_out(text, args.out)
    return 0


# ── argparse wiring ─────────────────────────────────────────────────────────
def build_parser():
    ap = argparse.ArgumentParser(
        prog="amyboardctl",
        description=__doc__.split("\n\nExamples")[0],
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="Run `amyboardctl <command> -h` for per-command options.")
    ap.add_argument("--version", action="version", version="amyboardctl " + __version__)
    ap.add_argument("-p", "--port", default=DEFAULT_PORT_MATCH,
                    help="MIDI port name substring, or ALSA hw:X,Y (default: %(default)s)")
    ap.add_argument("--backend", choices=["auto", "mido", "alsa"], default="auto",
                    help="MIDI backend (default: mido if installed, else ALSA amidi)")
    ap.add_argument("-q", "--quiet", action="store_true", help="less chatter on stderr")
    sub = ap.add_subparsers(dest="command", required=True)

    sub.add_parser("ports", help="list MIDI ports").set_defaults(fn=cmd_ports)

    p = sub.add_parser("ping", help="zI ping — is the board alive?")
    p.add_argument("--timeout", type=float, default=2.5)
    p.set_defaults(fn=cmd_ping)

    p = sub.add_parser("version", help="report the board's firmware version")
    p.add_argument("--timeout", type=float, default=5.0)
    p.set_defaults(fn=cmd_version)

    p = sub.add_parser("upload_sketch", help="write a sketch to the board and (re)start it")
    p.add_argument("file", help="local .py file to upload")
    p.add_argument("--path", default=SKETCH_PATH,
                   help="destination on the board (default: %(default)s; a "
                        "non-default path is written without restarting)")
    p.add_argument("--reset", choices=["amy", "reboot", "none"], default="amy",
                   help="clean-slate strategy before the transfer (default: %(default)s)")
    p.add_argument("--volume", type=float, default=None,
                   help="set amy volume after the sketch starts")
    p.add_argument("--display-reset", choices=["logo", "clear", "none"], default="logo",
                   help="reset the OLED before the restart (default: %(default)s)")
    p.add_argument("--no-start", action="store_true",
                   help="transfer only; don't restart the sketch")
    p.set_defaults(fn=cmd_upload_sketch)

    p = sub.add_parser("download_sketch", help="read the current sketch off the board")
    p.add_argument("-o", "--out", default=None, help="output file (default: stdout)")
    p.add_argument("--path", default=SKETCH_PATH)
    p.add_argument("--timeout", type=float, default=30.0)
    p.set_defaults(fn=cmd_download_sketch)

    p = sub.add_parser("download_file", help="read any file off the board's filesystem")
    p.add_argument("boardpath", help="path on the board, e.g. /user/current/sketch.py")
    p.add_argument("-o", "--out", default=None, help="output file (default: stdout)")
    p.add_argument("--timeout", type=float, default=30.0)
    p.set_defaults(fn=cmd_download_file)

    p = sub.add_parser("download_state",
                       help="dump live synth state as AMY wire commands (zD)")
    p.add_argument("-o", "--out", default=None, help="output file (default: stdout)")
    p.add_argument("--timeout", type=float, default=30.0)
    p.set_defaults(fn=cmd_download_state)

    p = sub.add_parser("save_state",
                       help="write current AMY state into the sketch on the board (zA)")
    p.add_argument("--path", default=None,
                   help="sketch path on the board (default: /user/current/sketch.py)")
    p.set_defaults(fn=cmd_save_state)

    p = sub.add_parser("run", help="run one line of Python on the board (zP, <=255 bytes)")
    p.add_argument("code")
    p.add_argument("--error-wait", type=float, default=1.0,
                   help="seconds to wait for error frames after running")
    p.set_defaults(fn=cmd_run)

    sub.add_parser("reset", help="reset the synth (amy.reset(); no reboot)"
                   ).set_defaults(fn=cmd_reset)
    sub.add_parser("restart", help="reload & restart sketch.py"
                   ).set_defaults(fn=cmd_restart)
    sub.add_parser("factory_reset", help="clear /user/current and reload the default sketch"
                   ).set_defaults(fn=cmd_factory_reset)

    p = sub.add_parser("reboot", help="reboot the board (zB)")
    p.add_argument("--mode", choices=["normal", "bootloader", "rom"], default="normal",
                   help="normal = run sketch.py on boot; bootloader = skip it; "
                        "rom = ESP download/flash mode (default: %(default)s)")
    p.add_argument("--no-wait", action="store_true",
                   help="don't wait for the board to re-enumerate")
    p.add_argument("--timeout", type=float, default=20.0,
                   help="seconds to wait for re-enumeration")
    p.set_defaults(fn=cmd_reboot)

    p = sub.add_parser("seq", help="start/stop the step sequencer (zY)")
    p.add_argument("action", choices=["start", "stop"])
    p.set_defaults(fn=cmd_seq)

    p = sub.add_parser("note", help="play one MIDI note on the loaded synth")
    p.add_argument("note", type=int, help="MIDI note number (60 = middle C)")
    p.add_argument("--velocity", type=int, default=100)
    p.add_argument("--duration", type=float, default=0.5)
    p.add_argument("--channel", type=int, default=1, help="MIDI channel 1-16 (default: 1)")
    p.set_defaults(fn=cmd_note)

    p = sub.add_parser("world", help="AMYboard World sketch API (no board needed)")
    wsub = p.add_subparsers(dest="world_command", required=True)
    w = wsub.add_parser("list", help="list World sketches")
    w.add_argument("--tag", default="")
    w.add_argument("--q", default="", help="search text")
    w.add_argument("--username", default="")
    w.add_argument("--limit", type=int, default=50)
    w.add_argument("--base", default=world.DEFAULT_BASE)
    w.set_defaults(fn=cmd_world_list)
    w = wsub.add_parser("get", help="download a World sketch by AUTHOR/NAME")
    w.add_argument("sketch", help="e.g. shorepine/woodpiano")
    w.add_argument("-o", "--out", default=None, help="output file (default: stdout)")
    w.add_argument("--base", default=world.DEFAULT_BASE)
    w.set_defaults(fn=cmd_world_get)

    return ap


def main(argv=None):
    args = build_parser().parse_args(argv)
    try:
        return args.fn(args)
    except (RuntimeError, TimeoutError, ValueError, OSError) as e:
        print("amyboardctl: %s" % e, file=sys.stderr)
        return 1
    except KeyboardInterrupt:
        return 130


if __name__ == "__main__":
    sys.exit(main())
