#include "pins.h"
#define MICROPY_HW_BOARD_NAME               "TulipCC"
#define MICROPY_HW_MCU_NAME                 "ESP32S3"

#define MICROPY_PY_MACHINE_DAC              (0)

// Enable UART REPL for modules that have an external USB-UART and don't use native USB.
#define MICROPY_HW_ENABLE_UART_REPL         (1)


#define MICROPY_HW_I2C0_SCL                 (I2C_SCL)
#define MICROPY_HW_I2C0_SDA                 (I2C_SDA)

#define MICROPY_HW_ENABLE_SDCARD            (0)
#define MICROPY_PY_MACHINE_I2S              (0)
#define MICROPY_PY_BLUETOOTH (0)
#define MICROPY_BLUETOOTH_NIMBLE (0)
#define MICROPY_HW_USB_CDC (1)
#define MICROPY_HW_ENABLE_USBDEV (1)
#define MICROPY_HW_ESP_USB_SERIAL_JTAG (0)
#define MICROPY_ENABLE_SCHEDULER (1)
#define MICROPY_SCHEDULER_DEPTH             (128)