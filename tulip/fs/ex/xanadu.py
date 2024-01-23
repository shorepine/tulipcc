"""Joseph T. Kung's XANADU, implemented for tulipcc.

See https://github.com/csound/examples/blob/master/csd/xanadu.csd for original.
"""


import math
import time

try:
    import amy
    amy.live()
except:
    amy = alles
    alles.chorus(1)

NEXT_OSC = 0
TOTAL_OSCS = 64 # 48  # 64
NUM_USABLE_OSCS = 62
OSC_BLOCKING = 32  # Don't let sets of oscs straddle this.

C0_FREQ = 440.0 / math.pow(2.0, 4 + 9/12)

def get_oscs(num_oscs):
    global NEXT_OSC
    # Maybe pad to achieve blocking.
    num_skipped_oscs = 0
    if ((NEXT_OSC % OSC_BLOCKING) + num_oscs) > OSC_BLOCKING:
        num_skipped_oscs = OSC_BLOCKING - (NEXT_OSC % OSC_BLOCKING)
    if NEXT_OSC >= NUM_USABLE_OSCS:
        num_skipped_oscs = TOTAL_OSCS - NEXT_OSC
    for osc in range(NEXT_OSC, NEXT_OSC + num_skipped_oscs):
        amy.reset(osc)
    NEXT_OSC += num_skipped_oscs
    #print("Skipped", num_skipped_oscs, "oscs.")
    oscs = [(NEXT_OSC + osc) % TOTAL_OSCS for osc in range(num_oscs)]
    NEXT_OSC = (NEXT_OSC + num_oscs) % TOTAL_OSCS
    for osc in oscs:
        amy.reset(osc)
    return oscs

def pitch2freq(pitch):
    # Pitch in OCT.(STEP/100)
    oct = math.floor(pitch)
    step = 100 * (pitch - oct)
    freq = C0_FREQ * math.pow(2.0, oct + step/12)
    return freq

def shift_pitch(pitch, shift):
    oct = math.floor(pitch)
    step = 100 * (pitch - oct)
    s_oct = math.floor(shift)
    s_step = 100 * (shift - s_oct)
    final_step = step + s_step
    final_oct = oct + s_oct + (final_step // 12)
    final_step = final_step % 12
    return final_oct + (final_step / 100)

def Note1(pitch, vel=1.0, time=None, duration=10.0, pan=0.5):
    global START
    if time is None:
        timestamp = amy.millis()
    else:
        timestamp = int(round(START + time * 1000))
    oscs = get_oscs(2)
    osc = oscs[0]
    modosc = oscs[1]
    amy.send(osc=modosc, wave=amy.SINE, freq=5, amp=0.005, timestamp=timestamp)
    amy.send(osc=osc, wave=amy.SAW_DOWN, freq=440, mod_source=modosc, mod_target=amy.TARGET_FREQ, 
             filter_freq=500, filter_type=amy.FILTER_LPF, timestamp=timestamp)
    amy.send(osc=osc, bp0="5000,0.01,0,0", bp0_target=amy.TARGET_FILTER_FREQ, resonance=0.5, timestamp=timestamp)
    amy.send(osc=osc, bp1="100,1.0,8000,0.0,250,0", bp1_target=amy.TARGET_AMP, timestamp=timestamp)
    freq = pitch2freq(pitch)
    amy.send(osc=osc, freq=freq, vel=vel, timestamp=timestamp, pan=pan)
    # Note off
    #amy.send(osc=osc, vel=0, timestamp=timestamp + int(round(1000 * duration)))

def Note2(pitch, vel=1.0, time=None, pitch_dev=0.05, duration=10.0, pan=0.5):
    global START
    timestamp = int(round(START + time * 1000))
    oscs = get_oscs(2)
    osc = oscs[0]
    modosc = oscs[1]
    amy.send(osc=modosc, wave=amy.SAW_DOWN, freq=0.1, amp=pitch_dev, timestamp=timestamp)
    amy.send(osc=osc, wave=amy.SAW_DOWN, freq=440, mod_source=modosc, mod_target=amy.TARGET_FREQ, 
             filter_freq=500, filter_type=amy.FILTER_LPF, timestamp = timestamp)
    amy.send(osc=osc, bp0="5000,0.01,0,0", bp0_target=amy.TARGET_FILTER_FREQ, resonance=0.5, 
             timestamp=timestamp)
    amy.send(osc=osc, bp1="100,1.0,8000,0.0,250,0", bp1_target=amy.TARGET_AMP, timestamp=timestamp)
    freq = pitch2freq(pitch)
    amy.send(osc=osc, freq=freq, vel=vel, timestamp=timestamp, pan=pan)
    # Note off
    #amy.send(osc=osc, vel=0, timestamp=timestamp + int(round(1000 * duration)))

def Note(pitch, vel=1.0, time=None, pitch_shift=0, second_delay=0.2, use_third=False):
    if time == None:
        START = amy.millis()
        time = 0
    Note1(pitch, vel, time)
    #Note2(pitch, vel * 0.5, time + 0.1, pitch_dev=0.01, pan=0)
    #Note1(pitch, vel, time + 1.1)
    if use_third:
        Note2(shift_pitch(pitch, pitch_shift), vel * 0.5, time + second_delay, pitch_dev=0.03, pan=0.8)
        Note1(pitch, vel * 0.25, time + 2, pan=0.2)

# FM note
def NoteFM(pitch, vel=1.0, time=0, duration=8.0):
    global START
    timestamp = int(round(START + time * 1000))
    oscs = get_oscs(4)
    amy.send(osc=oscs[2], wave=amy.SINE, freq=1/duration, phase=0.75, amp=1, timestamp=timestamp)
    amy.send(osc=oscs[1], wave=amy.SINE, ratio=1, amp=0.1, mod_source=oscs[2], mod_target=amy.TARGET_AMP,
             timestamp=timestamp )
    amy.send(osc=oscs[0], wave=amy.SINE, ratio=1, amp=0.2, bp0_target=amy.TARGET_AMP, bp0="0,0,1000,1,1000,0",
             timestamp=timestamp )
    amy.send(osc=oscs[3], wave=amy.ALGO,algorithm=1,algo_source="-1,-1,-1,-1,%d,%d" % (oscs[1], oscs[0]),
             bp0_target=amy.TARGET_AMP,bp0="0,1,1000,1,200,0", timestamp = timestamp)
    amy.send(osc=oscs[1], vel=0.3, timestamp=timestamp)
    amy.send(osc=oscs[3], freq=pitch2freq(pitch), vel=1, timestamp=timestamp)
    # Note off
    #amy.send(osc=oscs[3], vel=0, timestamp=timestamp + int(round(1000 * duration)))
    # Note offs cause trouble if they are executed after the osc has been stolen for another note.


def wait_another(time_sec):
    global last_time
    time.sleep(last_time + time_sec - amy.millis() / 1000)
    last_time += time_sec        


def broken_chord(base_pitch, intervals, start_time, wait_time=7.5, **kwargs):
    """Emit a set of notes as a staggered chord."""
    for index, interval in enumerate([0] + intervals):
        pitch = shift_pitch(base_pitch, interval)
        NoteFM(pitch - 1.0, 1, start_time)
        Note(pitch, 1, start_time + 0.1 * index, **kwargs)
    wait_another(wait_time)

tulip.display_stop()
amy.reset()
# Make all our times be a little behind real time.  Make the offset larger if the script doesn't keep up.
START = amy.millis() + 1500
last_time = START / 1000

# F#7addB chord on a guitar
broken_chord(4.06, [.07, 1.0, 1.04, 1.05, 1.10], start_time=0, pitch_shift=0.029, second_delay=1.0, use_third=True)

# D6add9 chord on a guitar
broken_chord(4.02, [.07, 1.0, 1.04, 0.09, 1.02], start_time=7.5)

# Bmajadd11 chord on a guitar
broken_chord(4.11, [.07, 1.0, 1.04, 1.0, 1.05], start_time=15)

# Amajadd9 chord on a guitar
broken_chord(4.09, [.07, 2.0, 1.04, 1.02, 1.07], start_time=22.5)

# Bmajadd11 chord on a guitar
broken_chord(4.11, [.07, 1.0, 1.04, 1.0, 1.05], start_time=30)

# Gmaj6 chord on a guitar
broken_chord(4.07, [.07, 1.0, 1.04, 1.04, 1.09], start_time=37.5)

# F#7addB chord on a guitar
broken_chord(5.06, [.07, 1.0, 1.04, 1.05, 1.10], start_time=45, pitch_shift=0.029, second_delay=1.0, use_third=True, wait_time=15)

# Crude fade-out.
for i in range(10):
    amy.volume(1 - i / 10)
    time.sleep(0.1)

amy.reset()
tulip.display_start()

