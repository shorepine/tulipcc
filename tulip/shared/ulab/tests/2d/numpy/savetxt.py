try:
    from ulab import numpy as np
except:
    import numpy as np

a = np.array(range(9))

print('savetxt with linear arrays')
np.savetxt('savetxt.dat', a)

with open('savetxt.dat', 'r') as fin:
    print(fin.read())

a = a.reshape((3, 3))

print('savetxt with no keyword arguments')
np.savetxt('savetxt.dat', a)

with open('savetxt.dat', 'r') as fin:
    print(fin.read())

print('savetxt with delimiter')
np.savetxt('savetxt.dat', a, delimiter=',')

with open('savetxt.dat', 'r') as fin:
    print(fin.read())

print('savetxt with header')
np.savetxt('savetxt.dat', a, header='column1 column2 column3')

with open('savetxt.dat', 'r') as fin:
    print(fin.read())

print('savetxt with footer')
np.savetxt('savetxt.dat', a, footer='written data file')

with open('savetxt.dat', 'r') as fin:
    print(fin.read())