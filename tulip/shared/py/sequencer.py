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

    def amy_sequence_string(self):
        return "%d,%d,%d" % (self.tick, self.sequence.period, self.tag)

    def remove(self):
        if(self.amy_sequenceable):
            amy.send(sequence=",,%d" % (self.tag))
        else:
            tulip.seq_remove_callback(self.tag)
        self.sequence.events.remove(self)

    def update(self, position, func, args=[], **kwargs):
        self.tick = self.sequence.event_length_ticks*position
        self.func = func
        self.g_args = args
        self.g_kwargs = kwargs
        self.tag = None
        try:
            self.tag = SequenceEvent.SEQUENCE_TAG 
            self.func(*self.g_args, **self.g_kwargs, sequence=self.amy_sequence_string())
            SequenceEvent.SEQUENCE_TAG = SequenceEvent.SEQUENCE_TAG + 1
            self.amy_sequenceable = True
        except TypeError:
            if(self.tag != None):
                tulip.seq_remove_callback(self.tag)
            self.tag = tulip.seq_add_callback(self.func, self.tick, self.sequence.period)
            if(self.tag == -1):
                raise Exception("No more Python sequencer callbacks available")
            self.amy_sequenceable = False

class Sequence:
    # Divider: 8 = 1/8th note, 4 = 1/4 note, 64 = 1/64 note, etc 
    def __init__(self, length=1, divider=8):
        self.events = []
        self.divider = divider
        self.length = length
        self.event_length_ticks = int((4.0/float(divider))*float(PPQ))
        self.period = self.event_length_ticks*self.length

    def clear(self):
        c = self.events.copy()
        for e in c:
            e.remove()

    def add(self, position, func, args=[], **kwargs):
        e = SequenceEvent(self)
        e.update(position, func, args, **kwargs)
        self.events = self.events + [e]
        return e


