#!/bin/bash
# This script cleans, builds and uploads binaries for the 4 supported Tulip CC versions
# It also does this for Tulip Desktop macOS

# call it like ./release.sh v-jan-2024 upload , where v-jan-2024 is a github release tag 
# and upload is one of [upload, test]
# test will just compile and not upload
# upload will do everything

# manually make a new release tag if you haven't done one in a while, or use this to overwrite the binaries on an existing tag
set -e


die() { echo "$*" 1>&2 ; exit 1; }

[ "$#" -eq 2 ] || die "usage: ./release.sh release_tag [upload,test]"

RELEASE=$1
TYPE=$2

# Copy over some known examples to tulip's /sys/ex before creating image
cp shared/py/drums.py fs/tulip/ex/my_drums.py
cp shared/py/juno6.py fs/tulip/ex/my_juno6.py
cp shared/py/worldui.py fs/tulip/ex/my_worldui.py
cp shared/py/voices.py fs/tulip/ex/my_voices.py

# Make sure the submodules are fresh
rm ../.submodules_ok

# Do the esp32s3 boards
cd esp32s3
source ~/esp/esp-idf-v5.4.1/export.sh 

# Otherwise, compile all boards. If upload set, upload them
declare -a boards=("TULIP4_R11" "TDECK" "N16R8" "N32R8")
for i in "${boards[@]}"
do
    rm -rf build
    idf.py -DMICROPY_BOARD=$i build 
    cd ..
    python fs_create.py tulip
    if [ "$TYPE" == "upload" ]; then
        gh release upload --clobber $RELEASE esp32s3/dist/tulip-full-$i.bin
        gh release upload --clobber $RELEASE esp32s3/dist/tulip-firmware-$i.bin
    fi
    cd esp32s3
done

cd ..
# Upload the sys folder (same for all Tulip boards) if set
if [ "$TYPE" == "upload" ]; then
    gh release upload --clobber $RELEASE esp32s3/dist/tulip-sys.bin
fi

# Now do macOS desktop
#cd macos
#./package.sh
#if [ "$TYPE" == "upload" ]; then
#    gh release upload --clobber $RELEASE dist/Tulip_Desktop.zip 
#fi
#cd ..

# Now do AMYboard
cd amyboard
rm -rf build
idf.py -DMICROPY_BOARD=AMYBOARD build
cd ..
python fs_create.py amyboard
if [ "$TYPE" == "upload" ]; then
    gh release upload --clobber $RELEASE amyboard/dist/amyboard-full-AMYBOARD.bin
    gh release upload --clobber $RELEASE amyboard/dist/amyboard-firmware-AMYBOARD.bin
    gh release upload --clobber $RELEASE amyboard/dist/amyboard-sys.bin
fi



