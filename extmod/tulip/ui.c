// ui.c
// user interface components
#include "ui.h"


// Now, how do i go and change this? I want to higlight it when it's in focus 
// i could save the metadata of the button (really, just str, and radius, and colors)... and redraw... 
// or i could just comptuationally invert the bitmap BG of the text ...  maybe that's better
void ui_button_flip(uint8_t ui_id) {
    struct ui_element *e = elements[ui_id];
    if(e->type == UI_BUTTON) {
        // I think just invert the pixels of the text line
        uint8_t fh = u8g2_a_height(e->c2);
        uint16_t start_y = elements[ui_id]->y + (elements[ui_id]->h/2)-(fh);
        display_invert_bg(elements[ui_id]->x+1, start_y, elements[ui_id]->w-2, fh*2);
    } else if(elements[ui_id]->type == UI_SLIDER) {
        // don't know -- dotted line around the slider? 
    }
}


void ui_element_new(uint8_t ui_id) {
    if(elements[ui_id] == NULL) {
        elements[ui_id] = (struct ui_element*)malloc_caps(sizeof(struct ui_element), MALLOC_CAP_SPIRAM);
        elements[ui_id]->active = 0;
    }
}

void ui_element_active(uint8_t ui_id,  uint8_t active) {
    struct ui_element *e = elements[ui_id];

    e->active = active;
    if(active) {
        // draw 
        if(e->type==UI_BUTTON) ui_button_draw(ui_id);
        if(e->type==UI_SLIDER) ui_slider_draw(ui_id);
        if(e->type==UI_TEXT) ui_text_draw(ui_id,0);
        if(e->type==UI_CHECKBOX) ui_check_draw(ui_id);
    }
}


void ui_element_del(uint8_t ui_id) {
    if(elements[ui_id] == NULL) {
        // nothing?
    } else {
        if(elements[ui_id]->cval != NULL) {
            free_caps(elements[ui_id]->cval);
        }
        free_caps(elements[ui_id]);
        elements[ui_id] = NULL;
    }
}

// when a text button is touched, the OS diverts all key scans to this function
// it stops when enter is hit here.
// if a tap is registered , enter is sent to this function as well
void ui_text_entry_update(uint8_t ui_id, uint8_t ch) {
    struct ui_element *e = elements[ui_id];
    uint8_t len = strlen(e->cval);
    if(ch != 13) {
        if(ch == 8) {
            // backspace, special case
            if(len>0) {
                e->cval[len-1] = 0;
            }
        } else {
            e->cval[len] = ch;
            e->cval[len+1] = 0;
        }
        ui_text_draw(ui_id, 1);
        drawRect(e->x,e->y,e->w,e->h, e->c0);
    } else {
        // Stop 
        ui_text_draw(ui_id, 0);
        keyboard_grab_ui_focus = -1;
        tulip_ui_isr(ui_id);

    }
}
void ui_text_entry_start(uint8_t ui_id) {
    // first, clear out the text inside the button
    struct ui_element *e = elements[ui_id];
    e->cval[0] = 0;
    ui_text_draw(ui_id, 1);
    // now, wait for keys and fill them in
    keyboard_grab_ui_focus = ui_id;
}


void ui_text_draw(uint8_t ui_id, uint8_t entry_mode) {
    struct ui_element *e = elements[ui_id];
    fillRect(e->x,e->y,e->w,e->h,e->c1);

    uint16_t width = 0;
    uint8_t fw, fh;
    if(strlen(e->cval) >0) {
        if(!entry_mode) {
            // Compute width and height of text for centering
            for(uint16_t i=0;i<strlen(e->cval);i++) {
                fw = u8g2_glyph_width(e->c2, e->cval[i]);
                width += fw;
            }
            fh = u8g2_a_height(e->c2);
            uint16_t start_x = e->x;
            uint16_t start_y = e->y + ((e->h+fh)/2);
            if(width < e->w) {
                start_x = e->x + (e->w - width)/2;
            }
            draw_new_str(e->cval, start_x, start_y, e->c0, e->c2);
        } else {
            // If we're in entry mode, fill in chars from the left side, will center later
            fh = u8g2_a_height(e->c2);
            uint16_t start_x = e->x + 1;
            uint16_t start_y = e->y + ((e->h+fh)/2);
            draw_new_str(e->cval, start_x, start_y, e->c0, e->c2);
        }
    }
    if(entry_mode) {
        // Draw a focus handle
        drawRect(e->x,e->y,e->w,e->h, e->c0);
    }
}


void ui_text_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t text_color, uint8_t box_color, uint8_t font_no) {
    ui_element_new(ui_id);
    struct ui_element *e = elements[ui_id];

    e->type = UI_TEXT;
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = h;
    e->c0 = text_color;
    e->c1 = box_color;
    e->c2 = font_no;

    // malloc space for the text. 
    elements[ui_id]->cval = malloc_caps(UI_TEXT_MAX_LEN, MALLOC_CAP_SPIRAM);
    strcpy(elements[ui_id]->cval, str);

}

char* ui_text_get_val(uint8_t ui_id) {
    return elements[ui_id]->cval;
}


void ui_check_draw(uint8_t ui_id) {
    struct ui_element *e = elements[ui_id];
    fillRect(e->x, e->y, e->w, e->w, e->c1);
    if(e->val > 0) {
        drawLine(e->x+1, e->y+1, e->x+e->w-1, e->y+e->w-1, e->c0);
        drawLine(e->x+e->w-1, e->y+1, e->x+1, e->y+e->w-1, e->c0);
    }
}
uint8_t ui_check_get_val(uint8_t ui_id) {
    return (uint8_t)elements[ui_id]->val;
}
void ui_check_set_val(uint8_t ui_id, uint8_t v) {
    elements[ui_id]->val = v;
    ui_check_draw(ui_id);
}

void ui_check_new(uint8_t ui_id,uint8_t val, uint16_t x, uint16_t y, uint16_t w, uint8_t x_color,uint8_t box_color) {
    ui_element_new(ui_id);
    struct ui_element *e = elements[ui_id];
    e->type = UI_CHECKBOX;
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = w;
    e->c0 = x_color;
    e->c1 = box_color;
    e->val = val;
}

void ui_button_draw(uint8_t ui_id) {
    struct ui_element *e = elements[ui_id];

    if((uint8_t)e->val) {
        fillRoundRect(e->x,e->y,e->w,e->h,10,e->c1);
    } else {
        drawRoundRect(e->x,e->y,e->w,e->h,10,e->c1);
    }
    uint16_t width = 0;
    uint8_t fw, fh;

    // Compute width of text for centering
    for(uint16_t i=0;i<strlen(e->cval);i++) {
        fw = u8g2_glyph_width(e->c2, e->cval[i]);
        width += fw;
    }
    fh = u8g2_a_height(e->c2);
    uint16_t start_x = e->x;
    uint16_t start_y = e->y + ((e->h+fh)/2);
    if(width < e->w) {
        start_x = e->x + (e->w - width)/2;
    }
    // This delay is v weird. I need to figure out why it's necessary. 
    delay_ms(50);
    draw_new_str(e->cval, start_x, start_y, e->c0, e->c2);
    //delay_ms(10);
}


void ui_button_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t fgc, uint8_t bc, uint8_t filled, uint8_t font_no) {
    ui_element_new(ui_id);
    struct ui_element *e = elements[ui_id];
    e->type = UI_BUTTON;
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = h;
    e->c0 = fgc;
    e->c1 = bc;
    e->val = filled;
    e->c2 = font_no;
    e->cval = malloc_caps(UI_TEXT_MAX_LEN, MALLOC_CAP_SPIRAM);
    strcpy(e->cval, str);

}

void ui_slider_draw(uint8_t ui_id) {
    // draw two boxes
    struct ui_element *e = elements[ui_id];
    if(e->w > e->h) { // horizontal slider
        uint16_t slider_width = 20;
        // Make the frame
        fillRect(e->x,e->y,e->w,e->h,e->c0);
        // Compute position of slider
        uint16_t slider_pos_x_relative = (uint16_t) ((e->val) * (float)(e->w-slider_width));
        // Make the handle
        fillRect(e->x+slider_pos_x_relative,e->y, slider_width, e->h, e->c1);
    }  else {
        uint16_t slider_height = 20;
        // Make the frame
        fillRect(e->x,e->y,e->w,e->h,e->c0);
        // Compute position of slider
        uint16_t slider_pos_y_relative = (uint16_t) ((1.0 - e->val) * (float)(e->h-slider_height));
        // Make the handle
        fillRect(e->x, e->y+slider_pos_y_relative, e->w, slider_height, e->c1);
    }
}

void ui_slider_set_val(uint8_t ui_id, float val) {
    struct ui_element *e = elements[ui_id];
    e->val = val;
    ui_slider_draw(ui_id);
}

void ui_slider_set_val_xy(uint8_t ui_id, uint16_t x, uint16_t y) {
    // given an x and y, update the val, then draw
    struct ui_element *e = elements[ui_id];
    if(e->w > e->h) { // horizontal slider
        e->val = (((float)(x-e->x) / (float)(e->w)));
    } else { // vertical slider
        e->val = (1.0 - ((float)(y-e->y) / (float)(e->h)));
    }
    ui_slider_draw(ui_id);
}

float ui_slider_get_val(uint8_t ui_id) {
    return elements[ui_id]->val;
}

void ui_slider_new(uint8_t ui_id, float val, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t bc, uint8_t hc) {
    ui_element_new(ui_id);
    struct ui_element *e = elements[ui_id];
    e->type = UI_SLIDER;
    e->x = x;
    e->y = y;
    e->w = w;
    e->h = h;
    e->val = val;
    e->c0 = bc;
    e->c1 = hc;
}

int8_t ui_bounds(uint16_t x, uint16_t y) {
    // find out if x&y are within bounds of any of the active elements, return the ID if so. return -1 if not
    int8_t ui_id = -1;
    for(uint8_t i=0;i<MAX_UI_ELEMENTS;i++) {
        struct ui_element * e = elements[i];
        if(e != NULL) {
            if(e->active) {
                if((x >= e->x) && (y >= e->y) && (x <= e->w + e->x) && (y <= e->y + e->h)) {
                    return i;
                }
            }
        }
    }
    return ui_id;

}

void ui_init() {
    elements = (struct ui_element **) malloc_caps(sizeof(struct ui_element*) * MAX_UI_ELEMENTS, MALLOC_CAP_SPIRAM);
    for(uint8_t i=0;i<MAX_UI_ELEMENTS;i++) {
        elements[i] = NULL;
    }
    ui_id_held = -1;
}


// for text inputs:
// a click inside an editing box should confirm the edit -- same as hitting enter
// a click outside an editing box should confirm the edit -- same as hitting enter
// a click to another box should start editing that box and confirm the edit on the old one

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up) {
    // respond to finger down / up
    if(touch_held && up) { // this is a finger up / click release
        // If there's any text entry happening, in all cases, a touch up stops it
        if(keyboard_grab_ui_focus > -1) {
            ui_text_entry_update(keyboard_grab_ui_focus, 13);
        }
        touch_held = 0;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) { 
            // Is this a text input?
            if(elements[ui_id]->type == UI_TEXT) {
                // start taking in text input to replace the text of the button
                ui_text_entry_start(ui_id);
            } else if(elements[ui_id]->type == UI_BUTTON || elements[ui_id]->type == UI_SLIDER) {
                // We've lifted up on an element. tell the isr
                tulip_ui_isr(ui_id);
                ui_button_flip(ui_id);
            }
        } else { // no element was touched on up
            // In case the pointer moved out of bounds before going up
            if(ui_id_held >= 0) {
                ui_button_flip(ui_id_held);
            }
        }
        tulip_touch_isr(up);
        ui_id_held = -1;
    } else if(touch_held && !up) { // this is a continuous hold -- update sliders, etc 
        //fprintf(stderr, "down hold\n") ;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) {
            if(elements[ui_id]->type == UI_SLIDER) {
                ui_slider_set_val_xy(ui_id, touch_x, touch_y);
                tulip_ui_isr(ui_id);
            }
        }
        tulip_touch_isr(up);
    } else if(!touch_held && !up) { // this is a new touch down 
        touch_held = 1;
        int8_t ui_id = ui_bounds(touch_x, touch_y);
        if(ui_id >= 0) {
            ui_button_flip(ui_id);
            if(elements[ui_id]->type == UI_SLIDER) {
                ui_slider_set_val_xy(ui_id, touch_x, touch_y);
            } else if(elements[ui_id]->type == UI_CHECKBOX) {
                if((uint8_t)elements[ui_id]->val) {
                    elements[ui_id]->val = 0;
                } else {
                    elements[ui_id]->val = 1;
                }
                ui_check_draw(ui_id);
                tulip_ui_isr(ui_id);
            }
            ui_id_held = ui_id;
            // make element active
        }
        tulip_touch_isr(up);
    } else if(!touch_held && up) { // just moving the mouse around on desktop 
        //fprintf(stderr, "touch not held and up event\n");
    }
}
