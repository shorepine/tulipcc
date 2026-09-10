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

# Start/stop the sequencer transport, independent of MIDI clock sync
# (tulip.external_midi_sync). Sent as an AMY wire command (sequencer_run), so it
# works identically on firmware and the AMYboard web (wasm) build, where AMY is
# a separate module reachable only via the wire protocol. Used by the AMYboard
# Online Start/Stop buttons.
def start():
    amy.send(sequencer_run=1)

def stop():
    amy.send(sequencer_run=0)

class AMYSequenceEvent:
    """One scheduled step of an AMYSequence.

    An event does not own an AMY sequencer tag; its Sequence does, and every
    event in that Sequence is scheduled under it. AMY accumulates events on a
    tag -- each ticks= adds another entry rather than replacing what was there
    -- which is what lets a whole pattern live on one tag. The cost is that
    there is no way to replace or drop ONE entry: editing or removing a step
    means clearing the tag (ticks="0,0,<tag>", which now takes the whole tag)
    and re-sending the events that remain. That is what Sequence.rebuild()
    does, and why an edit costs a message per surviving step. Use
    `with seq.batch():` to coalesce a run of edits into one rebuild.
    """
    def __init__(self, sequence):
        self.sequence = sequence

    @property
    def tag(self):
        # Events used to carry their own tag, one AMY tag per step, which ran
        # the 256-tag space out after a few minutes of editing. Kept readable
        # here for anything that still looks at it.
        return self.sequence.tag

    def amy_sequence_string(self):
        return "%d,%d,%d" % (self.tick, self.sequence.period, self.sequence.tag)

    def store(self, position, func, args=[], amy_sequenceable=False, **kwargs):
        """Record what this step plays and when, without telling AMY."""
        self.tick = self.sequence.event_length_ticks * position
        self.func = func
        self.g_args = args
        self.g_kwargs = kwargs

    def schedule(self):
        """Add this step to AMY under the Sequence's tag."""
        self.func(*self.g_args, **self.g_kwargs, ticks=self.amy_sequence_string())

    def remove(self):
        self.sequence.events.remove(self)
        self.sequence.rebuild()  # the tag is shared: put back what's left

    def update(self, position, func, args=[], amy_sequenceable=False, **kwargs):
        self.store(position, func, args=args, amy_sequenceable=amy_sequenceable, **kwargs)
        self.sequence.rebuild()  # ditto -- one entry can't be edited in place


class _Batch:
    """Context manager returned by AMYSequence.batch(); see there."""
    def __init__(self, sequence):
        self.sequence = sequence

    def __enter__(self):
        self.sequence.deferred = self.sequence.deferred + 1
        return self.sequence

    def __exit__(self, *exc):
        seq = self.sequence
        seq.deferred = seq.deferred - 1
        if seq.deferred == 0 and seq.dirty:
            seq.dirty = False
            seq.rebuild()
        return False


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

class TulipSequence(Sequence):
    def __init__(self, divider, func):
        super().__init__(1, divider)
        self.func = func
        self.tag = tulip.seq_add_callback(self.func, 0, self.period)

    def clear(self):
        tulip.seq_remove_callback(self.tag)

class AMYSequence(Sequence):
    # One AMY sequencer tag per Sequence, taken at construction and held for
    # the Sequence's life. AMY's tag space is max_sequencer_tags (256 by
    # default) and this counter doesn't recycle, so a session that builds
    # hundreds of Sequences will eventually run out -- but a Sequence is an
    # app-sized object, where the old one-tag-per-step scheme burned a tag on
    # every step edit and ran out during ordinary use.
    SEQUENCE_TAG = 0

    def __init__(self, length=1, divider=8):
        super().__init__(length, divider)
        self.tag = AMYSequence.SEQUENCE_TAG
        AMYSequence.SEQUENCE_TAG = AMYSequence.SEQUENCE_TAG + 1
        self.deferred = 0   # depth of open batch() blocks
        self.dirty = False  # a rebuild was asked for while batching

    def add(self, position, func, args=[], amy_sequenceable=False, **kwargs):
        e = AMYSequenceEvent(self)
        e.store(position, func, args=args, amy_sequenceable=amy_sequenceable, **kwargs)
        self.events = self.events + [e]
        if self.deferred:
            self.dirty = True
        else:
            e.schedule()  # adding accumulates, so this needs no rebuild
        return e

    def rebuild(self):
        """Re-send every event, replacing what AMY holds under our tag.

        Editing or removing a step needs this because AMY can only erase a
        whole tag, never one entry in it. Inside a batch() it just marks the
        Sequence dirty and the rebuild happens once, on the way out.
        """
        if self.deferred:
            self.dirty = True
            return
        amy.send(ticks=",,%d" % (self.tag))  # neither tick nor period: clear the tag
        for e in self.events:
            e.schedule()

    def batch(self):
        """Coalesce the rebuilds from a run of edits into a single one:

            with seq.batch():
                for switch in row:
                    switch.sequencer_event.update(...)

        Without it each edit rebuilds the whole tag, so N edits to a sequence
        of N steps cost N**2 messages. Nests, and rebuilds only if something
        inside actually changed.
        """
        return _Batch(self)

    def clear(self):
        # One message, whatever the sequence holds -- the tag is ours alone.
        amy.send(ticks=",,%d" % (self.tag))
        self.events = []
