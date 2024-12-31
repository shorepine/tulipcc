import math

try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

def matrix_is_close(A, B, n):
    # primitive (i.e., independent of other functions) check of closeness of two square matrices
    for i in range(n):
        for j in range(n):
            print(math.isclose(A[i][j], B[i][j], rel_tol=1E-9, abs_tol=1E-9))

a = np.array([1,2,3], dtype=np.int16)
b = np.array([4,5,6], dtype=np.int16)
ab = np.dot(a.transpose(), b)
print(math.isclose(ab, 32.0, rel_tol=1E-9, abs_tol=1E-9))

a = np.array([1,2,3], dtype=np.int16)
b = np.array([4,5,6], dtype=np.float)
ab = np.dot(a.transpose(), b)
print(math.isclose(ab, 32.0, rel_tol=1E-9, abs_tol=1E-9))

a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])

c = np.array([[19, 22], [43, 50]])
matrix_is_close(np.dot(a, b), c, 2)

c = np.array([[26, 30], [38, 44]])
matrix_is_close(np.dot(a.transpose(), b), c, 2)

c = np.array([[17, 23], [39, 53]])
matrix_is_close(np.dot(a, b.transpose()), c, 2)

c = np.array([[23, 31], [34, 46]])
matrix_is_close(np.dot(a.transpose(), b.transpose()), c, 2)

a = np.array([[1., 2.], [3., 4.]])
b = np.linalg.inv(a)
ab = np.dot(a, b)
c = np.eye(2)
matrix_is_close(ab, c, 2)

a = np.array([[1, 2, 3, 4], [4, 5, 6, 4], [7, 8.6, 9, 4], [3, 4, 5, 6]])
b = np.linalg.inv(a)
ab = np.dot(a, b)
c = np.eye(4)
matrix_is_close(ab, c, 4)

a = np.array([[1, 2, 3, 4], [4, 5, 6, 4], [7, 8.6, 9, 4], [3, 4, 5, 6]])
result = (np.linalg.det(a))
ref_result = 7.199999999999995
print(math.isclose(result, ref_result, rel_tol=1E-9, abs_tol=1E-9))

a = np.array([1, 2, 3])
w, v = np.linalg.eig(np.diag(a))
for i in range(3):
    print(math.isclose(w[i], a[i], rel_tol=1E-9, abs_tol=1E-9))
for i in range(3):
    for j in range(3):
        if i == j:
            print(math.isclose(v[i][j], 1.0, rel_tol=1E-9, abs_tol=1E-9))
        else:
            print(math.isclose(v[i][j], 0.0, rel_tol=1E-9, abs_tol=1E-9))


a = np.array([[25, 15, -5], [15, 18,  0], [-5,  0, 11]])
result = (np.linalg.cholesky(a))
ref_result = np.array([[5.,  0.,  0.], [ 3.,  3.,  0.], [-1.,  1.,  3.]])
for i in range(3):
    for j in range(3):
        print(math.isclose(result[i][j], ref_result[i][j], rel_tol=1E-9, abs_tol=1E-9))

a = np.array([1,2,3,4,5], dtype=np.float)
result = (np.linalg.norm(a))
ref_result = 7.416198487095663
print(math.isclose(result, ref_result, rel_tol=1E-9, abs_tol=1E-9))

a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
result = (np.linalg.norm(a))  ## Here is a problem
ref_result = 16.881943016134134
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

a = np.array([[0, 1, 2], [3, 4 ,5], [5, 4, 8], [4, 4, 8] ], dtype=np.int16)
result = (np.linalg.norm(a,axis=0)) # fails on low tolerance
ref_result = np.array([7.071068, 7.0, 12.52996])
for i in range(3):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))

a = np.array([[0, 1, 2], [3, 4 ,5], [5, 4, 8], [4, 4, 8] ], dtype=np.int16)
result = (np.linalg.norm(a,axis=1)) # fails on low tolerance
ref_result = np.array([2.236068, 7.071068, 10.24695, 9.797959])
for i in range(4):
        print(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))
