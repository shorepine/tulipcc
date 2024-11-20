# midi.py
# always running midi listener
# based on dan's polyvoice

import json
import time

import amy
import arpegg
import tulip
from patches import drumkit

class MidiConfig:
    """System-wide Midi input config."""

    def __init__(self, voices_per_channel, patch_per_channel, show_warnings=True):
        self.show_warnings = show_warnings
        self.synth_per_channel = dict()
        self.arpeggiator_per_channel = {}
        for channel, num_voices in voices_per_channel.items():
            patch = patch_per_channel[channel] if channel in patch_per_channel else None
            self.add_synth(channel, patch, num_voices)

    def add_synth_object(self, channel, synth_object):
        if channel in self.synth_per_channel:
            # Old Synth allocated - Expicitly return the amy_voices to the pool.
            self.synth_per_channel[channel].release()
            del self.synth_per_channel[channel]
        self.synth_per_channel[channel] = synth_object
        if channel in self.arpeggiator_per_channel:
            self.arpeggiator_per_channel[channel].synth = synth_object

    def add_synth(self, channel=1, patch_number=0, num_voices=6):
        if channel == 10:
            synth_object = DrumSynth(num_voices=num_voices)
        else:
            synth_object = Synth(num_voices=num_voices, patch_number=patch_number)
        self.add_synth_object(channel, synth_object)

    def insert_arpeggiator(self, channel, arpeggiator):
        if channel in self.synth_per_channel:
            self.arpeggiator_per_channel[channel] = arpeggiator
            arpeggiator.synth = self.synth_per_channel[channel]

    def remove_arpeggiator(self, channel):
        if channel in self.arpeggiator_per_channel:
            self.arpeggiator_per_channel.synth = None
            del self.arpeggiator_per_channel[channel]

    def program_change(self, channel, patch_number):
        # update the map
        self.synth_per_channel[channel].program_change(patch_number)

    def get_active_channels(self):
        """Return numbers of MIDI channels with allocated synths."""
        return list(self.synth_per_channel.keys())

    def get_synth(self, channel):
        """Return the Synth associated with a given channel."""
        return self.synth_per_channel[channel] if channel in self.synth_per_channel else None

    def channel_info(self, channel):
        """Report the current patch_num and list of amy_voices for this channel."""
        if channel not in self.synth_per_channel:
            return (None, None)
        return (
            self.synth_per_channel[channel].patch_number,
            self.synth_per_channel[channel].amy_voices,
        )

    def get_channel_state(self, channel):
        if channel not in self.synth_per_channel:
            return None
        return self.synth_per_channel[channel].get_patch_state()

    def set_channel_state(self, channel, state):
        if channel not in self.synth_per_channel:
            #raise ValueError('Attempting to set state for unallocated channel %d.' % channel)
            return
        self.synth_per_channel[channel].set_patch_state(state)

    def voices_for_channel(self, channel):
        """Return a list of AMY voices assigned to a channel."""
        if channel not in self.synth_per_channel:
            return []
        return self.synth_per_channel[channel].amy_voices()
                  


# Global MidiConfig object.
config = None


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


class VoiceObject:
    """Object to wrap an amy voice."""

    def __init__(self, amy_voice):
        self.amy_voice = amy_voice

    def note_on(self, note, vel, time=None, sequence=None):
        amy.send(time=time, voices=self.amy_voice, note=note, vel=vel, sequence=sequence)

    def note_off(self, time=None, sequence=None):
        amy.send(time=time, voices=self.amy_voice, vel=0, sequence=sequence)



class Synth:
    """Manage a polyphonic synthesizer by rotating among a fixed pool of voices.

    Provides methods:
      synth.note_on(midi_note, velocity, time=None, sequence=None)
      synth.note_off(midi_note, time=None, sequence=None)
      synth.all_notes_off()
      synth.program_change(patch_num) changes preset for all voices.
      synth.control_change(control, value) modifies a parameter for all voices.
    Provides read-back attributes (for voices.py UI):
      synth.amy_voices
      synth.patch_number
      synth.patch_state  - patch-specific data only used by clients e.g. UI state
  
    Note: The synth internally refers to its voices by indices in
    range(0, num_voices).  These numbers are not related to the actual amy
    voices rendering the note; the amy voice number is internal to the
    VoiceObjects and is opaque to the Synth object.
    """

    """Manage the pool of amy voices."""
    # Class-wide record of which voice to allocate next.
    allocated_amy_voices = set()
    next_amy_patch_number = 1024

    @classmethod
    def reset(cls):
        """Resets AMY and Synth's tracking of its state."""
        cls.allocated_amy_voices = set()
        cls.next_amy_patch_number = 1024
        amy.reset()

    def __init__(self, num_voices=6, patch_number=None, patch_string=None):
        self.voice_objs = self._get_new_voices(num_voices)
        self.released_voices = Queue(num_voices, name='Released')
        for voice_index in range(num_voices):
            self.released_voices.put(voice_index)
        self.active_voices = Queue(num_voices, name='Active')
        # Dict to look up active voice from note number, for note-off.
        self.voice_of_note = {}
        self.note_of_voice = [None] * num_voices
        self.sustaining = False
        self.sustained_notes = set()
        # Fields used by UI
        #self.num_voices = num_voices
        self.patch_number = None
        self.patch_state = None
        if patch_number is not None and patch_string is not None:
            raise ValueError('You cannot specify both patch_number and patch_string.')
        if patch_string is not None:
            patch_number = Synth.next_amy_patch_number
            Synth.next_amy_patch_number = patch_number + 1
            amy.send(store_patch='%d,%s' % (patch_number, patch_string))
        self.program_change(patch_number)

    def _get_new_voices(self, num_voices):
        new_voices = []
        next_amy_voice = 0
        while len(new_voices) < num_voices:
            while next_amy_voice in Synth.allocated_amy_voices:
                next_amy_voice += 1
            new_voices.append(next_amy_voice)
            next_amy_voice += 1
        self.amy_voice_nums = new_voices
        Synth.allocated_amy_voices.update(new_voices)
        voice_objects = []
        for amy_voice_num in self.amy_voice_nums:
            voice_objects.append(VoiceObject(amy_voice_num))
        return voice_objects

    @property
    def amy_voices(self):
        return [o.amy_voice for o in self.voice_objs]

    @property
    def num_voices(self):
        return len(self.voice_objs)

    # send an AMY message to the voices in this synth
    def amy_send(self, **kwargs):
        vstr = ",".join([str(a) for a in self.amy_voice_nums])
        amy.send(voices=vstr, **kwargs)

    def _get_next_voice(self):
        """Return the next voice to use."""
        # First try free/released_voices in order, then steal from active_voices.
        if not self.released_voices.empty():
            return self.released_voices.get()
        # We have to steal an active voice.
        stolen_voice = self.active_voices.get()
        #print('Stealing voice for', self.note_of_voice[stolen_voice])
        self._voice_off(stolen_voice)
        return stolen_voice

    def _voice_off(self, voice, time=None, sequence=None):
        """Terminate voice, update note_of_voice, but don't alter the queues."""
        self.voice_objs[voice].note_off(time=time, sequence=sequence)
        # We no longer have a voice playing this note.
        del self.voice_of_note[self.note_of_voice[voice]]
        self.note_of_voice[voice] =  None

    def note_off(self, note, time=None, sequence=None):
        if self.sustaining:
            self.sustained_notes.add(note)
            return
        if note not in self.voice_of_note:
            return
        old_voice = self.voice_of_note[note]
        self._voice_off(old_voice, time=time, sequence=sequence)
        # Return to released.
        self.active_voices.remove(old_voice)
        self.released_voices.put(old_voice)

    def all_notes_off(self):
        self.sustain(False)
        while not self.active_voices.empty():
            voice = self.active_voices.get()
            self._voice_off(voice)


    def note_on(self, note, velocity=1, time=None, sequence=None):
        if not self.amy_voice_nums:
            # Note on after synth.release()?
            raise ValueError('Synth note on with no voices - synth has been released?')
        if velocity == 0:
            self.note_off(note, time=time, sequence=sequence)
        else:
            # Velocity > 0, note on.
            if note in self.voice_of_note:
                # Send another note-on to the voice already playing this note.
                new_voice = self.voice_of_note[note]
            else:
                new_voice = self._get_next_voice()
                self.active_voices.put(new_voice)
                self.voice_of_note[note] = new_voice
                self.note_of_voice[new_voice] = note
            self.voice_objs[new_voice].note_on(note, velocity, time=time, sequence=sequence)

    def sustain(self, state):
        """Turn sustain on/off."""
        if state:
            self.sustaining = True
        else:
            self.sustaining = False
            for midinote in self.sustained_notes:
                self.note_off(midinote)
            self.sustained_notes = set()

    def get_patch_state(self):
        return self.patch_state

    def set_patch_state(self, state):
        self.patch_state = state

    def program_change(self, patch_number):
        if patch_number != self.patch_number:
            self.patch_number = patch_number
            # Reset any modified state due to previous patch modifications.
            self.patch_state = None
            time.sleep(0.1)  # "AMY queue will fill if not slept."
            self.amy_send(load_patch=patch_number)

    def control_change(self, control, value):
        print('control_change not implemented for amy-managed voices.')

    def release(self):
        """Called to terminate this synth and release its amy_voice resources."""
        # Turn off any active notes
        self.all_notes_off()
        # Return all the amy_voices
        for amy_voice in self.amy_voice_nums:
            Synth.allocated_amy_voices.remove(amy_voice)
        self.amy_voice_nums = []
        del self.voice_objs[:]


class SingleOscSynthBase:
    """Base class for synth using one osc per note from preallocated pool."""

    def __init__(self, num_voices=6, first_osc=None):
        """Create a synth that plays the specified note on a single osc configured with params_dict."""
        if first_osc is None:
            first_osc = amy.AMY_OSCS - num_voices
        self.oscs = list(range(first_osc, first_osc + num_voices))
        self.next_osc = 0
        self.osc_of_note = {}
        # Put the oscs in a default state.
        for osc in self.oscs:
            amy.send(reset=osc)
        # Fields used by UI
        self.amy_voices = self.oscs  # Actually osc numbers not amy voices.
        self.patch_number = None

    # This method must be overridden by the derived class to actually send the note.
    def _note_on_with_osc(self, osc, note, velocity, time=None, sequence=None):
        raise NotImplementedError

    def note_on(self, note, velocity=1, time=None, sequence=None):
        osc = self.oscs[self.next_osc]
        self.next_osc = (self.next_osc + 1) % len(self.oscs)
        # Update mapping of note to osc.  If notes are repeated, this will lose track.
        self.osc_of_note[note] = osc
        # Actually issue the note-on via derived class function
        self._note_on_with_osc(osc, note, velocity, time=time, sequence=sequence)

    def note_off(self, note, time=None, sequence=None):
        if note in self.osc_of_note:
             amy.send(time=time, sequence=sequence, osc=self.osc_of_note[note], vel=0)
             del self.osc_of_note[note]

    def all_notes_off(self):
        """Execute note-offs for all the notes we believe currently active."""
        for osc in self.oscs:
            amy.send(osc=osc, vel=0)
        # Delete active osc record.
        self.osc_of_note = {}

    def release(self):
        """Called to terminate this synth and release its amy_voice resources."""
        # Turn off any active notes
        self.all_notes_off()
        # Release the oscs we use .. not registered at the moment.
        self.oscs = []
        self.amy_voices = []

    # Rest of Synth protocol doesn't do anything by default.
    def sustain(self, state):
        pass

    def program_change(self, patch_number):
        pass

    def control_change(self, control, value):
        pass


class OscSynth(SingleOscSynthBase):
    """Synth that uses one osc per note.  Osc parameters are specified at initialization."""

    def __init__(self, num_voices=6, first_osc=None, **kwargs):
        """Create a synth that plays the specified note on a single osc configured with keyword args."""
        super().__init__(num_voices, first_osc)
        # Configure oscs.
        self.update_oscs(**kwargs)

    def update_oscs(self, **kwargs):
        """Update all our oscs with the params in the dict."""
        if 'osc' in kwargs:
            raise ValueError('Do not provide an \'osc\' argument to OscSynth.')
        for osc in self.oscs:
            amy.send(osc=osc, **kwargs)

    def _note_on_with_osc(self, osc, note, velocity, time=None, sequence=None):
        amy.send(time=time, sequence=sequence, osc=osc, note=note, vel=velocity)


class DrumSynth(SingleOscSynthBase):
    """Synth for Drum channel (10).
    Each MIDI note plays a separately-configured sound.
    """
    missing_note_warned = []

    def __init__(self, num_voices=6, first_osc=None):
        super().__init__(num_voices, first_osc)
        # A dict of parameters associated with each midi note.  Make a copy
        # of the defaults.
        self.midi_note_params = {}
        # These are the patches in amy/src/pcm_small.h reconciled to general midi chan 10.
        for patch, (base_pitch, _, midi_note) in enumerate(drumkit):
            if midi_note is not None:
                self.midi_note_params[midi_note] = {'wave': amy.PCM, 'note': base_pitch, 'freq': 0, 'patch': patch}

    def setup_midi_note(self, midi_note, params_dict):
        """Configure a midi note with a dict of osc params."""
        self.midi_note_params[midi_note] = params_dict

    def _note_on_with_osc(self, osc, note, velocity=None, time=None, sequence=None):
        if note not in self.midi_note_params:
            if config.show_warnings and note not in DrumSynth.missing_note_warned:
                print("DrumSynth note_on for note %d but only %s set up." % (
                    note, str(sorted(list(self.midi_note_params.keys())))
                ))
                DrumSynth.missing_note_warned.append(note)
            return

        # If velocity not set, attempt to use amp from the stored settings. 
        if(velocity is None): velocity = self.midi_note_params[note].get('amp',1)

        send_args = {'time': time, 'sequence': sequence, 'osc': osc, 'vel': velocity}
        # Add the args for this note
        send_args |= self.midi_note_params[note]
        amy.send(**send_args)



arpeggiator = arpegg.ArpeggiatorSynth(synth=None)


def ensure_midi_config():
    global config
    if not config:
        # Tulip defaults, 6 note polyphony on channel 1
        # drum machine always on channel 10
        config = MidiConfig(
            voices_per_channel={1: 6, 10: 10},
            patch_per_channel={1: 0},
            show_warnings=True,
        )
        config.insert_arpeggiator(channel=1, arpeggiator=arpeggiator)


# Hooks for Arpeggiator UI control from MIDI input CCs.

def get_voices_app():
    # Return voices app if it exists, else None.
    return tulip.running_apps.get("voices", None)

def tempo_update(midi_value):
    """Called when arpeggiator rate knob turned."""
    # Log range from 30 to 240 bpm.
    new_bpm = int(round(30 * (2 ** (midi_value / (127 / 3)))))
    tulip.seq_bpm(new_bpm)
    if get_voices_app():
        get_voices_app().settings.set_tempo(new_bpm)

def arp_on():
    arpeggiator.set('on', not arpeggiator.active)
    if get_voices_app():
        get_voices_app().settings.update_from_arp(arpeggiator)

def arp_hold():
    arpeggiator.set('hold', not arpeggiator.hold),
    if get_voices_app():
        get_voices_app().settings.update_from_arp(arpeggiator)

def arp_mode_next():
    arpeggiator.cycle_direction()
    if get_voices_app():
        get_voices_app().settings.update_from_arp(arpeggiator)

def arp_rng_next():
    arpeggiator.cycle_octaves()
    if get_voices_app():
        get_voices_app().settings.update_from_arp(arpeggiator)


# Wire up MIDI controls
# Oxygen49 slider IDs, starting from left.
#SLIDER_IDS = [0x49, 0x4b, 0x48, 0x4a, 0x4f, 0x54, 0x5b, 0x5d, 0x7]
SLIDER_IDS = [74, 71, 91, 93, 73, 72, 5, 84, 7]
# Oxygen49 knobs, top row then second row.
#KNOB_IDS = [0x10, 0x11, 0x12, 0x0a, 0x13, 0x50, 0x51, 0x14]
KNOB_IDS = [75, 76, 92, 95, 10, 77, 78, 79]
# Oxygen49 buttons.  They toggle between 0 and 0x7f.
#BUTTON_IDS = [0x18, 0x19, 0x1a, 0x1b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
#              0x71, 0x72, 0x73, 0x74, 0x75, 0x76]
# I had to program these.  See Oxygen49-UserGuide-v1.3.pdf + notes.
BUTTON_IDS = [96, 97, 98, 99, 100, 101, 102, 103, 104,   113, 114, 115, 116, 117, 118]

PITCH_WHEEL = 0   # Pitch wheel is a special case, hard-coded in juno.py.
MOD_WHEEL = 1

def setup_midi_codes():
    """Set up the global MIDI controller codes.  Happens late so filesystem is present."""
    global SLIDER_IDS, KNOB_IDS, BUTTON_IDS
    try:
        midi_cc_file = tulip.root_dir() + 'user/midi_cc_file.json'
        data = json.loads(open(midi_cc_file, 'rt').read())
        SLIDER_IDS = data['sliders']
        KNOB_IDS = data['knobs']
        BUTTON_IDS = data['buttons']
        #print('MIDI CC mappings read from', midi_cc_file)
    except OSError:  # Anticipating midi_cc_file not found.
        pass
    except TypeError: # data is not a dict
        pass
    except KeyError: # data doesn't have one of the required things in it
        pass

def setup_global_midi_cc_bindings():
    """Set up the global midi control code bindings, once the code arrays are set."""
    global GLOBAL_MIDI_CC_BINDINGS
    # My default connection of MIDI CCs to sequencer/arpeggiator
    # to the Oxygen49 transport keys.
    try:
        TEMPO_KNOB = KNOB_IDS[7]  # Rightmost knob
        ARP_ON_BTN = BUTTON_IDS[9]  # C27, transport button
        ARP_HOLD_BTN = BUTTON_IDS[10]
        ARP_MODE_BTN = BUTTON_IDS[11]
        ARP_RANGE_BTN = BUTTON_IDS[12]
        
        GLOBAL_MIDI_CC_BINDINGS = {
            TEMPO_KNOB: tempo_update,
            # Some buttons send 0 on release, ignore that.
            ARP_ON_BTN: lambda x: arp_on() if x else None,
            ARP_HOLD_BTN: lambda x: arp_hold() if x else None,
            ARP_MODE_BTN: lambda x: arp_mode_next() if x else None,
            ARP_RANGE_BTN: lambda x: arp_rng_next() if x else None,
        }

    except IndexError: # custom midi cc file did not specify buttons or knobs that go this high
        print("Warning: Not enough knobs and buttons defined in midi_cc_file.json to control arp and tempo")
        GLOBAL_MIDI_CC_BINDINGS = {}


WARNED_MISSING_CHANNELS = set()


# midi.py's own python midi callback. you can remove this if you don't want it active
def midi_event_cb(midi_message):
    """Callback that takes MIDI note on/off to create Note objects."""
    ensure_midi_config()

    # Ignore single value messages (clock, etc) for now.
    if(len(midi_message)<2): 
        return

    message = midi_message[0] & 0xF0
    channel = (midi_message[0] & 0x0F) + 1
    control = midi_message[1]
    value = midi_message[2] if len(midi_message) > 2 else None
    #print("MIDI in:", channel, message, control, value)
    if message == 0xb0 and control in GLOBAL_MIDI_CC_BINDINGS:
        # Accept GLOBAL_MIDI_CC_BINDINGS regardless of channel.
        GLOBAL_MIDI_CC_BINDINGS[control](value)
        return  # Early exit
    if channel not in config.synth_per_channel:
        if config.show_warnings and channel not in WARNED_MISSING_CHANNELS:
            print("Warning: No synth configured for MIDI channel %d. message was %s %s" %(channel, hex(midi_message[0]), hex(midi_message[1])))
            WARNED_MISSING_CHANNELS.add(channel)
        return  # Early exit
    # We have a populated channel.
    synth = config.synth_per_channel[channel]
    # Fetch the arpeggiator for this channel, or use synth if there isn't one.
    note_receiver = config.arpeggiator_per_channel.get(channel, synth)
    midinote = control
    if message == 0x90:  # Note on (or note off, if vel = 0)
        vel = value / 127.
        note_receiver.note_on(midinote, vel)
    elif message == 0x80:  # Note off.
        note_receiver.note_off(midinote)
    elif message == 0xc0:  # Program change
        synth.program_change(control)
    elif message == 0xb0 and control == 0x40:
        # Sustain pedal.
        synth.sustain(value)
    elif message == 0xe0:  # Pitch bend goes direct to AMY.
        # m[2] is MSB, m[1] is LSB. 14 bits
        pb_value = ((midi_message[2] << 7) | (midi_message[1])) - 8192 # -8192-8192, where 0 is nothing
        amy_value = float(pb_value)/(8192*6.0) # convert to -2 / +2 semitones
        amy.send(pitch_bend=amy_value)
    elif message == 0xB0 and control == 123: # all notes off
        synth.all_notes_off()




MIDI_CALLBACKS = set()

# Add a midi callback and return a slot number
def add_callback(fn):
    MIDI_CALLBACKS.add(fn)

def remove_callback(fn):
    MIDI_CALLBACKS.remove(fn)

def start_default_callback():
    add_callback(midi_event_cb)

def stop_default_callback():
    remove_callback(midi_event_cb)


# The midi callback sent over from C, fires all the other ones if set.
def c_fired_midi_event(x):
    m = tulip.midi_in() 
    while m is not None and len(m) > 0:
        # call the other callbacks
        for c in MIDI_CALLBACKS:
            c(m)

        m = tulip.midi_in()


def deferred_midi_config(t):
    setup_midi_codes()
    setup_global_midi_cc_bindings()
    tulip.midi_callback(c_fired_midi_event)
    start_default_callback()
    ensure_midi_config()

def setup():
    deferred_midi_config(None)
    # we can't setup on boot right away as we need to get the bleep going and the alles setup done, so wait on a defer
    #tulip.defer(deferred_midi_config, None, 500)

