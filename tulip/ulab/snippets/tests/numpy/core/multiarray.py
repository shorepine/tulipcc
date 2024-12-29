import math
import sys
sys.path.append('.')

from snippets import numpy
from ulab import numpy as np
np.set_printoptions(threshold=100)

print (numpy.asarray([1]))
print (numpy.asarray([1.0, 2.0, 3j]))
print (numpy.asarray([4, 3, 1, (2-2j), (2+2j), (2-1j), (2+1j), (2-1j), (2+1j), (1+1j), (1-1j)]))