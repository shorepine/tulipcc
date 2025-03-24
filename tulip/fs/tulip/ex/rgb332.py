# draw the rgb332 palette nicely
import tulip
w = 32
h = 75
tulip.bg_clear()
for b in [0, 1, 2, 3]:
    for rg in range(64):
        c = (rg << 2) | b
        x = ((b*8) + (rg % 8)) * w
        y = int(rg / 8) * h
        tulip.bg_rect(x,y,w,h,c,1)
        tc = 255-c
        tulip.bg_str("%03d" % (c), x+7, y+41, tc, 8)
                
