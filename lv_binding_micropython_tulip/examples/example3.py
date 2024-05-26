import usys as sys
sys.path.append('') # See: https://github.com/micropython/micropython/issues/6419

import lv_utils
import lvgl as lv

try:
    import pyb
    import rk043fn48h as lcd

    hres = 480
    vres = 272
    color_format = lv.COLOR_FORMAT.ARGB8888

    lv.init()
    event_loop = lv_utils.event_loop()
    lcd.init(w=hres, h=vres)

    buf1 = lcd.framebuffer(1)
    buf2 = lcd.framebuffer(2)
    
    disp_drv = lv.display_create(hres, vres)
    disp_drv.set_flush_cb(lcd.flush)
    disp_drv.set_color_format(color_format)
    disp_drv.set_buffers(buf1, buf2, len(buf1), lv.DISPLAY_RENDER_MODE.PARTIAL)

    # disp_drv.gpu_blend_cb = lcd.gpu_blend
    # disp_drv.gpu_fill_cb = lcd.gpu_fill

    indev_drv = lv.indev_create()
    indev_drv.set_type(lv.INDEV_TYPE.POINTER)
    indev_drv.set_read_cb(lcd.ts_read)

except ImportError:
    import display_driver

scr1 = lv.obj()
scr2 = lv.obj()
lv.screen_load(scr1)

slider = lv.slider(scr2)
slider.set_width(150)
slider.align(lv.ALIGN.TOP_MID, 0, 15)

button1 = lv.button(scr1)
button1.align(lv.ALIGN.TOP_RIGHT, -5, 5)
label = lv.label(button1)
label.set_text(">")

button2 = lv.button(scr2)
button2.align(lv.ALIGN.TOP_LEFT, 5, 5)
label2 = lv.label(button2)
label2.set_text("<")

led1 = lv.led(scr2)
led1.align(lv.ALIGN.CENTER, 0, 0)
led1.set_brightness(slider.get_value() * 2)
# led1.set_drag(True)
led1.set_size(20,20)


def slider_event_cb(event):
    led1.set_brightness(slider.get_value() * 2)

def button1_event_cb(event):
    lv.screen_load(scr2)


def button2_event_cb(event):
    lv.screen_load(scr1)

slider.add_event_cb(slider_event_cb, lv.EVENT.VALUE_CHANGED, None)
button1.add_event_cb(button1_event_cb, lv.EVENT.CLICKED, None)
button2.add_event_cb(button2_event_cb, lv.EVENT.CLICKED, None)

# Create a keyboard
kb = lv.keyboard(scr1)
# kb.set_cursor_manage(True)

# Create a text area. The keyboard will write here
ta = lv.textarea(scr1)
ta.set_width(450)
ta.set_height(70)
ta.align(lv.ALIGN.CENTER, 0, 0)
ta.set_text("")

# Assign the text area to the keyboard
kb.set_textarea(ta)

# Create a Spinner object
spin = lv.spinner(scr2)
spin.set_anim_params(1000, 100)
spin.set_size(100, 100)
spin.align(lv.ALIGN.CENTER, 0, 0)
# spin.set_type(lv.spinner.TYPE.FILLSPIN_ARC)
