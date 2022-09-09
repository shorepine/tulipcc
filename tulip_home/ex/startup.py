import time, music, random
tulip.bg_clear(255)
tulip.bg_png("g/tulip3.png",384,128)

chord = music.Chord("F:min7")
for j in range(1):
    patch = 4
    ratio = 0.1
    tic = alles.millis()
    
    for i,note in enumerate(chord.midinotes()[:3]):
        # note on
        alles.send(wave=alles.ALGO,osc=i*9,note=note,vel=0.25,patch=patch,ratio=ratio,timestamp=tic+(i*200))
        # note off
        alles.send(osc=i*9, vel=0, timestamp=tic+1500)
    #print(str(patch))
    #time.sleep(3.5)
    #alles.reset()    

ct = 150
for c in range(ct):
    swell = ct - 1 - c
    for y in range(600):
        tulip.bg_scroll_x_offset(y, random.randint(-swell,swell))
    time.sleep(0.01)

time.sleep(0.5)
tulip.gpu_reset()
 
