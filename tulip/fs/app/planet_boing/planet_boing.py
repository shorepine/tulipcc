import time, random, math, tulip, alles

(WIDTH, HEIGHT) = tulip.screen_size()

class Blob(tulip.Sprite):
    DEBOUNCE = 20 # frames
    DEWORM = 15
    BLOB_SPRITE_START = -1
    BLOBS_EXTANT = 0

    def __init__(self, diameter = 32, copy_of=None):
        super().__init__(copy_of=copy_of)
        name = "copyblob"

        if  Blob.BLOBS_EXTANT < PlanetBoing.MAX_BLOB_MODELS:
            name = "pix/blob_texture_0" + str(Blob.BLOBS_EXTANT) + ".png"
        
        if Blob.BLOB_SPRITE_START < 0:
            Blob.BLOB_SPRITE_START = self.sprite_id

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
            print("BLOBS_EXTANT:", Blob.BLOBS_EXTANT)

   
        self.follow()

        Blob.BLOBS_EXTANT += 1

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
            alles.send(osc=32+self.sprite_id, wave=alles.PCM, patch=13, note=48+self.sprite_id, vel=0.75, pan=p)
            self.last_bounce = frame
            return True
        else:
            return False

    def wormholize(self, frame, wormhole):
        if(frame - self.last_wormhole > Blob.DEWORM):
            self.x = wormhole.x
            self.y = wormhole.y
            p = self.x / tulip.Sprite.SCREEN_WIDTH
            alles.send(osc=32+self.sprite_id, wave=alles.PCM, patch=18, note=48+self.sprite_id, vel=0.75, pan=p)
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
    def __init__(self, speed=0):
        super().__init__()
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

    def __init__(self):
        super().__init__()
        self.load("pix/bang_texture_03.png", 32, 32)

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
    def __init__(self):
        super().__init__()
        self.width = 39
        self.height = 39

        self.draw_flag()

    def draw_flag(self):
        tulip.bg_rect(WIDTH+600, 0, self.width+1,self.width+1, 0x55, 1)
        r = int(self.width/2)
        tulip.bg_circle(WIDTH+600 + int(self.width/2), int(self.width/2), r, 235, 1)
        tulip.bg_circle(WIDTH+600 + int(self.width/2), int(self.width/2), r-4, 0x55, 1)
        self.width += 1
        self.height += 1
        bitmap = tulip.bg_bitmap(WIDTH+600,0, self.width, self.height)
        tulip.sprite_bitmap(bitmap, tulip.Sprite.mem_pointer)
        tulip.sprite_register(self.sprite_id,tulip.Sprite.mem_pointer, self.width, self.height)
        tulip.Sprite.mem_pointer += self.width * self.height

class Thrust(tulip.Sprite):
    
    def __init__(self):
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
        tulip.bg_rect(WIDTH+500, 0, 32,32, 0x55, 1)
        tulip.bg_line(WIDTH+500,16, WIDTH+532, 0, 0)
        tulip.bg_line(WIDTH+500,16, WIDTH+532, 16, 0)
        tulip.bg_line(WIDTH+500,16, WIDTH+532, 32, 0)

        bitmap = tulip.bg_bitmap(WIDTH+500,0, 32,32)
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
    def __init__(self):
        super().__init__()
        self.load("pix/wormhole.png", 32, 32)


class PlanetBoing(tulip.Game):
    MAX_BLOB_MODELS = 10
    MAX_BLOBS = 20
    BLOBS_ACTIVE = 0
        
    def __init__(self, num_blobs = 4):
        # If you want TFB etc to stay, use debug=True here 
        super().__init__(debug=False)

        self.next_blob_at = 10
        
        alles.reverb(0.8)
        self.draw_background()

        self.score = Score()
        self.score.draw()        

        self.num_blobs = num_blobs


        self.wormhole1 = WormHole() # sprite 1 & 2 for wormholes
        self.wormhole1.moveto(random.randrange(int(tulip.Sprite.SCREEN_WIDTH * 0.2),
                                               int(tulip.Sprite.SCREEN_WIDTH * 0.35)),
                              random.randrange(int(tulip.Sprite.SCREEN_HEIGHT * 0.1),
                                               int(tulip.Sprite.SCREEN_HEIGHT * 0.8)))
        self.wormhole1.on()        
        self.wormhole2 = WormHole()
        self.wormhole2.moveto(random.randrange(int(tulip.Sprite.SCREEN_WIDTH * 0.65),
                                               int(tulip.Sprite.SCREEN_WIDTH * 0.9)),
                              random.randrange(int(tulip.Sprite.SCREEN_HEIGHT * 0.2),
                                               int(tulip.Sprite.SCREEN_HEIGHT * 0.8)))
        self.wormhole2.on()
        
        self.thrust = Thrust()
        self.thrust.x = 500
        self.thrust.y = 250
        #self.thrust.move()
        #self.thrust.on()

        self.flag = Flag()

        self.player = BlobPlayer() # sprite 4 for player
        self.flag.x = self.player.X
        self.flag.y = self.player.y
        self.flag.move()
        self.flag.on()

        self.blobs = [self.player]
        #for i in range(self.num_blobs):
        for i in range(PlanetBoing.MAX_BLOBS):
            self.new_blob()

        for i in range(self.num_blobs):
            self.next_blob_on()

        self.bang = Bang() # bang on top?
            
        self.frame_counter = 0            
        
        # Register the frame callback and data
        tulip.frame_callback(game_loop, self)



    def draw_background(self):
        tulip.bg_clear(167)
        a = 10
        a_span = (a * 2) + 2
        pi = 3.14159
        ph_incr = pi/10
        ph = 0.0
        for x in range(tulip.Sprite.SCREEN_WIDTH*2):
            y = int(math.sin(ph) * a) + a
            ph += ph_incr
            tulip.bg_pixel(x,y+tulip.Sprite.SCREEN_HEIGHT,15)

        for y in range(1,tulip.Sprite.SCREEN_HEIGHT,a_span):
            x_start = random.randrange(tulip.Sprite.SCREEN_WIDTH)
            tulip.bg_blit(x_start,tulip.Sprite.SCREEN_HEIGHT,tulip.Sprite.SCREEN_WIDTH,a_span,0,y)

    def new_blob(self):
        b = None
        if len(self.blobs) < PlanetBoing.MAX_BLOB_MODELS:
            b = Blob()
        else:
            if len(self.blobs) < PlanetBoing.MAX_BLOBS:
                blob_copy_num = len(self.blobs) % PlanetBoing.MAX_BLOB_MODELS
                b = Blob(copy_of = self.blobs[blob_copy_num])

        if b != None:
            self.blobs.append(b)
        return b

    def next_blob_on(self):
        #print("nbo:", PlanetBoing.BLOBS_ACTIVE, PlanetBoing.MAX_BLOBS)
        if PlanetBoing.BLOBS_ACTIVE < PlanetBoing.MAX_BLOBS:
            self.blobs[PlanetBoing.BLOBS_ACTIVE].on()
            PlanetBoing.BLOBS_ACTIVE += 1
            return True
        else:
            return False

class Score():
    def __init__(self):
        self.score = 0

    def draw(self):
        tulip.bg_roundrect(10,10,200,50,2,103,1)
        w = tulip.bg_str(str(self.score),tulip.Sprite.SCREEN_WIDTH, 100,
                      227,18)
        tulip.bg_str(str(self.score), 190 - w,50,227,18)

    def increment(self):
        self.score += 1
        self.draw()


# This is called every frame by the GPU.
def game_loop(g):
    g.frame_counter += 1
    # Turn off the bang
    if Bang.bang_frames >= 0:
        Bang.bang_frames += 1
    if Bang.bang_frames > Bang.bang_timeout:
        g.bang.off()
        Bang.bang_frames = -1
    g.thrust.off()
    # Move the player blob according to joy input
    move_dir = g.player.joy_move()
    if move_dir > 0:
        #print("moved: ", move_dir)
        g.thrust.thrust(move_dir, g.player)

    # Check for collisions...
    collisions = tulip.collisions()
    culled_collisions = [c for c in collisions if 
                         g.bang.sprite_id not in c and 
                         g.thrust.sprite_id not in c and 
                         g.flag.sprite_id not in c]

    for (a,b) in culled_collisions:

            # check for wormhole trips
            # this should be a list of connected wormholes...
        if(a == g.wormhole1.sprite_id):
            g.blobs[b-Blob.BLOB_SPRITE_START].wormholize(g.frame_counter,g.wormhole2)
        elif(a == g.wormhole2.sprite_id):
            g.blobs[b-Blob.BLOB_SPRITE_START].wormholize(g.frame_counter,g.wormhole1)
        
        else:
            # a and b are sprite numbers, but we want to access the blob array, 
            # which starts at zero. so we'll offset sprite numbers.
            a -= Blob.BLOB_SPRITE_START
            b -= Blob.BLOB_SPRITE_START

            g.bang.bang(g.blobs[a], g.blobs[b], g.frame_counter)

            if g.blobs[a].has_flag:
                g.blobs[a].update_flag(False)
                g.blobs[b].update_flag(True)
            elif g.blobs[b].has_flag:
                g.blobs[b].update_flag(False)
                g.blobs[a].update_flag(True)
            
            g.score.increment()

    # Update all the blob positions
    for blob in g.blobs:
        blob.follow() 
        if blob.has_flag:
            g.flag.on()
            g.flag.x = blob.x - 3
            g.flag.y = blob.y - 3
            g.flag.move()

    if g.score.score >= g.next_blob_at:
        if g.next_blob_on():
            g.next_blob_at += 10

    #g.player.move()
    #g.flag.x = g.player.x - 3
    #g.flag.y = g.player.y - 3
    #g.flag.move()

game = PlanetBoing()

# Run in a loop forever. Catch ctrl-c 
try:
    while game.run:
        # In an infinite loop , it's better to sleep than to say "pass", give the Tulip some time to breathe
        time.sleep_ms(100)
except KeyboardInterrupt:
    game.quit()















