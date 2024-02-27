# ui.py
# imports into tulip for ui_x translation into lvgl
# also has keyboard and other small LVGL things
import tulip
import lvgl as lv

# Since this is called on boot this is guaranteed to grab the repl screen
lv_scr = lv.screen_active() 

# gets the group that receives keyboard events on the repl
# you have to add objs to this group for them to receive kbd
lv_kb_group = lv.group_by_index(0) 
lv_soft_kb = None
lv_launcher = None

lv_default_font = lv.font_montserrat_12

# For our convenience functions, Can be overriden
tulip.ui_callback = None

# Convert tulip rgb332 pal idx into lv color
def pal_to_lv(pal):
    (r,g,b) = tulip.rgb(pal, wide=True) # todo -- not sure if we use wide or not
    return lv.color_make(r,g,b)

# Remove padding from an LVGL object. Sometimes useful. 
def lv_depad(obj):
    obj.set_style_pad_left(0,0)
    obj.set_style_pad_right(0,0)
    obj.set_style_pad_top(0,0)
    obj.set_style_pad_bottom(0,0)
    obj.set_style_margin_left(0,0)
    obj.set_style_margin_right(0,0)
    obj.set_style_margin_top(0,0)
    obj.set_style_margin_bottom(0,0)

# The entire UI is loaded into this screen, which we can swap out from "main" REPL screen
class UIScreen():
    # Constants you can change
    default_bg_color = 73
    # Start drawing at this position, a little to the right of the edge and 100px down
    default_offset_x = 10
    default_offset_y = 100

    # Class vars we use to keep the screen around
    def __init__(self, bg_color=default_bg_color, offset_x=default_offset_x, offset_y=default_offset_y):
        self.screen = lv.obj()
        self.bg_color = bg_color
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.change_callback = None
        self.last_screen = lv_scr
        self.last_obj_added = None
        self.screen.set_style_bg_color(pal_to_lv(self.bg_color), lv.PART.MAIN)

    # add an obj (or list of obj) to the screen, aligning by the last one added,
    # or the object relative (if you want to for example make a new line)
    def add(self, obj, direction=lv.ALIGN.OUT_RIGHT_MID, relative=None, pad=0):
        if(relative is not None):
            self.last_obj_added = relative.group

        if(type(obj) != list): obj = [obj]
        for o in obj:
            o.group.set_parent(self.screen)
            o.group.set_style_bg_color(pal_to_lv(self.bg_color), lv.PART.MAIN)
            o.group.set_height(lv.SIZE_CONTENT)
            if(self.last_obj_added is None):
                o.group.align_to(self.screen,lv.ALIGN.TOP_LEFT,self.offset_x,self.offset_y)
            else:
                o.group.align_to(self.last_obj_added, direction,0,0)
            o.group.set_width(o.group.get_width()+pad)
            self.last_obj_added = o.group

    # Show the UI on the screen
    def present(self):
        lv.screen_load(self.screen)

    # Keep everything around, but load the repl screen
    def clear(self):
        lv.screen_load(self.last_screen)

    # Remove the elements you created
    def remove_items(self):
        self.clear()
        things = self.screen.get_child_count()
        for i in range(things):
            self.screen.get_child(0).delete()
        self.last_obj_added = None


# A base class for our UI elements -- will also move this into Tulip
class UIElement():
    # We make one temp screen for the elements to use, then add it to the UIScreen parent at add.
    temp_screen = lv.obj()
    def __init__(self):
        self.group = lv.obj(UIElement.temp_screen)
        self.group.set_style_border_width(0, lv.PART.MAIN)



# Callback for soft keyboard to send chars to Tulip.
def lv_soft_kb_cb(e):
    global lv_soft_kb, lv_last_mode
    kb = e.get_target_obj()
    button = kb.get_selected_button()
    text = kb.get_button_text(button)
    code = text[0]

    if(code==lv.SYMBOL.NEW_LINE): 
        tulip.key_send(13)
    elif(code==lv.SYMBOL.BACKSPACE): 
        if(lv_last_mode == kb.get_mode()): # there's a bug where the mode swticher sends BS
            tulip.key_send(8)
    elif(code==lv.SYMBOL.KEYBOARD):
        lv_soft_kb.delete()
        lv_soft_kb = None
        return
    elif(ord(code)==49): # special -- sends a "1" char even if just hit the mode switcher '1#'
        if(kb.get_mode() == lv_last_mode):  # only update after switching modes
            tulip.key_send(49)
    elif(len(text)==1 and ord(code)>31 and ord(code)<127): 
        tulip.key_send(ord(code))

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

def launcher_cb(e):
    global lv_launcher
    if(e.get_code() == lv.EVENT.CLICKED):
        button = e.get_target_obj()
        text = button.get_child(1).get_text()
        lv_launcher.delete()
        lv_launcher = None
        if(text=="Juno-6"):
            tulip.run('juno_ui')
        if(text=="Keyboard"):
            keyboard()
        if(text=="Editor"):
            # TODO something weird about calling editor from here
            pass
        if(text=="Wordpad"):
            tulip.run("wordpad")
        if(text=="Wi-Fi"):
            wifi()
        if(text=="Reset"):
            if(tulip.board()!="DESKTOP"):
                import machine
                machine.reset()


# Draw a lvgl list box as a little launcher
def launcher():
    global lv_launcher
    if(lv_launcher is not None):
        lv_launcher.delete()
        lv_launcher=None
        return

    lv_launcher = lv.list(lv_scr)
    lv_launcher.set_size(195, 140)
    lv_launcher.set_align(lv.ALIGN.BOTTOM_RIGHT)
    b_close = lv_launcher.add_button(lv.SYMBOL.CLOSE, "Close")
    b_close.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_juno = lv_launcher.add_button(lv.SYMBOL.AUDIO, "Juno-6")
    b_juno.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_keyboard = lv_launcher.add_button(lv.SYMBOL.KEYBOARD, "Keyboard")
    b_keyboard.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_editor = lv_launcher.add_button(lv.SYMBOL.EDIT, "Editor")
    b_editor.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_wordpad = lv_launcher.add_button(lv.SYMBOL.FILE, "Wordpad")
    b_wordpad.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_wifi = lv_launcher.add_button(lv.SYMBOL.WIFI, "Wi-Fi")
    b_wifi.add_event_cb(launcher_cb, lv.EVENT.ALL, None)
    b_power = lv_launcher.add_button(lv.SYMBOL.POWER,"Reset")
    b_power.add_event_cb(launcher_cb, lv.EVENT.ALL, None)



# removes all ui elements
def ui_clear():
    current_screen = lv.screen_active()
    while(current_screen.get_child_count()):
        current_screen.get_child(0).delete()

    if(current_screen != lv_scr):
        # we're in a UI program. kill it
        lv.screen_load(lv_scr)
        current_screen.delete()

# Return how many LVGL objs you've created
def ui_count():
    return lv_scr.get_child_count()

# Callback that you can have LVGL objects register. 
# It gets the event (which you can get the object from) and "extra" which we put ui_id in for legacy uses
def lv_callback(e, extra):
    code = e.get_code()
    obj = e.get_target_obj()
    if(tulip.ui_callback is not None):
        tulip.ui_callback(obj, code, extra)

# Draw a msgbox on screen. 
def ui_msgbox(buttons=['OK', 'Cancel'], title='Title', message='Message box', ui_id=None):
    mbox = lv.msgbox(tulip.lv_scr)
    mbox.add_text(message)
    mbox.add_title(title)
    mbox.add_close_button()
    for b in buttons:
        mbox.add_footer_button(b)
    if(ui_id is not None):
        mbox.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.CLICKED, None)
    return mbox

# bar_color - the color of the whole bar, or just the set part if using two colors
# unset_bar_color - the color of the unset side of the bar, if None will just be all one color
# handle_v_pad, h_pad -- how many px above/below / left/right of the bar it extends
# handle_radius - 0 for square 
def ui_slider(val=0, x=0, y=0, w=None, h=None, bar_color=None, unset_bar_color=None, handle_color=None, handle_radius=None, 
    handle_v_pad=None, handle_h_pad=None, ui_id=None):
    slider = lv.slider(tulip.lv_scr)
    slider.set_pos(x,y)
    # Set opacity to full (COVER). Default is to mix the color with the BG.
    slider.set_style_bg_opa(lv.OPA.COVER, lv.PART.MAIN)
    if(w is not None):
        slider.set_width(w)
    if(h is not None):
        slider.set_height(h)
    if(bar_color is not None):
        slider.set_style_bg_color(pal_to_lv(bar_color), lv.PART.INDICATOR)
        if(unset_bar_color is None):
            slider.set_style_bg_color(pal_to_lv(bar_color), lv.PART.MAIN)
        else:
            slider.set_style_bg_color(pal_to_lv(unset_bar_color), lv.PART.MAIN)
    if(handle_color is not None):
        slider.set_style_bg_color(pal_to_lv(handle_color), lv.PART.KNOB)
    if(handle_radius is not None):
        slider.set_style_radius(handle_radius, lv.PART.KNOB)
    if(handle_v_pad is not None):
        slider.set_style_pad_ver(handle_v_pad, lv.PART.KNOB)
    if(handle_h_pad is not None):
        slider.set_style_pad_hor(handle_h_pad, lv.PART.KNOB)
    slider.set_value(int(val),lv.ANIM.OFF)
    if(ui_id is not None):
        slider.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return slider

# Copy of our "ui_button" with lvgl buttons
#tulip.ui_button(ui_element_id, "Button text", x, y, w, h, bg_pal_idx, fg_pal_idx, filled, font_number)
def ui_button(text=None, x=0, y=0, w=None, h=None, bg_color=None, fg_color=None, font=None, radius=None, ui_id=None):
    button = lv.button(lv_scr)
    button.set_x(x)
    button.set_y(y)
    if(w is not None):
        button.set_width(w)
    if(h is not None):
        button.set_height(h)
    if(radius is not None):
        button.set_style_radius(radius, lv.PART.MAIN)
    if(bg_color is not None):
        button.set_style_bg_color(pal_to_lv(bg_color), lv.PART.MAIN)
    if(text is not None):
        label = lv.label(button)
        label.set_text(text)
        if(font is not None):
            label.set_style_text_font(font, 0)
        label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
        # if button width was manually set, we need to re-pad the text so it is centered
        if(w is not None):
            label.set_width(w-(button.get_style_pad_left(0)*2))
        if(fg_color is not None):
            label.set_style_text_color(pal_to_lv(fg_color), 0)
    if(ui_id is not None):
        button.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.CLICKED, None)
    return button

def ui_label(text="", x=0, y=0, fg_color=None, w=None, font=None):
    label = lv.label(lv_scr)
    label.set_pos(x,y)
    if(w is not None):
        label.set_width(w)
    label.set_text(text)
    label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
    if(font is not None):
        label.set_style_text_font(font, 0)
    if(fg_color is not None):
        label.set_style_text_color(pal_to_lv(fg_color),0)
    return label

# Copy of our ui_text with lvgl textarea 
#tulip.ui_text(ui_element_id, default_value, x, y, w, h, text_color, box_color, font_number)
def ui_text(ui_id=None, text=None, placeholder=None, x=0, y=0, w=None, h=None, bg_color=None, fg_color=None, font=None, one_line=True):
    ta = lv.textarea(tulip.lv_scr)
    ta.set_pos(x,y)
    if(w is not None):
        ta.set_width(w)
    if(h is not None):
        ta.set_height(h)
    if(font is not None):
        ta.set_style_text_font(font, 0)
    if(bg_color is not None):
        ta.set_style_bg_color(pal_to_lv(bg_color), lv.PART.MAIN)
    if(fg_color is not None):
        ta.set_style_text_color(pal_to_lv(fg_color),0)
    if placeholder is not None:
        ta.set_placeholder_text(placeholder)
    if text is not None:
        ta.set_text(text)
    if(one_line): ta.set_one_line(True)
    tulip.lv_kb_group.add_obj(ta)
    if(ui_id is not None):
        ta.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return ta

# Copy of our ui_checkbox with lvgl 
def ui_checkbox(ui_id=None, text=None, val=False, x=0, y=0, bg_color=None, fg_color=None):
    cb = lv.checkbox(tulip.lv_scr)
    if(text is not None):
        cb.set_text(text)
    cb.set_pos(x,y)
    if(bg_color is not None):
        cb.set_style_bg_color(pal_to_lv(bg_color), lv.PART.INDICATOR)
        cb.set_style_bg_color(pal_to_lv(bg_color), lv.PART.INDICATOR | lv.STATE.CHECKED)
    if(fg_color is not None):
        cb.set_style_border_color(pal_to_lv(fg_color), lv.PART.INDICATOR)
    cb.set_state(lv.STATE.CHECKED, val)
    if(ui_id is not None):
        cb.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return cb





