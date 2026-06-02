#!/bin/bash

# grab_submodules.sh

# Do the first run submodule init if needed
echo `pwd`
pushd ../../ > /dev/null
if test -f ".submodules_ok"; then
    echo "Submodules already loaded";
else
    echo "Syncing submodules for first build..."
    git submodule update --init amy
    git submodule update --init micropython
    cd micropython
    git submodule update --init lib/axtls
    git submodule update --init lib/libffi
    git submodule update --init lib/mbedtls
    git submodule update --init lib/berkeley-db-1.xx
    git submodule update --init lib/micropython-lib
    git submodule update --init lib/tinyusb
    cd ..
    touch .submodules_ok
fi

# Patch micropython/mpy-cross/Makefile for newer Apple clang tightenings.
# Run every invocation (even when .submodules_ok exists) and gate each
# individual sed on a grep so it stays idempotent. This way users whose
# submodules are already set up pick up fresh patches when they update
# macOS to a clang version that added new default warnings, without
# having to rm .submodules_ok.
#
# -Wno-gnu-folding-constant               — Apple clang tightening, earlier round
# -Wno-unknown-warning-option             — stop older clangs from erroring on
#                                           flag names they don't recognize yet
# -Wno-unterminated-string-initialization — macOS Tahoe / clang >= 19 turned the
#                                           {10,"r10"} style init into an error;
#                                           py/emitinlinethumb.c uses this
#                                           pattern intentionally (3-char array,
#                                           null byte deliberately dropped).
if [ -f micropython/mpy-cross/Makefile ]; then
    if ! grep -q -- "-Wno-gnu-folding-constant" micropython/mpy-cross/Makefile; then
        sed -i.bak 's/^CWARN += /CWARN += -Wno-gnu-folding-constant /' micropython/mpy-cross/Makefile
    fi
    if ! grep -q -- "-Wno-unterminated-string-initialization" micropython/mpy-cross/Makefile; then
        sed -i.bak 's/^CWARN += /CWARN += -Wno-unknown-warning-option -Wno-unterminated-string-initialization /' micropython/mpy-cross/Makefile
    fi
    # The seds above prepend the suppressions, but newer Apple clang (macOS Tahoe
    # / clang >= 19) re-enables -Wunterminated-string-initialization through
    # -Wextra, which the upstream CWARN line applies *after* them — so -Werror
    # still trips on py/emitinlinethumb.c. Append the suppressions to the end of
    # the Makefile so they are the last CFLAGS entries and win. Idempotent.
    if ! grep -q -- "amyboard-clang-overrides" micropython/mpy-cross/Makefile; then
        cat >> micropython/mpy-cross/Makefile <<'MK'

# amyboard-clang-overrides: keep last so they win over -Wextra (see grab_submodules.sh)
CFLAGS += -Wno-unknown-warning-option -Wno-unterminated-string-initialization -Wno-gnu-folding-constant
MK
    fi
fi
popd > /dev/null
