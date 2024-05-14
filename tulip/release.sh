#!/bin/bash
# This script cleans, builds and uploads binaries for the 5 supported Tulip CC versions
# It also does this for Tulip Desktop macOS.

# call it like ./release.sh v-jan-2024 upload , where v-jan-2024 is a github release tag 
# and upload is one of [sys, upload, test]
# sys will just generate the latest fs and upload it
# test will just compile and not upload
# upload will do everything

# manually make a new release tag if you haven't done one in a while, or use this to overwrite the binaries on an existing tag
# this also uploads to Tulip World for the tulip.upgrade() feature
set -e


die() { echo "$*" 1>&2 ; exit 1; }

[ "$#" -eq 2 ] || die "usage: ./release.sh release_tag [sys,upload,test]"

RELEASE=$1
TYPE=$2

cd esp32s3

# If sys, just create/upload the last sys and exit
if [ "$TYPE" == "sys" ]; then
    python tulip_fs_create.py
    python upload_firmware.py sys
    gh release upload --clobber $RELEASE dist/tulip-sys.bin
    exit 1;
fi

# Otherwise, compile all boards. If upload set, upload them
declare -a boards=("TULIP4_R10" "MATOUCH7" "N16R8" "N32R8" "TDECK")
for i in "${boards[@]}"
do
    rm -rf build
    idf.py -DMICROPY_BOARD=$i build 
    python tulip_fs_create.py
    if [ "$TYPE" == "upload" ]; then
        python upload_firmware.py
        gh release upload --clobber $RELEASE dist/tulip-full-$i.bin
        gh release upload --clobber $RELEASE dist/tulip-firmware-$i.bin
    fi
done

if [ "$TYPE" == "upload" ]; then
    python upload_firmware.py sys
    gh release upload --clobber $RELEASE dist/tulip-sys.bin
fi

# Now do desktop
cd ../macos
./package.sh
if [ "$TYPE" == "upload" ]; then
    gh release upload --clobber $RELEASE dist/Tulip_Desktop.zip 
fi
cd ..





