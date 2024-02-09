# driver for m5stack dac2
# import m5dac2; m5dac2.set(5.0, channel=0) # up to 10.0V
# If you connect 2 dac2s, you can have four channels here
# just remove the 0 ohm resistor on A0 and it will appear at 88 (default is 89)
from machine import Pin, I2C
import time

i2c = I2C(0, freq=400000)

def set(volts, channel=0):
    val = int((volts/10.0) * 65535.0)
    if(val>65535): val = 65535
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    if(channel > 1):
            i2c.writeto_mem(88, 0x02+channel-2, bytes([b0,b1]))
    else:
        i2c.writeto_mem(89, 0x02+channel, bytes([b0,b1]))



