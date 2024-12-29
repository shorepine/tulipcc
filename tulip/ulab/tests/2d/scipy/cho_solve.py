import math

try:
    from ulab import scipy, numpy as np
except ImportError:
    import scipy
    import numpy as np

## test cholesky solve
L = np.array([[3, 0, 0, 0], [2, 1, 0, 0], [1, 0, 1, 0], [1, 2, 1, 8]])
b = np.array([4, 2, 4, 2])

# L needs to be a lower triangular matrix
result = scipy.linalg.cho_solve(L, b)
ref_result = np.array([-0.01388888888888906, -0.6458333333333331, 2.677083333333333, -0.01041666666666667])

for i in range(4):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))

## test cholesky and cho_solve together
C = np.array([[18, 22,  54,  42], [22, 70,  86,  62], [54, 86, 174, 134], [42, 62, 134, 106]])
L = np.linalg.cholesky(C)

# L is a lower triangular matrix obtained by performing cholesky of positive-definite linear system
result = scipy.linalg.cho_solve(L, b)
ref_result = np.array([6.5625, 1.1875, -2.9375, 0.4375])

for i in range(4):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))
