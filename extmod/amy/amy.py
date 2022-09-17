import time

BLOCK_SIZE = 256
SAMPLE_RATE = 44100.0
OSCS = 64
MAX_QUEUE = 400
[SINE, PULSE, SAW_DOWN, SAW_UP, TRIANGLE, NOISE, KS, PCM, ALGO, PARTIAL, PARTIALS, OFF] = range(12)
TARGET_AMP, TARGET_DUTY, TARGET_FREQ, TARGET_FILTER_FREQ, TARGET_RESONANCE, TARGET_FEEDBACK, TARGET_LINEAR, TARGET_TRUE_EXPONENTIAL, TARGET_DX7_EXPONENTIAL = (1, 2, 4, 8, 16, 32, 64, 128, 256)
FILTER_NONE, FILTER_LPF, FILTER_BPF, FILTER_HPF = range(4)
AMY_LATENCY_MS = 0
AMY_MAX_DRIFT_MS = 20000

override_send = None

"""
    A bunch of useful presets
"""
def preset(which,osc=0, **kwargs):
    # Reset the osc first
    reset(osc=osc)
    if(which==0): # simple note
        send(osc=osc, wave=SINE, bp0="10,1,250,0.7,500,0", bp0_target=TARGET_AMP, **kwargs)
    if(which==1): # filter bass
        send(osc=osc, filter_freq=2500, resonance=5, wave=SAW_DOWN, filter_type=FILTER_LPF, bp0="100,0.5,25,0", bp0_target=TARGET_AMP+TARGET_FILTER_FREQ, **kwargs)

    # TODO -- this is a good one to test the whistle on the bps... 
    if(which==2): # long sine pad to test ADSR
        send(osc=osc, wave=SINE, bp0="0,0,500,1,1000,0.25,750,0", bp0_target=TARGET_AMP, **kwargs)

    if(which==3): # amp LFO example
        reset(osc=osc+1)
        send(osc=osc+1, wave=SINE, vel=0.50, freq=1.5, **kwargs)
        send(osc=osc, wave=PULSE, bp0="150,1,250,0.25,250,0", bp0_target=TARGET_AMP, mod_target=TARGET_AMP, mod_source=osc+1, **kwargs)
    if(which==4): # pitch LFO going up 
        reset(osc=osc+1)
        send(osc=osc+1, wave=SINE, vel=0.50, freq=0.25, **kwargs)
        send(osc=osc, wave=PULSE, bp0="150,1,400,0,0,0", bp0_target=TARGET_AMP, mod_target=TARGET_FREQ, mod_source=osc+1, **kwargs)
    if(which==5): # bass drum
        # Uses a 0.25Hz sine wave at 0.5 phase (going down) to modify frequency of another sine wave
        reset(osc=osc+1)
        send(osc=osc+1, wave=SINE, vel=0.50, freq=0.25, phase=0.5, **kwargs)
        send(osc=osc, wave=SINE, vel=0, bp0="500,0,0,0", bp0_target=TARGET_AMP, mod_target=TARGET_FREQ, mod_source=osc+1, **kwargs)
    if(which==6): # noise snare
        send(osc=osc, wave=NOISE, vel=0, bp0="250,0,0,0", bp0_target=TARGET_AMP, **kwargs)
    if(which==7): # closed hat
        send(osc=osc, wave=NOISE, vel=0, bp0="25,1,75,0,0,0", bp0_target=TARGET_AMP, **kwargs)
    if(which==8): # closed hat from PCM 
        send(osc=osc, wave=PCM, vel=0, patch=0, freq=0, **kwargs)
    if(which==9): # cowbell from PCM
        send(osc=osc, wave=PCM, vel=0, patch=10, freq=0, **kwargs)
    if(which==10): # high cowbell from PCM
        send(osc=osc, wave=PCM, vel=0, patch=10, note=70, **kwargs)
    if(which==11): # snare from PCM
        send(osc=osc, wave=PCM, vel=0, patch=5, freq=0, **kwargs)
    if(which==12): # FM bass 
        send(osc=osc, wave=ALGO, vel=0, patch=21, **kwargs)
    if(which==13): # Pcm bass drum
        send(osc=osc, wave=PCM, vel=0, patch=1, freq=0, **kwargs)
    if(which==14): # filtered algo 
        send(wave=ALGO, vel=0, patch=62,filter_freq=2000,resonance=2.5,filter_type=FILTER_LPF, bp0_target=TARGET_FILTER_FREQ,bp0="1,1,500,0,0,0")



def millis():
    import datetime
    # Timestamp to send over to synths
    # This is a suggestion. I use ms since today started
    d = datetime.datetime.now()
    return int((datetime.datetime.utcnow() - datetime.datetime(d.year, d.month, d.day)).total_seconds()*1000)


# Removes trailing 0s and x.0000s from floating point numbers to trim wire message size
# Fun historical trivia: this function caused a bug so bad that Dan had to file a week-long PR for micropython
# https://github.com/micropython/micropython/pull/8905
def trunc(number):
    return ('%.10f' % number).rstrip('0').rstrip('.')

# Construct an AMY message
def message(osc=0, wave=-1, patch=-1, note=-1, vel=-1, amp=-1, freq=-1, duty=-1, feedback=-1, timestamp=None, reset=-1, phase=-1, \
        client=-1, retries=1, volume=-1, filter_freq = -1, resonance = -1, bp0="", bp1="", bp2="", bp0_target=-1, bp1_target=-1, bp2_target=-1, mod_target=-1, \
        debug=-1, mod_source=-1, eq_l = -1, eq_m = -1, eq_h = -1, filter_type= -1, algorithm=-1, ratio = -1, latency_ms = -1, algo_source=None):

    m = ""
    if(timestamp is None): timestamp = millis()
    m = m + "t" + str(timestamp)
    if(osc>=0): m = m + "v" + str(osc)
    if(wave>=0): m = m + "w" + str(wave)
    if(duty>=0): m = m + "d" + trunc(duty)
    if(feedback>=0): m = m + "b" + trunc(feedback)
    if(freq>=0): m = m + "f" + trunc(freq)
    if(note>=0): m = m + "n" + str(note)
    if(patch>=0): m = m + "p" + str(patch)
    if(phase>=0): m = m + "P" + trunc(phase)
    if(client>=0): m = m + "c" + str(client)
    if(amp>=0): m = m + "a" + trunc(amp)
    if(vel>=0): m = m + "l" + trunc(vel)
    if(volume>=0): m = m + "V" + trunc(volume)
    if(latency_ms>=0): m = m + "N" + str(latency_ms)
    if(resonance>=0): m = m + "R" + trunc(resonance)
    if(filter_freq>=0): m = m + "F" + trunc(filter_freq)
    if(ratio>=0): m = m + "I" + trunc(ratio)
    if(algorithm>=0): m = m + "o" + str(algorithm)
    if(len(bp0)): m = m +"A%s" % (bp0)
    if(len(bp1)): m = m +"B%s" % (bp1)
    if(len(bp2)): m = m +"C%s" % (bp2)
    if(algo_source is not None): m = m +"O%s" % (algo_source)
    if(bp0_target>=0): m = m + "T" +str(bp0_target)
    if(bp1_target>=0): m = m + "W" +str(bp1_target)
    if(bp2_target>=0): m = m + "X" +str(bp2_target)
    if(mod_target>=0): m = m + "g" + str(mod_target)
    if(mod_source>=0): m = m + "L" + str(mod_source)
    if(reset>=0): m = m + "S" + str(reset)
    if(debug>=0): m = m + "D" + str(debug)
    if(eq_l>=0): m = m + "x" + trunc(eq_l)
    if(eq_m>=0): m = m + "y" + trunc(eq_m)
    if(eq_h>=0): m = m + "z" + trunc(eq_h)
    if(filter_type>=0): m = m + "G" + str(filter_type)
    return m+'Z'


def send_raw(m):
    import libamy
    libamy.send(m)

# Send an AMY message to amy
def send(**kwargs):
    global override_send
    if(override_send is not None):
        override_send(**kwargs)
    else:
        m = message(**kwargs)
        send_raw(m)


# Plots a time domain and spectra of audio
def show(data):
    import matplotlib.pyplot as plt
    import numpy as np
    fftsize = len(data)
    windowlength = fftsize
    window = np.hanning(windowlength)
    wavepart = data[:len(window)]
    logspecmag = 20 * np.log10(np.maximum(1e-10, 
        np.abs(np.fft.fft(wavepart * window)))[:(fftsize // 2 + 1)])
    freqs = SAMPLE_RATE * np.arange(len(logspecmag)) / fftsize
    plt.subplot(211)
    times = np.arange(len(wavepart)) / SAMPLE_RATE
    plt.plot(times, wavepart, '.')
    plt.subplot(212)
    plt.plot(freqs, logspecmag, '.-')
    plt.ylim(np.array([-100, 0]) + np.max(logspecmag))
    plt.show()

# Writes a WAV file of rendered data
def write(data, filename):
    import scipy.io.wavfile as wav
    import numpy as np
    wav.write(filename, int(SAMPLE_RATE), (32768.0 * data).astype(np.int16))

# Play a rendered sound out of default sounddevice
def play(samples):
    import sounddevice as sd
    sd.play(samples)

# Render AMY's internal buffer to a numpy array of floats
def render(seconds):
    import numpy as np
    import libamy
    # Output a npy array of samples
    frame_count = int((seconds*SAMPLE_RATE)/BLOCK_SIZE)
    frames = []
    for f in range(frame_count):
        frames.append( np.array(libamy.render())/32767.0 )
    return np.hstack(frames)


# Starts a live mode, with audio playing out default sounddevice
def start():
    live()
def live():
    import libamy
    libamy.live()

# Stops live mode
def pause():
    stop()
def stop():
    import libamy
    libamy.pause()

"""
    Convenience functions
"""

def reset(osc=None):
    if(osc is not None):
        send(reset=osc)
    else:
        send(reset=100) # reset > AMY_OSCS resets all oscs

def volume(volume, client = -1):
    send(client=client, volume=volume)

def latency_ms(latency, client=-1):
    send(client=client, latency_ms =latency)

"""
    Run a scale through all the synth's sounds
"""
def test():
    while True:
        for wave in [SINE, SAW_DOWN, PULSE, TRIANGLE, NOISE]:
            for i in range(12):
                send(osc=0, wave=wave, note=40+i, patch=i, vel=1)
                time.sleep(0.5)


"""
    Play all of the FM patches in order
"""
def play_patches(wait=0.500, patch_total = 100, **kwargs):
    once = True
    patch_count = 0
    while True:
        for i in range(24):
            patch = patch_count % patch_total
            patch_count = patch_count + 1
            send(osc=i % OSCS, note=i+50, wave=ALGO, patch=patch, vel=1, **kwargs)
            time.sleep(wait)
            send(osc=i % OSCS, vel=0)

"""
    Play up to AMY_OSCS patches at once
"""
def polyphony(max_voices=OSCS,**kwargs):
    note = 0
    oscs = []
    for i in range(int(max_voices/2)):
        oscs.append(int(i))
        oscs.append(int(i+(OSCS/2)))
    print(str(oscs))
    while(1):
        osc = oscs[note % max_voices]
        print("osc %d note %d filter %f " % (osc, 30+note, note*50))
        send(osc=osc, **kwargs, patch=note, filter_type=FILTER_NONE, filter_freq=note*50, note=30+(note), client = -1, vel=1)
        time.sleep(0.5)
        note =(note + 1) % 64

def eq_test():
    reset()
    eqs = [ [0,0,0], [15,0,0], [0,0,15], [0,15,0],[-15,-15,15],[-15,-15,30],[-15,30,-15], [30,-15,-15] ]
    for eq in eqs:
        print("eq_l = %ddB eq_m = %ddB eq_h = %ddB" % (eq[0], eq[1], eq[2]))
        send(eq_l=eq[0], eq_m=eq[1], eq_h=eq[2])
        drums(loops=2)
        time.sleep(1)
        reset()
        time.sleep(0.250)

"""
    Sweep the filter
"""
def sweep(speed=0.100, res=0.5, loops = -1):
    end = 2000
    cur = 0
    while(loops != 0):
        for i in [0, 1, 4, 5, 1, 3, 4, 5]:
            cur = (cur + 100) % end
            send(osc=0,filter_type=FILTER_LPF, filter_freq=cur+250, resonance=res, wave=PULSE, note=50+i, duty=0.50, vel=1)
            send(osc=1,filter_type=FILTER_LPF, filter_freq=cur+500, resonance=res, wave=PULSE, note=50+12+i, duty=0.25, vel=1)
            send(osc=2,filter_type=FILTER_LPF, filter_freq=cur, resonance=res, wave=PULSE, note=50+6+i, duty=0.90, vel=1)
            time.sleep(speed)

"""
    An example drum machine using osc+PCM presets
"""
def drums(bpm=120, loops=-1, **kwargs):
    preset(13, osc=0, **kwargs) # sample bass drum
    preset(8, osc=3, **kwargs) # sample hat
    preset(9, osc=4, **kwargs) # sample cow
    preset(10, osc=5, **kwargs) # sample hi cow
    preset(11, osc=2, **kwargs) # sample snare
    preset(1, osc=7, **kwargs) # filter bass
    [bass, snare, hat, cow, hicow, silent] = [1, 2, 4, 8, 16, 32]
    pattern = [bass+hat, hat+hicow, bass+hat+snare, hat+cow, hat, hat+bass, snare+hat, hat]
    bassline = [50, 0, 0, 0, 50, 52, 51, 0]
    while (loops != 0):
        loops = loops - 1
        for i,x in enumerate(pattern):
            if(x & bass): 
                send(osc=0, vel=4, **kwargs)
            if(x & snare):
                send(osc=2, vel=1.5)
            if(x & hat): 
                send(osc=3, vel=1)
            if(x & cow): 
                send(osc=4, vel=1)
            if(x & hicow): 
                send(osc=5, vel=1)
            if(bassline[i]>0):
                send(osc=7, vel=0.5, note=bassline[i]-12, **kwargs)
            else:
                send(vel=0, osc=7, **kwargs)
            time.sleep(1.0/(bpm*2/60))


"""
    C-major chord
"""
def c_major(octave=2,wave=SINE, **kwargs):
    send(osc=0, freq=220.5*octave, wave=wave, vel=1, **kwargs)
    send(osc=1, freq=138.5*octave, wave=wave, vel=1, **kwargs)
    send(osc=2, freq=164.5*octave, wave=wave, vel=1, **kwargs)



