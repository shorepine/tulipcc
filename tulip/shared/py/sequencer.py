# sequencer.py
# Wrapper for AMY sequencer, like synth.py
import synth
import amy
import tulip
PPQ = amy.AMY_SEQUENCER_PPQ

# set and/or return the tempo 
def tempo(tempo=None):
    if(tempo is not None):
        tulip.seq_bpm(tempo)
    return tulip.seq_bpm()

def clear():
    amy.send(reset=amy.RESET_SEQUENCER)
    tulip.seq_remove_callbacks()


# musical events that accept sequence= params (note_on, note_off, more to come)
# will use the AMY C-side sequencer, and be very reliable and good timing
class MusicSequenceEvent:
    SEQUENCE_TAG = 0
    def __init__(self, sequence, tick, func, args, kwargs):
        self.sequence = sequence
        self.tick = tick
        self.tag = MusicSequenceEvent.SEQUENCE_TAG
        self.func = func
        self.g_args = args
        self.g_kwargs = kwargs
        self.update()
        MusicSequenceEvent.SEQUENCE_TAG = MusicSequenceEvent.SEQUENCE_TAG + 1

    def sequence_string(self):
        return "%d,%d,%d" % (self.tick, self.sequence.period, self.tag)

    def update(self):
        self.func(*self.g_args, **self.g_kwargs, sequence=self.sequence_string())

    def remove(self):
        amy.send(sequence=",,%d" % (self.tag))


# Just rando python functions (used for display updates, etc)
# uses mp_sched_schedule and may not execute until some ms after the expected time
class PythonSequenceEvent:
    def __init__(self, sequence, tick, func):
        self.sequence = sequence
        self.tick = tick
        self.func = func
        self.tag = None
        self.update()
        if(self.tag == -1):
            raise Exception("No more Python sequencer callbacks available")
    def update(self):
        if(self.tag != None):
            tulip.seq_remove_callback(self.tag)
        self.tag = tulip.seq_add_callback(self.func, self.tick, self.sequence.period)

    def remove(self):
        tulip.seq_remove_callback(self.tag)

class Sequence:
    # 8 = 1/8th note, 4 = 1/4 note, 64 = 1/64 note, etc 
    def __init__(self, divider=8, length=1):
        self.events = []
        self.divider = divider
        self.length = length
        self.event_length_ticks = int((4.0/float(divider))*float(PPQ))
        self.period = self.event_length_ticks*self.length

    def clear(self):
        for e in self.events:
            e.remove()
            self.events = []

    def add(self, position, func, args=[], **kwargs):
        tick = self.event_length_ticks*position
        try:
            e = MusicSequenceEvent(self, tick, func, args, kwargs)
        except TypeError: # not an AMY sequenceable event
            e = PythonSequenceEvent(self, tick, func)
        self.events = self.events + [e]
        return e

