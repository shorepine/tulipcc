# AMYboard Sketch
# Generative house track with chord progressions, bass, piano, and 808 drums.
# Top-level code runs once at boot. loop() is called every 32nd note.
import amyboard, amy, sequencer
from music import Chord, Key, Progression
import time, random

sequencer.tempo(125)  # Change this to speed up or slow down

# Display
amyboard.display.fill(0)
amyboard.display.text("HOUSE", 0, 0, 255)
amyboard.display.text("GENERATOR", 0, 16, 255)
amyboard.display_refresh()

# --- Synth setup ---

# Synth 1: Wood Piano bass (mono, low octave)
amy.send(synth=1, num_voices=1, oscs_per_voice=5)
amy.send(synth=1, osc=4, wave=amy.SINE, ratio=1, bp0='10,1,1000,0.8,100,0', amp='0.4,0,0,1', eg0_type=2, phase=0)
amy.send(synth=1, osc=3, wave=amy.SINE, ratio=0.5, bp0='0,1,1000,0,100,0', amp='1,0,0,1', eg0_type=2, phase=0)
amy.send(synth=1, osc=2, wave=amy.SINE, ratio=1, bp0='0,1,300,0.5,500,0.3,1000,0', amp='0.8,0,0,1,0,0', eg0_type=2, phase=0)
amy.send(synth=1, osc=1, wave=amy.SINE, ratio=0.495, bp0='0,1,2000,0,300,0', amp='1,0,0,1,0,0', eg0_type=2, phase=0)
amy.send(synth=1, osc=0, wave=amy.ALGO, algorithm=5, algo_source=',,4,3,2,1', bp0='0,1,1000,1,300,0', amp='4,0,1,1', freq='220,1,0,0,0,0')

# Synth 2: Piano chords (3 voices for triads)
amy.send(synth=2, patch=256, num_voices=3)

# Synth 10: 808 drums (GM mapping with synth_flags=3)
amy.send(synth=10, num_voices=4, oscs_per_voice=1, synth_flags=3, amp=5, wave=amy.PCM)

# A little reverb
amy.send(reverb="0.7,0.5,0.1")

# --- Music data ---

# Classic house chord progressions in A minor
PROGRESSIONS = [
    ["i", "VI", "III", "VII"],   # Am F C G  (classic house)
    ["i", "iv", "VI", "VII"],    # Am Dm F G
    ["i", "III", "VII", "VI"],   # Am C G F
    ["i", "VII", "VI", "VII"],   # Am G F G
]
KEY = Key("A:min")

# Pick a progression
prog = Progression(random.choice(PROGRESSIONS), KEY)
chords = prog.chords

# GM drum note numbers
KICK = 36
SNARE = 38
CLAP = 39
CLOSED_HAT = 42
OPEN_HAT = 46

# --- Pattern definitions ---
# 32 steps per bar (32nd notes), 4 bars per chord change = 128 steps per progression cycle
STEPS_PER_BAR = 32
BARS_PER_CHORD = 4
STEPS_PER_CHORD = STEPS_PER_BAR * BARS_PER_CHORD
TOTAL_STEPS = STEPS_PER_CHORD * len(chords)

# Drum patterns (32 steps per bar, repeats each bar)
# Four-on-the-floor kick: every 8 steps (quarter notes)
kick_pattern =  [1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0]
# Offbeat clap on 2 and 4
clap_pattern =  [0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0]
# Closed hats on eighth notes, open hat on offbeat quarters
hat_pattern =   [1,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0, 1,0,0,0]
ohat_pattern =  [0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,0,0, 1,0,0,0]

# Bass patterns: root and fifth, syncopated (per bar of 32 steps)
# Pattern index into chord tones: 0=root, 1=fifth
bass_patterns = [
    # root on 1, fifth on the "and" of 2, root on 3
    [(0, 0), (7, 1), (16, 0)],
    # root on 1, root octave up on "and" of 2, fifth on 3, root on "and" of 4
    [(0, 0), (6, 2), (16, 1), (22, 0)],
    # syncopated: root, rest, fifth quick, root
    [(0, 0), (10, 1), (16, 0), (26, 1)],
]

# Piano voicing: play chord on beat 2 and 4 (offbeat), with some variation
piano_patterns = [
    # offbeat stabs
    [4, 20],
    # on 2 and 4
    [8, 24],
    # dotted rhythm
    [4, 16, 28],
]

current_bass_pattern = random.choice(bass_patterns)
current_piano_pattern = random.choice(piano_patterns)

# --- State ---
step = 0
bars_played = 0

def get_chord_index():
    """Which chord in the progression are we on?"""
    return (step // STEPS_PER_CHORD) % len(chords)

def get_bar_step():
    """Position within current bar (0-31)."""
    return step % STEPS_PER_BAR

def get_chord_bar():
    """Which bar within the current chord (0-3)."""
    return (step % STEPS_PER_CHORD) // STEPS_PER_BAR

def loop():
    global step, bars_played, current_bass_pattern, current_piano_pattern, prog, chords
    step += 1
    if step < 5:
        return
    bar_step = get_bar_step()
    chord_idx = get_chord_index()
    chord = chords[chord_idx]
    chord_bar = get_chord_bar()
    root_midi = chord.notes[0].midinote(octave=2)  # bass octave
    fifth_midi = root_midi + 7  # perfect fifth above root
    octave_midi = root_midi + 12

    # --- Drums (every bar) ---
    if kick_pattern[bar_step]:
        amy.send(synth=10, note=KICK, vel=1)
    if clap_pattern[bar_step]:
        amy.send(synth=10, note=CLAP, vel=0.8)
    if hat_pattern[bar_step] and not ohat_pattern[bar_step]:
        amy.send(synth=10, note=CLOSED_HAT, vel=0.5)
    if ohat_pattern[bar_step]:
        amy.send(synth=10, note=OPEN_HAT, vel=0.4)

    # --- Bass (per bar within chord) ---
    for (trigger_step, tone_idx) in current_bass_pattern:
        if bar_step == trigger_step:
            if tone_idx == 0:
                note = root_midi
            elif tone_idx == 1:
                note = fifth_midi
            else:
                note = octave_midi
            amy.send(synth=1, note=note, vel=0.9)

    # Turn off bass between notes (simple gate)
    off_steps = set()
    for i, (trigger_step, _) in enumerate(current_bass_pattern):
        # Off 2 steps before next trigger (or end of bar)
        if i + 1 < len(current_bass_pattern):
            off_at = current_bass_pattern[i + 1][0] - 2
        else:
            off_at = STEPS_PER_BAR - 2
        if off_at >= 0:
            off_steps.add(off_at)
    if bar_step in off_steps:
        amy.send(synth=1, vel=0, note=root_midi)

    # --- Piano chords (per bar within chord) ---
    chord_notes = chord.midinotes(octave=4)
    for trigger_step in current_piano_pattern:
        if bar_step == trigger_step:
            for cn in chord_notes:
                amy.send(synth=2, note=cn, vel=0.5)
        # Short stab: off 3 steps later
        if bar_step == (trigger_step + 3) % STEPS_PER_BAR:
            for cn in chord_notes:
                amy.send(synth=2, note=cn, vel=0)

    # --- Variation: change patterns every 4 bars ---
    if bar_step == 0 and chord_bar == 0 and step > 0:
        bars_played += 4
        # Every 16 bars, maybe pick a new progression
        if bars_played % 16 == 0 and random.random() < 0.4:
            prog = Progression(random.choice(PROGRESSIONS), KEY)
            chords = prog.chords
        # Every 4 bars, maybe vary bass/piano patterns
        if random.random() < 0.3:
            current_bass_pattern = random.choice(bass_patterns)
        if random.random() < 0.3:
            current_piano_pattern = random.choice(piano_patterns)

    # --- Display update (only on bar boundaries to save CPU) ---
    if bar_step == 0:
        amyboard.display.fill(0)
        amyboard.display.text("HOUSE", 0, 0, 255)
        names = " ".join([c.root_note.name() + ("m" if "min" in c.annotation_name else "") for c in chords])
        amyboard.display.text(names, 0, 12, 255)
        # Show current chord highlighted
        amyboard.display.text(">"+chords[chord_idx].root_note.name(), 0, 24, 255)
        amyboard.display_refresh()
