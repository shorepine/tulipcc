"""juno_ui: GUI for controlling Juno patches."""
import juno, tulip

registered_callbacks = {}

def register_callback(id_, callback):
  global registered_callback
  registered_callbacks[id_] = callback

def ui_callback(x):
  # x is the element ID that was triggered
  global registered_callback
  if x in registered_callbacks:
    registered_callbacks[x](x)
  else:
    print("Unrecognized element:", x)

tulip.ui_callback(ui_callback)


class IdFactory:
  id = 0

  @classmethod
  def next_id(cls):
    id = cls.id
    cls.id += 1
    return id


class UIBase:
  """Base class for UI elements, supports placing before drawing."""
  x = 0
  y = 0
  w = 100
  h = 100
  fg_color = tulip.color(100, 100, 100)
  bg_color = tulip.color(0, 0, 0)
  text_color = tulip.color(255, 255, 255)
  body_font = 8
  text_height = 12
  title_font = 5
  # Indicates that UI elements have been created.
  drawn = False
  
  def __init__(self, name="<none>"):
    self.name = name
    # Update w, h here or in place()

  def place(self, x, y):
    self.x = x
    self.y = y

  def draw(self):
    """Replace with actual."""
    tulip.bg_rect(self.x, self.y, self.w, self.h, self.fg_color, False)
    tulip.bg_str(self.title, self.x, self.y + self.text_height,
                 self.text_color, self.title_font, 
                 self.w, 2 * self.text_height)
    self.drawn = True


class Slider(UIBase):
  w_sli = 10
  y_sli = 12
  h_sli = 200
  y_txt = 0
  y_val = 235
  value = 0
  padx = 15
  w_leg = 26
  
  def __init__(self, name, callback=None):
    super().__init__(name)
    self.value_callback_fn = callback
    self.id_ = IdFactory.next_id()
    self.w = self.w_sli + 2 * self.padx
    self.h = self.y_val + 2 * self.text_height

  def draw(self):
    tulip.ui_slider(self.id_, self.value,
                    self.x + self.w_leg,
                    self.y + self.y_sli, self.w_sli, self.h_sli,
                    self.fg_color, self.bg_color)
    tulip.ui_active(self.id_, 1)
    register_callback(self.id_, self.callback)
    tulip.bg_str(self.name, self.x + self.w_leg - self.padx,
                 self.y + self.y_txt - self.text_height // 2,
                 self.text_color, self.body_font, 2 * self.padx, self.text_height)
    thumb_height = self.h_sli // 10
    # Slider legend.
    for i in range(11):
      tulip.bg_str(str(10 - i), self.x - self.padx + self.w_leg,
                   self.y + self.y_sli - (self.text_height - thumb_height) // 2 + (i * self.h_sli) // 11,
                   self.text_color, self.body_font, self.padx, self.text_height)
    self.drawn = True
    self.set_value(self.value)
    
  def set_value(self, v):
    self.value = v
    if (self.drawn):
      x = self.x + self.w_leg - self.padx
      y = self.y + self.y_val - self.text_height // 2
      w = 2 * self.padx
      h = self.text_height
      tulip.bg_rect(x, y, w, h, self.bg_color, True)
      tulip.bg_str("%d" % round(127 * self.value), x, y, self.text_color, self.body_font, w, h)
      tulip.ui_slider(self.id_, self.value)
    if self.value_callback_fn is not None:
      self.value_callback_fn(self.value)

  def callback(self, id_):
    self.set_value(tulip.ui_slider(id_))


class ControlledLabel(UIBase):
  """A label with some press-to-act buttons (e.g. + and -)."""
  button_size = 28
  button_space = 4
  
  def __init__(self, name, button_labels, callbacks, text, width=240, height=40):
    super().__init__(name)
    self.w = width
    self.h = height
    self.button_labels = button_labels
    self.callbacks = callbacks
    self.text = text
    self.ids = [IdFactory.next_id() for _ in self.button_labels]

  def draw(self):
    x = self.x
    y = self.y + (self.h - self.button_size) // 2
    w = self.button_size
    h = self.button_size
    dw = self.button_space
    for id_, tag in zip(self.ids, self.button_labels):
      tulip.ui_button(id_, tag, x, y, w, h, self.text_color, self.bg_color, False, self.body_font) 
      
      tulip.ui_active(id_, 1)
      self.ids.append(id_)
      register_callback(id_, self.callback)
      x = x + w + dw
    self.redraw_text()

  def redraw_text(self):
    # Label box
    y = self.y
    buttons_w = (
      len(self.button_labels) * (self.button_size + self.button_space)
    )
    x = self.x + buttons_w
    w = self.w - buttons_w
    h = self.h
    tulip.bg_rect(x, y, w, h, self.bg_color, True)
    tulip.bg_str(self.text, x, y, self.text_color, self.body_font, w, h)
    
  def set_text(self, text):
    self.text = text
    self.redraw_text()

  def callback(self, ui_id):
    # Dispatch to provided per-button callbacks
    for id_, callback in zip(self.ids, self.callbacks):
      if ui_id == id_:
        callback()

  def press(self, button_text):
    """Simulate a button press."""
    for label, callback in zip(self.button_labels, self.callbacks):
      if button_text == label:
        callback()


class ButtonSet(UIBase):
  y_top = 24
  y_txt = 0
  y_spacing = 44
  padx = 10
  button_w = 10
  text_height = 12

  def __init__(self, name, tags, callbacks=None, checkbox_style=0):
    super().__init__(name)
    if callbacks is None:
      callbacks = [None] * len(tags)
    self.value_callback_fns = {tag: callback for tag, callback in zip(tags, callbacks)}
    # Update geometry
    self.w = 2 * self.padx
    self.h = self.y_top + len(tags) * self.y_spacing
    # Set up state
    self.tags = tags
    self.checkbox_style = checkbox_style
    # Must allocate IDs during init so that self.set_value will work
    # before draw().
    self.ids = [IdFactory.next_id() for _ in self.tags]
    self.state = {}
    for tag in self.tags:
      self.state[tag] = False
      
  def current_button_index(self):
    for index, tag in enumerate(self.tags):
      if self.state[tag]:
        return index
    return -1

  def draw(self):
    x = self.x + self.padx
    y = self.y + self.y_txt
    tulip.bg_str(self.name, x - self.padx, y - self.text_height // 2,
                 self.text_color, self.body_font,
                 2 * self.padx, self.text_height)
    y = self.y + self.y_top
    for id_, tag in zip(self.ids, self.tags):
      tulip.bg_str(tag, x - self.padx, y - self.text_height // 2,
                   self.text_color, self.body_font,
                   2 * self.padx, self.text_height)
      y = y + self.text_height
      tulip.ui_checkbox(id_, self.state[tag],
                        x - self.button_w // 2, y, self.button_w,
                        self.fg_color, self.bg_color, self.checkbox_style)
      tulip.ui_active(id_, 1)
      self.ids.append(id_)
      register_callback(id_, self.callback)
      y = y + (self.y_spacing - self.text_height)

    self.drawn = True


class RadioButton(ButtonSet):

  def __init__(self, name, tags, callbacks):
    # checkbox_style: 0 is filled box, 1 is X, 2 is filled circle
    super().__init__(name, tags, callbacks, 2)
  
  def set_value(self, tag):
    for id_, button_tag in zip(self.ids, self.tags):
      if button_tag == tag:
        self.state[button_tag] = True
      else:
        self.state[button_tag] = False
      if self.drawn:
        tulip.ui_checkbox(id_, self.state[button_tag])
      if self.value_callback_fns[button_tag] is not None:
        self.value_callback_fns[button_tag](self.state[button_tag])

  def next(self):
    self.set_value(self.tags[(self.current_button_index() + 1) % len(self.tags)])
        
  def prev(self):
    self.set_value(self.tags[(self.current_button_index() - 1) % len(self.tags)])
        
  def callback(self, ui_id):
    # RadioButton deselects all other buttons.
    for id_, button_tag in zip(self.ids, self.tags):
      if ui_id == id_:
        self.set_value(button_tag)
        

class OptionButtons(ButtonSet):

  def __init__(self, name, tags, callbacks):
    # checkbox_style: 0 is filled box, 1 is X, 2 is filled circle
    super().__init__(name, tags, callbacks, 1)
    self.values = {}
    for id_, tag in zip(self.ids, self.tags):
      self.state[tag] = False
  
  def set_value(self, tag, value):
    for id_, button_tag in zip(self.ids, self.tags):
      if button_tag == tag:
        self.state[button_tag] = value
        if self.drawn:
          tulip.ui_checkbox(id_, value)
      if self.value_callback_fns[button_tag] is not None:
        self.value_callback_fns[button_tag](self.state[button_tag])

  def get_value(self, tag):
    return self.state[tag]

  def callback(self, ui_id):
    for id_, button_tag in zip(self.ids, self.tags):
      if ui_id == id_:
        self.set_value(button_tag, tulip.ui_checkbox(id_))


class UIGroup(UIBase):
  inset_x = 5
  inset_y = 5
  top_height = 30
  top_color = tulip.color(255, 0, 0)
  
  def __init__(self, name, elements):
    super().__init__(name)
    self.elements = elements
    
  def place(self, x, y):
    self.x = x
    self.y = y
    x = self.x + self.inset_x
    y = self.y + self.top_height + 2 * self.inset_y
    h = 0
    for element in self.elements:
      element.place(x, y)
      x += element.w + self.inset_x
      h = element.h if element.h > h else h
    self.w = x - self.x
    self.h = h + self.top_height + 2 * self.inset_y

  def draw(self):
    if self.name:
      # Draw frame.
      tulip.bg_rect(self.x, self.y, self.w, self.h, self.fg_color, False)
      # Draw title.
      tulip.bg_rect(self.x, self.y, self.w, self.top_height, self.top_color, True)
      tulip.bg_str(self.name, self.x, self.y,
                   self.text_color, self.title_font, 
                   self.w, self.top_height)
    # Draw elements.
    for element in self.elements:
      element.draw()


class Spinbox(ControlledLabel):
  value = 0
  min_value = 0
  max_value = 127
  set_fn = None
  
  def __init__(self, set_fn, initial_text='', min_value=0, max_value=127, initial_value=0, **kwargs):
    self.set_fn = set_fn  # called when value changes, returns text to display.
    self.min_value = min_value
    self.max_value = max_value
    super().__init__('Spinbox', ['-', '+'],
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
    self.name = self.set_fn(self.value)
    self.set_text(self.name)


import juno
midi_channel = 0
juno_patch_from_midi_channel = [juno.JunoPatch.from_patch_number(i) for i in range(16)]

def current_juno():
  return juno_patch_from_midi_channel[midi_channel]

current_juno().init_AMY()


#jp = juno.JunoPatch.from_patch_number(20)
#jp.init_AMY()
#alles.send(osc=0, note=60, vel=1)

# Make the callback function.
def jcb(arg):
  #callback = lambda x: jp.set_param(arg, x)
  callback = lambda x: current_juno().set_param(arg, x)
  return callback

lfo_rate = Slider('Rate', jcb('lfo_rate'))
lfo_delay_time = Slider('Delay', jcb('lfo_delay_time'))

lfo = UIGroup('LFO', [lfo_rate, lfo_delay_time])

dco_range = RadioButton("Range", ["4'", "8'", "16'"],
                        [jcb('stop_4'), jcb('stop_8'), jcb('stop_16')])
dco_lfo = Slider('LFO', jcb('dco_lfo'))
dco_pwm = Slider('PWM', jcb('dco_pwm'))
dco_pwm_mode = RadioButton('PWM', ['LFO', 'Man'], [None, jcb('pwm_manual')])
dco_wave = OptionButtons('Wave', ['Pulse', 'Saw'], [jcb('pulse'), jcb('saw')])
dco_sub = Slider('Sub', jcb('dco_sub'))
dco_noise = Slider('Noise', jcb('dco_noise'))

dco = UIGroup('DCO', [dco_range, dco_lfo, dco_pwm, dco_pwm_mode, dco_wave, dco_sub, dco_noise])

#hpf_freq = Slider('Freq', jcb('hpf'))
def hpf(n):
  callback = lambda x: current_juno().set_param('hpf', n) if x else None
  return callback
  
hpf_freq = RadioButton('Freq', ['3', '2', '1', '0'],
                       [hpf(3), hpf(2), hpf(1), hpf(0)])
hpf = UIGroup('HPF', [hpf_freq])

vcf_freq = Slider('Freq', jcb('vcf_freq'))
vcf_res = Slider('Res', jcb('vcf_res'))
vcf_pol = RadioButton('Pol', ['Pos', 'Neg'], [None, jcb('vcf_neg')])
vcf_env = Slider('Env', jcb('vcf_env'))
vcf_lfo = Slider('LFO', jcb('vcf_lfo'))
vcf_kbd = Slider('Kybd', jcb('vcf_kbd'))

vcf = UIGroup('VCF', [vcf_freq, vcf_res, vcf_pol, vcf_env, vcf_lfo, vcf_kbd])

vca_mode = RadioButton('Mode', ['Env', 'Gate'], [None, jcb('vca_gate')])
vca_level = Slider('Level', jcb('vca_level'))

vca = UIGroup('VCA', [vca_mode, vca_level])

env_a = Slider('A', jcb('env_a'))
env_d = Slider('D', jcb('env_d'))
env_s = Slider('S', jcb('env_s'))
env_r = Slider('R', jcb('env_r'))

env = UIGroup('ENV', [env_a, env_d, env_s, env_r])

def cho(n):
  callback = lambda x: current_juno().set_param('chorus', n) if x else None
  return callback

chorus_mode = RadioButton('Mode', ['Off', 'I', 'II', 'III'],
                          [cho(0), cho(1), cho(2), cho(3)])
chorus = UIGroup('CH', [chorus_mode])

juno_ui = UIGroup('', [lfo, dco, hpf, vcf, vca, env, chorus])


def setup_from_patch(patch):
  """Make the UI match the values in a JunoPatch."""
  current_juno().defer_param_updates = True
  glob_fns = globals()
  for el in ['lfo_rate', 'lfo_delay_time',
             'dco_lfo', 'dco_pwm', 'dco_sub', 'dco_noise',
             'vcf_freq', 'vcf_res', 'vcf_env', 'vcf_lfo', 'vcf_kbd',
             'vca_level', 'env_a', 'env_d', 'env_s', 'env_r']:
    # globals()[el] is the (UI) object with that name
    # getattr(patch, el) is that member of the patch object
    glob_fns[el].set_value(getattr(patch, el))

  dco_range.set_value("4'" if patch.stop_4 else "16'" if patch.stop_16 else "8'")
  dco_pwm_mode.set_value('Man' if patch.pwm_manual else 'LFO')
  dco_wave.set_value('Pulse', patch.pulse)
  dco_wave.set_value('Saw', patch.saw)
  hpf_freq.set_value(str(patch.hpf))
  vcf_pol.set_value('Neg' if patch.vcf_neg else 'Pos')
  vca_mode.set_value('Gate' if patch.vca_gate else 'Env')
  chorus_mode.set_value(['Off', 'I', 'II', 'III'][patch.chorus])

  current_juno().send_deferred_params()

  return patch.name


def setup_from_patch_number(patch_number):
  current_juno().patch_number = patch_number
  current_juno().name = setup_from_patch(juno.JunoPatch.from_patch_number(patch_number))
  return current_juno().name

def setup_from_midi_chan(new_midi_channel):
  """Switch which JunoPatch we display based on MIDI channel."""
  global midi_channel
  midi_channel = new_midi_channel
  new_patch = current_juno()
  new_patch.init_AMY()
  patch_selector.value = new_patch.patch_number  # Bypass actually reading that patch, just set the state.
  patch_selector.set_text(new_patch.name)
  setup_from_patch(new_patch)
  return "MIDI chan %d" % (midi_channel + 1)


patch_selector = Spinbox(set_fn=setup_from_patch_number)
midi_selector = Spinbox(set_fn=setup_from_midi_chan, max_value=15, width=160)

# Wire up MIDI controls

# Oxygen49 slider IDs, starting from left.
SLIDER_IDS = [0x49, 0x4b, 0x48, 0x4a, 0x4f, 0x54, 0x5b, 0x5d, 0x7]
#SLIDER_IDS = [74, 71, 91, 93, 73, 72, 5, 84, 7]
# Oxygen49 knobs, top row then second row.
KNOB_IDS = [0x10, 0x11, 0x12, 0x0a, 0x13, 0x50, 0x51, 0x14]
#KNOB_IDS = [75, 76, 92, 95, 10, 77, 78, 79]
# Oxygen49 buttons.  They toggle between 0 and 0x7f.
BUTTON_IDS = [0x18, 0x19, 0x1a, 0x1b, 0x2c, 0x2d, 0x2e, 0x2f, 0x00, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76]
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
    SLIDER_IDS[8]: 'vca_level',
    SLIDER_IDS[4]: 'env_a',
    SLIDER_IDS[5]: 'env_d',
    SLIDER_IDS[6]: 'env_s',
    SLIDER_IDS[7]: 'env_r',
    BUTTON_IDS[0]: 'dco_range',
    BUTTON_IDS[1]: 'dco_pwm_mode', 
    BUTTON_IDS[2]: 'Pulse',
    BUTTON_IDS[3]: 'Saw',
    BUTTON_IDS[4]: 'hpf_freq',
    BUTTON_IDS[5]: 'vcf_pol',
    BUTTON_IDS[6]: 'vca_mode',
    BUTTON_IDS[7]: 'chorus_mode',
}

def control_change(control, value):
  #print("juno_ui control_change: control", control, "value", value)
  value = value / 127.0
  if control == 0:  # Pitch bend.
    current_juno().set_pitch_bend(2 * value - 1)
  if control in param_map:
    param_name = param_map[control]
    # Special cases.
    if param_name == 'Pulse' or param_name == 'Saw':
      dco_wave.set_value(param_name, not dco_wave.get_value(param_name))
      return  # Early exit.
    param_obj = globals()[param_name]
    if isinstance(param_obj, RadioButton):
      param_obj.next()  # value ignored.
      return
    param_obj.set_value(value)




# Juno UI
tulip.bg_clear()

juno_ui.place(10, 30)
juno_ui.draw()

patch_selector.place(550, 20)
patch_selector.draw()
patch_selector.set_value(0)

midi_selector.place(815, 20)
midi_selector.draw()

# Start the polyvoice
import polyvoice

polyvoice.init(current_juno(), tulip.midi_in, control_change, patch_selector.set_value)
tulip.midi_callback(polyvoice.midi_event_cb)

