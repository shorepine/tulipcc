set(IDF_TARGET esp32s3)

set(MICROPY_PY_TINYUSB ON)


set(BOARD_DEFINITION1 TULIP4_R11)
set(BOARD_DEFINITION2 MAKERFABS)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N32R8/sdkconfig.board
    boards/TULIP4_R11/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_BOARD
     gt911_touchscreen.c
     esp_lcd_touch_gt911.c
     esp32s3_display.c
     usb_host.c
)