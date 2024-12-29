from ulab import numpy as np

a = np.array([[1, 2], [2, 5]])
print(np.linalg.cholesky(a))

b = a = np.array([[25, 15, -5], [15, 18,  0], [-5,  0, 11]])
print(np.linalg.cholesky(b))

c = np.array([[18, 22,  54,  42], [22, 70,  86,  62], [54, 86, 174, 134], [42, 62, 134, 106]])
print(np.linalg.cholesky(c))




