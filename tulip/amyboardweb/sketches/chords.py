# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
# DESCRIPTION: Each MIDI key triggers a major chord rooted at that key.
# In simulate mode, you have to use MIDI input, not the onscreen keyboard.

import amy, midi
from music import Chord

# Tell synth 1 to not grab midi notes - we'll play them from this sketch.
amy.send(synth=1, grab_midi_notes=0)


ROOT_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']

# Per-root midi notes currently sounding so we can release them on note off.
active = {}


def chord_for_root(root_midi):
    """Return midi notes for a major chord rooted at root_midi."""
    name = ROOT_NAMES[root_midi % 12]
    c = Chord(name + ":maj")
    return [root_midi + offset for offset in c.annotations]


def midi_cb(m):
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    note = m[1]
    vel = m[2]
    if status == 0x90 and vel > 0:
        notes = chord_for_root(note)
        active[note] = notes
        v = vel / 127.0
        for n in notes:
            amy.send(synth=1, note=n, vel=v)
    elif status == 0x80 or (status == 0x90 and vel == 0):
        for n in active.pop(note, ()):
            amy.send(synth=1, note=n, vel=0)


midi.add_callback(midi_cb)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
