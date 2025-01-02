# Set location of base MicroPython directory.
if(NOT MICROPY_DIR)
    get_filename_component(MICROPY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../micropython ABSOLUTE)
endif()

# Set location of base MicroPython esp32 port (which this is based on).
if(NOT MICROPY_ESP32_DIR)
    get_filename_component(MICROPY_ESP32_DIR ${CMAKE_CURRENT_LIST_DIR}/../../micropython/ports/esp32 ABSOLUTE)
endif()

# Set location of the ESP32 port directory.
if(NOT MICROPY_PORT_DIR)
    get_filename_component(MICROPY_PORT_DIR ${CMAKE_CURRENT_LIST_DIR}/ ABSOLUTE)
endif()


# Set location of the tulip shared directory.
if(NOT TULIP_SHARED_DIR)
    get_filename_component(TULIP_SHARED_DIR ${CMAKE_CURRENT_LIST_DIR}/../shared ABSOLUTE)
endif()

# Set location of the ulab directory.
if(NOT ULAB_DIR)
    get_filename_component(ULAB_DIR ${CMAKE_CURRENT_LIST_DIR}/../shared/ulab/code ABSOLUTE)
endif()

# Set location of the amy directory.
if(NOT AMY_DIR)
    get_filename_component(AMY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../amy ABSOLUTE)
endif()

# Set location of the tulip esp32s3 directory.
if(NOT TULIP_ESP32S3_DIR)
    get_filename_component(TULIP_ESP32S3_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
endif()


# Set location of lvgl_mp dir
if(NOT LV_BINDING_DIR)
    get_filename_component(LV_BINDING_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lv_binding_micropython_tulip ABSOLUTE)
endif()

# Set location of lvgl dir
if(NOT LVGL_DIR)
    get_filename_component(LVGL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lv_binding_micropython_tulip/lvgl ABSOLUTE)
endif()

file(GLOB_RECURSE LVGL_SOURCES ${LVGL_DIR}/src/*.c)


# Include core source components.
include(${MICROPY_DIR}/py/py.cmake)

if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    # Enable extmod components that will be configured by extmod.cmake.
    # A board may also have enabled additional components.
    set(MICROPY_PY_BTREE ON)

    include(${MICROPY_DIR}/py/usermod.cmake)
    include(${MICROPY_DIR}/extmod/extmod.cmake)
endif()

list(APPEND MICROPY_QSTRDEFS_PORT
    ${MICROPY_PORT_DIR}/qstrdefsport.h
)

list(APPEND MICROPY_SOURCE_SHARED
    ${MICROPY_DIR}/shared/readline/readline.c
    ${MICROPY_DIR}/shared/netutils/netutils.c
    ${MICROPY_DIR}/shared/timeutils/timeutils.c
    ${MICROPY_DIR}/shared/runtime/interrupt_char.c
    ${MICROPY_DIR}/shared/runtime/mpirq.c
    ${MICROPY_DIR}/shared/runtime/stdout_helpers.c
    ${MICROPY_DIR}/shared/runtime/sys_stdio_mphal.c
    ${MICROPY_DIR}/shared/runtime/pyexec.c
)


list(APPEND MICROPY_SOURCE_LIB
    ${MICROPY_DIR}/lib/littlefs/lfs1.c
    ${MICROPY_DIR}/lib/littlefs/lfs1_util.c
    ${MICROPY_DIR}/lib/littlefs/lfs2.c
    ${MICROPY_DIR}/lib/littlefs/lfs2_util.c
    ${MICROPY_DIR}/lib/mbedtls_errors/esp32_mbedtls_errors.c
    ${MICROPY_DIR}/lib/oofatfs/ff.c
    ${MICROPY_DIR}/lib/oofatfs/ffunicode.c
)


list(APPEND MICROPY_SOURCE_DRIVERS
    ${MICROPY_DIR}/drivers/bus/softspi.c
    ${MICROPY_DIR}/drivers/dht/dht.c
)


string(CONCAT GIT_SUBMODULES "${GIT_SUBMODULES} " lib/tinyusb)
if(MICROPY_PY_TINYUSB)
    set(TINYUSB_SRC "${MICROPY_DIR}/lib/tinyusb/src")
    string(TOUPPER OPT_MCU_${IDF_TARGET} tusb_mcu)

    list(APPEND MICROPY_DEF_TINYUSB
        CFG_TUSB_MCU=${tusb_mcu}
    )

    list(APPEND MICROPY_SOURCE_TINYUSB
        ${TINYUSB_SRC}/tusb.c
        ${TINYUSB_SRC}/common/tusb_fifo.c
        ${TINYUSB_SRC}/device/usbd.c
        ${TINYUSB_SRC}/device/usbd_control.c
        ${TINYUSB_SRC}/class/cdc/cdc_device.c
        ${TINYUSB_SRC}/portable/synopsys/dwc2/dcd_dwc2.c
        ${MICROPY_DIR}/shared/tinyusb/mp_usbd.c
        ${MICROPY_DIR}/shared/tinyusb/mp_usbd_cdc.c
        ${MICROPY_DIR}/shared/tinyusb/mp_usbd_descriptor.c
    )

    list(APPEND MICROPY_INC_TINYUSB
        ${TINYUSB_SRC}
        ${MICROPY_DIR}/shared/tinyusb/
    )

    list(APPEND MICROPY_LINK_TINYUSB
        -Wl,--wrap=dcd_event_handler
    )
endif()


list(APPEND MICROPY_SOURCE_PORT
    ${MICROPY_PORT_DIR}/main.c
    ${MICROPY_PORT_DIR}/multicast.c
    ${MICROPY_PORT_DIR}/help.c
    ${MICROPY_PORT_DIR}/build/lv_mpy.c
    ${MICROPY_PORT_DIR}/network_common.c
    ${MICROPY_PORT_DIR}/esp_lcd_touch.c
    ${MICROPY_PORT_DIR}/modsocket.c
    ${MICROPY_PORT_DIR}/mphalport.c
    ${MICROPY_PORT_DIR}/usb.c

    ${MICROPY_ESP32_DIR}/panichandler.c
    ${MICROPY_ESP32_DIR}/adc.c
    ${MICROPY_ESP32_DIR}/uart.c
    ${MICROPY_ESP32_DIR}/usb_serial_jtag.c
    ${MICROPY_ESP32_DIR}/gccollect.c
    ${MICROPY_ESP32_DIR}/fatfs_port.c
    ${MICROPY_ESP32_DIR}/machine_bitstream.c
    ${MICROPY_ESP32_DIR}/machine_sdcard.c
    ${MICROPY_ESP32_DIR}/machine_timer.c
    ${MICROPY_ESP32_DIR}/machine_pin.c
    ${MICROPY_ESP32_DIR}/machine_touchpad.c
    ${MICROPY_ESP32_DIR}/machine_dac.c
    ${MICROPY_ESP32_DIR}/machine_i2c.c
    ${MICROPY_ESP32_DIR}/network_lan.c
    ${MICROPY_ESP32_DIR}/network_wlan.c
    ${MICROPY_ESP32_DIR}/modesp.c
    ${MICROPY_ESP32_DIR}/esp32_nvs.c
    ${MICROPY_ESP32_DIR}/esp32_partition.c
    ${MICROPY_ESP32_DIR}/esp32_rmt.c
    ${MICROPY_ESP32_DIR}/esp32_ulp.c
    ${MICROPY_ESP32_DIR}/modesp32.c
    ${MICROPY_ESP32_DIR}/machine_hw_spi.c
    ${MICROPY_ESP32_DIR}/mpthreadport.c
    ${MICROPY_ESP32_DIR}/machine_rtc.c
    ${MICROPY_ESP32_DIR}/machine_sdcard.c
    ${MICROPY_ESP32_DIR}/modespnow.c
)

list(TRANSFORM MICROPY_SOURCE_BOARD PREPEND ${MICROPY_PORT_DIR}/)

list(APPEND MICROPY_SOURCE_PORT ${CMAKE_BINARY_DIR}/pins.c)


list(APPEND MICROPY_SOURCE_EXTMOD 
    ${TULIP_SHARED_DIR}/modtulip.c
    ${TULIP_SHARED_DIR}/polyfills.c
    ${TULIP_SHARED_DIR}/smallfont.c
    ${TULIP_SHARED_DIR}/display.c
    ${TULIP_SHARED_DIR}/bresenham.c
    ${TULIP_SHARED_DIR}/tulip_helpers.c
    ${TULIP_SHARED_DIR}/editor.c
    ${TULIP_SHARED_DIR}/keyscan.c
    ${TULIP_SHARED_DIR}/help.c
    ${TULIP_SHARED_DIR}/alles.c
    ${TULIP_SHARED_DIR}/ui.c
    ${TULIP_SHARED_DIR}/midi.c
    ${TULIP_SHARED_DIR}/sounds.c
    ${TULIP_SHARED_DIR}/tsequencer.c
    ${TULIP_SHARED_DIR}/lodepng.c
    ${TULIP_SHARED_DIR}/lvgl_u8g2.c
    ${TULIP_SHARED_DIR}/u8fontdata.c
    ${TULIP_SHARED_DIR}/u8g2_fonts.c
    ${AMY_DIR}/src/dsps_biquad_f32_ae32.S
    ${AMY_DIR}/src/algorithms.c
    ${AMY_DIR}/src/custom.c
    ${AMY_DIR}/src/patches.c
    ${AMY_DIR}/src/custom.c
    ${AMY_DIR}/src/amy.c
    ${AMY_DIR}/src/delay.c
    ${AMY_DIR}/src/envelope.c
    ${AMY_DIR}/src/filters.c
    ${AMY_DIR}/src/oscillators.c
    ${AMY_DIR}/src/transfer.c
    ${AMY_DIR}/src/sequencer.c
    ${AMY_DIR}/src/partials.c
    ${AMY_DIR}/src/pcm.c
    ${AMY_DIR}/src/log2_exp2.c
    ${ULAB_DIR}/scipy/integrate/integrate.c
    ${ULAB_DIR}/scipy/linalg/linalg.c
    ${ULAB_DIR}/scipy/optimize/optimize.c
    ${ULAB_DIR}/scipy/signal/signal.c
    ${ULAB_DIR}/scipy/special/special.c
    ${ULAB_DIR}/ndarray_operators.c
    ${ULAB_DIR}/ulab_tools.c
    ${ULAB_DIR}/ndarray.c
    ${ULAB_DIR}/numpy/ndarray/ndarray_iter.c
    ${ULAB_DIR}/ndarray_properties.c
    ${ULAB_DIR}/numpy/approx.c
    ${ULAB_DIR}/numpy/bitwise.c
    ${ULAB_DIR}/numpy/compare.c
    ${ULAB_DIR}/numpy/carray/carray.c
    ${ULAB_DIR}/numpy/carray/carray_tools.c
    ${ULAB_DIR}/numpy/create.c
    ${ULAB_DIR}/numpy/fft/fft.c
    ${ULAB_DIR}/numpy/fft/fft_tools.c
    ${ULAB_DIR}/numpy/filter.c
    ${ULAB_DIR}/numpy/io/io.c
    ${ULAB_DIR}/numpy/linalg/linalg.c
    ${ULAB_DIR}/numpy/linalg/linalg_tools.c
    ${ULAB_DIR}/numpy/numerical.c
    ${ULAB_DIR}/numpy/poly.c
    ${ULAB_DIR}/numpy/random/random.c
    ${ULAB_DIR}/numpy/stats.c
    ${ULAB_DIR}/numpy/transform.c
    ${ULAB_DIR}/numpy/vector.c
    ${ULAB_DIR}/numpy/numpy.c
    ${ULAB_DIR}/scipy/scipy.c
    ${ULAB_DIR}/user/user.c
    ${ULAB_DIR}/utils/utils.c
    ${ULAB_DIR}/ulab.c
)

list(APPEND MICROPY_SOURCE_QSTR
    ${MICROPY_SOURCE_PY}
    ${MICROPY_SOURCE_EXTMOD}
    ${MICROPY_SOURCE_USERMOD}
    ${MICROPY_SOURCE_SHARED}
    ${MICROPY_SOURCE_LIB}
    ${MICROPY_SOURCE_PORT}
    ${MICROPY_SOURCE_BOARD}
    ${MICROPY_SOURCE_TINYUSB}
)

list(APPEND IDF_COMPONENTS
    app_update
    bootloader_support
    #bt
    driver
    esp_driver_tsens
    esp_adc
    esp_app_format
    esp_bootloader_format
    esp_common
    esp_eth
    esp_driver_uart
    esp_driver_i2s
    esp_driver_i2c
    esp_driver_sdmmc
    esp_driver_sdspi
    esp_driver_spi
    esp_driver_gpio
    esp_driver_ledc
    esp_event
    esp_hw_support
    esp_lcd
    esp_mm
    esp_netif
    esp_partition
    esp_pm
    esp_psram
    esp_ringbuf
    esp_rom
    esp_system
    esp_timer
    esp_wifi
    freertos
    hal
    heap
    log
    lwip
    mbedtls
    newlib
    nvs_flash
    sdmmc
    soc
    spi_flash
    ulp
    usb
    vfs
    xtensa
)

# Register the main IDF component.
idf_component_register(
    SRCS
        ${MICROPY_SOURCE_PY}
        ${MICROPY_SOURCE_EXTMOD}
        ${MICROPY_SOURCE_SHARED}
        ${MICROPY_SOURCE_LIB}
        ${MICROPY_SOURCE_DRIVERS}
        ${MICROPY_SOURCE_PORT}
        ${MICROPY_SOURCE_BOARD}
        ${MICROPY_SOURCE_TINYUSB}
        ${LVGL_SOURCES}
    INCLUDE_DIRS
        .
        ${MICROPY_INC_CORE}
        ${MICROPY_INC_USERMOD}
        ${MICROPY_ESP32_DIR}
        ${MICROPY_BOARD_DIR}
        ${CMAKE_BINARY_DIR}
        ${MICROPY_INC_TINYUSB}
        ../../tulip/shared
        ../../amy/src
        ../../tulip/shared/ulab/code
        ${LV_BINDING_DIR}
        ${LVGL_DIR}/src
    REQUIRES
        ${IDF_COMPONENTS}
)

                    
# Set the MicroPython target as the current (main) IDF component target.
set(MICROPY_TARGET ${COMPONENT_TARGET})

# Define mpy-cross flags, for use with frozen code.
set(MICROPY_CROSS_FLAGS -march=xtensawin)

# Set compile options for this port.
target_compile_definitions(${MICROPY_TARGET} PUBLIC
    ${MICROPY_DEF_CORE}
    MICROPY_ESP_IDF_4=1
    MICROPY_VFS_FAT=1
    MICROPY_VFS_LFS2=1
    MICROPY_VFS_LFS1=1
    MODULE_ULAB_ENABLED=1
    FFCONF_H=\"${MICROPY_OOFATFS_DIR}/ffconf.h\"
    LFS1_NO_MALLOC LFS1_NO_DEBUG LFS1_NO_WARN LFS1_NO_ERROR LFS1_NO_ASSERT
    LFS2_NO_MALLOC LFS2_NO_ASSERT
    ESP_PLATFORM
    TULIP
    #AMY_DEBUG
    LV_CONF_INCLUDE_SIMPLE
    ${BOARD_DEFINITION1}
    ${BOARD_DEFINITION2}
    ${MICROPY_DEF_TINYUSB}
)

#LFS2_NO_DEBUG LFS2_NO_WARN LFS2_NO_ERROR 

# Disable some warnings to keep the build output clean.
target_compile_options(${MICROPY_TARGET} PUBLIC
    -Wno-clobbered
    -Wno-uninitialized
    -Wno-dangling-pointer
    -Wno-deprecated-declarations
    -Wno-missing-field-initializers
    -Wno-unused-const-variable
    -fsingle-precision-constant
    -Wno-strict-aliasing
    -DESP_PLATFORM
    -DAMY_HAS_AUDIO_IN
    -DSTATIC=static
    -DLFS2_NO_DEBUG
)

# Additional include directories needed for private NimBLE headers.
#target_include_directories(${MICROPY_TARGET} PUBLIC
#    ${IDF_PATH}/components/bt/host/nimble/nimble
#)

target_link_options(${MICROPY_TARGET} PUBLIC
     ${MICROPY_LINK_TINYUSB}
)

# Add additional extmod and usermod components.
target_link_libraries(${MICROPY_TARGET} micropy_extmod_btree)
target_link_libraries(${MICROPY_TARGET} usermod)

# Collect all of the include directories and compile definitions for the IDF components.
foreach(comp ${IDF_COMPONENTS})
    micropy_gather_target_properties(__idf_${comp})
    micropy_gather_target_properties(${comp})
endforeach()


# Include the main MicroPython cmake rules.
include(${MICROPY_DIR}/py/mkrules.cmake)



# Generate source files for named pins (requires mkrules.cmake for MICROPY_GENHDR_DIR).

set(GEN_PINS_PREFIX "${MICROPY_ESP32_DIR}/boards/pins_prefix.c")
set(GEN_PINS_MKPINS "${MICROPY_ESP32_DIR}/boards/make-pins.py")
set(GEN_PINS_SRC "${CMAKE_BINARY_DIR}/pins.c")
set(GEN_PINS_HDR "${MICROPY_GENHDR_DIR}/pins.h")

if(EXISTS "${MICROPY_BOARD_DIR}/pins.csv")
    set(GEN_PINS_BOARD_CSV "${MICROPY_BOARD_DIR}/pins.csv")
    set(GEN_PINS_BOARD_CSV_ARG --board-csv "${GEN_PINS_BOARD_CSV}")
endif()

target_sources(${MICROPY_TARGET} PRIVATE ${GEN_PINS_HDR})

add_custom_command(
    OUTPUT ${GEN_PINS_SRC} ${GEN_PINS_HDR}
    COMMAND ${Python3_EXECUTABLE} ${GEN_PINS_MKPINS} ${GEN_PINS_BOARD_CSV_ARG}
        --prefix ${GEN_PINS_PREFIX} --output-source ${GEN_PINS_SRC} --output-header ${GEN_PINS_HDR}
    DEPENDS
        ${MICROPY_MPVERSION}
        ${GEN_PINS_MKPINS}
        ${GEN_PINS_BOARD_CSV}
        ${GEN_PINS_PREFIX}
    VERBATIM
    COMMAND_EXPAND_LISTS
)




