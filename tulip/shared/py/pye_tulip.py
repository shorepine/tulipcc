# pye_tulip.py
# Run the pye editor on Tulip's text framebuffer (TFB).
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


def pye(*args, tab_size=4, undo=50):
    # 7,8: mouse reporting on/off. 9,10: scroll one line up/down.
    # 11,12: set/reset scrolling region.
    for i in (7, 8, 9, 10, 11, 12):
        _pye.Editor.TERMCMD[i] = ""
    # Tulip's keyboard sends 0x08 for the backspace key; stock pye expects
    # 0x7f and treats 0x08 (ctrl-H) as find-replace, which is still on ctrl-R.
    _pye.Editor.KEYMAP["\x08"] = _pye.KEY_BACKSPACE
    io_device = TFB_IO()
    try:
        ret = _pye.pye_edit(args, tab_size=tab_size, undo=undo, io_device=io_device)
    finally:
        io_device.deinit_tty()
        # make sure the cursor is back for the REPL
        io_device.wr("\x1b[?25h")
    return ret
