# worldui.py

import lvgl as lv
import tulip, time, world
(H_RES,V_RES) = tulip.screen_size()

app = None
TIME_BETWEEN_CHECKS_S = 60

def check_messages(x=None):
    global app
    messages = world.messages(n=50)
    messages.reverse()
    for i in messages:
        if(i['age_ms'] < app.newest_message_ms_ago):
            nt = world.nice_time(i['age_ms'])
            text = "\n["+ nt +"] "+ i['username'] +": " +i['content']
            app.messages.ta.set_accepted_chars('')
            app.messages.ta.add_text(text)
            app.messages.ta.set_accepted_chars('~')
            app.newest_message_ms_ago = i['age_ms']

def check_files():
    global app
    text = ""
    files = world.unique_files(count=50)
    files.reverse()

    for i in files:
        if(i['age_ms'] < app.newest_file_ms_ago):
            nt = world.nice_time(i['age_ms'])
            fn = i['filename']
            if(fn.endswith('.tar')): fn = fn[:-4]
            text = "\n["+ nt +"] "+ i['username'] +": " + fn + " (" +i['content'] + ")"
            app.files.ta.set_accepted_chars('')
            app.files.ta.add_text(text)
            app.files.ta.set_accepted_chars('~')
            app.newest_file_ms_ago = i['age_ms']

def check():
    check_files()
    check_messages()
    app.last_check_ms = tulip.ticks_ms()

def checker(x):
    global app
    if(tulip.ticks_ms() > app.last_check_ms + (TIME_BETWEEN_CHECKS_S*1000)):
        check()
        

def activate(screen):
    tulip.frame_callback(checker)

def deactivate(screen):
    tulip.frame_callback()

def quit(screen):
    pass

def enter_cb(e):
    global app
    text = app.entry.ta.get_text()
    if(len(text)>1 and world.username is not None):
        world.post_message(text)
        app.entry.ta.set_text("")
        tulip.defer(check_messages, None, 5000)

class TextSection(tulip.UIElement):
    def __init__(self, h, name, bgcolor=0, editable=False):
        super().__init__()
        self.ta = lv.textarea(self.group)
        self.group.set_size(H_RES-40,h)
        #app.files.set_pos(0,50)
        self.ta.set_size(H_RES-50, h)
        self.ta.set_style_text_font(lv.font_montserrat_18, 0)
        self.ta.set_style_bg_color(tulip.pal_to_lv(bgcolor), lv.PART.MAIN)
        self.ta.set_style_text_color(tulip.pal_to_lv(255),0)
        if(editable):
            self.ta.set_style_border_color(tulip.pal_to_lv(255), lv.PART.CURSOR | lv.STATE.FOCUSED)
            self.ta.set_placeholder_text("Type a message....")
            self.ta.remove_flag(lv.obj.FLAG.SCROLLABLE)
            self.ta.set_one_line(True)
            self.ta.add_event_cb(enter_cb, lv.EVENT.READY,None)
            lv.group_focus_obj(self.ta)
        else:
            self.ta.set_style_border_color(tulip.pal_to_lv(0), lv.PART.CURSOR | lv.STATE.FOCUSED)

        if(name is not None):
            self.label = lv.label(self.group)
            self.label.set_text(name)
            self.label.align_to(self.ta, lv.ALIGN.OUT_TOP_LEFT, 0, 0)

def run(screen):
    global app
    if tulip.ip() is None:
        print("Needs wifi")
        screen.quit()
    
    if(world.prompt_username() is None):
        screen.quit()

    app = screen

    app.last_check_ms = -(TIME_BETWEEN_CHECKS_S*1000)
    
    app.newest_message_ms_ago = 1000*60*60*24*365 # a year ago
    app.newest_file_ms_ago = 1000*60*60*24*365 # a year ago

    screen.set_bg_color(5)
    screen.quit_callback = quit
    screen.activate_callback = activate
    screen.deactivate_callback = deactivate
    screen.handle_keyboard=True
    screen.offset_y = 30
    app.files = TextSection(120, "Latest files. Use world.download(name) to get them.", bgcolor=32)
    app.messages = TextSection(280, "Latest messages")
    app.entry = TextSection(60, None, editable=True)

    screen.add(app.files, direction=lv.ALIGN.OUT_BOTTOM_LEFT)
    screen.add(app.messages, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    screen.add(app.entry, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    screen.present()




