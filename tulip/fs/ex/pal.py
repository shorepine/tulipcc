# pal.py
# show all 255 RGB332 colors with pal_idx #s
import tulip
tulip.gpu_reset()
tulip.bg_clear(0)
color = 0
for row in range(13):
    for col in range(20):
        if(color < 256):
            x = col*50
            y = 25 + (row*44)
            tulip.bg_str(str(color), x+18, y, 255)
            tulip.bg_rect(x+20, y+4, 25, 25, color, 1)
            color += 1
