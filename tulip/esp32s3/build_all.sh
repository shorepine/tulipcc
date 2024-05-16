#!/bin/bash
set -e
declare -a boards=("TULIP4_R10" "N16R8" "N32R8" "TDECK")
for i in "${boards[@]}"
do
    rm -rf build
    idf.py -DMICROPY_BOARD=$i build 
    python tulip_fs_create.py
done


