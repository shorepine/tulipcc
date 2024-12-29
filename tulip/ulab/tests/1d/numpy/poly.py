import math

try:
    from ulab import numpy as np
except ImportError:
    import numpy as np
    
p = [1, 1, 1, 0]
x = [0, 1, 2, 3, 4]
result = np.polyval(p, x)
ref_result = np.array([0, 3, 14, 39, 84])
for i in range(len(x)):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))

a = np.array(x)
result = np.polyval(p, a)
ref_result = np.array([0, 3, 14, 39, 84])
for i in range(len(x)):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))

# linear fit
x = np.linspace(-10, 10, 20)
y = 1.5*x + 3
result = np.polyfit(x, y, 1)
ref_result = np.array([ 1.5, 3.0])
for i in range(2):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))

# 2nd degree fit
x = np.linspace(-10, 10, 20)
y = x*x*2.5 - x*0.5 + 1.2
result = np.polyfit(x, y, 2)
ref_result = np.array([2.5,  -0.5, 1.2])
for i in range(3):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))

# 3rd degree fit
x = np.linspace(-10, 10, 20)
y = x*x*x*1.255 + x*x*1.0 - x*0.75 + 0.0
result = np.polyfit(x, y, 3)
ref_result = np.array([1.255, 1.0, -0.75, 0.0])
for i in range(4):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))

# 4th degree fit
x = np.linspace(-10, 10, 20)
y = x*x*x*x + x*x*x*1.255 + x*x*1.0 - x*0.75 + 0.0
result = np.polyfit(x, y, 4)
ref_result = np.array([1.0, 1.255, 1.0, -0.75, 0.0])
for i in range(5):
    print(math.isclose(result[i], ref_result[i], rel_tol=1E-9, abs_tol=1E-9))    
