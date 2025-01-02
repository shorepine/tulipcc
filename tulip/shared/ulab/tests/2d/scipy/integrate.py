import sys
from math import *

try:
    from ulab import scipy
except ImportError:
    import scipy

f = lambda x: x * sin(x) * exp(x)
a=1
b=2

(res, err) = scipy.integrate.tanhsinh(f, a, b)
if isclose (res, 7.11263821415851) and isclose (err, 5.438231077315757e-14):
	print (res, err)
		
res = scipy.integrate.romberg(f, a, b)
if isclose (res, 7.112638214158507):
	print (res)

res = scipy.integrate.simpson(f, a, b)
if isclose (res, 7.112638214158494):
	print (res)

(res, err) = scipy.integrate.quad(f, a, b)
if isclose (res, 7.112638214158507) and isclose (err, 7.686723611780195e-14):
		print (res, err)

