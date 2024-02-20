# Top-level cmake file for building MicroPython on ESP32.

cmake_minimum_required(VERSION 3.12)

execute_process(COMMAND bash -c "../shared/grab_submodules.sh"
                    WORKING_DIRECTORY ".."
                    OUTPUT_VARIABLE GIT_SUBMOD_RESULT)

# We have to do some nasty stuff to get the LVGL MP submodule compiled


# First, we copy over our version of lv_conf.h to the place it's expected to be found
execute_process(COMMAND bash -c "cp ../../shared/lv_conf.h ../../../lv_binding_micropython/"
                    WORKING_DIRECTORY "."
                )

# Then our fonts
execute_process(COMMAND bash -c "cp ../../shared/lv_fonts/*.c ../../../lv_binding_micropython/lvgl/src/font/"
                    WORKING_DIRECTORY "."
                )

# Then we generate a lvgl.pp.c file, which is just the compiler preprocessor running in this env
execute_process(COMMAND bash -c "xtensa-esp32s3-elf-gcc -E  -I ../../../lv_binding_micropython/pycparser/utils/fake_libc_include -I../../../lv_binding_micropython -I. -I../../../lv_binding_micropython/lvgl/src ../../../lv_binding_micropython/lvgl/lvgl.h > ../build/lvgl.pp.c"
                    WORKING_DIRECTORY "."
                )

# Then we run a python script which generates a MP module for LVGL based on the source files (and your conf and stuff)
# This gets compiled into Tulip 
execute_process(COMMAND bash -c "python3 ../../../lv_binding_micropython/gen/gen_mpy.py -M lvgl -MP lv -MD ../build/lv_mpy.json -E ../build/lvgl.pp.c ../../../lv_binding_micropython/lvgl/lvgl.h > ../build/lv_mpy.c"
                    WORKING_DIRECTORY "."
                )

# Turn this on for debugging submodules
#file(WRITE "submod" "${GIT_SUBMOD_RESULT}")


# Set the board if it's not already set.
if(NOT MICROPY_BOARD)
    set(MICROPY_BOARD N32R8)
endif()

# Set the board directory and check that it exists.
if(NOT MICROPY_BOARD_DIR)
    set(MICROPY_BOARD_DIR ${CMAKE_CURRENT_LIST_DIR}/boards/${MICROPY_BOARD})
endif()
if(NOT EXISTS ${MICROPY_BOARD_DIR}/mpconfigboard.cmake)
    message(FATAL_ERROR "Invalid MICROPY_BOARD specified: ${MICROPY_BOARD}")
endif()

# Define the output sdkconfig so it goes in the build directory.
set(SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig)

# Save the manifest file set from the cmake command line.
set(MICROPY_USER_FROZEN_MANIFEST ${MICROPY_FROZEN_MANIFEST})

# Include board config; this is expected to set (among other options):
# - SDKCONFIG_DEFAULTS
# - IDF_TARGET
include(${MICROPY_BOARD_DIR}/mpconfigboard.cmake)

# Set the frozen manifest file. Note if MICROPY_FROZEN_MANIFEST is set from the cmake
# command line, then it will override the default and any manifest set by the board.
if (MICROPY_USER_FROZEN_MANIFEST)
    set(MICROPY_FROZEN_MANIFEST ${MICROPY_USER_FROZEN_MANIFEST})
elseif (NOT MICROPY_FROZEN_MANIFEST)
    set(MICROPY_FROZEN_MANIFEST ${CMAKE_CURRENT_LIST_DIR}/boards/manifest.py)
endif()

# Concatenate all sdkconfig files into a combined one for the IDF to use.
file(WRITE ${CMAKE_BINARY_DIR}/sdkconfig.combined.in "")
foreach(SDKCONFIG_DEFAULT ${SDKCONFIG_DEFAULTS})
    file(READ ${SDKCONFIG_DEFAULT} CONTENTS)
    file(APPEND ${CMAKE_BINARY_DIR}/sdkconfig.combined.in "${CONTENTS}")
endforeach()
configure_file(${CMAKE_BINARY_DIR}/sdkconfig.combined.in ${CMAKE_BINARY_DIR}/sdkconfig.combined COPYONLY)
set(SDKCONFIG_DEFAULTS ${CMAKE_BINARY_DIR}/sdkconfig.combined)

# Include main IDF cmake file.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the location of the main component for the project (one per target).
set(EXTRA_COMPONENT_DIRS main components)

# Define the project.
project(micropython)
idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)
