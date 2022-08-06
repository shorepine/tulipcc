# A simple game demo that shows off BG and sprites and the GPU callback
import time

tulip.gpu_reset() # clear everything

# Load the background image
tulip.bg_png("g/mountain-bg.png",0,200)
# Copy the background over a bunch to make it repeat, but overlap to avoid double moons
for i in range(10):
    tulip.bg_blit(0,200,272,160,272+(i*100),200)

# Sample a pixel from the BG and make the top of the screen that color
c = tulip.bg_pixel(0,200)
tulip.rect(0,0,1024,200,c,1)

# now load the mountain pics over the background
# let's load the data to a var first as we'll load it three times
# why not blit? because it can't copy the alpha that way, we composite on placement for BG
mountain = open('g/mountain.png','rb').read()
tulip.bg_png(mountain,0,250)
tulip.bg_png(mountain,544,250)
tulip.bg_png(mountain,1088,250)
mountain = None # clear the ram

# And put black under the mountains
tulip.rect(0,250+160,1024,190,0,1)

# and some tiles
tulip.bg_png('g/meadow.png',0,410)
tulip.bg_png('g/meadow.png',0,442)
tulip.bg_png('g/meadow.png',0,474)
tulip.bg_png('g/meadow.png',0,506)
tulip.bg_png('g/brick.png',0,538)
tulip.bg_png('g/water.png',0,572)

for i in range(1280/32):
    tulip.bg_blit(0,410,32,190,i*32,410)

# put some empty spots along the brick
for i in [3, 8, 10, 14, 18, 25, 32, 33, 38]:
    tulip.bg_blit(0,442,32,32,32*i, 538)

# Now scroll the moon and the mountains at separate speeds
for i in range(100):
    tulip.bg_scroll_x_speed(i+200, 2)
for i in range(110):
    tulip.bg_scroll_x_speed(i+300, 5)


# Load the rabbit sprite frames into sprite RAM
(rabbit_w, rabbit_h) = (48, 32)
for i in range(4):
    tulip.sprite_png("g/rabbit_r_%d.png" % (i),(rabbit_w*rabbit_h)*i)
for i in range(4):
    tulip.sprite_png("g/rabbit_l_%d.png" % (i),(rabbit_w*rabbit_h)*(i+4))

# Register the first frame, we'll swap out frames during animation
tulip.sprite_register(0, 0, rabbit_w, rabbit_h)
tulip.sprite_on(0)

# Now run the game loop. First setup some variables for the game state. rabbit x and y, frame counter etc
d = {"dir":0, "f":0, "rx":50, "ry":510, "jump":0, "run":1} 

rabbit_speed = 10

# This is called every frame by the GPU.
def game_loop(d):
    # Increment the frame counter
    d["f"] = d["f"] + 1
    # Move the rabbit if direction is set
    d0, d1 = tulip.keys()[1:3]
    if(d0==0x4f or d1==0x4f):
        # Update the frame animation if we're moving (right facing rabbit)
        tulip.sprite_register(0,(rabbit_w*rabbit_h)*(d["f"]%4), rabbit_w, rabbit_h)
        # If we're beyond the middle of the screen, scroll the bricks instead
        if(d["rx"] > 512):
            for i in range(64): # lower 2 tile rows
                tulip.bg_scroll_x_speed(536+i, rabbit_speed)
        else:
            d["rx"] += rabbit_speed
    elif(d0==0x50 or d1==0x50):
        d["rx"] -= rabbit_speed
        tulip.sprite_register(0,(rabbit_w*rabbit_h)*((d["f"]%4)+4), rabbit_w, rabbit_h)
    else:
        # not moving L or R
        for i in range(64):
            tulip.bg_scroll_x_speed(536+i, 0) # stop scrolling
    if(d0==0x2c or d1==0x2c):
        if(d["jump"]==0):
            d["jump"] = d["f"]
    else:
        d["jump"] = 0

    # calculate position from jump start frame time
    if(d["jump"]>0):
        d["ry"] = 510-(d["f"]-d["jump"])*rabbit_speed
    else:
        d["ry"] = 510
    if(d0==0x29): # esc
        d["run"] = 0
    # Update the sprite position
    tulip.sprite_move(0,d["rx"], d["ry"])

# Register the frame callback and data
tulip.frame_callback(game_loop, d)

tulip.key_scan(1) # enter direct scan mode, keys will not hit the REPL this way

# Run in a loop forever. Catch ctrl-c 
try:
    while d["run"]:
        # In an infinite loop , it's better to sleep than to say "pass", give the Tulip some time to breathe
        time.sleep_ms(100)
except KeyboardInterrupt:
    d["run"] = 0

# Clean up a bit
tulip.key_scan(0)
tulip.frame_callback()
tulip.gpu_reset()
tulip.display_restart()












