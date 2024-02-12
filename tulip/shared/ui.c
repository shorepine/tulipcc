// ui.c
// user interface components
#include "ui.h"
#define MAX_BG_ELEMENTS 32

struct bg_element ** bg_elements;

#define CHECK_BG_UIID(ui_id)  if (bg_elements[ui_id]==NULL) mp_raise_ValueError(MP_ERROR_TEXT("unknown ui_id"))
uint8_t bg_bounds(uint16_t x, uint16_t y) {
    // find out if x&y are within bounds of any of the active elements, return the ID if so. return -1 if not
    for(uint8_t i=0;i<MAX_BG_ELEMENTS;i++) {
        struct bg_element * e = bg_elements[i];
        if(e != NULL) {
            if((x >= e->x) && (y >= e->y) && (x <= e->w + e->x) && (y <= e->y + e->h)) {
                return i;
            }
        }
    }
    return 255; 
}


uint8_t bg_touch_up(uint8_t ui_id) {
    CHECK_BG_UIID(ui_id);
    return bg_elements[ui_id]->up;
}

void bg_touch_register(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    CHECK_BG_UIID(ui_id);
    bg_elements[ui_id] = malloc_caps(sizeof(struct bg_element), MALLOC_CAP_SPIRAM);
    bg_elements[ui_id]->x = x;
    bg_elements[ui_id]->y = y;
    bg_elements[ui_id]->w = w;
    bg_elements[ui_id]->h = h;
    bg_elements[ui_id]->up = 0;
}
void bg_touch_deregister(uint8_t ui_id) {
    CHECK_BG_UIID(ui_id);
    free_caps(bg_elements[ui_id]);
    bg_elements[ui_id] = NULL;
}

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up) {
    // respond to finger down / up
    if(touch_held && up) { // this is a finger up / click release
       
        tulip_touch_isr(up);
        // Check BG
        uint8_t bg_id = bg_bounds(touch_x, touch_y);
        if(bg_id != 255) {
            bg_elements[bg_id]->up = 1;
            tulip_bg_touch_isr(bg_id); // up
        }
    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        tulip_touch_isr(up);
    } else if(!touch_held && !up) { // this is a new touch down 
        tulip_touch_isr(up);
        // Check BG
        uint8_t bg_id = bg_bounds(touch_x, touch_y);
        if(bg_id != 255) {
            bg_elements[bg_id]->up = 0;
            tulip_bg_touch_isr(bg_id); // down
        }
    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}
