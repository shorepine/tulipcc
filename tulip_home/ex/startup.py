import time, music, random
tulip.bg_clear()
lx = 310
ly = 75
tulip.bg_png("g/tulipbw.png",lx,ly)
chord = music.Chord("F:min7")
patch = 4
ratio = 0.1
sound_start_ms = alles.millis()

for i,note in enumerate(chord.midinotes()):
    alles.send(wave=alles.ALGO,osc=i*9,note=note,vel=0.25,patch=patch,ratio=ratio,timestamp=sound_start_ms+(i*200))
    alles.send(osc=i*9, vel=0, timestamp=sound_start_ms+1500)

def anim_loop(data):
    if(not data["run"]):
        data["run"] = True
        c = random.randint(1,255)
        data["which"] = (data["which"] + 1) % 3
        if(data['which'] == 0): tulip.bg_fill(lx+75,ly+200, c)
        if(data['which'] == 1): tulip.bg_fill(lx+175,ly+200,c)
        if(data['which'] == 2): tulip.bg_fill(lx+320,ly+200,c)
        data["run"] = False
    data["tic"] += 1

anim_data = {"tic": 0, "run": False, "which":0}
tulip.frame_callback(anim_loop, anim_data) # starts calling the callback every frame

go = True
while go:
    if(anim_data["tic"] / tulip.fps() > 2):
        go = False
        tulip.frame_callback()

tulip.gpu_reset()
 
