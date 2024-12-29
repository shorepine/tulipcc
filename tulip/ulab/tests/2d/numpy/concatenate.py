try:
    from ulab import numpy as np
except:
    import numpy as np

# test input types; the following should raise ValueErrors
objects = [([1, 2], [3, 4]),
            ((1, 2), (3, 4)), 
            (1, 2, 3)]

for obj in objects:
    try:
        np.concatenate(obj)
    except ValueError as e:
        print('ValueError: {}; failed with object {}\n'.format(e, obj))


a = np.array([1,2,3], dtype=np.float)
b = np.array([4,5,6], dtype=np.float)

print(np.concatenate((a,b)))
print(np.concatenate((a,b), axis=0))

a = np.array([[1,2,3],[4,5,6],[7,8,9]], dtype=np.float)
b = np.array([[1,2,3],[4,5,6],[7,8,9]], dtype=np.float)

print(np.concatenate((a,b), axis=0))
print(np.concatenate((a,b), axis=1))
print(np.concatenate((b,a), axis=0))
print(np.concatenate((b,a), axis=1))
