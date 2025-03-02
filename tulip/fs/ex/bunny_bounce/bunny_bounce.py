# a pooping bunny screen saver
import random, math, os
import tulip, amy, music
(WIDTH, HEIGHT) = tulip.screen_size()


    
play_osc = 32
def get_osc():
    global play_osc
    play_osc += 1
    if(play_osc > 63): play_osc = 32
    return play_osc

def twiddle_path(app, x=True):
    incr = app.d["x_incr"]
    if(x is False): incr = app.d["y_incr"]
    poop_color = app.d["poop_color"]
    note = app.d["curr_note"]
    twiddle = random.random()
    if twiddle < 0.1:
        if twiddle <= 0.05:
            incr -= random.random() * 2
            note -= 1
        else:
            incr += random.random() * 2
        note += 1
        poop_color = random.choice(app.poop_colors)
    if(x is False):
        app.d['y_incr'] = incr
    else:
        app.d['x_incr'] = incr
    
    app.d['poop_color'] = poop_color
    app.d['curr_note'] = note


def activate_callback(app):
    pix_dir = app.app_dir + "/pix"
    app.ringing_pan = 0.0

    app.grass_colors = [80, 157, 180, 249]
    app.poop_colors = [143, 162, 198, 226, 238, 247]
    app.poop_prob = 0.5
    app.flower_colors = [16, 56, 116]
    app.flower_prob = 0.1
    app.shroom_colors = [104,36,141,177]
    app.shroom_prob = 0.05

    app.time_til_flowers = 15000
    app.time_til_shrooms = 30000

    app.bounce_base_note = 45
    app.bounce_curr_note = app.bounce_base_note

    app.flower_notes = [3,4,11,12,13]
    app.shroom_notes = [28,25,1]

    tulip.bg_clear(random.choice(app.grass_colors))
    (app.rabbit_w, app.rabbit_h) = (48, 32)
    for i in range(4):
        tulip.sprite_png(pix_dir + "/rabbit_r_%d.png" % (i),(app.rabbit_w*app.rabbit_h)*i)
    for i in range(4):
        tulip.sprite_png(pix_dir + "/rabbit_l_%d.png" % (i),(app.rabbit_w*app.rabbit_h)*(i+4))

    # Draw a line of pixels up top with random colors
    for x in range(WIDTH):
        tulip.bg_pixel(x,0,random.choice(app.grass_colors))

    # For the rest of the lines 
    for y in range(1,HEIGHT):
        x_start = random.randrange(WIDTH)
        tulip.bg_blit(x_start,0,0,1,0,y)


    # Now run the game loop. First setup some variables for the game state. rabbit x and y, frame counter etc
    x_incr = (random.random() * 10) + 10
    y_incr = (random.random() * 10) + 10

    if random.random() < 0.5:
        x_incr *= -1.0
    if random.random() < 0.5:
        y_incr *= -1.0

    app.d = {"poop_color":app.poop_colors[0],
        "x":math.floor(WIDTH/2), "y":math.floor(HEIGHT/2),
        "x_incr":x_incr,"y_incr":y_incr,
        "ani_frame":0,"ani_frames":4,"curr_note":app.bounce_base_note,
        "run":1}

    app.half_rabbit_w = math.floor(app.rabbit_w / 2)
    app.half_rabbit_h = math.floor(app.rabbit_h / 2)

    # Register the first frame, we'll swap out frames during animation
    tulip.sprite_register(0,0, app.rabbit_w, app.rabbit_h)
    tulip.sprite_move(0, 
              math.floor(WIDTH/2) - app.half_rabbit_w, 
              math.floor(HEIGHT/2) - app.half_rabbit_h)
    tulip.sprite_on(0)


    # do this right before takeoff...
    app.start_time = tulip.ticks_ms()
    tulip.frame_callback(game_loop, app)

# This is called every frame by the GPU.
def game_loop(app):
    # This is a bit of a hack for web; the frame scheduler can happen after we quit/alttab away from this app, so we check
    if(not app.active): return

    f_x = math.floor(app.d["x"])
    f_y = math.floor(app.d["y"])

    tulip.sprite_move(0, f_x, f_y)

    poop_x = f_x
    if app.d["x_incr"] < 0:
        poop_x += app.rabbit_w

    poop_y = f_y + app.half_rabbit_h + 5

    if random.random() < app.poop_prob:
        tulip.bg_circle(poop_x,poop_y,5, 72, 1)
        tulip.bg_circle(poop_x,poop_y,3, app.d["poop_color"],1)


    if tulip.ticks_ms() > app.start_time + app.time_til_flowers:
        if random.random() < app.flower_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            for i in range(5):
                #fc = random.randint(0,len(flower_colors)-1)
                off_x = random.randint(-3,3)
                off_y = random.randint(-3,3)
                tulip.bg_circle(center_x + off_x,
                    center_y + off_y,
                    5, random.choice(app.flower_colors), 1)
            tulip.bg_circle(center_x, center_y,2,253,1)
            p_select = random.randrange(0,len(app.flower_notes))
            p = app.flower_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)


    if tulip.ticks_ms() > app.start_time + app.time_til_shrooms:
        if random.random() < app.shroom_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            tulip.bg_roundrect(center_x, center_y,6,15,1,random.choice(app.shroom_colors),1)
            for i in range(2):
                w = random.randrange(10,20)
                off_x = -math.floor(w/2) + 3#random.randint(-3,3) - math.floor(w/2)
                off_y = random.randint(-3,3)
                tulip.bg_roundrect(center_x + off_x,
                                center_y + off_y,
                                w,5,1,random.choice(app.shroom_colors),1)
            p_select = random.randrange(0,len(app.shroom_notes))
            p = app.shroom_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)

    app.d["x"] += app.d["x_incr"]
    app.d["y"] += app.d["y_incr"]

    if app.d["x"] + app.rabbit_w >= WIDTH or app.d["x"] <= 0:
        app.d["x_incr"] *= -1
        twiddle_path(app)
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=app.d["curr_note"], vel=0.25)
        app.ringing_pan += (random.random() - 0.5) * 0.1
        if app.ringing_pan > 0.75:
            app.ringing_pan = 0.75
        elif app.ringing_pan < -0.75:
            app.ringing_pan = -0.75
        amy.send(osc=0,pan=app.ringing_pan)
        #print("x_incr: " , x_incr)
        #tulip.cpu(2)
    if app.d["y"] + app.rabbit_h >= HEIGHT or app.d["y"] <= 0:
        app.d["y_incr"] *= -1
        twiddle_path(app)
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=app.d["curr_note"], vel=0.25)
        app.ringing_pan += (random.random() - 0.5) * 0.1
        if app.ringing_pan > 0.75:
            app.ringing_pan = 0.75
        elif app.ringing_pan < -0.75:
            app.inging_pan = -0.75
        amy.send(osc=0,pan=app.ringing_pan)


    if app.d["x"] <= 0:
        app.d["x"] = 1
    elif app.d["x"] >= WIDTH:
        app.d["x"] = WIDTH - 1

    if app.d["y"] <= 0:
        app.d["y"] = 1
    elif app.d["y"] >= HEIGHT:
        app.d["y"] = HEIGHT - 1

    # Change the sprite 0 to the next sprite in the stack
    app.d["sprite_frame"] = app.d["ani_frame"]
    if app.d["x_incr"] < 0:
        app.d["sprite_frame"] += 4
    tulip.sprite_register(0, (app.rabbit_w*app.rabbit_h)*(app.d["sprite_frame"]), app.rabbit_w, app.rabbit_h)
    app.d["ani_frame"] = (app.d["ani_frame"] + 1) % app.d["ani_frames"]

    # fill background with noise pattern
    for i in range(100):
        g_x = int(random.random() * WIDTH)
        g_y = int(random.random() * HEIGHT)    
        tulip.bg_pixel(g_x,g_y,random.choice(app.grass_colors))

    #14
    amy.send(osc=0, wave=amy.PCM, patch=18,  vel=0.25, feedback=1)


def deactivate_callback(app):
    amy.reset()

def run(app):
    app.game = True
    app.hide_task_bar = True
    amy.reset()
    app.activate_callback = activate_callback
    app.deactivate_callback = deactivate_callback
    app.present()



