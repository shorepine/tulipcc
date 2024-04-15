# midi.py
# always running midi listener
# based on dan's polyvoice

import amy
import tulip
import time
import random


class MidiSynthsConfig:
    """System-wide Midi input config."""

    def __init__(self, voices_per_channel, patch_per_channel):
        self.synth_per_channel = [None] * 16  # Fixed array of 16 slots for Synths.
        for channel, polyphony in voices_per_channel.items():
            patch = patch_per_channel[channel] if channel in patch_per_channel else None
            self.add_synth(channel, patch, polyphony)

    def add_synth(self, channel, patch, polyphony):
        if channel == 10:
            synth = DrumSynth(num_voices=polyphony)
        else:
            synth = Synth(voice_source=VoiceSource(), num_voices=polyphony)
            if patch is not None:
                synth.program_change(patch)
        self.synth_per_channel[channel] = synth

    def program_change(self, channel, patch):
        # update the map
        self.synth_per_channel[channel].program_change(patch)

    def music_map(self, channel, patch_number=0, voice_count=None):
        """Implement the tulip music_map API."""
        if not voice_count:
            # Simply changing patch.
            self.program_change(channel, patch_number)
        else:
            # Setting up a new channel.
            self.add_synth(channel, patch_number, voice_count)
                  


# Global object.
midi_synths = None


# TODO(dpwe):
#   - Implement synth_for_channel[channel]
#   - Add sustain to Synth
#   - Wrap amy.send(voices) as a voice_source with VoiceObjects
#   - Provide a DrumSynth (same API, no voice rotation etc.)


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

    def note_on(self, note, vel, time=None):
        amy.send(voices=self.amy_voice, note=note, vel=vel, time=time)

    def note_off(self, time=None):
        amy.send(voices=self.amy_voice, vel=0, time=time)


class VoiceSource:
    """Manage the pool of amy voices. Provide voice_source for Synth objects."""
    # Class-wide record of which voice to allocate next.
    next_voice = 0

    def get_new_voices(self, num_voices):
        self.voice_nums = list(range(VoiceSource.next_voice, VoiceSource.next_voice + num_voices))
        VoiceSource.next_voice += num_voices
        voice_objects = []
        for voice_num in self.voice_nums:
            voice_objects.append(VoiceObject(voice_num))
        return voice_objects

    def program_change(self, patch_num):
        time.sleep(0.1)  # "AMY queue will fill if not slept."
        amy.send(voices=','.join([str(v) for v in self.voice_nums]), load_patch=patch_num)

    def control_change(self, control, value):
        # Aha.  This is the sticking point.
        print('control_change not implemented for amy-managed voices.')
        pass


class Synth:
    """Manage a polyphonic synthesizer by rotating among a fixed pool of voices.

    Provides methods:
      synth.note_on(midi_note, velocity)
      synth.note_off(midi_note)
      synth.control_change(control, value)
      synth.program_change(patch_num)
  
    Argument voice_source provides the following methods:
      voice_source.get_new_voices(num_voices) returns num_voices VoiceObjects.
        VoiceObjects accept voice.note_on(note, vel), voice.note_off()
      voice_source.program_change(patch_num) changes preset for all voices.
      voice_source.control_change(control, value) modifies a parameter for all
          voices.
    """

    def __init__(self, voice_source, num_voices=6):
        self.voice_source = voice_source
        self.voices = voice_source.get_new_voices(num_voices)
        self.released_voices = Queue(num_voices, name='Released')
        for voice_num in range(num_voices):
            self.released_voices.put(voice_num)
        self.active_voices = Queue(num_voices, name='Active')
        # Dict to look up active voice from note number, for note-off.
        self.voice_of_note = {}
        self.note_of_voice = [None] * num_voices
        self.sustaining = False
        self.sustained_notes = set()

    def _get_next_voice(self):
        """Return the next voice to use."""
        # First try free/released_voices in order, then steal from active_voices.
        if not self.released_voices.empty():
            return self.released_voices.get()
        # We have to steal an active voice.
        stolen_voice = self.active_voices.get()
        print('Stealing voice for', self.note_of_voice[stolen_voice])
        self._voice_off(stolen_voice)
        return stolen_voice

    def _voice_off(self, voice):
        """Terminate voice, update note_of_voice, but don't alter the queues."""
        self.voices[voice].note_off()
        # We no longer have a voice playing this note.
        del self.voice_of_note[self.note_of_voice[voice]]
        self.note_of_voice[voice] =  None

    def note_off(self, note):
        if self.sustaining:
            self.sustained_notes.add(note)
            return
        if note not in self.voice_of_note:
            return
        old_voice = self.voice_of_note[note]
        self._voice_off(old_voice)
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
                new_voice = self._get_next_voice()
                self.active_voices.put(new_voice)
                self.voice_of_note[note] = new_voice
                self.note_of_voice[new_voice] = note
                self.voices[new_voice].note_on(note, velocity)

    def sustain(self, state):
        """Turn sustain on/off."""
        if state:
            self.sustained = True
        else:
            self.sustained = False
            for midinote in self.sustained_notes:
                self.note_off(midinote)
            self.sustained_notes = set()

    def program_change(self, patch_number):
        self.voice_source.program_change(patch_number)

    def control_change(self, control, value):
        self.voice_source.control_change(control, value)


class DrumSynth:
    """Simplified Synth for Drum channel (10)."""
    PCM_PATCHES = 29

    def __init__(self, num_voices=10):
        self.oscs = range(amy.AMY_OSCS - num_voices, amy.AMY_OSCS)
        self.next_osc = 0
        self.note_to_osc = {}

    def note_on(self, note, velocity):
        osc = self.oscs[self.next_osc]
        self.next_osc = (self.next_osc + 1) % len(self.oscs)
        amy.send(osc=osc, wave=amy.PCM, patch=note % self.PCM_PATCHES, vel=velocity, freq=0)
        self.note_to_osc[note] = osc

    def note_off(self, note):
        # Drums don't really need note-offs, but handle them anyway.
        try:
            osc = self.note_to_osc[note]
            amy.send(osc=osc, vel=0)
            del self.note_to_osc[note]
        except KeyError:
            pass

    # Rest of Synth protocol doesn't do anything for drums.
    def sustain(self, state):
        pass

    def program_change(self, patch_number):
        pass

    def control_change(self, control, value):
        pass


def ensure_midi_setup():
    global midi_synths
    if not midi_synths:
        # Tulip defaults, 4 note polyphony on channel 1
        # drum machine always on channel 10
        midi_synths = MidiSynthsConfig(
            voices_per_channel={1: 4, 10: 10},
            patch_per_channel={1: 2},
        )
        tulip.midi_add_callback(midi_event_cb)


def midi_event_cb(x):
    """Callback that takes MIDI note on/off to create Note objects."""
    ensure_midi_setup()
    m = tulip.midi_in() 
    while m is not None and len(m) > 0:
        channel = (m[0] & 0x0F) + 1
        message = m[0] & 0xF0
        if message == 0x90:  # Note on.
            midinote = m[1]
            midivel = m[2]
            vel = midivel / 127.
            midi_synths.synth_per_channel[channel].note_on(midinote, vel)
        elif message == 0x80:  # Note off.
            midinote = m[1]
            midi_synths.synth_per_channel[channel].note_off(midinote)
        elif message == 0xc0:  # Program change
            midi_synths.synth_per_channel[channel].program_change(m[1])
        elif message == 0xe0:  # Pitch bend. m[2] is MSB, m[1] is LSB. 14 bits 
            pb_value = ((m[2] << 7) | (m[1])) - 8192 # -8192-8192, where 0 is nothing
            amy_value = float(pb_value)/(8192*6.0) # convert to -2 / +2 semitones
            amy.send(pitch_bend=amy_value)
        elif message == 0xb0 and m[1] == 0x40:
            midi_synths.synth_per_channel[channel].sustain(m[2])
    
        # Are there more events waiting?
        m = m[3:]
        if len(m) == 0:
            m = tulip.midi_in()


# Keep this -- this is a tulip API 
def music_map(channel, patch_number=None, voice_count=None):
    """API to set a patch and polyphony for a given MIDI channel."""
    midi_synths.music_map(channel, patch_number, voice_count)
    try:
        voices.refresh()  # Update voices UI if it is running.
    except:
        pass


arpegg = None

def midi_step(t):
    if(tulip.seq_ticks() > tulip.seq_ppq()):
        ensure_midi_setup()
    if(arpegg):
        play_arpegg_step(t, arpegg_mode)


def setup():
    # we can't setup on boot right away as we need to get the bleep going and the alles setup done, so wait on a callback
    tulip.seq_add_callback(midi_step, int(tulip.seq_ppq()/2))


