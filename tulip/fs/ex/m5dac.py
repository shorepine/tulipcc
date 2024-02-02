# Use an m5stack DAC / MPC4725 over I2C on Tulip.
# To use:
# import dac
# dac.dac(value) # 0-4095

import tulip, time
from machine import Pin, I2C
DAC_ADDR = 0x60

def value_to_packet(value):
    if(value>4095): value=4095
    if(value<0): value = 0
    b0 = 0x40 
    b1 = int(value/16)
    b2 = int((value % 16) << 4)
    print("b1 %d b2 %d" % (b1, b2))
    
    return bytes([b0,b1,b2])

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

found = False
if(DAC_ADDR in i2c.scan()):
    print("Found DAC")
    found = True
else:
    print("No DAC found.")

def dac(value):
    if(found):
        i2c.writeto(DAC_ADDR, value_to_packet(value))






