#ifndef __TDECKDISPLAYH__
#define __TDECKDISPLAYH__

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
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "esp_random.h"
#include "soc/soc_caps.h"




#define TDECK_PERI_GPIO 10
#define TDECK_LCD_BK_LIGHT_GPIO  42
#define TDECK_LCD_RST_GPIO       -1
#define TDECK_LCD_CS_GPIO        12
#define TDECK_LCD_DC_GPIO        11
#define TDECK_LCD_PCLK_GPIO      40
#define TDECK_LCD_MOSI_GPIO     41 // MOSI
#define TDECK_LCD_MISO_GPIO     38 // MISO
#define TDECK_LCD_PIXEL_CLOCK_HZ (80 * 1000 * 1000)
#define TDECK_SPI_HOST_ID SPI2_HOST



void run_tdeck_display();
void display_brightness(uint8_t amount);

void tdeck_display_stop();
void tdeck_display_start();


#define BK_LIGHT_ON_LEVEL  1
#define DEFAULT_BRIGHTNESS 5 // 9 is max, 1 is min
#define BK_LIGHT_OFF_LEVEL !BK_LIGHT_ON_LEVEL



#endif