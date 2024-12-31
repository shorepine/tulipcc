# this test is meaningful only, when the firmware supports complex arrays

try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float, np.complex)

for dtype in dtypes:
    a = np.array(range(5), dtype=dtype)
    print('real part: ', np.real(a))
    print('imaginary part: ', np.imag(a))


b = np.array([0, 1j, 2+2j, 3-3j], dtype=np.complex)
print('real part: ', np.real(b))
print('imaginary part: ', np.imag(b))

