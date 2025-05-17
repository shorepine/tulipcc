# sequencer.py
# Wrapper for AMY sequencer, like synth.py
import synth
import amy
import tulip
PPQ = amy.AMY_SEQUENCER_PPQ

# set and/or return the global tempo 
def tempo(tempo=None):
    if(tempo is not None):
        tulip.seq_bpm(tempo)
    return tulip.seq_bpm()

# clears all sequence data (across all of Tulip)
def clear():
    amy.send(reset=amy.RESET_SEQUENCER)
    tulip.seq_remove_callbacks()

class SequenceEvent:
    SEQUENCE_TAG = 0
    def __init__(self, sequence):
        self.sequence = sequence
        self.amy_sequenceable = False
        self.tag = None

    def amy_sequence_string(self):
        return "%d,%d,%d" % (self.tick, self.sequence.period, self.tag)

    def remove(self):
        if(self.amy_sequenceable):
            amy.send(sequence=",,%d" % (self.tag))
        else:
            tulip.seq_remove_callback(self.tag)
        self.sequence.events.remove(self)

    def update(self, position, func, args=[], amy_sequenceable=False, **kwargs):
        self.tick = self.sequence.event_length_ticks * position
        self.func = func
        self.g_args = args
        self.g_kwargs = kwargs
        self.amy_sequenceable = amy_sequenceable
        if self.amy_sequenceable:
            if self.tag is None:
                self.tag = SequenceEvent.SEQUENCE_TAG
                SequenceEvent.SEQUENCE_TAG = SequenceEvent.SEQUENCE_TAG + 1
            sequence = self.amy_sequence_string()
            self.func(*self.g_args, **self.g_kwargs, sequence=sequence)
        else:
            if self.tag is not None:
                tulip.seq_remove_callback(self.tag)
            self.tag = tulip.seq_add_callback(self.func, self.tick, self.sequence.period)
            if self.tag == -1:
                raise Exception("No more Python sequencer callbacks available")


class Sequence:
    # Divider: 8 = 1/8th note, 4 = 1/4 note, 64 = 1/64 note, etc 
    def __init__(self, length=1, divider=8):
        self.events = []
        self.divider = divider
        self.length = length
        self.event_length_ticks = int((4.0/float(divider))*float(PPQ))
        self.period = self.event_length_ticks*self.length

    def clear(self):
        for e in list(self.events):  # list makes a temporary copy before we modify self.events.
            e.remove()  # reflexively removes e from self.events too.

    def add(self, position, func, args=[], amy_sequenceable=False, **kwargs):
        #print('seq %s add: pos %d func %s args %s kwargs %s' % (self, position, func, args, kwargs))
        e = SequenceEvent(self)
        e.update(position, func=func, args=args, amy_sequenceable=amy_sequenceable, **kwargs)
        self.events = self.events + [e]
        return e


