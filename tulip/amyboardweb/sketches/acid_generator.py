# AMYboard Sketch
# DESCRIPTION: Generative acid track: TB-303 bassline + 808 drums in classic acid house style.
# Top-level code runs once at boot. loop() is called every 32nd note.
import amyboard, amy, sequencer
import random

amy.send(synth=1, portamento=50, osc=0)
sequencer.tempo(138)  # Classic acid tempo

# Display
amyboard.display.fill(0)
amyboard.display.text("ACID", 0, 0, 255)
amyboard.display.text("GENERATOR", 0, 16, 255)
amyboard.display_refresh()

# --- Synth setup ---

# Synth 1: TB-303 bass — already configured via knobs
# Synth 10: 808 drums (GM mapping with synth_flags=3)
amy.send(synth=10, num_voices=5, oscs_per_voice=1, synth_flags=3, amp=5, wave=amy.PCM)

# A little reverb, plus some chorus for the 303
amy.send(chorus="0.6,2,0.3")
amy.send(reverb="0.5,0.3,0.05")

# --- Music data ---

# 16-step patterns (16th notes), 2 steps per 32nd-note loop call
# We trigger on even steps only (every other loop call = 16th note grid)
STEPS = 16  # 16th notes per bar

# Scales for the 303 bassline (minor pentatonic is classic acid)
# We'll transpose these to different roots
SCALE = [0, 3, 5, 7, 10]  # minor pentatonic intervals
ROOTS = [33, 36, 38, 40]  # A1, C2, D2, E2 — MIDI notes for bass roots

root = random.choice(ROOTS)

# --- 303 pattern generator ---
def make_303_pattern():
    """Generate a 16-step acid bassline pattern.
    Each step is (note_offset, vel, accent, slide, rest).
    Classic 303 patterns have lots of rests, accents, and slides."""
    pattern = []
    for i in range(STEPS):
        rest = random.random() < 0.3  # 30% chance of rest
        if rest:
            pattern.append(None)
        else:
            note = random.choice(SCALE)
            # Occasionally jump up an octave
            if random.random() < 0.2:
                note += 12
            accent = random.random() < 0.35
            slide = random.random() < 0.3
            vel = 0.9 if accent else 0.55
            pattern.append((note, vel, accent, slide))
    return pattern

# --- 808 pattern generators ---
def make_kick_pattern():
    """Acid-style kick: four-on-the-floor with occasional doubles."""
    p = [0] * STEPS
    for i in range(0, STEPS, 4):  # quarter notes
        p[i] = 1
    # Sometimes add a pickup kick
    if random.random() < 0.5:
        p[random.choice([3, 7, 15])] = 1
    return p

def make_drum_pattern():
    """Returns (kick, snare, closed_hat, open_hat, clap) patterns."""
    kick = make_kick_pattern()

    # Snare on 2 and 4 (steps 4 and 12)
    snare = [0] * STEPS
    snare[4] = 1
    snare[12] = 1
    # Occasional ghost snare
    if random.random() < 0.4:
        snare[random.choice([7, 10, 15])] = 1

    # Closed hats: 8th or 16th note patterns
    hat = [0] * STEPS
    if random.random() < 0.6:
        # 16th note hats
        hat = [1] * STEPS
    else:
        # 8th note hats
        for i in range(0, STEPS, 2):
            hat[i] = 1

    # Open hat on offbeats
    ohat = [0] * STEPS
    if random.random() < 0.7:
        for i in [2, 6, 10, 14]:  # offbeat 8ths
            if random.random() < 0.4:
                ohat[i] = 1
    # When open hat hits, mute the closed hat
    for i in range(STEPS):
        if ohat[i]:
            hat[i] = 0

    # Clap — usually with snare or slightly offset
    clap = [0] * STEPS
    clap[4] = 1
    clap[12] = 1

    return kick, snare, hat, ohat, clap

# GM drum note numbers
KICK = 36
SNARE = 38
CLAP = 39
CLOSED_HAT = 42
OPEN_HAT = 46

# --- Initial patterns ---
bass_pattern = make_303_pattern()
kick_pat, snare_pat, hat_pat, ohat_pat, clap_pat = make_drum_pattern()

# --- State ---
step = 0         # 32nd note counter
bar_count = 0
prev_note = None  # for slide tracking

def get_16th():
    """Current 16th-note step (0-15) from 32nd-note counter."""
    return (step // 2) % STEPS

def is_16th_boundary():
    """True on every other 32nd note (= 16th note grid)."""
    return step % 2 == 0

def loop():
    global step, bar_count, bass_pattern, prev_note
    global kick_pat, snare_pat, hat_pat, ohat_pat, clap_pat, root
    step += 1
    if step < 5:
        return

    if not is_16th_boundary():
        return

    s = get_16th()

    # --- 808 Drums ---
    if kick_pat[s]:
        amy.send(synth=10, note=KICK, vel=1.0)
    if snare_pat[s]:
        amy.send(synth=10, note=SNARE, vel=0.8)
    if clap_pat[s]:
        amy.send(synth=10, note=CLAP, vel=0.7)
    if hat_pat[s]:
        amy.send(synth=10, note=CLOSED_HAT, vel=0.5)
    if ohat_pat[s]:
        amy.send(synth=10, note=OPEN_HAT, vel=0.4)

    # --- 303 Bassline ---
    bs = bass_pattern[s]
    if bs is None:
        # Rest — note off
        if prev_note is not None:
            amy.send(synth=1, vel=0)
            prev_note = None
    else:
        note_offset, vel, accent, slide = bs
        midi_note = root + note_offset
        if slide and prev_note is not None:
            # Slide: don't re-trigger, just change pitch (portamento feel)
            amy.send(synth=1, note=midi_note, vel=vel)
        else:
            # Normal trigger
            if prev_note is not None:
                amy.send(synth=1, vel=0)
            amy.send(synth=1, note=midi_note, vel=vel)
        prev_note = midi_note

    # --- Variation: new patterns every 4 bars ---
    if s == 0:
        bar_count += 1
        if bar_count % 4 == 0:
            # New 303 pattern
            bass_pattern = make_303_pattern()
            # Maybe shift root
            if random.random() < 0.3:
                root = random.choice(ROOTS)
            # Maybe new drum pattern
            if random.random() < 0.3:
                kick_pat, snare_pat, hat_pat, ohat_pat, clap_pat = make_drum_pattern()

    # --- Display ---
    if s == 0:
        amyboard.display.fill(0)
        amyboard.display.text("ACID", 0, 0, 255)
        note_names = ["A","Bb","B","C","Db","D","Eb","E","F","Gb","G","Ab"]
        rname = note_names[root % 12]
        amyboard.display.text("root:" + rname, 0, 12, 255)
        # Show pattern as visual grid
        row = ""
        for i in range(STEPS):
            if bass_pattern[i] is None:
                row += "."
            elif bass_pattern[i][2]:  # accent
                row += "X"
            else:
                row += "o"
        amyboard.display.text(row, 0, 28, 255)
        # Drum line
        drow = ""
        for i in range(STEPS):
            if kick_pat[i]:
                drow += "K"
            elif snare_pat[i] or clap_pat[i]:
                drow += "S"
            elif ohat_pat[i]:
                drow += "O"
            elif hat_pat[i]:
                drow += "h"
            else:
                drow += "."
        amyboard.display.text(drow, 0, 40, 255)
        amyboard.display_refresh()

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
i1ic255Z
i1iv6in4Z
i1v0w20F469.410,1.000,,,5.000R1.754m51c2L1G4A,,1000,0.200,100,0.000B0,1.000,1000,0.200,1000,0.000Z
i1v1w4a,,0.000f4.000,0.000,,,,,0.000A,,10000,Z
i1v2w2a,,0.000,0.000c3L1Z
i1v3w2a0.001,,0.000,0.000f220.000L1Z
i1V1.000x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k0.600,2.000,0.300,0.500h0.500,0.300,0.050,3000.000Z
i10ic255Z
i10iv5in1Z
i10if3Z
i10v0w7p1a5.000Z
"""