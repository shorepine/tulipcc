set(IDF_TARGET esp32s3)



set(BOARD_DEFINITION1 TULIP4_R10)
set(BOARD_DEFINITION2 MAKERFABS)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.ble
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N32R8/sdkconfig.board
    boards/TULIP4_R10/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_PORT
     ../../../tulip/esp32s3/gt911_touchscreen.c
     ../../../tulip/esp32s3/esp32s3_display.c
     ../../../tulip/esp32s3/usb_keyboard.c
)