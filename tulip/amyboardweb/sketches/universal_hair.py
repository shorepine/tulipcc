# UNIVERSAL HAIR soundtrack  universalhair.net
# By Douglas Repetto
# adapted by Dan Ellis

# Nobody is happy with universal hair

import amy, random

# Based on Juno Voice patch - single filtered pulse wave with a little vibrato.
amy.send(synth=1, num_voices=4, oscs_per_voice=2)
amy.send(synth=1, osc=0, wave=amy.PULSE, mod_source=1, filter_type=amy.FILTER_LPF24,
         freq='440,1,,,,0.003', duty=0.598, portamento=1000, filter_freq='777,0,,0.2',
         resonance=5, bp0='0,1,72,1,10000,0')
amy.send(synth=1, osc=1, wave=amy.TRIANGLE, freq=2.98)
amy.send(eq='-8,0,0', chorus='1,,0.5,0.5', reverb='1,0.9,0.16')

pan_values = [0.05, 0.35, 0.65, 0.95]
root = 60
current_notes = [0, 0, 0, 0]
target_notes = [0, 0, 0, 0]
root_offsets =[0, 3, 7, 10]
next_notes_to_change = []

def rand_without(low, high, without):
    """Return a value betwen `low` and `high` excluding `without`."""
    r = random.randint(low, high - 1)
    return r + (r >= without)

def rand_perm(n):
    """Random permutation of numbers 0..n-1"""
    values = list(range(n))
    result = []
    for _ in range(n):
        index = random.randrange(len(values))
        result.append(values[index])
        del values[index]
    return result

tick = 0

def loop():
    global current_notes, target_notes, pan_values, root, root_offsets, next_notes_to_change
    global tick
    if tick >= 40:
        tick = 0
        if not next_notes_to_change:
            # We ran out of notes to change, choose a new chord and new ordering
            root = rand_without(45, 70, root)
            target_notes = [root + note for note in root_offsets]
            next_notes_to_change = rand_perm(4)
        note_to_change = next_notes_to_change[0]
        next_notes_to_change = next_notes_to_change[1:]
        current_notes[note_to_change] = target_notes[note_to_change]
        # Update the sounding voice
        amy.send(synth=1, vel=0.5, pan=pan_values[note_to_change], note=current_notes[note_to_change])
    tick += 1

        