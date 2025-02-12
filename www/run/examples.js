// examples.js
const example_snippets = [{
    't':'music',
    'd':"ITD's pattern sequencer",
    'c':`
tulip.download_and_run('tracks')
`},{
    't':'music',
    'd':"Run a drum machine",
    'c':`
tulip.run('drums')
`},{
    't':'music',
    'd':"Set up a MIDI channel to play a piano",
    'c':`
midi.config.reset()
midi.config.add_synth(synth.PatchSynth(6, 256))
# Now set your MIDI device on this page and play a note!
`},{
    't':'games',
    'd':"Move a sprite around the screen with the keyboard",
    'c':`
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
    't':'network',
    'd':"Chat on the Tulip World BBS",
    'c':`
# Set your own username!!
world.username="anonymous"
tulip.run('worldui')
`},{
    't':'music',
    'd':"Play a complex FM synthesis example",
    'c':`
tulip.run('xanadu')
`},{
    't':'games',
    'd':"Scrolling backgrounds",
    'c':`
# Use control-Q to exit
tulip.run('parallax')
`},{
    't':'other',
    'd':"Buttons, sliders and entry widgets",
    'c':`
tulip.run("buttons")
`},{
    't':'music',
    'd':"Load a .wav sample and play a chord",
    'c':`
# some stuart dempster in your browser
import music
amy.load_sample('../sys/ex/bcla3.wav', patch=50)
s = synth.OscSynth(wave=amy.PCM, patch=50)
for i,note in enumerate(music.Chord('F:min7').midinotes()):
    s.note_on(note+24, 1, time=i*4000)
    s.note_off(note+24, time=20000)
`},{
    't':'games',
    'd':"A small holiday animation with MIDI file playback",
    'c':`
# use control-q to quit
tulip.download_and_run('xmas')
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
    t:'music',
    d:"Sequence with drums and the Juno-6 editor",
    c:`
import sequencer
# Set up default synth and add a new one on channel 2
midi.config.reset()
midi.add_default_synths()
sequencer.tempo(140)
syn = synth.PatchSynth(1, 0)
midi.config.add_synth(channel=2, synth=syn)
arp_notes = [48,50,None,49,56,None,46,44,None,None,49,56,58,60,None,56]
seq= sequencer.Sequence(8, 16)
for i in range(16):
    note = arp_notes[i%len(arp_notes)]
    if note is not None:
        seq.add(i, syn.note_on, [note, 1])

# We update the juno6 channel dropdown to show ours after it opens
run('drums')
run('juno6')
def update(x):
    juno6.channel_selector.do_callback(1)
    juno6.channel_selector.set_selected(1)
tulip.defer(update, None, 500)
print("Type seq.clear() to stop Juno-6 pattern")
`},{
    't':'music',
    'd':'Microtonal scales (@diydsp)',
    'c':`
import math 

# hello
print("Microtonal keyboard fun!")
print("play on keys Q through P and 2 through -")
print("add your own ratios for microtonal scales")
print("page up/down to change patch")

def key_to_note( key ):

    print(f'key {key}, {str(key)}')
    super_piano_string = "q2w3e4r5t6y7u8i9o0p-";

    
    index = super_piano_string.find( chr(key) )
    print(f"index:{index}")
    if index == -1:
        return None
    
    ratios_12tet = [
        1.0,                   # C
        1.0594630943592953,    # C#
        1.122462048309373,     # D
        1.189207115002721,     # D#
        1.2599210498948732,    # E
        1.3348398541700344,    # E#
        1.3348398541700344,    # F
        1.4142135623730951,    # F# 
        1.4983070768766815,    # G
        1.5874010519681994,    # G#
        1.681792830507429,     # A
        1.7817974362806785,    # A#
        1.8877486253633868,    # B
        2.0,                    # B#
        2.0,                    # C
        2*1.05946309435929,    # C2#
        2*1.122462048309373,     # D2
        2*1.189207115002721,     # D2#
        2*1.2599210498948732,    # E2
        2*1.3348398541700344,    # E2#
        2*1.3348398541700344,    # F2
        2*1.4142135623730951,    # F2#
        2*1.4983070768766815,    # G2
        2*1.5874010519681994,    # G2#
        2*1.681792830507429,     # A2
        2*1.7817974362806785,    # A2#
    ]

    ratios_metric = [
        1.0,    # C
        1.1,    # C#
        1.2,     # D
        1.3,     # D#
        1.4,    # E
        1.5,    # E#
        1.6,    # F
        1.7,    # F# 
        1.8,    # G
        1.9,    # G#
        2.0,     # A
        2.2,    # A#
        2.4,    # B
        2.6, # B#
        2.8,  # C
        3.0,    # C2#
        3.2,     # D2
        3.4,     # D2#
        3.6,    # E2
        3.8,    # E2#
        4.0,    # F2
        2*1.4142135623730951,    # F2#
        2*1.4983070768766815,    # G2
        2*1.5874010519681994,    # G2#
        2*1.681792830507429,     # A2
        2*1.7817974362806785,    # A2#
    ]

    
    return ratios_metric[index]


def process_key( key ):
    global patch_num

    # space
    if key == 32: print( "toggle")        
        
    elif key >= 65 and key <= 90: # shift-A through shift-Z
        pass
    
    # a through z, lowercase 
    elif (key >= 97 and key <= 122) or key >= 45 and key <= 57:   # numbers, and - and a few more
        ratio = key_to_note( key )
        print(f'ratio: {ratio}')
        if ratio is not None:
            note_num = 36 + 12 * math.log( ratio ) / math.log( 2 )
            print( f"note_num: {note_num}" )
                  
            amy.send(voices=0,load_patch=patch_num,note=note_num,vel=1)
            
        pass
    
    # cursor keys
    elif key == 259: print("up")
    elif key == 258: print("down")  
    elif key == 260: print("left")  
    elif key == 261: print("right") 

    # esc
    #if key == 0x29: 

    # page up/down
    elif key== 25:
        #print(1)
        patch_num += 1
        print(f'patch_num: {patch_num}')
        amy.send(osc=0,vel=0)
    elif key == 22:#print(-1)
        patch_num -= 1
        print(f'patch_num: {patch_num}')
        amy.send(osc=0,vel=0)

    else:   
        print("unhandled key: %d" % (key))
        print(f'{tulip.keys()}')


patch_num = 129
amy.send(voices=0,load_patch=patch_num,note=45,vel=1)

tulip.key_scan(1)
tulip.keyboard_callback( process_key )
`},{
    't':'other',
    'd':'Show the Tulip RGB332 pallete',
    'c':
`
# draw the rgb332 palette nicely
import tulip
w = 32
h = 75
tulip.bg_clear()
for b in [0, 1, 2, 3]:
    for rg in range(64):
        c = (rg << 2) | b
        x = ((b*8) + (rg % 8)) * w
        y = int(rg / 8) * h
        tulip.bg_rect(x,y,w,h,c,1)
        tc = 255-c
        tulip.bg_str("%03d" % (c), x+7, y+41, tc, 8)
`}
]
