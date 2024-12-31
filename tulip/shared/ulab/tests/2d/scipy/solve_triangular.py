import math

try:
    from ulab import scipy, numpy as np
except ImportError:
    import scipy
    import numpy as np

A = np.array([[3, 0, 2, 6], [2, 1, 0, 1], [1, 0, 1, 4], [1, 2, 1, 8]])
b = np.array([4, 2, 4, 2])

# forward substitution
result = scipy.linalg.solve_triangular(A, b, lower=True)
ref_result = np.array([1.333333333, -0.666666666, 2.666666666, -0.083333333])
for i in range(4):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))

# backward substitution
result = scipy.linalg.solve_triangular(A, b, lower=False)
ref_result = np.array([-1.166666666, 1.75, 3.0, 0.25])
for i in range(4):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))
