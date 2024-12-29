try:
    from ulab import numpy as np
except:
    import numpy as np

for num in range(1,4):
    for start in range(-num, num+1):
        for end in range(-num, num+1):
            for stride in (-3, -2, -1, 1, 2, 3):
                l = list(range(num))
                a = np.array(l, dtype=np.int8)
                sl = l[start:end:stride]
                ll = len(sl)
                try:
                    sa = list(a[start:end:stride])
                except IndexError as e:
                    sa = str(e)
                print("%2d [% d:% d:% d]     %-24r %-24r%s" % (
                    num, start, end, stride, sl, sa, " ***" if sa != sl else ""))

                a[start:end:stride] = np.ones(len(sl)) * -1
                print("%2d [% d:% d:% d]     %r" % (
                    num, start, end, stride, list(a)))
