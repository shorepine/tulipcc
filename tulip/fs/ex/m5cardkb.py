# m5cardkb.py - driver for CardKB keyboard

# If you execfile("m5cardkb.py"), Tulip will take input from the cardKB
# it uses the frame callback to "thread" out the keyboard listener
# You can adapt this to read characters without sending them to Tulip

from machine import Pin, I2C
import tulip

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def fb(stuff):
    b = i2c.readfrom(0x5f,1)
    if(len(b)):
        if b[0] != 0:
            tulip.key_send(b[0])
            
tulip.frame_callback(fb)

