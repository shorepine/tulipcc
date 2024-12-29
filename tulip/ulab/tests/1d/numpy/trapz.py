try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

x = np.linspace(0, 9, num=10)
y = x*x
print(np.trapz(y))
print(np.trapz(y, x=x))
