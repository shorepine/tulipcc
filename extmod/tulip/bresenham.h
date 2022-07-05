// bresenham.h
#include "display.h"
#include <math.h>

#define swap(x,y) { x = x + y; y = x - y; x = x - y; }

void plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t pal_idx);
void plot_basic_bezier (int x0, int y0, int x1, int y1, int x2, int y2, uint8_t pal_idx);
void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color);
void tft_drawCircle(short x0, short y0, short r, unsigned short color);
void tft_drawCircleHelper( short x0, short y0, short r, unsigned char cornername, unsigned short color);
void tft_fillCircle(short x0, short y0, short r, unsigned short color) ;
void tft_fillCircleHelper(short x0, short y0, short r, unsigned char cornername, short delta, unsigned short color) ;
void tft_drawLine(short x0, short y0, short x1, short y1,  unsigned short color);
void tft_drawRect(short x, short y, short w, short h, unsigned short color) ;
void tft_drawRoundRect(short x, short y, short w, short h, short r, unsigned short color);
void tft_fillRoundRect(short x, short y, short w, short h, short r, unsigned short color);
void tft_drawTriangle(short x0, short y0, short x1, short y1, short x2, short y2, unsigned short color);
void tft_fillTriangle ( short x0, short y0, short x1, short y1, short x2, short y2, unsigned short color);


