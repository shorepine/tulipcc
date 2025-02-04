# midi.py
# always running midi listener
# based on dan's polyvoice

import amy
import arpegg
import tulip

# the synths we apply to MIDI channels
from synth import PatchSynth, OscSynth, DrumSynth

class MidiConfig:
    """System-wide Midi input config."""

    def __init__(self, voices_per_channel={}, patch_per_channel={}, show_warnings=True):
        self.show_warnings = show_warnings
        self.synth_per_channel = dict()
        self.arpeggiator_per_channel = {}
        for channel, num_voices in voices_per_channel.items():
            patch = patch_per_channel[channel] if channel in patch_per_channel else None
            self.add_synth(channel=channel, synth=PatchSynth(patch_number=patch, num_voices=num_voices))

    def release_synth_for_channel(self, channel):
        if channel in self.synth_per_channel:
            # Old synth allocated - Expicitly return the amy_voices to the pool.
            self.synth_per_channel[channel].release()
            del self.synth_per_channel[channel]
        if channel in self.arpeggiator_per_channel:
            self.arpeggiator_per_channel[channel].synth = None

    def add_synth(self, synth=None, patch_number=None, channel=1, num_voices=None):
        if synth is None and patch_number is None:
            raise ValueError('No synth (or patch_number) specified')
        self.release_synth_for_channel(channel)
        if synth is None:
            print('add_synth(patch_number=..) is deprecated and will be removed.  Use add_synth(PatchSynth(patch_number=..)) instead.')
            if num_voices is None:
                num_voices = 6  # Default
            synth = PatchSynth(num_voices=num_voices, patch_number=patch_number)
        elif patch_number is not None or num_voices is not None:
            raise ValueError('You cannot specify both synth and patch_number/num_voices')
            # .. because we can't reconfigure num_voices which you might be expecting.
        self.synth_per_channel[channel] = synth
        if channel in self.arpeggiator_per_channel:
            self.arpeggiator_per_channel[channel].synth = synth
        if hasattr(synth, 'deferred_init'):
            synth.deferred_init()
        # Return the newly-created synth object so client can tweak it.
        return synth

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
        """Return the synth associated with a given channel."""
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

arpeggiator = arpegg.ArpeggiatorSynth(synth=None)

def ensure_midi_config():
    global config
    if not config:
        # Tulip defaults, 6 note polyphony on channel 1
        # drum machine always on channel 10
        # utility sine wave bleeper on channel 16
        config = MidiConfig(show_warnings=True)
        # The "system bleep" synth
        config.add_synth(channel=16, synth=OscSynth(num_voices=1))
        # GeneralMidi Drums.
        config.add_synth(channel=10, synth=DrumSynth(num_voices=10))
        # Default Juno synth on Channel 1.
        config.add_synth(channel=1, synth=PatchSynth(patch_number=0, num_voices=6))
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
        import json
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

# Resets AMY timebase and plays the bleep
def startup_bleep():
    amy.send(reset=amy.RESET_TIMEBASE)
    if 16 in config.synth_per_channel:
        config.synth_per_channel[16].note_on(57, 1, time=0)
        config.synth_per_channel[16].note_on(69, 1, time=150)
        config.synth_per_channel[16].note_off(69, time=300)


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

