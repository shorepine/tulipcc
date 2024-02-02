# driver for m5stack dac2
# import m5dac2; m5dac2.set(5.0) # up to 10.0V
from machine import Pin, I2C
import time

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def set(volts, channel=0):
    val = int((volts/10.0) * 65535.0)
    if(val>65535): val = 65535
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    i2c.writeto_mem(89, 0x02+channel, bytes([b0,b1]))



