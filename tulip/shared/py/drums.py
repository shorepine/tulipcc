# drums.py
# lvgl drum machine for Tulip

from tulip import UIScreen, UIElement, pal_to_lv, lv_depad, lv, frame_callback, ticks_ms, seq_ticks
import amy
import sequencer
from patches import drumkit

# Use AMY synths starting at this number (avoid MIDI channels and system synth)
_BASE_SYNTH = 20

# A single drum machine switch with LED
class DrumSwitch(UIElement):
    led_off_color = 255
    led_on_color = 224
    # R O Y W 808 colors
    switch_colors = [128,208,248,255]
    button_width = 30
    button_height = 40

    def __init__(self, row, col, synth, switch_color_idx=0):
        super().__init__() 
        self.row = row
        self.col = col
        self.synth = synth
        self.sequencer_event = None
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

    def update_sequencer(self):
        """Update the sequencer entry when switch is turned on or off. Also has to be done when row voice changes, because _NOTE_OF_ROW changes."""
        row = app.rows[self.row]
        if(self.on):
            kwargs = {'position': self.col, 'func': amy.send, 'args': [], 'synth': self.synth, 'note': _NOTE_OF_ROW[self.row], 'vel': 1.0}
            if self.sequencer_event is None:
                self.sequencer_event = app.drum_seq.add(**kwargs)
            else:
                self.sequencer_event.update(**kwargs)
            length = int((amy.AMY_SEQUENCER_PPQ/2) * 16) 
            offset = int(amy.AMY_SEQUENCER_PPQ/2) * self.col
        else:
            if self.sequencer_event is not None:
                self.sequencer_event.remove()
            self.sequencer_event = None

    def set(self, val):
        if(val):
            self.on = True
            self.led.on()
        else:
            self.on = False
            self.led.off()
        self.update_sequencer()

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
    def __init__(self, items, row, synth=None):
        super().__init__()
        self.preset = ''
        self.vel = 0.5
        self.pitch = 0.5
        self.pan = 0.5
        self.row = row
        self.objs = []
        self.items = items
        self.synth = _BASE_SYNTH + self.row if synth is None else synth
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
                d = DrumSwitch(switch_color_idx=i, row=row, col = j + i*4, synth=self.synth)
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
        self.update_synth()

    def get_vel(self):
        return self.knobs[0].get_value()/100.0

    def set_pitch(self, val):
        self.knobs[1].set_value(int(val*100.0))
        self.pitch = val
        self.update_synth()

    def get_pitch(self):
        return self.knobs[1].get_value()/100.0

    def set_pan(self, val):
        self.knobs[2].set_value(int(val*100.0))
        self.pan = val
        self.update_synth()

    def get_pan(self):
        return self.knobs[2].get_value()/100.0

    def get_preset(self):
        #return self.dropdown.get_selected()
        return self.preset

    def set_preset(self, name):
        idx = self.items.index(name)
        self.dropdown.set_selected(idx)
        self.preset = name
        self.update_synth(name)
        self.update_switches()

    def update_synth(self, name=None):
        if name is not None:
            # Potentially new drum voice.
            drumkit_index = drumkit_index_of_name(name)
            amy.send(synth=self.synth, preset=drumkit_index)
            _NOTE_OF_ROW[self.row] = drumkit[drumkit_index][0]
        amy.send(
            synth=self.synth,
            amp=0.001 + self.vel * 2,  # amp_coef of 0 is ignored.
            freq=amy.ZERO_LOGFREQ_IN_HZ * (2 ** (4 * (self.pitch - 0.5))),  # -2..+2 octaves
            pan=self.pan
        )

    def update_switches(self):
        """For each on switch in the row, we have to update the sequencer with the new midi base_note."""
        for switch in self.objs:
            switch.update_sequencer()

    def vel_cb(self, e):
        self.vel = e.get_target_obj().get_value() / 100.0
        self.update_synth()

    def pitch_cb(self, e):
        self.pitch = e.get_target_obj().get_value() / 100.0
        self.update_synth()

    def pan_cb(self, e):
        self.pan = e.get_target_obj().get_value() / 100.0
        self.update_synth()

    def dropdown_cb(self, e):
        name = self.items[e.get_target_obj().get_selected()]
        self.set_preset(name)


# Called from AMY's sequencer, just updates the LEDs
def beat_callback(t):
    global app
    app.current_beat = int((t / 24) % 16)

    app.leds.set((app.current_beat-1)% 16, 0)
    app.leds.set(app.current_beat, 1)

def quit(screen):
    # Clear all the sequenced stuff
    screen.drum_seq.clear()
    screen.led_seq.clear()


def drumkit_index_of_name(name):
    global drumkit
    return [d[1] for d in drumkit].index(name)


_NUM_ROWS = 7

def run(screen):
    global app 
    global _NOTE_OF_ROW
    app = screen # we can use the screen obj passed in as a general "store stuff here" class, as well as inspect the UI 

    # Drum machine sequencer - plays notes
    app.drum_seq = sequencer.AMYSequence(16, 8)

    app.offset_y = 10
    app.set_bg_color(0)
    app.quit_callback = quit
    app.leds = LEDStrip()
    app.add(app.leds, direction=lv.ALIGN.OUT_BOTTOM_LEFT, pad_y=0)
    drum_items = [x[1] for x in drumkit]
    _NOTE_OF_ROW = [0] * _NUM_ROWS
    initial_voices = ['Std Kick', 'Snare', 'Maraca', 'Closed Hat', 'Open Hat', 'Cowbell', 'Congo Low']
    app.rows = []
    for i in range(_NUM_ROWS):
        row = DrumRow(drum_items, row=i)
        app.rows.append(row)
        drumkit_index = drumkit_index_of_name(initial_voices[i])
        _NOTE_OF_ROW[i] = drumkit[drumkit_index][0]  # drumkit entries are (base_note, name, general_midi_note_if_any).
        amy.send(synth=_BASE_SYNTH + i, num_voices=1, patch=amy.message(wave=amy.PCM, preset=drumkit_index))
        row.set_preset(initial_voices[i])
        row.set_vel(.5)
        row.set_pan(.5)
        row.set_pitch(.5)
    app.add(app.rows, direction=lv.ALIGN.OUT_BOTTOM_LEFT)
    # Have some notes present as default.
    for i in range(16):
        app.rows[2].objs[i].set(True)

    app.current_beat = int((seq_ticks() / 24) % 16)
    # Graphical update sequencer -- moves the LEDs
    app.led_seq = sequencer.TulipSequence(8, beat_callback)
    app.present()


