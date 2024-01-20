#ifndef __ESP32S3DISPLAYH__
#define __ESP32S3DISPLAYH__

#include "display.h"
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
#include "driver/ledc.h"
#include "tasks.h"

void run_esp32s3_display();
void esp32s3_display_timings(uint32_t t0,uint32_t t1,uint32_t t2,uint32_t t3,uint32_t t4,uint32_t t5,uint32_t t6,uint32_t t7,uint32_t t8,uint32_t t9);
void display_brightness(uint8_t amount);
void esp_display_set_clock(uint8_t mhz) ;
void esp32s3_display_restart();
void esp32s3_display_stop();
void esp32s3_display_start();


// can't use
// USB: D- = 19, D+ = 20
// 35, 36, 37 -- for quad psram
// PCLK on 38 or 13 didn't work -- bad image


// 12 -- 15.5FPS, fine
// 16 -- 18.7FPS, fine , some skips
// 18 -- 24.1FPS skips during long lines of TFB text but otherwise fine
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout.jpg


#define DEFAULT_HSYNC_BACK_PORCH 16
#define DEFAULT_HSYNC_FRONT_PORCH 210
#define DEFAULT_HSYNC_PULSE_WIDTH 30
#define DEFAULT_VSYNC_BACK_PORCH 10
#define DEFAULT_VSYNC_FRONT_PORCH 22
#define DEFAULT_VSYNC_PULSE_WIDTH 13




// VGA
/*
#define DEFAULT_HSYNC_BACK_PORCH 48
#define DEFAULT_HSYNC_FRONT_PORCH 16
#define DEFAULT_HSYNC_PULSE_WIDTH 96
#define DEFAULT_VSYNC_BACK_PORCH 33
#define DEFAULT_VSYNC_FRONT_PORCH 10
#define DEFAULT_VSYNC_PULSE_WIDTH 2
*/
extern uint32_t HSYNC_BACK_PORCH, HSYNC_FRONT_PORCH, HSYNC_PULSE_WIDTH, VSYNC_BACK_PORCH, VSYNC_FRONT_PORCH, VSYNC_PULSE_WIDTH;


#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL

//#define PIN_NUM_BK_LIGHT       10 // was 19, was 14 white
#define PIN_NUM_HSYNC          39
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             40
#define PIN_NUM_PCLK           42 // was 38, was 20, was 13 black
#define PIN_NUM_BK_PWM		   10


// https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html
#define PIN_NUM_DATA0          9 //  B6, 14
#define PIN_NUM_DATA1          1 //  B7, 13

#define PIN_NUM_DATA2          15  // G5, 23
#define PIN_NUM_DATA3          16  // G6, 22
#define PIN_NUM_DATA4          4 // G7, 21 

#define PIN_NUM_DATA5          47   // R5, 31
#define PIN_NUM_DATA6          21   // R6, 30
#define PIN_NUM_DATA7          14  // R7, 29




//... We keep the rest as we have to drive them low while i have it plugged in, but i'm wroking on that...
#define PIN_NUM_DATA8          46 // B5
#define PIN_NUM_DATA9          3 // B4
#define PIN_NUM_DATA10         8 // B3
#define PIN_NUM_DATA11         7 // G4
#define PIN_NUM_DATA12         6 // G3
#define PIN_NUM_DATA13         5 // G2
#define PIN_NUM_DATA14         48 // R4
#define PIN_NUM_DATA15         45 // R3


#define PIN_NUM_DISP_EN        -1
/*
//These are from MCUxpresso for the HOTMCU 10.1
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20

*/

#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)



#endif