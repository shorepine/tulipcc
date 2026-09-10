"""Host-side test of sequencer.py's AMYSequence against a real AMY.

sequencer.py is frozen into the firmware, so this stubs the two firmware-only
modules it imports (tulip, synth) and drives AMYSequence against the pip-
installed `amy` module, reading back what AMY actually holds via its own
`debug=6` dump.

What it pins down is the accumulate rule. AMY's `ticks="tick,period,tag"` used
to REPLACE whatever sat on a tag; it now ADDS another event under it, so a tag
can carry a whole pattern. sequencer.py leans on that: an AMYSequence takes ONE
tag and schedules all of its steps under it, where it used to burn a tag per
step and exhaust AMY's 256 after a few minutes of editing.

The consequence, and the thing most of these checks are about, is that AMY can
only erase a whole tag -- never one entry in it. (The erase is the same
ticks="0,0,<tag>" cancel that has always been there; it just takes everything
on the tag now.) So editing or removing a step has to rebuild the tag from the
surviving events, and a naive edit loop costs a message per step per edit;
batch() exists to collapse that.

Needs the `amy` module built from the pinned submodule (`cd amy && make
amy-module`).

Run from anywhere:  python3 tulip/shared/test_amy_sequencer.py
"""
import os, re, sys, tempfile, types

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "py"))
# sequencer.py imports these at module level; only its Tulip-callback half uses
# them, and nothing here touches that half.
for name in ("tulip", "synth"):
    m = types.ModuleType(name)
    m.seq_bpm = lambda *a: 108
    m.seq_add_callback = lambda *a, **k: 0
    m.seq_remove_callback = lambda *a, **k: None
    m.seq_remove_callbacks = lambda *a, **k: None
    sys.modules[name] = m

import amy
import sequencer

_ENTRY = re.compile(r'sequence slot \d+ tag (\d+) tick (\d+) period (\d+) wire "(.*)"')


def entries():
    """What AMY currently has scheduled: [(tag, tick, wire), ...], sorted.

    amy.send(debug=6) prints to the C library's stderr, so grab fd 2 rather
    than sys.stderr.
    """
    sys.stderr.flush()
    saved = os.dup(2)
    path = tempfile.mktemp(suffix=".seqdebug")
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
    os.dup2(fd, 2)
    os.close(fd)
    try:
        amy.send(debug=6)
        sys.stderr.flush()
    finally:
        os.dup2(saved, 2)
        os.close(saved)
    found = []
    with open(path) as f:
        for line in f:
            m = _ENTRY.match(line)
            if m:
                found.append((int(m.group(1)), int(m.group(2)), m.group(4)))
    os.unlink(path)
    return sorted(found)


failures = 0


def check(cond, msg):
    global failures
    print(("  ok   " if cond else "  FAIL ") + msg)
    if not cond:
        failures += 1


def note_event(seq, position, note):
    return seq.add(position, amy.send, osc=0, wave=amy.SINE, note=note, vel=1)


class CountingSend:
    """Wraps amy.send to count the wire messages an operation costs."""
    def __init__(self):
        self.n = 0
        self.real = amy.send

    def __call__(self, *a, **k):
        self.n += 1
        return self.real(*a, **k)

    def __enter__(self):
        self.n = 0
        amy.send = self
        return self

    def __exit__(self, *exc):
        amy.send = self.real
        return False


def main():
    global failures
    amy.restart()
    seq = sequencer.AMYSequence(length=1, divider=8)   # 8 steps of 24 ticks

    print("adding steps")
    e0 = note_event(seq, 0, 36)
    e1 = note_event(seq, 2, 38)
    e2 = note_event(seq, 4, 42)
    amy.render(0.2)
    check(len(entries()) == 3, "three added steps give three entries")

    print("re-editing a step (the accumulate case)")
    e1.update(6, amy.send, osc=0, wave=amy.SINE, note=45, vel=1)
    amy.render(0.2)
    es = entries()
    check(len(es) == 3, "editing a step leaves three entries, not four: %r" % (es,))
    ticks = dict((tag, tick) for tag, tick, _ in es)
    check(ticks.get(e1.tag) == 144, "the edited step moved to tick 144 (got %r)" % (ticks.get(e1.tag),))
    check(not any("n38" in w for _, _, w in es), "the pre-edit version of the step is gone")
    check(any("n45" in w for _, _, w in es), "the post-edit version is there")

    for position in (1, 3, 5):
        e1.update(position, amy.send, osc=0, wave=amy.SINE, note=45, vel=1)
    amy.render(0.2)
    check(len(entries()) == 3, "three further edits still leave three entries")

    print("one tag for the whole sequence")
    es = entries()
    check(len(set(tag for tag, _, _ in es)) == 1,
          "all three steps share one tag: %r" % ([t for t, _, _ in es],))
    check(es[0][0] == seq.tag, "and it is the Sequence's tag (%d)" % seq.tag)
    check(e0.tag == seq.tag and e2.tag == seq.tag,
          "an event reports its Sequence's tag")
    other = sequencer.AMYSequence(length=1, divider=8)
    check(other.tag != seq.tag, "a second Sequence gets a different tag")
    note_event(other, 0, 60)
    amy.render(0.2)
    check(len(entries()) == 4, "the second Sequence's step is scheduled too")
    check(len([1 for tag, _, _ in entries() if tag == seq.tag]) == 3,
          "and it did not disturb the first Sequence")

    print("batching")
    # Editing every step one at a time rebuilds the tag every time. Inside a
    # batch() block it should rebuild exactly once, on the way out.
    with CountingSend() as c:
        for e in (e0, e1, e2):
            e.update(2, amy.send, osc=0, wave=amy.SINE, note=50, vel=1)
    unbatched = c.n
    with CountingSend() as c:
        with seq.batch():
            for e in (e0, e1, e2):
                e.update(3, amy.send, osc=0, wave=amy.SINE, note=50, vel=1)
    batched = c.n
    print("       (%d messages unbatched, %d batched)" % (unbatched, batched))
    check(batched == 4, "a batch of three edits costs one reset + three steps (got %d)" % batched)
    check(batched < unbatched, "which is fewer than editing one at a time (%d)" % unbatched)
    amy.render(0.2)
    check(len(entries()) == 4, "and the sequence still holds three steps (+1 elsewhere)")
    other.clear()
    amy.render(0.2)

    # drums.py drives all three operations from one batched loop (a row's
    # switches can be turned on, edited, or turned off in the same pass).
    print("mixed operations in one batch")
    with seq.batch():
        e2.update(7, amy.send, osc=0, wave=amy.SINE, note=55, vel=1)
        e3 = note_event(seq, 5, 57)
        e0.remove()
    amy.render(0.2)
    es = entries()
    check(len(es) == 3, "an edit, an add and a remove batch to the right three: %r" % (es,))
    check(any("n55" in w for _, _, w in es), "the edited step is there")
    check(any("n57" in w for _, _, w in es), "the added step is there")
    check(not any("n36" in w for _, _, w in es), "the removed step is not")
    check(all(tag == seq.tag for tag, _, _ in es), "all still on the Sequence's tag")
    e3.remove()
    e0 = seq.add(0, amy.send, osc=0, wave=amy.SINE, note=36, vel=1)
    e2.update(4, amy.send, osc=0, wave=amy.SINE, note=42, vel=1)
    amy.render(0.2)

    print("removing")
    e1.remove()
    amy.render(0.2)
    es = entries()
    check(len(es) == 2, "removing a step leaves the other two: %r" % (es,))
    check(e1 not in seq.events, "and the event is off the Sequence's list")
    check(all(tag == seq.tag for tag, _, _ in es), "the survivors kept the Sequence's tag")

    with CountingSend() as c:
        seq.clear()
    amy.render(0.2)
    check(entries() == [], "Sequence.clear() removes everything")
    check(seq.events == [], "...and empties the event list")
    check(c.n == 1, "...in a single message, whatever the sequence held (got %d)" % c.n)

    if failures:
        print("\n%d check(s) FAILED" % failures)
        return 1
    print("\nall AMY sequencer.py checks passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
