# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
import amy

# DESCRIPTION: Apply a low pass filter to audio in controlled by CV1

# The two oscs aren't chained, so every per-osc setting has to be sent to each osc.
# AMY's default volume (1) attenuates the output by -20dB for synth headroom. Give each AUDIO_IN osc
# a fixed 10x gain (amp=10) to reach unity, leaving vel for note dynamics. The LPF24 is a linear
# biquad cascade, so the hot pre-filter signal just scales through it.
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0, amp=10)
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1, amp=10)
# Turn on a note on each osc so both channels sound. The note number is required but ignored.
amy.send(synth=18, osc=0, vel=1, note=60)
amy.send(synth=18, osc=1, vel=1, note=60)
# Set filter-freq to be controlled by ext0 (CV1), on each osc so both channels are filtered.
amy.send(synth=18, osc=0, filter_freq={'const': 300, 'ext0': 0.25}, filter_type=amy.FILTER_LPF24)
amy.send(synth=18, osc=1, filter_freq={'const': 300, 'ext0': 0.25}, filter_type=amy.FILTER_LPF24)

def loop():
    pass
