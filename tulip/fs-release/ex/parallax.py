# A simple game demo that shows off BG and sprites and the GPU callback
import time, tulip

tulip.gpu_reset() # clear everything
(sw,sh) = tulip.screen_size()
tulip.display_stop()
# Load the background image
tulip.bg_png("g/mountain-bg.png",0,0)
# Copy the background over a bunch to make it repeat, but overlap to avoid double moons
for i in range(20):
    tulip.bg_blit(0,0,272,160,272+(i*100),0)

# Sample a pixel from the BG and make the top of the screen that color
#c = tulip.bg_pixel(0,0)
#tulip.bg_rect(0,0,1024,200,c,1)

# now load the mountain pics over the background
# let's load the data to a var first as we'll load it three times
# why not blit? because it can't copy the alpha that way, we composite on placement for BG
mountain = open('g/mountain.png','rb').read()
for i in range(5):
    tulip.bg_png(mountain,544*i,50)
mountain = None
# And put black under the mountains
tulip.bg_rect(0,50+160,sw,190,0,1)

# and some tiles
tulip.bg_png('g/meadow.png',0,280)
# copy it three times underneath
tulip.bg_blit(0,300,32,32,0,312)
tulip.bg_blit(0,300,32,32,0,344)
tulip.bg_blit(0,300,32,32,0,376)
tulip.bg_png('g/brick.png',0,408)
tulip.bg_png('g/water.png',0,440)

# Copy this column across the screen
for i in range((sw*2)/32):
    tulip.bg_blit(0,280,32,190,i*32,280)

# put some empty spots along the brick
for i in [3, 8, 10, 14, 18, 25, 32, 33, 38]:
    tulip.bg_blit(0,312,32,32,32*i, 408)

# Now scroll the moon and the mountains at separate speeds
for i in range(100):
    tulip.bg_scroll_x_speed(i, 2)
for i in range(110):
    tulip.bg_scroll_x_speed(i+100, 5)


# Load the rabbit sprite frames into sprite RAM
(rabbit_w, rabbit_h) = (48, 32)
for i in range(4):
    tulip.sprite_png("g/rabbit_r_%d.png" % (i),(rabbit_w*rabbit_h)*i)
for i in range(4):
    tulip.sprite_png("g/rabbit_l_%d.png" % (i),(rabbit_w*rabbit_h)*(i+4))

tulip.display_start()
# Register the first frame, we'll swap out frames during animation
tulip.sprite_register(0, 0, rabbit_w, rabbit_h)
tulip.sprite_on(0)

# Now run the game loop. First setup some variables for the game state. rabbit x and y, frame counter etc
d = {"dir":0, "f":0, "rx":50, "ry":380, "jump":0, "run":1, "scroll":0} 

rabbit_speed = 10

# This is called every frame by the GPU.
def game_loop(d):
    # Increment the frame counter
    d["f"] = d["f"] + 1
    # Move the rabbit if direction is set
    if(tulip.joyk() & tulip.Joy.RIGHT):
        # Update the frame animation if we're moving (right facing rabbit)
        tulip.sprite_register(0,(rabbit_w*rabbit_h)*(d["f"]%4), rabbit_w, rabbit_h)
        # If we're beyond the middle of the screen, scroll the bricks instead
        if(d["rx"] > (sw/2)):
            if(d["scroll"]==0):
                d["scroll"] = 1
                for i in range(64): # lower 2 tile rows
                    tulip.bg_scroll_x_speed(408+i, rabbit_speed)
        else:
            d["rx"] += rabbit_speed
    else:
        if(d["scroll"] == 1):
            d["scroll"] = 0
            for i in range(64):
                tulip.bg_scroll_x_speed(408+i, 0) # stop scrolling

    if(tulip.joyk() & tulip.Joy.LEFT):
        d["rx"] -= rabbit_speed
        tulip.sprite_register(0,(rabbit_w*rabbit_h)*((d["f"]%4)+4), rabbit_w, rabbit_h)

    if(tulip.joyk() & tulip.Joy.B):
        if(d["jump"]==0):
            d["jump"] = d["f"]
    else:
        d["jump"] = 0

    # calculate position from jump start frame time
    if(d["jump"]>0):
        d["ry"] = 380-(d["f"]-d["jump"])*rabbit_speed
    else:
        d["ry"] = 380
    if(tulip.keys()[1]==0x29): # esc
        d["run"] = 0
    # Update the sprite position
    if(d["rx"] < 0):
        d["rx"] = 0
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












