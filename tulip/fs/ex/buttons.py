# make some UI elements

import tulip
# cb gets the lvgl object being interacted with, an event code, and a ui_id
def cb(obj, code, ui_id):
    if(ui_id==0 or ui_id==1):
        button = obj
        print("button %d pressed" % (ui_id))
    if(ui_id==2 or ui_id==3):
        slider = obj
        print("slider %d is %d" % (ui_id, slider.get_value()))
    if(ui_id==4 or ui_id==5):
        textarea = obj
        print("text box %d now says %s" % (ui_id, textarea.get_text()))
    if(ui_id==6 or ui_id==7):
        checkbox = obj
        print("checkbox %d now %s" % (ui_id, checkbox.get_state()))

tulip.ui_callback = cb

    
tulip.ui_button(ui_id=0,text="hi there",x=600,y=200,fg_color=255,bg_color=200)
tulip.ui_button(ui_id=1,text="here again",x=700,y=400, fg_color=255, bg_color=3, w=150)
tulip.ui_slider(ui_id=2, val=50, x=300, y=400, w=15, h=150, bar_color=151, handle_color=188)
tulip.ui_slider(ui_id=3, val=50, x=250, y=300, w=400, bar_color=255, handle_color=0)

tulip.ui_text(ui_id=4, text="12", x=200, y=200, w=100, fg_color=255, bg_color=0, font=tulip.lv.font_unscii_8)
tulip.ui_text(ui_id=5, placeholder="Type here", x=400, y=200, w=200, fg_color=255, bg_color=0)

tulip.ui_checkbox(ui_id=6, text="Extra text", x=400, y=400, fg_color=255, bg_color=0)
tulip.ui_checkbox(ui_id=7, x=400, y=430, fg_color=255, bg_color=0)




