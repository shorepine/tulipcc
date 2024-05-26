# Timer implementation for the JS port.
#
# Idea by X-Ryl669: https://github.com/lvgl/lvgl/issues/4011#issuecomment-1483094051

import jswindow
class Timer:
    PERIODIC = 0
    ONE_SHOT = 1

    def __init__(self, id):
        pass

    def init(self, mode=PERIODIC, period=-1, callback=None):
        self.mode = mode
        if mode == Timer.PERIODIC:
           self.id = jswindow.setInterval(callback, period)
        else:
           self.id = jswindow.setTimeout(callback, period)

    def deinit(self):
        if self.mode == Timer.PERIODIC:
           jswindow.clearInterval(self.id)
        else:
           jswindow.clearTimeout(self.id)
