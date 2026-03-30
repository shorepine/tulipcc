#
# Front-end for Micropython standard console IO
#
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
        char = self.rd()  ## expect ESC[yyy;xxxR
        while char != "R":
            pos += char
            char = self.rd()
        return [int(i, 10) for i in pos.lstrip("\n\x1b[").split(";")]


## test, if the Editor class is already present
if "pye_edit" not in globals().keys():
    from pye import pye_edit


def pye(*args, tab_size=4, undo=50):
    io_device = IO_DEVICE()
    ret = pye_edit(args, tab_size=tab_size, undo=undo, io_device=io_device)
    io_device.deinit_tty()
    return ret
