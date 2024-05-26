import sys
sys.path.append('.')
from st77xx import *
from xpt2046 import *

spi=machine.SPI(
    1,
    baudrate=24_000_000,
    polarity=0,
    phase=0,
    sck=machine.Pin(10,machine.Pin.OUT),
    mosi=machine.Pin(11,machine.Pin.OUT),
    miso=machine.Pin(12,machine.Pin.IN)
)

if 0:
    # with DMA, the repaints seem to be too slow? To be investigated
    # we seem to be fine performance-wise without DMA with 320x240 anyway
    import rp2_dma
    rp2_dma=rp2_dma.DMA(0)
else: rp2_dma=None

import lvgl as lv
lv.init()

lcd=St7789(rot=3,res=(240,320),spi=spi,cs=9,dc=8,bl=13,rst=15,rp2_dma=rp2_dma,factor=8)
lcd.set_backlight(30)
touch=Xpt2046(spi=spi,cs=16,rot=1,spiPrereadCb=lcd.rp2_wait_dma)

scr=lv.obj()
btn=lv.button(scr)
lbl=lv.label(btn)
lbl.set_text("Press me!")
btn.center()
btn.add_event(lambda event: print('Button clicked!'),lv.EVENT.CLICKED,None)
lv.screen_load(scr)
