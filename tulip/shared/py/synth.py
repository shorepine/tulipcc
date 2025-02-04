# synth.py
# manages AMY voice allocated Synths

import amy
import tulip
from patches import drumkit
from tulip_queue import Queue

class VoiceObject:
    """Object to wrap an amy voice."""

    def __init__(self, amy_voice):
        self.amy_voice = amy_voice

    def note_on(self, note, vel, time=None, sequence=None):
        amy.send(time=time, voices=self.amy_voice, note=note, vel=vel, sequence=sequence)

    def note_off(self, time=None, sequence=None):
        amy.send(time=time, voices=self.amy_voice, vel=0, sequence=sequence)

class PatchSynth:
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
    VoiceObjects and is opaque to the PatchSynth object.
    """

    """Manage the pool of amy voices."""
    # Class-wide record of which voice to allocate next.
    allocated_amy_voices = set()
    next_amy_patch_number = 1024

    @classmethod
    def reset(cls):
        """Resets AMY and PatchSynth's tracking of its state."""
        cls.allocated_amy_voices = set()
        cls.next_amy_patch_number = 1024
        amy.reset()

    def __init__(self, num_voices=6, patch_number=None, patch_string=None):
        if patch_number is not None and patch_string is not None:
            raise ValueError('You cannot specify both patch_number and patch_string.')
        if patch_string is not None:
            patch_number = PatchSynth.next_amy_patch_number
            PatchSynth.next_amy_patch_number = patch_number + 1
            amy.send(store_patch='%d,%s' % (patch_number, patch_string))
        self._pre_init_num_voices = num_voices
        self._pre_init_patch_number = patch_number
        # The actual grabbing of AMY voices is deferred until the first time this
        # synth is used.  This is to cleanly handle the case of replacing a MIDI
        # channel synth, when a new PatchSynth object is constructed and passed to
        # config.add_synth, but the AMY voices held by the existing synth on that
        # channel are not released until add_synth() runs.  This way, the new,
        # replacement synth can use the same voice numbers when it eventually
        # does its deferred_init().
        self._initialized = False

    def deferred_init(self):
        """Finish synth initialization once we can assume all voices are available."""
        if not self._initialized:
            self._initialized = True
            num_voices = self._pre_init_num_voices
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
            self.program_change(self._pre_init_patch_number)

    def _get_new_voices(self, num_voices):
        new_voices = []
        next_amy_voice = 0
        while len(new_voices) < num_voices:
            while next_amy_voice in PatchSynth.allocated_amy_voices:
                next_amy_voice += 1
            new_voices.append(next_amy_voice)
            next_amy_voice += 1
        self.amy_voice_nums = new_voices
        PatchSynth.allocated_amy_voices.update(new_voices)
        voice_objects = []
        for amy_voice_num in self.amy_voice_nums:
            voice_objects.append(VoiceObject(amy_voice_num))
        return voice_objects

    @property
    def amy_voices(self):
        self.deferred_init()
        return [o.amy_voice for o in self.voice_objs]

    @property
    def num_voices(self):
        self.deferred_init()
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
        self.deferred_init()
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
        self.deferred_init()
        self.sustain(False)
        while not self.active_voices.empty():
            voice = self.active_voices.get()
            self._voice_off(voice)
            self.released_voices.put(voice)


    def note_on(self, note, velocity=1, time=None, sequence=None):
        self.deferred_init()
        if not self.amy_voice_nums:
            # Note on after synth.release()?
            raise ValueError('PatchSynth note on with no voices - synth has been released?')
        if velocity == 0:
            self.note_off(note, time=time, sequence=sequence)
        else:  # Velocity > 0, note on.
            if note in self.voice_of_note:
                # Send another note-on to the voice already playing this note.
                new_voice = self.voice_of_note[note]
                # Remove pending note-off from sustain, if any.
                if note in self.sustained_notes:
                    self.sustained_notes.remove(note)
            else:
                new_voice = self._get_next_voice()
                self.active_voices.put(new_voice)
                self.voice_of_note[note] = new_voice
                self.note_of_voice[new_voice] = note
            self.voice_objs[new_voice].note_on(note, velocity, time=time, sequence=sequence)

    def sustain(self, state):
        """Turn sustain on/off."""
        self.deferred_init()
        if state:
            self.sustaining = True
        else:
            self.sustaining = False
            for midinote in self.sustained_notes:
                self.note_off(midinote)
            self.sustained_notes = set()

    def get_patch_state(self):
        self.deferred_init()
        return self.patch_state

    def set_patch_state(self, state):
        self.deferred_init()
        self.patch_state = state

    def program_change(self, patch_number):
        import time
        self.deferred_init()
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
        self.deferred_init()
        # Turn off any active notes
        self.all_notes_off()
        # Return all the amy_voices
        for amy_voice in self.amy_voice_nums:
            PatchSynth.allocated_amy_voices.remove(amy_voice)
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

    # Rest of PatchSynth protocol doesn't do anything by default.
    def sustain(self, state):
        pass

    def program_change(self, patch_number):
        pass

    def control_change(self, control, value):
        pass


class OscSynth(SingleOscSynthBase):
    """PatchSynth that uses one osc per note.  Osc parameters are specified at initialization."""

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


