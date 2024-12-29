try:
    from ulab import numpy as np
except:
    import numpy as np

a = np.zeros((3, 4))

print(np.size(a, axis=0))
print(np.size(a, axis=1))
print(np.size(a, axis=None))
