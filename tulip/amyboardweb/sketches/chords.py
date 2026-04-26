# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: Each MIDI key triggers a major chord rooted at that key.
import amy, midi
from music import Chord

# Free up channel 1 (default synth) and put our voice on synth 17.
amy.send(synth=1, num_voices=0)
amy.send(synth=17, patch=257, num_voices=6)

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
            amy.send(synth=17, note=n, vel=v)
    elif status == 0x80 or (status == 0x90 and vel == 0):
        for n in active.pop(note, ()):
            amy.send(synth=17, note=n, vel=0)


midi.add_callback(midi_cb)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
