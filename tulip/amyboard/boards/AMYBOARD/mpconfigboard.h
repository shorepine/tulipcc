#include "pins.h"
#define MICROPY_HW_BOARD_NAME               "AMYboard"
#define MICROPY_HW_MCU_NAME                 "ESP32S3"

#define MICROPY_PY_MACHINE_DAC              (0)

#define MICROPY_HW_ENABLE_UART_REPL         (0)
#define MICROPY_HW_USB_CDC (1)
#define MICROPY_HW_ESP_USB_SERIAL_JTAG (0)
#define MICROPY_HW_ENABLE_USBDEV (1)
#define MICROPY_HW_USB_CDC_1200BPS_TOUCH    (1)
//#define MICROPY_SCHEDULER_STATIC_NODES      (1)
