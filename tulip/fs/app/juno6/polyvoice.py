"""Implement a polyphonic synthesizer by managing a fixed pool of voices."""

# Optional monkeypatch of send() method to diagnose exactly what is being sent.
def amy_send_patch(**kwargs):
    print("amy_send:", kwargs)
    orig_amy_send(**kwargs)

# Apply the monkeypatch?
#orig_amy_send = amy.send
#amy.send = amy_send_patch

# Micropython collections.deque does not support remove.
class Queue:
    def __init__(self, maxsize=64, name=''):
        self.name = name  # Just for debug.
        self.maxsize = maxsize + 1
        self.queue = [None] * self.maxsize
        self.head = 0
        self.tail = 0

    def _next(self, pointer):
        """Incrementing a cicular buffer pointer."""
        return (pointer + 1) % self.maxsize
        
    def _prev(self, pointer):
        """Decrementing a cicular buffer pointer."""
        return (pointer + self.maxsize - 1) % self.maxsize
        
    def put(self, item):
        self.queue[self.tail] = item
        self.tail = self._next(self.tail)
        if self.tail == self.head:
            # Wrap around
            self.head = self._next(self.head)
            print("%s: dropped oldest item", self)

    def _delete_at(self, pointer):
        """Remove the value at queue[pointer], and close up the rest."""
        if self.tail > pointer:
            self.queue[pointer : self.tail - 1] = (
                self.queue[pointer + 1 : self.tail])
            self.tail = self._prev(self.tail)
        elif self.tail < pointer:
            # Shift head: pointer one step forward.
            self.queue[self.head + 1 : pointer + 1] = self.queue[self.head : pointer]
            self.head = self._next(self.head)
        else:
            raise ValueError('pointer at tail???')

    def remove(self, value):
        """Remove first occurrence of value from queue."""
        pointer = self.head
        while pointer != self.tail:
            if self.queue[pointer] == value:
                self._delete_at(pointer)
                return
            pointer = self._next(pointer)
        # Fell through, value wasn't found.
        raise ValueError('%s: No such item: %s' % (self, str(value)))

    def empty(self):
        return self.head == self.tail

    def full(self):
        return self.head == self._next(self.tail)

    def qsize(self):
        return (self.tail - self.head + self.maxsize) % self.maxsize

    def get(self):
        if self.empty():
            # get() on empty queue.
            raise ValueError
        value = self.queue[self.head]
        self.head = self._next(self.head)
        return value

    def __repr__(self):
        result = []
        p = self.head
        while p != self.tail:
            result.append(self.queue[p])
            p = self._next(p)
        return ("Queue%s(maxsize=%d) [" % (self.name, self.maxsize - 1)
                + (", ".join(str(s) for s in result))
                + "]")


class Synth:
  """Manage a polyphonic synthesizer by rotating among a fixed pool of voices.

  Argument synth_source provides the following methods:
    synth_source.get_new_voices(num_voices) returns num_voices VoiceObjects.
      VoiceObjects accept voice.note_on(note, vel), voice.note_off()
    synth_source.set_patch(patch_num) changes preset for all voices.
    synth_source.control_change(control, value) modifies a parameter for all voices.
  """
  
  def __init__(self, synth_source, num_voices=6):
    self.synth_source = synth_source
    self.voices = synth_source.get_new_voices(num_voices)
    self.released_voices = Queue(num_voices, name='Released')
    for voice_num in range(num_voices):
      self.released_voices.put(voice_num)
    self.active_voices = Queue(num_voices, name='Active')
    # Dict to look up active voice from note number, for note-off.
    self.voice_of_note = {}
    self.note_of_voice = [None] * num_voices

  def get_next_voice(self):
    """Return the next voice to use."""
    # First try free/released_voices in order, then steal from active_voices.
    if not self.released_voices.empty():
      return self.released_voices.get()
    # We have to steal an active voice.
    stolen_voice = self.active_voices.get()
    print('Stealing voice for', self.note_of_voice[stolen_voice])
    self.voice_off(stolen_voice)
    return stolen_voice

  def voice_off(self, voice):
    """Terminate voice, update note_of_voice, but don't alter the queues."""
    self.voices[voice].note_off()
    # We no longer have a voice playing this note.
    del self.voice_of_note[self.note_of_voice[voice]]
    self.note_of_voice[voice] = None

  def note_off(self, note):
    if note not in self.voice_of_note:
      return
    old_voice = self.voice_of_note[note]
    self.voice_off(old_voice)
    # Return to released.
    self.active_voices.remove(old_voice)
    self.released_voices.put(old_voice)

  def note_on(self, note, velocity):
    if velocity == 0:
      self.note_off(note)
    else:
      # Velocity > 0, note on.
      if note in self.voice_of_note:
        # Send another note-on to the voice already playing this note.
        new_voice = self.voice_of_note[note]
      else:
        new_voice = self.get_next_voice()
        self.active_voices.put(new_voice)
        self.voice_of_note[note] = new_voice
        self.note_of_voice[new_voice] = note
      self.voices[new_voice].note_on(note, velocity)

  def set_patch(self, patch_number):
    self.synth_source.set_patch(patch_number)

  def control_change(self, control, value):
    self.synth_source.control_change(control, value)

midi_in_fn = None
control_change_fn = None
set_patch_fn = None
SYNTH = None

def midi_event_cb(x):
  """Callback that takes MIDI note on/off to create Note objects."""
  m = midi_in_fn()  # tulip.midi_in()
  while m is not None and len(m) > 0:
    #print("midi in: 0x%x 0x%x 0x%x" % (m[0], m[1], m[2]))
    if m[0] == 0x90:  # Note on.
      midinote = m[1]
      midivel = m[2]
      vel = midivel / 127.
      SYNTH.note_on(midinote, vel)
    elif m[0] == 0x80:  # Note off.
      midinote = m[1]
      SYNTH.note_off(midinote)
    elif m[0] == 0xc0:  # Program change - choose the DX7 preset
      set_patch_fn(m[1])
    elif m[0] == 0xe0:  # Pitch bend.
      control_change_fn(0, m[2])
      # Special case.  Pitch bend is -1.0 .. 1.0.
      #amy.send(pitch_bend=(m[2] / 64 + m[1] / 8192) - 1.0)
    elif m[0] == 0xb0:  # Other control slider.
      control_change_fn(m[1], m[2])
    elif m[0] == 0xbf:
      # Special case for Oxygen49 transport buttons which send val 0x00 on release.
      if m[2] == 0x7f:
          control_change_fn(m[1], m[2])
        
    # Are there more events waiting?
    m = m[3:]
    if len(m) == 0:
        m = midi_in_fn()



def init(synth=None, my_midi_in_fn=None, my_control_change_fn=None, my_set_patch_fn=None):
  # Install the callback.
  #tulip.midi_callback(midi_event_cb)
  global midi_in_fn, control_change_fn, set_patch_fn, SYNTH

  midi_in_fn = my_midi_in_fn
  control_change_fn = my_control_change_fn
  set_patch_fn = my_set_patch_fn

  #if not synth:
  #    import juno
  #    synth = juno.JunoPatch.from_patch_number(0)
  
  NUM_VOICES = 8
  SYNTH = Synth(synth, NUM_VOICES)
