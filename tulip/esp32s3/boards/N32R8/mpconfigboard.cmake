set(IDF_TARGET esp32s3)

set(BOARD_DEFINITION1 N32R8)
set(BOARD_DEFINITION2 TULIP_DIY)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/sdkconfig.tulip
    boards/N32R8/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_PORT
     esp32s3_display.c
     ft5x06_touchscreen.c
     usb_host.c
)
