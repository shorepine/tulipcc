# 8angle.py - support for the m5stack 8angle

from machine import Pin, I2C

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def knob(num):
    i2c.writeto(67, bytes([0x10 + num]))
    b = i2c.readfrom(67, 1)
    i = int.from_bytes(b, "big")
    return 1.0 - float(i)/255.0


for i in range(8):
    print("[%d] %.2f " % (i, knob(i)), end='')
print()




