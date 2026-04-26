# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: MIDI to CV: notes -> 1V/oct on CV2, CC23 -> -10..10V on CV1.
import amy, amyboard, midi

# We're emitting CV, not playing audio. Disable the default synth on ch 1.
amy.send(synth=1, num_voices=0)

# CC# we treat as the modulation source for CV1.
MOD_CC = 23


def midi_cb(m):
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    a = m[1]
    b = m[2]
    if status == 0x90 and b > 0:
        # 1 volt per octave on CV2 (channel=1). MIDI 0 = 0V, MIDI 12 = 1V, ...
        amyboard.cv_out(a / 12.0, channel=1)
    elif status == 0xB0 and a == MOD_CC:
        # Scale CC value 0..127 to -10..+10V on CV1 (channel=0).
        amyboard.cv_out((b / 127.0) * 20.0 - 10.0, channel=0)


midi.add_callback(midi_cb)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
