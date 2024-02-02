# driver for m5stack adc
# import m5adc; m5adc.get() # returns volts 0-12V
from machine import Pin, I2C
import time

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def get():
    b = i2c.readfrom(0x48,3)
    val = (b[0] << 8) | (b[1] & 0xff)
    # 26435 = 10V measured from the dac2
    return (val / 26435.0)*10.0




