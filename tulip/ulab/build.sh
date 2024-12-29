#!/bin/sh

GIT_HASH=`git describe --abbrev=8 --always`

# POSIX compliant version
readlinkf_posix() {
  [ "${1:-}" ] || return 1
  max_symlinks=40
  CDPATH='' # to avoid changing to an unexpected directory

  target=$1
  [ -e "${target%/}" ] || target=${1%"${1##*[!/]}"} # trim trailing slashes
  [ -d "${target:-/}" ] && target="$target/"

  cd -P . 2>/dev/null || return 1
  while [ "$max_symlinks" -ge 0 ] && max_symlinks=$((max_symlinks - 1)); do
    if [ ! "$target" = "${target%/*}" ]; then
      case $target in
        /*) cd -P "${target%/*}/" 2>/dev/null || break ;;
        *) cd -P "./${target%/*}" 2>/dev/null || break ;;
      esac
      target=${target##*/}
    fi

    if [ ! -L "$target" ]; then
      target="${PWD%/}${target:+/}${target}"
      printf '%s\n' "${target:-/}"
      return 0
    fi

    # `ls -dl` format: "%s %u %s %s %u %s %s -> %s\n",
    #   <file mode>, <number of links>, <owner name>, <group name>,
    #   <size>, <date and time>, <pathname of link>, <contents of link>
    # https://pubs.opengroup.org/onlinepubs/9699919799/utilities/ls.html
    link=$(ls -dl -- "$target" 2>/dev/null) || break
    target=${link#*" $target -> "}
  done
  return 1
}
NPROC=`python3 -c 'import multiprocessing; print(multiprocessing.cpu_count())'`
PLATFORM=`python3 -c 'import sys; print(sys.platform)'`
set -e
HERE="$(dirname -- "$(readlinkf_posix -- "${0}")" )"
dims=${1-2}
if [ ! -d "micropython" ] ; then
  git clone https://github.com/micropython/micropython
else
  git -C micropython pull
fi
make -C micropython/mpy-cross -j${NPROC}
make -C micropython/ports/unix submodules
make -C micropython/ports/unix -j${NPROC} USER_C_MODULES="${HERE}" DEBUG=1 STRIP=: MICROPY_PY_FFI=0 MICROPY_PY_BTREE=0 CFLAGS_EXTRA=-DULAB_MAX_DIMS=$dims CFLAGS_EXTRA+=-DULAB_HASH=$GIT_HASH BUILD=build-$dims PROG=micropython-$dims

PROG="micropython/ports/unix/build-$dims/micropython-$dims"
if [ ! -e "$PROG" ]; then
  # Older MicroPython revision, executable is still in ports/unix.
  PROG="micropython/ports/unix/micropython-$dims"
fi

bash test-common.sh "${dims}" "$PROG"

# Build with single-precision float.
make -C micropython/ports/unix -j${NPROC} USER_C_MODULES="${HERE}" DEBUG=1 STRIP=: MICROPY_PY_FFI=0 MICROPY_PY_BTREE=0 CFLAGS_EXTRA=-DMICROPY_FLOAT_IMPL=MICROPY_FLOAT_IMPL_FLOAT CFLAGS_EXTRA+=-DULAB_MAX_DIMS=$dims CFLAGS_EXTRA+=-DULAB_HASH=$GIT_HASH BUILD=build-nanbox-$dims PROG=micropython-nanbox-$dims

# The unix nanbox variant builds as a 32-bit executable and requires gcc-multilib.
# macOS doesn't support i386 builds so only build on linux.
if [ $PLATFORM = linux ]; then
    make -C micropython/ports/unix -j${NPROC} VARIANT=nanbox USER_C_MODULES="${HERE}" DEBUG=1 STRIP=: MICROPY_PY_FFI=0 MICROPY_PY_BTREE=0 CFLAGS_EXTRA=-DULAB_MAX_DIMS=$dims CFLAGS_EXTRA+=-DULAB_HASH=$GIT_HASH BUILD=build-nanbox-$dims PROG=micropython-nanbox-$dims
fi

