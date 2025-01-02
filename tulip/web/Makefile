################################################################################
# Initial setup of Makefile environment.

TOP = ../../micropython

# Set parallel flag to # of CPUs
CPUS ?= $(shell sysctl -n hw.ncpu || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

# Select the variant to build for:
ifdef VARIANT_DIR
# Custom variant path - remove trailing slash and get the final component of
# the path as the variant name.
VARIANT ?= $(notdir $(VARIANT_DIR:/=))
else
# If not given on the command line, then default to standard.
VARIANT ?= standard
VARIANT_DIR ?= variants/$(VARIANT)
endif

ifeq ($(wildcard $(VARIANT_DIR)/.),)
$(error Invalid VARIANT specified: $(VARIANT_DIR))
endif

# If the build directory is not given, make it reflect the variant name.
BUILD ?= build-$(VARIANT)/tulip/obj

include $(TOP)/py/mkenv.mk
include $(VARIANT_DIR)/mpconfigvariant.mk

# Use the default frozen manifest, variants may override this.
FROZEN_MANIFEST ?= variants/manifest.py

# Qstr definitions (must come before including py.mk).
QSTR_DEFS = qstrdefsport.h

# Include py core make definitions.
include $(TOP)/py/py.mk
include $(TOP)/extmod/extmod.mk

################################################################################
# Project specific settings and compiler/linker flags.


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


CC = emcc
LD = emcc
NODE ?= node
TERSER ?= npx terser

INC += -I.
INC += -I../shared/
INC += -I../shared/ulab/code/
INC += -I../shared/desktop/
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(VARIANT_DIR)
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src
INC += -I$(TOP)/../lv_binding_micropython_tulip/lvgl/src/libs/lodepng

CFLAGS += -Wall -Werror -Wdouble-promotion -Wfloat-conversion -DSTATIC=static -Wno-unused-variable -Wno-unused-function
CFLAGS += -O3 -DNDEBUG
CWARN += -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wpointer-arith -Wno-double-promotion -Wfloat-conversion -Wno-missing-declarations  -Wno-unused-but-set-variable -Wno-sign-compare -Wno-gnu-variable-sized-type-not-at-end -Wno-undefined-internal
CFLAGS += $(INC) $(CWARN)
CFLAGS += -DTULIP_WEB -DMA_ENABLE_AUDIO_WORKLETS -sUSE_SDL=2 -DAMY_IS_EXTERNAL -DMODULE_ULAB_ENABLED=1 -DAMY_HAS_AUDIO_IN

EXPORTED_FUNCTIONS_EXTRA += ,\
	_mp_js_do_exec,\
	_mp_js_do_exec_async,\
	_mp_js_frozen_exec,\
	_mp_js_do_import,\
	_mp_js_register_js_module,\
	_proxy_c_free_obj,\
	_proxy_c_init,\
	_proxy_c_to_js_call,\
	_proxy_c_to_js_delete_attr,\
	_proxy_c_to_js_dir,\
	_proxy_c_to_js_get_array,\
	_proxy_c_to_js_get_dict,\
	_proxy_c_to_js_get_iter,\
	_proxy_c_to_js_get_type,\
	_proxy_c_to_js_has_attr,\
	_proxy_c_to_js_iternext,\
	_proxy_c_to_js_lookup_attr,\
	_proxy_c_to_js_resume,\
	_proxy_c_to_js_store_attr,\
	_proxy_convert_mp_to_js_obj_cside,\
	_tulip_tick

EXPORTED_RUNTIME_METHODS_EXTRA += ,\
	PATH,\
	PATH_FS,\
	UTF8ToString,\
	getValue,\
	lengthBytesUTF8,\
	setValue,\
	stringToUTF8

JSFLAGS += \
	-lidbfs.js \
    -s USE_SDL=2 \
	-s ASYNCIFY \
	-s ASYNCIFY_STACK_SIZE=16000 \
	-s ASSERTIONS \
	-s EXPORTED_FUNCTIONS="\
	_free,\
	_malloc,\
	_mp_js_init,\
	_mp_js_repl_init,\
	_mp_js_repl_process_char,\
	_mp_hal_get_interrupt_char,\
	_mp_handle_pending,\
	_process_single_midi_byte,\
	_mp_sched_keyboard_interrupt$(EXPORTED_FUNCTIONS_EXTRA)"
JSFLAGS += -s EXPORTED_RUNTIME_METHODS="\
	ccall,\
	cwrap,\
	FS$(EXPORTED_RUNTIME_METHODS_EXTRA)"
JSFLAGS += --js-library library.js
JSFLAGS += -s SUPPORT_LONGJMP=emscripten
JSFLAGS += -s MODULARIZE -s EXPORT_NAME=_createMicroPythonModule
JSFLAGS += -s INITIAL_MEMORY=128mb -s TOTAL_STACK=64mb -s ALLOW_MEMORY_GROWTH=1 
JSFLAGS += --preload-file ../fs@tulip4/sys

################################################################################
# Source files and libraries.

SRC_SHARED = $(addprefix shared/,\
	runtime/interrupt_char.c \
	runtime/stdout_helpers.c \
	runtime/pyexec.c \
	readline/readline.c \
	timeutils/timeutils.c \
	)

SRC_C += \
	lexer_dedent.c \
	main.c \
	modjs.c \
	modjsffi.c \
	mphalport.c \
	objjsproxy.c \
	proxy_c.c \
	$(LVGL_MPY)


TULIP_EXTMOD_DIR = ../shared
ULAB_DIR = ../shared/ulab/code

EXTMOD_SRC_C += $(addprefix $(TULIP_EXTMOD_DIR)/, \
	modtulip.c \
	polyfills.c \
	smallfont.c \
	display.c \
	u8g2_fonts.c \
	u8fontdata.c \
	bresenham.c \
	ui.c \
	help.c \
	tulip_helpers.c \
	editor.c \
	keyscan.c \
	lodepng.c \
	lvgl_u8g2.c \
	tsequencer.c \
	midi.c \
	desktop/unix_display.c \
	)

EXTMOD_SRC_C += $(addprefix $(ULAB_DIR)/, \
	scipy/integrate/integrate.c \
	scipy/linalg/linalg.c \
	scipy/optimize/optimize.c \
	scipy/signal/signal.c \
	scipy/special/special.c \
	ndarray_operators.c \
	ulab_tools.c \
	ndarray.c \
	numpy/ndarray/ndarray_iter.c \
	ndarray_properties.c \
	numpy/approx.c \
	numpy/bitwise.c \
	numpy/compare.c \
	numpy/carray/carray.c \
	numpy/carray/carray_tools.c \
	numpy/create.c \
	numpy/fft/fft.c \
	numpy/fft/fft_tools.c \
	numpy/filter.c \
	numpy/io/io.c \
	numpy/linalg/linalg.c \
	numpy/linalg/linalg_tools.c \
	numpy/numerical.c \
	numpy/poly.c \
	numpy/random/random.c \
	numpy/stats.c \
	numpy/transform.c \
	numpy/vector.c \
	numpy/numpy.c \
	scipy/scipy.c \
	user/user.c \
	utils/utils.c \
	ulab.c \
	)

# List of sources for qstr extraction.
SRC_QSTR += $(SRC_C) $(SRC_SHARED) $(EXTMOD_SRC_C)

SRC_JS += \
	api.js \
	objpyproxy.js \
	proxy_js.js \

OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_SHARED:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(EXTMOD_SRC_C:.c=.o))


################################################################################
# Main targets.

.PHONY: all repl min test test_min

all: $(BUILD)/micropython.mjs

$(BUILD)/micropython.mjs: $(OBJ) library.js $(SRC_JS)
	$(ECHO) "LINK $@"
	$(Q)emcc $(LDFLAGS) -o $@ $(OBJ) $(JSFLAGS)
	$(Q)cat $(SRC_JS) >> $@

$(BUILD)/micropython.min.mjs: $(BUILD)/micropython.mjs
	$(TERSER) $< --compress --module -o $@

repl: $(BUILD)/micropython.mjs
	$(NODE) $<

min: $(BUILD)/micropython.min.mjs

test: $(BUILD)/micropython.mjs $(TOP)/tests/run-tests.py
	cd $(TOP)/tests && MICROPY_MICROPYTHON_MJS=../ports/webassembly/$< ./run-tests.py -t webassembly

test_min: $(BUILD)/micropython.min.mjs $(TOP)/tests/run-tests.py
	cd $(TOP)/tests && MICROPY_MICROPYTHON_MJS=../ports/webassembly/$< ./run-tests.py -t webassembly

################################################################################
# Remaining make rules.

include $(TOP)/py/mkrules.mk
