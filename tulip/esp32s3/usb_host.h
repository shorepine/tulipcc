// usb_host.h
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
#include "display.h"
#include "midi.h"  // from extmod/tulip/

extern uint8_t tulip_ready;

void usbh_setup();
void run_usb();

#endif
