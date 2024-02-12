// ui.h
#ifndef UI_H
#define UI_H
#include "polyfills.h"
#include "display.h"
#include "bresenham.h"
#include "keyscan.h"



struct bg_element {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint8_t up;
};


extern struct bg_element ** bg_elements;

uint8_t bg_touch_up(uint8_t ui_id);
void bg_touch_register(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void bg_touch_deregister(uint8_t ui_id);

void tulip_ui_isr(uint8_t ui_id);
void tulip_touch_isr(uint8_t up);
void tulip_bg_touch_isr(uint8_t id);

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up);


#endif