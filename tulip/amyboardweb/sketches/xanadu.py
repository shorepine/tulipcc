# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).


"""Joseph T. Kung's XANADU, implemented for tulipcc.

See https://github.com/csound/examples/blob/master/csd/xanadu.csd for original.
"""

import math

import tulip
import synth
import amy


C0_FREQ = 440.0 / math.pow(2.0, 4 + 9/12)

def pitch2freq(pitch):
    """Convert pitch in OCT.(STEP/100) to Hz."""
    oct = math.floor(pitch)
    step = 100 * (pitch - oct)
    freq = C0_FREQ * math.pow(2.0, oct + step/12)
    return freq


def pitch2note(pitch):
    """Convert pitch in OCT.(STEP/100) to midi."""
    return int(round(12.0 * math.log2(pitch2freq(pitch) / C0_FREQ)))


def shift_pitch(pitch, shift):
    """Shift a OCT.(STEP/100) pitch by shift (also OCT.STEP)."""
    oct = math.floor(pitch)
    step = 100 * (pitch - oct)
    s_oct = math.floor(shift)
    s_step = 100 * (shift - s_oct)
    final_step = step + s_step
    final_oct = oct + s_oct + (final_step // 12)
    final_step = final_step % 12
    return final_oct + (final_step / 100)


def amy_message_of_send_args(list_of_arg_dicts):
    """Convert a list of amy.send args into a message string."""
    message = []
    for arg_dict in list_of_arg_dicts:
        message.append(amy.message(**arg_dict))
    return ''.join(message)


def note1_patch(pan=0.5):
    """Return the setup string for note1 (guitar string)."""
    osc = 0
    modosc = 1
    message = []
    return amy_message_of_send_args([
        {'osc': modosc, 'wave': amy.SINE, 'freq': 5, 'amp': 0.005},
        {'osc': osc, 'wave': amy.SAW_DOWN, 'freq': '440,1,0,0,0,1', 'mod_source': modosc,
         'filter_freq': '200,0,0,0,4', 'filter_type': amy.FILTER_LPF24, 'resonance': 0.7,
         'pan': pan},
        {'osc': osc, 'bp0': '0,1,8000,0,100,0'},
        {'osc': osc, 'bp1': '0,1,3000,0.1,100,0'},
    ])


def note2_patch(pitch_dev=0.1, pan=0.5):
    """Return the setup string for note2 (guitar echo with pitch sigh)."""
    osc = 0
    modosc = 1
    return amy_message_of_send_args([
        {'osc': modosc, 'wave': amy.SINE, 'freq': 0.1, 'phase': 0, 'amp': pitch_dev},
        {'osc': osc, 'wave': amy.SAW_DOWN, 'freq': '440,1,0,0,0,-1', 'mod_source': modosc,
         'filter_freq': '200,0,0,0,4', 'filter_type': amy.FILTER_LPF24, 'resonance': 0.7,
         'pan': pan},
        {'osc': osc, 'bp0': '0,1,8000,0,100,0'},
        {'osc': osc, 'bp1': '0,1,3000,0.1,100,0'},
    ])


def fm_note_patch(duration=7.5):
    """Patch for hand-made 2-operator FM note."""
    return amy_message_of_send_args([
        {'osc': 3, 'wave': amy.SINE, 'freq': 1.0 / duration, 'phase': 0.75, 'amp': 1},
        {'osc': 2, 'wave': amy.SINE, 'ratio': 1, 'amp': '0.5,0,0,0,0,1', 'mod_source': 3},
        {'osc': 1, 'wave': amy.SINE, 'ratio': 1, 'amp': '1,0,0,1,', 'bp0': '0,0.1,1000,1,4000,0'},
        {'osc': 0, 'wave': amy.ALGO, 'algorithm': 1, 'algo_source': ',,,,2,1', 'bp0': '0,1,1000,1,2000,0'},
    ])


def Note(pitch, vel=1.0, time=0, pitch_shift=0, second_delay=200, use_third=False):
    """Composite 'note' triggers up to 3 delayed instances."""
    global synths
    timestamp = time
    pitch += 1.0  # 1 octave up
    note = pitch2note(pitch)
    synths[0].note_on(note, vel, timestamp)
    if use_third:
        synths[1].note_on(pitch2note(shift_pitch(pitch, pitch_shift)), vel * 0.5, timestamp + second_delay)
        synths[2].note_on(pitch2note(pitch), vel * 0.25, timestamp + 2000)

def NoteFM(pitch, vel=1.0, time=0, duration=8000):
    """Play a note on the FM voice."""
    global synths
    synths[3].note_on(pitch2note(pitch), vel, time)
    # Note off
    synths[3].note_off(pitch2note(pitch), time + duration)


def broken_chord(base_pitch, intervals, start_time, **kwargs):
    """Emit a set of notes as a staggered chord."""
    for index, interval in enumerate([0] + intervals):
        pitch = shift_pitch(base_pitch, interval)
        NoteFM(pitch - 1.0, 1, start_time)
        Note(pitch, 1, start_time + 100 * index, **kwargs)

def xanadu_stage1(x):
    amy.chorus(1)
    global synths
    synths = []
    synths.append(synth.PatchSynth(num_voices=6, patch_string=note1_patch()))
    synths.append(synth.PatchSynth(num_voices=6, patch_string=note1_patch(pan=0.8)))
    synths.append(synth.PatchSynth(num_voices=6, patch_string=note2_patch(pan=0.2)))
    synths.append(synth.PatchSynth(num_voices=12, patch_string=fm_note_patch()))
    for s in synths:
        s.deferred_init()
    amy.send(volume=0.5)
    tulip.defer(xanadu_stage2, 0, 250)

def xanadu_stage2(chord):
    chords = [
        # F#7addB chord on a guitar
        {'base_pitch': 4.06, 'intervals': [.07, 1.0, 1.04, 1.05, 1.10],
         'start_time': 2000, 'pitch_shift': 0.029, 'second_delay': 1000, 'use_third' :True},
        # D6add9 chord on a guitar
        {'base_pitch': 4.02, 'intervals': [.07, 1.0, 1.04, 0.09, 1.02],
         'start_time': 9500},
        # Bmajadd11 chord on a guitar
        {'base_pitch': 4.11, 'intervals': [.07, 1.0, 1.04, 2.00, 1.05],
         'start_time': 17000},
        # Amajadd9 chord on a guitar
        {'base_pitch': 4.09, 'intervals': [.07, 2.0, 1.04, 1.02, 1.07],
         'start_time': 24500},
        # Bmajadd11 chord on a guitar
        {'base_pitch': 4.11, 'intervals': [.07, 1.0, 1.04, 2.0, 1.05],
         'start_time': 32000},
        # Gmaj6 chord on a guitar
        {'base_pitch': 4.07, 'intervals': [.07, 1.0, 1.04, 2.04, 1.09],
         'start_time': 39500},
        # F#7addB chord on a guitar
        {'base_pitch': 5.06, 'intervals': [.07, 1.0, 1.04, 1.05, 1.10],
         'start_time': 47000, 'pitch_shift': 0.029, 'second_delay': 1000, 'use_third': True},
    ]
    for c in chords:
        broken_chord(**c)


tulip.defer(xanadu_stage1, None, 250)

def loop():
    pass
