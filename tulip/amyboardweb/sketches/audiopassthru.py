# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
import amy

# DESCRIPTION: Pass audio in through to audio out
# We just set up AMY to have a synth with two oscs
# one for left channel and one for right channel. 

# This will just pass through any audio in to audio out, including SPDIF to analog & vice versa
# This also lets you set global effects on audio input

# The two oscs aren't chained, so every per-osc setting has to be sent to each osc.
# AMY's default volume (1) attenuates the output by -20dB for synth headroom. Rather than turn up
# the global volume (which would affect everything), give each AUDIO_IN osc a fixed 10x gain (amp=10)
# to reach unity, leaving vel for note dynamics.
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0, amp=10)
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1, amp=10)
# Turn on a note on each osc so both channels sound. The note number is required but ignored.
amy.send(synth=18, osc=0, vel=1, note=60)
amy.send(synth=18, osc=1, vel=1, note=60)

def loop():
    pass
