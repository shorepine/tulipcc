# wordpad.py
# use lvgl to make a full screen editor with proportional fonts and word wrap
import lvgl as lv
import tulip, time
(H_RES,V_RES) = tulip.screen_size()

tulip.tfb_stop()

#try:
wordpad = lv.obj()
wordpad.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.MAIN)
ta = lv.textarea(wordpad)
ta.set_pos(0,0)
ta.set_size(H_RES, V_RES)
ta.set_style_text_font(lv.font_montserrat_24, 0)
ta.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.MAIN)
ta.set_style_text_color(tulip.pal_to_lv(0),0)
ta.set_style_border_color(tulip.pal_to_lv(0), lv.PART.CURSOR | lv.STATE.FOCUSED)
ta.set_placeholder_text("Type away...")
tulip.lv_kb_group.add_obj(ta)
lv.screen_load(wordpad)
lv.group_focus_obj(ta)


def button_callback(e):
    # quit
    lv.screen_load(tulip.lv_scr)
    ta.delete()
    wordpad.delete()
    tulip.tfb_start()

button = lv.button(wordpad)
button.set_pos(900,540)
label = lv.label(button)
label.set_text("Close")
label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
button.add_event_cb(button_callback, lv.EVENT.CLICKED, None)

    #while True:
    #    time.sleep(0.01)

#except KeyboardInterrupt:
#    lv.screen_load(tulip.lv_scr)
#    ta.delete()
#    wordpad.delete()
#    tulip.tfb_start()


