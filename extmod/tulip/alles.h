// alles.h

#ifndef __ALLES_H
#define __ALLES_H

#include "amy.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <math.h>

#ifdef ESP_PLATFORM
//#define configUSE_TASK_NOTIFICATIONS 1
//#define configTASK_NOTIFICATION_ARRAY_ENTRIES 2
#define MAX_WIFI_WAIT_S 120
#define PCM_PATCHES_SIZE_SMALL 1
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "driver/uart.h"
#include "driver/i2s.h"
#include "nvs_flash.h"

#include "tasks.h"

// This can be 32 bit, int32_t -- helpful for digital output to a i2s->USB teensy3 board
#define I2S_SAMPLE_TYPE I2S_BITS_PER_SAMPLE_16BIT
typedef int16_t i2s_sample_type;


// Pins & buttons
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout.jpg
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 4//45 // this is also VSPI 
#define CONFIG_I2S_BCLK 1//2//1//47 // SPICLK_P 
#define CONFIG_I2S_DIN 2//2//48 // SPICLK_N and LED, so move
#define CONFIG_I2S_NUM 0 
#define AMY_CORES 2
#define OSCS 64
#else // ESP_PLATFORM
#define PCM_PATCHES_SIZE_LARGE 1
#endif


#include "amy/amy.h"

#define UDP_PORT 9294        // port to listen on
#define MULTICAST_TTL 255     // hops multicast packets can take
#define MULTICAST_IPV4_ADDR "232.10.11.12"
#define PING_TIME_MS 10000   // ms between boards pinging each other
#define MAX_RECEIVE_LEN 4095

// enums
#define DEVBOARD 0
#define ALLES_BOARD_V1 1
#define ALLES_BOARD_V2 2
#define ALLES_DESKTOP 3
#define ALLES_TULIP 4

void delay_ms(uint32_t ms);

// Status mask 
#define RUNNING 1


// Sounds
extern void bleep();
extern void debleep();
extern void upgrade_tone();
extern void wifi_tone();
extern void scale(uint8_t wave);

void alles_parse_message(char *message, uint16_t length);
void update_map(uint8_t client, uint8_t ipv4, int64_t time);
void handle_sync(int64_t time, int8_t index);
void ping(int64_t sysclock);


// multicast
extern void mcast_send(char*, uint16_t len);
extern void create_multicast_ipv4_socket();

void alles_init_multicast();

void esp_show_debug(uint8_t type);
void alles_send_message(char * message, uint16_t len);

#ifdef ESP_PLATFORM
void run_alles();
#else
void * alles_start(void *vargs);
#endif


#ifdef __cplusplus
}
#endif

#endif



