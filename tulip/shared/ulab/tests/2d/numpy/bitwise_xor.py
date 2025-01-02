try:
    from ulab import numpy as np
except:
    import numpy as np


dtypes = (np.uint8, np.int8, np.uint16, np.int16)

for dtype1 in dtypes:
    x1 = np.array(range(5), dtype=dtype1)
    for dtype2 in dtypes:
        x2 = np.array(range(5, 0, -1), dtype=dtype2)

        print(np.bitwise_xor(x1, x2))
