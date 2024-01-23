set(IDF_TARGET esp32s3)


set(BOARD_DEFINITION1 N16R8)
set(BOARD_DEFINITION2 TULIP_DIY)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.ble
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N16R8/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_PORT
     ../../../tulip/esp32s3/esp32s3_display.c
     ../../../tulip/esp32s3/usb_keyboard.c
     ../../../tulip/esp32s3/ft5x06_touchscreen.c
     ../../../tulip/esp32s3/esp32s3_joy.c
)