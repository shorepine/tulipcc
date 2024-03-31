# a pooping bunny screen saver
import random, time, math, os
import tulip, amy, music


def twiddle_path(incr, poop_color, note):
    global poop_colors
    twiddle = random.random()
    if twiddle < 0.1:
        if twiddle <= 0.05:
            incr -= random.random() * 2
            note -= 1
        else:
            incr += random.random() * 2
            note += 1

        return (incr, random.choice(poop_colors),note)

    else:
        return (incr, poop_color,note)
    
play_osc = 32
def get_osc():
    global play_osc
    play_osc += 1
    if(play_osc > 63): play_osc = 32
    return play_osc


tulip.tfb_stop()

pix_dir = "pix"

(WIDTH, HEIGHT) = tulip.screen_size()

ringing_pan = 0.0

grass_colors = [80, 157, 180, 249]
poop_colors = [143, 162, 198, 226, 238, 247]
poop_prob = 0.5
flower_colors = [16, 56, 116]
flower_prob = 0.1
shroom_colors = [104,36,141,177]
shroom_prob = 0.05

time_til_flowers = 15000
time_til_shrooms = 30000

bounce_base_note = 45
bounce_curr_note = bounce_base_note

flower_notes = [3,4,11,12,13]
shroom_notes = [28,25,1]

str_len = tulip.bg_str("LOADING...", WIDTH, math.floor(HEIGHT/2), 0, 2)
# clear the screen
tulip.bg_clear(random.choice(grass_colors))
tulip.bg_str("LOADING...", math.floor(WIDTH/2)-(math.floor(str_len/2)), math.floor(HEIGHT/2), 0, 2)
# Load the rabbit sprite frames into sprite RAM
(rabbit_w, rabbit_h) = (48, 32)
for i in range(4):
    tulip.sprite_png(pix_dir + "/rabbit_r_%d.png" % (i),(rabbit_w*rabbit_h)*i)
for i in range(4):
    tulip.sprite_png(pix_dir + "/rabbit_l_%d.png" % (i),(rabbit_w*rabbit_h)*(i+4))


#ms_start = tulip.ticks_ms()
#print("starting bg fill at: ", ms_start)
# fill background with noise pattern

#num_c = len(grass_colors)
#for x in range(WIDTH/16):
#    for y in range(HEIGHT/16):         
#        c_n = int(num_c * random.random())
#        tulip.bg_pixel(x+WIDTH,y,grass_colors[c_n])
#        tulip.bg_pixel(x+WIDTH,y,random.choice(grass_colors))

#tulip.bg_blit(WIDTH,0,WIDTH,HEIGHT,0,0)

# Draw a line of pixels up top with random colors
for x in range(WIDTH*2):
    tulip.bg_pixel(x,0,random.choice(grass_colors))

# For the rest of the lines 
for y in range(1,HEIGHT):
    x_start = random.randrange(WIDTH)
    tulip.bg_blit(x_start,0,WIDTH,1,0,y)

#ms_end = tulip.ticks_ms()

#print("ended bg fill at:", ms_end, "ellapsed time:", ms_end - ms_start)


# Now run the game loop. First setup some variables for the game state. rabbit x and y, frame counter etc
x_incr = (random.random() * 10) + 10
y_incr = (random.random() * 10) + 10

if random.random() < 0.5:
    x_incr *= -1.0
if random.random() < 0.5:
    y_incr *= -1.0

d = {"poop_color":poop_colors[0],
    "x":math.floor(WIDTH/2), "y":math.floor(HEIGHT/2),
    "x_incr":x_incr,"y_incr":y_incr,
    "ani_frame":0,"ani_frames":4,"curr_note":bounce_base_note,
    "run":1}



half_rabbit_w = math.floor(rabbit_w / 2)
half_rabbit_h = math.floor(rabbit_h / 2)

# Register the first frame, we'll swap out frames during animation
tulip.sprite_register(0,0, rabbit_w, rabbit_h)
tulip.sprite_move(0, 
                  math.floor(WIDTH/2) - half_rabbit_w, 
                  math.floor(HEIGHT/2) - half_rabbit_h)
tulip.sprite_on(0)


# This is called every frame by the GPU.
def game_loop(d):

    global rabbit_h,rabbit_w,WIDTH,HEIGHT,ringing_pan

    f_x = math.floor(d["x"])
    f_y = math.floor(d["y"])

    if f_x < 0 or f_y < 0:
        print("why???", f_x, f_y)
    tulip.sprite_move(0, f_x, f_y)


    poop_x = f_x
    if d["x_incr"] < 0:
        poop_x += rabbit_w

    poop_y = f_y + half_rabbit_h + 5

    if random.random() < poop_prob:
        tulip.bg_circle(poop_x,poop_y,5, 72, 1)
        tulip.bg_circle(poop_x,poop_y,3, d["poop_color"],1)


    if tulip.ticks_ms() > start_time + time_til_flowers:
        if random.random() < flower_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            for i in range(5):
                #fc = random.randint(0,len(flower_colors)-1)
                off_x = random.randint(-3,3)
                off_y = random.randint(-3,3)
                tulip.bg_circle(center_x + off_x,
                    center_y + off_y,
                    5, random.choice(flower_colors), 1)
            tulip.bg_circle(center_x, center_y,2,253,1)
            p_select = random.randrange(0,len(flower_notes))
            p = flower_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)


    if tulip.ticks_ms() > start_time + time_til_shrooms:
        if random.random() < shroom_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            tulip.bg_roundrect(center_x, center_y,6,15,1,random.choice(shroom_colors),1)
            for i in range(2):
                w = random.randrange(10,20)
                off_x = -math.floor(w/2) + 3#random.randint(-3,3) - math.floor(w/2)
                off_y = random.randint(-3,3)
                tulip.bg_roundrect(center_x + off_x,
                                center_y + off_y,
                                w,5,1,random.choice(shroom_colors),1)
            p_select = random.randrange(0,len(shroom_notes))
            p = shroom_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)

    d["x"] += d["x_incr"]
    d["y"] += d["y_incr"]

    if d["x"] + rabbit_w >= WIDTH or d["x"] <= 0:
        d["x_incr"] *= -1
        (d["x_incr"], d["poop_color"],d["curr_note"]) = twiddle_path(d["x_incr"], d["poop_color"],d["curr_note"])
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=d["curr_note"], vel=0.25)
        ringing_pan += (random.random() - 0.5) * 0.1
        if ringing_pan > 0.75:
            ringing_pan = 0.75
        elif ringing_pan < -0.75:
            ringing_pan = -0.75
        amy.send(osc=0,pan=ringing_pan)
        #print("x_incr: " , x_incr)
        #tulip.cpu(2)
    if d["y"] + rabbit_h >= HEIGHT or d["y"] <= 0:
        d["y_incr"] *= -1
        (d["y_incr"], d["poop_color"],d["curr_note"]) = twiddle_path(d["y_incr"], d["poop_color"],d["curr_note"])
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=d["curr_note"], vel=0.25)
        ringing_pan += (random.random() - 0.5) * 0.1
        if ringing_pan > 0.75:
            ringing_pan = 0.75
        elif ringing_pan < -0.75:
            ringing_pan = -0.75
        amy.send(osc=0,pan=ringing_pan)
        #tulip.cpu(2)
        #print("y_incr: " , y_incr)

    if d["x"] <= 0:
        d["x"] = 1
    elif d["x"] >= WIDTH:
        d["x"] = WIDTH - 1

    if d["y"] <= 0:
        d["y"] = 1
    elif d["y"] >= HEIGHT:
        d["y"] = HEIGHT - 1

    # Change the sprite 0 to the next sprite in the stack
    d["sprite_frame"] = d["ani_frame"]
    if d["x_incr"] < 0:
        d["sprite_frame"] += 4
    tulip.sprite_register(0, (rabbit_w*rabbit_h)*(d["sprite_frame"]), rabbit_w, rabbit_h)
    d["ani_frame"] = (d["ani_frame"] + 1) % d["ani_frames"]

    # fill background with noise pattern
    for i in range(100):
        g_x = int(random.random() * WIDTH)
        g_y = int(random.random() * HEIGHT)    
        tulip.bg_pixel(g_x,g_y,random.choice(grass_colors))


# do this right before takeoff...
start_time = tulip.ticks_ms()

# Register the frame callback and data
tulip.frame_callback(game_loop, d)

#14
amy.send(osc=0, wave=amy.PCM, patch=18,  vel=0.25, feedback=1)

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


