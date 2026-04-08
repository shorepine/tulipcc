PYE_VERSION = " V2.79 "
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
    KEYMAP = {
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
        "\x1b[H": KEY_HOME,
        "\x1bOH": KEY_HOME,
        "\x1b[1~": KEY_HOME,
        "\x1b[F": KEY_END,
        "\x1bOF": KEY_END,
        "\x1b[4~": KEY_END,
        "\x1b[5~": KEY_PGUP,
        "\x1b[6~": KEY_PGDN,
        "\x1b[5;5~": KEY_PREV,
        "\x1b[6;5~": KEY_NEXT,
        "\x1b[1;5D": KEY_WORD_LEFT,
        "\x1b[1;5C": KEY_WORD_RIGHT,
        "\x03": KEY_COPY,
        "\r": KEY_ENTER,
        "\x7f": KEY_BACKSPACE,
        "\x1b[3~": KEY_DELETE,
        "\x1b[Z": KEY_BACKTAB,
        "\x19": KEY_REDO,
        "\x08": KEY_REPLC,
        "\x12": KEY_REPLC,
        "\x11": KEY_QUIT,
        "\x1b": KEY_QUIT,
        "\n": KEY_ENTER,
        "\x13": KEY_WRITE,
        "\x06": KEY_FIND,
        "\x0e": KEY_FIND_AGAIN,
        "\x07": KEY_GOTO,
        "\x05": KEY_REDRAW,
        "\x1a": KEY_UNDO,
        "\x09": KEY_TAB,
        "\x15": KEY_BACKTAB,
        "\x18": KEY_CUT,
        "\x16": KEY_PASTE,
        "\x04": KEY_UNDO_YANK,
        "\x0c": KEY_MARK,
        "\x00": KEY_MARK,
        "\x14": KEY_FIRST,
        "\x02": KEY_LAST,
        "\x01": KEY_TOGGLE,
        "\x17": KEY_NEXT,
        "\x0f": KEY_GET,
        "\x10": KEY_COMMENT,
        "\x1f": KEY_COMMENT,
        "\x1b[1;5A": KEY_SCRLUP,
        "\x1b[1;5B": KEY_SCRLDN,
        "\x1b[1;5H": KEY_FIRST,
        "\x1b[1;5F": KEY_LAST,
        "\x1b[3;5~": KEY_DEL_WORD,
        "\x1b[3;2~": KEY_DEL_LINE,
        "\x0b": KEY_MATCH,
        "\x1b[M": KEY_MOUSE,
        "\x1b[2;3~": KEY_PLACE,
        "\x1b[5;3~": KEY_PREV_PLACE,
        "\x1b[6;3~": KEY_NEXT_PLACE,
        "\x1b[1;3H": KEY_UNDO_PREV,
        "\x1b[1;3F": KEY_UNDO_NEXT,
    }
    TERMCMD = [
        "\x1b[{row};{col}H",
        "\x1b[0K",
        "\x1b[?25h",
        "\x1b[?25l",
        "\x1b[0m",
        "\x1b[1;37;44m",
        "\x1b[43m",
        "\x1b[?9h",
        "\x1b[?9l",
        "\x1bM",
        "\n",
        "\x1b[1;{stop}r",
        "\x1b[r",
        "\b",
        "{chd}{file} Row: {row}/{total} Col: {col}  {msg}",
        "{chd}{file} {row}:{col}  {msg}",
    ]
    yank_buffer = []
    find_pattern = ""
    case = "n"
    autoindent = "y"
    replc_pattern = ""
    comment_char = "\x23 "
    word_char = "_\\"
    file_char = "_.-"
    match_span = 50
    place_list = []
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
        if mode == 1:
            self.wr(Editor.TERMCMD[5])
        elif mode == 2:
            self.wr(Editor.TERMCMD[6])
        else:
            self.wr(Editor.TERMCMD[4])
    def mouse_reporting(self, onoff):
        self.wr(
            Editor.TERMCMD[7] if onoff else Editor.TERMCMD[8]
        )
    def scroll_region(self, stop):
        self.wr(
            Editor.TERMCMD[11].format(stop=stop) if stop else Editor.TERMCMD[12]
        )
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
        Editor.scrbuf = [(False, "\x00")] * Editor.height
        self.row = min(Editor.height - 1, self.row)
        self.scroll_region(Editor.height)
        self.mouse_reporting(True)
        if flag:
            self.message = PYE_VERSION
        if is_micropython:
            gc.collect()
            if flag:
                self.message += "{} Bytes Memory available".format(gc.mem_free())
        self.changed = "" if self.hash == self.hash_buffer() else "*"
    def get_input(
        self,
    ):
        while True:
            in_buffer = self.io_device.rd()
            if in_buffer[0] == "\x1b":
                while True:
                    in_buffer += self.io_device.rd()
                    c = in_buffer[-1]
                    if c == "~" or (c.isalpha() and len(in_buffer) > 2):
                        break
                    if len(in_buffer) == 2 and c.isalpha() and c != "O":
                        in_buffer = chr(ord(in_buffer[1]) & 0x1F)
                        break
                    if len(in_buffer) >= self.key_max:
                        break
                    if in_buffer == "\x1b\x1b":
                        in_buffer = "\x1b"
                        break
            if in_buffer in Editor.KEYMAP:
                c = Editor.KEYMAP[in_buffer]
                if c != KEY_MOUSE:
                    return c, None
                else:
                    mouse_fct = ord(self.io_device.rd_raw())
                    mouse_x = ord(self.io_device.rd_raw()) - 33
                    mouse_y = ord(self.io_device.rd_raw()) - 33
                    if mouse_fct == 0x61:
                        return KEY_SCRLDN, 3
                    elif mouse_fct == 0x60:
                        return KEY_SCRLUP, 3
                    else:
                        return KEY_MOUSE, [mouse_x, mouse_y, mouse_fct]
            elif ord(in_buffer[0]) >= 32:
                return KEY_NONE, in_buffer
    def display_window(self):
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
                if (flag and line == self.cur_line) or l != Editor.scrbuf[
                    c
                ]:
                    self.goto(c, 0)
                    if flag == 0:
                        self.wr(l[1])
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
    def spaces(self, line, pos=None):
        return (
            len(line) - len(line.lstrip(" "))
            if pos is None
            else len(line[:pos]) - len(line[:pos].rstrip(" "))
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
    ):
        def push_msg(msg):
            self.wr(msg + Editor.TERMCMD[13] * len(msg))
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
            key, char = self.get_input()
            if key == KEY_NONE:
                if len(prompt) + len(res) < self.width - 2:
                    res = res[:pos] + char + res[pos:]
                    self.wr(res[pos])
                    pos += len(char)
                    push_msg(res[pos:])
            elif key in (KEY_ENTER, KEY_TAB):
                self.hilite(0)
                return res
            elif key in (KEY_QUIT, KEY_COPY):
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
            elif key == KEY_DELETE:
                if del_all:
                    self.wr(Editor.TERMCMD[13] * pos)
                    self.wr(" " * len(res))
                    self.wr(Editor.TERMCMD[13] * len(res))
                    pos = 0
                    res = ""
                else:
                    if pos < len(res):
                        res = res[:pos] + res[pos + 1 :]
                        push_msg(res[pos:] + " ")
            elif key == KEY_BACKSPACE:
                if pos > 0:
                    res = res[: pos - 1] + res[pos:]
                    self.wr(Editor.TERMCMD[13])
                    pos -= 1
                    push_msg(res[pos:] + " ")
            elif key == KEY_PASTE:
                res += self.getsymbol(self.content[self.cur_line], self.col, zap)[
                    : Editor.width - pos - len(prompt) - 1
                ]
                push_msg(res[pos:])
            elif key == KEY_MOUSE:
                if char[1] < Editor.height and (char[1] + self.top_line) < self.total_lines:
                    self.col = char[0] + self.margin
                    self.cur_line = char[1] + self.top_line
                    if (self.col, self.cur_line) != mouse_last:
                        mouse_last = (self.col, self.cur_line)
                        self.wr(Editor.TERMCMD[13] * pos)
                        self.wr(" " * len(res))
                        self.wr(Editor.TERMCMD[13] * len(res))
                        pos = 0
                        res = self.getsymbol(self.content[self.cur_line], self.col, zap)
                        push_msg(res)
                    else:
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
    def find_in_file(self, pattern, col, end):
        Editor.find_pattern = pattern
        if Editor.case != "y":
            pattern = pattern.lower()
        try:
            rex = re_compile(pattern)
        except:
            self.message = "Invalid pattern: " + pattern
            return None
        start = self.cur_line
        if col > len(self.content[start]) or (
            pattern[0] == "^" and col != 0
        ):
            start, col = start + 1, 0
        for line in range(start, end):
            l = self.content[line][col:]
            if Editor.case != "y":
                l = l.lower()
            match = rex.search(l)
            if match:
                self.cur_line = line
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
            if len(self.undo) >= self.undo_limit:
                del self.undo[0]
            self.undo.append([lnum, span, text, key, self.col, chain])
            self.redo = []
    def undo_redo(self, undo, redo):
        chain = True
        redo_start = len(redo)
        while len(undo) > 0 and chain:
            action = undo.pop()
            if action[3] not in (KEY_INDENT, KEY_DEDENT, KEY_COMMENT):
                self.cur_line = action[0]
            self.col = action[4]
            if len(redo) >= self.undo_limit:
                del redo[0]
            if action[1] >= 0:
                redo.append(
                    action[0:1]
                    + [len(action[2])]
                    + [self.content[action[0] : action[0] + action[1]]]
                    + action[3:]
                )
                if action[0] < self.total_lines:
                    self.content[action[0] : action[0] + action[1]] = action[2]
                else:
                    self.content += action[2]
            else:
                redo.append(
                    action[0:1]
                    + [1]
                    + [
                        self.content[action[0] : action[0] - action[1] + 1]
                    ]
                    + action[3:]
                )
                del self.content[action[0] : action[0] - action[1]]
                self.content[action[0]] = action[2][0]
            chain = action[5]
        if (len(redo) - redo_start) > 0:
            redo[-1][5] = True
            redo[redo_start][5] = False
            self.total_lines = len(self.content)
            self.changed = "" if self.hash == self.hash_buffer() else "*"
            self.clear_mark()
    def set_mark(self, flag=999999999):
        if self.mark is None:
            self.mark = (self.cur_line, self.col)
        if self.mark_flag < flag:
            self.mark_flag = flag
    def check_mark(self):
        if self.mark is not None:
            self.mark_flag -= 1
            if self.mark_flag <= 0:
                self.clear_mark()
    def clear_mark(self):
        self.mark = None
        self.mark_flag = 0
        self.mouse_last = (0, 0, 0)
    def yank_mark(self):
        start_row, start_col, end_row, end_col = self.mark_range()
        Editor.yank_buffer = self.content[start_row:end_row]
        Editor.yank_buffer[-1] = Editor.yank_buffer[-1][:end_col]
        Editor.yank_buffer[0] = Editor.yank_buffer[0][start_col:]
    def delete_mark(self, yank):
        if yank:
            self.yank_mark()
        start_row, start_col, end_row, end_col = self.mark_range()
        self.undo_add(start_row, self.content[start_row:end_row], KEY_NONE, 1, False)
        self.content[start_row] = (
            self.content[start_row][:start_col] + self.content[end_row - 1][end_col:]
        )
        if start_row + 1 < end_row:
            del self.content[start_row + 1 : end_row]
        self.col = start_col
        if self.content == []:
            self.content = [""]
            self.undo[-1][1] = 1
        self.total_lines = len(self.content)
        self.cur_line = start_row
        self.clear_mark()
    def handle_edit_keys(self, key, char):
        l = self.content[self.cur_line]
        if key == KEY_NONE:
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
            return key
        elif key == KEY_SHIFT_CTRL_LEFT:
            self.set_mark()
            key = KEY_WORD_LEFT
        elif key == KEY_SHIFT_CTRL_RIGHT:
            self.set_mark()
            key = KEY_WORD_RIGHT
        elif key == KEY_MOUSE:
            if char[2] == 0x22:
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
                    else:
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
            elif (self.cur_line + 1) < self.total_lines:
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
            elif self.cur_line > 0:
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
        elif key == KEY_GOTO:
            line = self.line_edit("Goto Line: ", "")
            if line:
                self.cur_line = int(line) - 1
                self.row = Editor.height >> 1
        elif key == KEY_FIRST:
            self.check_mark()
            self.cur_line = 0
        elif key == KEY_LAST:
            self.check_mark()
            self.cur_line = self.total_lines - 1
            self.row = Editor.height - 1
        elif key == KEY_TOGGLE:
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
        elif key == KEY_SCRLUP:
            ni = 1 if char is None else 3
            if self.top_line > 0:
                self.top_line = max(self.top_line - ni, 0)
                self.cur_line = min(self.cur_line, self.top_line + Editor.height - 1)
                self.scroll_up(ni)
        elif key == KEY_SCRLDN:
            ni = 1 if char is None else 3
            if self.top_line + Editor.height < self.total_lines:
                self.top_line = min(self.top_line + ni, self.total_lines - 1)
                self.cur_line = max(self.cur_line, self.top_line)
                self.scroll_down(ni)
        elif key == KEY_MATCH:
            if self.col < len(l):
                brackets = "<{[()]}>"
                srch = l[self.col]
                i = brackets.find(srch)
                if i >= 0:
                    match = brackets[7 - i]
                    level = 0
                    way = 1 if i < 4 else -1
                    i = self.cur_line
                    c = self.col + way
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
                                    if level == 0:
                                        self.cur_line, self.col = i, c
                                        return key
                                    else:
                                        level -= 1
                                elif l[c] == srch:
                                    level += 1
                                c += way
                        i += way
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
            if Editor.autoindent == "y":
                ni = min(self.spaces(l), self.col)
            self.cur_line += 1
            self.content[self.cur_line : self.cur_line] = [" " * ni + l[self.col :]]
            self.total_lines += 1
            self.col = ni
        elif key == KEY_TAB:
            if self.mark is None:
                self.col = self.vcol
                self.undo_add(self.cur_line, [l], KEY_TAB)
                ni = self.tab_size - self.col % self.tab_size
                self.content[self.cur_line] = l[: self.col] + " " * ni + l[self.col :]
                self.col += ni
            else:
                lrange = self.line_range()
                self.undo_add(
                    lrange[0],
                    self.content[lrange[0] : lrange[1]],
                    KEY_INDENT,
                    lrange[1] - lrange[0],
                )
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
                )
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
                )
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
                if rpat is not None:
                    Editor.replc_pattern = rpat
                    q = ""
                    cur_line, cur_col = self.cur_line, self.col
                    if self.mark is not None:
                        (self.cur_line, self.col, end_line, end_col) = self.mark_range()
                    else:
                        end_line = self.total_lines
                        end_col = 999999
                    self.message = "Replace (yes/No/all/quit) ? "
                    chain = False
                    while True:
                        ni = self.find_in_file(pat, self.col, end_line)
                        if ni is not None and (
                            self.cur_line != (end_line - 1) or self.col < end_col
                        ):
                            if q != "a":
                                self.display_window()
                                key, char = self.get_input()
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
                                self.col += len(rpat) + (ni == 0)
                                count += 1
                                chain = True
                            else:
                                self.col += 1
                        else:
                            break
                    self.cur_line, self.col = cur_line, cur_col
                    self.message = "'{}' replaced {} times".format(pat, count)
        elif key == KEY_CUT:
            if self.mark is None:
                if self.cur_line < self.total_lines - 1:
                    self.mark = (self.cur_line + 1, 0)
                else:
                    self.mark = (self.cur_line, len(l))
                self.col = 0
            self.delete_mark(True)
        elif key == KEY_COPY:
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
        elif key == KEY_PASTE:
            if Editor.yank_buffer:
                self.col = self.vcol
                if self.mark is not None:
                    self.delete_mark(False)
                    chain = True
                else:
                    chain = False
                head, tail = Editor.yank_buffer[0], Editor.yank_buffer[-1]
                Editor.yank_buffer[0] = (
                    self.content[self.cur_line][: self.col] + Editor.yank_buffer[0]
                )
                Editor.yank_buffer[-1] += self.content[self.cur_line][self.col :]
                ni = 1 if len(Editor.yank_buffer) <= 1 else 1 - len(Editor.yank_buffer)
                self.undo_add(
                    self.cur_line, [self.content[self.cur_line]], KEY_NONE, ni, chain
                )
                self.content[
                    self.cur_line : self.cur_line + 1
                ] = Editor.yank_buffer
                Editor.yank_buffer[-1], Editor.yank_buffer[0] = tail, head
                self.total_lines = len(self.content)
        elif key == KEY_WRITE:
            fname = self.line_edit(
                "Save File: ", self.fname if self.is_dir is False else "", Editor.file_char
            )
            if fname:
                if fname != self.fname:
                    try:
                        open(fname).close()
                        res = self.line_edit("The file exists! Overwrite (y/N)? ", "N")
                        if not res or res[0].upper() != "Y":
                            return
                    except:
                        pass
                self.put_file(fname)
                self.fname = fname
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
            )
            ni = len(Editor.comment_char)
            for i in range(lrange[0], lrange[1]):
                if self.content[i].strip() != "":
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
    def edit_loop(self):
        if not self.content:
            self.content = [""]
        self.total_lines = len(self.content)
        os.chdir(self.work_dir)
        self.redraw(self.message == "")
        while True:
            self.display_window()
            key, char = self.get_input()
            self.message = ""
            key = self.handle_edit_keys(key, char)
            if key == KEY_QUIT:
                if self.hash != self.hash_buffer():
                    res = self.line_edit("File changed! Quit (y/N/f)? ", "N")
                    if not res or res[0].upper() == "N":
                        continue
                    if res[0].upper() == "F":
                        key = KEY_FORCE_QUIT
                Editor.place_list = [item for item in Editor.place_list if item[1] != self]
                Editor.place_index = 0
                self.scroll_region(0)
                self.mouse_reporting(False)
                self.goto(Editor.height, 0)
                self.clear_to_eol()
                self.undo = []
                return key
            elif key == KEY_NEXT or key == KEY_PREV or type(key) is Editor:
                return key
            elif key == KEY_GET:
                if self.mark is not None:
                    self.clear_mark()
                    self.display_window()
                return key
    def packtabs(self, s):
        sb = StringIO()
        for i in range(0, len(s), 8):
            c = s[i : i + 8]
            cr = c.rstrip(" ")
            if (len(c) - len(cr)) > 1:
                sb.write(cr + "\t")
            else:
                sb.write(c)
        return sb.getvalue()
    def hash_buffer(self):
        res = 0
        for line in self.content:
            res = ((res * 227 + 1) ^ hash(line)) & 0x3FFFFFFF
        return res
    def get_file(self, fname):
        if fname:
            try:
                self.fname = fname
                if fname in (".", "..") or (os.stat(fname)[0] & 0x4000):
                    os.chdir(fname)
                    self.work_dir = os.getcwd()
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
    def expandtabs(self, s):
        if "\t" in s:
            self.write_tabs = True
            sb = StringIO()
            pos = 0
            for c in s:
                if c == "\t":
                    sb.write(" " * (8 - pos % 8))
                    pos += 8 - pos % 8
                else:
                    sb.write(c)
                    pos += 1
            return sb.getvalue()
        else:
            return s
def pye_edit(content, tab_size=4, undo=50, io_device=None):
    if io_device is None:
        print("IO device not defined")
        return
    gc.collect()
    index = 0
    undo = max(4, (undo if type(undo) is int else 0))
    current_dir = os.getcwd()
    if content:
        slot = []
        for f in content:
            slot.append(Editor(tab_size, undo, io_device))
            if type(f) == str and f:
                try:
                    slot[index].get_file(f)
                except Exception as err:
                    slot[index].message = "{!r}".format(err)
            else:
                try:
                    slot[index].content = [
                        str(_) for _ in f
                    ]
                except:
                    slot[index].content = [str(f)]
            index += 1
    else:
        slot = [Editor(tab_size, undo, io_device)]
        slot[0].get_file(current_dir)
    while True:
        try:
            index %= len(slot)
            key = slot[index].edit_loop()
            if key == KEY_QUIT:
                if len(slot) == 1:
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
    Editor.yank_buffer = []
    os.chdir(current_dir)
    return slot[0].content if (slot[0].fname == "") else slot[0].fname
try:
    import usys as sys
except:
    import sys
class IO_DEVICE:
    def __init__(self):
        try:
            from micropython import kbd_intr
            kbd_intr(-1)
        except ImportError:
            pass
        if hasattr(sys.stdin, "buffer"):
            self.rd_raw_fct = sys.stdin.buffer.read
        else:
            self.rd_raw_fct = sys.stdin.read
    def wr(self, s):
        sys.stdout.write(s)
    def rd(self):
        return sys.stdin.read(1)
    def rd_raw(self):
        return self.rd_raw_fct(1)
    def deinit_tty(self):
        try:
            from micropython import kbd_intr
            kbd_intr(3)
        except ImportError:
            pass
    def get_screen_size(self):
        self.wr("\x1b[999;999H\x1b[6n")
        pos = ""
        char = self.rd()
        while char != "R":
            pos += char
            char = self.rd()
        return [int(i, 10) for i in pos.lstrip("\n\x1b[").split(";")]
if "pye_edit" not in globals().keys():
    from pye import pye_edit
def pye(*args, tab_size=4, undo=50):
    io_device = IO_DEVICE()
    ret = pye_edit(args, tab_size=tab_size, undo=undo, io_device=io_device)
    io_device.deinit_tty()
    return ret
