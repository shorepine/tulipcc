#ifndef __TLONGDISPLAYH__
#define __TLONGDISPLAYH__
#include "pins_config.h"
#include "display.h"
#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include <time.h>
#include <string.h>
#include "esp_random.h"
#include "tulip_helpers.h"
#include "driver/ledc.h"
#include "tasks.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "soc/soc_caps.h"





void run_tlong_display();
void display_brightness(uint8_t amount);

void tlong_display_stop();
void tlong_display_start();


#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL



#endif