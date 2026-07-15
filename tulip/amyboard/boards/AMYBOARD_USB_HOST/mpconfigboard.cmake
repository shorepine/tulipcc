set(IDF_TARGET esp32s3)

set(MICROPY_PY_TINYUSB ON)


# AMYBOARD is always defined for this port (see esp32_common.cmake); this adds
# the variant macro that switches USB to host (MIDI) mode + UART REPL.
set(BOARD_DEFINITION1 AMYBOARD_USB_HOST)
set(BOARD_DEFINITION2 MAKERFABS)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    ../esp32s3/boards/sdkconfig.tulip
    boards/AMYBOARD/sdkconfig.board
)

list(APPEND MICROPY_SOURCE_BOARD
     ../esp32s3/usb_host.c
)
