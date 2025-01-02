import math
import sys
sys.path.append('.')

from snippets import numpy
from ulab import numpy as np


print(numpy.poly((0, 0, 0)))

print(numpy.poly((-1./2, 0, 1./2)))

print(numpy.poly((0.847, 0, 0.9883)))

#P = np.array([[0, 1./3], [-1./2, 0]])
#print(numpy.poly(P))
