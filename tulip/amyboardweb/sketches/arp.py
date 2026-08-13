# AMYboard Sketch
# Top-level code runs once at boot. loop(tick) is called every 32nd note.
# DESCRIPTION: Hold MIDI keys; plays them in order as 8th-note arpeggios.
# In simulate mode, you have to use MIDI input, not the onscreen keyboard.

import amy, midi, amyboard

# Tell synth 1 to not grab midi notes - we'll play them from this sketch.
amy.send(synth=1, grab_midi_notes=0)

# Advance the arp every 8th note. loop() runs on the sequencer's 32nd-note
# grid, so an 8th note is 4 of those steps -- no wall clock, and it follows
# the tempo instead of assuming one (this used to be a hardcoded 250 ms,
# i.e. an 8th note only while the tempo happened to be 120 BPM).
STEP_STEPS = 4

held = set()           # midi note numbers currently held down
arp_idx = 0            # next index into the sorted held list to play
last_played = None     # most recently triggered note (so we can release it)


def midi_cb(m):
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    note = m[1]
    vel = m[2]
    if status == 0x90 and vel > 0:
        held.add(note)
    elif status == 0x80 or (status == 0x90 and vel == 0):
        held.discard(note)


midi.add_callback(midi_cb)


def loop(tick):
    global arp_idx, last_played
    if (tick // amyboard.TICKS_PER_STEP) % STEP_STEPS:
        return

    # Release the previous step's note before triggering the next one.
    if last_played is not None:
        amy.send(synth=1, note=last_played, vel=0)
        last_played = None

    if not held:
        arp_idx = 0
        return

    notes = sorted(held)
    arp_idx %= len(notes)
    n = notes[arp_idx]
    amy.send(synth=1, note=n, vel=0.8)
    last_played = n
    arp_idx += 1

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
