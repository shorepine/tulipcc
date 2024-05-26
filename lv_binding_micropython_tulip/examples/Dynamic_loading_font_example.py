#
# Command line for running this example on the unix port from examples directory:
# MICROPYPATH=./:../lib ../../../ports/unix/micropython -i Dynamic_loading_font_example.py
#

import usys as sys
sys.path.append('') # See: https://github.com/micropython/micropython/issues/6419

try:
    script_path = __file__[:__file__.rfind('/')] if __file__.find('/') >= 0 else '.'
except NameError:
    script_path = ''

import lvgl as lv
import fs_driver

lv.init()

# display driver init...
import display_driver_utils # Default implementation. Replace by your driver
driver = display_driver_utils.driver()

# FS driver init.

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'S')

'''
　load the font file from filesystem(For me is flash )
　How to convert font files refer here: https://github.com/lvgl/lv_font_conv
　font-PHT-en-20.bin:
　　　lv_font_conv --size 20 --format bin --bpp 1 --font Alibaba-PuHuiTi-Medium.subset.ttf --range 0x20-0x7f --no-compress -o font-PHT-en-20.bin
　font-PHT-cn-20.bin:
　　　lv_font_conv --size 20 --format bin --bpp 1 --font Alibaba-PuHuiTi-Medium.subset.ttf --range 0x4e00-0x4e56　--no-compress　-o font-PHT-cn-20.bin
　font-PHT-jp-20.bin:
　　　lv_font_conv --size 20 --format bin --bpp 1 --font Alibaba-PuHuiTi-Medium.subset.ttf --range 0x3042-0x3093　--no-compress　-o font-PHT-jp-20.bin
'''
scr = lv.screen_active()
scr.clean()

myfont_cn = lv.binfont_create("S:%s/font/font-PHT-cn-20.bin" % script_path)

label1 = lv.label(scr)
label1.set_style_text_font(myfont_cn, 0)  # set the font
label1.set_text("上中下乎")  
label1.align(lv.ALIGN.CENTER, 0, -25)

myfont_en = lv.binfont_create("S:%s/font/font-PHT-en-20.bin" % script_path)

label2 = lv.label(scr)
label2.set_style_text_font(myfont_en, 0)  # set the font
label2.set_text("Hello LVGL!")
label2.align(lv.ALIGN.CENTER, 0, 25)


myfont_jp= lv.binfont_create("S:%s/font/font-PHT-jp-20.bin" % script_path)

label3 = lv.label(scr)
label3.set_style_text_font(myfont_jp, 0)  # set the font
label3.set_text("こんにちはありがとう")
label3.align(lv.ALIGN.CENTER, 0, 0)





