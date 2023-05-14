# tulip.mk

EXTMOD_DIR = extmod
TULIP_EXTMOD_DIR = $(EXTMOD_DIR)/tulip

EXTMOD_SRC_C += $(addprefix $(EXTMOD_DIR)/amy/src/, \
	amy.c \
	algorithms.c \
	delay.c \
	envelope.c \
	filters.c \
	oscillators.c \
	partials.c \
	pcm.c \
)

EXTMOD_SRC_C += $(addprefix $(TULIP_EXTMOD_DIR)/, \
	modtulip.c \
	polyfills.c \
	lodepng.c \
	u8g2_fonts.c \
	u8fontdata.c \
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
	sounds.c \
	)

INC += -I$(TOP)/$(TULIP_EXTMOD_DIR)
INC += -I$(TOP)/$(EXTMOD_DIR)/amy/src/

#$(BUILD)/tulip/%.o: CFLAGS += -Wno-maybe-uninitialized -Wno-pointer-arith -Wno-unused-but-set-variable -Wno-format -Wno-sign-compare -Wno-old-style-declaration

