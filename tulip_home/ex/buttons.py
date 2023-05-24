# make some buttons
import tulip
def cb(x):
    if(x==0 or x==1):
        print("button %d pressed" % (x))
    if(x==2 or x==3):
        print("slider %d is %f" % (x, tulip.ui_slider(x)))
    if(x==4 or x==5):
        print("text box %d now says %s" % (x, tulip.ui_text(x)))
    if(x==6 or x==7):
        print("check box %d now %d" % (x, tulip.ui_checkbox(x)))

tulip.bg_clear()
tulip.ui_callback(cb)

    
tulip.ui_button(0,"hey there",600,200,150,60,255,200,0,0)
tulip.ui_button(1,"here again", 700, 400, 150, 60, 255, 200, 1,1)
tulip.ui_slider(2, 0.5, 200,400,30,150, 151,188)
tulip.ui_slider(3, 0.5, 200,300,150,30, 151,188)
tulip.ui_text(4,"12", 200,200,100,40,255,0,2)
tulip.ui_text(5,"Hello there", 400,200,100,40,255,0,3)
tulip.ui_checkbox(6, 0, 400, 400, 50, 255, 0)
tulip.ui_checkbox(7, 1, 475, 400, 50, 255, 0)

for i in range(8):
    tulip.ui_active(i, 1)

print("done")

