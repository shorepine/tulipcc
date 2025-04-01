// examples.js
const example_snippets = [{
    't':'music',
    'd':"Set up a MIDI channel to play a piano",
    'c':`
midi.config.reset()
midi.config.add_synth(synth.PatchSynth(6, 256))
# Now set your MIDI device on this page and play a note!
`},{
    't':'music',
    'd':"Load a .wav sample and play a chord",
    'c':`
# some stuart dempster in your browser
import music, tulip
amy.load_sample(tulip.root_dir()+'sys/ex/bcla3.wav', patch=50)
s = synth.OscSynth(wave=amy.PCM, patch=50)
for i,note in enumerate(music.Chord('F:min7').midinotes()):
    s.note_on(note+24, 1, time=i*4000)
    s.note_off(note+24, time=20000)
`},{
    't':'music',
    'd':"Construct a custom FM synth",
    'c':`
import midi, music
# WOOD PIANO amy setup, 4-op FM synth

def make_patch(x):
    amy.start_store_patch()
    amy.send(osc=1, bp0="0,1,5300,0,0,0", phase=0.25, ratio=1, amp="0.3,0,0,1,0,0")
    amy.send(osc=2, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
    amy.send(osc=3, bp0="0,1,6700,0,0,0", phase=0.25, ratio=1, amp="0.23,0,0,1,0,0")
    amy.send(osc=4, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
    amy.send(osc=0, wave=amy.ALGO, algorithm=5, algo_source="1,2,3,4", bp0="0,1,147,0", bp1="0,1,179,1", freq="0,1,0,0,1,1")
    amy.stop_store_patch(1024)

def next(t):
    s = synth.PatchSynth(8, 1024)
    p = music.Progression(["I", "vi", "IV", "V"], music.Key("E:maj"))
    chord_len = 2000
    note_len = 500
    start = 2000
    for x,chord in enumerate(p.chords):
        # Play a chord
        chord_time = (x*chord_len)
        for i,note in enumerate(chord.midinotes()):
            s.note_on(note-12, 1, time=start+chord_time)
            s.note_off(note-12, time=start+chord_time+chord_len-100)
        # And a little arp over it
        for i,note in enumerate(chord.midinotes()):
            note_time = (x*chord_len)+(i*note_len)
            s.note_on(note, 1, time=start+note_time)
            s.note_off(note, time=start+note_time+100)
midi.config.reset()
tulip.defer(make_patch, None, 500)
tulip.defer(next, None, 1500)
`},{
    't':'network',
    'd':"Parse a live API response from the web",
    'c':`
import json, js
def show(j):
    for i in j.keys():
        print("%s -> %s" % (i, j[i]))
        
url = "https://api.github.com/users/shorepine"
js.fetch(url).then(lambda r: r.text()).then(lambda x: json.loads(x)).then(lambda x: show(x))
`},{
    't':'music',
    'd':'Put an echo effect on audio input',
    'c':`
# "Allow audio input" must be turned on 
amy.send(osc=30, pan=0, wave=amy.AUDIO_IN0, vel=1)
amy.send(osc=31, pan=1, wave=amy.AUDIO_IN1, vel=1)
amy.echo(level=1, delay_ms=400, max_delay_ms=1500, feedback=0.8, filter_coef=None)
`},{
    't':'music',
    'd':'Directly play a wav file',
    'c':`
# wav.py
# plays a wavfile from amy external audio

import amy_wave
f = None
try:
    f = amy_wave.open(wav_filename,'rb')
except:
    print("Upload a wav file and type >>> wav_filename='file.wav' first.")

def cb(x):
    frames = f.readframes(256)
    if(len(frames)!=1024):
        frames = bytes(1024)
        tulip.amy_block_done_callback()
    tulip.amy_set_external_input_buffer(frames)

if(f is not None):
    amy.reset()
    amy.send(osc=0,wave=amy.AUDIO_EXT0, pan=0, vel=1)
    amy.send(osc=1,wave=amy.AUDIO_EXT1, pan=1, vel=1)
    tulip.amy_block_done_callback(cb)
`},{
    't':'music',
    'd':'Sample audio in and play it as an instrument',
    'c':`
# sample.py 
# try sampling audio to pcm memorypcm

import tulip, amy, synth, music

buf = bytes()
t = 0
tick_start = 0
ms = 2000
syn = None

def play():
    amy.send(reset=amy.RESET_TIMEBASE+amy.RESET_ALL_OSCS)
    amy.load_sample_bytes(buf, stereo=True, patch=50, loopstart=0, loopend=int(amy.AMY_SAMPLE_RATE*(ms/1000.0)))
    syn = synth.OscSynth(wave=amy.PCM, patch=50)
    for i,note in enumerate(music.Chord('F:min7').midinotes()):
        syn.note_on(note+24, 1, time=i*1000)
        syn.note_off(note+24, time=5000)

def sample(x):
    global buf, syn
    buf = buf + tulip.amy_get_input_buffer()
    if(tulip.ticks_ms() > tick_start + ms): 
        tulip.amy_block_done_callback()
        play()

tick_start = tulip.ticks_ms()
print("Recording for 2s. Make sure audio input is on!")
tulip.amy_block_done_callback(sample)
`
},{
    'd':'Generate audio buffers in Python',
    't':'music',
    'c':`
# makes a sine wave. obviously, it's easier to do this in AMY directly! 
# but just showing how to do it for any audio synthesis in python
from ulab import numpy as np
import ulab
freq = 440.0
amp = 32767
lut_size = 512
count = 0
lut = np.array(amp * np.sin(np.linspace(0,2*np.pi,lut_size,endpoint=False)), dtype=np.int16)
# freq/2 because we send stereo buffer in this example
step_size = ((freq/2.0) * lut_size) / float(amy.AMY_SAMPLE_RATE)
index = 0

def cb(x):
    global count
    t = np.array(np.arange(count*512,count*512+512)*step_size)
    samples = ulab.user.arraymodlut(lut, t);
    tulip.amy_set_external_input_buffer(samples.tobytes())
    if(count==1000):  # stop
        tulip.amy_block_done_callback()
        amy.reset()
    count = count + 1

amy.reset()
amy.send(osc=0,wave=amy.AUDIO_EXT0, pan=0, vel=1)
amy.send(osc=1,wave=amy.AUDIO_EXT1, pan=1, vel=1)
tulip.amy_block_done_callback(cb)
`
}]
