// display.h

#ifndef __DISPLAYH__
#define __DISPLAYH__

#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_types.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include <time.h>
#include <string.h>
#include "esp_random.h"
#include "tulip_helpers.h"
#include "lodepng.h"

//#define RGB565
#define RGB332

uint8_t bg_pal_color;
uint8_t tfb_pal_color;
uint8_t ansi_active_bg_color; 
int16_t ansi_active_format;


void display_set_bg_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void display_get_bg_pixel(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b);
void display_get_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *);
void display_bg_bitmap(int x_start, int y_start, int x_end, int y_end, uint8_t* data);
void display_load_sprite(uint32_t mem_pos, uint32_t len, uint8_t* data);
void display_screenshot(char * filename);
void display_tfb_str(char*str, uint16_t len, uint8_t format);
uint8_t display_get_clock();
void display_set_clock(uint8_t mhz);
void display_tfb_new_row();
void display_run();

void unpack_rgb_565(uint8_t px0, uint8_t px1, uint8_t *r, uint8_t *g, uint8_t *b);
void unpack_rgb_332(uint8_t px0, uint8_t *r, uint8_t *g, uint8_t *b);


extern const unsigned char font_8x12_r[256][12];

// can't use
// USB: D- = 19, D+ = 20
// 35, 36, 37 -- for quad psram
// PCLK on 38 or 13 didn't work -- bad image




// 12 -- 15.5FPS, fine
// 16 -- 18.7FPS, fine , some skips
// 18 -- 24.1FPS skips during long lines of TFB text but otherwise fine

#define PIXEL_CLOCK_MHZ     12
#define BK_LIGHT_ON_LEVEL  1
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL
#define PIN_NUM_BK_LIGHT       39 // was 19, was 14 white
#define PIN_NUM_HSYNC          40
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             42
#define PIN_NUM_PCLK           14 // was 38, was 20, was 13 black

#ifdef RGB332
// TODO, wait, shouldn't i be connecitng these to B7,B6.. ??? 
#define PIN_NUM_DATA0          21  // B4
#define PIN_NUM_DATA1          12  // B3
#define PIN_NUM_DATA2          46  // G5 
#define PIN_NUM_DATA3          3 // G4
#define PIN_NUM_DATA4          8 // G3
#define PIN_NUM_DATA5         15 // R4
#define PIN_NUM_DATA6         7 // R3
#define PIN_NUM_DATA7         6 // R2
//... We keep the rest as we have to drive them low while i have it plugged in
#define PIN_NUM_DATA8          11 // B2
#define PIN_NUM_DATA9          10 // B1
#define PIN_NUM_DATA10         9 // B0
#define PIN_NUM_DATA11         18 // G2
#define PIN_NUM_DATA12         17 // G1
#define PIN_NUM_DATA13         16 // G0
#define PIN_NUM_DATA14         5 // R1
#define PIN_NUM_DATA15         4 // R0
#endif

#ifdef RGB565
// TODO, wait, shouldn't i be connecitng these to B7,B6.. ??? 
#define PIN_NUM_DATA0          9  // B0
#define PIN_NUM_DATA1          10  // B1
#define PIN_NUM_DATA2          11 // B2
#define PIN_NUM_DATA3          12  // B3
#define PIN_NUM_DATA4          21  // B4
#define PIN_NUM_DATA5          16  // G0 // this may be PSRAM CS? it's unclear 
#define PIN_NUM_DATA6          17 // G1
#define PIN_NUM_DATA7          18 // G2
#define PIN_NUM_DATA8          8 // G3
#define PIN_NUM_DATA9          3 // G4
#define PIN_NUM_DATA10         46 // G5
#define PIN_NUM_DATA11         4 // R0
#define PIN_NUM_DATA12         5 // R1
#define PIN_NUM_DATA13         6 // R2
#define PIN_NUM_DATA14         7 // R3
#define PIN_NUM_DATA15         15 // R4
#endif

#define PIN_NUM_DISP_EN        -1

// We can address this many moving things on screen
#define SPRITES 16
// We assume we can store 16 unique 32x32 sprite tiles, you can swap these out from RAM
#define SPRITE_RAM_BYTES (32*32*2*16) // 32KB

//These are from MCUxpresso for the HOTMCU 10.1
#define H_RES              1024
#define V_RES              600
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20



#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
extern int Cache_WriteBack_Addr(uint32_t addr, uint32_t size);
extern int Cache_Invalidate_Addr(uint32_t addr, uint32_t size);
extern uint32_t Cache_Start_DCache_Preload(uint32_t addr, uint32_t size, uint32_t order);
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


#define OFFSCREEN_Y_PX 150
#define OFFSCREEN_X_PX 256

#ifdef RGB332
#define BYTES_PER_PIXEL 1
#else
#define BYTES_PER_PIXEL 2
#endif

#define FONT_HEIGHT 12
#define FONT_WIDTH 8
#define TFB_ROWS (V_RES/FONT_HEIGHT)
#define TFB_COLS (H_RES/FONT_WIDTH)
#ifdef RGB565
#define BOUNCE_BUFFER_SIZE_PX (H_RES*FONT_HEIGHT) 
#define BOUNCE_BUFFER_SIZE_BYTES (BOUNCE_BUFFER_SIZE_PX*BYTES_PER_PIXEL)
#else
#define BOUNCE_BUFFER_SIZE_PX (H_RES*FONT_HEIGHT) 
#define BOUNCE_BUFFER_SIZE_BYTES (BOUNCE_BUFFER_SIZE_PX)
#endif

#define FLASH_FRAMES 12
#define ALPHA0 0x55
#define ALPHA1 0x53
#define ALPHA 0x55

#define FORMAT_INVERSE 0x80 
#define FORMAT_UNDERLINE 0x40
#define FORMAT_FLASH 0x20
#define FORMAT_BOLD 0x10 


#define ANSI_BLACK 0
#define ANSI_RED 1
#define ANSI_GREEN 2
#define ANSI_YELLOW 3
#define ANSI_BLUE 4
#define ANSI_MAGENTA 5
#define ANSI_CYAN 6
#define ANSI_WHITE 7
#define ANSI_BOLD_BLACK 8
#define ANSI_BOLD_RED 9
#define ANSI_BOLD_GREEN 10
#define ANSI_BOLD_YELLOW 11
#define ANSI_BOLD_BLUE 12
#define ANSI_BOLD_MAGENTA 13
#define ANSI_BOLD_CYAN 14
#define ANSI_BOLD_WHITE 15
#define TULIP_TEAL 16
#define FORMAT_BG_COLOR_NONE 16

uint8_t tfb_y_row; 
uint8_t tfb_x_col; 
uint8_t py_callback;
uint8_t task_screenshot;
uint8_t task_start;
uint8_t task_stop;
int32_t vsync_count;
uint8_t display_mhz;

uint16_t *sprite_x_px;//[SPRITES]; 
uint16_t *sprite_y_px;//[SPRITES]; 
uint16_t *sprite_w_px;//[SPRITES]; 
uint16_t *sprite_h_px;//[SPRITES]; 
uint8_t *sprite_vis;//[SPRITES];
uint32_t *sprite_mem;//[SPRITES];

uint8_t *TFB;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBf;//[TFB_ROWS][TFB_COLS];
uint16_t *x_offsets;//[V_RES];
uint16_t *y_offsets;//[V_RES];
int16_t *x_speeds;//[V_RES];
int16_t *y_speeds;//[V_RES];

uint32_t **bg_lines;//[V_RES];
uint8_t *ansi_pal;

#endif