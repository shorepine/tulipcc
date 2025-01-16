import random, time, math, os
import tulip, amy, music
from tulip import ticks_ms, seq_add_callback, seq_remove_callback, seq_ticks
import m5_8encoder as enc

# initialize
tulip.tfb_stop()
pix_dir = "pix"
(WIDTH, HEIGHT) = tulip.screen_size()

grass_colors = [80, 157, 180, 249]


# clear the screen
tulip.bg_clear(random.choice(grass_colors))
tulip.bg_str("LOADING...", 0, math.floor(HEIGHT/2), 0, 2)


# Load the rabbit sprite frames into sprite RAM
(rabbit_w, rabbit_h) = (48, 32)
tulip.sprite_png(pix_dir + "/rabbit_r_%d.png" % (0), rabbit_w*rabbit_h*0 )

# Draw a line of pixels up top with random colors
for x in range(WIDTH):
    tulip.bg_pixel(x,0,random.choice(grass_colors))

# For the rest of the lines 
for y in range(1,HEIGHT):
    x_start = random.randrange(WIDTH)
    tulip.bg_blit(x_start,0,0,1,0,y)   


d = {   "x":math.floor(WIDTH/2), "y":math.floor(HEIGHT/2),
        "run":1}

half_rabbit_w = math.floor(rabbit_w / 2)
half_rabbit_h = math.floor(rabbit_h / 2)

# Register the first frame, we'll swap out frames during animation
# sprite_register(self.sprite_id,self.mem_pos, self.width, self.height)
tulip.sprite_register(0,0, rabbit_w, rabbit_h)
tulip.sprite_move(0, 
                  math.floor(WIDTH/2) - half_rabbit_w, 
                  math.floor(HEIGHT/2) - half_rabbit_h)
tulip.sprite_on(0)

tulip.sprite_register(1,0, rabbit_w, rabbit_h)
tulip.sprite_move(1, math.floor(WIDTH/2) - half_rabbit_w, 0)
tulip.sprite_on(1)

class Note():
    def __init__(self, pos, note, vel, dur):
        self.pos = pos
        self.note = note
        self.vel = vel
        self.dur = dur

    def __repr__(self):
        return f"Note: {self.note} at {self.pos} with vel {self.vel} for {self.dur} ticks"


class NoteManager():
    def __init__(self):
        self.notes = []
        
    def add(self, pos, note, vel, dur):
        new_note = Note(pos, note, vel, dur)
        self.notes.append(new_note)
    
    def display_notes(self):
        print(self.notes)

class EncVal():
    def __init__(self):
        self.prev = [0] * 8
        enc.read_all_increments()  # clear out values

    def set(self, idx, val):
        self.prev[idx] = val

    def get(self):
        return self.prv_pos
    

def clip(val, min_val, max_val):
    return max(min(val, max_val), min_val)

class KeebMgr():
    def __init__(self):
        self.note_add_down=False

    def note_add_down_get(self):
        return self.note_add_down

    def note_add_down_set(self, val):
        self.note_add_down = val    

KNOB_XPOS = 7
KNOB_YPOS = 0
XPOS_PUSH_SCALE = 8
YPOS_PUSH_SCALE = 8
NEW_NOTE_BUTTON1 = 6
NEW_NOTE_BUTTON2 = 1

note_manager = NoteManager()
keeb_mgr = KeebMgr()    

def beat_callback(t):
    global app
    current_beat = int((seq_ticks() / 96) % 4)
    tulip.sprite_move(1, math.floor(WIDTH/4)*current_beat, 0 )

    
# This is called every frame by the GPU.
def game_loop(d):

    global rabbit_h,rabbit_w,WIDTH,HEIGHT,ringing_pan
    
    enc_butts = enc.read_all_buttons()
    enc_butts = [1-x for x in enc_butts]  # rev polarity

    if enc_butts[NEW_NOTE_BUTTON1] == 1 and enc_butts[NEW_NOTE_BUTTON2] == 1:
        tulip.bg_clear(random.choice(grass_colors))

    elif keeb_mgr.note_add_down_get() == False and \
            ( enc_butts[NEW_NOTE_BUTTON1] == 1 or enc_butts[NEW_NOTE_BUTTON2] == 1):
            tulip.bg_circle(math.floor( d["x"] + rabbit_w/2), 
                            math.floor( d["y"] + rabbit_h/2), 10, 1, 1)  
            keeb_mgr.note_add_down_set(True)
            note_manager.add(d["x"], d["y"], 0.5, 12 ) 
            note_manager.display_notes()    
        
    else:
        dx = enc.read_increment(KNOB_XPOS)
        dy = enc.read_increment(KNOB_YPOS)
        bx = 1 - enc_butts[KNOB_XPOS]
        by = 1 - enc_butts[KNOB_YPOS]
        d["x"] += dx*(1 + bx*XPOS_PUSH_SCALE)
        d["y"] -= dy*(1 + by*YPOS_PUSH_SCALE)
        d["x"] = clip(d["x"], 0, WIDTH-rabbit_w)
        d["y"] = clip(d["y"], 0, HEIGHT-rabbit_h)    
        f_x = math.floor(d["x"])
        f_y = math.floor(d["y"])
        tulip.sprite_move(0, f_x, f_y)

    if keeb_mgr.note_add_down_get() == True and \
        enc_butts[NEW_NOTE_BUTTON1] == 0 and enc_butts[NEW_NOTE_BUTTON2] == 0:
            keeb_mgr.note_add_down_set(False)

    # fill background with noise pattern
    for i in range(10):
        g_x = int(random.random() * WIDTH)
        g_y = int(random.random() * HEIGHT)    
        tulip.bg_pixel(g_x,g_y,random.choice(grass_colors))


# initialize
amy.reset()
start_time = tulip.ticks_ms()  # do this right before takeoff...
tulip.frame_callback(game_loop, d)   # Register the frame callback and data
amy.send(osc=0, wave=amy.PCM, patch=18,  vel=0.25, feedback=1) #14


current_beat = int((seq_ticks() / 96) % 4)
tulip.seq_add_callback(beat_callback, int(amy.SEQUENCER_PPQ*4/2))

# Run in a loop forever. Catch ctrl-c
try:
    while d["run"]:
        # In an infinite loop , it's better to sleep than to say "pass", give the Tulip some time to breathe
        time.sleep_ms(100)
        #pass
except KeyboardInterrupt:
    d["run"] = 0


# Clean up a bit
amy.reset()
tulip.key_scan(0)
tulip.frame_callback()
tulip.bg_clear()
tulip.sprite_clear()
tulip.tfb_start()

