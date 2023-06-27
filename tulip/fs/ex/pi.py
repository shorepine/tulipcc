# compute pi digits to test CPU and GPU contention

import time

# Try the different display clock MHz 
for clock in [10, 12, 14, 18, 22, 28]:
    tulip.display_clock(clock)
    time.sleep(10) # just let it catch up to fps counter
    tulip.cpu()
    k =1
    s = 0
    iters = 100000
    t = time.ticks_ms()
    for i in range(iters):
        if i % 2 == 0:
            s += 4/k
        else:
            s -= 4/k
        k+=2
    
    print("%d MHz %2.2f%% CPU %2.1f%% GPU %2.1f FPS %dmS for %d iters: %f" % \
        (clock, tulip.cpu(), tulip.gpu(), tulip.fps(),time.ticks_ms()-t, iters, s))

#Back to normal
tulip.display_clock(18)

