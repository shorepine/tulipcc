# A demo of tile drawing and sprites
import random, time

# load and draw tiles offscreen into bitmaps in BG space
tulip.bg_png("g/flowers.png",1024,0) # offscreen
tulip.bg_png("g/meadow.png",1024,32) # offscreen

# Load the rabbit sprite frames into sprite RAM
(rabbit_w, rabbit_h) = (48, 32)
for i in range(4):
    tulip.sprite_png("g/rabbit_r_%d.png" % (i),(rabbit_w*rabbit_h)*i)
for i in range(4):
    tulip.sprite_png("g/rabbit_l_%d.png" % (i),(rabbit_w*rabbit_h)*(i+4))

# Register the first frame, we'll swap out frames during animation
tulip.sprite_register(0, 0, rabbit_w, rabbit_h)
tulip.sprite_on(0)


# Do the background tiles
for y in range(600/32):
    for x in range(1024/32):
        which = random.choice([0, 1])
        if(which==0):
            tulip.bg_blit(1024,0,32,32,(x*32), (y*32))
        else:
            tulip.bg_blit(1024,32,32,32,(x*32), (y*32))

# And draw the rabbit going right then left again, over and over
while 1:
    for i in range(100):
        tulip.sprite_move(0, 300+(i*4), 300)
        time.sleep_ms(50)
        # Change the sprite 0 to the next sprite in the stack
        tulip.sprite_register(0, (rabbit_w*rabbit_h)*(i%4), rabbit_w, rabbit_h)
    
    for i in range(100):
        tulip.sprite_move(0, 700-(i*4), 300)
        time.sleep_ms(50)
        tulip.sprite_register(0, (rabbit_w*rabbit_h)*(4 + (i%4)), rabbit_w, rabbit_h)


