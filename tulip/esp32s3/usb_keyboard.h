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
#include "midi.h"  // from extmod/tulip/


// vortex 8
// keychron 16


void usbh_setup();
void run_usb();

#endif
