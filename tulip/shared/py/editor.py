# editor.py
# Tulip C editor wrapped in UIScreen.
# One day (not today), we should re-write the editor in pure python here too (still using the TFB)
# That will let us do LVGL stuff for saving/searching/etc

import tulip
editor = None


def draw(x):
    global editor
    editor.alttab_button.invalidate()
    editor.quit_button.invalidate()

class Editor(tulip.UIScreen):
    def __init__(self, filename):
        self.filename = filename
        # Make sure to turn off the offsets for the task bar
        super().__init__('edit', bg_color=36, keep_tfb=True, offset_x=0, offset_y=0)
        self.quit_callback = self.quit_editor_cb
        self.deactivate_callback = self.deactivate_editor_cb
        self.activate_callback = self.activate_editor_cb
        self.first_run = True
        self.present()

    def deactivate_editor_cb(self, screen):
        tulip.keyboard_callback()
        tulip.tfb_restore()
 

    def quit_editor_cb(self, screen):
        tulip.deinit_editor()

    def activate_editor_cb(self,screen):
        # Only load in the file on first run
        tulip.tfb_save()
        if(self.first_run):
            self.first_run = False
            if(self.filename is None):
                tulip.run_editor()
            else:
                tulip.run_editor(self.filename)

        tulip.keyboard_callback(tulip.key_editor)
        # The TFB switches over, but the REPL will print >>> after this runs, 
        # overwriting the first line. So wait a bit and activate then
        tulip.defer(tulip.activate_editor, None, 50)
        # And because of the TFB clearing, the buttons may get destroyed, so re-draw them
        tulip.defer(draw, None, 100)

# Launch the tulip editor as a UIScreen
def edit(filename=None):
    global editor
    if 'edit' in tulip.running_apps:
        print("Editor already running.")
        return
    else:
        editor = Editor(filename)
