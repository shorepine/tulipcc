import random, math, tulip, amy

(WIDTH, HEIGHT) = tulip.screen_size()
MAX_BLOB_MODELS = 10
MAX_BLOBS = 20

class Blob(tulip.Sprite):
    DEBOUNCE = 20 # frames
    DEWORM = 15

    def __init__(self, app, diameter = 32, copy_of=None):
        super().__init__(copy_of=copy_of)
        name = "copyblob"

        if  app.BLOBS_EXTANT < MAX_BLOB_MODELS:
            name = app.app_dir+"/pix/blob_texture_0" + str(app.BLOBS_EXTANT) + ".png"
        
        if app.BLOB_SPRITE_START < 0:
            app.BLOB_SPRITE_START = self.sprite_id

        self.x = random.randrange(int(tulip.Sprite.SCREEN_WIDTH/2)) + int(tulip.Sprite.SCREEN_WIDTH/4)
        self.y = random.randrange(int(tulip.Sprite.SCREEN_HEIGHT/2)) + int(tulip.Sprite.SCREEN_HEIGHT/4)
        self.x_v = random.randrange(-10,10)
        self.y_v = random.randrange(-10,10)
        self.x_a = (random.random() - 0.5) * 0.2
        self.y_a = (random.random() - 0.5) * 0.2
        self.diameter = diameter
        self.last_bounce = 0
        self.last_wormhole = 0
        self.has_flag = False

        try:
            # Name will be ignored if copy_of is set, so don't worry about not having the file
            self.load(name, 32, 32)
        except:
            print("hmm, didn't like ", name)
            print("BLOBS_EXTANT:", app.BLOBS_EXTANT)

   
        self.follow()

        app.BLOBS_EXTANT += 1

    def update_flag(self, has_flag):
        self.has_flag = has_flag

    def follow(self):
        # Move along the existing path (if not a player character)
        # Bounce the blob off of walls
        self.x_v += self.x_a
        self.y_v += self.y_a

        if self.x_v > 10:
            self.x_v = 10
            self.x_a *= -1.0
        elif self.x_v < -10:
            self.x_v = -10
            self.x_a *= -1.0
        if self.y_v > 10:
            self.y_v = 10
            self.y_a *= -1
        elif self.y_v < -10:
            self.y_v = -10
            self.y_a *= -1

        self.x += self.x_v
        self.y += self.y_v
        if(self.x >= tulip.Sprite.SCREEN_WIDTH-self.diameter):
            self.x_v *= -1
            self.x = tulip.Sprite.SCREEN_WIDTH - (self.diameter + 1)
        elif(self.x <= 0):
            self.x_v *= -1
            self.x = 1
        if(self.y >= tulip.Sprite.SCREEN_HEIGHT - self.diameter):
            self.y_v *= -1
            self.y = tulip.Sprite.SCREEN_HEIGHT - (self.diameter + 1)
        elif(self.y <= 0):
            self.y_v *= -1
            self.y = 1
        self.clamp() # Make sure we're on screen
        self.move()

    def bounce(self, frame):
        if(frame - self.last_bounce > Blob.DEBOUNCE):
            self.x_v = random.randrange(-10, 10)
            self.y_v = random.randrange(-10, 10)
            p = self.x / tulip.Sprite.SCREEN_WIDTH
            #print("x:", self.x, "pan:", p)
            amy.send(osc=32+self.sprite_id, wave=amy.PCM, patch=13, note=48+self.sprite_id, vel=0.75, pan=p)
            self.last_bounce = frame
            return True
        else:
            return False

    def wormholize(self, frame, wormhole):
        if(frame - self.last_wormhole > Blob.DEWORM):
            self.x = wormhole.x
            self.y = wormhole.y
            p = self.x / tulip.Sprite.SCREEN_WIDTH
            amy.send(osc=32+self.sprite_id, wave=amy.PCM, patch=18, note=48+self.sprite_id, vel=0.75, pan=p)
            self.last_wormhole = frame        



# A sprite who can move from the joystick/keyboard
class DooglePlayer(tulip.Sprite):
    X = 0
    R = 1
    L = 2
    RL = 3
    U = 4
    RU = 5
    LU = 6
    RLU = 7
    D = 8
    RD = 9
    LD = 10
    RLD = 11
    UD = 12
    RUD = 13
    LUD = 14
    RLUD = 15

    def __init__(self, speed=0):
        super().__init__()
        self.x_v = speed
        self.y_v = speed


    def joy_move(self):
        mask = tulip.joyk()

        moved = 0

        if (mask > 0):
            # Joystick moves twiddle velocity
            if(mask & tulip.Joy.RIGHT):
                self.x_v += 0.25
                moved += 1
            if(mask & tulip.Joy.LEFT):
                self.x_v -= 0.25
                moved += 2
            if(mask & tulip.Joy.UP):
                self.y_v -= 0.25
                moved += 4
            if(mask & tulip.Joy.DOWN):
                self.y_v += 0.25
                moved += 8
            
            if self.x_v > 10:
                self.x_v = 10
            elif self.x_v < -10:
                self.x_v = -10
            if self.y_v > 10:
                self.y_v = 10
            elif self.y_v < -10:
                self.y_v = -10
            self.clamp() # Make sure we're on screen

        return moved
        


class BlobPlayer(Blob, DooglePlayer):
    def __init__(self, app, speed=0):
        super().__init__(app)
        self.x_v = 0
        self.y_v = 0
        self.x_a = 0.0
        self.y_a = 0.0
        self.moveto(random.randrange(tulip.Sprite.SCREEN_WIDTH), random.randrange(tulip.Sprite.SCREEN_HEIGHT))

        self.has_flag = True
        
    #def move(self):
    #    super().move(self)

        


class Bang(tulip.Sprite):

    bang_bit = None
    bang_frames = -1
    bang_timeout = 5

    def __init__(self, app):
        super().__init__()
        self.load(app.app_dir+"/pix/bang_texture_03.png", 32, 32)

        Bang.bang_bit = tulip.sprite_bitmap(self.mem_pos, 32*32)

    def bang(self, a, b, frame_counter):
        
        # Bounce blobs
        if a.bounce(frame_counter) and b.bounce(frame_counter):
            a_x, a_y = a.x, a.y
            self.moveto(int((a_x + b.x)/2), int((a_y + b.y)/2))
            self.on()
            Bang.bang_frames = 0

#    def on(self):
#        super().on()
        #tulip.bg_bitmap(int(self.x), int(self.y), 32, 32, Bang.bang_bit)

class Flag(tulip.Sprite):
    def __init__(self, app):
        super().__init__()
        self.width = 39
        self.height = 39

        self.draw_flag()

    def draw_flag(self):
        tulip.bg_rect(WIDTH, 0, self.width+1,self.width+1, 0x55, 1)
        r = int(self.width/2)
        tulip.bg_circle(WIDTH + int(self.width/2), int(self.width/2), r, 235, 1)
        tulip.bg_circle(WIDTH + int(self.width/2), int(self.width/2), r-4, 0x55, 1)
        self.width += 1
        self.height += 1
        bitmap = tulip.bg_bitmap(WIDTH,0, self.width, self.height)
        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        tulip.sprite_register(self.sprite_id,tulip.Sprite.mem_pointer, self.width, self.height)
        tulip.Sprite.mem_pointer += self.width * self.height

class Thrust(tulip.Sprite):
    
    def __init__(self, app):
        super().__init__()
        self.width = 32
        self.height = 32
        #print("self.sprite_id:", self.sprite_id)
        #print("self.width, self.height", self.width, self.height)

        self.make_accel_wiggles()


    def thrust(self,direction,blob):
        if direction == DooglePlayer.R:
            tulip.sprite_register(self.sprite_id,self.mem_right, self.width, self.height)
            self.x = blob.x - 32
            self.y = blob.y
        elif direction == DooglePlayer.L:
            tulip.sprite_register(self.sprite_id,self.mem_left, self.width, self.height)
            self.x = blob.x + 32
            self.y = blob.y
        elif direction == DooglePlayer.U:
            tulip.sprite_register(self.sprite_id,self.mem_up, self.width, self.height)
            self.x = blob.x
            self.y = blob.y + 32
        elif direction == DooglePlayer.D:
            tulip.sprite_register(self.sprite_id,self.mem_down, self.width, self.height)
            self.x = blob.x
            self.y = blob.y - 32

        self.move()
        self.clamp() # Make sure we're on screen
        self.on()

    def make_accel_wiggles(self):
        w = 32
        # make left thrust
        tulip.bg_rect(WIDTH+0, 0, 32,32, 0x55, 1)
        tulip.bg_line(WIDTH+0,16, WIDTH+32, 0, 0)
        tulip.bg_line(WIDTH+0,16, WIDTH+32, 16, 0)
        tulip.bg_line(WIDTH+0,16, WIDTH+32, 32, 0)

        bitmap = tulip.bg_bitmap(WIDTH,0, 32,32)
        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        tulip.sprite_register(self.sprite_id,tulip.Sprite.mem_pointer, self.width, self.height)
        self.mem_left = tulip.Sprite.mem_pointer
        tulip.Sprite.mem_pointer += 32*32

        # make UP thrust

        bitmap_list = list(bitmap)
        cols = 32
        bitmap2d = [bitmap_list[i:i+cols] for i in range(0,len(bitmap_list), cols)]
        rotated = zip(*bitmap2d[::-1])
        bitmap = bytes([e for l in rotated for e in l])

        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        self.mem_up = tulip.Sprite.mem_pointer
        tulip.Sprite.mem_pointer += 32*32
        
        # make RIGHT thrust

        bitmap_list = list(bitmap)
        cols = 32
        bitmap2d = [bitmap_list[i:i+cols] for i in range(0,len(bitmap_list), cols)]
        rotated = zip(*bitmap2d[::-1])
        bitmap = bytes([e for l in rotated for e in l])

        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        self.mem_right = tulip.Sprite.mem_pointer
        tulip.Sprite.mem_pointer += 32*32

        # make DOWN thrust

        bitmap_list = list(bitmap)
        cols = 32
        bitmap2d = [bitmap_list[i:i+cols] for i in range(0,len(bitmap_list), cols)]
        rotated = zip(*bitmap2d[::-1])
        bitmap = bytes([e for l in rotated for e in l])

        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        self.mem_down = tulip.Sprite.mem_pointer
        tulip.Sprite.mem_pointer += 32*32

class WormHole(tulip.Sprite):
    def __init__(self, app):
        super().__init__()
        self.load(app.app_dir+"/pix/wormhole.png", 32, 32)


def setup(app):
    app.BLOBS_ACTIVE = 0
    app.BLOB_SPRITE_START = -1
    app.num_blobs = 4        
    app.BLOBS_EXTANT = 0
    app.next_blob_at = 10
    amy.reset()
    amy.reverb(0.8)

           
    app.wormhole1 = WormHole(app) # sprite 1 & 2 for wormholes
    app.wormhole1.moveto(random.randrange(int(tulip.Sprite.SCREEN_WIDTH * 0.2),
                                           int(tulip.Sprite.SCREEN_WIDTH * 0.35)),
                          random.randrange(int(tulip.Sprite.SCREEN_HEIGHT * 0.1),
                                           int(tulip.Sprite.SCREEN_HEIGHT * 0.8)))
    app.wormhole1.on()        
    app.wormhole2 = WormHole(app)
    app.wormhole2.moveto(random.randrange(int(tulip.Sprite.SCREEN_WIDTH * 0.65),
                                               int(tulip.Sprite.SCREEN_WIDTH * 0.9)),
                              random.randrange(int(tulip.Sprite.SCREEN_HEIGHT * 0.2),
                                               int(tulip.Sprite.SCREEN_HEIGHT * 0.8)))
    app.wormhole2.on()
        
    app.thrust = Thrust(app)
    app.thrust.x = 500
    app.thrust.y = 250
    app.flag = Flag(app)

    app.player = BlobPlayer(app) # sprite 4 for player
    app.flag.x = app.player.x
    app.flag.y = app.player.y
    app.flag.move()
    app.flag.on()

    app.blobs = [app.player]
    for i in range(MAX_BLOBS):
        new_blob(app)

    for i in range(app.num_blobs):
        next_blob_on(app)

    app.bang = Bang(app) # bang on top?    
    app.frame_counter = 0            
        

def draw_background(app, extra=None):
    tulip.bg_rect(0,0, WIDTH, HEIGHT,167, 1)
    a = 10
    a_span = (a * 2) + 2
    pi = 3.14159
    ph_incr = pi/10
    ph = 0.0
    for x in range(tulip.Sprite.SCREEN_WIDTH):
        y = int(math.sin(ph) * a) + a
        ph += ph_incr
        tulip.bg_pixel(x,y+tulip.Sprite.SCREEN_HEIGHT,15)
    app.score.draw()

def new_blob(app):
    b = None
    if len(app.blobs) < MAX_BLOB_MODELS:
        b = Blob(app)
    else:
        if len(app.blobs) < MAX_BLOBS:
            blob_copy_num = len(app.blobs) % MAX_BLOB_MODELS
            b = Blob(app,copy_of = app.blobs[blob_copy_num])
    if b != None:
        app.blobs.append(b)
    return b

def next_blob_on(app):
    if app.BLOBS_ACTIVE < MAX_BLOBS:
        app.blobs[app.BLOBS_ACTIVE].on()
        app.BLOBS_ACTIVE += 1
        return True
    else:
        return False

class Score():
    def __init__(self, app):
        self.score = 0

    def draw(self):
        tulip.bg_roundrect(10,50,200,50,2,103,1)
        w = tulip.bg_str(str(self.score),tulip.Sprite.SCREEN_WIDTH, 100,
                      227,18)
        tulip.bg_str(str(self.score), 190 - w,90,227,18)

    def increment(self):
        self.score += 1
        self.draw()


# This is called every frame by the GPU.
def game_loop(app):
    # This is a bit of a hack for web; the frame scheduler can happen after we quit/alttab away from this app, so we check
    if(not app.active): return

    app.frame_counter += 1
    # Turn off the bang
    if Bang.bang_frames >= 0:
        Bang.bang_frames += 1
    if Bang.bang_frames > Bang.bang_timeout:
        app.bang.off()
        Bang.bang_frames = -1
    app.thrust.off()
    # Move the player blob according to joy input
    move_dir = app.player.joy_move()
    if move_dir > 0:
        #print("moved: ", move_dir)
        app.thrust.thrust(move_dir, app.player)

    # Check for collisions...
    collisions = tulip.collisions()
    culled_collisions = [c for c in collisions if 
                         app.bang.sprite_id not in c and 
                         app.thrust.sprite_id not in c and 
                         app.flag.sprite_id not in c]

    for (a,b) in culled_collisions:

            # check for wormhole trips
            # this should be a list of connected wormholes...
        if(a == app.wormhole1.sprite_id):
            app.blobs[b-app.BLOB_SPRITE_START].wormholize(app.frame_counter,app.wormhole2)
        elif(a == app.wormhole2.sprite_id):
            app.blobs[b-app.BLOB_SPRITE_START].wormholize(app.frame_counter,app.wormhole1)
        
        else:
            # a and b are sprite numbers, but we want to access the blob array, 
            # which starts at zero. so we'll offset sprite numbers.
            a -= app.BLOB_SPRITE_START
            b -= app.BLOB_SPRITE_START

            app.bang.bang(app.blobs[a], app.blobs[b], app.frame_counter)

            if app.blobs[a].has_flag:
                app.blobs[a].update_flag(False)
                app.blobs[b].update_flag(True)
            elif app.blobs[b].has_flag:
                app.blobs[b].update_flag(False)
                app.blobs[a].update_flag(True)
            
            app.score.increment()

    # Update all the blob positions
    for blob in app.blobs:
        blob.follow() 
        if blob.has_flag:
            app.flag.on()
            app.flag.x = blob.x - 3
            app.flag.y = blob.y - 3
            app.flag.move()

    if app.score.score >= app.next_blob_at:
        if next_blob_on(app):
            app.next_blob_at += 10

def activate_callback(app):
    setup(app)
    draw_background(app)
    tulip.frame_callback(game_loop, app)

def deactivate_callback(app):
    amy.reset()

def run(app):
    app.game = True
    app.hide_task_bar = True
    app.score = Score(app)

    app.activate_callback = activate_callback
    app.deactivate_callback = deactivate_callback
    app.present()







