// ui.c
// user interface components
#include "ui.h"
uint8_t ui_element_active[MAX_UI_ELEMENTS];
uint16_t ui_element_x[MAX_UI_ELEMENTS];
uint16_t ui_element_y[MAX_UI_ELEMENTS];
uint16_t ui_element_w[MAX_UI_ELEMENTS];
uint16_t ui_element_h[MAX_UI_ELEMENTS];


void ui_element_remove(uint8_t ui_id) {
    // what to about the graphics?
    ui_element_active[ui_id] = 0;
}

// Now, how do i go and change this? I want to higlight it when it's in focus 
// i could save the metadata of the button (really, just str, and radius, and colors)... and redraw... 
// or i could just comptuationally invert the bitmap BG of the text ...  maybe that's better
void ui_button_flip(uint8_t ui_id) {
    // I think just invert the pixels of the text line
    uint8_t fh = 20; // don't know yet, maybe ask the font for this 
    uint16_t start_y = ui_element_y[ui_id] + (ui_element_h[ui_id]/2)-(fh/2);
    display_invert_bg(ui_element_x[ui_id]+1, start_y, ui_element_w[ui_id]-2, fh);
}

void ui_button_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t fgc, uint8_t bc, uint8_t filled) {
    ui_element_active[ui_id]= 1;
    ui_element_x[ui_id] = x;
    ui_element_y[ui_id] = y;
    ui_element_w[ui_id] = w;
    ui_element_h[ui_id] = h;
    if(filled) {
        fillRoundRect(x,y,w,h,r,bc);
    } else {
        drawRoundRect(x,y,w,h,r,bc);
    }
    uint16_t width = 0;
    uint8_t fw, fh;

    // Compute width of text for centering
    for(uint16_t i=0;i<strlen(str);i++) {
        width_height_glyph(str[i], &fw, &fh);
        width += fw;
    }
    uint16_t start_x = x;
    uint16_t start_y = y + ((h+fh)/2);
    if(width < w) {
        start_x = x + (w - width)/2;
    }
    for(uint16_t i=0;i<strlen(str);i++) {
        uint8_t advance = draw_glyph(str[i], start_x,start_y, fgc);
        start_x =start_x + advance;
    }

}


void ui_slider_new(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t o, int16_t min, int16_t max, uint8_t bc, uint8_t hc) {
    // draw two boxes

}

int8_t ui_bounds(uint16_t x, uint16_t y) {
    // find out if x&y are within bounds of any of the active elements, return the ID if so. return -1 if not
    int8_t ui_id = -1;
    for(uint8_t i=0;i<MAX_UI_ELEMENTS;i++) {
        if(ui_element_active[i]) {
            if((x >= ui_element_x[i]) && (y >= ui_element_y[i]) && (x <= ui_element_w[i] + ui_element_x[i]) && (y <= ui_element_y[i] + ui_element_h[i])) {
                return i;
            }
        }
    }
    return ui_id;

}

// problem here: if i hold and mouse up outside of a button it stays lit
void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up) {
    // respond to finger down / up
    if(touch_held && up) { // this is a finger up / click relea
        //fprintf(stderr, "up\n") ;
        touch_held = 0;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) { 
            // We've lifted up on an element. tell the isr
            tulip_ui_isr(ui_id);
            ui_button_flip(ui_id);
        }
    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        //fprintf(stderr, "down hold\n") ;

    } else if(!touch_held && !up) { // this is a new touch down 
        //fprintf(stderr, "down\n") ;

        touch_held = 1;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) {
            ui_button_flip(ui_id);
            // make element active
        }
    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}
