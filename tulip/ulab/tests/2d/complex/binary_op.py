try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

n = 5
a = np.array(range(n), dtype=np.complex)
c = np.array(range(n), dtype=np.complex)

print(a == c)
print(a != c)
print()

c = np.array(range(n), dtype=np.complex) * 1j
print(a == c)
print(a != c)
print()

for dtype in dtypes:
    b = np.array(range(n), dtype=dtype)
    print(b == a)
    print(b != a)
    print()

