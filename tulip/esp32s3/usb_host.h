// usb_host.h
#ifndef __USB_KEYBOARD_H__
#define __USB_KEYBOARD_H__

// On AMYboard (AMYBOARD_USB_HOST variant) we only host USB MIDI devices —
// there's no display or keyboard input path, so all the HID keyboard/mouse
// handling (keyscan, lvgl, touch) is compiled out.
#ifdef AMYBOARD
#define USB_HOST_MIDI_ONLY 1
#endif

#include "esp_rom_gpio.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "usb/usb_host.h"
#ifndef USB_HOST_MIDI_ONLY
#include "keyscan.h"
#include "display.h"
#else
// On Tulip this arrives via display.h -> esp32s3_display.h -> tasks.h.
extern TaskHandle_t tulip_mp_handle;
#endif
#include "tulip_helpers.h"
#include "esp_timer.h"
#include "amy.h"       // MAX_MESSAGE_LEN (via display.h on Tulip, direct here)
#include "amy_midi.h"  // from extmod/tulip/

extern uint8_t tulip_ready;

void usbh_setup();
void run_usb();

#endif
