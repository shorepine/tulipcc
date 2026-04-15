# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).

import amy, amyboard

# DESCRIPTION: Sets CV1 to control filter freq and CV2 to resonance
# We have built in CtrlCoef to set filter_freq, but not resonance, so we use
# the loop() to check for CV2 and set resonance. 

# Set filter-freq to be set by ext0 (CV1) 
amy.send(synth=1, filter_freq={'const': 300, 'ext0': 0.25}, filter_type=amy.FILTER_LPF24)

def loop():
    # During loop, see if cv2 is active and set resonance
    r = (amyboard.cv_in(1) + 10.0) / 5.0 # 0-4
    amy.send(synth=1, resonance=r)

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
