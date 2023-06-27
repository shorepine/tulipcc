set(IDF_TARGET esp32s3)

set(SDKCONFIG_DEFAULTS
    ../../micropython/ports/esp32/boards/sdkconfig.base
    ../../micropython/ports/esp32/boards/sdkconfig.usb
    ../../micropython/ports/esp32/boards/sdkconfig.ble
    ../../micropython/ports/esp32/boards/sdkconfig.240mhz
    boards/TULIP4_N8R8/sdkconfig.board
)
