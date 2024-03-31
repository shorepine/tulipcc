# driver for m5stack extendIO
from machine import Pin, I2C

i2c = I2C(0, freq=400000)
addr = 0x27
INPUTPORT = 0x00
OUTPUTPORT = 0x01
POLINVPORT = 0x02
CONFIGPORT = 0x03


def _write_gpio(register, val):
    buf = bytearray([register&0xFF, val&0xFF])
    i2c.writeto(addr, buf)

def _read_gpio(register):
    readbuf = bytearray([register & 0xFF])
    i2c.readfrom_into(addr, readbuf)
    return readbuf[0]

# True -> input
# False -> output
def set_pin_mode(pin, val):
    current = _read_gpio(CONFIGPORT)
    if(val):
        _write_gpio(CONFIGPORT, current | (1 << pin))
    else:
        _write_gpio(CONFIGPORT, current & ~(1 << pin))

# True -> input
# False -> output
def get_pin_mode(pin):
    return bool((_read_gpio(CONFIGPORT)>> pin) & 0x01)

def write_pin(pin, val):
    current = _read_gpio(OUTPUTPORT)
    if(val):
        _write_gpio(OUTPUTPORT, current | (1<<pin))
    else:
        _write_gpio(OUTPUTPORT, current & ~(1 << pin))

def read_pin(pin):
    return bool((_read_gpio(INPUTPORT) >> pin) & 0x01)
