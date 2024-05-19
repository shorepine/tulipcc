# make some UI elements on the REPL

import tulip

def button_cb(e):
    obj = e.get_target_obj()
    print("label of button pressed is %s" % (obj.get_child(0).get_text()))

def slider_cb(e):
    obj = e.get_target_obj()
    print("slider value is %d" % (obj.get_value()))

# Choose your screen to draw on.
# If you're making a UIScreen 'app", use the screen object passed into run(screen):
# Here we'll use the REPL.
screen = tulip.repl_screen

screen.add(tulip.UILabel("hello it is a test"), x=200,y=200)
screen.add(tulip.UIButton(text="Click me", fg_color=255, bg_color=200, w=250, callback=button_cb), x=600, y=200)
screen.add(tulip.UIButton(text="here again", fg_color=255, bg_color=3, w=250, callback=button_cb), x=700, y=400)

screen.add(tulip.UISlider(val=50,  w=15, h=150, bar_color=151, handle_color=188, callback=slider_cb), x=300, y=400)
screen.add(tulip.UISlider(val=50,  w=400, h=15, bar_color=255, handle_color=0, callback=slider_cb), x=250, y=300)


#tulip.ui_text(ui_id=4, text="12", x=200, y=200, w=100, fg_color=255, bg_color=0, font=tulip.lv.font_unscii_8)
#tulip.ui_text(ui_id=5, placeholder="Type here", x=400, y=200, w=200, fg_color=255, bg_color=0)

#tulip.ui_checkbox(ui_id=6, text="Extra text", x=400, y=400, fg_color=255, bg_color=0)
#tulip.ui_checkbox(ui_id=7, x=400, y=430, fg_color=255, bg_color=0)



