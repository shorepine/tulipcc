// bresenham.h
#ifndef BRESENHAM_H
#define BRESENHAM_H

#include "display.h"
#include <math.h>
#include "u8g2_fonts.h"


#define swap(x,y) { x = x + y; y = x - y; x = x - y; }

void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t pal_idx);
void plot_basic_bezier (int x0, int y0, int x1, int y1, int x2, int y2, uint8_t pal_idx);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color);
void drawCircle(short x0, short y0, short r, unsigned short color);
void drawCircleHelper( short x0, short y0, short r, unsigned char cornername, unsigned short color);
void fillCircle(short x0, short y0, short r, unsigned short color) ;
void fillCircleHelper(short x0, short y0, short r, unsigned char cornername, short delta, unsigned short color) ;
void drawLine(short x0, short y0, short x1, short y1,  unsigned short color);
void drawRect(short x, short y, short w, short h, unsigned short color) ;
void drawRoundRect(short x, short y, short w, short h, short r, unsigned short color);
void fillRoundRect(short x, short y, short w, short h, short r, unsigned short color);
void drawTriangle(short x0, short y0, short x1, short y1, short x2, short y2, unsigned short color);
void fillTriangle ( short x0, short y0, short x1, short y1, short x2, short y2, unsigned short color);
void fill(int16_t x, int16_t y, uint8_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawFastVLine(short x0, short y0, short h, short color);
uint16_t draw_new_str(const char * str, uint16_t x, uint16_t y, uint8_t fg, uint8_t font_no, uint16_t w, uint16_t h, uint8_t centered);
uint16_t draw_new_char(const char c, uint16_t x, uint16_t y, uint8_t fg, uint8_t font_no);

#endif