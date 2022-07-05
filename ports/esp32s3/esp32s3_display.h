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


void esp32s3_display_run();
void display_brightness(uint8_t amount);
void esp_display_set_clock(uint8_t mhz) ;


// can't use
// USB: D- = 19, D+ = 20
// 35, 36, 37 -- for quad psram
// PCLK on 38 or 13 didn't work -- bad image


// 12 -- 15.5FPS, fine
// 16 -- 18.7FPS, fine , some skips
// 18 -- 24.1FPS skips during long lines of TFB text but otherwise fine
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout.jpg

#define PIXEL_CLOCK_MHZ     18
#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL

#define PIN_NUM_BK_LIGHT       39 // was 19, was 14 white
#define PIN_NUM_HSYNC          40
#define PIN_NUM_VSYNC          41
#define PIN_NUM_DE             42
#define PIN_NUM_PCLK           14 // was 38, was 20, was 13 black
#define PIN_NUM_BK_PWM		   16


// https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html
#define PIN_NUM_DATA0          12 //  B6, 14
#define PIN_NUM_DATA1          21 //  B7, 13

#define PIN_NUM_DATA2          8  // G5, 23
#define PIN_NUM_DATA3          3  // G6, 22
#define PIN_NUM_DATA4          46 // G7, 21 

#define PIN_NUM_DATA5          6   // R5, 31
#define PIN_NUM_DATA6          7   // R6, 30
#define PIN_NUM_DATA7          15  // R7, 29




//... We keep the rest as we have to drive them low while i have it plugged in, but i'm wroking on that...
#define PIN_NUM_DATA8          5 // B2
#define PIN_NUM_DATA9          5 // B1
#define PIN_NUM_DATA10         5 // B0
#define PIN_NUM_DATA11         5 // G2
#define PIN_NUM_DATA12         5 // G1
#define PIN_NUM_DATA13         5 // G0
#define PIN_NUM_DATA14         5 // R1
#define PIN_NUM_DATA15         5 // R0


#define PIN_NUM_DISP_EN        -1

//These are from MCUxpresso for the HOTMCU 10.1
#define HSYNC_BACK_PORCH 139
#define HSYNC_FRONT_PORCH 140
#define HSYNC_PULSE_WIDTH 20
#define VSYNC_BACK_PORCH 20
#define VSYNC_FRONT_PORCH 12
#define VSYNC_PULSE_WIDTH 20



#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)



#endif