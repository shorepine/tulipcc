# m5cardkb.py - driver for CardKB keyboard
from machine import Pin, I2C
import tulip

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def fb(stuff):
    b = i2c.readfrom(0x5f,1)
    if(len(b)):
        if b[0] != 0:
            tulip.key_send(b[0])
            
tulip.frame_callback(fb)

