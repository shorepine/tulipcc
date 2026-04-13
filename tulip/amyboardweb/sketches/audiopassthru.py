# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
import amy

# Pass audio in through to audio out
# We just set up AMY to have a synth with two oscs
# one for left channel and one for right channel. 

# This will just pass through any audio in to audio out, including SPDIF to analog & vice versa
# This also lets you set global effects on audio input

volume = 1 # Make it louder if you want
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0)
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1)
amy.send(synth=18, vel=volume,note=60) # turn on a note. The note number is required but ignored

def loop():
    pass
