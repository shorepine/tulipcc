try:
    from ulab import numpy as np
except:
    import numpy as np

a = np.array(range(9), dtype=np.float)
print("a:\t", list(a))
print("a < 5:\t", list(a[a < 5]))
