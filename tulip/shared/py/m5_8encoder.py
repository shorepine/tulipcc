# Copied from https://github.com/HowManyOliversAreThere/micropython-8encoder
# MIT licensed
# Examples at original repo

from machine import I2C
import struct

i2c = I2C(0, freq=400000)

ENCODERS = const(8)
LEDS = const(9)
address = 0x41

def _read(mem_addr: int, num_bytes: int):
    i2c.writeto(address, bytes([mem_addr]))
    return i2c.readfrom(address, num_bytes)

def _write(mem_addr: int, data: bytes):
    i2c.writeto_mem(address, mem_addr, data)

def read_counter(counter: int):
    if not (0 <= counter < ENCODERS):
        raise ValueError(
            f"Invalid counter number {counter} (valid values: 0-{ENCODERS - 1})"
        )

    val = _read(4 * counter, 4)
    return struct.unpack("<i", val)[0]

def read_all_counters():
    return [read_counter(i) for i in range(ENCODERS)]

def reset_counter(counter: int):
    if not (0 <= counter < ENCODERS):
        raise ValueError(
            f"Invalid counter number {counter} (valid values: 0-{ENCODERS - 1})"
        )

    _write(0x40 + counter, bytes([0xFF]))

def reset_all_counters():
    _write(0x40, bytes([0xFF] * ENCODERS))

def read_increment(counter: int):
    if not (0 <= counter < ENCODERS):
        raise ValueError(
            f"Invalid increment number {counter} (valid values: 0-{ENCODERS - 1})"
        )

    val = _read(0x20 + 4 * counter, 4)
    return struct.unpack("<i", val)[0]

def read_all_increments():
    return [read_increment(i) for i in range(ENCODERS)]

def read_button(button: int):
    if not (0 <= button < ENCODERS):
        raise ValueError(
            f"Invalid button number {button} (valid values: 0-{ENCODERS - 1})"
        )

    val = _read(0x50 + button, 1)
    return struct.unpack("B", val)[0]

def read_all_buttons():
    return [read_button(idx) for idx in range(ENCODERS)]

def read_switch():
    val = _read(0x60, 1)
    return struct.unpack("B", val)[0]

def set_led(led: int, color: bytes):
    if not (0 <= led < ENCODERS):
        raise ValueError(
            f"Invalid led number {led} (valid values: 0-{LEDS - 1})"
        )

    if len(color) != 3:
        raise ValueError(
            f"Colour must be a bytearray of lenth 3 (not {len(color)})"
        )

    _write(0x70 + 3 * led, color)

def firmware_version():
    val = _read(0xFE, 1)
    return struct.unpack("B", val)[0]
