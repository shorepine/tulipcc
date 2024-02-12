WHICH_ARCH ?= $(shell arch)


ifeq ($(WHICH_ARCH), arm64)
	ARCHFLAGS = -target arm64-macos11
	HOMEBREW = /opt/homebrew
else
	ARCHFLAGS = -target x86_64-apple-macos10.15
	# On x86, homebrew FFI seems to have two locations
	# /usr/local/homebrew/opt/libffi/include/ffi.h
	# /usr/local/opt/libffi/include/ffi.h
	# Check which it is
	ifneq ("$(wildcard /usr/local/opt/libffi/include/ffi.h)","")
		HOMEBREW = /usr/local
	else
		HOMEBREW = /usr/local/homebrew
	endif
endif

# Select the variant to build for.
VARIANT ?= standard

# Set parallel flag to # of CPUs
CPUS ?= $(shell sysctl -n hw.ncpu || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

# If the build directory is not given, make it reflect the variant name.
BUILD ?= build-$(VARIANT)/tulip/obj

TOP = ../../micropython

VARIANT_DIR ?= ../../micropython/ports/unix/variants/$(VARIANT)

include $(TOP)/py/mkenv.mk
include ../shared_desktop/mpconfigport.mk
include $(VARIANT_DIR)/mpconfigvariant.mk

# Use the default frozen manifest, variants may override this.
FROZEN_MANIFEST ?= manifest.py

# This should be configured by the mpconfigvariant.mk
PROG = tulip.$(WHICH_ARCH)

# qstr definitions (must come before including py.mk)
QSTR_DEFS = $(TOP)/ports/unix/qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = $(VARIANT_DIR)/mpconfigvariant.h

# OS name, for simple autoconfig
UNAME_S := $(shell uname -s)

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk


# LVGL stuff
LVGL_BINDING_DIR = $(TOP)/../lv_binding_micropython
LVGL_DIR = $(LVGL_BINDING_DIR)/lvgl
LVGL_GENERIC_DRV_DIR = $(LVGL_BINDING_DIR)/driver/generic
INC += -I$(LVGL_BINDING_DIR) -I.
ALL_LVGL_SRC = $(shell find $(LVGL_DIR) -type f) lv_conf.h
LVGL_PP = $(BUILD)/lvgl/lvgl.pp.c
LVGL_MPY = $(BUILD)/lvgl/lv_mpy.c
LVGL_MPY_METADATA = $(BUILD)/lvgl/lv_mpy.json
QSTR_GLOBAL_DEPENDENCIES += $(LVGL_MPY)
CFLAGS_MOD += $(LV_CFLAGS) 
SRC_C += $(shell find $(LVGL_DIR)/src -type f -name '*.c')
#CSRCS += $(shell find $(LVGL_PATH)/demos -type f -name '*.c')
#CSRCS += $(shell find $(LVGL_PATH)/examples -type f -name '*.c')
CXXEXT := .cpp
SRC_CPP += $(shell find $(LVGL_DIR)/src/libs/thorvg -type f -name '*${CXXEXT}')
CFLAGS += -DLV_CONF_INCLUDE_SIMPLE

$(LVGL_MPY): $(ALL_LVGL_SRC) $(LVGL_BINDING_DIR)/gen/gen_mpy.py 
	$(ECHO) "LVGL-GEN $@"
	$(Q)cp lv_conf.h $(LVGL_BINDING_DIR)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CPP) $(LV_CFLAGS) -I $(LVGL_BINDING_DIR)/pycparser/utils/fake_libc_include $(INC) $(LVGL_DIR)/lvgl.h > $(LVGL_PP)
	$(Q)$(PYTHON) $(LVGL_BINDING_DIR)/gen/gen_mpy.py -M lvgl -MP lv -MD $(LVGL_MPY_METADATA) -E $(LVGL_PP) $(LVGL_DIR)/lvgl.h > $@



#GIT_SUBMODULES += lib/axtls lib/berkeley-db-1.xx lib/libffi lib/mbedtls lib/micropython-lib 

INC +=  -I.
INC +=  -I$(TOP)
INC += -I$(BUILD)
INC += -I../shared_desktop/
INC += -I$(TOP)/ports/unix
INC += -I../shared/
INC += -I../../amy/src/
INC += -I$(TOP)/lib/mbedtls/include
INC += -I$(TOP)/../lv_binding_micropython/lvgl/src

INC += -I./SDL2.framework/Headers

# compiler settings
CWARN = -Wall -Werror
CWARN += -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wpointer-arith -Wdouble-promotion -Wfloat-conversion -Wno-missing-declarations
CFLAGS += $(INC) $(CWARN) -std=gnu99 -DUNIX $(CFLAGS_MOD) $(COPT) -I$(VARIANT_DIR) $(CFLAGS_EXTRA) 
CFLAGS += -DTULIP_DESKTOP
#CFLAGS += -DAMY_DEBUG

CFLAGS += $(ARCHFLAGS) 
# Debugging/Optimization
ifdef DEBUG
COPT ?= -Og
else
COPT ?= -Os
COPT += -DNDEBUG
endif

# Remove unused sections.
COPT += -fdata-sections -ffunction-sections

# Always enable symbols -- They're occasionally useful, and don't make it into the
# final .bin/.hex/.dfu so the extra size doesn't matter.
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

# On OSX, 'gcc' is a symlink to clang unless a real gcc is installed.
# The unix port of MicroPython on OSX must be compiled with clang,
# while cross-compile ports require gcc, so we test here for OSX and
# if necessary override the value of 'CC' set in py/mkenv.mk
ifeq ($(UNAME_S),Darwin)
ifeq ($(MICROPY_FORCE_32BIT),1)
CC = clang -m32
else
CC = clang
endif
# Use clang syntax for map file
LDFLAGS_ARCH = -Wl,-map,$@.map -Wl,-dead_strip
else
# Use gcc syntax for map file
LDFLAGS_ARCH = -Wl,-Map=$@.map,--cref -Wl,--gc-sections 
endif
LDFLAGS += $(LDFLAGS_MOD) $(LDFLAGS_ARCH) -lm $(LDFLAGS_EXTRA)
LDFLAGS += -F. -framework SDL2 -framework CoreFoundation -framework CoreMIDI  -framework AudioToolbox -framework CoreAudio -framework Cocoa -lstdc++  $(ARCHFLAGS)
CFLAGS += -DVIRTUAL_MIDI

# Flags to link with pthread library
LIBPTHREAD = -lpthread

ifeq ($(MICROPY_FORCE_32BIT),1)
# Note: you may need to install i386 versions of dependency packages,
# starting with linux-libc-dev:i386
ifeq ($(MICROPY_PY_FFI),1)
ifeq ($(UNAME_S),Linux)
CFLAGS_MOD += -I/usr/include/i686-linux-gnu
endif
endif
endif

ifeq ($(MICROPY_USE_READLINE),1)
INC += -I$(TOP)/shared/readline
CFLAGS_MOD += -DMICROPY_USE_READLINE=1
SHARED_SRC_C_EXTRA += readline/readline.c
endif
ifeq ($(MICROPY_PY_TERMIOS),1)
CFLAGS_MOD += -DMICROPY_PY_TERMIOS=1
SRC_MOD += $(MICROPY_PORT_DIR)/modtermios.c
endif
ifeq ($(MICROPY_PY_SOCKET),1)
CFLAGS_MOD += -DMICROPY_PY_SOCKET=1
SRC_MOD += $(MICROPY_PORT_DIR)/modsocket.c
endif
ifeq ($(MICROPY_PY_THREAD),1)
CFLAGS_MOD += -DMICROPY_PY_THREAD=1 -DMICROPY_PY_THREAD_GIL=0
LDFLAGS_MOD += $(LIBPTHREAD)
endif

include ../shared/tulip.mk

MICROPY_PORT_DIR=../../micropython/ports/unix

ifeq ($(MICROPY_PY_FFI),1)

ifeq ($(MICROPY_STANDALONE),1)
LIBFFI_CFLAGS_MOD := -I$(shell ls -1d $(BUILD)/lib/libffi/out/lib/libffi-*/include)
 ifeq ($(MICROPY_FORCE_32BIT),1)
  LIBFFI_LDFLAGS_MOD = $(BUILD)/lib/libffi/out/lib32/libffi.a
 else
  LIBFFI_LDFLAGS_MOD = $(BUILD)/lib/libffi/out/lib/libffi.a
 endif
else
LIBFFI_CFLAGS_MOD := $(shell pkg-config --cflags libffi)
LIBFFI_LDFLAGS_MOD := $(shell pkg-config --libs libffi)
endif


# libffi is installed in system on 10.12, so force use of homebrew version 
LIBFFI_LDFLAGS_MOD = $(HOMEBREW)/opt/libffi/lib/libffi.a
LIBFFI_CFLAGS_MOD := -I$(HOMEBREW)/opt/libffi/include


ifeq ($(UNAME_S),Linux)
LIBFFI_LDFLAGS_MOD += -ldl
endif

CFLAGS_MOD += $(LIBFFI_CFLAGS_MOD) -DMICROPY_PY_FFI=1
LDFLAGS_MOD += $(LIBFFI_LDFLAGS_MOD)
SRC_MOD += $(MICROPY_PORT_DIR)/modffi.c
endif

ifeq ($(MICROPY_PY_JNI),1)
# Path for 64-bit OpenJDK, should be adjusted for other JDKs
CFLAGS_MOD += -I/usr/lib/jvm/java-7-openjdk-amd64/include -DMICROPY_PY_JNI=1
SRC_MOD += $(MICROPY_PORT_DIR)/modjni.c
endif


# source files
SRC_C += \
	main.c \
	../shared_desktop/unix_mphal.c \
	../shared_desktop/unix_display.c \
	../shared_desktop/multicast.c \
	../shared_desktop/mpthreadport.c \
	../../amy/src/libminiaudio-audio.c \
	$(MICROPY_PORT_DIR)/gccollect.c \
	$(MICROPY_PORT_DIR)/input.c \
	$(MICROPY_PORT_DIR)/modmachine.c \
	$(MICROPY_PORT_DIR)/modselect.c \
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

SRC_M = virtualmidi.m

OBJ = $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_M:.m=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SHARED_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(EXTMOD_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SRC_CXX) $(SHARED_SRC_C) $(EXTMOD_SRC_C) 
# Append any auto-generated sources that are needed by sources listed in
# SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

ifneq ($(FROZEN_MANIFEST),)
# To use frozen code create a manifest.py file with a description of files to
# freeze, then invoke make with FROZEN_MANIFEST=manifest.py (be sure to build from scratch).
CFLAGS += -DMICROPY_QSTR_EXTRA_POOL=mp_qstr_frozen_const_pool
CFLAGS += -DMICROPY_MODULE_FROZEN_MPY
CFLAGS += -DMPZ_DIG_SIZE=16 # force 16 bits to work on both 32 and 64 bit archs
CFLAGS += -DMICROPY_MODULE_FROZEN_STR
endif

CPU_BRAND_NAME=`sysctl -n machdep.cpu.brand_string`
CFLAGS += -DMICROPY_HW_MCU_NAME="\"$(CPU_BRAND_NAME)\""
CFLAGS += -Wno-double-promotion -Wno-unused-function -Wno-unused-variable
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

#undefine compile_c

define compile_c
$(ECHO) "CC $<"
$(Q)$(CC) $(CFLAGS) -c -MD -o $@ $< || (echo -e $(HELP_BUILD_ERROR); false)
@# The following fixes the dependency file.
@# See http://make.paulandlesley.org/autodep.html for details.
@# Regex adjusted from the above to play better with Windows paths, etc.
@$(CP) $(@:.o=.d) $(@:.o=.P); \
  $(SED) -e 's/#.*//' -e 's/^.*:  *//' -e 's/ *\\$$//' \
      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
  $(RM) -f $(@:.o=.d)
endef

$(BUILD)/%.o: %.m
	$(ECHO) "M $<"
	clang  -I$(INC) $(CFLAGS) -c -o $@ $<

# Value of configure's --host= option (required for cross-compilation).
# Deduce it from CROSS_COMPILE by default, but can be overridden.
ifneq ($(CROSS_COMPILE),)
CROSS_COMPILE_HOST = --host=$(patsubst %-,%,$(CROSS_COMPILE))
else
CROSS_COMPILE_HOST =
endif

deplibs: libffi axtls

libffi: $(BUILD)/lib/libffi/include/ffi.h

$(TOP)/lib/libffi/configure: $(TOP)/lib/libffi/autogen.sh
	cd $(TOP)/lib/libffi; ./autogen.sh

# install-exec-recursive & install-data-am targets are used to avoid building
# docs and depending on makeinfo
$(BUILD)/lib/libffi/include/ffi.h: $(TOP)/lib/libffi/configure
	mkdir -p $(BUILD)/lib/libffi; cd $(BUILD)/lib/libffi; \
	$(abspath $(TOP))/lib/libffi/configure $(CROSS_COMPILE_HOST) --prefix=$$PWD/out --disable-structs CC="$(CC)" CXX="$(CXX)" LD="$(LD)" CFLAGS="-Os -fomit-frame-pointer -fstrict-aliasing -ffast-math -fno-exceptions"; \
	$(MAKE) install-exec-recursive; $(MAKE) -C include install-data-am

PREFIX = /usr/local
BINDIR = $(DESTDIR)$(PREFIX)/bin

install: $(PROG)
	install -d $(BINDIR)
	install $(PROG) $(BINDIR)/$(PROG)

uninstall:
	-rm $(BINDIR)/$(PROG)
