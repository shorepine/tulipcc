# test joystick / keys
tulip.gpu_reset()
r = 40 # button radius

pos_map = {
    # x, y, color
    tulip.Joy.UP: [300,300, 129],
    tulip.Joy.DOWN: [300,500, 129],
    tulip.Joy.LEFT: [200,400, 129],
    tulip.Joy.RIGHT: [400,400, 129],
    tulip.Joy.X: [800,300, 229],
    tulip.Joy.B: [800,500, 229],
    tulip.Joy.A: [900,400, 229],
    tulip.Joy.Y: [700,400, 229],
    tulip.Joy.SELECT: [550,300, 145],
    tulip.Joy.START: [550,500, 145],
    tulip.Joy.L1: [300,200, 245],
    tulip.Joy.R1: [800,200, 245]

}
held = {}

for(k,v) in pos_map.items():
    tulip.bg_circle(v[0], v[1], r, v[2], 0)
    held[k] = False

while(True):
    mask = tulip.joyk()
    for k,v in pos_map.items():
        if(mask & k):
            tulip.bg_circle(v[0], v[1], r, v[2], 1)
            held[k] = True
        else:
            if(held.get(k,False) == True):
                tulip.bg_circle(v[0], v[1], r, 9, 1)
                tulip.bg_circle(v[0], v[1], r, v[2], 0)
                held[k] = False
