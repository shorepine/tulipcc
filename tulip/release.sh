#!/bin/bash
# This script cleans, builds and uploads binaries for the 5 supported Tulip CC versions
# It also does this for Tulip Desktop macOS.

# call it like ./release.sh v-jan-2024 , where v-jan-2024 is a github release tag 
# manually make a new release tag if you haven't done one in a while, or use this to overwrite the binaries on an existing tag
# this also uploads to Tulip World for the tulip.upgrade() feature
set -e


die() { echo "$*" 1>&2 ; exit 1; }

[ "$#" -eq 1 ] || die "Provide the release tag as an argument to this script"

RELEASE=$1

cd esp32s3
declare -a boards=("TULIP4_R10" "MATOUCH7" "N16R8" "N32R8" "TDECK")
for i in "${boards[@]}"
do
    rm -rf build
    idf.py -DMICROPY_BOARD=$i build 
    python tulip_fs_create.py
    python upload_firmware.py
    gh release upload --clobber $RELEASE dist/tulip-full-$i.bin
done
python upload_firmware.py sys
gh release upload --clobber $RELEASE dist/tulip-sys.bin


# Now do desktop
cd ../macos
./package.sh
gh release upload --clobber $RELEASE dist/Tulip_Desktop.zip 
cd ..





