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
void display_brightness(uint8_t amount);
void esp_display_set_clock(uint8_t mhz) ;
void esp32s3_display_restart();

void esp32s3_display_stop();
void esp32s3_display_start();

#ifdef TULIP_DIY
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20
#elif defined MAKERFABS
#define HSYNC_BACK_PORCH 16
#define HSYNC_FRONT_PORCH 210
#define HSYNC_PULSE_WIDTH 30
#define VSYNC_BACK_PORCH 10
#define VSYNC_FRONT_PORCH 22
#define VSYNC_PULSE_WIDTH 13
#endif


#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL

#ifdef TULIP_DIY
#define PIN_NUM_BK_LIGHT       39 // was 19, was 14 white
#define PIN_NUM_HSYNC          40
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             42
#define PIN_NUM_PCLK           14 // was 38, was 20, was 13 black
#define PIN_NUM_BK_PWM		   16
#define PIN_NUM_DATA0          12 //  B6, 14
#define PIN_NUM_DATA1          21 //  B7, 13
#define PIN_NUM_DATA2          8  // G5, 23
#define PIN_NUM_DATA3          3  // G6, 22
#define PIN_NUM_DATA4          46 // G7, 21 
#define PIN_NUM_DATA5          6   // R5, 31
#define PIN_NUM_DATA6          7   // R6, 30
#define PIN_NUM_DATA7          15  // R7, 29
//... We keep the rest as we have to drive them low 
#define PIN_NUM_DATA8          13 // B2
#define PIN_NUM_DATA9          13 // B1
#define PIN_NUM_DATA10         13 // B0
#define PIN_NUM_DATA11         13 // G2
#define PIN_NUM_DATA12         13 // G1
#define PIN_NUM_DATA13         13 // G0
#define PIN_NUM_DATA14         13 // R1
#define PIN_NUM_DATA15         13 // R0
#define PIN_NUM_DISP_EN        -1

#elif defined MAKERFABS
#define PIN_NUM_HSYNC          39
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             40
#define PIN_NUM_PCLK           42 
#define PIN_NUM_BK_PWM         10
#define PIN_NUM_DATA0          9 //  B6, 14
#define PIN_NUM_DATA1          1 //  B7, 13
#define PIN_NUM_DATA2          15  // G5, 23
#define PIN_NUM_DATA3          16  // G6, 22
#define PIN_NUM_DATA4          4 // G7, 21 
#define PIN_NUM_DATA5          47   // R5, 31
#define PIN_NUM_DATA6          21   // R6, 30
#define PIN_NUM_DATA7          14  // R7, 29
#define PIN_NUM_DATA8          46 // B5
#define PIN_NUM_DATA9          3 // B4
#define PIN_NUM_DISP_EN        -1


// Two color pins are not hooked up on R10 but are in MaTouch 7
#ifdef MATOUCH7
#define PIN_NUM_DATA10         8 // B3
#else
#define PIN_NUM_DATA10         3 // B4, repeated
#endif

#define PIN_NUM_DATA11         7 // G4
#define PIN_NUM_DATA12         6 // G3

#ifdef MATOUCH7
#define PIN_NUM_DATA13         5 // G2
#else
#define PIN_NUM_DATA13         6 // G3, repeated
#endif

#define PIN_NUM_DATA14         48 // R4
#define PIN_NUM_DATA15         45 // R3
#endif

#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)



#endif