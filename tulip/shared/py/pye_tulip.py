# pye_tulip.py
# Run the pye editor on Tulip's text framebuffer (TFB), styled and keyed like
# the old built-in C editor (tulip/shared/editor.c).
#
# The TFB's ANSI parser (tulip/shared/display.c) covers the sequences pye
# emits except hardware scrolling / scroll regions / mouse reporting, so we
# blank those TERMCMD entries and pye falls back to repainting lines instead.
#
# We don't use pye's stock IO_DEVICE: sys.stdin/sys.stdout don't exist on
# every Tulip port (Tulip Desktop builds with MICROPY_PY_SYS_STDFILES=0, and
# the unix-port stdfiles would bypass the TFB anyway). print() and
# tulip.key_wait() both go through the mp_hal console, which is teed to the
# TFB and the serial console. Screen size comes straight from the TFB rather
# than the ESC[6n cursor position report, which the TFB cannot answer.
#
# Note: ctrl-Q is Tulip's global "quit app" hotkey and never reaches the
# editor; quit pye by hitting ESC twice.
import tulip
import pye as _pye

# Colors from the old C editor (EDITOR_COLOR_* in tulip/shared/editor.c),
# as Tulip RRRGGGBB pal indices.
_FG = 255
_BG = 36
_COMMENT = 229
_STRING = 249
_NUMBER = 175
_OPERATOR = 240
_SELECTION_BG = 72


def _rgb(p):
    # unpack a RRRGGGBB pal index to full r,g,b (mirrors unpack_rgb_332_repeat,
    # so the TFB's 24-bit SGR handler maps it back to exactly pal index p)
    r = (p & 0xE0) | ((p & 0xE0) >> 3) | ((p & 0xC0) >> 6)
    g = ((p & 0x1C) << 3) | (p & 0x1C) | ((p & 0x18) >> 3)
    b = (p & 0x03) | ((p & 0x03) << 2) | ((p & 0x03) << 4) | ((p & 0x03) << 6)
    return r, g, b


def _fg_esc(p):
    return "\x1b[38;2;%d;%d;%dm" % _rgb(p)


def _bg_esc(p):
    return "\x1b[48;2;%d;%d;%dm" % _rgb(p)


# Same rules as editor_highlight_at_row in editor.c: strings, comments,
# numbers and operators, scanned per visible line.
_OPERATORS = ":;-/=+()[]{}'\".,\\|!@#$%^&*<>?"
_COLOR_ESC = {c: _fg_esc(c) for c in (_FG, _COMMENT, _STRING, _NUMBER, _OPERATOR)}


def _colorize(line):
    out = []
    cur = None
    quote = None
    comment = False
    for ch in line:
        if comment:
            col = _COMMENT
        elif quote:
            col = _STRING
            if ch == quote:
                quote = None
        elif ch == '"' or ch == "'":
            quote = ch
            col = _STRING
        elif ch == "#":
            comment = True
            col = _COMMENT
        elif "0" <= ch <= "9":
            col = _NUMBER
        elif ch in _OPERATORS:
            col = _OPERATOR
        else:
            col = _FG
        if col != cur:
            out.append(_COLOR_ESC[col])
            cur = col
        out.append(ch)
    if cur != _FG:
        out.append(_COLOR_ESC[_FG])
    return "".join(out)


# pye's display_window with one change: unmarked lines are written through
# _colorize() for editor.c-style syntax highlighting. Marked (selected) lines
# keep the plain selection style, and scrbuf still caches the raw text.
def _display_window(self):
    Editor = _pye.Editor
    self.cur_line = min(self.total_lines - 1, max(self.cur_line, 0))
    self.vcol = max(0, min(self.col, len(self.content[self.cur_line])))
    if self.vcol >= Editor.width + self.margin:
        self.margin = self.vcol - Editor.width + (Editor.width >> 2)
    elif self.vcol < self.margin:
        self.margin = max(self.vcol - (Editor.width >> 2), 0)
    if not (self.top_line <= self.cur_line < self.top_line + Editor.height):
        self.top_line = max(self.cur_line - self.row, 0)
    self.row = self.cur_line - self.top_line
    self.cursor(False)
    line = self.top_line
    if self.mark is None:
        flag = 0
    else:
        start_line, start_col, end_line, end_col = self.mark_range()
        start_col = max(start_col - self.margin, 0)
        end_col = max(end_col - self.margin, 0)
    for c in range(Editor.height):
        if line == self.total_lines:
            if Editor.scrbuf[c] != (False, ""):
                self.goto(c, 0)
                self.clear_to_eol()
                Editor.scrbuf[c] = (False, "")
        else:
            if self.mark is not None:
                flag = (
                    (start_line <= line < end_line)
                    + ((start_line == line) << 1)
                    + (((end_line - 1) == line) << 2)
                )
            l = (flag, self.content[line][self.margin : self.margin + Editor.width])
            if (flag and line == self.cur_line) or l != Editor.scrbuf[c]:
                self.goto(c, 0)
                if flag == 0:
                    self.wr(_colorize(l[1]))
                elif flag == 7:
                    self.wr(l[1][:start_col])
                    self.hilite(2)
                    self.wr(l[1][start_col:end_col])
                    self.hilite(0)
                    self.wr(l[1][end_col:])
                elif flag == 3:
                    self.wr(l[1][:start_col])
                    self.hilite(2)
                    self.wr(l[1][start_col:])
                    self.wr(" ")
                    self.hilite(0)
                elif flag == 5:
                    self.hilite(2)
                    self.wr(l[1][:end_col])
                    self.hilite(0)
                    self.wr(l[1][end_col:])
                else:
                    self.hilite(2)
                    self.wr(l[1])
                    self.wr(" ")
                    self.hilite(0)
                if len(l[1]) < Editor.width:
                    self.clear_to_eol()
                Editor.scrbuf[c] = l
            line += 1
    self.goto(Editor.height, 0)
    self.hilite(1)
    self.wr(
        Editor.TERMCMD[14 if Editor.width > 40 else 15].format(
            chd=self.changed,
            file=self.fname,
            row=self.cur_line + 1,
            total=self.total_lines,
            col=self.vcol + 1,
            msg=self.message,
        )[: self.width - 1]
    )
    self.clear_to_eol()
    self.hilite(0)
    self.goto(self.row, self.vcol - self.margin)
    self.cursor(True)


class TFB_IO:
    def __init__(self):
        try:
            from micropython import kbd_intr
            kbd_intr(-1)
        except:
            pass

    def wr(self, s):
        print(s, end="")

    def rd(self):
        return chr(tulip.key_wait()[0])

    def rd_raw(self):
        return self.rd()

    def deinit_tty(self):
        try:
            from micropython import kbd_intr
            kbd_intr(3)
        except:
            pass

    def get_screen_size(self):
        cols, rows = tulip.tfb_size()
        return [rows, cols]


def _setup():
    Editor = _pye.Editor
    # 7,8: mouse reporting on/off. 9,10: scroll one line up/down.
    # 11,12: set/reset scrolling region. None of these on the TFB.
    for i in (7, 8, 9, 10, 11, 12):
        Editor.TERMCMD[i] = ""
    # Old-editor colors: normal text, status line (inverse, like the old
    # editor's bottom bar), and marked text (the old selection bg).
    Editor.TERMCMD[4] = "\x1b[0m" + _fg_esc(_FG) + _bg_esc(_BG)
    Editor.TERMCMD[5] = "\x1b[0m\x1b[7m" + _fg_esc(_FG) + _bg_esc(_BG)
    Editor.TERMCMD[6] = _bg_esc(_SELECTION_BG)
    Editor.display_window = _display_window

    # Keep pye's own keys (ctrl-S save, ctrl-F find, ctrl-Z undo, ctrl-G goto,
    # ctrl-C copy, ctrl-L mark, ctrl-O open, ctrl-R find+replace, ESC-ESC
    # quit) and layer the old C editor's bindings on top. Where they collide,
    # the old editor wins; pye's displaced actions are noted per line.
    K = Editor.KEYMAP
    # Tulip's keyboard sends 0x08 for the backspace key; stock pye expects
    # 0x7f and treats 0x08 (ctrl-H) as find-replace, which is still on ctrl-R.
    K["\x08"] = _pye.KEY_BACKSPACE
    K["\x18"] = _pye.KEY_WRITE   # ctrl-X: save (prompts for name; was pye cut -- use ctrl-K)
    K["\x0b"] = _pye.KEY_CUT     # ctrl-K: cut line/selection, old editor yank (was bracket match)
    K["\x15"] = _pye.KEY_PASTE   # ctrl-U: paste, old editor unyank (was backtab; still shift-tab)
    K["\x17"] = _pye.KEY_FIND    # ctrl-W: search, old editor (was next file)
    K["\x01"] = _pye.KEY_HOME    # ctrl-A: start of line, old editor (was autoindent toggle)
    K["\x05"] = _pye.KEY_END     # ctrl-E: end of line, old editor (was redraw)
    K["\x19"] = _pye.KEY_PGUP    # ctrl-Y: page up, old editor (was redo)
    K["\x16"] = _pye.KEY_PGDN    # ctrl-V: page down, old editor (paste is ctrl-U)


def pye(*args, tab_size=4, undo=50):
    _setup()
    io_device = TFB_IO()
    # Like the old editor: stash the REPL screen and colors, restore on exit
    tulip.tfb_save()
    try:
        ret = _pye.pye_edit(args, tab_size=tab_size, undo=undo, io_device=io_device)
    finally:
        io_device.deinit_tty()
        # reset ANSI colors and bring the cursor back for the REPL
        io_device.wr("\x1b[0m\x1b[?25h")
        tulip.tfb_restore()
    return ret
