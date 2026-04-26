# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: Load a .wav file as a PCM instrument on synth 1.
import amy, tulip

# Built-in tulipcc samples live under tulip.root_dir() + 'sys/ex/'.
# wav files can also be loaded from an SD card with a path like '/sd/file.wav'.
WAV_PATH = tulip.root_dir() + 'sys/ex/bcla3.wav'
PRESET = 50

# Load the wav into AMY's in-memory PCM table at PRESET.
amy.load_sample(WAV_PATH, preset=PRESET)
amy.send(synth=1, oscs_per_voice=1, num_voices=4, wave=amy.PCM, preset=PRESET)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
i1ic255Z
i1iv4in4Z
i1v0w7p50Z
i1v1Z
i1v2Z
i1v3Z
i1V1.000x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k0.000,320.000,0.500,0.500h0.000,0.850,0.500,3000.000Z
"""
