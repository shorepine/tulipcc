#!/bin/bash

# Change this when you make a new one
RELEASE="v-jan-2024"

set -e
declare -a boards=("TULIP4_R10" "MATOUCH7" "N16R8" "N32R8" "TDECK")
for i in "${boards[@]}"
do
    rm -rf build
    idf.py -DMICROPY_BOARD=$i build 
    python tulip_fs_create.py
    python upload_firmware.py
    gh release upload --clobber v-jan-2024 dist/tulip-full-$i.bin
done
python upload_firmware.py sys
gh release upload --clobber v-jan-2024 dist/tulip-sys.bin



