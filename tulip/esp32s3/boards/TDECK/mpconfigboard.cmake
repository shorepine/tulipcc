set(IDF_TARGET esp32s3)


set(BOARD_DEFINITION1 N16R8)
set(BOARD_DEFINITION2 TDECK)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N16R8/sdkconfig.board
    boards/TDECK/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_BOARD
     tdeck_display.c
     tdeck_keyboard.c
     esp_lcd_touch_gt911.c
     gt911_touchscreen.c
     ../../micropython/ports/esp32/usb_serial_jtag.c
)

