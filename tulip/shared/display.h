// display.h

#ifndef __DISPLAYH__
#define __DISPLAYH__
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "lodepng.h"
#include "tulip_helpers.h"
#include "polyfills.h"
#include "ui.h"
#include <inttypes.h>
#include "lvgl.h"
#define RGB332

#ifdef ESP_PLATFORM
#include "esp32s3_display.h"
#else
#define IRAM_ATTR
#endif

extern uint8_t bg_pal_color;
extern uint8_t tfb_fg_pal_color;
extern uint8_t tfb_bg_pal_color;
extern uint8_t ansi_active_bg_color; 
extern uint8_t ansi_active_fg_color; 
extern int16_t ansi_active_format;

#define TULIP_TEAL 9
static const uint8_t ansi_pal[256] = {
0, 128, 16, 144, 2, 130, 18, 219, 146, 224, 28, 252, 3, 227, 31, 255, 0, 1, 2, 2, 3, 3, 8, 9, 10, 10, 
11, 11, 16, 17, 18, 18, 19, 19, 20, 21, 22, 22, 23, 23, 24, 25, 26, 26, 27, 27, 28, 29, 30, 30, 31, 31, 
64, 65, 66, 66, 67, 67, 72, 73, 74, 74, 75, 75, 80, 81, 82, 82, 83, 83, 84, 85, 86, 86, 87, 87, 88, 89, 
90, 90, 91, 91, 92, 93, 94, 94, 95, 95, 128, 129, 130, 130, 131, 131, 136, 137, 138, 138, 139, 139, 144, 
145, 146, 146, 147, 147, 148, 149, 150, 150, 151, 151, 152, 153, 154, 154, 155, 155, 156, 157, 158, 158, 
159, 159, 160, 161, 162, 162, 163, 163, 168, 169, 170, 170, 171, 171, 176, 177, 178, 178, 179, 179, 180, 
181, 182, 182, 183, 183, 184, 185, 186, 186, 187, 187, 188, 189, 190, 190, 191, 191, 192, 193, 194, 194, 
195, 195, 200, 201, 202, 202, 203, 203, 208, 209, 210, 210, 211, 211, 212, 213, 214, 214, 215, 215, 216, 
217, 218, 218, 219, 219, 220, 221, 222, 222, 223, 223, 224, 225, 226, 226, 227, 227, 232, 233, 234, 234, 
235, 235, 240, 241, 242, 242, 243, 243, 244, 245, 246, 246, 247, 247, 248, 249, 250, 250, 251, 251, 252, 
253, 254, 254, 255, 255, 0, 0, 0, 36, 36, 36, 73, 73, 73, 109, 109, 109, 146, 146, 146, 146, 182, 182, 
182, 219, 219, 219, 255, 255
};

extern int16_t last_touch_x[3];
extern int16_t last_touch_y[3];
extern uint8_t touch_held;
extern uint8_t tfb_log;
//extern lv_group_t * lvgl_kb_group;

void display_reset_sprites();
void display_reset_tfb();
void display_reset_bg();
void display_tfb_update(int8_t tfb_row_hint);
void display_set_clock(uint8_t mhz) ;
uint8_t lvgl_focused();
uint8_t tab_eater();

void display_set_bg_pixel_pal(uint16_t x, uint16_t y, uint8_t pal_idx);
void display_set_bg_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void display_get_bg_pixel(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b);
uint8_t display_get_bg_pixel_pal(uint16_t x, uint16_t y);
void display_invert_bg(uint16_t x, uint16_t y, uint16_t w, uint16_t h) ;

void display_get_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
void display_set_bg_bitmap_rgba(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);
void display_set_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);
void display_bg_bitmap_blit(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t x1,uint16_t y1);
void display_bg_bitmap_blit_alpha(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t x1,uint16_t y1);

void display_load_sprite_rgba(uint32_t mem_pos, uint32_t len, uint8_t* data);
void display_load_sprite_raw(uint32_t mem_pos, uint32_t len, uint8_t* data);
void display_screenshot(char * filename);
void display_screenshot_pal(char * filename);
void display_tfb_str(unsigned char*str, uint16_t len, uint8_t format, uint8_t fg_color, uint8_t bg_color);
void display_wireframe_update(uint8_t sprite_no);

void display_tfb_new_row();
void display_run();
void display_init();
void display_brightness(uint8_t amount);

void unpack_rgb_332(uint8_t px0, uint8_t *r, uint8_t *g, uint8_t *b);
void unpack_pal_idx(uint16_t pal_idx, uint8_t *r, uint8_t *g, uint8_t *b);
void unpack_ansi_idx(uint8_t ansi_idx, uint8_t *r, uint8_t *g, uint8_t *b);
bool display_bounce_empty(void *bounce_buf, int pos_px, int len_bytes, void *user_ctx);
bool display_frame_done_generic();
void display_swap();
uint8_t rgb565to332(uint16_t rgb565);
void display_teardown(void);

uint8_t check_dim_xy(uint16_t x, uint16_t y);
uint8_t check_dim_xywh(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint8_t collide_mask_get(uint8_t a, uint8_t b);

extern const unsigned char font_8x12_r[256][12];
extern const unsigned char portfolio_glyph_bitmap[1792];

#define MAX_LINE_EMITS 60000

// We can address this many moving things on screen
#define SPRITES 32
// We assume we can store 16 unique 32x32 sprite tiles, you can swap these out from RAM
#define SPRITE_RAM_BYTES (32*32*SPRITES)

#ifndef TDECK
#define H_RES 1024
#define V_RES 600
#else
#define H_RES 320
#define V_RES 240
#endif

#ifdef TDECK
#define FONT_HEIGHT 8
#define FONT_WIDTH 6
#else
#define FONT_HEIGHT 12
#define FONT_WIDTH 8
#endif

#define OFFSCREEN_X_PX 1024
#define OFFSCREEN_Y_PX 100
#define DEFAULT_PIXEL_CLOCK_MHZ 28
#define BOUNCE_BUFFER_SIZE_PX (H_RES*12)
#define TFB_ROWS (V_RES/FONT_HEIGHT)
#define TFB_COLS (H_RES/FONT_WIDTH)

extern uint16_t PIXEL_CLOCK_MHZ;

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


#ifdef RGB332
#define BYTES_PER_PIXEL 1
#else
#define BYTES_PER_PIXEL 2
#endif



#define FLASH_FRAMES 12
#define ALPHA0 0x55
#define ALPHA1 0x53
#define ALPHA 0x55

#define FORMAT_INVERSE 0x80 
#define FORMAT_UNDERLINE 0x40
#define FORMAT_FLASH 0x20
#define FORMAT_BOLD 0x10 
#define FORMAT_STRIKE 0x08

#define SPRITE_IS_SPRITE 0x80
#define SPRITE_IS_WIREFRAME 0x40
#define SPRITE_IS_BEZIER 0x20
#define SPRITE_IS_ELLIPSE 0x10

extern uint8_t gpu_log;
extern uint8_t tfb_active;
extern uint8_t tfb_y_row; 
extern uint8_t tfb_x_col; 
extern int32_t vsync_count;
extern uint8_t brightness;
extern float reported_fps;
extern float reported_gpu_usage;
extern uint8_t *collision_bitfield;

extern const uint16_t rgb332_rgb565_i[256];
// RAM for sprites and background FB
extern uint8_t *sprite_ids;  // IRAM
extern uint8_t *sprite_ram; // in IRAM
extern uint8_t * bg; // in SPIRAM
extern uint8_t * bg_tfb; // in SPIRAM
extern uint16_t *sprite_x_px;//[SPRITES]; 
extern uint16_t *sprite_y_px;//[SPRITES]; 
extern uint16_t *sprite_w_px;//[SPRITES]; 
extern uint16_t *sprite_h_px;//[SPRITES]; 
extern uint8_t *sprite_vis;//[SPRITES];
extern uint32_t *sprite_mem;//[SPRITES];
extern uint8_t *TFB;//[TFB_ROWS][TFB_COLS];
extern uint8_t *TFBfg;//[TFB_ROWS][TFB_COLS];
extern uint8_t *TFBbg;//[TFB_ROWS][TFB_COLS];
extern uint8_t *TFBf;//[TFB_ROWS][TFB_COLS];
extern int16_t *x_offsets;//[V_RES];
extern int16_t *y_offsets;//[V_RES];
extern int16_t *x_speeds;//[V_RES];
extern int16_t *y_speeds;//[V_RES];
extern uint32_t **bg_lines;//[V_RES];
extern uint16_t *TFB_pxlen;
extern uint8_t *lines_bitmap;

#endif