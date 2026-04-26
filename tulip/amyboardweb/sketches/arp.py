# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
# DESCRIPTION: Hold MIDI keys; plays them in order as 8th-note arpeggios.
# In simulate mode, you have to use MIDI input, not the onscreen keyboard.

import amy, midi, tulip

# Tell synth 1 to not grab midi notes - we'll play them from this sketch.
amy.send(synth=1, grab_midi_notes=0)

# 8th note at 120 BPM = 250 ms per step.
STEP_MS = 250

held = set()           # midi note numbers currently held down
arp_idx = 0            # next index into the sorted held list to play
last_played = None     # most recently triggered note (so we can release it)
last_step_ms = 0       # last time we advanced the arp


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


def loop():
    global arp_idx, last_played, last_step_ms
    now = tulip.amy_ticks_ms()
    if now - last_step_ms < STEP_MS:
        return
    last_step_ms = now

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
