# this test is meaningful only, when the firmware supports complex arrays

try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float, np.complex)

for dtype in dtypes:
    a = np.array(range(8), dtype=dtype)
    print('\narray:\n', a)
    print('\nreal part:\n', np.real(a))
    print('\nimaginary part:\n', np.imag(a))
    for m in (a.reshape((2, 4)), a.reshape((2, 2, 2))):
        print('\narray:\n', m)
        print('\nreal part:\n', np.real(m))
        print('\nimaginary part:\n', np.imag(m), '\n')


a = np.array([0, 1j, 2+2j, 3-3j], dtype=np.complex)
b = np.array([[0, 1j, 2+2j, 3-3j], [0, 1j, 2+2j, 3-3j]], dtype=np.complex)
c = np.array([[[0, 1j, 2+2j, 3-3j], [0, 1j, 2+2j, 3-3j]], [[0, 1j, 2+2j, 3-3j], [0, 1j, 2+2j, 3-3j]]], dtype=np.complex)

for m in (a, b, c):
    print('\n\narray:\n', m)
    print('\nreal part:\n', np.real(m))
    print('\nimaginary part:\n', np.imag(m))
