import machine
import sys
sys.path.append('.')
from st77xx import *

def build_rect_buf(w, h, inner=[0x00,0x00]):
    top = b"\xFF\xFF"*w
    body=(b"\xFF\xFF\xFF" + bytes(inner)*(w-3) + b"\xFF\xFF\xFF")*(h-3)
    bot = b"\xFF\xFF"*w
    return top + body + bot

def test_lcd(lcd):
    # lcd.hard_reset()
    lcd.set_backlight(30)
    for rot in (0,1,2,3):
        lcd.apply_rotation(rot)

        lcd.clear(0x0000)
        # 1/4 screen pixels square with white border red background
        w,h=lcd.width//4,lcd.height//8
        bmp=build_rect_buf(w,h,[0x03,0x03])
        t0=time.ticks_us()
        lcd.blit(w,h,w,h,bmp)
        t1=time.ticks_us()
        bmp=build_rect_buf(lcd.width,lcd.height//20,[0x09,0x09])
        lcd.blit(0,0,lcd.width,lcd.height//20,bmp)

        print("Maximum FPS @24MHz:",24e6/(320*240*16)) # FPS = F/(W*H*BPP)
        print("Achieved FPS:",1/(16*(t1-t0)*1e-6))     # Note: Test only draws 1/16 of the sreen area

        print( "Draw TSC calibration pattern")
        w,h,wu,hu=lcd.width//10,lcd.height//10,lcd.width//5,lcd.height//5
        bmp=build_rect_buf(w,h,[0xa0,0xf0])
        lcd.blit(wu,hu,w,h,bmp)
        lcd.blit(4*wu,hu,w,h,bmp)
        lcd.blit(4*wu,4*hu,w,h,bmp)
        lcd.blit(wu,4*hu,w,h,bmp)
        time.sleep(.5)

    for p in (20,100,80,50,10,60):
        lcd.set_backlight(p)
        time.sleep(.1)

spi=machine.SPI(
    1,
    baudrate=24_000_000,
    polarity=0,
    phase=0,
    sck=machine.Pin(10,machine.Pin.OUT),
    mosi=machine.Pin(11,machine.Pin.OUT),
    miso=machine.Pin(12,machine.Pin.IN)
)
# dma=rp2_dma.DMA(0)
rp2_dma=None

if 1:
    # Waveshare Pi Pico 2.8 LCD https://www.waveshare.com/Pico-ResTouch-LCD-2.8.htm
    waveshare_28_lcd=St7789_hw(rot=0,res=(240,320),spi=spi,rp2_dma=rp2_dma,cs=9,dc=8,bl=13,rst=15)
    test_lcd(lcd=waveshare_28_lcd)
if 1:
    # Waveshare Pi Pico 1.8 LCD https://www.waveshare.com/wiki/Pico-LCD-1.8
    # (not sure if this is redtab, but the driver works; someone with access to more hardware can adjust perhaps)
    waveshare_18_lcd=St7735_hw(rot=0,res=(128,160),spi=spi,rp2_dma=rp2_dma,cs=9,dc=8,bl=13,rst=12,model='redtab')
    test_lcd(lcd=waveshare_18_lcd)
if 1:
    # no-name variant which arduino library calls blacktab  (IIRC)
    noname_177_lcd=St7735_hw(rot=0,res=(128,160),spi=spi,rp2_dma=None,cs=9,dc=8,bl=13,rst=12,model='blacktab')
    test_lcd(lcd=noname_177_lcd)

