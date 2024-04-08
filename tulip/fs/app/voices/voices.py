# voices.py
# drums.py
# lvgl drum machine for Tulip

import tulip
import lvgl as lv
import amy
from patches import patches

def redraw(app):
    # draw bg_x stuff, like the piano
    (app.screen_w, app.screen_h) = tulip.screen_size()

    # piano size
    app.piano_w = app.screen_w - 100 # initial, will get adjusted slightly
    app.piano_x = 50 # initial, will get adjusted slightly
    app.piano_h = 250
    app.white_key_w = 50

    # computed
    app.piano_y = app.screen_h - app.piano_h
    app.black_key_w = int(app.white_key_w/2)
    app.black_key_h = int(float(app.piano_h) * (2.0/3.0))
    app.white_keys = int(app.piano_w / app.white_key_w)-1
    app.piano_w = app.white_keys * app.white_key_w # reset
    app.piano_x = int( (app.screen_w - app.piano_w)/2)
    tulip.bg_rect(app.piano_x, app.piano_y, app.piano_w, app.piano_h, 255, 1)
    app.black_idx = [1,2,4,5,6,8,9,11,12,13,15,16,18,19,20,22,23] # etc
    for i in range(app.white_keys)[1:]:
        white_line_x = app.piano_x+(i*app.white_key_w)
        tulip.bg_line(white_line_x, app.piano_y, white_line_x, app.piano_y+app.piano_h, 0)
        if i in app.black_idx:
            tulip.bg_rect(white_line_x-int(app.white_key_w/4), app.piano_y, app.black_key_w, app.black_key_h, 0, 1)

class Settings(tulip.UIElement):
    def __init__(self, width=350, height=300):
        super().__init__()
        self.group.set_size(width, height)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)
        self.label = lv.label(self.group)
        self.label.set_text("sequencer and arpeggiator")
        self.rect = lv.obj(self.group)
        self.rect.set_style_bg_color(tulip.pal_to_lv(9), 0)
        self.rect.remove_flag(lv.obj.FLAG.SCROLLABLE)

        self.rect.set_size(width-25,height-20)
        self.rect.align_to(self.label,lv.ALIGN.OUT_BOTTOM_LEFT,0,0)

        self.tempo = lv.slider(self.rect)
        self.tempo.set_style_bg_opa(lv.OPA.COVER, lv.PART.MAIN)
        self.tempo.set_width(180)
        self.tempo.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.INDICATOR)
        self.tempo.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.MAIN)
        self.tempo.set_style_bg_color(tulip.pal_to_lv(129), lv.PART.KNOB)
        self.tempo.align_to(self.rect, lv.ALIGN.TOP_LEFT,0,0)
        self.tempo_label = lv.label(self.rect)
        self.tempo.set_value(int(tulip.seq_bpm() / 2.4),lv.ANIM.OFF)
        self.tempo_label.set_text("%d BPM" % (tulip.seq_bpm()))
        self.tempo_label.align_to(self.tempo, lv.ALIGN.OUT_RIGHT_MID,10,0)
        self.tempo.add_event_cb(self.tempo_cb, lv.EVENT.VALUE_CHANGED, None)

        alabel = lv.label(self.rect)
        alabel.set_text("Arpeggiator:")
        alabel.align_to(self.tempo, lv.ALIGN.OUT_BOTTOM_LEFT,0,30)
        self.arpegg = lv.switch(self.rect)
        self.arpegg.set_style_border_width(1, lv.PART.MAIN)
        self.arpegg.set_style_border_color(tulip.pal_to_lv(129), 0)
        self.arpegg.align_to(alabel, lv.ALIGN.OUT_RIGHT_MID,10,0)
        self.arpegg.add_event_cb(self.arpegg_cb, lv.EVENT.VALUE_CHANGED, None)
        hlabel = lv.label(self.rect)
        hlabel.set_text("Hold:")
        hlabel.align_to(self.arpegg, lv.ALIGN.OUT_RIGHT_MID,10,0)
        self.hold = lv.switch(self.rect)
        self.hold.set_style_border_width(1, lv.PART.MAIN)
        self.hold.set_style_border_color(tulip.pal_to_lv(129), 0)
        self.hold.align_to(hlabel, lv.ALIGN.OUT_RIGHT_MID,10,0)
        self.hold.add_event_cb(self.hold_cb, lv.EVENT.VALUE_CHANGED, None)

        self.mode = ListColumn("mode", ["Up", "Down", "U&D", "Rand"], width=130, height=160, selected=0)
        self.mode.group.set_parent(self.rect)
        self.mode.group.set_style_bg_color(tulip.pal_to_lv(9),0)
        self.mode.group.align_to(alabel, lv.ALIGN.OUT_BOTTOM_LEFT, 0, 20)
        self.range = ListColumn("range", ["1", "2", "3"], width=130, height=160, selected=0)
        self.range.group.set_parent(self.rect)
        self.range.group.set_style_bg_color(tulip.pal_to_lv(9),0)
        self.range.group.align_to(self.mode.group, lv.ALIGN.OUT_RIGHT_TOP, 10, 0)

    def tempo_cb(self,e):
        new_bpm = self.tempo.get_value()*2.4
        if(new_bpm < 1.0): new_bpm = 1
        tulip.seq_bpm(new_bpm)
        self.tempo_label.set_text("%d BPM" % (tulip.seq_bpm()))
    def hold_cb(self,e):
        if(self.hold.get_state()==3): 
            self.arpegg_hold = True
        else:
            self.arpegg_hold = False
    def arpegg_cb(self,e):
        if(self.arpegg.get_state()==3): 
            self.arpegg_on = True
        else:
            self.arpegg_on = False


class ListColumn(tulip.UIElement):
    def __init__(self, name, items=None, selected=None, width=175, height=300):
        super().__init__() 
        self.name = name
        self.selected = selected
        self.group.set_size(width,height)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)
        self.label = lv.label(self.group)
        self.label.set_text(name)
        self.list = lv.list(self.group)
        self.list.set_size(width-25,height-20)
        self.list.align_to(self.label,lv.ALIGN.OUT_BOTTOM_LEFT,0,0)
        self.buttons = []
        tulip.lv_depad(self.list)
        tulip.lv_depad(self.group)
        tulip.lv_depad(self.label)
        self.replace_items(items)
        self.default_bg = 36
        if(self.selected is not None):
            self.buttons[self.selected].set_style_bg_color(tulip.pal_to_lv(129), 0)

    def replace_items(self, items):
        if items is not None:
            self.list.clean()
            self.buttons = []
            for i in items:
                button = self.list.add_button(lv.SYMBOL.PLUS, i)
                button.add_event_cb(self.list_cb, lv.EVENT.CLICKED, None)
                self.buttons.append(button)

    def list_cb(self, e):
        if(self.selected is not None):
            self.buttons[self.selected].set_style_bg_color(tulip.pal_to_lv(self.default_bg), 0)
        button = e.get_target_obj()
        text = button.get_child(1).get_text()
        self.selected = button.get_index()
        self.buttons[self.selected].set_style_bg_color(tulip.pal_to_lv(129), 0)
        if(self.name=='synth'):
            update_patches(text)

        print('list_db ' + self.name + ' ' + text)

def play_note_from_coord(app, x, y):
    white_key = int((x-app.piano_x)/app.white_key_w)
    if(white_key in app.black_idx and y<app.piano_y+app.black_key_h):
        print('black %d' % (white_key))
    else:
        print('white %d' % (white_key))


def touch(up):
    global app
    x,y = [-1,-1,-1], [-1,-1,-1]
    (x[0],y[0],x[1],y[1],x[2],y[2]) = tulip.touch()
    for i in range(3):
        if(x[i] >= app.piano_x and x[i] <= app.piano_x+app.piano_w and y[i] >= app.piano_y and y[i] <= app.piano_y+app.piano_h):
            if(not up):
                play_note_from_coord(app, x[i], y[i])
    #print("got points up %d : %d,%d %d,%d %d,%d" % (up, points[0], points[1], points[2], points[3], points[4], points[5] ))

def process_key(key):
    global app
    # play kb notes from keyboard?
    pass

def step(t):
    global app
    if(app.redraw_ticks is not None):
        app.redraw_ticks = app.redraw_ticks - 1
        if(app.redraw_ticks == 0):
            redraw(app)
            app.redraw_ticks = None

def quit(screen):
    deactivate(app)
    tulip.seq_remove_callback(step)

def activate(screen):
    # Re-draw grid -- for tulip bg_X commands, you have to defer this as LVGL will write for a few frames and would overwrite your BG 
    # so i just use the sequencer to wait for the next tick and redraw then. adds a tiny bit of lag on activation
    app.redraw_ticks = 2
    # start listening to the keyboard again
    tulip.keyboard_callback(process_key)
    tulip.touch_callback(touch)

def deactivate(screen):
    # i am being switched away -- keep running but clear and close any active callbacks 
    tulip.bg_clear()
    tulip.keyboard_callback()
    tulip.touch_callback()

def update_patches(synth):
    global app
    if(synth=='DX7'):
        app.patches.replace_items(patches[128:256])
    if(synth=='Juno-6'):
        app.patches.replace_items(patches[0:128])
    if(synth=='Custom'):
        app.patches.replace_items([("Custom %d" % x) for x in range(32)])
    if(synth=='Misc'):
        app.patches.replace_items([])

    app.patches.label.set_text("%s patches" % (synth))


def run(screen):
    global app 
    app = screen # we can use the screen obj passed in as a general "store stuff here" class, as well as inspect the UI 
    app.set_bg_color(0)
    app.offset_y = 25
    app.offset_x = 50
    app.redraw_ticks = None
    app.quit_callback = quit
    app.activate_callback = activate
    app.deactivate_callback = deactivate
    tulip.seq_add_callback(step, int(tulip.seq_ppq()/2))

    app.channels = ListColumn('channel',[str(i+1) for i in range(16)], selected=0, width=100)
    app.add(app.channels, direction=lv.ALIGN.OUT_BOTTOM_LEFT)

    app.synths = ListColumn('synth', ["Juno-6", "DX7", "Misc", "Custom"])
    app.add(app.synths)

    app.patches = ListColumn('Patches')
    update_patches("Juno-6")
    app.add(app.patches)

    app.polyphony = ListColumn('polyphony', [str(x+1) for x in range(6)], width=100)
    app.add(app.polyphony)

    app.settings = Settings()
    app.add(app.settings)

    app.present()


