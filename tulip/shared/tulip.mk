# tulip.mk

TULIP_EXTMOD_DIR = ../shared

EXTMOD_SRC_C += $(addprefix ../amy/src/, \
	amy.c \
	algorithms.c \
	custom.c \
	patches.c \
	delay.c \
	envelope.c \
	filters.c \
	oscillators.c \
	partials.c \
	pcm.c \
	log2_exp2.c \
)

EXTMOD_SRC_C += $(addprefix $(TULIP_EXTMOD_DIR)/, \
	modtulip.c \
	polyfills.c \
	smallfont.c \
	display.c \
	u8g2_fonts.c \
	u8fontdata.c \
	bresenham.c \
	wireframe.c \
	ui.c \
	help.c \
	tulip_helpers.c \
	editor.c \
	keyscan.c \
	midi.c \
	alles.c \
	sounds.c \
	lodepng.c \
	sequencer.c \
	lvgl_u8g2.c \
	)


INC += -I$(TOP)/$(TULIP_EXTMOD_DIR)
INC += -I../amy/src/

#$(BUILD)/tulip/%.o: CFLAGS += -Wno-maybe-uninitialized -Wno-pointer-arith -Wno-unused-but-set-variable -Wno-format -Wno-sign-compare -Wno-old-style-declaration

