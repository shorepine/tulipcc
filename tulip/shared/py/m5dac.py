# driver for the m5stack DAC (DAC1, not DAC2)
# import m5dac
# m5dac.set(volts) # 0-3.3V

from machine import Pin, I2C
i2c = I2C(0, freq=400000)

def set(volts):
    value = int((volts / 3.3)*4095)
    if(value>4095): value=4095
    if(value<0): value = 0
    b0 = 0x40 
    b1 = int(value/16)
    b2 = int((value % 16) << 4)
    i2c.writeto(0x60, bytes([b0,b1,b2]))




