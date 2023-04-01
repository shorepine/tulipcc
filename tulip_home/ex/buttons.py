# make some buttons
import tulip
def cb(x):
    if(x<2):
        print("button %d pressed" % (x))
    if(x==2):
        print("slider val is %f" % (tulip.slider(x)))
    if(x>2):
        print("text box %d now says %s" % (x, tulip.text(x)))

tulip.bg_clear()
tulip.ui_callback(cb)
tulip.button(0,"hey there",600,200,150,60,10,255,200,0)
tulip.button(1,"here again", 500, 400, 150, 60, 10, 255, 200, 0)
tulip.slider(2,200,400,30,150,0,0.5,tulip.color(255,0,0),tulip.color(0,255,0))
tulip.text(3,"12", 200,200,100,40,255,0)
tulip.text(4,"Hello there", 400,200,100,40,255,0)
print("done")

