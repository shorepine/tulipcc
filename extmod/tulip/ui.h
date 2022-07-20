// ui.h
#include "display.h"
#include "bresenham.h"
#include "bitmap_fonts.h"
#define MAX_UI_ELEMENTS 32 

uint8_t py_ui_callback;
void tulip_ui_isr(uint8_t ui_id);

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up);
void ui_button_new(uint8_t ui_id, const char * str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t fgc, uint8_t bc, uint8_t filled);
void ui_slider_new(uint8_t ui_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t o, int16_t min, int16_t max, uint8_t bc, uint8_t hc) ;
void ui_element_remove(uint8_t ui_id) ;
