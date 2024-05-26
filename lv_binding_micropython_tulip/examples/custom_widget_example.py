
# This example shows how to create a custom widget and a custom theme

##############################################################################
# Initializations
##############################################################################

import usys as sys
sys.path.append('') # See: https://github.com/micropython/micropython/issues/6419

import lvgl as lv
import display_driver

lv.init()

##############################################################################
# Helper debug function to print member name
##############################################################################

member_name_cache = {}

def get_member_name(obj, value):
    try:
        return member_name_cache[id(obj)][id(value)]
    except KeyError:
        pass

    for member in dir(obj):
        if getattr(obj, member) == value:
            try:
                member_name_cache[id(obj)][id(value)] = member
            except KeyError:
                member_name_cache[id(obj)] = {id(value): member}
            return member

##############################################################################
# A class that describes a custom widget class
# An instance of this class can be used to create custom widgets
##############################################################################

class CustomWidgetClass():

    def __init__(self, width, height):
        # Define LVGL widget class
        # In this example the class receive parameters (width, height)
        self.lv_cls = lv.obj_class_t()
        self.lv_cls.constructor_cb = self.constructor
        self.lv_cls.destructor_cb = self.destructor
        self.lv_cls.event_cb = self.event_cb
        self.lv_cls.width_def = int(width)
        self.lv_cls.height_def = int(height)
        self.lv_cls.group_def = lv.obj.CLASS_GROUP_DEF.TRUE
        self.lv_cls.base_class = lv.obj_class

    def create(self, parent):
        # Create LVGL object from class
        return self.lv_cls.create_obj(parent)

    def get_class(self):
        # Return the internal LVGL class
        return self.lv_cls

    def constructor(self, lv_cls, obj):
        # Initialize the custom widget instance
        obj.valid = False
        obj.add_flag(obj.FLAG.CLICKABLE)
        obj.add_flag(obj.FLAG.CHECKABLE)
        obj.add_flag(obj.FLAG.SCROLL_ON_FOCUS)
        # print("Constructor called!")

    def destructor(self, lv_cls, obj):
        pass

    def event_cb(self, lv_cls, e):
        # Call the ancestor's event handler
        res = lv_cls.event_base(e)
        if res != lv.RESULT.OK:
            return

        code = e.get_code()
        obj = e.get_target_obj()

        # print("Event %s" % get_member_name(lv.EVENT, code))

        if code == lv.EVENT.DRAW_MAIN:
            # Draw the widget
            layer = e.get_layer()
            self.draw(obj, layer)
        elif code in [
                lv.EVENT.STYLE_CHANGED,
                lv.EVENT.VALUE_CHANGED,
                lv.EVENT.PRESSING,
                lv.EVENT.RELEASED]:
            # Check if need to recalculate widget parameters
            obj.valid = False

    def calc(self, obj):
        # Calculate object parameters
        area = lv.area_t()
        obj.get_content_coords(area)

        obj.draw_desc = lv.draw_triangle_dsc_t()
        obj.draw_desc.init()
        obj.draw_desc.bg_opa = lv.OPA.COVER
        obj.draw_desc.bg_color = obj.get_style_bg_color(lv.PART.MAIN)
        obj.points = [
            {'x':area.x1 + area.get_width()//2,
             'y':area.y2 if obj.get_state() & lv.STATE.CHECKED else area.y1},
            {'x':area.x2,
             'y':area.y1 + area.get_height()//2},
            {'x':area.x1,
             'y':area.y1 + area.get_height()//2},
        ]
        obj.draw_desc.p[0] = obj.points[0]
        obj.draw_desc.p[1] = obj.points[1]
        obj.draw_desc.p[2] = obj.points[2]

        obj.valid = True

    def draw(self, obj, layer):
        # If object invalidated, recalculate its parameters
        if not obj.valid:
            self.calc(obj)

        # Draw the custom widget
        lv.draw_triangle(layer, obj.draw_desc)

##############################################################################
# A Python class to wrap the LVGL custom widget
##############################################################################

class CustomWidget():

    # An instance of a widget-class to be used for creating custom widgets
    # d = lv.display_get_default()
    dpi = 130 # d.get_dpi()
    cls = CustomWidgetClass(dpi, dpi)

    @staticmethod
    def get_class():
        # Return the internal LVGL class
        return CustomWidget.cls.get_class()

    def __new__(cls, parent):
        # Return a new lv object instead of CustomWidget, 
        # but first bind the LVGL object with CustomWidgetWrapper
        wrapper = cls.CustomWidgetWrapper(parent)
        return wrapper.lv_obj

    class CustomWidgetWrapper():

        def __init__(self, parent):

            # Create the LVGL object from class
            self.lv_obj = CustomWidget.cls.create(parent)

            # Associates the LVGL object with CustomWidget wrapper
            self.lv_obj.set_user_data(self)

            # Initialize the object
            self.lv_obj.class_init_obj()


        def __getattr__(self, attr):
            # Provide access to LVGL object functions
            # print("__getattr__(%s, %s)" % (repr(self), repr(attr)))
            return getattr(self.lv_obj, attr)

        def __repr__(self):
            return "Custom Widget"

##############################################################################
# A theme to apply styles to the custom widget
##############################################################################

class CustomTheme(lv.theme_t):

    class Style(lv.style_t):
        def __init__(self):
            super().__init__()
            self.init()

            # Default color is gray
            self.set_bg_color(lv.palette_main(lv.PALETTE.GREY))

            # Child elements are centered
            self.set_layout(lv.LAYOUT.FLEX)
            self.set_flex_main_place(lv.FLEX_ALIGN.CENTER)
            self.set_flex_cross_place(lv.FLEX_ALIGN.CENTER)
            self.set_flex_track_place(lv.FLEX_ALIGN.CENTER)

    class PressedStyle(lv.style_t):
        def __init__(self):
            super().__init__()
            self.init()

            # Pressed color is blue
            self.set_bg_color(lv.palette_main(lv.PALETTE.BLUE))


    def __init__(self):
        super().__init__()
        self.custom_style = CustomTheme.Style()
        self.custom_pressed_style = CustomTheme.PressedStyle()

        # This theme is based on active theme
        base_theme = lv.theme_get_from_obj(lv.screen_active())

        # This theme will be applied only after base theme is applied
        self.set_parent(base_theme)

        # Set the "apply" callback of this theme to a custom callback
        self.set_apply_cb(self.apply)

        # Activate this theme on the default display
        lv.display_get_default().set_theme(self)
    
    def apply(self, theme, obj):
        # Apply this theme on CustomWidget class
        if obj.get_class() == CustomWidget.get_class():
            obj.add_style(self.custom_style, lv.PART.MAIN)
            obj.add_style(self.custom_pressed_style, lv.PART.MAIN | lv.STATE.PRESSED)


##############################################################################
# Main program - create screen and widgets
##############################################################################

# Create the theme for the custom widget
theme = CustomTheme()

# Create a screen with flex layout
scr = lv.screen_active()
scr.set_flex_flow(lv.FLEX_FLOW.COLUMN)
scr.set_flex_align(lv.FLEX_ALIGN.SPACE_EVENLY, lv.FLEX_ALIGN.CENTER, lv.FLEX_ALIGN.CENTER)

# Add a button with a label
button = lv.button(scr)
l1 = lv.label(button)
l1.set_text("Hello!")

# Add a custom widget with a label
customWidget = CustomWidget(scr)
l2 = lv.label(customWidget)
l2.set_text("Click me!")

# Add click events to both button and custom widget
def event_cb(e):
    print("%s Clicked!" % repr(e.get_target_obj()))

for widget in [button, customWidget]:
    widget.add_event_cb(event_cb, lv.EVENT.CLICKED, None)

