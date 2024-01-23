#ifndef __TDECK_KEYBOARD_H__
#define __TDECK_KEYBOARD_H__

#include "keyscan.h"
#include "tulip_helpers.h"
#include "esp_timer.h"
#include "pins.h"

// How long you hold down a key before it starts repeating
#define KEY_REPEAT_TRIGGER_MS 500
// How often (in ms) to repeat a key once held
#define KEY_REPEAT_INTER_MS 90

void run_tdeck_keyboard();

#endif
