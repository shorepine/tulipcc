# tulip.mk

EXTMOD_DIR = extmod
TULIP_EXTMOD_DIR = $(EXTMOD_DIR)/tulip

EXTMOD_SRC_C += $(addprefix $(TULIP_EXTMOD_DIR)/, \
	modtulip.c \
	polyfills.c \
	lodepng.c \
	bitmap_fonts.c \
	smallfont.c \
	display.c \
	bresenham.c \
	ui.c \
	help.c \
	tulip_helpers.c \
	editor.c \
	keyscan.c \
	midi.c \
	alles.c \
	amy/amy.c \
	amy/algorithms.c \
	amy/envelope.c \
	amy/filters.c \
	amy/oscillators.c \
	amy/partials.c \
	amy/pcm.c \
	sounds.c \
	)

INC += -I$(TOP)/$(TULIP_EXTMOD_DIR)

#$(BUILD)/tulip/%.o: CFLAGS += -Wno-maybe-uninitialized -Wno-pointer-arith -Wno-unused-but-set-variable -Wno-format -Wno-sign-compare -Wno-old-style-declaration

