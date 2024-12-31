try:
    from ulab import numpy as np
    from ulab import scipy as spy
except:
    import numpy as np
    import scipy as spy

x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
sos = [[1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6]]
zi = np.array([[1, 2], [3, 4], [5, 6]],dtype=np.float)
y, zo = spy.signal.sosfilt(sos, x, zi=zi)
print('y: ', y)
print('zo: ', zo)
