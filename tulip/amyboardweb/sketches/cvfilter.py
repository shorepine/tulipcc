# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).

import amy, amyboard

# This sketch sets CV1 to control filter freq and CV2 to resonance
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
i1v0w1F300.000,1.000,,,5.000,,,0.250R2.019c2L1G4A0,1.000,1000,0.500,200,0.000B0,1.000,1000,0.200,1000,0.000Z
i1v1w4a,,0.000f4.000,0.000,,,,,0.000A0,1.000,10000,0.000Z
i1v2w2f220.000L1A0,1.000,1000,0.200,200,0.000Z
i1V1.000x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k0.000,320.000,0.500,0.500h0.000,0.850,0.500,3000.000Z
i1ic70,1,50.000,2000.000,0.000,i%iv0f%vZ
i1ic71,0,0.000,9.000,0.000,i%iv0w%vZ
i1ic72,0,0.500,0.990,0.000,i%iv0d%vZ
i1ic73,1,0.001,1.000,0.100,i%iv0a,,%vZ
i1ic74,1,20.000,8000.000,0.000,i%iv0F%vZ
i1ic75,1,0.500,16.000,0.000,i%iv0R%vZ
i1ic76,0,0.000,1.000,0.000,i%iv0F,%vZ
i1ic77,0,-10.000,10.000,0.000,i%iv0F,,,,%vZ
i1ic78,0,-15.000,15.000,0.000,x%vZ
i1ic79,0,-15.000,15.000,0.000,x,%vZ
i1ic80,0,-15.000,15.000,0.000,x,,%vZ
i1ic81,1,50.000,2000.000,0.000,i%iv2f%vZ
i1ic82,0,0.000,9.000,0.000,i%iv2w%vZ
i1ic83,0,0.500,0.990,0.000,i%iv2d%vZ
i1ic84,1,0.001,1.000,0.100,i%iv2a,,%vZ
i1ic85,0,0.000,1000.000,0.000,i%iv0B%v,1,,,,0Z
i1ic86,1,0.000,2000.000,50.000,i%iv0B,1,%v,,,0Z
i1ic87,0,0.000,1.000,0.000,i%iv0B,1,,%v,,0Z
i1ic88,1,0.000,8000.000,50.000,i%iv0B,1,,,%v,0Z
i1ic89,1,0.000,1.000,0.100,k%vZ
i1ic90,1,0.100,20.000,0.000,k,,%vZ
i1ic91,0,0.010,1.000,0.000,k,,,%vZ
i1ic92,1,0.100,20.000,0.000,i%iv1f%vZ
i1ic93,0,0.000,9.000,0.000,i%iv1w%vZ
i1ic94,1,0.000,4.000,0.001,i%iv0f,,,,,%vZi%iv2f,,,,,%vZ
i1ic95,0,0.000,0.490,0.000,i%iv0d,,,,,%vZi%iv2d,,,,,%vZ
i1ic96,0,0.000,4.000,0.001,i%iv0F,,,,,%vZ
i1ic97,0,0.000,1000.000,0.000,i%iv0A%v,1,,,,0Zi%iv2A%v,1,,,,0Z
i1ic98,1,0.000,2000.000,50.000,i%iv0A,1,%v,,,0Zi%iv2A,1,%v,,,0Z
i1ic99,0,0.000,1.000,0.000,i%iv0A,1,,%v,,0Zi%iv2A,1,,%v,,0Z
i1ic100,1,0.000,8000.000,50.000,i%iv0A,1,,,%v,0Zi%iv2A,1,,,%v,0Z
i1ic101,1,0.000,1.000,0.100,h%vZ
i1ic102,0,0.000,1.000,0.000,h,%vZ
i1ic103,0,0.000,1.000,0.000,h,,%vZ
i1ic104,0,0.000,2.000,0.000,M%vZ
i1ic105,0,0.000,5000.000,0.000,M,%vZ
i1ic106,0,0.000,1.000,0.000,M,,,%vZ
"""
