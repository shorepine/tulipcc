#ifndef __TDECK_KEYBOARD_H__
#define __TDECK_KEYBOARD_H__

#include "keyscan.h"
#include "tulip_helpers.h"
#include "esp_timer.h"


#define TDECK_POWERON       10
#define TDECK_I2C_SDA       18
#define TDECK_I2C_SCL       8
#define LILYGO_KB_SLAVE_ADDRESS     0x55
#define TDECK_TOUCH_ADDR_0 0x14
#define TDECK_TOUCH_ADDR_1 0x5D

#define TDECK_TRACKBALL_UP 3
#define TDECK_TRACKBALL_DOWN 15
#define TDECK_TRACKBALL_LEFT 1
#define TDECK_TRACKBALL_RIGHT 2
#define TDECK_TRACKBALL_CLICK 0

#define TDECK_TOUCH_INT 16

// How long you hold down a key before it starts repeating
#define KEY_REPEAT_TRIGGER_MS 500
// How often (in ms) to repeat a key once held
#define KEY_REPEAT_INTER_MS 90

void run_tdeck_keyboard();

#endif
