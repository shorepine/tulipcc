#include "pins.h"
#define MICROPY_HW_BOARD_NAME               "TulipCC"
#define MICROPY_HW_MCU_NAME                 "ESP32S3"

#define MICROPY_PY_MACHINE_DAC              (0)

// Enable UART REPL for modules that have an external USB-UART and don't use native USB.
#define MICROPY_HW_ENABLE_UART_REPL         (1)

#define USB_SERIAL_JTAG_PACKET_SZ_BYTES 64

#define MICROPY_ESP32_USE_BOOTLOADER_RTC (0)
#define MICROPY_BOARD_ENTER_BOOTLOADER(nargs,args) void
