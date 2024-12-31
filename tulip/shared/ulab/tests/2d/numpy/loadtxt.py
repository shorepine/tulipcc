try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16)

a = np.array(range(8)).reshape((2, 4))
np.savetxt('loadtxt.dat', a, header='test file data')

print(np.loadtxt('loadtxt.dat'))
print()

for dtype in dtypes:
    print(np.loadtxt('loadtxt.dat', dtype=dtype))
    print()

np.savetxt('loadtxt.dat', a, delimiter=',', header='test file data')

print(np.loadtxt('loadtxt.dat', delimiter=','))
print()

np.savetxt('loadtxt.dat', a, delimiter=',', comments='!', header='test file data')

print(np.loadtxt('loadtxt.dat', delimiter=',', comments='!'))
print()
print(np.loadtxt('loadtxt.dat', delimiter=',', comments='!', usecols=1))
print()
print(np.loadtxt('loadtxt.dat', delimiter=',', comments='!', usecols=(0, 1)))
print()

a = np.array(range(36)).reshape((9, 4))
np.savetxt('loadtxt.dat', a, header='9 data rows and a comment')
print(np.loadtxt('loadtxt.dat', max_rows=5))

print()
print(np.loadtxt('loadtxt.dat', skiprows=5, dtype=np.uint16))
