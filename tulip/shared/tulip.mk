# tulip.mk

TULIP_EXTMOD_DIR = $(TOP)/../tulip/shared
ULAB_DIR = $(TOP)/../tulip/shared/ulab/code

EXTMOD_SRC_C += $(addprefix $(TOP)/../amy/src/, \
	amy.c \
	algorithms.c \
	custom.c \
	patches.c \
	delay.c \
	envelope.c \
	filters.c \
	oscillators.c \
	transfer.c \
	sequencer.c \
	partials.c \
	pcm.c \
	log2_exp2.c \
	interp_partials.c \
	parse.c \
	midi.c \
	instrument.c \
	api.c \
)

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
	amy_connector.c \
	sounds.c \
	lodepng.c \
	tsequencer.c \
	lvgl_u8g2.c \
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



INC += -I$(ULAB_DIR)
INC += -I$(TULIP_EXTMOD_DIR)
INC += -I$(TOP)/../amy/src

CFLAGS_EXTRA += -DMODULE_ULAB_ENABLED=1


#$(BUILD)/tulip/%.o: CFLAGS += -Wno-maybe-uninitialized -Wno-pointer-arith -Wno-unused-but-set-variable -Wno-format -Wno-sign-compare -Wno-old-style-declaration

