import time, music, random
tulip.bg_clear()
lx = 310
ly = 75
tulip.bg_png("tulipbw.png",lx,ly)

chord = music.Chord("F:min7")
patch = 4
ratio = 0.1
tic = alles.millis()
    
for i,note in enumerate(chord.midinotes()):
    alles.send(wave=alles.ALGO,osc=i*9,note=note,vel=0.25,patch=patch,ratio=ratio,timestamp=tic+(i*200))
    alles.send(osc=i*9, vel=0, timestamp=tic+1500)

for c in range(200):
    c = [random.randint(1,255),random.randint(1,255),random.randint(1,255)]
    tulip.fill(lx+75,ly+200, c[0])
    tulip.fill(lx+175,ly+200,c[1])
    tulip.fill(lx+320,ly+200,c[2])
    time.sleep(0.01)

time.sleep(0.5)
tulip.gpu_reset()
 
