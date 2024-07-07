# driver for mabee dac
# https://www.makerfabs.com/mabee-dac-gp8413.html
# import mabeedac; mabeedac.set(5.0, channel=0) # up to 10.0V
# If you connect up to 8 dacs, you can have 16 channels here
# you can change the address bits with placing/removing 0ohm resistor (or solder bridge) on pins A0 and A1 to set address
# just remove the 0 ohm resistor on A0 and it will appear at 88 (default is 89)
from machine import Pin, I2C
import time

i2c = I2C(0, freq=400000)

def set(volts, channel=0):
    val = int((volts/10.0) * 65535.0)
    if(val>65535): val = 65535
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    ch = 0x02
    addr = 89
    if(channel == 1): ch = 0x04
    if(channel == 2): addr = 88
    if(channel == 3): (ch, addr) = (0x04, 88)
    i2c.writeto_mem(addr, ch, bytes([b0,b1]))



