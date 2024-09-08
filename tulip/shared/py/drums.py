# drums.py
# lvgl drum machine for Tulip

from tulip import UIScreen, UIElement, pal_to_lv, lv_depad, lv, frame_callback, ticks_ms, seq_add_callback, seq_remove_callback, seq_ppq, seq_ticks
import amy
import midi
from patches import drumkit


# A single drum machine switch with LED
class DrumSwitch(UIElement):
    led_off_color = 255
    led_on_color = 224
    # R O Y W 808 colors
    switch_colors = [128,208,248,255]
    button_width = 30
    button_height = 40

    def __init__(self, switch_color_idx=0):
        super().__init__() 

        self.group.set_size(DrumSwitch.button_width+10,DrumSwitch.button_height+25)
        lv_depad(self.group)

        top_rect = lv.obj(self.group)
        top_rect.set_style_bg_color(pal_to_lv(DrumSwitch.switch_colors[switch_color_idx]), 0)
        top_rect.set_size(DrumSwitch.button_width, 20)
        lv_depad(top_rect)

        top_rect.align_to(self.group, lv.ALIGN.TOP_MID, 0, 0)
        bottom_rect = lv.obj(self.group)
        bottom_rect.set_size(DrumSwitch.button_width, DrumSwitch.button_height)
        bottom_rect.set_style_radius(0,0)
        bottom_rect.align_to(top_rect, lv.ALIGN.OUT_BOTTOM_MID,0,0)
        bottom_rect.set_style_bg_color(pal_to_lv(DrumSwitch.switch_colors[switch_color_idx]), 0)
        bottom_rect.add_event_cb(self.cb, lv.EVENT.CLICKED, None)
        lv_depad(bottom_rect)

        self.led = lv.led(self.group)
        self.led.set_size(5,5)
        self.led.align_to(top_rect, lv.ALIGN.CENTER,0,0)
        self.led.set_brightness(150)
        self.led.set_color(pal_to_lv(DrumSwitch.led_on_color))
        self.led.off()
        self.on = False


    def set(self, val):
        if(val):
            self.on = True
            self.led.on()
        else:
            self.off = False
            self.led.off()

    def get(self):
        return self.on

    def cb(self, e):
        if(self.on):
            self.on = False
            self.led.off()
        else:
            self.on = True
            self.led.on()

# A row of LEDs to keep time with and some labels
class LEDStrip(UIElement): 
    def __init__(self):
        super().__init__() 
        self.leds= []
        self.group.set_size((DrumSwitch.button_width+12)*16+175+100,DrumSwitch.button_height+65)
        for i in range(16):
            l = lv.led(self.group)
            l.set_size(10,10)
            l.set_brightness(150)
            if(i%4==0):
                l.set_color(pal_to_lv(DrumSwitch.led_on_color))
            else:
                l.set_color(pal_to_lv(92))                
            l.off()
            if i==0:
                l.align_to(self.group, lv.ALIGN.LEFT_MID,105,0)
            else:
                l.align_to(self.leds[i-1], lv.ALIGN.OUT_RIGHT_MID, DrumSwitch.button_width, 0)
            self.leds.append(l)
        descs = lv.label(self.group)
        descs.set_text("Vol             Pitch            Pan")
        descs.align_to(self.leds[-1], lv.ALIGN.OUT_RIGHT_MID, 45, 0)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)

    def set(self, idx, val):
        if(val):
            self.leds[idx].on()
        else:
            self.leds[idx].off()



# A row of drum switches with a dropdown on the left for tone
# and maybe volume or other knob on right idk 
class DrumRow(UIElement):
    knob_color = 209
    def __init__(self, items, midi_note=0):
        super().__init__()
        self.midi_note = midi_note
        self.preset = 0
        self.vel = 1.0
        self.pitch = 60
        self.pan = 0.5
        self.objs = []
        # Droodown on left
        self.group.set_size((DrumSwitch.button_width+12)*16+175+100,DrumSwitch.button_height+65)
        self.dropdown = lv.dropdown(self.group)
        self.dropdown.set_options("\n".join(items))
        self.dropdown.set_dir(lv.DIR.BOTTOM)
        self.dropdown.add_event_cb(self.dropdown_cb, lv.EVENT.VALUE_CHANGED, None)
        self.dropdown.set_height(40)
        self.dropdown.set_width(100)
        self.dropdown.align_to(self.group, lv.ALIGN.LEFT_MID,0,0)
        lv_depad(self.dropdown)
        for i in range(4):
            for j in range(4):
                d = DrumSwitch(switch_color_idx=i)
                self.objs.append(d)
                d.group.set_parent(self.group)
                d.group.set_style_bg_color(pal_to_lv(UIScreen.default_bg_color),lv.PART.MAIN)
                dc = len(self.objs)-1
                if(dc==0):
                    d.group.align_to(self.dropdown, lv.ALIGN.OUT_RIGHT_MID,10,0)
                else:
                    d.group.align_to(self.objs[dc-1].group, lv.ALIGN.OUT_RIGHT_MID,0,0)
                d.group.remove_flag(lv.obj.FLAG.SCROLLABLE)
        # volume / vel knob
        self.knobs = []
        for i in range(3):
            k = lv.arc(self.group)
            k.set_size(50,50)
            k.set_style_pad_hor(0,lv.PART.KNOB)
            k.set_style_pad_ver(0,lv.PART.KNOB)
            if(i==2): # pan is special color white/red
                k.set_style_bg_color(pal_to_lv(255), lv.PART.KNOB)
                k.set_style_arc_color(pal_to_lv(224), lv.PART.INDICATOR)
            else: # one color
                k.set_style_bg_color(pal_to_lv(DrumRow.knob_color), lv.PART.KNOB)
                k.set_style_arc_color(pal_to_lv(DrumRow.knob_color), lv.PART.INDICATOR)
            if(i==0):
                k.align_to(self.objs[-1].group, lv.ALIGN.OUT_RIGHT_MID,15,0)
            else:
                k.align_to(self.knobs[i-1], lv.ALIGN.OUT_RIGHT_MID,15,0)
            cb = [self.vel_cb, self.pitch_cb, self.pan_cb][i]
            k.add_event_cb(cb, lv.EVENT.VALUE_CHANGED, None)
            self.knobs.append(k)

        lv_depad(self.group)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)

    def get(self, idx):
        return self.objs[idx].get()

    def set_vel(self, val):
        self.knobs[0].set_value(int(val*100.0))
        self.vel = val

    def get_vel(self):
        return self.knobs[0].get_value()/100.0

    def set_pitch(self, val):
        self.knobs[1].set_value(int(val*100.0))
        self.pitch = val
        self.update_note()

    def get_pitch(self):
        return self.knobs[1].get_value()/100.0

    def set_pan(self, val):
        self.knobs[2].set_value(int(val*100.0))
        self.pan = val
        self.update_note()

    def get_pan(self):
        return self.knobs[2].get_value()/100.0

    def get_preset(self):
        #return self.dropdown.get_selected()
        return self.preset

    def set_preset(self, idx):
        self.dropdown.set_selected(idx)
        self.preset = idx
        self.update_note()

    def update_note(self):
        base_note = drumkit[self.preset][0]
        note_for_pitch = int(base_note + (self.pitch - 0.5)*24.0)
        params_dict={
            'wave': amy.PCM,
            'patch': self.preset,
            'freq': 0,
            'note': note_for_pitch,
            'pan': self.pan,
        }
        app.synth.setup_midi_note(midi_note=self.midi_note, params_dict=params_dict)

    def vel_cb(self, e):
        self.vel = e.get_target_obj().get_value() / 100.0

    def pitch_cb(self, e):
        self.pitch = e.get_target_obj().get_value() / 100.0
        self.update_note()

    def pan_cb(self, e):
        self.pan = e.get_target_obj().get_value() / 100.0
        self.update_note()

    def dropdown_cb(self, e):
        self.preset = e.get_target_obj().get_selected()
        self.update_note()


# Called from tulip's sequencer
def beat_callback(t):
    global app
    # Hm, how should we deal with visual latency 
    app.leds.set((app.current_beat-1)% 16, 0)
    app.leds.set(app.current_beat, 1)
    #print("beat %d at time %d sysclock %d delta %d tick %d" % (app.current_beat, t, ticks_ms(), t-ticks_ms(), seq_ticks()))
    for i, row in enumerate(app.rows):
        if(row.get(app.current_beat)):
            base_note = drumkit[row.get_preset()][0]
            note_for_pitch = None
            if row.get_pitch() != 0.5:
                note_for_pitch = int(base_note + (row.get_pitch() - 0.5)*24.0)
            #app.synth.note_on(note_for_pitch, row.get_vel()*2, pcm_patch=row.get_preset(), pan=row.get_pan(), time=t)
            app.synth.note_on(note=row.midi_note, velocity=row.get_vel()*2, time=t)
            #amy.send(osc=50+i+2, wave=amy.PCM, patch=row.get_preset(), note=note_for_pitch, vel=row.get_vel()*2, pan=row.get_pan(), time=t)
    app.current_beat = (app.current_beat+1) % 16

def quit(screen):
    seq_remove_callback(screen.slot)

def run(screen):
    global app 
    app = screen # we can use the screen obj passed in as a general "store stuff here" class, as well as inspect the UI 
    app.synth = midi.config.synth_per_channel[10]
    app.current_beat = 0
    app.offset_y = 10
    app.set_bg_color(0)
    app.quit_callback = quit
    app.leds = LEDStrip()
    app.add(app.leds, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    app.rows = [DrumRow([x[1] for x in drumkit], midi_note=i) for i in range(7)]
    app.add(app.rows, direction=lv.ALIGN.OUT_BOTTOM_LEFT)

    for i,row in enumerate(app.rows):
        row.set_preset([1, 2, 0, 6, 7, 9, 10][i])
        row.set_vel(.5)
        row.set_pan(.5)
        row.set_pitch(.5)
    for i in range(16):
        app.rows[2].objs[i].set(True)

    app.slot = seq_add_callback(beat_callback, int(seq_ppq()/2))
    app.present()


