/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 * Tuan PM <tuanpm at live dot com>
 */
#ifndef _ESP_PLATFORM_H__
#define _ESP_PLATFORM_H__

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include <stdint.h>
#include <sys/time.h>

char *platform_create_id_string(void);
int platform_random(int max);
uint64_t platform_tick_get_ms(void);

#define ESP_MEM_CHECK(TAG, a, action) if (!(a)) {                                                      \
        ESP_LOGE(TAG,"%s(%d): %s",  __FUNCTION__, __LINE__, "Memory exhausted"); \
        action;                                                                                         \
        }

#define ESP_OK_CHECK(TAG, a, action) if ((a) != ESP_OK) {                                                     \
        ESP_LOGE(TAG,"%s(%d): %s", __FUNCTION__, __LINE__, "Failed with non ESP_OK err code"); \
        action;                                                                                               \
        }

#endif
