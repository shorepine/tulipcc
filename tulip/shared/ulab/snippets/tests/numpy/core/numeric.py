import math
import sys
sys.path.append('.')

from ulab import numpy as np
from snippets import numpy

x = np.array([[0, 1, 2],
           [3, 4, 5]])
print(numpy.zeros_like(x))

y = np.array([[0, 1j, -2j],[3, 4, 5]], dtype=np.complex)
print(numpy.zeros_like(y))

