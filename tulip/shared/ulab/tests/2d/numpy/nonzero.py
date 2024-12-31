try:
   from ulab import numpy as np
except:
   import numpy as np

array = np.array(range(16)).reshape((4,4))
print(array)
print(array < 5)
print(np.nonzero(array < 5))

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    array = (np.arange(2, 12, 3, dtype=dtype)).reshape((2,2)) - 2
    print(array)
    print(np.nonzero(array))