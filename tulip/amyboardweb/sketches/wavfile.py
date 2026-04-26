# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: Load a .wav file as a PCM instrument on synth 1.
import amy, tulip

# Built-in tulipcc samples live under tulip.root_dir() + 'sys/ex/'.
# wav files can also be loaded from an SD card with a path like '/sd/file.wav'.
WAV_PATH = tulip.root_dir() + 'sys/ex/bcla3.wav'
PRESET = 50

amy.reset()

# Load the wav into AMY's in-memory PCM table at PRESET.
amy.load_sample(WAV_PATH, preset=PRESET)

# Make synth 1 a 4-voice PCM player using that preset. Play it with MIDI ch 1.
amy.send(synth=1, wave=amy.PCM, preset=PRESET, num_voices=4)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
