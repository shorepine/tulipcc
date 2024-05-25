ARCHFLAGS = -target arm64-apple-darwin11
HOMEBREW = /opt/homebrew

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
include mpconfigport.mk
include mpconfigvariant.mk

# Use the default frozen manifest, variants may override this.
FROZEN_MANIFEST ?= manifest.py

# This should be configured by the mpconfigvariant.mk
PROG = tulip.ios

# qstr definitions (must come before including py.mk)
QSTR_DEFS = $(TOP)/ports/unix/qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = mpconfigvariant.h

# OS name, for simple autoconfig
UNAME_S := $(shell uname -s)

# include py core make definitions
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

INC +=  -I.
INC +=  -I$(TOP)
INC += -I$(BUILD)
INC += -I../shared/desktop/
INC += -I$(TOP)/ports/unix
INC += -I../shared/
INC += -I../../amy/src/
INC += -I$(TOP)/lib/mbedtls/include
#INC += -I./SDL2.framework/Headers
INC += -I$(TOP)/../lv_binding_micropython/lvgl/src
INC += -I$(TOP)/../lv_binding_micropython/lvgl/src/libs/lodepng

INC += -I/Users/bwhitman/ioslib/SDL-release-2.28.1/include
INC += -I/Users/bwhitman/ioslib/SDL-release-2.28.1/src/video/uikit/




# LVGL stuff
LVGL_BINDING_DIR = $(TOP)/../lv_binding_micropython
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

$(LVGL_MPY): $(ALL_LVGL_SRC) $(LVGL_BINDING_DIR)/gen/gen_mpy.py 
	$(ECHO) "LVGL-GEN $@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CPP) $(LV_CFLAGS) -DLVGL_PREPROCESS -I $(LVGL_BINDING_DIR)/pycparser/utils/fake_libc_include $(INC) $(LVGL_DIR)/lvgl.h > $(LVGL_PP)
	$(Q)$(PYTHON) $(LVGL_BINDING_DIR)/gen/gen_mpy.py -M lvgl -MP lv -MD $(LVGL_MPY_METADATA) -E $(LVGL_PP) $(LVGL_DIR)/lvgl.h > $@

#GIT_SUBMODULES += lib/axtls lib/berkeley-db-1.xx lib/libffi lib/mbedtls lib/micropython-lib 




# compiler settings
CWARN = -Wall -Werror
CWARN += -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wpointer-arith -Wdouble-promotion -Wno-float-conversion -Wno-missing-declarations  -Wno-unused-but-set-variable -Wno-sign-compare -Wno-gnu-variable-sized-type-not-at-end -Wno-undefined-internal
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
CFLAGS += -g -D__TULIP_IOS__

ifndef DEBUG
CFLAGS += -U _FORTIFY_SOURCE
endif
TOOLCHAIN ?= iphonesimulator
CC = xcrun --sdk $(TOOLCHAIN) --toolchain $(TOOLCHAIN) clang 
COBJC = xcrun --sdk $(TOOLCHAIN) --toolchain $(TOOLCHAIN) clang -x objective-c
# Use clang syntax for map file
LDFLAGS_ARCH = -Wl,-map,$@.map -Wl,-dead_strip
LDFLAGS += $(LDFLAGS_MOD) $(LDFLAGS_ARCH) -lm $(LDFLAGS_EXTRA)
LDFLAGS += -F. -framework SDL2 -framework CoreFoundation -framework CoreMIDI -framework AVFAudio -framework AudioToolbox -framework UserNotifications -framework Foundation -lstdc++  $(ARCHFLAGS)
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
#LIBFFI_LDFLAGS_MOD = $(HOMEBREW)/opt/libffi/lib/libffi.a
#LIBFFI_CFLAGS_MOD := -I$(HOMEBREW)/opt/libffi/include


ifeq ($(UNAME_S),Linux)
LIBFFI_LDFLAGS_MOD += -ldl
endif

CFLAGS_MOD += $(LIBFFI_CFLAGS_MOD) # -DMICROPY_PY_FFI=1
LDFLAGS_MOD += $(LIBFFI_LDFLAGS_MOD)
#SRC_MOD += $(MICROPY_PORT_DIR)/modffi.c
endif

ifeq ($(MICROPY_PY_JNI),1)
# Path for 64-bit OpenJDK, should be adjusted for other JDKs
CFLAGS_MOD += -I/usr/lib/jvm/java-7-openjdk-amd64/include -DMICROPY_PY_JNI=1
SRC_MOD += $(MICROPY_PORT_DIR)/modjni.c
endif


# source files
SRC_C += \
	main.c \
	../shared/desktop/unix_mphal.c \
	../shared/desktop/unix_display.c \
	../shared/desktop/multicast.c \
	../shared/desktop/mpthreadport.c \
	$(MICROPY_PORT_DIR)/gccollect.c \
	$(MICROPY_PORT_DIR)/input.c \
	$(MICROPY_PORT_DIR)/modmachine.c \
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

SRC_M = \
	libminiaudio.m \
	virtualmidi.m \
	bridge.m


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
	$(CC) -I$(INC) $(CFLAGS) -c -o $@ $<



deplibs: axtls

PREFIX = /usr/local
BINDIR = $(DESTDIR)$(PREFIX)/bin

install: $(PROG)
	install -d $(BINDIR)
	install $(PROG) $(BINDIR)/$(PROG)

uninstall:
	-rm $(BINDIR)/$(PROG)
