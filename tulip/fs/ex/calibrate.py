# calibrate.py
# find touch delta
import time

test_pos = [
    [512,300],
    [20, 300],
    [1014, 300],
    [512,20],
    [512,580],
    [500, 300],
    [540, 300]
    ]

got = [-1. -1]
deltas = []
def touch_cb(x):
    global got
    t = tulip.touch()
    if(x==0): # down
        got = [t[0], t[1]]

tulip.touch_callback(touch_cb)

print("Tap the middle of the pink circles.")
for (x,y) in test_pos:
    tulip.bg_clear()
    time.sleep_ms(500)
    got = [-1, -1]
    tulip.circle(x,y,20, 194, 1)
    while(got[0] < 0):
        time.sleep_ms(50)
    deltas.append( (x- got[0], y-got[1]) )

x_mean = 0
y_mean = 0
for (x,y) in deltas:
    x_mean += x
    y_mean += y
x_mean = int(float(x_mean) / float(len(deltas)))
y_mean = int(float(y_mean) / float(len(deltas)))

old_delta = tulip.touch_delta()
print("New computed delta is [%d, %d]. It was [%d, %d]. " %(x_mean, y_mean, old_delta[0], old_delta[1]))
print("Set it? [Y/N]")
r = input()
if(r=='y' or r=='Y'):
    tulip.touch_delta(x_mean, y_mean)
    print("Set. Add tulip.touch_delta(%d, %d) to boot.py if you want this to persist." % (x_mean, y_mean))

tulip.bg_clear()
tulip.touch_callback()