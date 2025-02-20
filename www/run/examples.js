// examples.js
const example_snippets = [{
    't':'music',
    'd':"Launch a pattern sequencer (by @ITD)",
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
# make some UI elements on the REPL
import tulip

def button_cb(e):
    obj = e.get_target_obj()
    print("label of button pressed is %s" % (obj.get_child(0).get_text()))

def slider_cb(e):
    obj = e.get_target_obj()
    print("slider value is %d" % (obj.get_value()))

def check_cb(e):
    obj = e.get_target_obj()
    print("checkbox value is %s" % (obj.get_state()))

def text_cb(e):
    obj = e.get_target_obj()
    print("text value is %s" % (obj.get_text()))


def run(screen):
    screen.bg_color = 9

    screen.add(tulip.UILabel("hello it is a test"), x=200,y=200)
    screen.add(tulip.UIButton(text="Click me", fg_color=255, bg_color=200,  callback=button_cb), x=600, y=200)
    screen.add(tulip.UIButton(text="here again", fg_color=255, bg_color=3, w=250, callback=button_cb), x=700, y=400)

    screen.add(tulip.UISlider(val=50,  w=15, h=150, bar_color=151, handle_color=188, callback=slider_cb), x=300, y=400)
    screen.add(tulip.UISlider(val=50,  w=400, h=15, bar_color=255, handle_color=0, callback=slider_cb), x=250, y=300)

    screen.add(tulip.UIText(text="12",fg_color=255,w=100,bg_color=0, font=tulip.lv.font_unscii_8, callback=text_cb), x=200, y=200)
    screen.add(tulip.UIText(placeholder="Type here", w=220, fg_color=255,bg_color=0, font=tulip.lv.font_unscii_8, callback=text_cb), x=400, y=100)

    screen.add(tulip.UICheckbox(text="Extra text", fg_color=255, bg_color = 0, callback=check_cb), x=400, y=400)
    screen.add(tulip.UICheckbox(fg_color=255, bg_color = 0, callback=check_cb), x=400, y=460)

    screen.handle_keyboard = True
    screen.keep_tfb = True
    screen.present()

if __name__ == '__main__':
    run(tulip.UIScreen())
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
juno6.select_midi_channel(2)
print("Type seq.clear() to stop Juno-6 pattern")
`},{
    't':'games',
    'd':'Rotate an image',
    'c':`
# rotate.py
# 3 shear rotate in the BG
import math, tulip

def shear(angle,x,y):
    '''
    |1  -tan(𝜃/2) |  |1        0|  |1  -tan(𝜃/2) |
    |0      1     |  |sin(𝜃)   1|  |0      1     |

    '''
    # shear 1
    tangent=math.tan(angle/2)
    new_x=round(x-y*tangent)
    new_y=y
    #shear 2
    new_y=round(new_x*math.sin(angle)+new_y)      #since there is no change in new_x according to the shear matrix
    #shear 3
    new_x=round(new_x-new_y*tangent)              #since there is no change in new_y according to the shear matrix
    return new_x,new_y

def rotate(x,y,w,h, out_x, out_y, angle):
    cos = math.cos(angle)
    sin = math.sin(angle)

    # Define the width and height of the destination image
    newWidth = round(abs(w*cos)+abs(h*sin))+1
    newHeight = round(abs(h*cos)+abs(w*sin))+1

    # Find the center of the source image for rotation
    origCenterWidth  = round(((w+1)/2)-1)    #with respect to the source image
    origCenterHeight = round(((h+1)/2)-1)    #with respect to the source image

    # Find the center of the destination image
    newCenterWidth  = round(((newWidth+1)/2)-1)  #with respect to the destination image
    newCenterHeight = round(((newHeight)/2)-1)  #with respect to the destination image

    for i in range(h):
        for j in range(w):
            #co-ordinates of pixel with respect to the center of source image
            src_x = w-1-j-origCenterWidth
            src_y = h-1-i-origCenterHeight

            #Applying the Shear Transformation
            new_x,new_y = shear(angle,src_x,src_y)

            #with rotation, the center will change so new_x and new_y will be the new center
            new_y = out_y+newCenterHeight-new_y
            new_x = out_x+newCenterWidth-new_x

            #tulip.bg_pixel(new_x, new_y, tulip.bg_pixel(j+x,i+y))
            tulip.bg_blit(j+x, i+y, 1, 1, new_x, new_y)

tulip.bg_clear()
tulip.bg_png('/tulip4/sys/ex/g/rabbit_l_0.png', 300, 200)

tic = tulip.ticks_ms()
for i in range(1,10):
    rotate(300,200,48,32,300+(i*50), 200, math.radians(i*10))
print("Took %dms to complete" % (tulip.ticks_ms() - tic))
`},{
    't':'music',
    'd':'Microtonal scales (by @diydsp)',
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
    'd':'Launch an algorithmic music composer (by @nicoVR)',
    'c':`
# see user/periodic2/config.py for instructions!
tulip.download_and_run("periodic2")
`},{
    't':'other',
    'd':'A full featured notepad (by @coolcoder613)',
    'c':`
import lvgl as lv
import tulip
import os

def split(path):
    if path == "":
        return ("", "")
    r = path.rsplit("/", 1)
    if len(r) == 1:
        return ("", path)
    head = r[0]  # .rstrip("/")
    if not head:
        head = "/"
    return (head, r[1])


def dirname(path):
    return split(path)[0]

def basename(path):
    return split(path)[1]

def join(*args):
    return "/".join(args).replace('//','/')

def isdir(path):
    try:
        mode = os.stat(path)[0]
        return mode & 0o040000
    except OSError:
        return False


def isfile(path):
    try:
        return bool(os.stat(path)[0] & 0x8000)
    except OSError:
        return False

class SimpleBrowser:
    def __init__(self, screen, mode, callback):
        """
        Initialize file browser dialog

        Args:
            screen: Parent screen object
            mode: Either "open", "save" or "saveas"
            callback: Function to call with selected path
        """
        self.screen = screen
        self.mode = mode
        self.callback = callback
        self.current_path = os.getcwd()

        # Create modal window
        self.window = lv.obj(screen.group)
        (H_RES, V_RES) = tulip.screen_size()
        self.window.set_size(int(H_RES * 0.9), int(V_RES * 0.8))
        self.window.center()
        self.window.set_style_bg_color(tulip.pal_to_lv(146), 0)  # White background

        # Add title
        title = "Save File" if mode in ("save", "saveas") else "Open File"
        self.title = lv.label(self.window)
        self.title.set_text(title)
        self.title.align(lv.ALIGN.TOP_LEFT, 0, 2)

        # Current path display
        self.path_label = lv.label(self.window)
        self.path_label.set_text(self.current_path)
        self.path_label.align(lv.ALIGN.TOP_RIGHT, 0, 2)

        # File list
        self.list = lv.list(self.window)
        self.list.set_size(int(H_RES * 0.88)-30, int(V_RES * 0.77)-95)
        self.list.align(lv.ALIGN.CENTER, 0, -10)
        self.list.set_style_bg_color(tulip.pal_to_lv(255), 0)

        # Filename input field for save mode
        if mode in ("save", "saveas"):
            self.filename = lv.textarea(self.window)
            self.filename.set_size(int(H_RES * 0.45), 35)
            self.filename.align(lv.ALIGN.BOTTOM_MID, 0, -0)
            self.filename.set_one_line(True)
            self.filename.set_style_text_font(lv.font_montserrat_12, 0)
            self.filename.set_placeholder_text("Enter filename...")
            if hasattr(screen, 'editor'):
                self.filename.set_text(basename(screen.editor.current_file))

        # Buttons
        btn_width = 50
        btn_height = 35

        self.ok_btn = lv.button(self.window)
        self.ok_btn.set_size(btn_width, btn_height)
        self.ok_btn.align(lv.ALIGN.BOTTOM_LEFT, 0, -0)
        self.ok_label = lv.label(self.ok_btn)
        self.ok_label.set_text("OK")
        self.ok_label.center()
        self.ok_btn.add_event_cb(self.on_ok, lv.EVENT.CLICKED, None)

        self.cancel_btn = lv.button(self.window)
        self.cancel_btn.set_size(btn_width, btn_height)
        self.cancel_btn.align(lv.ALIGN.BOTTOM_RIGHT, 0, -0)
        self.cancel_label = lv.label(self.cancel_btn)
        self.cancel_label.set_text("Cancel")
        self.cancel_label.center()
        self.cancel_btn.add_event_cb(self.on_cancel, lv.EVENT.CLICKED, None)

        # Initial population
        self.refresh_file_list()

    def refresh_file_list(self):
        """Refresh the file list with current directory contents"""
        self.list.clean()
        self.path_label.set_text(self.current_path)

        try:
            entries = os.listdir(self.current_path)
            # Sort directories first, then files
            dirs = sorted([e for e in entries if isdir(join(self.current_path, e))])
            if self.current_path != '/':
                dirs.insert(0,'..')

            files = sorted([e for e in entries if isfile(join(self.current_path, e))])
            # Add directories with a "/" suffix
            for d in dirs:
                btn = self.list.add_button(None, f"{d}/")
                btn.add_event_cb(lambda e, d=d: self.on_dir_click(d), lv.EVENT.CLICKED, None)

            # Add files
            for f in files:
                btn = self.list.add_button(None, f"{f}")
                btn.add_event_cb(lambda e, f=f: self.on_file_click(f), lv.EVENT.CLICKED, None)

        except OSError as e:
            raise e
            # Handle case where directory can't be read
            self.list.add_button(None, "Error reading directory")

    def on_dir_click(self, dir):
        """Handle directory click by entering it"""
        if dir=='..':
            parent = dirname(self.current_path)
            if parent != self.current_path:  # Prevent going above root
                self.current_path = parent
                self.refresh_file_list()
        else:
            self.current_path = join(self.current_path, dir)
        self.current_path.replace('//','/')
        self.refresh_file_list()

    def on_file_click(self, filename):
        """Handle file click by selecting it"""
        if self.mode == "open":
            self.selected_file = filename
            self.path_label.set_text(join(self.current_path,filename))
        elif hasattr(self, 'filename'):
            self.filename.set_text(filename)

    def on_ok(self, event):
        """Handle OK button click"""
        if self.mode in ("save", "saveas"):
            if hasattr(self, 'filename'):
                filename = self.filename.get_text().strip()
                if filename:
                    full_path = join(self.current_path, filename)
                    self.cleanup()
                    self.callback(full_path)
        else:  # open mode
            if hasattr(self, 'selected_file'):
                full_path = join(self.current_path, self.selected_file)
                self.cleanup()
                self.callback(full_path)

    def on_cancel(self, event):
        """Handle cancel button click"""
        self.cleanup()
        self.callback(None)

    def cleanup(self):
        """Clean up the browser window"""
        self.window.delete()

class Editor:
    def __init__(self, screen):
        self.screen = screen
        self.current_file = "untitled.txt"
        self.undo_stack = []
        self.redo_stack = []
        self.last_text = ""
        self.filename_mode = False
        self.browser=None

    def create_file_browser(self, mode):
        def browser_callback(path):
            if path:
                self.current_file = path
                if mode == "open":
                    if self.load_text():
                        self.status.set_text(f"Opened {self.current_file}!")
                    else:
                        self.status.set_text("Failed to open file!")
                else:  # save or saveas
                    if self.save_text():
                        self.status.set_text(f"Saved to {self.current_file}!")
                    else:
                        self.status.set_text("Save failed!")
            lv.group_focus_obj(self.ta)

        self.browser = SimpleBrowser(self.screen, mode, browser_callback)


    def save_state(self):
        try:
            current = self.ta.get_text()
            if current != self.last_text:
                self.undo_stack.append(self.last_text)
                self.last_text = current
                self.redo_stack.clear()
        except:
            pass

    def undo(self):
        if self.undo_stack:
            current = self.ta.get_text()
            self.redo_stack.append(current)
            text = self.undo_stack.pop()
            self.ta.set_text(text)
            self.last_text = text

    def redo(self):
        if self.redo_stack:
            current = self.ta.get_text()
            self.undo_stack.append(current)
            text = self.redo_stack.pop()
            self.ta.set_text(text)
            self.last_text = text

    def save_text(self):
        try:
            with open(self.current_file, "wb") as f:
                f.write(self.ta.get_text().encode("utf-8", "ignore"))
            return True
        except:
            return False

    def load_text(self):
        try:
            with open(self.current_file, "rb") as f:
                text = f.read().decode("utf-8", "ignore")
                self.ta.set_text(text)
                self.last_text = text
                self.undo_stack.clear()
                self.redo_stack.clear()
            return True
        except:
            return False
SAVE="Save ^S"
SAVEAS="Save As"
OPEN="Open ^O"
NEW="New ^N"
EXIT="Exit ^Q"
UNDO="Undo ^Z"
REDO="Redo ^Y"
KEYBOARD="Keyboard ^K"

def run(screen):
    def menu_cb(e):
        dropdown = e.get_target_obj()
        option = b" "*64 # should be large enough to store the option
        #option=dropdown.__dereference__(1024)
        dropdown.get_selected_str(option, len(option))
        action(str(option.strip().rstrip(b'\\0'),'utf-8'))

    editor = Editor(screen)
    screen.bg_color = 146  # White background
    screen.handle_keyboard = True
    screen.quit_callback = quit

    (H_RES, V_RES) = tulip.screen_size()

    file_menu = lv.dropdown(screen.group)
    file_menu.align(lv.ALIGN.TOP_LEFT, 2, 2)
    file_menu.set_options(
        "\\n".join([SAVE, SAVEAS, OPEN, NEW, EXIT])
    )
    file_menu.set_text("File")
    file_menu.add_event_cb(menu_cb, lv.EVENT.VALUE_CHANGED, None)

    edit_menu = lv.dropdown(screen.group)
    edit_menu.align(lv.ALIGN.TOP_LEFT, 138, 2)
    edit_menu.set_options("\\n".join([UNDO, REDO, KEYBOARD]))
    edit_menu.set_text("Edit")
    edit_menu.add_event_cb(menu_cb, lv.EVENT.VALUE_CHANGED, None)

    # Create status bar
    status = lv.label(screen.group)
    status.set_pos(5, V_RES - 25)
    status.set_style_text_color(tulip.pal_to_lv(0), 0)
    status.set_text("Ready")
    editor.status = status

    # Create main text area
    ta = lv.textarea(screen.group)
    ta.set_pos(0, 30)
    ta.set_size(H_RES, V_RES - 65)  # Leave room for status bar
    ta.set_style_text_font(lv.font_montserrat_12, 0)
    ta.set_style_bg_color(tulip.pal_to_lv(255), lv.PART.MAIN)
    ta.set_style_text_color(tulip.pal_to_lv(0), 0)
    ta.set_style_border_color(tulip.pal_to_lv(0), lv.PART.CURSOR | lv.STATE.FOCUSED)
    ta.set_placeholder_text("Type away...")
    editor.ta = ta



    def action(name):
        if name == SAVE:
            if editor.current_file == "untitled.txt":
                editor.create_file_browser("save")
            else:
                if editor.save_text():
                    editor.status.set_text(f"Saved to {editor.current_file}!")
                else:
                    editor.status.set_text("Save failed!")
        elif name == SAVEAS:
            editor.create_file_browser("saveas")
        elif name == OPEN:
            editor.create_file_browser("open")
        elif name == NEW:
            editor.ta.set_text("")
            editor.current_file = "untitled.txt"
            editor.save_state()
            editor.status.set_text("New file created")
        elif name == UNDO:
            editor.undo()
        elif name == REDO:
            editor.redo()
        elif name == KEYBOARD:
            tulip.keyboard()
        elif name == EXIT:
            screen.quit()

    def key_handler(key):
        keys = tulip.keys()

        if key == 19:  # Ctrl+S
            action(SAVE)
        elif key == 15:  # Ctrl+O
            action(OPEN)
        elif key == 26:  # Ctrl+Z
            action(UNDO)
        elif key == 25:  # Ctrl+Y
            action(REDO)
        elif key == 11:  # Ctrl+K
            action(KEYBOARD)
        elif key == 14:  # Ctrl+N
            action(NEW)
        else:
            editor.save_state()  # Save state for undo/redo

    # Register keyboard callback
    tulip.keyboard_callback(key_handler)

    # Set initial focus
    lv.group_focus_obj(ta)

    # Try to load previous text
    editor.load_text()

    screen.present()

    # Store references for cleanup
    screen.editor = editor


def quit(screen):
    tulip.keyboard_callback()
    
if __name__ == '__main__':
    run(tulip.UIScreen())
`
}
]
