try:
    from ulab import numpy as np
except:
    import numpy as np

np.set_printoptions(threshold=200)

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    a = np.array(range(25), dtype=dtype).reshape((5,5))
    print(np.delete(a, [1, 2], axis=0))
    print(np.delete(a, [1, 2], axis=1))
    print(np.delete(a, [], axis=1))
    print(np.delete(a, [1, 5, 10]))
    print(np.delete(a, []))

for dtype in dtypes:
    a = np.array(range(25), dtype=dtype).reshape((5,5))
    print(np.delete(a, 2, axis=0))
    print(np.delete(a, 2, axis=1))
    print(np.delete(a, 2))

for dtype in dtypes:
    a = np.array(range(25), dtype=dtype).reshape((5,5))
    print(np.delete(a, -3, axis=0))
    print(np.delete(a, -3, axis=1))
    print(np.delete(a, -3))
