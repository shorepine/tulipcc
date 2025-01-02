import math
import sys
sys.path.append('.')

from snippets import numpy
from ulab import numpy as np

print(numpy.append([1, 2, 3], [[4, 5, 6], [7, 8, 9]]))

print(numpy.append([[1, 2, 3], [4, 5, 6]], [[7, 8, 9]], axis=0))

