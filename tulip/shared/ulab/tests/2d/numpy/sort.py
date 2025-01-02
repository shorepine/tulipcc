try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    print()
    a = np.array([], dtype=dtype)
    print(np.sort(a, axis=0))
    print(list(np.argsort(a, axis=0)))

    a = np.array([4, 1, 3, 2], dtype=dtype)
    print(np.sort(a, axis=0))
    print(list(np.argsort(a, axis=0)))


