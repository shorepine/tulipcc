set(IDF_TARGET esp32s3)

set(MICROPY_PY_TINYUSB ON)


set(BOARD_DEFINITION1 AMYBOARD)
set(BOARD_DEFINITION2 MAKERFABS)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    ../esp32s3/boards/sdkconfig.tulip
    boards/AMYBOARD/sdkconfig.board
)

#list(APPEND MICROPY_SOURCE_BOARD
#)