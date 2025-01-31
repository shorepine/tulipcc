import tulip, random

every = 2500
last = tulip.ticks_ms()-every
play = True

def r(u):
    return random.randint(0,u)
    
def draw():
    tulip.bg_clear(36)
    x = r(1024-128)
    y = r(600-128)
    tulip.bg_png(tulip.root_dir()+"sys/ex/g/tulipbw.png", x,y)
    c0, c1, c2 = r(255)+1, r(255)+1, r(255)+1
    if(c0 == 36): c0 = 35
    if(c1 == 36): c1 = 35
    if(c2 == 36): c2 = 35
    tulip.bg_fill(x+50, y+60, c0)
    tulip.bg_fill(x+30, y+60, c1)
    tulip.bg_fill(x+95, y+60, c2)
    

def cb(x):
    global last, every
    if(tulip.ticks_ms()-last > every):
        draw()
        last = tulip.ticks_ms()

def touchcb(up):
    global app
    if(not up):
        app.quit()
        
def keycb(key):
    global app
    app.quit()

class Screensaver(tulip.UIScreenGame):
    def __init__(self):
        super().__init__()

    def draw_background(self, extra=None):
        tulip.frame_callback(cb)
        tulip.touch_callback(touchcb)
        tulip.keyboard_callback(keycb)
    
    def deactivate(self):
        tulip.frame_callback()
        tulip.keyboard_callback()
        tulip.touch_callback()
        tulip.bg_clear()

    



def quit_callback(screen):
    screen.game.quit()

def activate_callback(screen):
    # Register the frame callback and data
    tulip.defer(screen.game.draw_background, screen.app_dir, 500)

def deactivate_callback(screen):
    screen.game.deactivate()

def run(screen):
    global app
    app = screen
    screen.game = Screensaver()
    screen.activate_callback = activate_callback
    screen.quit_callback = quit_callback
    screen.deactivate_callback = deactivate_callback
    screen.present()














