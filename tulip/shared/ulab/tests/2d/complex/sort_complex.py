try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float, np.complex)

for dtype in dtypes:
    print(np.sort_complex(np.array(range(5, 0, -1), dtype=dtype)))
    # these should all return an empty complex array
    print(np.sort_complex(np.array(range(5, 0, 1), dtype=dtype)))

print()
n = 6
a = np.array(range(n, 0, -1)) * 1j
b = np.array([1] * n)
print(np.sort_complex(a + b))

a = np.array(range(n)) * 1j
b = np.array([1] * n)
print(np.sort_complex(a + b))

print()
a = np.array([0, -3j, 1+2j, 1-2j, 2j], dtype=np.complex)
print(np.sort_complex(a))

a = np.array([0, 3j, 1-2j, 1+2j, -2j], dtype=np.complex)
print(np.sort_complex(a))
