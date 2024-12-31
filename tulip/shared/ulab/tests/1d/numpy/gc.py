from ulab import numpy as np
import gc

data = np.ones(1000)[6:-6]
print(sum(data))
print(data)

gc.collect()

print(sum(data))
print(data)