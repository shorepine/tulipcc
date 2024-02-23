# ui.py
# imports into tulip for ui_x translation into lvgl
# also has keyboard and other small LVGL things
import tulip
import lvgl as lv

lv_scr = lv.screen_active() # gets the currently active screen
# gets the group that receives keyboard events. 
# you have to add objs to this group for them to receive kbd
lv_kb_group = lv.group_by_index(0) 
lv_soft_kb = None
lv_default_font = lv.font_montserrat_14

# Can be overriden
tulip.ui_callback = None

# Callback for soft keyboard to send chars to Tulip.
def lv_soft_kb_cb(e):
    global lv_soft_kb, lv_last_mode
    kb = e.get_target_obj()
    button = kb.get_selected_button()
    text = kb.get_button_text(button)
    code = text[0]
    #print("button %d text %s code %d" % (button, text, ord(code)))

    if(code==lv.SYMBOL.NEW_LINE): 
        key_send(13)
    elif(code==lv.SYMBOL.BACKSPACE): 
        if(lv_last_mode == kb.get_mode()): # there's a bug where the mode swticher sends BS
            key_send(8)
    elif(code==lv.SYMBOL.KEYBOARD):
        lv_soft_kb.delete()
        lv_soft_kb = None
        return
    elif(ord(code)==49): # special
        if(kb.get_mode() == lv_last_mode):  # only update after switching modes
            key_send(49)
    elif(len(text)==1 and ord(code)>31 and ord(code)<127): 
        key_send(ord(code))

    lv_last_mode = kb.get_mode()

# Starts or stops the soft keyboard
def keyboard():
    global lv_soft_kb, lv_last_mode
    if(lv_soft_kb is not None):
        lv_soft_kb.delete()
        lv_soft_kb = None
        return
    lv_soft_kb = lv.keyboard(lv_scr)
    lv_soft_kb.add_event_cb(lv_soft_kb_cb, lv.EVENT.VALUE_CHANGED, None)
    lv_last_mode = lv_soft_kb.get_mode()

def ui_msgbox(buttons=['OK', 'Cancel'], title='Title', message='Message box'):
    mbox = lv.msgbox(tulip.lv_scr)
    mbox.add_text(message)
    mbox.add_title(title)
    mbox.add_close_button()
    for b in buttons:
        mbox.add_footer_button(b)

# Convert tulip rgb332 pal idx into lv color
def pal_to_lv(pal):
    (r,g,b) = tulip.rgbw(pal) # todo -- not sure if we use wide or not
    return lv.color_make(r,g,b)

# removes all ui elements
def ui_clear():
    # remove all the children of scr
    while(lv_scr.get_child_count()):
        lv_scr.get_child(0).delete()

# Return how many LVGL objs you've created
def ui_count():
    return lv_scr.get_child_count()

# Callback that you can have LVGL objects register. It gets the event (which you can get the object from) and "extra" which we put ui_id in for legacy
def lv_callback(e, extra):
    code = e.get_code()
    obj = e.get_target_obj()
    if(tulip.ui_callback is not None):
        tulip.ui_callback(obj, code, extra)

def ui_rect(x,y,w,h,fg_color,filled):
    rect = lv.obj(tulip.lv_scr)
    rect.set_size(w,h)
    rect.set_pos(x,y)
    rect.set_style_bg_color(tulip.pal_to_lv(fg_color), 0)
    return rect


#tulip.ui_slider(ui_element_id, default_value, x, y, w, h, bar_color, handle_color)
# Gets a slider val
#val = tulip.ui_slider(ui_element_id)
# Set a slider val
#tulip.ui_slider(ui_element_id, val)
def ui_slider(val=.5, x=0, y=0, w=400, h=50, bar_color=40, handle_color=255, ui_id=0):
    slider = lv.slider(tulip.lv_scr)
    slider.set_pos(x,y)
    slider.set_size(w,h)
    slider.set_style_bg_color(tulip.pal_to_lv(handle_color), lv.PART.INDICATOR | lv.STATE.DEFAULT)
    slider.set_style_bg_color(tulip.pal_to_lv(handle_color), lv.PART.KNOB)
    slider.set_value(int(val*100.0),lv.ANIM.OFF)
    slider.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return slider

#tulip.ui_button(ui_element_id, "Button text", x, y, w, h, bg_pal_idx, fg_pal_idx, filled, font_number)
def ui_button(text="Hello", x=0, y=0, w=None, h=None, bg_color=None, fg_color=None, font=lv_default_font, ui_id=0):
    button = lv.button(lv_scr)
    button.set_x(x)
    button.set_y(y)
    if(w is not None):
        button.set_width(w)
    if(h is not None):
        button.set_height(h)
    label = lv.label(button)
    label.set_text(text)
    button.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.CLICKED, None)
    return button

# tulip.bg_str(string, x, y, pal_idx, font_number) # same as char, but with a string. x and y are the bottom left
# tulip.bg_str(string, x, y, pal_idx, font_number, w, h) # Will center the text inside w,h
def ui_label(s, x, y, pal_idx, w=0, font=lv.font_montserrat_10):
    label = lv.label(lv_scr)
    label.set_pos(x,y)
    if(w != 0):
        label.set_width(w)
    label.set_text(s)
    label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
    label.set_style_text_font(font, 0)
    label.set_style_text_color(tulip.pal_to_lv(pal_idx),0)
    return label

# This text entry box UI element is limited to 32 characters. It will wait for you to hit return to finish input
#tulip.ui_text(ui_element_id, default_value, x, y, w, h, text_color, box_color, font_number)
def ui_text(ui_id=0, text="Type here", x=0, y=0, w=200, h=36, bg_color=0, fg_color=255, font=lv.font_unscii_8, one_line=True):
    ta = lv.textarea(tulip.lv_scr)
    ta.set_pos(x,y)
    ta.set_size(w,h)
    ta.set_style_text_font(font, 0)
    ta.set_style_bg_color(tulip.pal_to_lv(bg_color), lv.PART.MAIN)
    ta.set_style_text_color(tulip.pal_to_lv(fg_color),0)
    ta.set_placeholder_text(text)
    if(one_line): ta.set_one_line(True)
    tulip.lv_kb_group.add_obj(ta)
    ta.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return ta

# Checkboxes - val -- 0 is unchecked, 1 is checked
# style -- 0 is filled box, 1 is X, 2 is filled circle
#tulip.ui_checkbox(ui_element_id, val, x, y, w, mark_color, box_color, style)
# set value
#tulip.ui_checkbox(ui_element_id, val)
#val = tulip.ui_checkbox(ui_element_id)

def ui_checkbox(ui_id=0, val=False, x=0, y=0, w=50, bg_color=0, fg_color=255):
    cb = lv.checkbox(tulip.lv_scr)
    #cb.set_text(label)
    cb.set_pos(x,y)
    cb.set_style_bg_color(tulip.pal_to_lv(bg_color), lv.PART.INDICATOR)
    cb.set_style_bg_color(tulip.pal_to_lv(bg_color), lv.PART.INDICATOR | lv.STATE.CHECKED)
    cb.set_style_border_color(tulip.pal_to_lv(fg_color), lv.PART.INDICATOR)
    #cb.set_style_fg_color(lv.color_hex(tulip.rgb332_565(fg_color)), lv.PART.INDICATOR)
    cb.set_state(lv.STATE.CHECKED, val)
    cb.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return cb





