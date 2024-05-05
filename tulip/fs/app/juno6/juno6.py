# juno6.py
# A more pure-LVGL (using Tulip's UIScreen) UI for Juno-6
from tulip import UIScreen, UIElement, pal_to_lv, lv_depad, lv, midi_in, midi_add_callback, midi_remove_callback, seq_ppq, seq_add_callback, seq_remove_callback, music_map
import time


class JunoSection(UIElement):
    """A group of elements in an red-header group with a title."""
    
    header_color = 224
    bg_color = 73
    header_font = lv.font_tulip_14
    text_color = 255
    section_gap = 10
    
    def __init__(self, name, elements=None, header_color=None):
        super().__init__() 
        self.objs = {}
        self.last_obj_added = None
        self.name = name
        self.max_height = 0
        if header_color is not None:
            self.header_color = header_color
        self.header = lv.label(self.group)
        self.header.set_text(name)
        self.header.set_style_text_font(self.header_font, lv.PART.MAIN)
        self.header.set_style_bg_color(pal_to_lv(self.header_color), lv.PART.MAIN)
        self.header.set_style_text_color(pal_to_lv(self.text_color), lv.PART.MAIN)
        self.header.set_style_bg_opa(lv.OPA.COVER, lv.PART.MAIN)
        self.header.align_to(self.group, lv.ALIGN.TOP_LEFT, 0, 0)
        self.header.set_style_text_align(lv.TEXT_ALIGN.CENTER,0)

        if elements:
            self.add(elements)

    def add(self, objs, direction=lv.ALIGN.OUT_RIGHT_MID):
        """Add elements (all at once) to the section."""
        total_width = 0
        for obj in objs:
            self.objs[obj.name] = obj
            obj.group.set_parent(self.group)
            if self.last_obj_added:
                obj.group.align_to(self.last_obj_added, direction, 0, 0)
            else:
                obj.group.align_to(self.header, lv.ALIGN.OUT_BOTTOM_LEFT, 0, 0)
            obj.group.set_style_bg_color(pal_to_lv(self.bg_color), lv.PART.MAIN)

            self.last_obj_added = obj.group
            total_width += obj.group.get_width() 

        self.header.set_width(total_width)
        lv_depad(self.header)

        self.group.set_width(total_width + self.section_gap)
        lv_depad(self.group)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)


class JunoButtons(UIElement):
    """A set of buttons/checkboxes, one under another."""

    font = lv.font_unscii_8
    width = 37

    def __init__(self, name, button_labels, callbacks=None, rounded=False):
        super().__init__()
        self.name = name
        self.button_labels = button_labels
        self.callbacks = callbacks
        self.checkboxes = []
        self.state = []
        self.group.set_width(self.width)
        self.group.set_height(JunoSlider.total_height)
        for button_label in button_labels:
            text = lv.label(self.group)
            text.set_text(button_label)
            text.set_style_text_font(self.font, 0)
            if self.checkboxes:
                text.align_to(self.checkboxes[-1], lv.ALIGN.OUT_BOTTOM_MID, 0, 10)
            else:
                text.align_to(self.group, lv.ALIGN.TOP_MID, 0, 0)
            checkbox = lv.checkbox(self.group)
            checkbox.add_event_cb(self.callback, lv.EVENT.VALUE_CHANGED, None)
            if rounded:
                checkbox.set_style_radius(100, lv.PART.INDICATOR)
            checkbox.set_ext_click_area(15)  # make the label count too 
            checkbox.align_to(text, lv.ALIGN.OUT_BOTTOM_MID, 0, 0)
            self.checkboxes.append(checkbox)
            self.state.append(False)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)

    
    def callback(self, e):
        obj = e.get_target_obj()
        idx = (obj.get_index() - 1)//2
        self.update_state(idx, not self.state[idx])

    def update_state(self, idx, val):
        self.state[idx] = val
        self.checkboxes[idx].set_state(lv.STATE.CHECKED, val)
        if self.callbacks and self.callbacks[idx]:
            self.callbacks[idx](val)
        
    def set(self, button_text, val):
        idx = self.button_labels.index(button_text)
        self.update_state(idx, val)
        
    def get(self, button_text):
        idx = self.button_labels.index(button_text)
        return self.state[idx]


class JunoRadioButtons(JunoButtons):
    """A set of buttons with 'radio' behavior."""

    def __init__(self, name, button_labels, callbacks=None):
        super().__init__(name, button_labels, callbacks, rounded=True)
        self.state = [False for _ in button_labels]
        # Set the first radio button
        self.current_idx = 0
        self.set(self.button_labels[self.current_idx])

    def set(self, button_label):
        idx = self.button_labels.index(button_label)
        self.update_state(self.current_idx, False)
        self.current_idx = idx
        self.update_state(self.current_idx, True)
            
    def get(self):
        return self.button_labels[self.current_idx]

    def callback(self, e):
        obj = e.get_target_obj()
        # The callback gets indexes both for the buttons and the labels. So do some math.
        idx = (obj.get_index() - 1) // 2
        self.set(self.button_labels[idx])

    def next(self, step=1):
        """Simulate selecting the next item."""
        idx = (self.current_idx + step) % len(self.button_labels)
        self.set(self.button_labels[idx])

    def prev(self):
        """Simulate selecting the previous item."""
        self.next(step=-1)


class JunoSlider(UIElement):
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
        self.slider.set_style_bg_color(pal_to_lv(JunoSlider.bar_color), lv.PART.INDICATOR)
        self.slider.set_style_bg_color(pal_to_lv(JunoSlider.bar_color), lv.PART.MAIN)
        self.slider.set_style_bg_color(pal_to_lv(JunoSlider.handle_color), lv.PART.KNOB)
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

class JunoControlledLabel(UIElement):
    """A label with some press-to-act buttons (e.g. + and -)."""
    button_size = 28
    button_space = 4
    font = lv.font_unscii_8
  
    def __init__(self, name, button_labels, callbacks, text, width=240, height=40):
        super().__init__()
        self.name = name
        self.button_labels = button_labels
        self.callbacks = callbacks
        self.buttons = []
        self.group.set_width(width)
        self.group.set_height(height)
        for button_label in button_labels:
            button = lv.button(self.group)
            button.set_size(self.button_size, self.button_size)
            if self.buttons:
                button.align_to(self.buttons[-1], lv.ALIGN.OUT_RIGHT_MID, 5, 0)
            else:
                button.align_to(self.group, lv.ALIGN.TOP_LEFT, 0, 0)
            button.add_event_cb(self.callback, lv.EVENT.PRESSED, None)
            label = lv.label(button)
            label.set_text(button_label)
            label.center()
            self.buttons.append(button)
        self.label_obj = lv.label(self.group)
        self.set_text(text)
        self.label_obj.align_to(self.buttons[-1], lv.ALIGN.OUT_RIGHT_MID, 5, 0)
        self.group.remove_flag(lv.obj.FLAG.SCROLLABLE)

    def set_text(self, text):
        self.text = text  
        self.label_obj.set_text(text)

    def get(self, idx):
        return self.text

    def callback_by_idx(self, idx):
        if self.callbacks and self.callbacks[idx]:
            self.callbacks[idx]()      

    def callback(self, e):
        self.callback_by_idx(e.get_target_obj().get_index())
    
    def press(self, button_text):
        """Simulate a button press by its label."""
        self.callback_by_idx(self.button_labels.index(button_text))

class JunoTokenSpinbox(JunoControlledLabel):
    """A token spinbox has up and down buttons, but the string is a token, not a numeric value."""
    value = 0
    min_value = 0
    max_value = 127
    set_fn = None

    def __init__(self, name, set_fn, initial_text='', min_value=0, max_value=127, initial_value=0, offset=0, **kwargs):
        self.set_fn = set_fn  # called when value changes, returns text to display.
        self.offset = offset
        self.min_value = min_value
        self.max_value = max_value
        super().__init__(name + 'TokenSpinbox', ['-', '+'],
            [self.value_down, self.value_up],
            initial_text, **kwargs)
        self.set_value(initial_value)

    def value_delta(self, increment=1):
        value_range = self.max_value - self.min_value + 1
        self.value = self.min_value + ((self.value - self.min_value + increment) % value_range)
        self.set_value(self.value)

    def value_up(self):
        self.value_delta(1)

    def value_down(self):
        self.value_delta(-1)

    def set_value(self, value):
        self.value = value
        self.name = self.set_fn(self.value+self.offset)
        self.set_text(self.name)



import juno, midi

midi_channel = 1
juno_patch_for_midi_channel = {}

# Get the patch to channel mapping and the AMY voices from midi.patch_map if set
for m_channel in range(1, 17):
    if m_channel != 10:
        patch_num, amy_voices = midi.config.channel_info(m_channel)
        if patch_num is not None and patch_num < 128:
            jp = juno.JunoPatch()  # .from_patch_number(patch_num)
            jp.set_voices(amy_voices)
            jp.set_patch(patch_num)
            juno_patch_for_midi_channel[m_channel] = jp

def current_juno():
    global midi_channel
    jp = juno_patch_for_midi_channel.get(midi_channel, None)
    if jp is not None:
        #print("patch number for channel %d is %d" %(midi_channel, jp.patch_number) )
        return juno_patch_for_midi_channel.get(midi_channel, None)
    return None



current_juno().init_AMY()


#current_juno().init_AMY()

# Make the callback function.
def jcb(arg):
    callback = lambda x: current_juno().set_param(arg, x)
    return callback

def hpf(n):
    callback = lambda x: current_juno().set_param('hpf', n) if x else None
    return callback

def cho(n):
    callback = lambda x: current_juno().set_param('chorus', n) if x else None
    return callback

lfo = JunoSection("LFO", [lfo_rate := JunoSlider("Rate", jcb('lfo_rate')),
                          lfo_delay_time := JunoSlider("Delay", jcb('lfo_delay_time'))])

dco_range = JunoRadioButtons("Range", ["4'", "8'", "16'"],
                                  [jcb('stop_4'), jcb('stop_8'), jcb('stop_16')])
dco_lfo = JunoSlider("LFO", jcb('dco_lfo'))

dco_pwm = JunoSlider("PWM", jcb('dco_pwm'))
dco_pwm_mode = JunoRadioButtons("PWM", ["LFO", "Man"], [None, jcb('pwm_manual')])
dco_wave = JunoButtons("Wave", ["Pls", "Saw"], [jcb('pulse'), jcb('saw')])
dco_sub = JunoSlider("Sub", jcb('dco_sub'))
dco_noise = JunoSlider("Noise", jcb('dco_noise'))
dco = JunoSection("DCO", [dco_range, dco_lfo, dco_pwm, dco_pwm_mode, dco_wave, dco_sub, dco_noise])

hpf = JunoSection("HPF", [hpf_freq := JunoRadioButtons("Freq", ["3", "2", "1", "0"],
                                                       [hpf(3), hpf(2), hpf(1), hpf(0)])])

vcf = JunoSection("VCF", [
    vcf_freq := JunoSlider("Freq", jcb('vcf_freq')), 
    vcf_res := JunoSlider("Res", jcb('vcf_res')),
    vcf_pol := JunoRadioButtons("Pol", ["Pos", "Neg"], [None, jcb('vcf_neg')]),
    vcf_env := JunoSlider("Env", jcb('vcf_env')),
    vcf_lfo := JunoSlider("LFO", jcb('vcf_lfo')),
    vcf_kbd := JunoSlider("Kybd", jcb('vcf_kbd'))
])

vca = JunoSection("VCA", [
    vca_mode := JunoRadioButtons("Mode", ["Env", "Gate"], [None, jcb('vca_gate')]),
    vca_level := JunoSlider("Level", jcb('vca_level'))
])

env = JunoSection('ENV', [
    env_a := JunoSlider("A", jcb('env_a')),
    env_d := JunoSlider("D", jcb('env_d')),
    env_s := JunoSlider("S", jcb('env_s')),
    env_r := JunoSlider("R", jcb('env_r'))
])

ch = JunoSection("CH", [chorus_mode := JunoRadioButtons("Mode", ["Off", "I", "II", "III"],
                                                        [cho(0), cho(1), cho(2), cho(3)])])

def setup_from_patch(patch):
    """Make the UI match the values in a JunoPatch."""
    current_juno().defer_param_updates = True
    glob_fns = globals()
    for el in ['lfo_rate', 'lfo_delay_time',
             'dco_lfo', 'dco_pwm', 'dco_sub', 'dco_noise',
             'vcf_freq', 'vcf_res', 'vcf_env', 'vcf_lfo', 'vcf_kbd',
             'vca_level', 'env_a', 'env_d', 'env_s', 'env_r']:
        glob_fns[el].set(int(round(127 * getattr(patch, el))))

    dco_range.set("4'" if patch.stop_4 else "16'" if patch.stop_16 else "8'")
    dco_pwm_mode.set('Man' if patch.pwm_manual else 'LFO')
    dco_wave.set('Pls', patch.pulse)
    dco_wave.set('Saw', patch.saw)
    hpf_freq.set(str(patch.hpf))
    vcf_pol.set('Neg' if patch.vcf_neg else 'Pos')
    vca_mode.set('Gate' if patch.vca_gate else 'Env')
    chorus_mode.set(['Off', 'I', 'II', 'III'][patch.chorus])

    current_juno().send_deferred_params()

    return patch.name



def setup_from_patch_number(patch_number):
    global midi_channel
    #print("resetting patch number to %d" % (patch_number))

    music_map(midi_channel, patch_number, 4)
    _, amy_voices = midi.config.channel_info(midi_channel)
    jp = juno.JunoPatch()  #.from_patch_number(patch_number)
    jp.set_voices(amy_voices)
    jp.set_patch(patch_number)
    juno_patch_for_midi_channel[midi_channel] = jp

    #current_juno().patch_number = patch_number
    #current_juno().name = setup_from_patch(jp)
    return current_juno().name

def setup_from_midi_chan(new_midi_channel):
    """Switch which JunoPatch we display based on MIDI channel."""
    global midi_channel
    #print("setup chan %d" % (new_midi_channel))
    midi_channel = (new_midi_channel)
    new_patch = current_juno()
    if(new_patch == None):
        patch_selector.set_text("None assigned")
        patch_selector.value = -1 
    else:
        #print("new patch patch is %d" % (new_patch.patch_number))
        new_patch.init_AMY()
        patch_selector.value = new_patch.patch_number  # Bypass actually reading that patch, just set the state.
        patch_selector.set_text(new_patch.name)
        setup_from_patch(new_patch)
    return "MIDI chan %d" % (midi_channel)

patch_selector = JunoTokenSpinbox('Patch', set_fn=setup_from_patch_number, initial_value=current_juno().patch_number)
midi_selector = JunoTokenSpinbox('MIDI', set_fn=setup_from_midi_chan, max_value=15, width=160, offset=1)




# Wire up MIDI controls
# Oxygen49 slider IDs, starting from left.
SLIDER_IDS = [0x49, 0x4b, 0x48, 0x4a, 0x4f, 0x54, 0x5b, 0x5d, 0x7]
#SLIDER_IDS = [74, 71, 91, 93, 73, 72, 5, 84, 7]
# Oxygen49 knobs, top row then second row.
KNOB_IDS = [0x10, 0x11, 0x12, 0x0a, 0x13, 0x50, 0x51, 0x14]
#KNOB_IDS = [75, 76, 92, 95, 10, 77, 78, 79]
# Oxygen49 buttons.  They toggle between 0 and 0x7f.
BUTTON_IDS = [0x18, 0x19, 0x1a, 0x1b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
              0x71, 0x72, 0x73, 0x74, 0x75, 0x76]
# I had to program these.  See Oxygen49-UserGuide-v1.3.pdf + notes.
#BUTTON_IDS = [96, 97, 98, 99, 100, 101, 102, 103, 104,   113, 114, 115, 116, 117, 118]
PITCH_WHEEL = 0   # Pitch wheel is a special case, hard-coded in juno.py.
MOD_WHEEL = 1

param_map = {
    KNOB_IDS[0]: 'lfo_rate',
    KNOB_IDS[1]: 'lfo_delay_time',
    #KNOB_IDS[2]: 'dco_lfo',
    MOD_WHEEL: 'dco_lfo',
    KNOB_IDS[3]: 'dco_pwm',
    SLIDER_IDS[0]: 'dco_sub',
    SLIDER_IDS[1]: 'dco_noise',
    SLIDER_IDS[2]: 'vcf_freq',
    SLIDER_IDS[3]: 'vcf_res',
    KNOB_IDS[4]: 'vcf_env',
    KNOB_IDS[5]: 'vcf_lfo',
    KNOB_IDS[6]: 'vcf_kbd',
    #
    KNOB_IDS[7]: 'arp_rate',
    #
    SLIDER_IDS[8]: 'vca_level',
    SLIDER_IDS[4]: 'env_a',
    SLIDER_IDS[5]: 'env_d',
    SLIDER_IDS[6]: 'env_s',
    SLIDER_IDS[7]: 'env_r',
    BUTTON_IDS[0]: 'dco_range',
    BUTTON_IDS[1]: 'dco_pwm_mode', 
    BUTTON_IDS[2]: 'Pls',
    BUTTON_IDS[3]: 'Saw',
    BUTTON_IDS[4]: 'hpf_freq',
    BUTTON_IDS[5]: 'vcf_pol',
    BUTTON_IDS[6]: 'vca_mode',
    BUTTON_IDS[7]: 'chorus_mode',
    #
    BUTTON_IDS[9]: 'On',
    BUTTON_IDS[10]: 'Hold',
    BUTTON_IDS[11]: 'arp_mode',
    BUTTON_IDS[12]: 'arp_rng',
}

def control_change(control, value):
    value = value / 127.0
    if control == 0:  # Pitch bend.
        current_juno().set_pitch_bend(2 * value - 1)
    if control in param_map:
        param_name = param_map[control]
        # Special cases.
        if param_name == 'Pls' or param_name == 'Saw':
            dco_wave.set(param_name, not dco_wave.get(param_name))
            return  # Early exit.
        if param_name == 'On' or param_name == 'Hold':
            arp_ctl.set(param_name, not arp_ctl.get(param_name))
            return  # Early exit.
        param_obj = globals()[param_name]
        if isinstance(param_obj, JunoRadioButtons):
            param_obj.next()  # value ignored.
            return
        param_obj.set(int(round(127 * value)))



def midi_event_cb(x):
    m = midi_in()   
    while m is not None and len(m) > 0:
        if m[0] == 0xb0:    # Other control slider.
            control_change(m[1], m[2])
        elif m[0] == 0xbf:
            # Special case for Oxygen49 transport buttons which send val 0x00 on release.
            if m[2] == 0x7f:
                control_change(m[1], m[2])
                
        # Are there more events waiting?
        m = m[3:]
        if len(m) == 0:
            m = midi_in_fn()


def quit(screen):
    midi_remove_callback(midi_event_cb)

def run(screen):
    screen.offset_y = 100
    screen.quit_callback = quit
    screen.set_bg_color(73)
    screen.add([lfo, dco, hpf, vcf, vca, env, ch])
    screen.add(midi_selector, relative=vcf, direction=lv.ALIGN.OUT_TOP_MID)
    screen.present()

    midi_add_callback(midi_event_cb)
