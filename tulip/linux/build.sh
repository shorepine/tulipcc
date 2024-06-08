#!/bin/bash
# build.sh
# Just builds locally
set -e

source ../shared/grab_submodules.sh
make DEBUG=1
mkdir -p dev
cp build-standard/tulip/obj/tulip dev/

mkdir -p ~/Documents/tulipcc
mkdir -p ~/Documents/tulipcc/user

rm -rf ~/Documents/tulipcc/sys
mkdir -p ~/Documents/tulipcc/sys
cp -Rf ../fs/* ~/Documents/tulipcc/sys/

