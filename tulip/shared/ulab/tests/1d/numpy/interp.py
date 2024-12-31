try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

x = np.array([1, 2, 3, 4, 5])
xp = np.array([1, 2, 3, 4])
fp = np.array([1, 2, 3, 4])    
print(np.interp(x, xp, fp))
print(np.interp(x, xp, fp, left=0.0))
print(np.interp(x, xp, fp, right=10.0))
print(np.interp(x, xp, fp, left=0.0, right=10.0))
