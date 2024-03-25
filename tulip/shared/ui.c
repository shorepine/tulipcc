// ui.c
// user interface components
#include "ui.h"



void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up) {
    // respond to finger down / up
    if(touch_held && up) { // this is a finger up / click release
        // If there's any text entry happening, in all cases, a touch up stops it
        touch_held = 0;
        tulip_touch_isr(up);


    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        tulip_touch_isr(up);
    } else if(!touch_held && !up) { // this is a new touch down 
        touch_held = 1;
        tulip_touch_isr(up);

    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}