# A simple game demo that shows off BG and sprites and the GPU callback
import tulip

(sw,sh) = tulip.screen_size()




def draw_background(app):
    pix_dir = app.app_dir+"/g/"
    tulip.bg_png(pix_dir+"mountain-bg.png",0,0)
    # Copy the background over a bunch to make it repeat, but overlap to avoid double moons
    for i in range(20):
        tulip.bg_blit(0,0,272,160,272+(i*100),0)

    # now load the mountain pics over the background
    # let's load the data to a var first as we'll load it three times
    # why not blit? because it can't copy the alpha that way, we composite on placement for BG
    mountain = open(pix_dir+'mountain.png','rb').read()
    for i in range(5):
        tulip.bg_png(mountain,544*i,50)
    mountain = None
    # And put black under the mountains
    tulip.bg_rect(0,50+160,sw,190,0,1)

    # and some tiles
    tulip.bg_png(pix_dir+'meadow.png',0,280)
    # copy it three times underneath
    tulip.bg_blit(0,300,32,32,0,312)
    tulip.bg_blit(0,300,32,32,0,344)
    tulip.bg_blit(0,300,32,32,0,376)
    tulip.bg_png(pix_dir+'brick.png',0,408)
    tulip.bg_png(pix_dir+'water.png',0,440)

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
    (app.rabbit_w, app.rabbit_h) = (48, 32)
    for i in range(4):
        tulip.sprite_png(pix_dir+"rabbit_r_%d.png" % (i),(app.rabbit_w*app.rabbit_h)*i)
    for i in range(4):
        tulip.sprite_png(pix_dir+"rabbit_l_%d.png" % (i),(app.rabbit_w*app.rabbit_h)*(i+4))

    # Register the first frame, we'll swap out frames during animation
    tulip.sprite_register(0, 0, app.rabbit_w, app.rabbit_h)
    tulip.sprite_on(0)

    # Now run the game loop. First setup some variables for the game state. rabbit x and y, frame counter etc
    app.d = {"dir":0, "f":0, "rx":50, "ry":380, "jump":0, "run":1, "scroll":0} 
    app.rabbit_speed = 10


# This is called every frame by the GPU.
def game_loop(app):
    # This is a bit of a hack for web; the frame scheduler can happen after we quit/alttab away from this app, so we check
    if(not app.active): return

    # Increment the frame counter
    app.d["f"] = app.d["f"] + 1
    # Move the rabbit if direction is set
    if(tulip.joyk() & tulip.Joy.RIGHT):
        # Update the frame animation if we're moving (right facing rabbit)
        tulip.sprite_register(0,(app.rabbit_w*app.rabbit_h)*(app.d["f"]%4), app.rabbit_w, app.rabbit_h)
        # If we're beyond the middle of the screen, scroll the bricks instead
        if(app.d["rx"] > (sw/2)):
            if(app.d["scroll"]==0):
                app.d["scroll"] = 1
                for i in range(64): # lower 2 tile rows
                    tulip.bg_scroll_x_speed(408+i, app.rabbit_speed)
        else:
            app.d["rx"] += app.rabbit_speed
    else:
        if(app.d["scroll"] == 1):
            app.d["scroll"] = 0
            for i in range(64):
                tulip.bg_scroll_x_speed(408+i, 0) # stop scrolling

    if(tulip.joyk() & tulip.Joy.LEFT):
        app.d["rx"] -= app.rabbit_speed
        tulip.sprite_register(0,(app.rabbit_w*app.rabbit_h)*((app.d["f"]%4)+4), app.rabbit_w, app.rabbit_h)

    if(tulip.joyk() & tulip.Joy.B):
        if(app.d["jump"]==0):
            app.d["jump"] = app.d["f"]
    else:
        app.d["jump"] = 0

    # calculate position from jump start frame time
    if(app.d["jump"]>0):
        app.d["ry"] = 380-(app.d["f"]-app.d["jump"])*app.rabbit_speed
    else:
        app.d["ry"] = 380
    if(tulip.keys()[1]==0x29): # esc
        app.d["run"] = 0
    # Update the sprite position
    if(app.d["rx"] < 0):
        app.d["rx"] = 0
    tulip.sprite_move(0,app.d["rx"], app.d["ry"])


def activate_callback(app):
    draw_background(app)
    # Register the frame callback and data
    tulip.frame_callback(game_loop, app)


def deactivate_callback(app):
    tulip.bg_scroll()

def run(app):
    app.game = True
    app.activate_callback = activate_callback
    app.deactivate_callback = deactivate_callback
    app.present()





