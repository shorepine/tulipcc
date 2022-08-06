// ui.c
// user interface components
#include "ui.h"


// Now, how do i go and change this? I want to higlight it when it's in focus 
// i could save the metadata of the button (really, just str, and radius, and colors)... and redraw... 
// or i could just comptuationally invert the bitmap BG of the text ...  maybe that's better
void ui_button_flip(uint8_t ui_id) {
    if(elements[ui_id]->type == UI_BUTTON) {
        // I think just invert the pixels of the text line
        uint8_t fh = 0;
        // Just get the height of the font
        width_height_glyph('Q', &fh, &fh);
        uint16_t start_y = elements[ui_id]->y + (elements[ui_id]->h/2)-(fh);
        display_invert_bg(elements[ui_id]->x+1, start_y, elements[ui_id]->w-2, fh*2);
    } else if(elements[ui_id]->type == UI_SLIDER) {
        // don't know -- dotted line around the slider? 
    }
}

void ui_element_new(uint8_t ui_id) {
    if(elements[ui_id] == NULL) {
        elements[ui_id] = (struct ui_element*)malloc_caps(sizeof(struct ui_element), MALLOC_CAP_INTERNAL);
    }
}

void ui_element_del(uint8_t ui_id) {
    if(elements[ui_id] == NULL) {
        // nothing?
    } else {
        free_caps(elements[ui_id]);
        elements[ui_id] = NULL;
    }
}


void ui_button_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t fgc, uint8_t bc, uint8_t filled) {
    ui_element_new(ui_id);
    elements[ui_id]->type = UI_BUTTON;
    elements[ui_id]->x = x;
    elements[ui_id]->y = y;
    elements[ui_id]->w = w;
    elements[ui_id]->h = h;
    elements[ui_id]->c0 = fgc;
    elements[ui_id]->c1 = bc;

    // Buttons don't need a separate draw meth
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

void ui_slider_draw(uint8_t ui_id) {
    // draw two boxes
    uint16_t slider_height = 20;
    struct ui_element * e = elements[ui_id];
    // Make the frame
    fillRect(e->x,e->y,e->w,e->h,e->c0);
    // Compute position of slider
    // val should come in 0-1 float, which is all we need to set position
    // Make the handle
    uint16_t slider_pos_relative = (uint16_t) ((1.0 - e->val) * (float)(e->h-slider_height));

    fillRect(e->x, e->y+slider_pos_relative, e->w, slider_height, e->c1);
}
void ui_slider_set_val(uint8_t ui_id, uint16_t x, uint16_t y) {
    // given an x and y, update the val, then draw
    // just y for now
    struct ui_element *e = elements[ui_id];
    e->val = (1.0 - ((float)(y-e->y) / (float)(e->h)));
    ui_slider_draw(ui_id);
}

float ui_slider_get_val(uint8_t ui_id) {
    return elements[ui_id]->val;
}

void ui_slider_new(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t o, float val, uint8_t bc, uint8_t hc) {
    ui_element_new(ui_id);
    elements[ui_id]->type = UI_SLIDER;
    elements[ui_id]->x = x;
    elements[ui_id]->y = y;
    elements[ui_id]->w = w;
    elements[ui_id]->h = h;
    elements[ui_id]->val = val;
    elements[ui_id]->c0 = bc;
    elements[ui_id]->c1 = hc;
    ui_slider_draw(ui_id);
}

int8_t ui_bounds(uint16_t x, uint16_t y) {
    // find out if x&y are within bounds of any of the active elements, return the ID if so. return -1 if not
    int8_t ui_id = -1;
    for(uint8_t i=0;i<MAX_UI_ELEMENTS;i++) {
        struct ui_element * e = elements[i];
        if(e != NULL) {
            if((x >= e->x) && (y >= e->y) && (x <= e->w + e->x) && (y <= e->y + e->h)) {
                return i;
            }
        }
    }
    return ui_id;

}

void ui_init() {
    elements = (struct ui_element **) malloc_caps(sizeof(struct ui_element*) * MAX_UI_ELEMENTS, MALLOC_CAP_INTERNAL);
    for(uint8_t i=0;i<MAX_UI_ELEMENTS;i++) {
        elements[i] = NULL;
    }
    ui_id_held = -1;
}


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
        } else {
            // In case the pointer moved out of bounds before going up
            if(ui_id_held >= 0) {
                ui_button_flip(ui_id_held);
            }
        }
        ui_id_held = -1;
    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        //fprintf(stderr, "down hold\n") ;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) {
            if(elements[ui_id]->type == UI_SLIDER) {
                ui_slider_set_val(ui_id, touch_x, touch_y);
                tulip_ui_isr(ui_id);
            }
        }

    } else if(!touch_held && !up) { // this is a new touch down 
        //fprintf(stderr, "down\n") ;
        touch_held = 1;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) {
            ui_button_flip(ui_id);
            if(elements[ui_id]->type == UI_SLIDER) {
                ui_slider_set_val(ui_id, touch_x, touch_y);
            }
            ui_id_held = ui_id;
            // make element active
        }
    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}
