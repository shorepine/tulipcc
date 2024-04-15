# midi.py
# always running midi listener
# based on dan's polyvoice



import amy
import tulip
import time
import random

# Tulip defaults, 4 note polyphony on channel 1
# drum machine always on channel 10
has_init = False
PCM_PATCHES = 29
polyphony_map = {1:4, 10:10}
patch_map = {1:2}

# Our state
voices_for_channel = {}
note_for_voice = {}
voices_active = []
# Dict of per-channel booleans
in_sustain_dict = {}
# Dict of per-channel sets of notes
sustained_notes_dict = {}

# Arpeggiator stuff, set by voices
arpegg = False
arpegg_hold = False
arpegg_mode = 0 
arpegg_range = 1
arpegg_note = 0
arpegg_dir = 1




# Micropython collections.deque does not support remove.
class Queue:
    def __init__(self, maxsize=64, name=''):
            self.name = name    # Just for debug.
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

    def voice_off(self, voice, time=None):
        """Terminate voice, update note_of_voice, but don't alter the queues."""
        self.voices[voice].note_off(time=time)
        # We no longer have a voice playing this note.
        del self.voice_of_note[self.note_of_voice[voice]]
        self.note_of_voice[voice] = None

    def note_off(self, note, time=None):
        if note not in self.voice_of_note:
            return
        old_voice = self.voice_of_note[note]
        self.voice_off(old_voice, time=time)
        # Return to released.
        self.active_voices.remove(old_voice)
        self.released_voices.put(old_voice)

    def note_on(self, note, velocity, time=None):
        if velocity == 0:
            self.note_off(note, time=time)
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
            self.voices[new_voice].note_on(note, velocity, time=time)

    def set_patch(self, patch_number):
        self.synth_source.set_patch(patch_number)

    def control_change(self, control, value):
        self.synth_source.control_change(control, value)


def get_in_sustain(channel):
    global in_sustain_dict
    if channel not in in_sustain_dict:
        in_sustain_dict[channel] = False
    return in_sustain_dict[channel]

def set_in_sustain(channel, state):
    global in_sustain_dict
    in_sustain_dict[channel] = state

def get_sustained_notes_set(channel):
    global sustained_notes_dict
    if channel not in sustained_notes_dict:
        sustained_notes_dict[channel] = set()
    return sustained_notes_dict[channel]

def pitch_bend(channel, val):
    # get the active voices for this channel
    if channel not in voices_for_channel:
        return
    v_str = ""
    for v in voices_for_channel[channel]:
        if v in voices_active: v_str = v_str + "%d," % (v)
        if(len(v_str)):
            amy.send(voices=v_str[:-1], pitch_bend=val)

def sustain_pedal(channel, value):
    if value:
        set_in_sustain(channel, True)
    else:
        set_in_sustain(channel, False)
        sustained_notes = get_sustained_notes_set(channel)
        for midinote in sustained_notes:
            note_off(channel, midinote)
            sustained_notes.remove(midinote)  # Modifies the global.

# TODO - REPLACE WITH Synth
# Get the voice for this chan/note 
def find_voice(channel, midinote):
    if channel not in voices_for_channel:
        return None

    for v in voices_for_channel[channel]:
        # Is this voice active already but also the existing note? that's ok
        if note_for_voice.get(v,None) == midinote:
            return v

        # Is this voice free? Use it
        if v not in voices_active:
            voices_active.append(v)
            note_for_voice[v] = midinote
            return v

    # couldn't find free voice. 
    # steal the earliest voice added
    for v in voices_active:
        if v in voices_for_channel[channel]:
            # move this stolen voice to the end
            voices_active.remove(v)
            voices_active.append(v)
            note_for_voice[v] = midinote
            return v

def note_on(channel, midinote, vel):
    global arpeg
    # Drum channel is special
    voice = find_voice(channel, midinote)
    if voice is not None:
        if(channel == 10):
            amy.send(osc=voice, wave=amy.PCM, patch=midinote % PCM_PATCHES, vel=vel, freq=0)
        else:
            if(not arpegg):
                amy.send(voices="%d" % (voice), note=midinote, vel=vel)

# TODO - REPLACE WITH Synth
def note_off(channel, midinote):  
    global arpeg
    # Get the voice number for this midinote
    if get_in_sustain(channel):
        sustained_notes = get_sustained_notes_set(channel)
        sustained_notes.add(midinote)
        return
    
    if channel not in voices_for_channel: return

    for v in voices_for_channel[channel]:
        if note_for_voice.get(v,None) == midinote:
            # Remove it
            voices_active.remove(v)
            del note_for_voice[v]
            # Turn off the note
            if(channel == 10):
                amy.send(osc=v, vel=0)
            else:
                amy.send(voices='%d' % (v), vel=0)
            return

# Do the actual setting of the voice for a channel
def update_channel(channel):
    time.sleep(0.1) # AMY queue will fill if not slept 
    vstr = ",".join([str(x) for x in voices_for_channel[channel]])
    amy.send(voices=vstr, load_patch=patch_map[channel])

def program_change(channel, patch):
    # update the map
    patch_map[channel] = patch
    update_channel(channel)
    # Update voices UI if it is running
    try:
        voices.refresh()
    except:
        pass

def play_arpegg_step(t, mode):
    # Get the channel number for the _last_ held note, and only arpeggiate that channel.
    # I don't know if multichannel arpeggiators exist, and I don't want to know
    global arpegg_note, arpegg_dir, arpegg_range
    if(len(voices_active)==0): return
    last_voice = voices_active[-1]
    last_channel = None
    for c in voices_for_channel.keys():
        if last_voice in voices_for_channel[c]:
            last_channel = c

    # Find held notes for that channel and add them to a list of notes
    all_held = []
    if last_channel is not None:
        for v in voices_for_channel[c]:
            if v in note_for_voice:
                all_held.append(note_for_voice[v])

    # Add range if set
    if(arpegg_range > 1):
        new_held = []
        for r in range(arpegg_range):
            for n in all_held:
                new_held.append(n + (r*12))
        all_held = new_held

    # TODO - This should call synth.note_on / note_off instead
    amy.send(voices='%d' % (voices_for_channel[last_channel][0]), note=all_held[arpegg_note % len(all_held)], vel=1, time=t)


    # mode -- up, down, up&down, rand
    if mode==0:
        arpegg_note = arpegg_note + 1
        if(arpegg_note == len(all_held)): arpegg_note = 0
    elif mode==1:
        arpegg_note = arpegg_note - 1
        if(arpegg_note < 0): arpegg_note = len(all_held)-1
    elif mode==2:
        if(arpegg_dir == 1): # up
            arpegg_note = arpegg_note + 1
            if(arpegg_note >= len(all_held)-1):
                arpegg_dir = 0 # down
        else:
            arpegg_note = arpegg_note - 1
            if(arpegg_note <= 0):
                arpegg_dir = 1
    elif mode==3:
        arpegg_note = random.randrange(0, len(all_held))




def midi_event_cb(x):
    """Callback that takes MIDI note on/off to create Note objects."""
    m = tulip.midi_in() 
    while m is not None and len(m) > 0:
        channel = (m[0] & 0x0F) + 1
        message = m[0] & 0xF0
        if message == 0x90:  # Note on.
            midinote = m[1]
            midivel = m[2]
            vel = midivel / 127.
            note_on(channel, midinote, vel)
        elif message == 0x80:  # Note off.
            midinote = m[1]
            note_off(channel, midinote)
        elif message == 0xc0:  # Program change
            program_change(channel, m[1])
        elif message == 0xe0:  # Pitch bend. m[2] is MSB, m[1] is LSB. 14 bits 
            pb_value = ((m[2] << 7) | (m[1])) - 8192 # -8192-8192, where 0 is nothing
            amy_value = float(pb_value)/(8192*6.0) # convert to -2 / +2 semitones
            amy.send(pitch_bend=amy_value)
        elif message == 0xb0 and m[1] == 0x40:
            sustain_pedal(channel, m[2])
    
        # Are there more events waiting?
        m = m[3:]
        if len(m) == 0:
            m = tulip.midi_in()

def setup_voices():
    amy.reset()
    v_counter = 0
    voices_for_channel[10] = list(range(110,120))
    for channel in polyphony_map.keys():
        if channel is not 10:
            voices_for_channel[channel] = list(range(v_counter, v_counter+polyphony_map[channel]))
            v_counter = v_counter + polyphony_map[channel]
            update_channel(channel)


def midi_step(t):
    global has_init, arpegg, arpegg_mode
    if(not has_init):
        # Deferred init
        if(tulip.seq_ticks() > tulip.seq_ppq()):
            setup_voices()
            tulip.midi_add_callback(midi_event_cb)
            has_init = True
    if(arpegg):
        play_arpegg_step(t, arpegg_mode)


# Keep this -- this is a tulip API 
def music_map(channel, patch_number=None, voice_count=None):
    if voice_count is not None:
        if patch_number is not None:
            patch_map[channel] = patch_number
        polyphony_map[channel] = voice_count
        setup_voices()
    elif patch_number is not None:
        patch_map[channel] = patch_number
        update_channel(channel)

    # Update voices UI if it is running
    try:
        voices.refresh()
    except:
        pass


def setup():
    # we can't setup on boot right away as we need to get the bleep going and the alles setup done, so wait on a callback
    tulip.seq_add_callback(midi_step, int(tulip.seq_ppq()/2))


