import math

try:
    from ulab import numpy as np
    from ulab import scipy as spy
except ImportError:
    import numpy as np
    import scipy as spy
    
result = (np.sin(np.pi/2))
ref_result = 1.0
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.cos(np.pi/2))
ref_result = 0.0
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.tan(np.pi/2))
ref_result = 1.633123935319537e+16
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.sinh(np.pi/2))
ref_result = 2.3012989023072947
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.cosh(np.pi/2))
ref_result = 2.5091784786580567
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.tanh(np.pi/2))
ref_result = 0.9171523356672744
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

ref_result = np.pi/2
result = (np.asin(np.sin(np.pi/2)))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.acos(np.cos(np.pi/2)))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.atan(np.tan(np.pi/2)))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.cosh(np.acosh(np.pi/2)))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (np.sinh(np.asinh(np.pi/2)))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))
    
print(np.degrees(np.pi))
print(np.radians(np.degrees(np.pi)))
print(np.floor(np.pi))
print(np.ceil(np.pi))
print(np.sqrt(np.pi))
print(np.exp(1))
print(np.log(np.exp(1)))

print(np.log2(2**1))

print(np.log10(10**1))
print(math.isclose(np.exp(1) - np.expm1(1), 1))

x = np.array([-1, +1, +1, -1])
y = np.array([-1, -1, +1, +1])
result = (np.arctan2(y, x) * 180 / np.pi)
ref_result = np.array([-135.0, -45.0, 45.0, 135.0], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

x = np.linspace(-2*np.pi, 2*np.pi, 5)
result = np.sin(x)
ref_result = np.array([2.4492936e-16, -1.2246468e-16,  0.0000000e+00,  1.2246468e-16, -2.4492936e-16], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = np.cos(x)
ref_result = np.array([1., -1.,  1., -1.,  1.], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = np.tan(x)
ref_result = np.array([2.4492936e-16, 1.2246468e-16, 0.0000000e+00, -1.2246468e-16, -2.4492936e-16], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = np.sinh(x)
ref_result = np.array([-267.74489404, -11.54873936, 0., 11.54873936, 267.74489404], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)    

result  = np.cosh(x)
ref_result = np.array([267.74676148,  11.59195328, 1.0, 11.59195328, 267.74676148], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = np.tanh(x)
ref_result = np.array([-0.9999930253396107, -0.99627207622075, 0.0, 0.99627207622075, 0.9999930253396107], dtype=np.float)
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = np.sinc(x)
ref_result = np.array([0.03935584386392389, -0.04359862862918773, 1.0, -0.04359862862918773, 0.03935584386392389])
cmp_result = []
for i in range(len(x)):
    cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = (spy.special.erf(np.linspace(-3, 3, num=5)))
ref_result = np.array([-0.9999779095030014, -0.9661051464753108, 0.0, 0.9661051464753108, 0.9999779095030014], dtype=np.float)
cmp_result = []
for i in range(len(ref_result)):
	cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = (spy.special.erfc(np.linspace(-3, 3, num=5)))
ref_result = np.array([1.99997791e+00, 1.96610515e+00, 1.00000000e+00, 3.38948535e-02, 2.20904970e-05], dtype=np.float)                          
cmp_result = []
for i in range(len(ref_result)):
	cmp_result.append(math.isclose(result[i], ref_result[i], rel_tol=1E-6, abs_tol=1E-6))
print(cmp_result)

result = (spy.special.gamma(np.array([0, 0.5, 1, 5])))
ref_result = np.array([1.77245385,  1.0, 24.0])
cmp_result = []
cmp_result.append(math.isinf(result[0]))
for i in range(len(ref_result)):
	cmp_result.append(math.isclose(result[i+1], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))
print(cmp_result)

result = (spy.special.gammaln([0, -1, -2, -3, -4]))
cmp_result = []
for i in range(len(ref_result)):
	cmp_result.append(math.isinf(result[i]))
print(cmp_result)
