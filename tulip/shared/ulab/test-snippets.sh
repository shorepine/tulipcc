#!/bin/sh
set -e
micropython="$1"
for level1 in numpy scipy;
do
    for level2 in core lib signal; do
        rm -f *.exp
	if ! env MICROPY_MICROPYTHON="$micropython" ./run-tests -d snippets/tests/"$level1"/"$level2"; then
            for exp in *.exp; do
                testbase=$(basename $exp .exp);
                echo -e "\nFAILURE $testbase";
                diff -u $testbase.exp $testbase.out;
            done
            exit 1
	fi
    done
done

