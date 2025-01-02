import math
import sys
sys.path.append('.')

from snippets import numpy
from ulab import numpy as np

a = np.array([[1,2,3],[4,5,6]])
print(numpy.size(a))
print(numpy.size(a,1))
print(numpy.size(a,0))

print(numpy.prod([1, 10, 100, 5]))
print(numpy.prod([]))
print(numpy.prod([1.,2.]))


a = np.array([1,2,3])
print(numpy.nonzero(a))
b = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print(numpy.nonzero(b > 3))

c =  np.array([0,1,0,-1])
print(numpy.nonzero(c > 0))
print(numpy.nonzero(c < 0))


