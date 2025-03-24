# make some UI elements on the REPL

import tulip

def button_cb(e):
    obj = e.get_target_obj()
    print("label of button pressed is %s" % (obj.get_child(0).get_text()))

def slider_cb(e):
    obj = e.get_target_obj()
    print("slider value is %d" % (obj.get_value()))

def check_cb(e):
    obj = e.get_target_obj()
    print("checkbox value is %s" % (obj.get_state()))

def text_cb(e):
    obj = e.get_target_obj()
    print("text value is %s" % (obj.get_text()))


def run(screen):
    screen.bg_color = 9

    screen.add(tulip.UILabel("hello it is a test"), x=200,y=200)
    screen.add(tulip.UIButton(text="Click me", fg_color=255, bg_color=200,  callback=button_cb), x=600, y=200)
    screen.add(tulip.UIButton(text="here again", fg_color=255, bg_color=3, w=250, callback=button_cb), x=700, y=400)

    screen.add(tulip.UISlider(val=50,  w=15, h=150, bar_color=151, handle_color=188, callback=slider_cb), x=300, y=400)
    screen.add(tulip.UISlider(val=50,  w=400, h=15, bar_color=255, handle_color=0, callback=slider_cb), x=250, y=300)

    screen.add(tulip.UIText(text="12",fg_color=255,w=100,bg_color=0, font=tulip.lv.font_unscii_8, callback=text_cb), x=200, y=200)
    screen.add(tulip.UIText(placeholder="Type here", w=220, fg_color=255,bg_color=0, font=tulip.lv.font_unscii_8, callback=text_cb), x=400, y=100)

    screen.add(tulip.UICheckbox(text="Extra text", fg_color=255, bg_color = 0, callback=check_cb), x=400, y=400)
    screen.add(tulip.UICheckbox(fg_color=255, bg_color = 0, callback=check_cb), x=400, y=460)

    screen.handle_keyboard = True
    screen.keep_tfb = True
    screen.present()

