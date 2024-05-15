# ui.py
# imports into tulip for ui_x translation into lvgl
# also has keyboard and other small LVGL things
import tulip
import lvgl as lv

lv_soft_kb = None
lv_launcher = None


# For our convenience functions, Can be overriden
tulip.ui_callback = None

running_apps = {}
current_app_string = "repl"


# Returns the keypad indev
def get_keypad_indev():
    nobody = lv.indev_t()
    a = nobody.get_next()
    if (a.get_type() == lv.INDEV_TYPE.KEYPAD):
        return a
    b = a.get_next()
    if (b.get_type() == lv.INDEV_TYPE.KEYPAD):
        return b
    print("Couldn't find indev of type KEYPAD")
    return None

# Convert tulip rgb332 pal idx into lv color
def pal_to_lv(pal):
    (r,g,b) = tulip.rgb(pal, wide=True) # todo -- not sure if we use wide or not
    return lv.color_make(r,g,b)

# Convert tulip rgb332 pal idx into lv color
def lv_to_pal(lvcolor):
    return tulip.color(lvcolor.red, lvcolor.green, lvcolor.blue)

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

def current_uiscreen():
    return running_apps[current_app_string]
def current_lv_group():
    return current_uiscreen().group

# The entire UI is loaded into this screen, which we can swap out from "main" REPL screen
class UIScreen():
    # Constants you can change
    default_bg_color = 0
    # Start drawing at this position, a little to the right of the edge and 100px down
    default_offset_x = 10
    default_offset_y = 100

    def __init__(self, name, keep_tfb = False, bg_color=default_bg_color, offset_x=default_offset_x, offset_y=default_offset_y, 
        activate_callback=None, quit_callback=None, deactivate_callback=None, handle_keyboard=False):
        self.screen = lv.obj() # a screen, will be display size (which is actually 2x H_RES)
        self.group = lv.obj(self.screen) # the group to write UI elements to in the screen
        self.group.set_width(tulip.screen_size()[0])
        self.group.set_height(tulip.screen_size()[1])
        self.group.set_style_radius(0,lv.PART.MAIN)
        self.group.set_style_border_width(0, lv.PART.MAIN)

        lv_depad(self.group)
        self.keep_tfb = keep_tfb
        self.handle_keyboard = handle_keyboard
        self.bg_color = bg_color
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.last_obj_added = None
        self.group.set_style_bg_color(pal_to_lv(self.bg_color), lv.PART.MAIN)
        self.name = name
        self.alttab_button = None
        self.quit_button = None
        self.running = True # is this code running 
        self.active = False # is it showing on screen 
        self.imported_modules = []
        self.activate_callback = activate_callback
        self.deactivate_callback = deactivate_callback
        self.quit_callback = quit_callback
        self.kb_group = lv.group_create()
        if(self.name != 'repl'):
            self.kb_group.set_default()
        running_apps[self.name] = self

    def draw_task_bar(self):
        # draw whatever all screens share
        if(self.alttab_button is None):
            if( len(running_apps)>1):
                self.alttab_button = lv.button(self.group)
                self.alttab_button.set_style_bg_color(pal_to_lv(11), lv.PART.MAIN)
                self.alttab_button.set_style_radius(0,lv.PART.MAIN)
                alttab_label = lv.label(self.alttab_button)
                alttab_label.set_style_text_font(lv.font_montserrat_12,0)
                alttab_label.set_text(lv.SYMBOL.SHUFFLE)
                alttab_label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
                self.alttab_button.align_to(self.group, lv.ALIGN.TOP_RIGHT,0,0)
                self.alttab_button.add_event_cb(self.alttab_callback, lv.EVENT.CLICKED, None)
        else:
            if(len(running_apps) == 1):
                self.alttab_button.delete()
                self.alttab_button = None

        if(self.quit_button is None):
            if(self.name != "repl"):
                self.quit_button = lv.button(self.group)
                self.quit_button.set_style_bg_color(pal_to_lv(128), lv.PART.MAIN)
                self.quit_button.set_style_radius(0,lv.PART.MAIN)
                quit_label = lv.label(self.quit_button)
                quit_label.set_style_text_font(lv.font_montserrat_12,0)
                quit_label.set_text(lv.SYMBOL.POWER)
                quit_label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
                self.quit_button.align_to(self.alttab_button, lv.ALIGN.OUT_LEFT_MID,0,0)
                self.quit_button.add_event_cb(self.screen_quit_callback, lv.EVENT.CLICKED, None)
            else:
                self.launcher_button = lv.button(self.group)
                self.launcher_button.set_style_bg_color(pal_to_lv(36), lv.PART.MAIN)
                self.launcher_button.set_style_radius(0,lv.PART.MAIN)
                launcher_label = lv.label(self.launcher_button)
                launcher_label.set_style_text_font(lv.font_montserrat_12,0)
                launcher_label.set_text(lv.SYMBOL.LIST)
                launcher_label.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)
                self.launcher_button.align_to(self.group, lv.ALIGN.BOTTOM_RIGHT,0,0)
                self.launcher_button.add_event_cb(launcher, lv.EVENT.CLICKED, None)



    def set_bg_color(self, bg_color):
        self.bg_color = bg_color
        self.group.set_style_bg_color(pal_to_lv(bg_color), lv.PART.MAIN)


    def alttab_callback(self, e):
        if(len(running_apps)>1):
            self.active = False
            if(self.deactivate_callback is not None):
                self.deactivate_callback(self)
    
            # Find the next app in the list (assuming dict is ordered by insertion, I think it is)
            apps = list(running_apps.items())
            for i,app in enumerate(apps):
                if(self.name == app[0]):
                    apps[(i + 1) % len(running_apps)][1].present()

    def screen_quit_callback(self, e):
        import gc
        if(self.quit_callback is not None):
            self.quit_callback(self)
        self.running = False
        self.active = False
        self.remove_items()
        del running_apps[self.name]
        # Delete the modules we imported
        for m in self.imported_modules:
            exec('del sys.modules["%s"]' % (m))
        gc.collect()
        repl_screen.present()

    # add an obj (or list of obj) to the screen, aligning by the last one added,
    # or the object relative (if you want to for example make a new line)
    def add(self, obj, direction=lv.ALIGN.OUT_RIGHT_MID, relative=None, pad_x=0, pad_y=0):
        if(relative is not None):
            self.last_obj_added = relative.group

        if(type(obj) != list): obj = [obj]
        for o in obj:
            #o.update_callbacks(self.change_callback)
            o.group.set_parent(self.group)
            o.group.set_style_bg_color(pal_to_lv(self.bg_color), lv.PART.MAIN)
            o.group.set_height(lv.SIZE_CONTENT)
            if(self.last_obj_added is None):
                o.group.align_to(self.group,lv.ALIGN.TOP_LEFT,self.offset_x,self.offset_y)
            else:
                o.group.align_to(self.last_obj_added, direction,0,0)
            o.group.set_width(o.group.get_width()+pad_x)
            o.group.set_height(o.group.get_height()+pad_y)
            self.last_obj_added = o.group

    # Show the UI on the screen. Set up the keyboard group listener. Draw the task bar. 
    def present(self):
        current_app_string = self.name
        self.active = True
        self.draw_task_bar()

        lv.screen_load(self.screen)

        if(self.handle_keyboard):
            get_keypad_indev().set_group(self.kb_group)
        if(self.name == 'repl'):
            tulip.tfb_start()
            tulip.set_screen_as_repl(1)
        else:
            tulip.set_screen_as_repl(0)
            if(self.keep_tfb):
                tulip.tfb_start()
            else:
                tulip.tfb_stop()
        if(self.activate_callback is not None):
            self.activate_callback(self)

    # Remove the elements you created
    def remove_items(self):
        things = self.screen.get_child_count()
        for i in range(things):
            if(self.screen.get_child(0) is not None):
                self.screen.get_child(0).delete()
        self.last_obj_added = None


# A base class for our UI elements -- will also move this into Tulip
class UIElement():
    # We make one temp screen for the elements to use, then add it to the UIScreen parent at add.
    temp_screen = lv.obj()

    def __init__(self):
        #self.change_callback = None
        self.group = lv.obj(UIElement.temp_screen)
        # Hot tip - set this to 1 if you're debugging why elements are not aligning like you think they should
        self.group.set_style_border_width(0, lv.PART.MAIN)

    def update_callbacks(self, cb):
        pass
        #self.change_callback = cb


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
    lv_soft_kb = lv.keyboard(current_lv_group())
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
            tulip.run('juno6')
        if(text=="Drums"):
            tulip.run('drums')
        if(text=='Voices'):
            tulip.run('voices')
        if(text=='Pattern'):
            tulip.run('pattern')
        if(text=="Keyboard"):
            keyboard()
        if(text=="Wordpad"):
            tulip.run("wordpad")
        if(text=="Wi-Fi"):
            try:
                wifi()
            except NameError:
                print("Put a wifi() function in your boot.py")
        if(text=="Reset"):
            if(tulip.board()!="DESKTOP"):
                import machine
                machine.reset()


# Draw a lvgl list box as a little launcher
def launcher(ignore=True):
    global lv_launcher
    if(lv_launcher is not None):
        #print("Shutting down launcher")
        lv_launcher.delete()
        lv_launcher=None
        return
    #print("starting up launcher")
    lv_launcher = lv.list(repl_screen.group)
    lv_launcher.set_size(195, 140)
    lv_launcher.set_align(lv.ALIGN.BOTTOM_RIGHT)
    lv_launcher.set_style_text_font(lv.font_montserrat_12,0)
    b_close = lv_launcher.add_button(lv.SYMBOL.CLOSE, "Close")
    b_close.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_voices = lv_launcher.add_button(lv.SYMBOL.AUDIO, "Voices")
    b_voices.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_juno = lv_launcher.add_button(lv.SYMBOL.AUDIO, "Juno-6")
    b_juno.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_drums = lv_launcher.add_button(lv.SYMBOL.NEXT, "Drums")
    b_drums.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_pattern = lv_launcher.add_button(lv.SYMBOL.NEXT, "Pattern")
    b_pattern.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_keyboard = lv_launcher.add_button(lv.SYMBOL.KEYBOARD, "Keyboard")
    b_keyboard.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_wordpad = lv_launcher.add_button(lv.SYMBOL.FILE, "Wordpad")
    b_wordpad.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_wifi = lv_launcher.add_button(lv.SYMBOL.WIFI, "Wi-Fi")
    b_wifi.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)
    b_power = lv_launcher.add_button(lv.SYMBOL.POWER,"Reset")
    b_power.add_event_cb(launcher_cb, lv.EVENT.CLICKED, None)




# Callback that you can have LVGL objects register. 
# It gets the event (which you can get the object from) and "extra" which we put ui_id in for legacy uses
def lv_callback(e, extra):
    code = e.get_code()
    obj = e.get_target_obj()
    if(tulip.ui_callback is not None):
        tulip.ui_callback(obj, code, extra)

# Draw a msgbox on screen. 
def ui_msgbox(buttons=['OK', 'Cancel'], title='Title', message='Message box', ui_id=None):
    mbox = lv.msgbox(current_lv_group())
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
    slider = lv.slider(current_lv_group())
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
    button = lv.button(current_lv_group())
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
    label = lv.label(current_lv_group())
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
    ta = lv.textarea(current_lv_group())
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
    if(ui_id is not None):
        ta.add_event_cb(lambda e: lv_callback(e, ui_id), lv.EVENT.VALUE_CHANGED, None)
    return ta

# Copy of our ui_checkbox with lvgl 
def ui_checkbox(ui_id=None, text=None, val=False, x=0, y=0, bg_color=None, fg_color=None):
    cb = lv.checkbox(current_lv_group())
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



repl_screen = UIScreen("repl", bg_color=9, handle_keyboard=True)
repl_screen.present()

