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

class BunnyBounce(tulip.UIScreenGame):
    def __init__(self, num_blobs = 4):
        super().__init__()

    def twiddle_path(self, incr, poop_color, note):
        twiddle = random.random()
        if twiddle < 0.1:
            if twiddle <= 0.05:
                incr -= random.random() * 2
                note -= 1
            else:
                incr += random.random() * 2
                note += 1

            return (incr, random.choice(self.poop_colors),note)

        else:
            return (incr, poop_color,note)

    def draw_background(self, app_dir):
        pix_dir = app_dir + "/pix"
        self.ringing_pan = 0.0

        self.grass_colors = [80, 157, 180, 249]
        self.poop_colors = [143, 162, 198, 226, 238, 247]
        self.poop_prob = 0.5
        self.flower_colors = [16, 56, 116]
        self.flower_prob = 0.1
        self.shroom_colors = [104,36,141,177]
        self.shroom_prob = 0.05

        self.time_til_flowers = 15000
        self.time_til_shrooms = 30000

        self.bounce_base_note = 45
        self.bounce_curr_note = self.bounce_base_note

        self.flower_notes = [3,4,11,12,13]
        self.shroom_notes = [28,25,1]

        tulip.bg_clear(random.choice(self.grass_colors))
        (self.rabbit_w, self.rabbit_h) = (48, 32)
        for i in range(4):
            tulip.sprite_png(pix_dir + "/rabbit_r_%d.png" % (i),(self.rabbit_w*self.rabbit_h)*i)
        for i in range(4):
            tulip.sprite_png(pix_dir + "/rabbit_l_%d.png" % (i),(self.rabbit_w*self.rabbit_h)*(i+4))

        # Draw a line of pixels up top with random colors
        for x in range(WIDTH):
            tulip.bg_pixel(x,0,random.choice(self.grass_colors))

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

        self.d = {"poop_color":self.poop_colors[0],
            "x":math.floor(WIDTH/2), "y":math.floor(HEIGHT/2),
            "x_incr":x_incr,"y_incr":y_incr,
            "ani_frame":0,"ani_frames":4,"curr_note":self.bounce_base_note,
            "run":1}



        self.half_rabbit_w = math.floor(self.rabbit_w / 2)
        self.half_rabbit_h = math.floor(self.rabbit_h / 2)

        # Register the first frame, we'll swap out frames during animation
        tulip.sprite_register(0,0, self.rabbit_w, self.rabbit_h)
        tulip.sprite_move(0, 
                  math.floor(WIDTH/2) - self.half_rabbit_w, 
                  math.floor(HEIGHT/2) - self.half_rabbit_h)
        tulip.sprite_on(0)


        # do this right before takeoff...
        self.start_time = tulip.ticks_ms()
        tulip.frame_callback(game_loop, self)

# This is called every frame by the GPU.
def game_loop(game):
    f_x = math.floor(game.d["x"])
    f_y = math.floor(game.d["y"])

    if f_x < 0 or f_y < 0:
        print("why???", f_x, f_y)
    tulip.sprite_move(0, f_x, f_y)


    poop_x = f_x
    if game.d["x_incr"] < 0:
        poop_x += game.rabbit_w

    poop_y = f_y + game.half_rabbit_h + 5

    if random.random() < game.poop_prob:
        tulip.bg_circle(poop_x,poop_y,5, 72, 1)
        tulip.bg_circle(poop_x,poop_y,3, game.d["poop_color"],1)


    if tulip.ticks_ms() > game.start_time + game.time_til_flowers:
        if random.random() < game.flower_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            for i in range(5):
                #fc = random.randint(0,len(flower_colors)-1)
                off_x = random.randint(-3,3)
                off_y = random.randint(-3,3)
                tulip.bg_circle(center_x + off_x,
                    center_y + off_y,
                    5, random.choice(game.flower_colors), 1)
            tulip.bg_circle(center_x, center_y,2,253,1)
            p_select = random.randrange(0,len(game.flower_notes))
            p = game.flower_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)


    if tulip.ticks_ms() > game.start_time + game.time_til_shrooms:
        if random.random() < game.shroom_prob:
            center_x = random.randint(15,WIDTH-15)
            center_y = random.randint(15,HEIGHT-15)
            tulip.bg_roundrect(center_x, center_y,6,15,1,random.choice(game.shroom_colors),1)
            for i in range(2):
                w = random.randrange(10,20)
                off_x = -math.floor(w/2) + 3#random.randint(-3,3) - math.floor(w/2)
                off_y = random.randint(-3,3)
                tulip.bg_roundrect(center_x + off_x,
                                center_y + off_y,
                                w,5,1,random.choice(game.shroom_colors),1)
            p_select = random.randrange(0,len(game.shroom_notes))
            p = game.shroom_notes[p_select]
            amy.send(osc=get_osc(), wave=amy.PCM, patch=p, vel=0.25)

    game.d["x"] += game.d["x_incr"]
    game.d["y"] += game.d["y_incr"]

    if game.d["x"] + game.rabbit_w >= WIDTH or game.d["x"] <= 0:
        game.d["x_incr"] *= -1
        (game.d["x_incr"], game.d["poop_color"],game.d["curr_note"]) = game.twiddle_path(game.d["x_incr"], game.d["poop_color"],game.d["curr_note"])
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=game.d["curr_note"], vel=0.25)
        game.ringing_pan += (random.random() - 0.5) * 0.1
        if game.ringing_pan > 0.75:
            game.ringing_pan = 0.75
        elif game.ringing_pan < -0.75:
            game.ringing_pan = -0.75
        amy.send(osc=0,pan=game.ringing_pan)
        #print("x_incr: " , x_incr)
        #tulip.cpu(2)
    if game.d["y"] + game.rabbit_h >= HEIGHT or game.d["y"] <= 0:
        game.d["y_incr"] *= -1
        (game.d["y_incr"], game.d["poop_color"],game.d["curr_note"]) = game.twiddle_path(game.d["y_incr"], game.d["poop_color"],game.d["curr_note"])
        amy.send(osc=get_osc(), wave=amy.PCM, patch=13, note=game.d["curr_note"], vel=0.25)
        game.ringing_pan += (random.random() - 0.5) * 0.1
        if game.ringing_pan > 0.75:
            game.ringing_pan = 0.75
        elif game.ringing_pan < -0.75:
            rgame.inging_pan = -0.75
        amy.send(osc=0,pan=game.ringing_pan)


    if game.d["x"] <= 0:
        game.d["x"] = 1
    elif game.d["x"] >= WIDTH:
        game.d["x"] = WIDTH - 1

    if game.d["y"] <= 0:
        game.d["y"] = 1
    elif game.d["y"] >= HEIGHT:
        game.d["y"] = HEIGHT - 1

    # Change the sprite 0 to the next sprite in the stack
    game.d["sprite_frame"] = game.d["ani_frame"]
    if game.d["x_incr"] < 0:
        game.d["sprite_frame"] += 4
    tulip.sprite_register(0, (game.rabbit_w*game.rabbit_h)*(game.d["sprite_frame"]), game.rabbit_w, game.rabbit_h)
    game.d["ani_frame"] = (game.d["ani_frame"] + 1) % game.d["ani_frames"]

    # fill background with noise pattern
    for i in range(100):
        g_x = int(random.random() * WIDTH)
        g_y = int(random.random() * HEIGHT)    
        tulip.bg_pixel(g_x,g_y,random.choice(game.grass_colors))

    #14
    amy.send(osc=0, wave=amy.PCM, patch=18,  vel=0.25, feedback=1)

def after_reset(x):
    amy.reset()
    tulip.bg_clear()

def quit_callback(screen):
    tulip.frame_callback()
    screen.game.quit()
    tulip.defer(after_reset, 0, 250)

def activate_callback(screen):
    # Register the frame callback and data
    tulip.defer(screen.game.draw_background, screen.app_dir, 500)

def deactivate_callback(screen):
    tulip.frame_callback()

def run(screen):
    screen.game = BunnyBounce()
    screen.activate_callback = activate_callback
    screen.quit_callback = quit_callback
    screen.deactivate_callback = deactivate_callback
    screen.present()



