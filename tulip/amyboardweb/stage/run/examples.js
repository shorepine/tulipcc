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
`}
]
