import lvgl as lv
from ili9XXX import *

# ili9341 example
disp = ili9341(
    mosi=18, clk=19, cs=13, dc=12, rst=4, backlight=15, backlight_on=1,
    width=128, height=160, colormode=COLOR_MODE_RGB, invert=False, rot=0)

# st7789 example
# disp = st7789(
#   mosi=19, clk=18, cs=5, dc=16, rst=23, backlight=4, backlight_on=1,
#   width=240, height=320, colormode=COLOR_MODE_RGB, invert=False, rot=0)

style = lv.style_t()
style.init()
style.set_bg_color(lv.palette_main(lv.PALETTE.RED))

btn = lv.button(lv.scr_act())
btn.set_size(disp.width, disp.height)
btn.align(lv.ALIGN.CENTER,0,0)
btn.add_style(style, 0)

label = lv.label(btn)
label.set_text("F")
label.center()