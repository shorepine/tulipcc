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
#include "pins.h"


void run_esp32s3_display();
void display_brightness(uint8_t amount);
void esp_display_set_clock(uint8_t mhz) ;
void esp32s3_display_restart();

void esp32s3_display_stop();
void esp32s3_display_start();




#endif