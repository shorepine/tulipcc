try:
    from ulab import numpy as np
except:
    import numpy as np


dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

square = np.vectorize(lambda n: n*n)

for dtype in dtypes:
    a = np.array(range(9), dtype=dtype).reshape((3, 3))
    print(a)
    print(square(a))
    
    b = a[:,2]
    print(square(b))
    print()
