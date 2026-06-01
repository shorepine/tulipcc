# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
import amy

# DESCRIPTION: Apply a low pass filter to audio in controlled by CV1

# AMY's default volume is 1, which is -20dB. This is good for synths for headroom but not good 
# for direct input to output effects. So let's set volume to 10, which is unity
amy.send(volume=10)
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0)
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1)
amy.send(synth=18, vel=1,note=60) # turn on a note. The note number is required but ignored
# Set filter-freq to be set by ext0 (CV1)
amy.send(synth=18, filter_freq={'const': 300, 'ext0': 0.25}, filter_type=amy.FILTER_LPF24)

def loop():
    pass
