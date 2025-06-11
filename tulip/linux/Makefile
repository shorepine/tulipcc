# Tulip linux, based on unix port makefile

DEBUG=1

# Select the variant to build for.
VARIANT ?= standard
MAKEFLAGS += --jobs=4

# If the build directory is not given, make it reflect the variant name.
BUILD ?= build-$(VARIANT)/tulip/obj

TOP = ../../micropython

VARIANT_DIR ?= ../../micropython/ports/unix/variants/$(VARIANT)

# If the build directory is not given, make it reflect the variant name.
BUILD ?= build-$(VARIANT)

include $(TOP)/py/mkenv.mk
include mpconfigport.mk
#include $(VARIANT_DIR)/mpconfigvariant.mk

# Use the default frozen manifest, variants may override this.
FROZEN_MANIFEST ?= variants/manifest.py

# This should be configured by the mpconfigvariant.mk
PROG = tulip

# qstr definitions (must come before including py.mk)
QSTR_DEFS = $(TOP)/ports/unix/qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = $(VARIANT_DIR)/mpconfigvariant.h

# OS name, for simple autoconfig
UNAME_S := $(shell uname -s)

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk


# LVGL stuff
LVGL_BINDING_DIR = $(TOP)/../lv_binding_micropython_tulip
LVGL_DIR = $(LVGL_BINDING_DIR)/lvgl
LVGL_GENERIC_DRV_DIR = $(LVGL_BINDING_DIR)/driver/generic
INC += -I$(LVGL_BINDING_DIR) -I.
ALL_LVGL_SRC = $(shell find $(LVGL_DIR) -type f) 
LVGL_PP = $(BUILD)/lvgl/lvgl.pp.c
LVGL_MPY = $(BUILD)/lvgl/lv_mpy.c
LVGL_MPY_METADATA = $(BUILD)/lvgl/lv_mpy.json
QSTR_GLOBAL_DEPENDENCIES += $(LVGL_MPY)
CFLAGS_MOD += $(LV_CFLAGS) 
SRC_C += $(shell find $(LVGL_DIR)/src -type f -name '*.c')
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE 

# This is for the lvgl stuff that uses STATIC 
CFLAGS += -DSTATIC=static

$(LVGL_MPY): $(ALL_LVGL_SRC) $(LVGL_BINDING_DIR)/gen/gen_mpy.py 
	$(ECHO) "LVGL-GEN $@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CPP) $(LV_CFLAGS) -DLVGL_PREPROCESS -I $(LVGL_BINDING_DIR)/pycparser/utils/fake_libc_include $(INC) $(LVGL_DIR)/lvgl.h > $(LVGL_PP)
	$(Q)$(PYTHON) $(LVGL_BINDING_DIR)/gen/gen_mpy.py -M lvgl -MP lv -MD $(LVGL_MPY_METADATA) -E $(LVGL_PP) $(LVGL_DIR)/lvgl.h > $@

#GIT_SUBMODULES += lib/axtls lib/berkeley-db-1.xx lib/libffi lib/mbedtls lib/micropython-lib 

INC +=  -I.
INC +=  -I$(TOP)
INC += -I$(BUILD)
INC += -I../shared/desktop/
INC += -I$(TOP)/ports/unix
INC += -I../shared/
INC += -I../../amy/src/
INC += -I$(TOP)/lib/mbedtls/include
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src/libs/lodepng
INC += -I/usr/include/SDL2


GIT_SUBMODULES += lib/berkeley-db-1.xx

INC +=  -I.
INC +=  -I$(TOP)
INC += -I$(BUILD)

# compiler settings

# compiler settings
CWARN = -Wall -Werror
CWARN += -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-old-style-declaration -Wno-unused-but-set-parameter -Wpointer-arith -Wdouble-promotion -Wno-float-conversion -Wno-missing-declarations  -Wno-unused-but-set-variable -Wno-sign-compare -Wno-gnu-variable-sized-type-not-at-end -Wno-undefined-internal
CFLAGS += $(INC) $(CWARN) -std=gnu99 -DUNIX $(CFLAGS_MOD) $(COPT) -I$(VARIANT_DIR) $(CFLAGS_EXTRA) 
CFLAGS += -DTULIP_DESKTOP
CFLAGS += $(ARCHFLAGS) 

# Debugging/Optimization
ifdef DEBUG
#COPT ?= -Og
else
#COPT ?= -Os
COPT += -DNDEBUG
endif

# Remove unused sections.
COPT += -fdata-sections -ffunction-sections -O0


# Note: Symbols and debug information will still be stripped from the final binary
# unless "DEBUG=1" or "STRIP=" is passed to make, see README.md for details.
CFLAGS += -g

ifndef DEBUG
# _FORTIFY_SOURCE is a feature in gcc/glibc which is intended to provide extra
# security for detecting buffer overflows. Some distros (Ubuntu at the very least)
# have it enabled by default.
#
# gcc already optimizes some printf calls to call puts and/or putchar. When
# _FORTIFY_SOURCE is enabled and compiling with -O1 or greater, then some
# printf calls will also be optimized to call __printf_chk (in glibc). Any
# printfs which get redirected to __printf_chk are then no longer synchronized
# with printfs that go through mp_printf.
#
# In MicroPython, we don't want to use the runtime library's printf but rather
# go through mp_printf, so that stdout is properly tied into streams, etc.
# This means that we either need to turn off _FORTIFY_SOURCE or provide our
# own implementation of __printf_chk. We've chosen to turn off _FORTIFY_SOURCE.
# It should also be noted that the use of printf in MicroPython is typically
# quite limited anyways (primarily for debug and some error reporting, etc
# in the unix version).
#
# Information about _FORTIFY_SOURCE seems to be rather scarce. The best I could
# find was this: https://securityblog.redhat.com/2014/03/26/fortify-and-you/
# Original patchset was introduced by
# https://gcc.gnu.org/ml/gcc-patches/2004-09/msg02055.html .
#
# Turning off _FORTIFY_SOURCE is only required when compiling with -O1 or greater
CFLAGS += -U _FORTIFY_SOURCE
endif

LDFLAGS_ARCH = -Wl,-Map=$@.map,--cref -Wl,--gc-sections
LDFLAGS += $(LDFLAGS_MOD) $(LDFLAGS_ARCH) -lm $(LDFLAGS_EXTRA)

# Flags to link with pthread library
LIBPTHREAD = -lpthread

ifeq ($(MICROPY_FORCE_32BIT),1)
# Note: you may need to install i386 versions of dependency packages,
# starting with linux-libc-dev:i386
ifeq ($(MICROPY_PY_FFI),1)
ifeq ($(UNAME_S),Linux)
CFLAGS += -I/usr/include/i686-linux-gnu
endif
endif
endif

ifeq ($(MICROPY_USE_READLINE),1)
INC += -I$(TOP)/shared/readline
CFLAGS += -DMICROPY_USE_READLINE=1
SHARED_SRC_C_EXTRA += readline/readline.c
endif
ifeq ($(MICROPY_PY_TERMIOS),1)
CFLAGS += -DMICROPY_PY_TERMIOS=1
endif
ifeq ($(MICROPY_PY_SOCKET),1)
CFLAGS += -DMICROPY_PY_SOCKET=1
SRC_MOD += $(MICROPY_PORT_DIR)/modsocket.c
endif
ifeq ($(MICROPY_PY_THREAD),1)
CFLAGS += -DMICROPY_PY_THREAD=1 -DMICROPY_PY_THREAD_GIL=0
LDFLAGS += $(LIBPTHREAD)
endif


include ../shared/tulip.mk

MICROPY_PORT_DIR=../../micropython/ports/unix

ifeq ($(MICROPY_PY_SSL),1)
ifeq ($(MICROPY_SSL_AXTLS),1)

endif
endif

# If the variant enables it, enable modbluetooth.
ifeq ($(MICROPY_PY_BLUETOOTH),1)
ifeq ($(MICROPY_BLUETOOTH_BTSTACK),1)
HAVE_LIBUSB := $(shell (which pkg-config > /dev/null && pkg-config --exists libusb-1.0) 2>/dev/null && echo '1')

# Figure out which BTstack transport to use.
ifeq ($(HAVE_LIBUSB),1)
# Default to btstack-over-usb.
MICROPY_BLUETOOTH_BTSTACK_USB ?= 1
else
# Fallback to HCI controller via a H4 UART (e.g. Zephyr on nRF) over a /dev/tty serial port.
MICROPY_BLUETOOTH_BTSTACK_H4 ?= 1
endif

SRC_BTSTACK_C += lib/btstack/platform/embedded/btstack_run_loop_embedded.c
endif
endif

ifeq ($(MICROPY_PY_FFI),1)

ifeq ($(MICROPY_STANDALONE),1)
# Build libffi from source.
GIT_SUBMODULES += lib/libffi
DEPLIBS += libffi
LIBFFI_CFLAGS := -I$(shell ls -1d $(BUILD)/lib/libffi/include)
 ifeq ($(MICROPY_FORCE_32BIT),1)
  LIBFFI_LDFLAGS = $(BUILD)/lib/libffi/out/lib32/libffi.a
 else
  LIBFFI_LDFLAGS = $(BUILD)/lib/libffi/out/lib/libffi.a
 endif
else
# Use system version of libffi.
LIBFFI_CFLAGS := $(shell pkg-config --cflags libffi)
LIBFFI_LDFLAGS := $(shell pkg-config --libs libffi)
endif



CFLAGS += $(LIBFFI_CFLAGS) -DMICROPY_PY_FFI=1
LDFLAGS += $(LIBFFI_LDFLAGS)
endif

ifeq ($(MICROPY_PY_JNI),1)
# Path for 64-bit OpenJDK, should be adjusted for other JDKs
CFLAGS += -I/usr/lib/jvm/java-7-openjdk-amd64/include -DMICROPY_PY_JNI=1
endif


ifeq ($(UNAME_S),Linux)
LDFLAGS += -ldl -lSDL2
endif


# source files
SRC_C += \
	main.c \
	../shared/desktop/unix_display.c \
	../shared/desktop/unix_mphal.c \
	../../amy/src/libminiaudio-audio.c \
	$(MICROPY_PORT_DIR)/mpthreadport.c \
	$(MICROPY_PORT_DIR)/gccollect.c \
	$(MICROPY_PORT_DIR)/input.c \
	$(MICROPY_PORT_DIR)/alloc.c \
	$(MICROPY_PORT_DIR)/fatfs_port.c \
	$(MICROPY_PORT_DIR)/mpbthciport.c \
	$(MICROPY_PORT_DIR)/mpbtstackport_common.c \
	$(MICROPY_PORT_DIR)/mpbtstackport_h4.c \
	$(MICROPY_PORT_DIR)/mpbtstackport_usb.c \
	$(MICROPY_PORT_DIR)/mpnimbleport.c \
	$(SRC_MOD) \
	$(wildcard $(VARIANT_DIR)/*.c)


#SRC_C += $(LVGL_PP)
SRC_C += $(LVGL_MPY)


SHARED_SRC_C += $(addprefix ../../micropython/shared/,\
	runtime/gchelper_generic.c \
	runtime/interrupt_char.c \
	timeutils/timeutils.c \
	runtime/pyexec.c \
	$(SHARED_SRC_C_EXTRA) \
	)

SRC_CXX += \
	$(SRC_MOD_CXX)

OBJ = $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SHARED_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(EXTMOD_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))



# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_CXX) $(SHARED_SRC_C) $(EXTMOD_SRC_C) 

ifneq ($(FROZEN_MANIFEST),)
CFLAGS += -DMPZ_DIG_SIZE=16 # force 16 bits to work on both 32 and 64 bit archs
endif


CPU_BRAND_NAME=linux
CFLAGS += -DMICROPY_HW_MCU_NAME="\"$(CPU_BRAND_NAME)\""
CFLAGS += -Wno-double-promotion -Wno-unused-function
CXXFLAGS += $(filter-out -Wno-double-promotion -Wmissing-prototypes -Wold-style-definition -std=gnu99,$(CFLAGS) $(CXXFLAGS_MOD))


ifeq ($(MICROPY_FORCE_32BIT),1)
RUN_TESTS_MPY_CROSS_FLAGS = --mpy-cross-flags='-march=x86'
endif

ifeq ($(CROSS_COMPILE),arm-linux-gnueabi-)
# Force disable error text compression when compiling for ARM as the compiler
# cannot optimise out the giant strcmp list generated for MP_MATCH_COMPRESSED.
# Checked on:
# arm-linux-gnueabi-gcc (Ubuntu/Linaro 7.5.0-3ubuntu1~18.04) 7.5.0
# arm-linux-gnueabi-gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
# See https://github.com/micropython/micropython/pull/7659 for details.
$(info Detected arm-linux-gnueabi-gcc. Disabling error message compression.)
MICROPY_ROM_TEXT_COMPRESSION = 0
endif

include $(TOP)/py/mkrules.mk

.PHONY: test test_full

test: $(BUILD)/$(PROG) $(TOP)/tests/run-tests.py
	$(eval DIRNAME=ports/$(notdir $(CURDIR)))
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py

test_full: $(BUILD)/$(PROG) $(TOP)/tests/run-tests.py
	$(eval DIRNAME=ports/$(notdir $(CURDIR)))
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py -d thread
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py --emit native
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py --via-mpy $(RUN_TESTS_MPY_CROSS_FLAGS) -d basics float micropython
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../$(DIRNAME)/$(BUILD)/$(PROG) ./run-tests.py --via-mpy $(RUN_TESTS_MPY_CROSS_FLAGS) --emit native -d basics float micropython
	cat $(TOP)/tests/basics/0prelim.py | ./$(BUILD)/$(PROG) | grep -q 'abc'

test_gcov: test_full
	gcov -o $(BUILD)/py $(TOP)/py/*.c
	gcov -o $(BUILD)/extmod $(TOP)/extmod/*.c


# Value of configure's --host= option (required for cross-compilation).
# Deduce it from CROSS_COMPILE by default, but can be overridden.
ifneq ($(CROSS_COMPILE),)
CROSS_COMPILE_HOST = --host=$(patsubst %-,%,$(CROSS_COMPILE))
else
CROSS_COMPILE_HOST =
endif

deplibs: $(DEPLIBS)

libffi: $(BUILD)/lib/libffi/include/ffi.h

$(TOP)/lib/libffi/configure: $(TOP)/lib/libffi/autogen.sh
	cd $(TOP)/lib/libffi; ./autogen.sh

# install-exec-recursive & install-data-am targets are used to avoid building
# docs and depending on makeinfo
$(BUILD)/lib/libffi/include/ffi.h: $(TOP)/lib/libffi/configure
	mkdir -p $(BUILD)/lib/libffi; cd $(BUILD)/lib/libffi; \
	$(abspath $(TOP))/lib/libffi/configure $(CROSS_COMPILE_HOST) --prefix=$$PWD/out --disable-shared --disable-structs CC="$(CC)" CXX="$(CXX)" LD="$(LD)" CFLAGS="-Os -fomit-frame-pointer -fstrict-aliasing -ffast-math -fno-exceptions"; \
	$(MAKE) install-exec-recursive; $(MAKE) -C include install-data-am

PREFIX = /usr/local
BINDIR = $(DESTDIR)$(PREFIX)/bin

install: $(BUILD)/$(PROG)
	install -d $(BINDIR)
	install $(BUILD)/$(PROG) $(BINDIR)/$(PROG)

uninstall:
	-rm $(BINDIR)/$(PROG)
