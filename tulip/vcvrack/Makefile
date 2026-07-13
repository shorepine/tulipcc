# AMYboard VCV Rack plugin.
#
# Requires the Rack SDK: https://vcvrack.com/downloads/
#   export RACK_DIR=<path to unzipped Rack-SDK>   (defaults below to ~/outside/Rack-SDK)
# The `amy` symlink points at the repo's amy submodule; bootstrap submodules
# first (cd tulip/shared && ./grab_submodules.sh) if amy/ is empty.
#
#   make          build plugin.dylib/.so
#   make dist     package a .vcvplugin
#   make install  copy into the local Rack2 user plugins folder

RACK_DIR ?= $(HOME)/outside/Rack-SDK

# AMY core synthesis sources (mirrors amy/godot/SConstruct), plus amy_midi.c
# for the MIDI message -> synth event routing (run_midi/stop_midi are stubbed
# in src/amy_stubs.c since we don't want AMY opening CoreMIDI -- Rack owns MIDI).
AMY_SRCS := \
	algorithms.c \
	amy.c \
	amy_midi.c \
	api.c \
	custom.c \
	cv_trigger.c \
	delay.c \
	envelope.c \
	examples.c \
	filters.c \
	instrument.c \
	interp_partials.c \
	log2_exp2.c \
	midi_mappings.c \
	oscillators.c \
	parse.c \
	patches.c \
	pcm.c \
	sequencer.c \
	transfer.c

FLAGS += -Iamy/src -DAMY_WAVETABLE -DAMY_NO_MINIAUDIO
CFLAGS += -Wno-sign-compare -Wno-missing-field-initializers -Wno-strict-aliasing

SOURCES += src/plugin.cpp src/AmyBoard.cpp src/amy_stubs.c
SOURCES += $(addprefix amy/src/, $(AMY_SRCS))

DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

include $(RACK_DIR)/plugin.mk
