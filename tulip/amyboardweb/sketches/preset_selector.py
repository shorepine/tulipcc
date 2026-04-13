# AMYboard Sketch
# Preset selector: scroll through all 257 AMY presets with the rotary encoder.
# Click to load the selected preset on synth 1.
import amyboard, amy
from patches import patches as PRESETS

NUM_PRESETS = len(PRESETS)
SEESAW = 0x36
BTN_PIN = 24

# --- State ---
enc_offset = -amyboard.read_encoder(seesaw_dev=SEESAW)
current_index = 0
prev_btn = False
loaded_patch = -1

# --- Display helpers ---
d = amyboard.display
amyboard.init_buttons(pins=(BTN_PIN,), seesaw_dev=SEESAW)

def draw():
    d.fill(0)
    d.text("PRESET SELECT", 0, 0, 255)
    d.text("%d/%d" % (current_index, NUM_PRESETS - 1), 0, 14, 255)
    name = PRESETS[current_index].strip()
    if len(name) <= 16:
        d.text(name, 0, 30, 255)
    else:
        d.text(name[:16], 0, 30, 255)
        d.text(name[16:], 0, 42, 255)
    if current_index == loaded_patch:
        d.text("[LOADED]", 0, 56, 255)
    amyboard.display_refresh()

def load_preset(idx):
    global loaded_patch
    amy.send(synth=1, patch=idx, num_voices=6)
    loaded_patch = idx
    draw()

draw()

def loop():
    global current_index, prev_btn
    raw = amyboard.read_encoder(seesaw_dev=SEESAW) + enc_offset
    idx = raw % NUM_PRESETS
    btn = amyboard.read_buttons(pins=(BTN_PIN,), seesaw_dev=SEESAW)[0]

    if idx != current_index:
        current_index = idx
        draw()

    if prev_btn and not btn:
        load_preset(current_index)

    prev_btn = btn
