# AMYboard Sketch
# Code put here runs first, then loop() is called every 32nd note.
# DESCRIPTION: Douglas Repetto's sineclock
import amyboard, amy, tulip, time

OFFSET_HOURS = -4 # EDT, set for your location

# Optionally set to set the time on your AMYboard automatically. Not needed on the simulator.
WIFI_SSID = None 
WIFI_PASS = None

amyboard.display.fill(0)
amyboard.display.show()

# sineclock
# see https://sineclock.com/
import math

# Reserve 3 oscs x 3 "hands"
amy.send(synth=16, num_voices=1, oscs_per_voice=9)

def setup_triple(
    synth=16, base_osc=0, base_freq=200.0, freq_dev=5.0, period_sec=60, phase=0, vel=1
):
    # Osc+2 is the mod osc
    amy.send(synth=synth, osc=base_osc + 2, freq=1.0/period_sec,
             phase=phase - 0.25)  # Want it at lowest value when phase = now.
    # Osc+1 is the moving sine.  Pan hard left
    mid_freq = base_freq + freq_dev / 2
    mod_depth = math.log2(mid_freq / base_freq)  # -1 x this deviation brings down to base_freq
    amy.send(synth=synth, osc=base_osc + 1, mod_source=base_osc + 2,
             freq={'const': mid_freq, 'mod': mod_depth}, pan=0)
    # Osc+0 is the fixed sine.  Pan hard right
    amy.send(synth=synth, osc=base_osc, freq=base_freq, pan=1)
    # Start the two sounding oscs
    amy.send(synth=synth, osc=base_osc, phase=0, vel=vel, note=69)
    amy.send(synth=synth, osc=base_osc + 1, phase=0, vel=vel, note=69)

def start_sine_clock(hour, minute, second, vel=0.1):
    # One-minute cycle
    setup_triple(base_osc=0, vel=vel, base_freq=200,
                 period_sec=60, phase=second / 60)
    # One-hour cycle
    setup_triple(base_osc=3, vel=vel, base_freq=300,
                 period_sec=60 * 60,
                 phase=(60 * minute + second) / (60 * 60))
    # One day cycle
    setup_triple(base_osc=6, vel=vel, base_freq=450,
                 period_sec=60 * 60 * 24,
                 phase=(3600 * hour + 60 * minute + second) / (60 * 60 * 24))

   
# Set the time if wifi is given and we're running on hardware
if((tulip.board()=="AMYBOARD") and (WIFI_SSID is not None) and (WIFI_PASS is not None)):
    amyboard.wifi(WIFI_SSID, WIFI_PASS) 
    tulip.set_time()

t = time.localtime(int(time.time()) + int(OFFSET_HOURS*3600))
hour, minute, second = t[3], t[4], t[5]

print("sineclock says it is %d:%d:%d" % (hour, minute, second))

vel = 0.1
start_sine_clock(hour=hour, minute=minute, second=second, vel=vel)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
i0ic255Z
i0iv6in2Z
i0v0w2f,,,,,1.000F200.000,,,,4.000L1G4A,,8000,,100,0.000B0,1.000,3000,0.100,100,0.000Z
i0v1a0.005,,0.000f5.000Z
i0V0.500x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k1.000,320.000,0.500,0.500h0.000,0.850,0.500,3000.000Z
i1ic255Z
i1iv6in2Z
i1v0w2f,,,,,1.000F200.000,,,,4.000Q0.800L1G4A,,8000,,100,0.000B0,1.000,3000,0.100,100,0.000Z
i1v1a0.005,,0.000f5.000Z
i2ic255Z
i2iv6in2Z
i2v0w2f,,,,,-1.000F200.000,,,,4.000Q0.200L1G4A,,8000,,100,0.000B0,1.000,3000,0.100,100,0.000Z
i2v1a0.100,,0.000f0.100P0.000Z
i3ic255Z
i3iv12in4Z
i3v0w8o1O,,,,2,1A,,1000,1.000,2000,0.000T2X3Z
i3v1a,,0.000I1.000A,0.100,1000,1.000,4000,0.000T2Z
i3v2a1.500,,0.000,0.400,,0.010I1.000L3A4000,,4000,,1000,0.000T2Z
i3v3a,,0.000f6.000,0.500P0.750Z
i16ic255Z
i16iv1in9Z
i16v0f200.000Q1.000P0.000Z
i16v1f202.500,,,,,0.018Q0.000P0.000L2Z
i16v2a,,0.000f0.017P-0.250Z
i16v3f300.000Q1.000P0.000Z
i16v4f302.500,,,,,0.012Q0.000P0.000L5Z
i16v5a,,0.000f0.000P0.683Z
i16v6f450.000Q1.000P0.000Z
i16v7f452.500,,,,,0.008Q0.000P0.000L8Z
i16v8a,,0.000f0.000P0.081Z
"""
