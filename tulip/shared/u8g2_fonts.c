// u8g2_fonts.c

/*

  U8g2_for_Adafruit_GFX.cpp
  
  Add unicode support and U8g2 fonts to Adafruit GFX libraries.
  
  U8g2 for Adafruit GFX Lib (https://github.com/olikraus/U8g2_for_Adafruit_GFX)

  Copyright (c) 2018, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

//#include <Adafruit_GFX.h>
#include "u8g2_fonts.h"

// HOW TO ADD NEW FONTS HERE
// clone this repo https://github.com/olikraus/U8g2_for_Adafruit_GFX
// choose the fonts you want from https://github.com/olikraus/u8g2/wiki/fntlistall
// copy the font data from https://github.com/olikraus/U8g2_for_Adafruit_GFX/blob/master/src/u8g2_fonts.c to our u8fontdata.c
// add the font name here
// add the font pointer to the bottom of u8g2_fonts.h

const uint8_t*tulip_fonts[MAX_TULIP_FONTS] = {
    u8g2_font_t0_22_tr,
    u8g2_font_t0_22b_tr,
    u8g2_font_crox4t_tr,
    u8g2_font_lubI12_tr,
    u8g2_font_calibration_gothic_nbp_tr,
    u8g2_font_helvB14_tr,
    u8g2_font_helvR14_tr,
    u8g2_font_logisoso16_tr,
    u8g2_font_6x13_tr,
    u8g2_font_8x13_tr,
    u8g2_font_profont15_tr,
    u8g2_font_crox1h_tr,
    u8g2_font_fewture_tr,
    u8g2_font_helvR12_tr,
    u8g2_font_luRS10_tr,
    u8g2_font_luRS18_tr,
    u8g2_font_osb18_tr,
    u8g2_font_logisoso24_tr,
    u8g2_font_lubB24_tr
};

//========================================================



static uint8_t u8g2_font_get_byte(const uint8_t *font, uint8_t offset)
{
  font += offset;
  return u8x8_pgm_read( font );  
}

static uint16_t u8g2_font_get_word(const uint8_t *font, uint8_t offset) U8X8_NOINLINE;
static uint16_t u8g2_font_get_word(const uint8_t *font, uint8_t offset)
{
    uint16_t pos;
    font += offset;
    pos = u8x8_pgm_read( font );
    font++;
    pos <<= 8;
    pos += u8x8_pgm_read( font);
    return pos;
}

/*========================================================================*/
/* new font format */
void u8g2_read_font_info(u8g2_font_info_t *font_info, const uint8_t *font)
{
  /* offset 0 */
  font_info->glyph_cnt = u8g2_font_get_byte(font, 0);
  font_info->bbx_mode = u8g2_font_get_byte(font, 1);
  font_info->bits_per_0 = u8g2_font_get_byte(font, 2);
  font_info->bits_per_1 = u8g2_font_get_byte(font, 3);
  
  /* offset 4 */
  font_info->bits_per_char_width = u8g2_font_get_byte(font, 4);
  font_info->bits_per_char_height = u8g2_font_get_byte(font, 5);
  font_info->bits_per_char_x = u8g2_font_get_byte(font, 6);
  font_info->bits_per_char_y = u8g2_font_get_byte(font, 7);
  font_info->bits_per_delta_x = u8g2_font_get_byte(font, 8);
  
  /* offset 9 */
  font_info->max_char_width = u8g2_font_get_byte(font, 9);
  font_info->max_char_height = u8g2_font_get_byte(font, 10);
  font_info->x_offset = u8g2_font_get_byte(font, 11);
  font_info->y_offset = u8g2_font_get_byte(font, 12);
  
  /* offset 13 */
  font_info->ascent_A = u8g2_font_get_byte(font, 13);
  font_info->descent_g = u8g2_font_get_byte(font, 14);
  font_info->ascent_para = u8g2_font_get_byte(font, 15);
  font_info->descent_para = u8g2_font_get_byte(font, 16);
  
  /* offset 17 */
  font_info->start_pos_upper_A = u8g2_font_get_word(font, 17);
  font_info->start_pos_lower_a = u8g2_font_get_word(font, 19); 
  
  /* offset 21 */
  font_info->start_pos_unicode = u8g2_font_get_word(font, 21); 
}

uint8_t u8g2_GetFontBBXWidth(u8g2_font_t *u8g2)
{
  return u8g2->font_info.max_char_width;    /* new font info structure */
}

uint8_t u8g2_GetFontBBXHeight(u8g2_font_t *u8g2)
{
  return u8g2->font_info.max_char_height;   /* new font info structure */
}

int8_t u8g2_GetFontBBXOffX(u8g2_font_t *u8g2)
{
  return u8g2->font_info.x_offset;    /* new font info structure */
}

int8_t u8g2_GetFontBBXOffY(u8g2_font_t *u8g2)
{
  return u8g2->font_info.y_offset;    /* new font info structure */
}


uint8_t u8g2_GetFontCapitalAHeight(u8g2_font_t *u8g2)
{
  return u8g2->font_info.ascent_A;    /* new font info structure */
}

uint8_t u8g2_a_height(uint8_t font_no) {
    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);
    uint8_t height = u8g2_GetFontCapitalAHeight(&ufont);
    return height;
}

uint8_t u8g2_glyph_width(uint8_t font_no, uint16_t glyph) {
    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    //fprintf(stderr, "getting width for %d %c\n", font_no, glyph);
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);
    uint8_t width = u8g2_GetGlyphWidth(&ufont, glyph);
    return width;
}

uint8_t u8g2_glyph_height(uint8_t font_no, uint16_t glyph) {
    u8g2_font_t ufont;
    ufont.font = NULL; 
    ufont.font_decode.fg_color = 1; 
    ufont.font_decode.is_transparent = 1; 
    ufont.font_decode.dir = 0;
    u8g2_SetFont(&ufont, tulip_fonts[font_no]);
    uint8_t height = u8g2_GetGlyphHeight(&ufont, glyph);
    return height;
}


static uint8_t u8g2_font_decode_get_unsigned_bits(u8g2_font_decode_t *f, uint8_t cnt) U8X8_NOINLINE;
static uint8_t u8g2_font_decode_get_unsigned_bits(u8g2_font_decode_t *f, uint8_t cnt) 
{
  uint8_t val;
  uint8_t bit_pos = f->decode_bit_pos;
  uint8_t bit_pos_plus_cnt;
  
  //val = *(f->decode_ptr);
  val = u8x8_pgm_read( f->decode_ptr );  
  
  val >>= bit_pos;
  bit_pos_plus_cnt = bit_pos;
  bit_pos_plus_cnt += cnt;
  if ( bit_pos_plus_cnt >= 8 )
  {
    uint8_t s = 8;
    s -= bit_pos;
    f->decode_ptr++;
    //val |= *(f->decode_ptr) << (8-bit_pos);
    val |= u8x8_pgm_read( f->decode_ptr ) << (s);
    //bit_pos -= 8;
    bit_pos_plus_cnt -= 8;
  }
  val &= (1U<<cnt)-1;
  //bit_pos += cnt;
  
  f->decode_bit_pos = bit_pos_plus_cnt;
  return val;
}


/*
    2 bit --> cnt = 2
      -2,-1,0. 1

    3 bit --> cnt = 3
      -2,-1,0. 1
      -4,-3,-2,-1,0,1,2,3

      if ( x < 0 )
  r = bits(x-1)+1;
    else
  r = bits(x)+1;

*/
/* optimized */
static int8_t u8g2_font_decode_get_signed_bits(u8g2_font_decode_t *f, uint8_t cnt) U8X8_NOINLINE;
static int8_t u8g2_font_decode_get_signed_bits(u8g2_font_decode_t *f, uint8_t cnt)
{
  int8_t v, d;
  v = (int8_t)u8g2_font_decode_get_unsigned_bits(f, cnt);
  d = 1;
  cnt--;
  d <<= cnt;
  v -= d;
  return v;
  //return (int8_t)u8g2_font_decode_get_unsigned_bits(f, cnt) - ((1<<cnt)>>1);
}


static int16_t u8g2_add_vector_y(int16_t dy, int8_t x, int8_t y, uint8_t dir) U8X8_NOINLINE;
static int16_t u8g2_add_vector_y(int16_t dy, int8_t x, int8_t y, uint8_t dir)
{
  switch(dir)
  {
    case 0:
      dy += y;
      break;
    case 1:
      dy += x;
      break;
    case 2:
      dy -= y;
      break;
    default:
      dy -= x;
      break;      
  }
  return dy;
}

static int16_t u8g2_add_vector_x(int16_t dx, int8_t x, int8_t y, uint8_t dir) U8X8_NOINLINE;
static int16_t u8g2_add_vector_x(int16_t dx, int8_t x, int8_t y, uint8_t dir)
{
  switch(dir)
  {
    case 0:
      dx += x;
      break;
    case 1:
      dx -= y;
      break;
    case 2:
      dx -= x;
      break;
    default:
      dx += y;
      break;      
  }
  return dx;
}



void drawLine_target(short x0, short y0,short x1, short y1, uint8_t *target, uint16_t target_width, uint16_t target_height) {
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
    if (steep) { // swap x and y. 
        target[y0 + 1 + (((target_height/2)+x0)*target_width)] = 0xff;
    } else {
        target[x0 + 1 + (((target_height/2)+y0)*target_width)] = 0xff;
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawFastHLine_target(int16_t x, int16_t y, int16_t w, uint8_t*target, uint16_t target_width, uint16_t target_height) {
    //fprintf(stderr, "draw H line x %d y %d w %d tw %d th %d\n", x,y,w,target_width, target_height);
    drawLine_target(x, y, x + w - 1, y, target, target_width, target_height);
}
void drawFastVLine_target(short x0, short y0, short h, uint8_t*target, uint16_t target_width, uint16_t target_height) {
    fprintf(stderr, "draw V line x0 %d y0 %d h %d tw %d th %d\n", x0,y0,h,target_width, target_height);
    drawLine_target(x0, y0, x0, y0+h-1, target, target_width, target_height);
}


void u8g2_draw_hv_line(u8g2_font_t *u8g2, int16_t x, int16_t y, int16_t len, uint8_t dir, uint16_t color) U8X8_NOINLINE;
void u8g2_draw_hv_line(u8g2_font_t *u8g2, int16_t x, int16_t y, int16_t len, uint8_t dir, uint16_t color)
{
  switch(dir)
  {
    case 0:
      drawFastHLine(x,y,len,color);
      break;
    case 1:
      drawFastVLine(x,y,len,color);
      break;
    case 2:
      drawFastHLine(x-len+1,y,len,color);
      break;
    case 3:
      drawFastVLine(x,y-len+1,len,color);
      break;
  }
  
}

void u8g2_draw_hv_line_target(u8g2_font_t *u8g2, int16_t x, int16_t y, int16_t len, uint8_t dir, uint8_t * target,uint16_t target_width, uint16_t target_height) U8X8_NOINLINE;
void u8g2_draw_hv_line_target(u8g2_font_t *u8g2, int16_t x, int16_t y, int16_t len, uint8_t dir, uint8_t * target,uint16_t target_width, uint16_t target_height)
{
  switch(dir)
  {
    case 0:
      drawFastHLine_target(x,y,len,target,target_width, target_height);
      break;
    case 1:
      drawFastVLine_target(x,y,len,target, target_width, target_height);
      break;
    case 2:
      drawFastHLine_target(x-len+1,y,len,target, target_width, target_height);
      break;
    case 3:
      drawFastVLine_target(x,y-len+1,len,target, target_width, target_height);
      break;
  }
  
}


/*
  Description:
    Draw a run-length area of the glyph. "len" can have any size and the line
    length has to be wrapped at the glyph border.
  Args:
    len:          Length of the line
    is_foreground     foreground/background?
    u8g2->font_decode.target_x    X position
    u8g2->font_decode.target_y    Y position
    u8g2->font_decode.is_transparent  Transparent mode
  Return:
    -
  Calls:
    u8g2_Draw90Line()
  Called by:
    u8g2_font_decode_glyph()
*/
/* optimized */
static void u8g2_font_decode_len(u8g2_font_t *u8g2, uint8_t len, uint8_t is_foreground)
{
  uint8_t cnt;  /* total number of remaining pixels, which have to be drawn */
  uint8_t rem;  /* remaining pixel to the right edge of the glyph */
  uint8_t current;  /* number of pixels, which need to be drawn for the draw procedure */
    /* current is either equal to cnt or equal to rem */
  
  /* local coordinates of the glyph */
  uint8_t lx,ly;
  
  /* target position on the screen */
  int16_t x, y;
  
  u8g2_font_decode_t *decode = &(u8g2->font_decode);
  
  cnt = len;
  
  /* get the local position */
  lx = decode->x;
  ly = decode->y;
  
  for(;;)
  {
    /* calculate the number of pixel to the right edge of the glyph */
    rem = decode->glyph_width;
    rem -= lx;
    
    /* calculate how many pixel to draw. This is either to the right edge */
    /* or lesser, if not enough pixel are left */
    current = rem;
    if ( cnt < rem )
      current = cnt;
    
    
    /* now draw the line, but apply the rotation around the glyph target position */
    //u8g2_font_decode_draw_pixel(u8g2, lx,ly,current, is_foreground);

    /* get target position */
    x = decode->target_x;
    y = decode->target_y;

    /* apply rotation */
    x = u8g2_add_vector_x(x, lx, ly, decode->dir);
    y = u8g2_add_vector_y(y, lx, ly, decode->dir);
    
    /* draw foreground and background (if required) */
    if ( current > 0 )      /* avoid drawing zero length lines, issue #4 */
    {
      if ( is_foreground )
      {
    u8g2_draw_hv_line(u8g2, x, y, current, decode->dir, decode->fg_color);
      }
      else if ( decode->is_transparent == 0 )    
      {
    u8g2_draw_hv_line(u8g2, x, y, current, decode->dir, decode->bg_color);
      }
    }
    
    /* check, whether the end of the run length code has been reached */
    if ( cnt < rem )
      break;
    cnt -= rem;
    lx = 0;
    ly++;
  }
  lx += cnt;
  
  decode->x = lx;
  decode->y = ly;
  
}



static void u8g2_font_decode_len_target(u8g2_font_t *u8g2, uint8_t len, uint8_t is_foreground, uint8_t * target)
{
  uint8_t cnt;  /* total number of remaining pixels, which have to be drawn */
  uint8_t rem;  /* remaining pixel to the right edge of the glyph */
  uint8_t current;  /* number of pixels, which need to be drawn for the draw procedure */
    /* current is either equal to cnt or equal to rem */
  
  /* local coordinates of the glyph */
  uint8_t lx,ly;
  
  /* target position on the screen */
  int16_t x, y;
  
  u8g2_font_decode_t *decode = &(u8g2->font_decode);
  
  cnt = len;
  
  /* get the local position */
  lx = decode->x;
  ly = decode->y;
  
  for(;;)
  {
    /* calculate the number of pixel to the right edge of the glyph */
    rem = decode->glyph_width;
    rem -= lx;
    
    /* calculate how many pixel to draw. This is either to the right edge */
    /* or lesser, if not enough pixel are left */
    current = rem;
    if ( cnt < rem )
      current = cnt;
    
    
    /* now draw the line, but apply the rotation around the glyph target position */
    //u8g2_font_decode_draw_pixel(u8g2, lx,ly,current, is_foreground);

    /* get target position */
    x = decode->target_x;
    y = decode->target_y;

    /* apply rotation */
    x = u8g2_add_vector_x(x, lx, ly, decode->dir);
    y = u8g2_add_vector_y(y, lx, ly, decode->dir);
    
    /* draw foreground and background (if required) */
    if ( current > 0 )      /* avoid drawing zero length lines, issue #4 */
      {
        if ( is_foreground )
          {
            uint16_t target_width = (decode->glyph_width+ abs(decode->target_x))*2 ;
            uint16_t target_height = decode->glyph_height*2;
            u8g2_draw_hv_line_target(u8g2, x, y, current, decode->dir, target, target_width, target_height);
          }
      }    
    /* check, whether the end of the run length code has been reached */
    if ( cnt < rem )
      break;
    cnt -= rem;
    lx = 0;
    ly++;
  }
  lx += cnt;
  
  decode->x = lx;
  decode->y = ly;
  
}
static void u8g2_font_setup_decode(u8g2_font_t *u8g2, const uint8_t *glyph_data)
{
  u8g2_font_decode_t *decode = &(u8g2->font_decode);
  decode->decode_ptr = glyph_data;
  decode->decode_bit_pos = 0;
  
  /* 8 Nov 2015, this is already done in the glyph data search procedure */
  /*
  decode->decode_ptr += 1;
  decode->decode_ptr += 1;
  */
  
  decode->glyph_width = u8g2_font_decode_get_unsigned_bits(decode, u8g2->font_info.bits_per_char_width);
  decode->glyph_height = u8g2_font_decode_get_unsigned_bits(decode,u8g2->font_info.bits_per_char_height);
  
}


/*
  Description:
    Decode and draw a glyph.
  Args:
    glyph_data:           Pointer to the compressed glyph data of the font
    u8g2->font_decode.target_x    X position
    u8g2->font_decode.target_y    Y position
    u8g2->font_decode.is_transparent  Transparent mode
  Return:
    Width (delta x advance) of the glyph.
  Calls:
    u8g2_font_decode_len()
*/
/* optimized */
static int8_t u8g2_font_decode_glyph(u8g2_font_t *u8g2, const uint8_t *glyph_data)
{
  uint8_t a, b;
  int8_t x, y;
  int8_t d;
  int8_t h;
  u8g2_font_decode_t *decode = &(u8g2->font_decode);
    
  u8g2_font_setup_decode(u8g2, glyph_data);
  h = u8g2->font_decode.glyph_height;
  
  x = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_char_x);
  y = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_char_y);
  d = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_delta_x);

  
  if ( decode->glyph_width > 0 )
  {
    decode->target_x = u8g2_add_vector_x(decode->target_x, x, -(h+y), decode->dir);
    decode->target_y = u8g2_add_vector_y(decode->target_y, x, -(h+y), decode->dir);
    //u8g2_add_vector(&(decode->target_x), &(decode->target_y), x, -(h+y), decode->dir);

   
    /* reset local x/y position */
    decode->x = 0;
    decode->y = 0;
    
    /* decode glyph */
    for(;;)
    {
      a = u8g2_font_decode_get_unsigned_bits(decode, u8g2->font_info.bits_per_0);
      b = u8g2_font_decode_get_unsigned_bits(decode, u8g2->font_info.bits_per_1);
      do
      {
        u8g2_font_decode_len(u8g2, a, 0);
        u8g2_font_decode_len(u8g2, b, 1);
      } while( u8g2_font_decode_get_unsigned_bits(decode, 1) != 0 );

      if ( decode->y >= h )
        break;
    }
    
  }
  return d;
}

static int8_t u8g2_font_decode_glyph_target(u8g2_font_t *u8g2, const uint8_t *glyph_data, uint8_t * target)
{
  uint8_t a, b;
  int8_t x, y;
  int8_t d;
  int8_t h;
  u8g2_font_decode_t *decode = &(u8g2->font_decode);
    
  u8g2_font_setup_decode(u8g2, glyph_data);
  h = u8g2->font_decode.glyph_height;
  
  x = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_char_x);
  y = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_char_y);
  d = u8g2_font_decode_get_signed_bits(decode, u8g2->font_info.bits_per_delta_x);

  
  if ( decode->glyph_width > 0 )
  {
    decode->target_x = u8g2_add_vector_x(decode->target_x, x, -(h+y), decode->dir);
    decode->target_y = u8g2_add_vector_y(decode->target_y, x, -(h+y), decode->dir);
    //u8g2_add_vector(&(decode->target_x), &(decode->target_y), x, -(h+y), decode->dir);

   
    /* reset local x/y position */
    decode->x = 0;
    decode->y = 0;
    
    /* decode glyph */
    for(;;)
    {
      a = u8g2_font_decode_get_unsigned_bits(decode, u8g2->font_info.bits_per_0);
      b = u8g2_font_decode_get_unsigned_bits(decode, u8g2->font_info.bits_per_1);
      do
      {
        u8g2_font_decode_len_target(u8g2, a, 0, target);
        u8g2_font_decode_len_target(u8g2, b, 1, target);
      } while( u8g2_font_decode_get_unsigned_bits(decode, 1) != 0 );

      if ( decode->y >= h )
        break;
    }
    
  }
  return d;
}

/*
  Description:
    Find the starting point of the glyph data.
  Args:
    encoding: Encoding (ASCII or Unicode) of the glyph
  Return:
    Address of the glyph data or NULL, if the encoding is not avialable in the font.
*/
const uint8_t *u8g2_font_get_glyph_data(u8g2_font_t *u8g2, uint16_t encoding)
{
  const uint8_t *font = u8g2->font;
  font += 23;

  
  if ( encoding <= 255 )
  {
    if ( encoding >= 'a' )
    {
      font += u8g2->font_info.start_pos_lower_a;
    }
    else if ( encoding >= 'A' )
    {
      font += u8g2->font_info.start_pos_upper_A;
    }
    
    for(;;)
    {
      if ( u8x8_pgm_read( font + 1 ) == 0 )
        break;
      if ( u8x8_pgm_read( font ) == encoding )
      {
        return font+2;  /* skip encoding and glyph size */
      }
      font += u8x8_pgm_read( font + 1 );
    }
  }
  else
  {
    uint16_t e;
    const uint8_t *unicode_lookup_table;
    /* support for the new unicode lookup table */
    
    font += u8g2->font_info.start_pos_unicode;
    unicode_lookup_table = font; 
  
    /* u8g2 issue 596: search for the glyph start in the unicode lookup table */
    do
    {
      font += u8g2_font_get_word(unicode_lookup_table, 0);
      e = u8g2_font_get_word(unicode_lookup_table, 2);
      unicode_lookup_table+=4;
    } while( e < encoding );
    
    /* variable "font" is now updated according to the lookup table */
    
    for(;;)
    {
      e = u8x8_pgm_read( font );
      e <<= 8;
      e |= u8x8_pgm_read( font + 1 );
      if ( e == 0 )
         break;
      if ( e == encoding )
      {
        return font+3;  /* skip encoding and glyph size */
      }
      font += u8x8_pgm_read( font + 2 );
    }  
  }  
  return NULL;
}

static int16_t u8g2_font_draw_glyph(u8g2_font_t *u8g2, int16_t x, int16_t y, uint16_t encoding)
{
  int16_t dx = 0;
  u8g2->font_decode.target_x = x;
  u8g2->font_decode.target_y = y;
  //u8g2->font_decode.is_transparent = is_transparent; this is already set
  //u8g2->font_decode.dir = dir;
  const uint8_t *glyph_data = u8g2_font_get_glyph_data(u8g2, encoding);
  if ( glyph_data != NULL )
  {
    dx = u8g2_font_decode_glyph(u8g2, glyph_data);
  }
  return dx;
}

static int16_t u8g2_font_draw_glyph_target(u8g2_font_t *u8g2, uint16_t encoding, uint8_t * target)
{
  int16_t dx = 0;
  u8g2->font_decode.target_x = 0;
  u8g2->font_decode.target_y = 0;

  const uint8_t *glyph_data = u8g2_font_get_glyph_data(u8g2, encoding);
  if ( glyph_data != NULL )
  {
    dx = u8g2_font_decode_glyph_target(u8g2, glyph_data, target);
  }
  return dx;
}

//========================================================

uint8_t u8g2_IsGlyph(u8g2_font_t *u8g2, uint16_t requested_encoding)
{
  /* updated to new code */
  if ( u8g2_font_get_glyph_data(u8g2, requested_encoding) != NULL )
    return 1;
  return 0;
}

/* side effect: updates u8g2->font_decode and u8g2->glyph_x_offset */
/* actually u8g2_GetGlyphWidth returns the glyph delta x and glyph width itself is set as side effect */
int8_t u8g2_GetGlyphWidth(u8g2_font_t *u8g2, uint16_t requested_encoding)
{
  const uint8_t *glyph_data = u8g2_font_get_glyph_data(u8g2, requested_encoding);
  if ( glyph_data == NULL )
    return 0; 
  
  u8g2_font_setup_decode(u8g2, glyph_data);
  u8g2->glyph_x_offset = u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_x);
  u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_y);
  
  /* glyph width is here: u8g2->font_decode.glyph_width */

  return u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_delta_x);
}
/* side effect: updates u8g2->font_decode and u8g2->glyph_x_offset */
/* actually u8g2_GetGlyphWidth returns the glyph delta x and glyph width itself is set as side effect */
int8_t u8g2_GetGlyphHeight(u8g2_font_t *u8g2, uint16_t requested_encoding)
{
  const uint8_t *glyph_data = u8g2_font_get_glyph_data(u8g2, requested_encoding);
  if ( glyph_data == NULL )
    return 0; 
  
  u8g2_font_setup_decode(u8g2, glyph_data);
  u8g2->glyph_x_offset = u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_x);
  u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_y);
  
  /* glyph width is here: u8g2->font_decode.glyph_width */
  return u8g2->font_decode.glyph_height;
  //return u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_delta_x);
}

u8g2_font_decode_t u8g2_GetGlyphInfo(u8g2_font_t *u8g2, uint16_t requested_encoding)
{
  const uint8_t *glyph_data = u8g2_font_get_glyph_data(u8g2, requested_encoding);

  u8g2_font_setup_decode(u8g2, glyph_data);
  u8g2->glyph_x_offset = u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_x);
  u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_char_y);
  u8g2_font_decode_get_signed_bits(&(u8g2->font_decode), u8g2->font_info.bits_per_delta_x);
  return u8g2->font_decode;
}




void u8g2_SetFontMode(u8g2_font_t *u8g2, uint8_t is_transparent)
{
  u8g2->font_decode.is_transparent = is_transparent;    // new font procedures
}

void u8g2_SetFontDirection(u8g2_font_t *u8g2, uint8_t dir)
{
  u8g2->font_decode.dir = dir;
}

int16_t u8g2_DrawGlyph_target(u8g2_font_t *u8g2, uint16_t encoding, uint8_t * target)
{
  return u8g2_font_draw_glyph_target(u8g2, encoding, target);
}

int16_t u8g2_DrawGlyph(u8g2_font_t *u8g2, int16_t x, int16_t y, uint16_t encoding)
{
  return u8g2_font_draw_glyph(u8g2, x, y, encoding);
}

int16_t u8g2_DrawStr(u8g2_font_t *u8g2, int16_t x, int16_t y, const char *s)
{
  int16_t sum, delta;
  sum = 0;
    
  while( *s != '\0' )
  {
    delta = u8g2_DrawGlyph(u8g2, x, y, *s);
    switch(u8g2->font_decode.dir)
    {
      case 0:
        x += delta;
        break;
      case 1:
        y += delta;
        break;
      case 2:
        x -= delta;
        break;
      case 3:
        y -= delta;
        break;
    }
    sum += delta;    
    s++;
  }
  return sum;
}



void u8g2_SetFont(u8g2_font_t *u8g2, const uint8_t  *font)
{
  if ( u8g2->font != font )
  {
    u8g2->font = font;
    u8g2->font_decode.is_transparent = 1; 
    
    u8g2_read_font_info(&(u8g2->font_info), font);
  }
}

void u8g2_SetForegroundColor(u8g2_font_t *u8g2, uint16_t fg)
{
  u8g2->font_decode.fg_color = fg;
}

void u8g2_SetBackgroundColor(u8g2_font_t *u8g2, uint16_t bg)
{
  u8g2->font_decode.bg_color = bg;
}

//========================================================
