# A simple test for Linux Frame Buffer
# Registers FB display and mouse evdev and shows line cursor
# then shows a button on screen.

import lvgl as lv
from lv_utils import event_loop
import evdev

lv.init()

# Register FB display driver

event_loop = event_loop()
disp = lv.linux_fbdev_create()
lv.linux_fbdev_set_file(disp, "/dev/fb0")

# Register mouse device and crosshair cursor

mouse = evdev.mouse_indev(lv.screen_active())

# Create a screen and a button

btn = lv.button(lv.screen_active())
btn.align(lv.ALIGN.CENTER, 0, 0)
label = lv.label(btn)
label.set_text("Hello World!")
