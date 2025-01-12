# enc_fun.py
# fun with encoders, based on drums.py

from tulip import UIScreen, UIElement, pal_to_lv, lv_depad, lv, frame_callback, ticks_ms, seq_add_callback, seq_remove_callback, seq_ticks
import amy
import midi
from patches import drumkit
import m5_8encoder as encoder
AMY_TAG_OFFSET = 1716 # random offset to allow other apps to share


class EncSlider(tulip.UIElement):
    """Builds a slider with description on top and a ganged label on bottom showing the value."""
    
    handle_color = 0
    bar_color = 68
    handle_radius = 0
    handle_v_pad = 1
    handle_h_pad  = 5
    width = 15
    height = 130
    # The height / width of the slider unit (+ label and value) is slightly bigger than the slider
    total_height = height + 85
    total_width = width + 25
    font = lv.font_unscii_8

    def __init__(self, name, callback=None):
        super().__init__()
        self.name = name
        self.callback = callback

        self.desc = lv.label(self.group)
        self.desc.set_text(name)
        self.desc.set_style_text_font(JunoSlider.font, 0)
        self.desc.align(lv.ALIGN.TOP_MID, 0, 0)

        self.slider = lv.slider(self.group)
        self.slider.set_range(0, 127)
        self.slider.set_style_bg_opa(lv.OPA.COVER, lv.PART.MAIN)
        self.slider.set_width(JunoSlider.width)
        self.slider.set_height(JunoSlider.height)
        self.slider.set_style_bg_color(tulip.pal_to_lv(JunoSlider.bar_color), lv.PART.INDICATOR)
        self.slider.set_style_bg_color(tulip.pal_to_lv(JunoSlider.bar_color), lv.PART.MAIN)
        self.slider.set_style_bg_color(tulip.pal_to_lv(JunoSlider.handle_color), lv.PART.KNOB)
        self.slider.set_style_radius(JunoSlider.handle_radius, lv.PART.KNOB)
        self.slider.set_style_pad_ver(JunoSlider.handle_v_pad, lv.PART.KNOB)
        self.slider.set_style_pad_hor(JunoSlider.handle_h_pad, lv.PART.KNOB)
        self.slider.center()

        self.label = lv.label(self.group)
        self.label.set_text('0')
        self.label.set_style_text_font(JunoSlider.font, 0)
        self.slider.add_event_cb(self.cb, lv.EVENT.VALUE_CHANGED, None)

        self.group.set_width(JunoSlider.total_width)
        self.group.set_height(JunoSlider.total_height)

        self.label.align_to(self.slider, lv.ALIGN.OUT_BOTTOM_MID, 0, 10)
        self.slider.align_to(self.desc, lv.ALIGN.OUT_BOTTOM_MID, 0, 10)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)


    def get(self):
        return self.slider.get_value()

    def update_label(self):
        self.label.set_text("{:d}".format(self.slider.get_value()))
        self.label.align_to(self.slider, lv.ALIGN.OUT_BOTTOM_MID, 0, 10)
        self.group.set_height(JunoSlider.total_height)
    
    def set(self, v):
        self.slider.set_value(v, lv.ANIM.OFF)
        # Update label and propagate to callback.
        self.cb(None)
        
    def cb(self, e):
        self.update_label()
        if self.callback:
            self.callback(self.slider.get_value() / 127.0)


# A single drum machine switch with LED
class DrumSwitch(UIElement):
    led_off_color = 255
    led_on_color = 224
    # R O Y W 808 colors
    switch_colors = [128,208,248,255]
    button_width = 30
    button_height = 40

    def __init__(self, row, col, switch_color_idx=0):
        super().__init__() 
        self.row = row
        self.col = col
        self.tag = (self.row * 16) + self.col + AMY_TAG_OFFSET
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

    def update_amy(self):
        row = app.rows[self.row]
        if(self.on):
            length = int((amy.SEQUENCER_PPQ/2) * 16) 
            offset = int(amy.SEQUENCER_PPQ/2) * self.col
            app.synth.note_on(note=row.midi_note, sequence = "%d,%d,%d" % (offset, length, self.tag))
        else:
            # Turn off note, the rest of stuff doesn't matter
            amy.send(sequence = ",,%d" %  (self.tag))

    def set(self, val):
        if(val):
            self.on = True
            self.led.on()
        else:
            self.on = False
            self.led.off()
        self.update_amy()

    def get(self):
        return self.on

    def cb(self, e):
        if(self.on):
            self.set(False)
        else:
            self.set(True)

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
# and some knobs on the right for pitch / vol / pan
class DrumRow(UIElement):
    knob_color = 209
    def __init__(self, items, row, midi_note=0):
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
                d = DrumSwitch(switch_color_idx=i, row=row, col = j + i*4)
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
        self.update_note()


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
            'amp': self.vel*2.0,
            'wave': amy.PCM,
            'patch': self.preset,
            'freq': 0,
            'note': note_for_pitch,
            'pan': self.pan,
        }
        app.synth.setup_midi_note(midi_note=self.midi_note, params_dict=params_dict)

        # For each on switch in the row, we have to update the sequence with the new params
        for switch in self.objs:
            switch.update_amy()


    def vel_cb(self, e):
        self.vel = e.get_target_obj().get_value() / 100.0
        self.update_note()

    def pitch_cb(self, e):
        self.pitch = e.get_target_obj().get_value() / 100.0
        self.update_note()

    def pan_cb(self, e):
        self.pan = e.get_target_obj().get_value() / 100.0
        self.update_note()

    def dropdown_cb(self, e):
        self.preset = e.get_target_obj().get_selected()
        self.update_note()


# Called from AMY's sequencer, 
# updates the LEDs
# reads encoders

def beat_callback(t):
    global app
    app.current_beat = int((seq_ticks() / 24) % 16)

    app.leds.set((app.current_beat-1)% 16, 0)
    app.leds.set(app.current_beat, 1)

    # Read encoders
    
    ctr = encoder.read_counter(0)


def quit(screen):
    seq_remove_callback(screen.slot)
    # Clear all the sequenced tags -- even if we never set them, it's a no-op if so 
    for i in range(16*7):
        amy.send(sequence=",,%d" % (i+AMY_TAG_OFFSET))


def run(screen):
    global app 
    app = screen # we can use the screen obj passed in as a general "store stuff here" class, as well as inspect the UI 
    app.synth = midi.config.synth_per_channel[10]
    app.offset_y = 10
    app.set_bg_color(0)
    app.quit_callback = quit
    app.leds = LEDStrip()
    app.add(app.leds, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    #app.rows = [DrumRow([x[1] for x in drumkit], i, midi_note=i) for i in range(7)]
    app.add(app.rows, direction=lv.ALIGN.OUT_BOTTOM_LEFT)
    for i,row in enumerate(app.rows):
        row.set_preset([1, 2, 0, 6, 7, 9, 10][i])
        row.set_vel(.5)
        row.set_pan(.5)
        row.set_pitch(.5)
    for i in range(16):
        app.rows[2].objs[i].set(True)

    app.current_beat = int((seq_ticks() / 24) % 16)
    app.slot = seq_add_callback(beat_callback, int(amy.SEQUENCER_PPQ/2))
    app.present()


