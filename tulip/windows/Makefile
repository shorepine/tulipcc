# Tulip Desktop — Windows (MinGW) Makefile.
#
# Builds a standalone tulip.exe: MicroPython + AMY + the Tulip display/LVGL/ulab
# modules, cross-compiled with x86_64-w64-mingw32-gcc. Modeled on
# tulip/linux/Makefile (Tulip Desktop) with the proven Windows-MicroPython
# recipe from tulip/vcvrack/Makefile.mp layered in:
#   - VARIANT_DIR points at the micropython WINDOWS port variant (its common
#     header does NOT enable POSIX select/poll like the unix one),
#   - threads / sockets / termios / ssl / btree / VFS-FAT/LFS all off,
#   - the windows-port HAL is replaced by tulip/windows/win_mphal.c (console +
#     on-screen text routing), plus ports/windows realpath.c / init.c / fmode.c.
#
# SDL2 (MinGW dev package) is provided by the build script via SDL_INC / SDL_LIB.
# Run through tulip/windows/toolchain-build-windows.sh inside the Rack toolchain
# docker image; see that script for the env it exports.

DEBUG ?= 1

VARIANT ?= standard
MAKEFLAGS += --jobs=4

BUILD ?= build-$(VARIANT)/tulip/obj

TOP = ../../micropython

# Under the rack-plugin-toolchain the cross compiler arrives as env CC
# (x86_64-w64-mingw32-gcc). micropython's mkenv.mk composes CC from
# CROSS_COMPILE, so translate one into the other, snapshotting with := so it
# isn't a circular reference once mkenv redefines CC.
ifeq ($(origin CROSS_COMPILE),undefined)
ifneq ($(filter %-gcc,$(CC)),)
CROSS_COMPILE := $(CC:%gcc=%)
endif
endif

# The micropython WINDOWS port (its common header avoids the unix POSIX-isms).
VARIANT_DIR ?= $(TOP)/ports/windows/variants/standard
MICROPY_PORT_DIR = $(TOP)/ports/unix
WINDOWS_PORT_DIR = $(TOP)/ports/windows

include $(TOP)/py/mkenv.mk
include mpconfigport.mk

# Force the windows-port feature set regardless of mpconfigport.mk (matches
# tulip/vcvrack/Makefile.mp).
MICROPY_PY_THREAD := 0
MICROPY_PY_SOCKET := 0
MICROPY_PY_TERMIOS := 0
MICROPY_PY_SSL := 0
MICROPY_SSL_MBEDTLS := 0
MICROPY_PY_BTREE := 0
MICROPY_VFS_FAT := 0
MICROPY_VFS_LFS1 := 0
MICROPY_VFS_LFS2 := 0

FROZEN_MANIFEST ?= variants/manifest.py

PROG = tulip

# qstr definitions (must come before including py.mk)
QSTR_DEFS = $(TOP)/ports/unix/qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES = $(VARIANT_DIR)/mpconfigvariant.h

UNAME_S := $(shell uname -s)

include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

# --- LVGL binding (same generator flow as linux) ---------------------------
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
CFLAGS += -DSTATIC=static

$(LVGL_MPY): $(ALL_LVGL_SRC) $(LVGL_BINDING_DIR)/gen/gen_mpy.py
	$(ECHO) "LVGL-GEN $@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CPP) $(LV_CFLAGS) -DLVGL_PREPROCESS -I $(LVGL_BINDING_DIR)/pycparser/utils/fake_libc_include $(INC) $(LVGL_DIR)/lvgl.h > $(LVGL_PP)
	$(Q)$(PYTHON) $(LVGL_BINDING_DIR)/gen/gen_mpy.py -M lvgl -MP lv -MD $(LVGL_MPY_METADATA) -E $(LVGL_PP) $(LVGL_DIR)/lvgl.h > $@

# --- includes --------------------------------------------------------------
INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I../shared/desktop/
INC += -I$(TOP)/ports/unix
INC += -I$(WINDOWS_PORT_DIR)
INC += -I../shared/
INC += -I../../amy/src/
INC += -I$(TOP)/lib/mbedtls/include
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src/libs/lodepng
# SDL2 headers (MinGW dev package), supplied by the build script.
INC += $(SDL_INC)

GIT_SUBMODULES += lib/berkeley-db-1.xx

# --- compiler settings -----------------------------------------------------
CWARN = -Wall
CWARN += -Wno-error
CWARN += -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-old-style-declaration -Wno-unused-but-set-parameter -Wpointer-arith -Wdouble-promotion -Wno-float-conversion -Wno-missing-declarations -Wno-unused-but-set-variable -Wno-sign-compare -Wno-gnu-variable-sized-type-not-at-end -Wno-undefined-internal
CFLAGS += $(INC) $(CWARN) -std=gnu99 -DUNIX $(CFLAGS_MOD) $(COPT) -I$(VARIANT_DIR) $(CFLAGS_EXTRA)
CFLAGS += -DTULIP_DESKTOP
# Marks the Tulip Desktop Windows port specifically (NOT just _WIN32 — the
# VCV Windows plugin also builds modtulip.c but not win_mphal.c/win_http.c).
CFLAGS += -DTULIP_WINDOWS_PORT
# MinGW ANSI stdio for %g/%hu etc.
CFLAGS += -D__USE_MINGW_ANSI_STDIO=1
# tulip sources still use the retired STATIC macro.
CFLAGS += -DSTATIC=static
# Use the system libc printf, not MP's minimal one (which aborts on AMY's %hu).
CFLAGS += -DMICROPY_USE_INTERNAL_PRINTF=0

ifdef DEBUG
COPT += -O0 -g
else
COPT += -Os -DNDEBUG
endif
COPT += -fdata-sections -ffunction-sections

CFLAGS += -g

ifeq ($(MICROPY_USE_READLINE),1)
INC += -I$(TOP)/shared/readline
CFLAGS += -DMICROPY_USE_READLINE=1
SHARED_SRC_C_EXTRA += readline/readline.c
endif

# --- Tulip module set (AMY + display/LVGL/ulab + drums_bin) ----------------
include ../shared/tulip.mk

# --- source files ----------------------------------------------------------
SRC_C += \
	main.c \
	../shared/desktop/unix_display.c \
	win_mphal.c \
	win_http.c \
	../../amy/src/libminiaudio-audio.c \
	$(MICROPY_PORT_DIR)/gccollect.c \
	$(MICROPY_PORT_DIR)/input.c \
	$(WINDOWS_PORT_DIR)/realpath.c \
	$(WINDOWS_PORT_DIR)/init.c \
	$(WINDOWS_PORT_DIR)/fmode.c \
	$(wildcard $(VARIANT_DIR)/*.c)

SRC_C += $(LVGL_MPY)

SHARED_SRC_C += $(addprefix $(TOP)/shared/,\
	runtime/gchelper_generic.c \
	runtime/interrupt_char.c \
	timeutils/timeutils.c \
	runtime/pyexec.c \
	$(SHARED_SRC_C_EXTRA) \
	)

OBJ = $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SHARED_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(EXTMOD_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) $(SHARED_SRC_C) $(EXTMOD_SRC_C)

ifneq ($(FROZEN_MANIFEST),)
CFLAGS += -DMPZ_DIG_SIZE=16
endif

CPU_BRAND_NAME=windows
CFLAGS += -DMICROPY_HW_MCU_NAME="\"$(CPU_BRAND_NAME)\""
CFLAGS += -Wno-double-promotion -Wno-unused-function

# --- link ------------------------------------------------------------------
# SDL2 + AMY/miniaudio (WASAPI) + windows platform libs. winpthread is linked
# statically (MicroPython runs on one), like the vcv build.
# -mwindows: GUI subsystem — no console window on double-click. main.c
# attaches to a parent console when launched from a terminal, or opens one
# for --console / tulip.win_console(). MinGW's runtime maps main() for us.
LDFLAGS += -Wl,--gc-sections -mwindows
LIB += $(SDL_LIB)
LIB += -lole32 -loleaut32 -limm32 -lversion -lsetupapi -lwinmm -lksuser -luuid
LIB += -lgdi32 -luser32 -lshell32 -lws2_32 -lbcrypt -lwinhttp -lm
LIB += -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic
LIB += -static-libgcc

include $(TOP)/py/mkrules.mk
