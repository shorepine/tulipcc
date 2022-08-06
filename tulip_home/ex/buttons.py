# make some buttons
def cb(x):
    if(x<2):
        print("button %d pressed" % (x))
    else:
        print("slider val is %f" % (tulip.slider(2)))

tulip.bg_clear()
tulip.ui_callback(cb)
tulip.button(0,"hey there",500,200,150,60,10,255,200,0)
tulip.button(1,"here again", 500, 400, 150, 60, 10, 255, 200, 0)
tulip.slider(2,200,400,30,150,0,0.5,tulip.color(255,0,0),tulip.color(0,255,0))
print("done")

