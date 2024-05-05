# voices.py
# drums.py
# lvgl drum machine for Tulip

import tulip
import midi
import lvgl as lv
import amy
from patches import patches


def redraw(app):
    # draw bg_x stuff, like the piano
    (app.screen_w, app.screen_h) = tulip.screen_size()
    # Since redraw is not within app.run() we are not guaranteed to be in the cwd of the app. 
    # luckily, tulip.run() adds the cwd of the app to the app class before starting.
    app.piano_w = 800
    app.piano_h = 301
    app.piano_y = app.screen_h - 280
    app.piano_x = int((app.screen_w - app.piano_w) / 2)
    app.white_key_w = 56
    tulip.bg_png(app.app_dir+'/piano.png', app.piano_x, app.piano_y)

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
        self.rect.align_to(self.label,lv.ALIGN.OUT_BOTTOM_LEFT,0,5)

        self.tempo = lv.slider(self.rect)
        self.tempo.set_style_bg_opa(lv.OPA.COVER, lv.PART.MAIN)
        self.tempo.set_width(220)
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
            midi.arpeggiator.set('hold', True)
        else:
            midi.arpeggiator.set('hold', False)
    def arpegg_cb(self,e):
        if(self.arpegg.get_state()==3):
            midi.arpeggiator.set('on', True)
        else:
            midi.arpeggiator.set('on', False)


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
        self.list.align_to(self.label,lv.ALIGN.OUT_BOTTOM_LEFT,0,5)
        self.buttons = []
        self.button_texts = []
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
            for idx,i in enumerate(items):
                button = self.list.add_button(None, i) #lv.SYMBOL.PLUS, i)
                button.get_child(0).set_long_mode(0) # wrap instead of scroll 
                button.add_event_cb(self.list_cb, lv.EVENT.CLICKED, None)
                self.buttons.append(button)
                self.button_texts.append(i)

    def select(self, index, defer=False):
        if(self.selected is not None):
            self.buttons[self.selected].set_style_bg_color(tulip.pal_to_lv(self.default_bg), 0)
        self.selected = index
        if index is not None:
            self.buttons[self.selected].set_style_bg_color(tulip.pal_to_lv(129), 0)
            if(self.name=='channel'):
                current_patch(self.selected + 1)
            elif(self.name=='synth'):
                update_patches(self.button_texts[self.selected])
            elif(self.name=='mode'):
                mode = ['up', 'down', 'updown', 'rand'][index]
                midi.arpeggiator.set('direction', mode)
            elif(self.name=='range'):
                midi.arpeggiator.set('octaves', index + 1)
            else:
                if not defer:
                    update_map()

    def list_cb(self, e):
        button = e.get_target_obj()
        self.select(button.get_index())

def play_note_from_coord(app, x, y, up):
    (r,g,b) = tulip.rgb(tulip.bg_pixel(x,y))
    intensity = float( (r+g+b) / (256*3) )
    white_key_notes = [0,2,4,5,7,9,11,12,14,16,17,19,21,23]

    white_key = int((x-app.piano_x)/app.white_key_w)
    # white key?
    if(intensity > 0.59):
        note_idx = white_key_notes[white_key]
    else:
        # how many px from the left side of the white key
        offset = x - (app.piano_x+(white_key*app.white_key_w))
        if(offset < int(app.white_key_w*0.66)): # left 2/3rds
            note_idx = white_key_notes[white_key] - 1
        else: # right third
            note_idx = white_key_notes[white_key] + 1
    if(up):
        tulip.midi_local((128+app.channels.selected, note_idx+48, 127))
    else:
        tulip.midi_local((144+app.channels.selected, note_idx+48, 127))



def touch(up):
    global app
    x,y = [-1,-1,-1], [-1,-1,-1]
    (x[0],y[0],x[1],y[1],x[2],y[2]) = tulip.touch()
    for i in range(3):
        if(x[i] >= app.piano_x and x[i] <= app.piano_x+app.piano_w and y[i] >= app.piano_y and y[i] <= app.piano_y+app.piano_h):
            play_note_from_coord(app, x[i], y[i], up)

def process_key(key):
    global app
    # play kb notes from keyboard?
    pass

# I don't love this approach. It works. We should make a more generic tulip defer (wrapper around timer) that doesn't eat a sequencer slot
def step(t):
    global app
    if(app.redraw_ticks is not None):
        app.redraw_ticks = app.redraw_ticks - 1
        if(app.redraw_ticks == 0):
            app.redraw_ticks = None
            redraw(app)

def quit(screen):
    deactivate(app)
    tulip.seq_remove_callback(step)

def activate(screen):
    app.redraw_ticks = 2
    # start listening to the keyboard again
    tulip.keyboard_callback(process_key)
    tulip.touch_callback(touch)

def deactivate(screen):
    # i am being switched away -- keep running but clear and close any active callbacks 
    tulip.bg_clear()
    tulip.keyboard_callback()
    tulip.touch_callback()

# actually make the change in our midi map
def update_map():
    global app
    # channels guaranteed to always be selected
    if(app.patchlist.selected is not None and app.polyphony.selected is not None and app.synths.selected is not None):
        patch_no = app.patchlist.selected
        if(app.synths.selected == 1): patch_no += 128
        if(app.synths.selected == 2): patch_no += 256
        if(app.synths.selected == 3): patch_no += 1024
        channel = app.channels.selected + 1
        polyphony = app.polyphony.selected + 1
        # Check if this is a new thing
        channel_patch, amy_voices = midi.config.channel_info(channel)
        channel_polyphony = 0 if amy_voices is None else len(amy_voices)
        if  (channel_patch, channel_polyphony) != (patch_no, polyphony):
            tulip.music_map(channel, patch_number=patch_no,
                            voice_count=polyphony)

# populate the patches dialog from patches,oy
def update_patches(synth):
    global app
    if(synth=='DX7'):
        app.patchlist.replace_items(patches[128:256])
    if(synth=='Juno-6'):
        app.patchlist.replace_items(patches[0:128])
    if(synth=='Custom'):
        app.patchlist.replace_items([("Custom %d" % x) for x in range(32)])
    if(synth=='Misc'):
        app.patchlist.replace_items([])
    app.patchlist.label.set_text("%s patches" % (synth))
# Get current settings for a channel from midi.config.
def current_patch(channel):
    global app
    channel_patch, amy_voices = midi.config.channel_info(channel)

    if channel_patch is not None:
        polyphony = len(amy_voices)
        if channel_patch < 128:
            # We defer here so that setting the UI component doesn't trigger an update before it updates
            app.synths.select(0, defer=True)
            app.patchlist.select(channel_patch, defer=True)
            pass
        elif channel_patch < 256:
            app.synths.select(1, defer=True)
            app.patchlist.select(channel_patch - 128, defer=True)
        elif channel_patch < 1024:
            app.synths.select(2, defer=True)
            app.patchlist.select(channel_patch - 256, defer=True)
        else:
            app.synths.select(3, defer=True)
            app.patchlist.select(channel_patch - 1024, defer=True)
        app.polyphony.select(polyphony - 1, defer=True)
    else:
        # no patch set for this chanel
        app.patchlist.select(None)
        app.polyphony.select(None)


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

    # Skip 10, drums
    app.channels = ListColumn('channel',["1","2","3","4","5","6","7","8","9","11","12","13","14","15","16"], selected=0, width=100)
    app.add(app.channels, direction=lv.ALIGN.OUT_BOTTOM_LEFT)

    app.synths = ListColumn('synth', ["Juno-6", "DX7", "Misc", "Custom"])
    app.add(app.synths)

    app.patchlist = ListColumn('patches')
    app.add(app.patchlist)

    app.polyphony = ListColumn('polyphony', [str(x+1) for x in range(8)], width=100)
    app.add(app.polyphony)
    app.settings = Settings()
    app.add(app.settings)

    current_patch(1)


    app.present()


