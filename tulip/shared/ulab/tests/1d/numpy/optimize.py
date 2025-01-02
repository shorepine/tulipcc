import math

try:
    from ulab import scipy as spy
except ImportError:
	import scipy as spy

def f(x):
    return x**2 - 2.0

ref_result = 1.4142135623715149
result = (spy.optimize.bisect(f, 1.0, 3.0))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

ref_result = -7.105427357601002e-15
result = spy.optimize.fmin(f, 3.0, fatol=1e-15)
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

ref_result = -7.105427357601002e-15
result = spy.optimize.fmin(f, 3.0, xatol=1e-8, fatol=1e-15, maxiter=500)
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

ref_result = 1.41421826342255
result = (spy.optimize.newton(f, 3.0, tol=0.001, rtol=0.01))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))

result = (spy.optimize.newton(f, 3.0, tol=0.001, rtol=0.01, maxiter=100))
print(math.isclose(result, ref_result, rel_tol=1E-6, abs_tol=1E-6))
