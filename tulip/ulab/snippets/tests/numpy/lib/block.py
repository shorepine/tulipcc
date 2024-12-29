from ulab.numpy import array, zeros, eye, ones
from snippets import numpy

a = array([[1, 1]])
b = array([[2]])
c = array(
    [[3, 3],
     [3, 3],
     [3, 3]])
d = array(
    [[4],
     [4],
     [4]])
print(numpy.block([[a, b], [c, d]]))
a = zeros((2, 3))
b = eye(2) * 2
c = eye(3) * 5
d = ones((3, 2))
print(numpy.block([[a, b], [c, d]]))
