// usb_keyboard.h
#ifndef __USB_KEYBOARD_H__
#define __USB_KEYBOARD_H__

#include "esp_rom_gpio.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "usb/usb_host.h"
#include "keyscan.h"
#include "tulip_helpers.h"
#include "esp_timer.h"

// vortex 8
// keychron 16
#define KEYBOARD_BYTES 8
extern uint16_t keyboard_bytes;

// How long you hold down a key before it starts repeating
#define KEY_REPEAT_TRIGGER_MS 500
// How often (in ms) to repeat a key once held
#define KEY_REPEAT_INTER_MS 90

void usbh_setup();
void run_usb();

#endif
