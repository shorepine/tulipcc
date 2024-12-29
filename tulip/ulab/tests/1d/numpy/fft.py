import math
try:
    from ulab import numpy as np
    use_ulab = True
except ImportError:
    import numpy as np
    use_ulab = False

x = np.linspace(-np.pi, np.pi, num=8)
y = np.sin(x)

if use_ulab:
    if 'real' in dir(np):
        a = np.fft.fft(y)
        c = np.real(np.fft.ifft(a))
    else:
        a, b = np.fft.fft(y)
        c, d = np.fft.ifft(a, b)
    # c should be equal to y
    cmp_result = []
    for p,q in zip(list(y), list(c)):
        cmp_result.append(math.isclose(p, q, rel_tol=1e-09, abs_tol=1e-09))
    print(cmp_result)

    z = np.zeros(len(x))
    if 'real' in dir(np):
        a = np.fft.fft(y)
        c = np.real(np.fft.ifft(a))
    else:
        a, b = np.fft.fft(y, z)
        c, d = np.fft.ifft(a, b)
    # c should be equal to y
    cmp_result = []
    for p,q in zip(list(y), list(c)):
        cmp_result.append(math.isclose(p, q, rel_tol=1e-09, abs_tol=1e-09))
    print(cmp_result)

else:
    a = np.fft.fft(y)
    c = np.fft.ifft(a)
    # c should be equal to y
    cmp_result = []
    for p,q in zip(list(y), list(c.real)):
        cmp_result.append(math.isclose(p, q, rel_tol=1e-09, abs_tol=1e-09))
    print(cmp_result) 
