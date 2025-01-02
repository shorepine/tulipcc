import math
try:
    from ulab import numpy as np
    from ulab import scipy as spy
except ImportError:
    import numpy as np
    import scipy as spy

x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=np.float)
sos = np.array([[1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6]],dtype=np.float)
result = spy.signal.sosfilt(sos, x)

ref_result = np.array([0.0000e+00, 1.0000e+00, -4.0000e+00, 2.4000e+01, -1.0400e+02, 4.4000e+02, -1.7280e+03,  6.5320e+03, -2.3848e+04, 8.4864e+04], dtype=np.float)
cmp_result = []
for p,q in zip(list(result), list(ref_result)):
    cmp_result.append(math.isclose(p, q, rel_tol=1e-06, abs_tol=1e-06))
print(cmp_result)

x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
sos = np.array([[1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6]],dtype=np.float)
zi = np.array([[1, 2], [3, 4]],dtype=np.float)
y, zo = spy.signal.sosfilt(sos, x, zi=zi)

y_ref = np.array([ 4.00000e+00, -1.60000e+01,  6.30000e+01, -2.27000e+02, 8.03000e+02, -2.75100e+03,  9.27100e+03, -3.07750e+04, 1.01067e+05, -3.28991e+05], dtype=np.float)
zo_ref = np.array([[37242.0, 74835.],[1026187.0, 1936542.0]], dtype=np.float)
cmp_result = []
for p,q in zip(list(y), list(y_ref)):
    cmp_result.append(math.isclose(p, q, rel_tol=1e-06, abs_tol=1e-06))
print(cmp_result)

cmp_result = []
for i in range(2):
    temp = []
    for j in range(2):
        temp.append(math.isclose(zo[i][j], zo_ref[i][j], rel_tol=1E-9, abs_tol=1E-9))
    cmp_result.append(temp)
print(cmp_result)
