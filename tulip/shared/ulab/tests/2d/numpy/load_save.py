try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    a = np.array(range(25), dtype=dtype)
    b = a.reshape((5, 5))
    np.save('out.npy', a)
    print(np.load('out.npy'))
    np.save('out.npy', b)
    print(np.load('out.npy'))
