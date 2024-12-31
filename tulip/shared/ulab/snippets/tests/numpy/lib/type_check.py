import math
import sys
sys.path.append('.')

from snippets import numpy
from ulab import numpy as np

a = np.array([1+1j, 1+0j, 4.5, 3, 2, 2j], dtype=np.complex)
print(numpy.isreal(a))


a = np.array([1+2j, 2+1j], dtype=np.complex)
print(numpy.isreal(a))


print(numpy.isreal(1))
print(numpy.isreal(1j))