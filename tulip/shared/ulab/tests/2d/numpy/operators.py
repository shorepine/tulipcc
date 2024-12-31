try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

  
print(len(np.array([1, 2, 3, 4, 5], dtype=np.uint8)))
print(len(np.array([[1, 2, 3],[4, 5, 6]])))

print(~np.array([0, -1, -100], dtype=np.uint8))
print(~np.array([0, -1, -100], dtype=np.uint16))
print(~np.array([0, -1, -100], dtype=np.int8))
print(~np.array([0, -1, -100], dtype=np.int16))

print(abs(np.array([0, -1, -100], dtype=np.uint8)))
print(abs(np.array([0, -1, -100], dtype=np.uint16)))
print(abs(np.array([0, -1, -100], dtype=np.int8)))
print(abs(np.array([0, -1, -100], dtype=np.int16)))
print(abs(np.array([0, -1, -100], dtype=np.float)))

print(-(np.array([0, -1, -100], dtype=np.uint8)))
print(-(np.array([0, -1, -100], dtype=np.uint16)))
print(-(np.array([0, -1, -100], dtype=np.int8)))
print(-(np.array([0, -1, -100], dtype=np.int16)))
print(-(np.array([0, -1, -100], dtype=np.float)))

print(+(np.array([0, -1, -100], dtype=np.uint8)))
print(+(np.array([0, -1, -100], dtype=np.uint16)))
print(+(np.array([0, -1, -100], dtype=np.int8)))
print(+(np.array([0, -1, -100], dtype=np.int16)))
print(+(np.array([0, -1, -100], dtype=np.float)))

print(np.array([1,2,3], dtype=np.float) > np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) > np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) > np.array([4,5,6], dtype=np.int16))
print(np.array([1,2,3], dtype=np.float) < np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) < np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) < np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) >= np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) >= np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) >= np.array([4,5,6], dtype=np.int16))
print(np.array([1,2,3], dtype=np.float) <= np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) <= np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) <= np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) > 4)
print(np.array([1,2,3], dtype=np.float) > 4.0)
print(np.array([1,2,3], dtype=np.float) < 4)
print(np.array([1,2,3], dtype=np.float) < 4.0)

print(np.array([1,2,3], dtype=np.float) == np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) == np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) == np.array([4,5,6], dtype=np.int16))
print(np.array([1,2,3], dtype=np.float) != np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) != np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) != np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) == 4)
print(np.array([1,2,3], dtype=np.float) == 4.0)
print(np.array([1,2,3], dtype=np.float) != 4)
print(np.array([1,2,3], dtype=np.float) != 4.0)

print(np.array([1,2,3], dtype=np.float) - np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) - np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) - np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) + np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) + np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) + np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) * np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) * np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) * np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) ** np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) ** np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) ** np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) / np.array([4,5,6], dtype=np.float))
print(np.array([1,2,3], dtype=np.float) / np.array([4,5,6], dtype=np.uint16))
print(np.array([1,2,3], dtype=np.float) / np.array([4,5,6], dtype=np.int16))

print(np.array([10,20,30], dtype=np.float) // np.array([4,5,6], dtype=np.float))
print(np.array([10,20,30], dtype=np.float) // np.array([4,5,6], dtype=np.uint16))
print(np.array([10,20,30], dtype=np.float) // np.array([4,5,6], dtype=np.int16))

print(np.array([1,2,3], dtype=np.float) - 4)
print(np.array([1,2,3], dtype=np.float) - 4.0)
print(np.array([1,2,3], dtype=np.float) + 4)
print(np.array([1,2,3], dtype=np.float) + 4.0)

print(np.array([1,2,3], dtype=np.float) * 4)
print(np.array([1,2,3], dtype=np.float) * 4.0)
print(np.array([1,2,3], dtype=np.float) ** 4)
print(np.array([1,2,3], dtype=np.float) ** 4.0)

print(np.array([1,2,3], dtype=np.float) / 4)
print(np.array([1,2,3], dtype=np.float) / 4.0)
print(np.array([10,20,30], dtype=np.float) // 4)
print(np.array([10,20,30], dtype=np.float) // 4.0)
print(np.array([10,20,30], dtype=np.int8) // 4)
print(np.array([10,20,30], dtype=np.int8) // 4.0)
print(np.array([10,20,30], dtype=np.uint16) // 4)
print(np.array([10,20,30], dtype=np.uint16) // 4.0)
print(np.array([10,20,30], dtype=np.int16) // 4)
print(np.array([10,20,30], dtype=np.int16) // 4.0)

a = np.array([1,2,3], dtype=np.float)
a -= np.array([4,5,6], dtype=np.float)
print(a)

a = np.array([1,2,3], dtype=np.float)
a -= np.array([4,5,6], dtype=np.uint16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a -= np.array([4,5,6], dtype=np.int16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a += np.array([4,5,6], dtype=np.float)
print(a)

a = np.array([1,2,3], dtype=np.float)
a += np.array([4,5,6], dtype=np.uint16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a += np.array([4,5,6], dtype=np.int16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a *= np.array([4,5,6], dtype=np.float)
print(a)

a = np.array([1,2,3], dtype=np.float)
a *= np.array([4,5,6], dtype=np.uint16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a *= np.array([4,5,6], dtype=np.int16)
print(a)

a = np.array([1,2,3], dtype=np.float)
#a /= np.array([4,5,6])
print(a)

a = np.array([1,2,3], dtype=np.float)
a **= np.array([4,5,6], dtype=np.float)
print(a)

a = np.array([1,2,3], dtype=np.float)
a **= np.array([4,5,6], dtype=np.uint16)
print(a)

a = np.array([1,2,3], dtype=np.float)
a **= np.array([4,5,6], dtype=np.int16)
print(a)

print(np.array([1,2,3],dtype=np.uint8) + np.array([4,5,6],dtype=np.uint8))
print(np.array([1,2,3],dtype=np.uint8) + np.array([4,5,6],dtype=np.int8))
print(np.array([1,2,3],dtype=np.int8) + np.array([4,5,6],dtype=np.int8))
print(np.array([1,2,3],dtype=np.uint8) + np.array([4,5,6],dtype=np.uint16))
print(np.array([1,2,3],dtype=np.int8) + np.array([4,5,6],dtype=np.uint16))
print(np.array([1,2,3],dtype=np.uint8) + np.array([4,5,6],dtype=np.int16))
print(np.array([1,2,3],dtype=np.int8) + np.array([4,5,6],dtype=np.int16))
print(np.array([1,2,3],dtype=np.uint16) + np.array([4,5,6],dtype=np.uint16))
print(np.array([1,2,3],dtype=np.int16) + np.array([4,5,6],dtype=np.int16))
print(np.array([1,2,3],dtype=np.int16) + np.array([4,5,6],dtype=np.uint16))

print(np.array([1,2,3],dtype=np.uint8) + np.array([4,5,6],dtype=np.float))
print(np.array([1,2,3],dtype=np.int8) + np.array([4,5,6],dtype=np.float))
print(np.array([1,2,3],dtype=np.uint16) + np.array([4,5,6],dtype=np.float))
print(np.array([1,2,3],dtype=np.int16) + np.array([4,5,6],dtype=np.float))
print(np.array([1,2,3],dtype=np.int16) + np.array([4,5,6],dtype=np.float))

a = np.array([1, 2, 3, 4, 5], dtype=np.uint8)
for i, _a in enumerate(a):
    print("element %d in a:"%i, _a)

