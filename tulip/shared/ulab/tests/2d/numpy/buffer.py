try:
    from ulab import numpy as np
except:
    import numpy as np

def print_as_buffer(a):
    print(len(memoryview(a)), list(memoryview(a)))
print_as_buffer(np.ones(3))
print_as_buffer(np.zeros(3))
print_as_buffer(np.eye(4))
print_as_buffer(np.ones(1, dtype=np.int8))
print_as_buffer(np.ones(2, dtype=np.uint8))
print_as_buffer(np.ones(3, dtype=np.int16))
print_as_buffer(np.ones(4, dtype=np.uint16))
print_as_buffer(np.ones(5, dtype=np.float))
print_as_buffer(np.linspace(0, 1, 9))

