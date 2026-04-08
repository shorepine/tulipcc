##
## Small python text editor based on the
## Very simple VT100 terminal text editor widget
## Copyright (c) 2015 Paul Sokolovsky (initial code)
## Copyright (c) 2015-2025 Robert Hammelrath (additional code)
## Distributed under MIT License
## Changes:
## - Ported the code to boards from micropython.org, Pycom Boards,
##   sipeed boards, Adafruit Circuitpython boards (still runs on Linux or Darwin)
## - changed read keyboard function to comply with char-by-char input
## - added support for TAB, BACKTAB, SAVE, DEL and Backspace joining lines,
##   Find, Replace, Goto Line, UNDO, REDO, GET file, Auto-Indent, Set Flags,
##   Copy/Cut & Paste, Indent, Dedent, line and character move
## - Basic mouse support for pointing, scrolling and selecting
## - handling tab (0x09) on reading & writing files,
## - Added a status line and single line prompts for
##   Quit, Save, Find, Replace, Flags and Goto
## - moved main into a function with some optional parameters
## - Added multi-file support
##

PYE_VERSION = " V2.80 "
try:
    import usys as sys
except:
    import sys
import gc

if sys.implementation.name == "micropython" or sys.implementation.name == "circuitpython":
    is_micropython = True
    import os as os
    from io import StringIO
else:
    is_micropython = False
    import os
    from _io import StringIO

    def const(x):
        return x

from re import compile as re_compile
import time

KEY_NONE = const(0x00)
KEY_UP = const(0x0B)
KEY_DOWN = const(0x0D)
KEY_LEFT = const(0x1F)
KEY_RIGHT = const(0x1E)
KEY_HOME = const(0x10)
KEY_END = const(0x03)
KEY_PGUP = const(0xFFF1)
KEY_PGDN = const(0xFFF2)
KEY_WORD_LEFT = const(0xFFF3)
KEY_WORD_RIGHT = const(0xFFF4)
KEY_SHIFT_UP = const(0xFFF5)
KEY_ALT_UP = const(0xFFEA)
KEY_SHIFT_DOWN = const(0xFFF6)
KEY_ALT_DOWN = const(0xFFEB)
KEY_SHIFT_LEFT = const(0xFFF0)
KEY_ALT_LEFT = const(0xFFE9)
KEY_SHIFT_CTRL_LEFT = const(0xFFED)
KEY_SHIFT_RIGHT = const(0xFFEF)
KEY_ALT_RIGHT = const(0xFFE8)
KEY_SHIFT_CTRL_RIGHT = const(0xFFEC)
KEY_QUIT = const(0x11)
KEY_FORCE_QUIT = const(0xFFE6)
KEY_ENTER = const(0x0A)
KEY_BACKSPACE = const(0x08)
KEY_DELETE = const(0x7F)
KEY_DEL_WORD = const(0xFFF7)
KEY_DEL_LINE = const(0xFFE7)
KEY_WRITE = const(0x13)
KEY_TAB = const(0x09)
KEY_BACKTAB = const(0x15)
KEY_FIND = const(0x06)
KEY_GOTO = const(0x07)
KEY_MOUSE = const(0x1B)
KEY_SCRLUP = const(0x1C)
KEY_SCRLDN = const(0x1D)
KEY_FIND_AGAIN = const(0x0E)
KEY_REDRAW = const(0x05)
KEY_UNDO = const(0x1A)
KEY_REDO = const(0xFFEE)
KEY_CUT = const(0x18)
KEY_PASTE = const(0x16)
KEY_COPY = const(0x04)
KEY_FIRST = const(0x14)
KEY_LAST = const(0x02)
KEY_REPLC = const(0x12)
KEY_TOGGLE = const(0x01)
KEY_GET = const(0x0F)
KEY_MARK = const(0x0C)
KEY_NEXT = const(0x17)
KEY_PREV = const(0xFFE5)
KEY_COMMENT = const(0xFFFC)
KEY_MATCH = const(0xFFFD)
KEY_INDENT = const(0xFFFE)
KEY_DEDENT = const(0xFFFF)
KEY_PLACE = const(0xFFE4)
KEY_NEXT_PLACE = const(0xFFE3)
KEY_PREV_PLACE = const(0xFFE2)
KEY_UNDO_PREV = const(0xFFE1)
KEY_UNDO_NEXT = const(0xFFE0)
KEY_UNDO_YANK = const(0xFFDF)


class Editor:
    KEYMAP = {  ## Gets lengthy
        "\x1b[A": KEY_UP,
        "\x1b[1;2A": KEY_SHIFT_UP,
        "\x1b[1;3A": KEY_ALT_UP,
        "\x1b[B": KEY_DOWN,
        "\x1b[1;2B": KEY_SHIFT_DOWN,
        "\x1b[1;3B": KEY_ALT_DOWN,
        "\x1b[D": KEY_LEFT,
        "\x1b[1;2D": KEY_SHIFT_LEFT,
        "\x1b[1;6D": KEY_SHIFT_CTRL_LEFT,
        "\x1b[1;3D": KEY_ALT_LEFT,
        "\x1b[C": KEY_RIGHT,
        "\x1b[1;2C": KEY_SHIFT_RIGHT,
        "\x1b[1;6C": KEY_SHIFT_CTRL_RIGHT,
        "\x1b[1;3C": KEY_ALT_RIGHT,
        "\x1b[H": KEY_HOME,  ## in Linux Terminal
        "\x1bOH": KEY_HOME,  ## Picocom, Minicom
        "\x1b[1~": KEY_HOME,  ## Putty
        "\x1b[F": KEY_END,  ## Linux Terminal
        "\x1bOF": KEY_END,  ## Picocom, Minicom
        "\x1b[4~": KEY_END,  ## Putty
        "\x1b[5~": KEY_PGUP,
        "\x1b[6~": KEY_PGDN,
        "\x1b[5;5~": KEY_PREV,  ## Ctrl-PgUp
        "\x1b[6;5~": KEY_NEXT,  ## Ctr-PgDn
        "\x1b[1;5D": KEY_WORD_LEFT,
        "\x1b[1;5C": KEY_WORD_RIGHT,
        "\x03": KEY_COPY,  ## Ctrl-C
        "\r": KEY_ENTER,
        "\x7f": KEY_BACKSPACE,  ## Ctrl-? (127)
        "\x1b[3~": KEY_DELETE,
        "\x1b[Z": KEY_BACKTAB,  ## Shift Tab
        "\x19": KEY_REDO,  ## Ctrl-Y
        "\x08": KEY_REPLC,  ## Ctrl-H
        "\x12": KEY_REPLC,  ## Ctrl-R
        "\x11": KEY_QUIT,  ## Ctrl-Q
        "\x1b": KEY_QUIT,  ## Escape twice
        "\n": KEY_ENTER,
        "\x13": KEY_WRITE,  ## Ctrl-S
        "\x06": KEY_FIND,  ## Ctrl-F
        "\x0e": KEY_FIND_AGAIN,  ## Ctrl-N
        "\x07": KEY_GOTO,  ##  Ctrl-G
        "\x05": KEY_REDRAW,  ## Ctrl-E
        "\x1a": KEY_UNDO,  ## Ctrl-Z
        "\x09": KEY_TAB,
        "\x15": KEY_BACKTAB,  ## Ctrl-U
        "\x18": KEY_CUT,  ## Ctrl-X
        "\x16": KEY_PASTE,  ## Ctrl-V
        "\x04": KEY_UNDO_YANK,  ## Ctrl-D
        "\x0c": KEY_MARK,  ## Ctrl-L
        "\x00": KEY_MARK,  ## Ctrl-Space
        "\x14": KEY_FIRST,  ## Ctrl-T
        "\x02": KEY_LAST,  ## Ctrl-B
        "\x01": KEY_TOGGLE,  ## Ctrl-A
        "\x17": KEY_NEXT,  ## Ctrl-W
        "\x0f": KEY_GET,  ## Ctrl-O
        "\x10": KEY_COMMENT,  ## Ctrl-P
        "\x1f": KEY_COMMENT,  ## Ctrl-P
        ## other keys
        "\x1b[1;5A": KEY_SCRLUP,  ## Ctrl-Up
        "\x1b[1;5B": KEY_SCRLDN,  ## Ctrl-Down
        "\x1b[1;5H": KEY_FIRST,  ## Ctrl-Home
        "\x1b[1;5F": KEY_LAST,  ## Ctrl-End
        "\x1b[3;5~": KEY_DEL_WORD,  ## Ctrl-Del
        "\x1b[3;2~": KEY_DEL_LINE,  ## Shift-Del
        "\x0b": KEY_MATCH,  ## Ctrl-K
        "\x1b[M": KEY_MOUSE,
        "\x1b[2;3~": KEY_PLACE,  ## Alt-Ins
        "\x1b[5;3~": KEY_PREV_PLACE,  ## Alt-PgUp
        "\x1b[6;3~": KEY_NEXT_PLACE,  ## Alt-PgDn
        "\x1b[1;3H": KEY_UNDO_PREV,  ## Alt-Home
        "\x1b[1;3F": KEY_UNDO_NEXT,  ## Alt-End
    }

    TERMCMD = [  ## list of terminal control strings
        "\x1b[{row};{col}H",  ## 0: Set cursor
        "\x1b[0K",  ## 1: Clear EOL
        "\x1b[?25h",  ## 2: Cursor ON
        "\x1b[?25l",  ## 3: Cursor OFF
        "\x1b[0m",  ## 4: Hilite 0 - normal text
        "\x1b[1;37;44m",  ## 5: Hilite 1 - Entering the status line
        "\x1b[43m",  ## 6: Hilite 2 - Highligthing Text
        "\x1b[?9h",  ## 7: Mouse reporting on
        "\x1b[?9l",  ## 8: Mouse reporting off
        "\x1bM",  ## 9: Scroll one line up
        "\n",  ## 10: Scroll one line down
        "\x1b[1;{stop}r",  ## 11: Set lowest line of scrolling range
        "\x1b[r",  ## 12: Scroll the full screen
        "\b",  ## 13: backspace one character, used in line_edit
        ## 14: Long status line format string.
        "{chd}{file} Row: {row}/{total} Col: {col}  {msg}",
        ## 15: Shorter status line format string.
        "{chd}{file} {row}:{col}  {msg}",
    ]

    ## symbols that are shared between instances of Editor
    yank_buffer = []
    find_pattern = ""
    case = "n"
    autoindent = "y"
    replc_pattern = ""
    comment_char = "\x23 "  ## for #
    word_char = "_\\"  ## additional characters in a word
    file_char = "_.-"  # additional characters in a file name
    match_span = 50  ## number of lines to search for a bracket match
    place_list = []  ##
    place_index = 0
    max_places = 20

    def __init__(self, tab_size, undo_limit, io_device):
        self.top_line = self.cur_line = self.row = self.vcol = self.col = self.margin = 0
        self.tab_size = tab_size
        self.changed = ""
        self.hash = 0
        self.message = self.fname = ""
        self.content = [""]
        self.undo = []
        self.undo_limit = undo_limit
        self.undo_index = 0
        self.redo = []
        self.clear_mark()
        self.write_tabs = "n"
        self.work_dir = os.getcwd()
        self.io_device = io_device
        self.wr = io_device.wr
        self.is_dir = False
        self.key_max = 0
        for _ in Editor.KEYMAP.keys():
            self.key_max = max(self.key_max, len(_))

    def goto(self, row, col):
        self.wr(Editor.TERMCMD[0].format(row=row + 1, col=col + 1))

    def clear_to_eol(self):
        self.wr(Editor.TERMCMD[1])

    def cursor(self, onoff):
        self.wr(Editor.TERMCMD[2] if onoff else Editor.TERMCMD[3])

    def hilite(self, mode):
        if mode == 1:  ## used for the status line
            self.wr(Editor.TERMCMD[5])
        elif mode == 2:  ## used for the marked area
            self.wr(Editor.TERMCMD[6])
        else:  ## plain text
            self.wr(Editor.TERMCMD[4])

    def mouse_reporting(self, onoff):
        self.wr(
            Editor.TERMCMD[7] if onoff else Editor.TERMCMD[8]
        )  ## enable/disable mouse reporting

    def scroll_region(self, stop):
        self.wr(
            Editor.TERMCMD[11].format(stop=stop) if stop else Editor.TERMCMD[12]
        )  ## set scrolling range

    def scroll_up(self, scrolling):
        if Editor.TERMCMD[9]:
            Editor.scrbuf[scrolling:] = Editor.scrbuf[:-scrolling]
            Editor.scrbuf[:scrolling] = [""] * scrolling
            self.goto(0, 0)
            self.wr(Editor.TERMCMD[9] * scrolling)

    def scroll_down(self, scrolling):
        if Editor.TERMCMD[10]:
            Editor.scrbuf[:-scrolling] = Editor.scrbuf[scrolling:]
            Editor.scrbuf[-scrolling:] = [""] * scrolling
            self.goto(Editor.height - 1, 0)
            self.wr(Editor.TERMCMD[10] * scrolling)

    def redraw(self, flag):
        self.cursor(False)
        Editor.height, Editor.width = self.io_device.get_screen_size()
        Editor.height -= 1
        Editor.scrbuf = [(False, "\x00")] * Editor.height  ## force delete
        self.row = min(Editor.height - 1, self.row)
        self.scroll_region(Editor.height)
        self.mouse_reporting(True)  ## enable mouse reporting
        if flag:
            self.message = PYE_VERSION
        if is_micropython:
            gc.collect()
            if flag:
                self.message += "{} Bytes Memory available".format(gc.mem_free())
        self.changed = "" if self.hash == self.hash_buffer() else "*"

    def get_input(
        self,
    ):  ## read from interface/keyboard one byte each and match against function keys
        while True:
            in_buffer = self.io_device.rd()
            if in_buffer[0] == "\x1b":  ## starting with ESC, must be fct
                while True:
                    in_buffer += self.io_device.rd()
                    c = in_buffer[-1]
                    if c == "~" or (c.isalpha() and len(in_buffer) > 2):
                        break
                    ## map alt-chr aka ESC-char onto ctrl-chr, except for ESC-O
                    if len(in_buffer) == 2 and c.isalpha() and c != "O":
                        in_buffer = chr(ord(in_buffer[1]) & 0x1F)
                        break
                    ## stop if sequence cannot be found
                    if len(in_buffer) >= self.key_max:
                        break
                    ## escape entered twice: escape!
                    if in_buffer == "\x1b\x1b":
                        in_buffer = "\x1b"
                        break
            if in_buffer in Editor.KEYMAP:
                c = Editor.KEYMAP[in_buffer]
                if c != KEY_MOUSE:
                    return c, None
                else:  ## special for mice
                    mouse_fct = ord(self.io_device.rd_raw())  ## read 3 more chars
                    mouse_x = ord(self.io_device.rd_raw()) - 33
                    mouse_y = ord(self.io_device.rd_raw()) - 33
                    if mouse_fct == 0x61:
                        return KEY_SCRLDN, 3
                    elif mouse_fct == 0x60:
                        return KEY_SCRLUP, 3
                    else:
                        return KEY_MOUSE, [mouse_x, mouse_y, mouse_fct]  ## set the cursor
            elif ord(in_buffer[0]) >= 32:
                return KEY_NONE, in_buffer

    def display_window(self):  ## Update window and status line
        ## Force cur_line and col to be in the reasonable bounds
        self.cur_line = min(self.total_lines - 1, max(self.cur_line, 0))
        self.vcol = max(0, min(self.col, len(self.content[self.cur_line])))
        ## Check if Column is out of view, and align margin if needed
        if self.vcol >= Editor.width + self.margin:
            self.margin = self.vcol - Editor.width + (Editor.width >> 2)
        elif self.vcol < self.margin:
            self.margin = max(self.vcol - (Editor.width >> 2), 0)
        ## if cur_line is out of view, align top_line to the given row
        if not (self.top_line <= self.cur_line < self.top_line + Editor.height):  # Visible?
            self.top_line = max(self.cur_line - self.row, 0)
        ## in any case, align row to top_line and cur_line
        self.row = self.cur_line - self.top_line
        ## update_screen
        self.cursor(False)
        line = self.top_line
        if self.mark is None:
            flag = 0
        else:
            start_line, start_col, end_line, end_col = self.mark_range()
            start_col = max(start_col - self.margin, 0)
            end_col = max(end_col - self.margin, 0)

        for c in range(Editor.height):
            if line == self.total_lines:  ## at empty bottom screen part
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
                if (flag and line == self.cur_line) or l != Editor.scrbuf[
                    c
                ]:  ## line changed, print it
                    self.goto(c, 0)
                    if flag == 0:  # no mark
                        self.wr(l[1])
                    elif flag == 7:  # only line of a mark
                        self.wr(l[1][:start_col])
                        self.hilite(2)
                        self.wr(l[1][start_col:end_col])
                        self.hilite(0)
                        self.wr(l[1][end_col:])
                    elif flag == 3:  # first line of mark
                        self.wr(l[1][:start_col])
                        self.hilite(2)
                        self.wr(l[1][start_col:])
                        self.wr(" ")
                        self.hilite(0)
                    elif flag == 5:  # last line of mark
                        self.hilite(2)
                        self.wr(l[1][:end_col])
                        self.hilite(0)
                        self.wr(l[1][end_col:])
                    else:  # middle line of a mark
                        self.hilite(2)
                        self.wr(l[1])
                        self.wr(" ")
                        self.hilite(0)
                    if len(l[1]) < Editor.width:
                        self.clear_to_eol()
                    Editor.scrbuf[c] = l
                line += 1
        ## display Status-Line
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
        self.clear_to_eol()  ## once moved up for mate/xfce4-terminal issue with scroll region
        self.hilite(0)
        self.goto(self.row, self.vcol - self.margin)
        self.cursor(True)

    def spaces(self, line, pos=None):  ## count spaces
        return (
            len(line) - len(line.lstrip(" "))
            if pos is None
            else len(line[:pos]) - len(line[:pos].rstrip(" "))  ## at line start
        )

    def mark_range(self):
        if self.mark_order(self.cur_line, self.col) >= 0:
            return (self.mark[0], self.mark[1], self.cur_line + 1, self.col)
        else:
            return (self.cur_line, self.col, self.mark[0] + 1, self.mark[1])

    def mark_order(self, line, col):
        return col - self.mark[1] if self.mark[0] == line else line - self.mark[0]

    def line_range(self):
        res = self.mark_range()
        return (res[0], res[2]) if res[3] > 0 else (res[0], res[2] - 1)

    def line_edit(
        self, prompt, default, zap=None
    ):  ## better one: added cursor keys and backsp, delete
        def push_msg(msg):
            self.wr(msg + Editor.TERMCMD[13] * len(msg))  ## Write a message and move cursor back

        self.goto(Editor.height, 0)
        self.hilite(1)
        self.wr(prompt)
        self.wr(default)
        self.clear_to_eol()
        res = default
        pos = len(res)
        del_all = True
        mouse_last = None
        while True:
            key, char = self.get_input()  ## Get Char of Fct.
            if key == KEY_NONE:  ## char to be inserted
                if len(prompt) + len(res) < self.width - 2:
                    res = res[:pos] + char + res[pos:]
                    self.wr(res[pos])
                    pos += len(char)
                    push_msg(res[pos:])  ## update tail
            elif key in (KEY_ENTER, KEY_TAB):  ## Finis
                self.hilite(0)
                return res
            elif key in (KEY_QUIT, KEY_COPY):  ## Abort
                self.hilite(0)
                return None
            elif key == KEY_LEFT:
                if pos > 0:
                    self.wr(Editor.TERMCMD[13])
                    pos -= 1
            elif key == KEY_RIGHT:
                if pos < len(res):
                    self.wr(res[pos])
                    pos += 1
            elif key == KEY_HOME:
                self.wr(Editor.TERMCMD[13] * pos)
                pos = 0
            elif key == KEY_END:
                self.wr(res[pos:])
                pos = len(res)
            elif key == KEY_DELETE:  ## Delete
                if del_all:
                    self.wr(Editor.TERMCMD[13] * pos)
                    self.wr(" " * len(res))
                    self.wr(Editor.TERMCMD[13] * len(res))
                    pos = 0
                    res = ""
                else:
                    if pos < len(res):
                        res = res[:pos] + res[pos + 1 :]
                        push_msg(res[pos:] + " ")  ## update tail
            elif key == KEY_BACKSPACE:  ## Backspace
                if pos > 0:
                    res = res[: pos - 1] + res[pos:]
                    self.wr(Editor.TERMCMD[13])
                    pos -= 1
                    push_msg(res[pos:] + " ")  ## update tail
            elif key == KEY_PASTE:  ## Get from content
                res += self.getsymbol(self.content[self.cur_line], self.col, zap)[
                    : Editor.width - pos - len(prompt) - 1
                ]
                push_msg(res[pos:])
            elif key == KEY_MOUSE:
                if char[1] < Editor.height and (char[1] + self.top_line) < self.total_lines:
                    self.col = char[0] + self.margin
                    self.cur_line = char[1] + self.top_line
                    if (self.col, self.cur_line) != mouse_last:  ## first click: copy
                        mouse_last = (self.col, self.cur_line)
                        self.wr(Editor.TERMCMD[13] * pos)
                        self.wr(" " * len(res))
                        self.wr(Editor.TERMCMD[13] * len(res))
                        pos = 0
                        res = self.getsymbol(self.content[self.cur_line], self.col, zap)
                        push_msg(res)
                    else:  ## second click: Go
                        self.hilite(0)
                        return res
            del_all = False

    def getsymbol(self, s, pos, zap):
        if pos < len(s) and zap is not None:
            start = self.skip_while(s, pos, zap, -1)
            stop = self.skip_while(s, pos, zap, 1)
            return s[start + 1 : stop]
        else:
            return ""

    def issymbol(self, c, zap):
        return c.isalpha() or c.isdigit() or c in zap

    def skip_until(self, s, pos, zap, way):
        stop = -1 if way < 0 else len(s)
        while pos != stop and not self.issymbol(s[pos], zap):
            pos += way
        return pos

    def skip_while(self, s, pos, zap, way):
        stop = -1 if way < 0 else len(s)
        while pos != stop and self.issymbol(s[pos], zap):
            pos += way
        return pos

    def move_up(self):
        if self.cur_line > 0:
            self.cur_line -= 1
            if self.cur_line < self.top_line:
                self.scroll_up(1)

    def skip_up(self):
        if self.col == 0 and self.cur_line > 0:
            self.col = len(self.content[self.cur_line - 1])
            self.move_up()
            return True
        else:
            return False

    def move_left(self):
        self.col = self.vcol
        if not self.skip_up():
            self.col -= 1

    def move_down(self):
        if self.cur_line < self.total_lines - 1:
            self.cur_line += 1
            if self.cur_line == self.top_line + Editor.height:
                self.scroll_down(1)

    def skip_down(self, l):
        if self.col >= len(l) and self.cur_line < self.total_lines - 1:
            self.col = 0
            self.move_down()
            return True
        else:
            return False

    def move_right(self, l):
        if not self.skip_down(l):
            self.col += 1

    ## This is the regex version of find.
    def find_in_file(self, pattern, col, end):
        Editor.find_pattern = pattern  ## remember it
        if Editor.case != "y":
            pattern = pattern.lower()
        try:
            rex = re_compile(pattern)
        except:
            self.message = "Invalid pattern: " + pattern
            return None
        start = self.cur_line
        if col > len(self.content[start]) or (  # After EOL
            pattern[0] == "^" and col != 0
        ):  # or anchored and not at BOL
            start, col = start + 1, 0  # Skip to the next line
        for line in range(start, end):
            l = self.content[line][col:]
            if Editor.case != "y":
                l = l.lower()
            match = rex.search(l)
            if match:  # Bingo
                self.cur_line = line
                ## Instead of match.span, a simple find has to be performed
                ## to get the cursor position.
                ## And '$' has to be treated separately, so look for a true EOL match first
                if pattern[-1:] == "$" and match.group(0)[-1:] != "$":
                    self.col = col + len(l) - len(match.group(0))
                else:
                    self.col = col + l.find(match.group(0))
                return len(match.group(0))
            col = 0
        else:
            self.message = Editor.find_pattern + " not found (again)"
            return None

    def undo_add(self, lnum, text, key, span=1, chain=False):
        if (
            len(self.undo) == 0
            or key == KEY_NONE
            or self.undo[-1][3] != key
            or self.undo[-1][0] != lnum
        ):
            self.changed = "*"
            if len(self.undo) >= self.undo_limit:  ## drop oldest undo(s), if full
                del self.undo[0]
            self.undo.append([lnum, span, text, key, self.col, chain])
            self.redo = []  ## clear re-do list.

    def undo_redo(self, undo, redo):
        chain = True
        redo_start = len(redo)
        while len(undo) > 0 and chain:
            action = undo.pop()  ## get action from stack
            if action[3] not in (KEY_INDENT, KEY_DEDENT, KEY_COMMENT):
                self.cur_line = action[0]  ## wrong for Bkspc of BOL
            self.col = action[4]
            if len(redo) >= self.undo_limit:  ## mybe not enough
                del redo[0]
            if action[1] >= 0:  ## insert or replace line
                redo.append(
                    action[0:1]
                    + [len(action[2])]
                    + [self.content[action[0] : action[0] + action[1]]]  ## safe to redo stack
                    + action[3:]
                )
                if action[0] < self.total_lines:
                    self.content[action[0] : action[0] + action[1]] = action[2]  # insert lines
                else:
                    self.content += action[2]
            else:  ## delete lines, restore the current line
                redo.append(
                    action[0:1]
                    + [1]
                    + [  ## undo deletes, redo inserts
                        self.content[action[0] : action[0] - action[1] + 1]
                    ]
                    + action[3:]
                )
                del self.content[action[0] : action[0] - action[1]]
                self.content[action[0]] = action[2][0]  # replace current line with save content
            chain = action[5]
        if (len(redo) - redo_start) > 0:  ## Performed at least one action
            redo[-1][5] = True  ## fix the chaining flags for reversed action order.
            redo[redo_start][5] = False
            self.total_lines = len(self.content)  ## Reset the length and change indicator
            self.changed = "" if self.hash == self.hash_buffer() else "*"
            self.clear_mark()

    def set_mark(self, flag=999999999):  ## start the highlighting if not done yet
        if self.mark is None:
            self.mark = (self.cur_line, self.col)
        if self.mark_flag < flag:
            self.mark_flag = flag

    def check_mark(self):  ## Check whether to auto-unmark
        if self.mark is not None:
            self.mark_flag -= 1
            if self.mark_flag <= 0:
                self.clear_mark()

    def clear_mark(self):
        self.mark = None
        self.mark_flag = 0
        self.mouse_last = (0, 0, 0)

    def yank_mark(self):  # Copy marked area to the yank buffer
        start_row, start_col, end_row, end_col = self.mark_range()
        ## copy first the whole area
        Editor.yank_buffer = self.content[start_row:end_row]
        ## then remove parts that do not have to be copied. Last line first
        Editor.yank_buffer[-1] = Editor.yank_buffer[-1][:end_col]
        Editor.yank_buffer[0] = Editor.yank_buffer[0][start_col:]

    def delete_mark(self, yank):  ## copy marked lines (opt) and delete them
        if yank:
            self.yank_mark()
        ## delete by composing fractional lines into the first one and erase remaining lines
        start_row, start_col, end_row, end_col = self.mark_range()
        self.undo_add(start_row, self.content[start_row:end_row], KEY_NONE, 1, False)
        self.content[start_row] = (
            self.content[start_row][:start_col] + self.content[end_row - 1][end_col:]
        )
        if start_row + 1 < end_row:
            del self.content[start_row + 1 : end_row]  ## delete the remaining area
        self.col = start_col

        if self.content == []:  ## if all was wiped
            self.content = [""]  ## add a line
            self.undo[-1][1] = 1  ## tell undo to overwrite this single line
        self.total_lines = len(self.content)
        self.cur_line = start_row
        self.clear_mark()  ## unset line mark

    def handle_edit_keys(self, key, char):  ## keys which change content
        l = self.content[self.cur_line]
        if key == KEY_NONE:  ## character to be added
            self.col = self.vcol
            if self.mark is not None:
                self.delete_mark(False)
                l = self.content[self.cur_line]
                chain = True
            else:
                chain = False
            self.undo_add(self.cur_line, [l], 0x20 if char == " " else 0x41, 1, chain)
            self.content[self.cur_line] = l[: self.col] + char + l[self.col :]
            self.col += len(char)
            return key  ## return here for a marginally faster paste
        elif key == KEY_SHIFT_CTRL_LEFT:
            self.set_mark()
            key = KEY_WORD_LEFT
        elif key == KEY_SHIFT_CTRL_RIGHT:
            self.set_mark()
            key = KEY_WORD_RIGHT
        elif key == KEY_MOUSE:  ## Set Cursor or open file/find
            if char[2] == 0x22:  ## right click opens find or get
                key = KEY_GET if self.is_dir else KEY_FIND
            elif char[1] < Editor.height:
                col = char[0] + self.margin
                line = char[1] + self.top_line
                if (col, line) == self.mouse_last[:2] and (time.time() - self.mouse_last[2]) < 2:
                    self.mouse_last = (0, 0, 0)
                    if (
                        self.mark is None
                        and col < len(l)
                        and self.issymbol(l[col], Editor.word_char)
                    ):
                        self.col = self.skip_while(l, col, Editor.word_char, -1) + 1
                        self.set_mark()
                        self.col = self.skip_while(l, self.col, Editor.word_char, 1)
                    else:  ## toggle single char mark
                        key = KEY_MARK
                else:
                    if self.mark is not None:
                        if (
                            self.mark_order(self.cur_line, self.col) * self.mark_order(line, col)
                            < 0
                        ):
                            self.mark = self.cur_line, self.col
                    self.cur_line, self.col = line, col
                    self.mouse_last = (col, line, time.time())
        ## start new if/elif sequence, since the value of key might have changed
        if key == KEY_DOWN:
            self.move_down()
        elif key == KEY_UP:
            self.move_up()
        elif key == KEY_LEFT:
            self.move_left()
        elif key == KEY_RIGHT:
            self.move_right(l)
        elif key == KEY_WORD_LEFT:
            self.col = self.vcol
            if self.skip_up():
                l = self.content[self.cur_line]
            pos = self.skip_until(l, self.col - 1, Editor.word_char, -1)
            self.col = self.skip_while(l, pos, Editor.word_char, -1) + 1
        elif key == KEY_WORD_RIGHT:
            if self.skip_down(l):
                l = self.content[self.cur_line]
            pos = self.skip_until(l, self.col, Editor.word_char, 1)
            self.col = self.skip_while(l, pos, Editor.word_char, 1)
        elif key == KEY_DELETE:
            self.col = self.vcol
            if self.mark is not None:
                self.delete_mark(False)
            elif self.col < len(l):
                self.undo_add(self.cur_line, [l], KEY_DELETE)
                self.content[self.cur_line] = l[: self.col] + l[self.col + 1 :]
            elif (self.cur_line + 1) < self.total_lines:  ## test for last line
                self.undo_add(self.cur_line, [l, self.content[self.cur_line + 1]], KEY_NONE)
                self.content[self.cur_line] = l + (
                    self.content.pop(self.cur_line + 1).lstrip()
                    if Editor.autoindent == "y" and self.col > 0
                    else self.content.pop(self.cur_line + 1)
                )
                self.total_lines -= 1
        elif key == KEY_BACKSPACE:
            self.col = self.vcol
            if self.mark is not None:
                self.delete_mark(False)
            elif self.col > 0:
                self.undo_add(self.cur_line, [l], KEY_BACKSPACE)
                self.content[self.cur_line] = l[: self.col - 1] + l[self.col :]
                self.col -= 1
            elif self.cur_line > 0:  # at the start of a line, but not the first
                self.undo_add(self.cur_line - 1, [self.content[self.cur_line - 1], l], KEY_NONE)
                self.col = len(self.content[self.cur_line - 1])
                self.content[self.cur_line - 1] += self.content.pop(self.cur_line)
                self.cur_line -= 1
                self.total_lines -= 1
        elif key == KEY_DEL_WORD:
            if self.col < len(l):
                pos = self.skip_while(l, self.col, Editor.word_char, 1)
                pos += self.spaces(l[pos:])
                if self.col < pos:
                    self.undo_add(self.cur_line, [l], KEY_DEL_WORD)
                    self.content[self.cur_line] = l[: self.col] + l[pos:]
        elif key == KEY_DEL_LINE:
            if self.cur_line < (self.total_lines - 1):
                self.undo_add(self.cur_line, [l, self.content[self.cur_line + 1]], KEY_NONE, 1)
            else:
                self.undo_add(self.cur_line, [l], KEY_NONE, 1)
            self.content.pop(self.cur_line)
            if self.content == []:
                self.content = [""]
        elif key == KEY_HOME:
            self.col = self.spaces(l) if self.col == 0 else 0
        elif key == KEY_END:
            ni = len(l.split(Editor.comment_char.strip())[0].rstrip())
            ns = self.spaces(l)
            self.col = ni if self.col >= len(l) and ni > ns else len(l)
        elif key == KEY_PGUP:
            self.cur_line -= Editor.height
        elif key == KEY_PGDN:
            self.cur_line += Editor.height
        elif key == KEY_FIND:
            pat = self.line_edit("Find: ", Editor.find_pattern, "_")
            if pat:
                self.clear_mark()
                self.find_in_file(pat, self.col + 1, self.total_lines)
                self.row = Editor.height >> 1
        elif key == KEY_FIND_AGAIN:
            if Editor.find_pattern:
                self.find_in_file(Editor.find_pattern, self.col + 1, self.total_lines)
                self.row = Editor.height >> 1
        elif key == KEY_GOTO:  ## goto line
            line = self.line_edit("Goto Line: ", "")
            if line:
                self.cur_line = int(line) - 1
                self.row = Editor.height >> 1
        elif key == KEY_FIRST:  ## first line
            self.check_mark()
            self.cur_line = 0
        elif key == KEY_LAST:  ## last line
            self.check_mark()
            self.cur_line = self.total_lines - 1
            self.row = Editor.height - 1  ## will be fixed if required
        elif key == KEY_TOGGLE:  ## Toggle Autoindent/Search case/ Tab Size, TAB write
            pat = self.line_edit(
                "Autoindent {}, Search Case {}"
                ", Tabsize {}, Comment {}, Tabwrite {}: ".format(
                    Editor.autoindent,
                    Editor.case,
                    self.tab_size,
                    Editor.comment_char,
                    self.write_tabs,
                ),
                "",
            )
            try:
                res = [i.lstrip().lower() for i in pat.split(",")]
                if res[0]:
                    Editor.autoindent = "y" if res[0][0] == "y" else "n"
                if res[1]:
                    Editor.case = "y" if res[1][0] == "y" else "n"
                if res[2]:
                    self.tab_size = int(res[2])
                if res[3]:
                    Editor.comment_char = res[3]
                if res[4]:
                    self.write_tabs = "y" if res[4][0] == "y" else "n"
            except IndexError:
                pass
        elif key == KEY_SCRLUP:  ##
            ni = 1 if char is None else 3
            if self.top_line > 0:
                self.top_line = max(self.top_line - ni, 0)
                self.cur_line = min(self.cur_line, self.top_line + Editor.height - 1)
                self.scroll_up(ni)
        elif key == KEY_SCRLDN:  ##
            ni = 1 if char is None else 3
            if self.top_line + Editor.height < self.total_lines:
                self.top_line = min(self.top_line + ni, self.total_lines - 1)
                self.cur_line = max(self.cur_line, self.top_line)
                self.scroll_down(ni)
        elif key == KEY_MATCH:
            if self.col < len(l):  ## ony within text
                brackets = "<{[()]}>"
                srch = l[self.col]
                i = brackets.find(srch)
                if i >= 0:  ## found a bracket
                    match = brackets[7 - i]  ## matching bracket
                    level = 0
                    way = 1 if i < 4 else -1  ## set direction up/down
                    i = self.cur_line  ## set starting point
                    c = self.col + way  ## one off the current position
                    lstop = (
                        min(self.total_lines, i + Editor.match_span)
                        if way > 0
                        else max(-1, i - Editor.match_span)
                    )
                    while i != lstop:
                        l = self.content[i]
                        cstop = len(l) if way > 0 else -1
                        if srch in l or match in l:
                            while c != cstop:
                                if l[c] == match:
                                    if level == 0:  ## match found
                                        self.cur_line, self.col = i, c
                                        return key  ## return here instead of ml-breaking
                                    else:
                                        level -= 1
                                elif l[c] == srch:
                                    level += 1
                                c += way
                        i += way
                        ## set starting point for the next line.
                        ## treatment for the first and last line is implicit.
                        c = 0 if way > 0 else len(self.content[i]) - 1
                    self.message = "No match in {} lines".format(abs(lstop - self.cur_line))
        elif key == KEY_MARK:
            if self.mark is None:
                self.set_mark()
                self.move_right(l)
            else:
                self.clear_mark()
        elif key == KEY_SHIFT_DOWN:
            self.set_mark()
            self.move_down()
        elif key == KEY_SHIFT_UP:
            self.set_mark()
            self.move_up()
        elif key == KEY_SHIFT_LEFT:
            self.set_mark()
            self.move_left()
        elif key == KEY_SHIFT_RIGHT:
            self.set_mark()
            self.move_right(l)
        elif key == KEY_ALT_LEFT:
            if self.col > 0 and self.col < len(l):
                self.undo_add(self.cur_line, [l], KEY_ALT_LEFT)
                i = self.col
                self.content[self.cur_line] = l[: i - 1] + l[i] + l[i - 1] + l[i + 1 :]
                self.move_left()
        elif key == KEY_ALT_RIGHT:
            if self.col < (len(l) - 1):
                self.undo_add(self.cur_line, [l], KEY_ALT_RIGHT)
                i = self.col
                self.content[self.cur_line] = l[:i] + l[i + 1] + l[i] + l[i + 2 :]
                self.move_right(l)
        elif key == KEY_ALT_UP:
            if self.mark is None:
                start_line = self.cur_line
                end_line = start_line + 1
            else:
                start_line, end_line = self.line_range()
                if start_line > 0:
                    self.mark = (self.mark[0] - 1, self.mark[1])
            if start_line > 0:
                self.undo_add(
                    start_line - 1,
                    self.content[start_line - 1 : end_line],
                    KEY_NONE,
                    end_line - start_line + 1,
                )
                self.content[start_line - 1 : end_line - 1], self.content[end_line - 1] = (
                    self.content[start_line:end_line],
                    self.content[start_line - 1],
                )
                self.move_up()
        elif key == KEY_ALT_DOWN:
            if self.mark is None:
                start_line = self.cur_line
                end_line = start_line + 1
            else:
                start_line, end_line = self.line_range()
                if end_line < self.total_lines:
                    self.mark = (self.mark[0] + 1, self.mark[1])
                    ## very special case: cursor at the start of the last line
                    if self.cur_line == end_line == (self.total_lines - 1):
                        self.move_left()
            if end_line < self.total_lines:
                self.undo_add(
                    start_line,
                    self.content[start_line : end_line + 1],
                    KEY_NONE,
                    end_line - start_line + 1,
                )
                self.content[start_line + 1 : end_line + 1], self.content[start_line] = (
                    self.content[start_line:end_line],
                    self.content[end_line],
                )
                self.move_down()
        elif key == KEY_ENTER:
            self.col = self.vcol
            self.clear_mark()
            self.undo_add(self.cur_line, [l], KEY_NONE, 2)
            self.content[self.cur_line] = l[: self.col]
            ni = 0
            if Editor.autoindent == "y":  ## Autoindent
                ni = min(self.spaces(l), self.col)  ## query indentation
            self.cur_line += 1
            self.content[self.cur_line : self.cur_line] = [" " * ni + l[self.col :]]
            self.total_lines += 1
            self.col = ni
        elif key == KEY_TAB:
            if self.mark is None:
                self.col = self.vcol
                self.undo_add(self.cur_line, [l], KEY_TAB)
                ni = self.tab_size - self.col % self.tab_size  ## determine spaces to add
                self.content[self.cur_line] = l[: self.col] + " " * ni + l[self.col :]
                self.col += ni
            else:
                lrange = self.line_range()
                self.undo_add(
                    lrange[0],
                    self.content[lrange[0] : lrange[1]],
                    KEY_INDENT,
                    lrange[1] - lrange[0],
                )  ## undo replaces
                for i in range(lrange[0], lrange[1]):
                    if len(self.content[i]) > 0:
                        self.content[i] = (
                            " " * (self.tab_size - self.spaces(self.content[i]) % self.tab_size)
                            + self.content[i]
                        )
        elif key == KEY_BACKTAB:
            if self.mark is None:
                self.col = self.vcol
                ni = min(
                    (self.col - 1) % self.tab_size + 1, self.spaces(l, self.col)
                )  ## determine spaces to drop
                if ni > 0:
                    self.undo_add(self.cur_line, [l], KEY_BACKTAB)
                    self.content[self.cur_line] = l[: self.col - ni] + l[self.col :]
                    self.col -= ni
            else:
                lrange = self.line_range()
                self.undo_add(
                    lrange[0],
                    self.content[lrange[0] : lrange[1]],
                    KEY_DEDENT,
                    lrange[1] - lrange[0],
                )  ## undo replaces
                for i in range(lrange[0], lrange[1]):
                    ns = self.spaces(self.content[i])
                    if ns > 0:
                        self.content[i] = self.content[i][(ns - 1) % self.tab_size + 1 :]
        elif key == KEY_REPLC:
            count = 0
            pat = self.line_edit("Replace: ", Editor.find_pattern, "_")
            if pat:
                rpat = self.line_edit(
                    "With: ", Editor.replc_pattern if Editor.replc_pattern else pat
                )
                if rpat is not None:  ## start with setting up loop parameters
                    Editor.replc_pattern = rpat
                    q = ""
                    cur_line, cur_col = self.cur_line, self.col  ## remember pos
                    if self.mark is not None:  ## Replace in Marked area
                        (self.cur_line, self.col, end_line, end_col) = self.mark_range()
                    else:  ## replace from cur_line to end
                        end_line = self.total_lines
                        end_col = 999999  ## just a large number
                    self.message = "Replace (yes/No/all/quit) ? "
                    chain = False
                    while True:  ## and go
                        ni = self.find_in_file(pat, self.col, end_line)
                        if ni is not None and (
                            self.cur_line != (end_line - 1) or self.col < end_col
                        ):  ## Pattern found
                            if q != "a":
                                self.display_window()
                                key, char = self.get_input()  ## Get Char of Fct.
                                q = char.lower()
                            if q == "q" or key == KEY_QUIT:
                                break
                            elif q in ("a", "y"):
                                self.undo_add(
                                    self.cur_line,
                                    [self.content[self.cur_line]],
                                    KEY_NONE,
                                    1,
                                    chain,
                                )
                                self.content[self.cur_line] = (
                                    self.content[self.cur_line][: self.col]
                                    + rpat
                                    + self.content[self.cur_line][self.col + ni :]
                                )
                                self.col += len(rpat) + (ni == 0)  # ugly but short
                                count += 1
                                ## delete the next line if undo for each replace is preferred.
                                chain = True
                            else:  ## everything else is no
                                self.col += 1
                        else:  ## not found, quit
                            break
                    self.cur_line, self.col = cur_line, cur_col  ## restore pos
                    self.message = "'{}' replaced {} times".format(pat, count)
        elif key == KEY_CUT:  # delete line or line(s) into buffer
            if self.mark is None:
                if self.cur_line < self.total_lines - 1:
                    self.mark = (self.cur_line + 1, 0)
                else:
                    self.mark = (self.cur_line, len(l))
                self.col = 0
            self.delete_mark(True)
        elif key == KEY_COPY:  # copy line(s) into buffer
            col = self.col
            if self.mark is None:
                if self.cur_line < self.total_lines - 1:
                    self.mark = (self.cur_line + 1, 0)
                else:
                    self.mark = (self.cur_line, len(l))
                self.col = 0
            self.yank_mark()
            self.clear_mark()
            self.col = col
        elif key == KEY_PASTE:  ## insert buffer
            if Editor.yank_buffer:
                self.col = self.vcol
                if self.mark is not None:
                    self.delete_mark(False)
                    chain = True  ## undo this delete too when undoing paste
                else:
                    chain = False  ## just undo the paste
                ## save the yank buffer state, complete the first and last line and insert it
                head, tail = Editor.yank_buffer[0], Editor.yank_buffer[-1]  ## save the buffer
                Editor.yank_buffer[0] = (
                    self.content[self.cur_line][: self.col] + Editor.yank_buffer[0]
                )
                Editor.yank_buffer[-1] += self.content[self.cur_line][self.col :]
                ni = 1 if len(Editor.yank_buffer) <= 1 else 1 - len(Editor.yank_buffer)
                self.undo_add(
                    self.cur_line, [self.content[self.cur_line]], KEY_NONE, ni, chain
                )  # replace
                self.content[
                    self.cur_line : self.cur_line + 1
                ] = Editor.yank_buffer  # insert lines
                Editor.yank_buffer[-1], Editor.yank_buffer[0] = tail, head  ## restore the buffer

                self.total_lines = len(self.content)
        elif key == KEY_WRITE:
            fname = self.line_edit(
                "Save File: ", self.fname if self.is_dir is False else "", Editor.file_char
            )
            if fname:
                if fname != self.fname:  ## save to a different name, confirm evtl. overwrite
                    try:
                        open(fname).close()  ## if that succeeds, the file exists
                        res = self.line_edit("The file exists! Overwrite (y/N)? ", "N")
                        if not res or res[0].upper() != "Y":
                            return
                    except:
                        pass
                self.put_file(fname)
                self.fname = fname  ## remember (new) name
                self.hash = self.hash_buffer()
                self.changed = ""
                self.is_dir = False
        elif key == KEY_UNDO:
            self.undo_redo(self.undo, self.redo)
        elif key == KEY_REDO:
            self.undo_redo(self.redo, self.undo)
        elif key == KEY_COMMENT:
            if self.mark is None:
                lrange = (self.cur_line, self.cur_line + 1)
            else:
                lrange = self.line_range()
            self.undo_add(
                lrange[0], self.content[lrange[0] : lrange[1]], KEY_COMMENT, lrange[1] - lrange[0]
            )  ## undo replaces
            ni = len(Editor.comment_char)
            for i in range(lrange[0], lrange[1]):
                if self.content[i].strip() != "":  ## do not touch empty lines
                    ns = self.spaces(self.content[i])
                    if self.content[i][ns : ns + ni] == Editor.comment_char:
                        self.content[i] = ns * " " + self.content[i][ns + ni :]
                    else:
                        self.content[i] = ns * " " + Editor.comment_char + self.content[i][ns:]
        elif key == KEY_REDRAW:
            self.redraw(True)
        elif key == KEY_PLACE:
            here = (self.cur_line, self)
            if here not in Editor.place_list:
                if len(Editor.place_list) >= Editor.max_places:
                    Editor.place_list.pop(0)
                Editor.place_list.append(here)
                Editor.place_index = len(Editor.place_list) - 1
        elif key == KEY_NEXT_PLACE or key == KEY_PREV_PLACE:
            ni = len(Editor.place_list)
            if ni > 0:
                Editor.place_index = (
                    Editor.place_index + (1 if key == KEY_NEXT_PLACE else -1)
                ) % ni
                here = Editor.place_list[Editor.place_index]
                if here[1] == self:
                    self.cur_line = here[0]
                    self.row = Editor.height >> 1
                else:
                    here[1].cur_line = here[0]
                    return here[1]
        elif key == KEY_UNDO_PREV or key == KEY_UNDO_NEXT:
            if len(self.undo) > 0:
                self.undo_index = (self.undo_index + (1 if key == KEY_UNDO_NEXT else -1)) % len(
                    self.undo
                )
                self.cur_line = self.undo[self.undo_index][0]
                self.col = self.undo[self.undo_index][4]
        elif key == KEY_UNDO_YANK:
            if len(self.undo) > 0:
                Editor.yank_buffer = self.undo[self.undo_index][2]

        return key

    def edit_loop(self):  ## main editing loop
        if not self.content:  ## ensure content
            self.content = [""]
        self.total_lines = len(self.content)
        os.chdir(self.work_dir)
        self.redraw(self.message == "")

        while True:
            self.display_window()  ## Update & display window
            key, char = self.get_input()  ## Get Char of Fct-key code
            self.message = ""  ## clear message

            key = self.handle_edit_keys(key, char)
            if key == KEY_QUIT:
                if self.hash != self.hash_buffer():
                    res = self.line_edit("File changed! Quit (y/N/f)? ", "N")
                    if not res or res[0].upper() == "N":
                        continue
                    if res[0].upper() == "F":
                        key = KEY_FORCE_QUIT
                # remove the file from the remembered places
                Editor.place_list = [item for item in Editor.place_list if item[1] != self]
                Editor.place_index = 0
                self.scroll_region(0)
                self.mouse_reporting(False)  ## disable mouse reporting
                self.goto(Editor.height, 0)
                self.clear_to_eol()
                self.undo = []
                return key
            elif key == KEY_NEXT or key == KEY_PREV or type(key) is Editor:
                return key
            elif key == KEY_GET:
                if self.mark is not None:
                    self.clear_mark()
                    self.display_window()  ## Update & display window
                return key

    ## packtabs: replace sequence of space by tab
    def packtabs(self, s):
        sb = StringIO()
        for i in range(0, len(s), 8):
            c = s[i : i + 8]
            cr = c.rstrip(" ")
            if (len(c) - len(cr)) > 1:
                sb.write(cr + "\t")  ## Spaces at the end of a section
            else:
                sb.write(c)
        return sb.getvalue()

    ## calculate a hash over the content
    def hash_buffer(self):
        res = 0
        for line in self.content:
            res = ((res * 227 + 1) ^ hash(line)) & 0x3FFFFFFF
        return res

    ## Read file into content
    def get_file(self, fname):
        if fname:
            try:
                self.fname = fname
                if fname in (".", "..") or (os.stat(fname)[0] & 0x4000):  ## Dir
                    os.chdir(fname)
                    self.work_dir = os.getcwd()  # let the os module do the normalization
                    self.fname = "/" if self.work_dir == "/" else self.work_dir.split("/")[-1]
                    self.content = ["Directory '{}'".format(self.work_dir), ""] + sorted(
                        os.listdir(".")
                    )
                    self.is_dir = True
                else:
                    if is_micropython:
                        with open(fname) as f:
                            self.content = f.readlines()
                    else:
                        with open(fname, errors="ignore") as f:
                            self.content = f.readlines()
                    self.write_tabs = False
                    i = 0
                    for l in self.content:
                        self.content[i] = self.expandtabs(l.rstrip())
                        i += 1
            except OSError:
                self.message = "Error: file '" + fname + "' may not exist"
        self.hash = self.hash_buffer()

    ## write file
    def put_file(self, fname):
        tmpfile = fname + ".pyetmp"
        with open(tmpfile, "w") as f:
            for l in self.content:
                if self.write_tabs == "y":
                    f.write(self.packtabs(l))
                else:
                    f.write(l)
                f.write("\n")
        try:
            os.remove(fname)
        except:
            pass
        os.rename(tmpfile, fname)

    ## expandtabs: hopefully sometimes replaced by the built-in function
    def expandtabs(self, s):
        if "\t" in s:
            self.write_tabs = True
            sb = StringIO()
            pos = 0
            for c in s:
                if c == "\t":  ## tab is seen
                    sb.write(" " * (8 - pos % 8))  ## replace by space
                    pos += 8 - pos % 8
                else:
                    sb.write(c)
                    pos += 1
            return sb.getvalue()
        else:
            return s


def pye_edit(content, tab_size=4, undo=50, io_device=None):
    ## prepare content
    ## test, if the IO class if provided
    if io_device is None:
        print("IO device not defined")
        return

    gc.collect()  ## all (memory) is mine
    index = 0
    undo = max(4, (undo if type(undo) is int else 0))  # minimum undo size
    current_dir = os.getcwd()  ## remember current dir
    if content:
        slot = []
        for f in content:
            slot.append(Editor(tab_size, undo, io_device))
            if type(f) == str and f:  ## String = non-empty Filename
                try:
                    slot[index].get_file(f)
                except Exception as err:
                    slot[index].message = "{!r}".format(err)
            else:
                try:
                    slot[index].content = [
                        str(_) for _ in f
                    ]  ## iterable item -> make strings and edit
                except:
                    slot[index].content = [str(f)]
            index += 1
    else:
        slot = [Editor(tab_size, undo, io_device)]
        slot[0].get_file(current_dir)
    ## edit
    while True:
        try:
            index %= len(slot)
            key = slot[index].edit_loop()  ## edit buffer
            if key == KEY_QUIT:
                if len(slot) == 1:  ## the last man standing is kept
                    break
                del slot[index]
            elif key == KEY_GET:
                f = slot[index].line_edit("Open file: ", "", Editor.file_char)
                if f is not None:
                    slot.append(Editor(tab_size, undo, io_device))
                    index = len(slot) - 1
                    slot[index].get_file(f)
            elif key == KEY_NEXT:
                index += 1
            elif key == KEY_PREV:
                index -= 1
            elif key == KEY_FORCE_QUIT:
                break
            elif key in slot:
                index = slot.index(key)

        except Exception as err:
            slot[index].message = "{!r}".format(err)
            ## raise  ## remove the comment to trace bugs
    ## All windows closed, clean up
    Editor.yank_buffer = []
    ## close
    os.chdir(current_dir)  ## restore dir
    return slot[0].content if (slot[0].fname == "") else slot[0].fname
