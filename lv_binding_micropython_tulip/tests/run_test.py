##############################################################################
# Run LVGL examples
# An example script should be provided as an argument.
#
# This script would:
# - Initialize LVGL and display
# - Read the example file
# - Chdir to the example file dir
# - Run the example
# - gc collect
# - Try to perform some actions such as sending click/value-changed events
# - Deinit the display
#
# It tries not to pollute the namespace of script under test.
# The only import visible for it is 'lv'.
#
# In case of an error, display the error and exit with code 255
# 255 was selected to stop xargs
#
##############################################################################

DELAY_MS=25

import usys
usys.path.append('') # See: https://github.com/micropython/micropython/issues/6419

import gc
import os
import time
import lvgl as lv
import lv_utils
import display_driver_utils

events = [lv.EVENT.SCROLL, lv.EVENT.CLICKED, lv.EVENT.VALUE_CHANGED, lv.EVENT.READY, lv.EVENT.FOCUSED]

class ExceptionHandler:
    def __init__(self):
        self.recorded_exception = None

    def handle_exceptions(self, e):
        lv_utils.event_loop.current_instance().deinit()
        if not self.recorded_exception:
            self.recorded_exception = e
    
    def reraise(self):
        if self.recorded_exception:
            raise self.recorded_exception

lv.init()
exception_handler = ExceptionHandler()
driver = display_driver_utils.driver(exception_sink = exception_handler.handle_exceptions)
scr = lv.screen_active()
objects = []

def collect_objects(obj, user_data):
    if hasattr(obj, 'lv_obj'):
        obj = obj.lv_obj
    objects.append(obj)
    return lv.obj.TREE_WALK.NEXT

def send_events():
    for obj in objects:
        if lv.obj.__cast__(obj): # skip deleted objects
            obj_info = ''
            if hasattr(obj, 'get_text'):
                obj_info += ' text:"%s"' % obj.get_text()
            if hasattr(obj, 'get_value'):
                obj_info += ' value:"%s"' % obj.get_value()
            print('%s %s' % (obj, obj_info))
            for event in events:
                if not lv.obj.__cast__(obj): # skip deleted objects
                    continue
                # print('\t%s' % get_member_name(lv.EVENT, event))
                obj.send_event(event, None)
                time.sleep_ms(DELAY_MS)
                gc.collect()

def run():
    try:
        script = usys.argv[1]
        script_path = script[:script.rfind('/')] if script.find('/') >= 0 else '.'
        script_name = script[script.rfind('/')+1:] if script.find('/') >= 0 else script

        print('Running %s ...' % script)

        with open(script, 'r') as file:
            file_string = file.read()
            os.chdir(script_path)
            usys.argv[0] = script_name
            del usys.argv[1]
            exec(file_string, {'__file__': script_name, 'lv': lv})
            time.sleep_ms(DELAY_MS * 2)
            gc.collect()
            lv.screen_active().tree_walk(collect_objects, None)
            send_events()
            time.sleep_ms(DELAY_MS)
            exception_handler.reraise()
            if lv_utils.event_loop.is_running():
                lv_utils.event_loop.current_instance().deinit()
                time.sleep_ms(DELAY_MS)

    except Exception as e:
        usys.print_exception(e)
        usys.exit(255) # 255 to exit xargs

run()
