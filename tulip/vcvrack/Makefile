# AMYboard VCV Rack plugin.
#
# Two-stage build:
#   1. Makefile.mp compiles MicroPython + the frozen AMYboard firmware +
#      the tulip modules + AMY into build-mp/libamyboardmp.a
#   2. This Makefile (Rack's plugin.mk) compiles the Rack module and links
#      the archive.
#
# Requires the Rack SDK: https://vcvrack.com/downloads/
#   export RACK_DIR=<path to unzipped Rack-SDK>   (defaults below to ~/outside/Rack-SDK)
# Submodules must be bootstrapped first (cd tulip/shared && ./grab_submodules.sh),
# and the drum data generated once:  cd ../../amy && python3 -m amy.headers gamma9001
#
#   make          build plugin.dylib/.so
#   make dist     package a .vcvplugin
#   make install  copy into the local Rack2 user plugins folder

RACK_DIR ?= $(HOME)/outside/Rack-SDK

# Same defines the archive was built with, so shared headers agree.
FLAGS += -Iamy/src -I../shared -DAMY_NO_MINIAUDIO -DGAMMA9001 -DAMYBOARD_VCV
CFLAGS += -Wno-sign-compare -Wno-missing-field-initializers -Wno-strict-aliasing

SOURCES += src/plugin.cpp src/AmyBoard.cpp src/amy_stubs.c

MPLIB := build-mp/libamyboardmp.a
LDFLAGS += $(MPLIB) -lpthread

DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

include $(RACK_DIR)/plugin.mk

# Build/refresh the MicroPython+AMY archive before linking the plugin.
$(TARGET): $(MPLIB)

$(MPLIB): FORCE
	$(MAKE) -f Makefile.mp

FORCE:

.PHONY: FORCE
