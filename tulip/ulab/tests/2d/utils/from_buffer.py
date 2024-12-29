from ulab import numpy as np
from ulab import utils

a = bytearray([1, 0, 0, 1, 0, 255, 255, 255])
print(utils.from_uint16_buffer(a))
a = bytearray([1, 0, 0, 1, 0, 255, 255, 255])
print(utils.from_int16_buffer(a))

a = bytearray([1, 0, 0, 1, 0, 255, 255, 255])
print(utils.from_uint32_buffer(a))
a = bytearray([1, 0, 0, 1, 0, 255, 255, 255])
print(utils.from_int32_buffer(a))

a = bytearray([1, 0, 0, 1, 0, 0, 255, 255])
print(utils.from_uint32_buffer(a))
a = bytearray([1, 0, 0, 1, 0, 0, 255, 255])
print(utils.from_int32_buffer(a))

a = bytearray([1, 0, 0, 0, 0, 0, 0, 1])
print(utils.from_uint32_buffer(a))
a = bytearray([1, 0, 0, 0, 0, 0, 0, 1])
print(utils.from_int32_buffer(a))
