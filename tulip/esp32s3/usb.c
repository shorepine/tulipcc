/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "py/mphal.h"
#include "usb.h"

#include "esp_rom_gpio.h"
#include "esp_mac.h"


#if MICROPY_HW_USB_CDC
#include "shared/tinyusb/mp_usbd.h"
#include "esp_private/usb_phy.h"
static usb_phy_handle_t phy_hdl;

#if CONFIG_IDF_TARGET_ESP32S3
// WORKAROUND for https://github.com/hathach/tinyusb/issues/2943
// On the ESP32-S3 the USB D+/D- pins power up muxed to the ROM USB-Serial-JTAG
// controller; the app must switch the mux to the USB-OTG (TinyUSB) PHY. As of
// ESP-IDF >= 5.2 the usb_new_phy() init no longer flips that mux reliably/early
// enough, so the host enumerates the JTAG device (303a:1001, "USB JTAG/serial
// debug unit") and keeps it -- our TinyUSB CDC+MIDI device (caf0:4009) never
// appears. It manifests worst on Windows (and direct-connect); a USB hub masks
// it by changing the reset timing. Force the DP/DM mux to the internal OTG PHY
// by hand instead, matching the merged CircuitPython workaround
// (adafruit/circuitpython#9973, diagnosed by TinyUSB's maintainer).
#include "esp_private/periph_ctrl.h"
#include "soc/periph_defs.h"
#include "soc/usb_pins.h"
#include "soc/rtc_cntl_struct.h"
#include "soc/usb_wrap_struct.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"
#endif


void usb_init(void) {
    // ref: https://github.com/espressif/esp-usb/blob/4b6a798d0bed444fff48147c8dcdbbd038e92892/device/esp_tinyusb/tinyusb.c

    // Configure USB PHY
    #if CONFIG_IDF_TARGET_ESP32S3
    // Manually route the DP/DM pads to the internal OTG PHY (see note above).
    // We deliberately bypass usb_new_phy() here, so phy_hdl stays NULL on S3 --
    // usb_usj_mode() below is written to tolerate that.
    (void)phy_hdl;
    periph_module_reset(PERIPH_USB_MODULE);
    periph_module_enable(PERIPH_USB_MODULE);

    USB_WRAP.otg_conf.pad_enable = 1;
    // USB_OTG uses the internal PHY
    USB_WRAP.otg_conf.phy_sel = 0;
    // phy_sel is controlled by the SW register value below
    RTCCNTL.usb_conf.sw_hw_usb_phy_sel = 1;
    // sw_usb_phy_sel = 1 -> USB_OTG connected to the internal PHY
    RTCCNTL.usb_conf.sw_usb_phy_sel = 1;

    gpio_set_drive_capability(USBPHY_DM_NUM, GPIO_DRIVE_CAP_3);
    gpio_set_drive_capability(USBPHY_DP_NUM, GPIO_DRIVE_CAP_3);

    // Switching the mux above moves the PHY off the ROM USB-Serial-JTAG, dropping
    // that device's D+ pull-up -- i.e. the host sees the 303a:1001 JTAG device
    // disconnect. Hold here before TinyUSB (mp_usbd_init below) re-asserts the
    // pull-up as caf0:4009, so a stubborn host has time to register the
    // disconnect and re-enumerate the new device instead of keeping the JTAG one.
    // Confirmed via USBView (#952) that a bare USB3 root port otherwise stays on
    // 303a:1001 forever and never sees AMYboard's MIDI device.
    esp_rom_delay_us(250 * 1000);  // 250 ms disconnect window
    #else
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_OTG,
        .otg_mode = USB_OTG_MODE_DEVICE,
    };
    // Internal USB PHY
    phy_conf.target = USB_PHY_TARGET_INT;

    // Init ESP USB Phy
    usb_new_phy(&phy_conf, &phy_hdl);
    #endif

    // Init MicroPython / TinyUSB
    mp_usbd_init();

}

void mp_usbd_port_get_serial_number(char *serial_buf) {
    // use factory default MAC as serial ID
    uint8_t mac[8];
    esp_efuse_mac_get_default(mac);
    MP_STATIC_ASSERT(sizeof(mac) * 2 <= MICROPY_HW_USB_DESC_STR_MAX);
    mp_usbd_hex_str(serial_buf, mac, sizeof(mac));
}

#endif

#if CONFIG_IDF_TARGET_ESP32S3

#if !MICROPY_HW_USB_CDC
#include "esp_private/usb_phy.h"
static usb_phy_handle_t phy_hdl;
#endif

void usb_usj_mode(void) {
    // Switch the USB PHY back to Serial/Jtag mode, disabling OTG support.
    // This should be run before jumping to bootloader.
    // usb_init() configures the OTG PHY by hand on the ESP32-S3 and never creates
    // a phy handle (see the #2943 workaround above), so only delete one if it
    // exists -- otherwise usb_del_phy(NULL) would fail the bootloader-entry path.
    if (phy_hdl != NULL) {
        usb_del_phy(phy_hdl);
        phy_hdl = NULL;
    }
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_SERIAL_JTAG,
    };
    usb_new_phy(&phy_conf, &phy_hdl);
}
#endif

