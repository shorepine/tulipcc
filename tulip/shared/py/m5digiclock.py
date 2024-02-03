# control an m5stack digi-clock 
from machine import Pin, I2C
i2c = I2C(0, freq=400000)

# Set with a 4 character string
def set(s):
    i2c.writeto_mem(0x30, 0x20, bytes(s.encode('ascii')))
