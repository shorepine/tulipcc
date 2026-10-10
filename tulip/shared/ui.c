// ui.c
// user interface components
#include "ui.h"

extern void mp_schedule_lv();

// LVGL samples touch_held only when its indev read timer runs (at most once per
// frame, and later if Python is busy). A quick tap can go down and up entirely
// between two samples and LVGL never sees it. So a new touch down is latched here
// and handed to LVGL on its next read even if the finger has already lifted.
volatile uint8_t touch_down_pending = 0;
volatile int16_t touch_down_x = 0;
volatile int16_t touch_down_y = 0;

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up) {
    // respond to finger down / up
    if(touch_held && up) { // this is a finger up / click release
        // If there's any text entry happening, in all cases, a touch up stops it
        touch_held = 0;
        tulip_touch_isr(up);
        mp_schedule_lv(); // run LVGL now instead of waiting for the next frame


    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        tulip_touch_isr(up);
    } else if(!touch_held && !up) { // this is a new touch down 
        touch_down_x = touch_x;
        touch_down_y = touch_y;
        touch_down_pending = 1;
        touch_held = 1;
        tulip_touch_isr(up);
        mp_schedule_lv();

    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}