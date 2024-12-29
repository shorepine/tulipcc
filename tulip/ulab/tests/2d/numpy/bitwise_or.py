try:
    from ulab import numpy as np
except:
    import numpy as np


dtypes = (np.uint8, np.int8, np.uint16, np.int16)
test_values1 = (0, 1, 0, 1, 2, 5)
test_values2 = (0, 1, 1, 0, 2, 7)


for dtype1 in dtypes:
    x1 = np.array(test_values1, dtype=dtype1)
    for dtype2 in dtypes:
        x2 = np.array(test_values2, dtype=dtype2)
        print(np.bitwise_or(x1, x2))
