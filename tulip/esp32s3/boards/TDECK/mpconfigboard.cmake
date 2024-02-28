set(IDF_TARGET esp32s3)


set(BOARD_DEFINITION1 N16R8)
set(BOARD_DEFINITION2 TDECK)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.ble
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N16R8/sdkconfig.board
    boards/TDECK/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_PORT
     ../../../tulip/esp32s3/tdeck_display.c
     ../../../tulip/esp32s3/tdeck_keyboard.c
     ../../../tulip/esp32s3/gt911_touchscreen.c
)
