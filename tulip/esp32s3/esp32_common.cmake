# Set location of base MicroPython directory.
if(NOT MICROPY_DIR)
    get_filename_component(MICROPY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../micropython ABSOLUTE)
endif()

# Set location of the ESP32 port directory.
if(NOT MICROPY_PORT_DIR)
    get_filename_component(MICROPY_PORT_DIR ${CMAKE_CURRENT_LIST_DIR}/ ABSOLUTE)
endif()


# Set location of the tulip shared directory.
if(NOT TULIP_SHARED_DIR)
    get_filename_component(TULIP_SHARED_DIR ${CMAKE_CURRENT_LIST_DIR}/../shared ABSOLUTE)
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
    ${MICROPY_DIR}/shared/runtime/stdout_helpers.c
    ${MICROPY_DIR}/shared/runtime/sys_stdio_mphal.c
    ${MICROPY_DIR}/shared/runtime/pyexec.c
)

list(APPEND MICROPY_SOURCE_LIB
    ${MICROPY_DIR}/lib/littlefs/lfs1.c
    ${MICROPY_DIR}/lib/littlefs/lfs1_util.c
    ${MICROPY_DIR}/lib/littlefs/lfs2.c
    ${MICROPY_DIR}/lib/littlefs/lfs2_util.c
    #${MICROPY_DIR}/lib/mbedtls_errors/esp32_mbedtls_errors.c
    ${MICROPY_DIR}/lib/oofatfs/ff.c
    ${MICROPY_DIR}/lib/oofatfs/ffunicode.c
)

list(APPEND MICROPY_SOURCE_DRIVERS
    ${MICROPY_DIR}/drivers/bus/softspi.c
    ${MICROPY_DIR}/drivers/dht/dht.c
)


list(APPEND MICROPY_SOURCE_PORT
    multicast.c
    mphalport.c
    network_common.c
    main.c
    uart.c
    help.c
    build/lv_mpy.c
    usb_serial_jtag.c
    gccollect.c
    fatfs_port.c
    machine_bitstream.c
    machine_sdcard.c
    machine_timer.c
    machine_pin.c
    machine_touchpad.c
    machine_adc.c
    machine_adcblock.c
    machine_dac.c
    machine_i2c.c
    machine_uart.c
    modmachine.c
    network_lan.c
    network_wlan.c
    network_ppp.c
    ppp_set_auth.c
    #mpnimbleport.c
    modsocket.c
    modesp.c
    esp32_nvs.c
    esp32_partition.c
    esp32_rmt.c
    esp32_ulp.c
    modesp32.c
    machine_hw_spi.c
    machine_wdt.c
    mpthreadport.c
    machine_rtc.c
    machine_sdcard.c
    modespnow.c
    usb.c
)
list(TRANSFORM MICROPY_SOURCE_PORT PREPEND ${MICROPY_PORT_DIR}/)

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
    ${TULIP_SHARED_DIR}/sequencer.c
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
    ${AMY_DIR}/src/partials.c
    ${AMY_DIR}/src/pcm.c
    ${AMY_DIR}/src/log2_exp2.c
)

list(APPEND MICROPY_SOURCE_QSTR
    ${MICROPY_SOURCE_PY}
    ${MICROPY_SOURCE_EXTMOD}
    ${MICROPY_SOURCE_USERMOD}
    ${MICROPY_SOURCE_SHARED}
    ${MICROPY_SOURCE_LIB}
    ${MICROPY_SOURCE_PORT}
    ${MICROPY_SOURCE_BOARD}
)

list(APPEND IDF_COMPONENTS
    app_update
    bootloader_support
    #bt
    driver
    esp_adc
    esp_app_format
    esp_bootloader_format
    esp_common
    esp_eth
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
        ${LVGL_SOURCES}
    INCLUDE_DIRS
        ../../tulip/esp32s3
        ../../tulip/esp32s3/managed_components/espressif__esp_lcd_touch_gt911/include
        ../../tulip/esp32s3/managed_components/espressif__esp_lcd_touch/include
        ${MICROPY_INC_CORE}
        ${MICROPY_INC_USERMOD}
        #${MICROPY_PORT_DIR}
        ${MICROPY_BOARD_DIR}
        ${CMAKE_BINARY_DIR}
        ../../tulip/shared
        ../../amy/src
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
    FFCONF_H=\"${MICROPY_OOFATFS_DIR}/ffconf.h\"
    LFS1_NO_MALLOC LFS1_NO_DEBUG LFS1_NO_WARN LFS1_NO_ERROR LFS1_NO_ASSERT
    LFS2_NO_MALLOC LFS2_NO_ASSERT
    ESP_PLATFORM
    TULIP
    #AMY_DEBUG
    LV_CONF_INCLUDE_SIMPLE
    ${BOARD_DEFINITION1}
    ${BOARD_DEFINITION2}
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
)

# Additional include directories needed for private NimBLE headers.
#target_include_directories(${MICROPY_TARGET} PUBLIC
#    ${IDF_PATH}/components/bt/host/nimble/nimble
#)

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
