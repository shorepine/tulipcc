try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float, np.complex)

for dtype in dtypes:
    print(np.conjugate(np.array(range(5), dtype=dtype)))

a = np.array([1, 2+2j, 3-3j, 4j], dtype=np.complex)
print(np.conjugate(a))