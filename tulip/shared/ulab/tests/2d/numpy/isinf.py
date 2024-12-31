
from ulab import numpy as np

print('isinf(0): ', np.isinf(0))

a = np.array([1, 2, np.nan])
print('\n' + '='*20)
print('a:\n', a)
print('\nisinf(a):\n', np.isinf(a))

b = np.array([1, 2, np.inf])
print('\n' + '='*20)
print('b:\n', b)
print('\nisinf(b):\n', np.isinf(b))

c = np.array([1, 2, 3], dtype=np.uint16)
print('\n' + '='*20)
print('c:\n', c)
print('\nisinf(c):\n', np.isinf(c))

d = np.eye(5) * 1e999
print('\n' + '='*20)
print('d:\n', d)
print('\nisinf(d):\n', np.isinf(d))