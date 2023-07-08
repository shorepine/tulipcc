#!/bin/bash
# build.sh
# Just builds locally

source ../shared/grab_submodules.sh
mkdir -p ~/tulip_home
make DEBUG=1
mkdir -p dev
cp build-standard/tulip/obj/tulip dev/
