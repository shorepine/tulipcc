# wordpad.py
# use lvgl to make a full screen editor with proportional fonts and word wrap
import lvgl as lv
import tulip, time
(H_RES,V_RES) = tulip.screen_size()



def run(screen):
    screen.set_bg_color(255)
    ta = lv.textarea(screen.group)
    ta.set_pos(0,0)
    ta.set_size(H_RES, V_RES)
    ta.set_style_text_font(lv.font_montserrat_24, 0)
    ta.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.MAIN)
    ta.set_style_text_color(tulip.pal_to_lv(0),0)
    ta.set_style_border_color(tulip.pal_to_lv(0), lv.PART.CURSOR | lv.STATE.FOCUSED)
    ta.set_placeholder_text("Type away...")
    tulip.lv_kb_group.add_obj(ta)
    lv.group_focus_obj(ta)
    screen.present()





