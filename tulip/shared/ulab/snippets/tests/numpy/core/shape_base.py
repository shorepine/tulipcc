import math
import sys
sys.path.append('.')

from ulab import numpy as np
from snippets import numpy

print(numpy.atleast_1d(1.0))

x = np.arange(9.0).reshape((3,3))

print(numpy.atleast_1d(x))
print(numpy.atleast_1d(x) is x)

print(numpy.atleast_1d(1, [3, 4]))
