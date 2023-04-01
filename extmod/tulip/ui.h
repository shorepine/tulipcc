// ui.h
#ifndef UI_H
#define UI_H
#include "polyfills.h"
#include "display.h"
#include "bresenham.h"
#include "bitmap_fonts.h"
#define MAX_UI_ELEMENTS 32

#define UI_NONE 0
#define UI_BUTTON 1
#define UI_SLIDER 2
#define UI_CHECKBOX 3
#define UI_RADIO 4
#define UI_TEXT 5


// TODO - put this in PSRAM instead
struct ui_element {
    uint8_t active;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    float val; 
    char *cval;
    uint8_t c0;
    uint8_t c1;
    uint8_t type;
};

struct ui_element ** elements;

int8_t ui_id_held;


void tulip_ui_isr(uint8_t ui_id);
void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up);
void ui_button_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t fgc, uint8_t bc, uint8_t filled);
void ui_text_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t fgc, uint8_t bc, uint8_t filled);
void ui_slider_new(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t o, float val, uint8_t bc, uint8_t hc);
float ui_slider_get_val(uint8_t ui_id);
char* ui_text_get_val(uint8_t ui_id);
void ui_element_del(uint8_t ui_id) ;
void ui_init();
#endif