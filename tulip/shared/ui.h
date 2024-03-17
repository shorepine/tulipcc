// ui.h
#ifndef UI_H
#define UI_H
#include "polyfills.h"
#include "display.h"
#include "bresenham.h"
#include "keyscan.h"


void tulip_ui_isr(uint8_t ui_id);
void tulip_touch_isr(uint8_t up);

void send_touch_to_micropython(int16_t touch_x, int16_t touch_y, uint8_t up);


#endif