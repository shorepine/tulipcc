import random, time, math, os
import tulip, amy, music
import m5_8encoder as enc

# initialize
tulip.tfb_stop()
pix_dir = "pix"
(WIDTH, HEIGHT) = tulip.screen_size()

grass_colors = [80, 157, 180, 249]


# clear the screen
tulip.bg_clear(random.choice(grass_colors))
#tulip.bg_str("LOADING...", math.floor(WIDTH/2)-(math.floor(str_len/2)), math.floor(HEIGHT/2), 0, 2)
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
tulip.sprite_register(0,0, rabbit_w, rabbit_h)
tulip.sprite_move(0, 
                  math.floor(WIDTH/2) - half_rabbit_w, 
                  math.floor(HEIGHT/2) - half_rabbit_h)
tulip.sprite_on(0)

class EncVal():
    def __init__(self):
        self.prev = [0] * 8
        enc.read_all_increments()  # clear out values

    def set(self, idx, val):
        self.prev[idx] = val

    def get(self):
        return self.prv_pos
    

# clip values to 0 to 10
def clip(val, min_val, max_val):
    return max(min(val, max_val), min_val)

KNOB_XPOS = 7
KNOB_YPOS = 0
XPOS_PUSH_SCALE = 8
YPOS_PUSH_SCALE = 8
NEW_NOTE_BUTTON1 = 6
NEW_NOTE_BUTTON2 = 1

def new_note_button_det(enc):
    v1 = enc.read_button(NEW_NOTE_BUTTON1)
    v2 = 1- v1
    w1 = enc.read_button(NEW_NOTE_BUTTON2)
    w2 = 1- w1
    x1 = v2 or w2
    print(f'v1: {v1}, v2: {v2}, w1: {w1}, w2: {w2}, x1: {x1}')

    return x1



# This is called every frame by the GPU.
def game_loop(d):

    global rabbit_h,rabbit_w,WIDTH,HEIGHT,ringing_pan
    
    enc_butts = enc.read_all_buttons()
    enc_butts = [1-x for x in enc_butts]  # rev polarity

    if enc_butts[NEW_NOTE_BUTTON1] == 1 and enc_butts[NEW_NOTE_BUTTON2] == 1:
        tulip.bg_clear(random.choice(grass_colors))

    if enc_butts[NEW_NOTE_BUTTON1] == 1 or enc_butts[NEW_NOTE_BUTTON2] == 1:
        tulip.bg_circle(math.floor( d["x"] + rabbit_w/2), 
                        math.floor( d["y"] + rabbit_h/2), 10, 1, 1)    

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

    if f_x < 0 or f_y < 0:
        print("why???", f_x, f_y)
    tulip.sprite_move(0, f_x, f_y)


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

