# m5joy.py - support for the m5stack joystick
# returns 0-1 for x, y and 0/1 for button press

from machine import Pin, I2C

i2c = I2C(0, scl=Pin(18), sda=Pin(17), freq=400000)

def joy():
    i2c.writeto(0x52, bytes([3]))
    b = i2c.readfrom(0x52, 3)
    x = float(int.from_bytes(bytes([b[0]]), 'big')/255.0)
    y = float(int.from_bytes(bytes([b[1]]), 'big')/255.0)
    z = int.from_bytes(bytes([b[2]]), 'big')
    return (x,y,z)


print(joy())




