# AMYboard Sketch
# DESCRIPTION: Gamma9001 drum machine: three drum channels at once -- a main kit that rotates through all six banks, TR-909 hats, and a tabla/shaker percussion layer. The OLED shows the current kit, section position and beat.
# Top-level code runs once at boot. loop() is called every 32nd note.
import amy, amyboard, sequencer
import random

sequencer.tempo(118)

# --- Drum synths: one channel per kit ---
# Gamma9001 GM drum kits: 384 TR-808, 385 TR-909, 386 Linn 9000, 387 MR-12,
# 388 Tokyo Synthetics, 389 80s Power Kit, 390 Percussion. synth_flags=3
# routes notes through the kit's GM note map and ignores note offs.
# Kits 385-390 stream from the drums flash partition, so hardware needs the
# full-image flash (kit 384 alone works on firmware-only upgrades).
KITS = [(384, "TR-808"), (385, "TR-909"), (386, "Linn 9000"),
        (387, "MR-12"), (388, "Tokyo Syn"), (389, "Power")]
kit_idx = 0

# Drum kits are single-voice (amy#913): the voice is a container with one
# dedicated osc per drum sound, so num_voices must be 1 per kit synth.
amy.send(synth=10, patch=KITS[kit_idx][0], num_voices=1, synth_flags=3)  # main kit (rotates)
amy.send(synth=11, patch=385, num_voices=1, synth_flags=3)               # TR-909: hats + clap
amy.send(synth=12, patch=390, num_voices=1, synth_flags=3)               # Percussion: tabla/shaker

amy.send(reverb="0.2,0.5,0.1")

# --- OLED display: kit name, section position, beat. Safe no-ops with no
# display attached; draws to the panel in the simulator. show() only sends
# the rows that changed, in the background, so per-beat updates are cheap.
d = amyboard.display

def draw_kit():
    d.fill_rect(0, 16, 128, 8, 0)
    d.text(KITS[kit_idx][1], 0, 16, 255)

d.fill(0)
d.text("DRUM MACHINE", 0, 0, 255)
draw_kit()
d.show()

# GM drum note numbers
KICK, SNARE, CLAP, RIM = 36, 38, 39, 37
CHAT, OHAT, CRASH, RIDE = 42, 46, 49, 51
LOW_TOM, HI_TOM = 45, 50
SHAKER, TABLA_LO, TABLA_HI, TRIANGLE, COWBELL = 42, 47, 50, 51, 56

# --- Patterns: 32 steps per bar (32nd notes), 8 bars per kit section ---
STEPS_PER_BAR = 32
BARS_PER_SECTION = 8

kick_pattern  = [0, 8, 16, 24]           # four on the floor
kick_synco    = [0, 8, 16, 22, 24]       # extra push on the "and" of 3
snare_pattern = [8, 24]                  # beats 2 and 4
hat_steps     = range(0, 32, 4)          # 8th-note closed hats
ohat_pattern  = [4, 20]                  # open hat offbeats
shaker_steps  = range(2, 32, 4)          # shakers between the hats
tabla_pattern = [6, 14, 22, 30]          # percussion answers on the "e"s

step = -1

def loop():
    global step, kit_idx
    step += 1
    bar_step = step % STEPS_PER_BAR
    bar = (step // STEPS_PER_BAR) % BARS_PER_SECTION
    fill_bar = (bar == BARS_PER_SECTION - 1)

    # --- Section change: rotate the main kit every 8 bars ---
    if step > 0 and bar_step == 0 and bar == 0:
        kit_idx = (kit_idx + 1) % len(KITS)
        amy.send(synth=10, patch=KITS[kit_idx][0], num_voices=1, synth_flags=3)
        amy.send(synth=10, note=CRASH, vel=0.7)  # announce the new kit
        draw_kit()

    # --- Main kit (synth 10) ---
    kicks = kick_synco if bar % 4 == 3 else kick_pattern
    if bar_step in kicks:
        amy.send(synth=10, note=KICK, vel=1)
    if bar_step in snare_pattern:
        amy.send(synth=10, note=SNARE, vel=0.9)
    elif not fill_bar and random.random() < 0.06:
        amy.send(synth=10, note=SNARE, vel=0.25)  # ghost snare
    if fill_bar and bar_step >= 16 and bar_step % 2 == 0:
        # snare/tom ramp into the kit change
        note = SNARE if bar_step < 24 else (LOW_TOM if bar_step < 28 else HI_TOM)
        amy.send(synth=10, note=note, vel=0.4 + (bar_step - 16) * 0.035)

    # --- TR-909 layer (synth 11): hats and clap, always the 909 ---
    if bar_step in ohat_pattern:
        amy.send(synth=11, note=OHAT, vel=0.4)
    elif bar_step in hat_steps:
        amy.send(synth=11, note=CHAT, vel=0.5 if bar_step % 8 == 0 else 0.3)
    if bar % 2 == 1 and bar_step in snare_pattern:
        amy.send(synth=11, note=CLAP, vel=0.6)

    # --- Percussion layer (synth 12): tabla, shaker, triangle ---
    if bar_step in shaker_steps:
        amy.send(synth=12, note=SHAKER, vel=0.35 if bar_step % 8 == 2 else 0.2)
    if bar >= 2 and bar_step in tabla_pattern:
        amy.send(synth=12, note=TABLA_LO if bar_step < 16 else TABLA_HI, vel=0.5)
    if bar >= 4 and bar_step == 28:
        amy.send(synth=12, note=TRIANGLE, vel=0.3)
    if bar == 0 and bar_step == 12:
        amy.send(synth=12, note=COWBELL, vel=0.5)

    # --- Display: bar dots + beat blocks, refreshed once per beat ---
    if bar_step % 8 == 0:
        beat = bar_step // 8
        if beat == 0:  # new bar: move the section-position dot
            for b in range(BARS_PER_SECTION):
                x = 4 + b * 15
                d.fill_rect(x, 32, 11, 8, 255 if b == bar else 0)
                d.rect(x, 32, 11, 8, 255)
        for b in range(4):
            x = 4 + b * 32
            d.fill_rect(x, 52, 22, 12, 255 if b == beat else 0)
            d.rect(x, 52, 22, 12, 255)
        d.show()
