// bresenham.c
// raster line etc drawing functions
#include "bresenham.h"



void tft_drawPixel(int cx, int cy, uint8_t pal_idx) {
    display_set_bg_pixel_pal(cx, cy, pal_idx);
}


void tft_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    tft_drawLine(x, y, x + w - 1, y, color);
}
void tft_drawFastVLine(short x0, short y0, short h, short color) {
    tft_drawLine(x0, y0, x0, y0+h-1, color);
}

void tft_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,  uint16_t color) {
    for (int16_t i = x; i < x + w; i++) {
        tft_drawFastVLine(i, y, h, color);
    }
}

void tft_drawCircle(short x0, short y0, short r, unsigned short color) {
/* Draw a circle outline with center (x0,y0) and radius r, with given color
 * Parameters:
 *      x0: x-coordinate of center of circle. The top-left of the screen
 *          has x-coordinate 0 and increases to the right
 *      y0: y-coordinate of center of circle. The top-left of the screen
 *          has y-coordinate 0 and increases to the bottom
 *      r:  radius of circle
 *      color: 16-bit color value for the circle. Note that the circle
 *          isn't filled. So, this is the color of the outline of the circle
 * Returns: Nothing
 */
  short f = 1 - r;
  short ddF_x = 1;
  short ddF_y = -2 * r;
  short x = 0;
  short y = r;

  tft_drawPixel(x0  , y0+r, color);
  tft_drawPixel(x0  , y0-r, color);
  tft_drawPixel(x0+r, y0  , color);
  tft_drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    tft_drawPixel(x0 + x, y0 + y, color);
    tft_drawPixel(x0 - x, y0 + y, color);
    tft_drawPixel(x0 + x, y0 - y, color);
    tft_drawPixel(x0 - x, y0 - y, color);
    tft_drawPixel(x0 + y, y0 + x, color);
    tft_drawPixel(x0 - y, y0 + x, color);
    tft_drawPixel(x0 + y, y0 - x, color);
    tft_drawPixel(x0 - y, y0 - x, color);
  }
}

void tft_drawCircleHelper( short x0, short y0,
               short r, unsigned char cornername, unsigned short color) {
// Helper function for drawing circles and circular objects
  short f     = 1 - r;
  short ddF_x = 1;
  short ddF_y = -2 * r;
  short x     = 0;
  short y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      tft_drawPixel(x0 + x, y0 + y, color);
      tft_drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      tft_drawPixel(x0 + x, y0 - y, color);
      tft_drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      tft_drawPixel(x0 - y, y0 + x, color);
      tft_drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      tft_drawPixel(x0 - y, y0 - x, color);
      tft_drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void tft_fillCircle(short x0, short y0, short r, unsigned short color) {
/* Draw a filled circle with center (x0,y0) and radius r, with given color
 * Parameters:
 *      x0: x-coordinate of center of circle. The top-left of the screen
 *          has x-coordinate 0 and increases to the right
 *      y0: y-coordinate of center of circle. The top-left of the screen
 *          has y-coordinate 0 and increases to the bottom
 *      r:  radius of circle
 *      color: 16-bit color value for the circle
 * Returns: Nothing
 */
  tft_drawFastVLine(x0, y0-r, 2*r+1, color);
  tft_fillCircleHelper(x0, y0, r, 3, 0, color);
}

void tft_fillCircleHelper(short x0, short y0, short r,
    unsigned char cornername, short delta, unsigned short color) {
// Helper function for drawing filled circles
  short f     = 1 - r;
  short ddF_x = 1;
  short ddF_y = -2 * r;
  short x     = 0;
  short y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      tft_drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      tft_drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      tft_drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      tft_drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void tft_drawLine(short x0, short y0,
                short x1, short y1,
                unsigned short color) {
/* Draw a straight line from (x0,y0) to (x1,y1) with given color
 * Parameters:
 *      x0: x-coordinate of starting point of line. The x-coordinate of
 *          the top-left of the screen is 0. It increases to the right.
 *      y0: y-coordinate of starting point of line. The y-coordinate of
 *          the top-left of the screen is 0. It increases to the bottom.
 *      x1: x-coordinate of ending point of line. The x-coordinate of
 *          the top-left of the screen is 0. It increases to the right.
 *      y1: y-coordinate of ending point of line. The y-coordinate of
 *          the top-left of the screen is 0. It increases to the bottom.
 *      color: 16-bit color value for line
 */
  short steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  short dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  short err = dx / 2;
  short ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      tft_drawPixel(y0, x0, color);
    } else {
      tft_drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void tft_drawRect(short x, short y, short w, short h, unsigned short color) {
/* Draw a rectangle outline with top left vertex (x,y), width w
 * and height h at given color
 * Parameters:
 *      x:  x-coordinate of top-left vertex. The x-coordinate of
 *          the top-left of the screen is 0. It increases to the right.
 *      y:  y-coordinate of top-left vertex. The y-coordinate of
 *          the top-left of the screen is 0. It increases to the bottom.
 *      w:  width of the rectangle
 *      h:  height of the rectangle
 *      color:  16-bit color of the rectangle outline
 * Returns: Nothing
 */
  tft_drawFastHLine(x, y, w, color);
  tft_drawFastHLine(x, y+h-1, w, color);
  tft_drawFastVLine(x, y, h, color);
  tft_drawFastVLine(x+w-1, y, h, color);
}

// Draw a rounded rectangle
void tft_drawRoundRect(short x, short y, short w, short h,
        short r, unsigned short color) {
/* Draw a rounded rectangle outline with top left vertex (x,y), width w,
 * height h and radius of curvature r at given color
 * Parameters:
 *      x:  x-coordinate of top-left vertex. The x-coordinate of
 *          the top-left of the screen is 0. It increases to the right.
 *      y:  y-coordinate of top-left vertex. The y-coordinate of
 *          the top-left of the screen is 0. It increases to the bottom.
 *      w:  width of the rectangle
 *      h:  height of the rectangle
 *      color:  16-bit color of the rectangle outline
 * Returns: Nothing
 */
  // smarter version
  tft_drawFastHLine(x+r  , y    , w-2*r, color); // Top
  tft_drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  tft_drawFastVLine(x    , y+r  , h-2*r, color); // Left
  tft_drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  tft_drawCircleHelper(x+r    , y+r    , r, 1, color);
  tft_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  tft_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  tft_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void tft_fillRoundRect(short x, short y, short w,
                 short h, short r, unsigned short color) {
  // smarter version
  tft_fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  tft_fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  tft_fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void tft_drawTriangle(short x0, short y0,
                short x1, short y1,
                short x2, short y2, unsigned short color) {
/* Draw a triangle outline with vertices (x0,y0),(x1,y1),(x2,y2) with given color
 * Parameters:
 *      x0: x-coordinate of one of the 3 vertices
 *      y0: y-coordinate of one of the 3 vertices
 *      x1: x-coordinate of one of the 3 vertices
 *      y1: y-coordinate of one of the 3 vertices
 *      x2: x-coordinate of one of the 3 vertices
 *      y2: y-coordinate of one of the 3 vertices
 *      color: 16-bit color value for outline
 * Returns: Nothing
 */
  tft_drawLine(x0, y0, x1, y1, color);
  tft_drawLine(x1, y1, x2, y2, color);
  tft_drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void tft_fillTriangle ( short x0, short y0,
                  short x1, short y1,
                  short x2, short y2, unsigned short color) {
/* Draw a filled triangle with vertices (x0,y0),(x1,y1),(x2,y2) with given color
 * Parameters:
 *      x0: x-coordinate of one of the 3 vertices
 *      y0: y-coordinate of one of the 3 vertices
 *      x1: x-coordinate of one of the 3 vertices
 *      y1: y-coordinate of one of the 3 vertices
 *      x2: x-coordinate of one of the 3 vertices
 *      y2: y-coordinate of one of the 3 vertices
 *      color: 16-bit color value
 * Returns: Nothing
 */
  short a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    tft_drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  short
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    tft_drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    tft_drawFastHLine(a, y, b-a+1, color);
  }
}

void plot_basic_bezier (int x0, int y0, int x1, int y1, int x2, int y2, uint8_t pal_idx) {                            
    int sx = x0 < x2 ? 1 : -1;
    int sy = y0 < y2 ? 1 : -1; /* step direction */
    int cur = sx * sy *((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); /* curvature */
    int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 +y2, xy = 2 * x * y * sx * sy;
                                /* compute error increments of P0 */
    long dx = (1 - 2 * abs (x0 - x1)) * y * y + abs (y0 - y1) * xy - 2 * cur * abs (y0 - y2);
    long dy = (1 - 2 * abs (y0 - y1)) * x * x + abs (x0 - x1) * xy + 2 * cur * abs (x0 - x2);
                                /* compute error increments of P2 */
    long ex = (1 - 2 * abs (x2 - x1)) * y * y + abs (y2 - y1) * xy + 2 * cur * abs (y0 - y2);
    long ey = (1 - 2 * abs (y2 - y1)) * x * x + abs (x2 - x1) * xy - 2 * cur * abs (x0 - x2);
                              /* sign of gradient must not change */
    assert ((x0 - x1) * (x2 - x1) <= 0 && (y0 - y1) * (y2 - y1) <= 0); 
    if (cur == 0)   { /* straight line */
        tft_drawLine (x0, y0, x2, y2, pal_idx);
        return;
    }
    x *= 2 * x;
    y *= 2 * y;
    if (cur < 0)  { /* negated curvature */
        x = -x;
        dx = -dx;
        ex = -ex;
        xy = -xy;
        y = -y;
        dy = -dy;
        ey = -ey;
    }
    /* algorithm fails for almost straight line, check error values */
    if (dx >= -y || dy <= -x || ex <= -y || ey >= -x)  {        
        tft_drawLine (x0, y0, x1, y1, pal_idx); /* simple approximation */
        tft_drawLine (x1, y1, x2, y2, pal_idx);
        return;
    }
    dx -= xy;
    ex = dx + dy;
    dy -= xy; /* error of 1.step */
    for (;;) { /* plot curve */
        tft_drawPixel (x0, y0, pal_idx);
        ey = 2 * ex - dy; /* save value for test of y step */
        if (2 * ex >= dx)  { /* x step */
            if (x0 == x2) break;
            x0 += sx;
            dy -= xy;
            ex += dx += y; 
        }
        if (ey <= 0) { /* y step */
            if (y0 == y2) break;
            y0 += sy;
            dx -= xy;
            ex += dy += x; 
        }
    }
}  


