// examples.js
const example_snippets = [{
    d:"Download and run a <b>pattern sequencer</b>",
    c:`
tulip.download_and_run('tracks')

`},{
    d:"Run a drum machine",
    c:`
tulip.run('drums')
`},{
    d:"Set up a MIDI channel to play a piano",
    c:`
midi.config.reset()
midi.config.add_synth(synth.PatchSynth(6, 256))
# Now set your MIDI device on this page and play a note!
`},{
    d:"Move a sprite around the screen with the keyboard",
    c:`
# Use control-q to exit
import tulip
def game_loop(app):
    app.player.joy_move()
    app.player.move()
def game_setup(app):
    app.player = tulip.Player()
    app.player.load(tulip.root_dir()+"/sys/im/tiny_town/tile_0105.png", 16, 16)
    app.player.on()
    app.player.x = 300
    app.player.y = 300
    tulip.frame_callback(game_loop, app)
def run(app):
    app.game = True
    app.activate_callback = game_setup
    app.present()
if __name__ == "__main__":
    run(tulip.UIScreen())
`},{
    d:"Chat on the Tulip World BBS",
    c:`
# Set your own username!!
world.username="anonymous"
tulip.run('worldui')
`},{
    d:"Play a complex FM synthesis example",
    c:`
tulip.run('xanadu')
`},{
    d:"Scrolling backgrounds",
    c:`
# Use control-Q to exit
tulip.run('parallax')
`},{
    d:"Buttons, sliders and entry widgets",
    c:`
tulip.run("buttons")
`},{
    d:"Load a .wav sample and play a chord",
    c:`
# some stuart dempster in your browser
import music
amy.load_sample('../sys/ex/bcla3.wav', patch=50)
s = synth.OscSynth(wave=amy.PCM, patch=50)
for i,note in enumerate(music.Chord('F:min7').midinotes()):
    s.note_on(note+24, 1, time=i*4000)
    s.note_off(note+24, time=20000)
`},{
    d:"Load a small holiday animation with music",
    c:`
# use control-q to quit
tulip.download_and_run('xmas')
`},{
    d:"Construct a custom FM synth and play a progression",
    c:`
import midi, music
# WOOD PIANO amy setup, 4-op FM synth

midi.config.reset()
try:
    already_stored
except NameError:
    amy.start_store_patch()
    amy.send(osc=1, bp0="0,1,5300,0,0,0", phase=0.25, ratio=1, amp="0.3,0,0,1,0,0")
    amy.send(osc=2, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
    amy.send(osc=3, bp0="0,1,6700,0,0,0", phase=0.25, ratio=1, amp="0.23,0,0,1,0,0")
    amy.send(osc=4, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
    amy.send(osc=0, wave=amy.ALGO, algorithm=5, algo_source="1,2,3,4", bp0="0,1,147,0", bp1="0,1,179,1", freq="0,1,0,0,1,1")
    amy.stop_store_patch(1024)
    already_stored = True

s = synth.PatchSynth(8, 1024)
p = music.Progression(["I", "vi", "IV", "V"], music.Key("E:maj"))
chord_len = 2000
note_len = 500
start = 1000
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
`},{
    d:"Parse a live API response from the web",
    c:`
import json, js
def show(j):
    for i in j.keys():
        print("%s -> %s" % (i, j[i]))
        
url = "https://api.github.com/users/shorepine"
js.fetch(url).then(lambda r: r.text()).then(lambda x: json.loads(x)).then(lambda x: show(x))
`},{
    d:"Play a sequence and show the Juno-6 patch editor",
    c:`
import sequencer
sequencer.clear()
syn = synth.PatchSynth(1, 0)
midi.config.add_synth(channel=2, synth=syn)
arp_notes = [48,50,52,49,56,58,60,57]
seq= sequencer.Sequence(8, 16)
for i in range(16):
    seq.add([i], syn.note_on, [arp_notes[i%8], 1])

def stop():
    sequencer.clear()
    syn.release()

# We update the juno6 channel dropdown to show ours after it opens
run('juno6')
def update(x):
    juno6.channel_selector.do_callback(1)
    juno6.channel_selector.set_selected(1)
tulip.defer(update, None, 500)

print("to stop sequencer, run stop()")
`}
]
