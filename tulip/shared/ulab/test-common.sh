#!/bin/sh
set -e
dims="$1"
micropython="$2"
for level1 in $(printf "%dd " $(seq 1 ${dims}))
do
    for level2 in numpy scipy utils complex; do
        rm -f *.exp
        if [ ! -d tests/"$level1"/"$level2" ]; then
            break;
        fi
        for file in tests/"$level1"/"$level2"/*.py; do
            if [ ! -f "$file"".exp"  ]; then
                echo "" > "$file"".exp"
            fi
        done
	    if ! env MICROPY_MICROPYTHON="$micropython" ./run-tests -d tests/"$level1"/"$level2"; then
            for exp in *.exp; do
                testbase=$(basename $exp .exp);
                echo "\nFAILURE $testbase";
                diff -u $testbase.exp $testbase.out;
            done
            exit 1
	    fi
    done
done

