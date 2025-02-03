# worldui.py

import lvgl as lv
import tulip, time
(H_RES,V_RES) = tulip.screen_size()
if(tulip.board()=='WEB'):
    import world_web as world
else:
    import world

app = None
TIME_BETWEEN_CHECKS_S = 60

def check_messages(x=None):
    global app

    # Different paths for web and normal world
    if(tulip.board()=="WEB"):
        world.grab("messages", n=25, mtype='text').then(lambda x: done(x))
    else:            
        messages = world.messages(n=25)
        done(messages)

    def done(messages):
        messages.reverse()
        text = ""
        for i in messages:
            nt = world.nice_time(i['age_ms'])
            text = text + "\n["+ nt +"] "+ i['username'] +": " +i['content']
        app.messages.ta.set_text(text)
        app.messages.ta.scroll_to_y(lv.COORD.MAX,0)

def check_files():
    global app
    if(tulip.board()=="WEB"):
        world.unique_files(count=12).then(lambda x: done(x))
    else:
        files = world.unique_files(count=12)
        done(files)

    def done(files):
        text = ""
        files.reverse()
        for i in files:
            nt = world.nice_time(i['age_ms'])
            fn = i['filename']
            if(fn.endswith('.tar')): fn = fn[:-4]
            text = text + "\n["+ nt +"] "+ i['username'] +": " + fn + " (" +i['content'] + ")"
        app.files.ta.set_text(text)
        app.files.ta.scroll_to_y(lv.COORD.MAX,0)

def check():
    check_files()
    check_messages()

def checker(x):
    global app
    if(tulip.ticks_ms() > app.last_check_ms + (TIME_BETWEEN_CHECKS_S*1000)):
        app.last_check_ms = tulip.ticks_ms()
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
        if(tulip.board()=="WEB"):
            world.post_message(text).then(lambda x:done(x))
        else:
            world.post_message(text)
            done(None)

        def done(x):
            app.entry.ta.set_text("")
            tulip.defer(check_messages, None, 5000)

class TextEntry(tulip.UIElement):
    def __init__(self, h, bgcolor=255):
        super().__init__()
        self.ta = lv.textarea(self.group)
        self.group.set_size(H_RES-40,h)
        self.ta.set_size(H_RES-80, h)
        self.ta.set_style_text_font(lv.font_montserrat_18, 0)
        self.ta.set_style_bg_color(tulip.pal_to_lv(bgcolor), lv.PART.MAIN)
        self.ta.set_style_text_color(tulip.pal_to_lv(255),0)
        self.ta.set_style_border_color(tulip.pal_to_lv(255), lv.PART.CURSOR | lv.STATE.FOCUSED)
        self.ta.set_placeholder_text("Type a message....")
        self.ta.remove_flag(lv.obj.FLAG.SCROLLABLE)
        self.ta.set_one_line(True)
        self.ta.add_event_cb(enter_cb, lv.EVENT.READY,None)
        lv.group_focus_obj(self.ta)

class TextSection(tulip.UIElement):
    def __init__(self, h, name, bgcolor=255):
        super().__init__()
        self.ta = lv.label(self.group)
        self.group.set_size(H_RES-40,h)
        self.ta.set_size(H_RES-80, h)
        self.ta.set_style_text_font(lv.font_unscii_8, 0)
        self.ta.set_style_bg_color(tulip.pal_to_lv(bgcolor), lv.PART.MAIN)
        self.ta.set_style_text_color(tulip.pal_to_lv(255),0)
        self.ta.set_style_border_color(tulip.pal_to_lv(0), lv.PART.CURSOR | lv.STATE.FOCUSED)

        self.label = lv.label(self.group)
        self.label.set_style_text_font(lv.font_montserrat_18, 0)
        self.label.set_text(name)
        self.label.set_style_text_color(tulip.pal_to_lv(255),0)
        self.label.align_to(self.ta, lv.ALIGN.OUT_TOP_LEFT, 0, 0)

def run(screen):
    global app
    if tulip.ip() is None:
        print("Needs wifi")
        screen.quit()
        return
    if(world.username is None):
        print("Type world.username='username' first.")
        screen.quit()
        return

    app = screen

    app.last_check_ms = -(TIME_BETWEEN_CHECKS_S*1000)
    

    screen.set_bg_color(13)
    screen.quit_callback = quit
    screen.activate_callback = activate
    screen.deactivate_callback = deactivate
    screen.handle_keyboard=True
    screen.offset_y = 30
    app.files = TextSection(120, "Latest files. Use world.download(name) in the REPL to get them.", bgcolor=35)
    app.messages = TextSection(280, "Latest messages", bgcolor=0)
    app.entry = TextEntry(60, bgcolor=0)

    screen.add(app.files, direction=lv.ALIGN.OUT_BOTTOM_LEFT)
    screen.add(app.messages, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    screen.add(app.entry, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    screen.present()




