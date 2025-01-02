#!/bin/sh
set -e
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
NPROC=$(python3 -c 'import multiprocessing; print(multiprocessing.cpu_count())')
HERE="$(dirname -- "$(readlinkf_posix -- "${0}")" )"
[ -e circuitpython/py/py.mk ] || (git clone --branch main https://github.com/adafruit/circuitpython && cd circuitpython && make fetch-all-submodules && git submodule update --init lib/uzlib tools)
rm -rf circuitpython/extmod/ulab; ln -s "$HERE" circuitpython/extmod/ulab
dims=${1-2}
make -C circuitpython/mpy-cross -j$NPROC
make -k -C circuitpython/ports/unix -j$NPROC DEBUG=1 MICROPY_PY_FFI=0 MICROPY_PY_BTREE=0 MICROPY_SSL_AXTLS=0 MICROPY_PY_USSL=0 CFLAGS_EXTRA="-Wno-tautological-constant-out-of-range-compare -Wno-unknown-pragmas -DULAB_MAX_DIMS=$dims" BUILD=build-$dims PROG=micropython-$dims

# bash test-common.sh "${dims}" "circuitpython/ports/unix/micropython-$dims"

# Docs don't depend on the dimensionality, so only do it once
if [ "$dims" -eq 2 ]; then
    (cd circuitpython && sphinx-build -E -W -b html . _build/html)
    (cd circuitpython && make check-stubs)
fi
