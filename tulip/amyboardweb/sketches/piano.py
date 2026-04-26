# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
# DESCRIPTION: Play an additive voiced piano

import amy
amy.send(synth=1,patch=256,num_voices=6)
def loop():
    pass
