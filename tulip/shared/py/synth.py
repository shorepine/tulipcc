# synth.py
# manages AMY voice allocated Synths
# This version uses the AMY synth= mechanism.

import amy
import tulip
from patches import drumkit
from tulip_queue import Queue
import midi

class PatchSynth:
    """Manage a polyphonic synthesizer by rotating among a fixed pool of voices.

    Provides methods:
      synth.note_on(midi_note, velocity, time=None, sequence=None)
      synth.note_off(midi_note, time=None, sequence=None)
      synth.all_notes_off()
      synth.program_change(patch_num) changes preset for all voices.
      synth.control_change(control, value) modifies a parameter for all voices.
    Provides read-back attributes (for voices.py UI):
      synth.patch_number
      synth.patch_state  - patch-specific data only used by clients e.g. UI state
    """

    # Class-wide record of which amy synths and patches are in use.
    amy_synth_allocated = set()
    amy_synth_next = 16
    amy_patch_allocated = set()
    amy_patch_next = 1024

    @classmethod
    def reset(cls):
        """Resets AMY and PatchSynth's tracking of its state."""
        cls.amy_synth_allocated = set()
        cls.amy_synth_next = 16
        amy.reset()

    def __init__(self, num_voices=4, patch_number=None, patch_string=None, channel=None, synth_flags=0, synth_already_initialized=False):
        if channel is not None:
            self.synth = channel
        else:
            self.synth = PatchSynth.amy_synth_next
            PatchSynth.amy_synth_next += 1
        self.num_voices = num_voices
        self.patch_string = patch_string
        self.patch_number = patch_number
        self.patch_state = None
        self.synth_flags = synth_flags
        # The actual setup of the synth is deferred until the first time this
        # synth is used.  The MIDI channel is not part of this API, but for AMY
        # the MIDI channel determines the synth number, so before allocating a
        # different (non-MIDI) synth number, let's wait and see if we're allocated
        # to a channel.
        self._initialized = synth_already_initialized

    def deferred_init(self):
        """Finish synth initialization once we can assume all voices are available."""
        if not self._initialized:
            if self.patch_number is None and self.patch_string is None:
                raise ValueError('Neither patch_number nor patch_string are set')
            amy_send_args = {'num_voices': self.num_voices, 'synth_flags': self.synth_flags}
            if self.patch_string:
                amy_send_args['patch'] = self.patch_string
            else:   # Assume patch_number is set
                amy_send_args['patch_number'] = self.patch_number
            self.amy_send(**amy_send_args)
            self._initialized = True
            # Fields used by UI
            #self.num_voices = num_voices

    # send an AMY message to the voices in this synth
    def amy_send(self, **kwargs):
        amy.send(synth=self.synth, **kwargs)

    def note_off(self, note, time=None, sequence=None):
        self.amy_send(note=note, vel=0, time=time, sequence=sequence)
        
    def all_notes_off(self):
        self.note_off(note=0)  # Note=0 means all notes off.

    def note_on(self, note, velocity=1, time=None, sequence=None, **kwargs):
        self.deferred_init()
        if self.synth is None:
            # Note on after synth.release()?
            raise ValueError('PatchSynth note on with no AMY synth - synth has been released?')
        if velocity == 0:
            self.note_off(note, time=time, sequence=sequence)
        else:  # Velocity > 0, note on.
            self.amy_send(note=note, vel=velocity, time=time, sequence=sequence, **kwargs)

    def sustain(self, state):
        """Turn sustain on/off."""
        self.deferred_init()
        self.amy_send(pedal=state)

    def get_patch_state(self):
        self.deferred_init()
        return self.patch_state

    def set_patch_state(self, state):
        self.deferred_init()
        self.patch_state = state

    def program_change(self, patch_number):
        import time
        if patch_number != self.patch_number:
            self.patch_number = patch_number
            # Reset any modified state due to previous patch modifications.
            self.patch_state = None
            if not self._initialized:
                self.deferred_init()
            else:
                self.amy_send(patch_number=patch_number) 

    def control_change(self, control, value):
        print('control_change not implemented for amy-managed voices.')

    def release(self):
        """Called to terminate this synth and release its amy_voice resources."""
        # Release the AMY synth by setting its num_voices to 0.
        self.amy_send(num_voices=0)
        # Mark this object as not usable.
        self._initialized = True
        self.synth = None

    def set_channel(self, channel):
        if self._initialized:
            # We need to renumber the old synth, if we made it already.
            self.amy_send(to_synth=channel)
        # Future commands go to the new synth
        self.synth = channel

    # This used to be a method on OscSynth, but it's just an alias for amy_send.
    def update_oscs(self, **kwargs):
        """Update all our oscs with the params in the dict."""
        # Pass the commands to all the voices.
        self.deferred_init()
        self.amy_send(**kwargs)



class OscSynth(PatchSynth):
    """PatchSynth that uses one osc per note.  Osc parameters are specified at initialization."""

    def __init__(self, num_voices=4, channel=None, synth_flags=0, **kwargs):
        """Create a synth that plays the specified note on a single osc configured with keyword args."""
        # Make sure the patch string isn't empty.
        if 'osc' not in kwargs:
            kwargs['osc'] = 0
        patch_string = amy.message(**kwargs)
        super().__init__(num_voices=num_voices, channel=channel, synth_flags=synth_flags, patch_string=patch_string)


class DrumSynth(PatchSynth):
    """Synth for Drum channel (10).
    Each MIDI note plays a separately-configured sound.
    """

    def __init__(self, num_voices=4, channel=None):
        super().__init__(num_voices=num_voices, channel=channel, patch_string='w7f0', synth_flags=3)  # synth_flags 3 means do the drum translation and ignore note offs.


