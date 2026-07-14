# Tulip Desktop — Windows (MinGW) build config.
#
# Unlike the linux/macos desktop builds, the MicroPython *windows* port has no
# mpthreadport / socket / termios / fatfs_port. We follow tulip/vcvrack's proven
# recipe: threads OFF (MicroPython runs on a winpthread we create in main.c,
# with the scheduler ring guarded by a mutex-backed atomic section), and
# socket / ssl / btree / VFS-FAT/LFS all off to match the windows port.

# Build 32-bit binaries on a 64-bit host
MICROPY_FORCE_32BIT = 0

#  0 - no readline, just simple stdin input
#  1 - use MicroPython version of readline
MICROPY_USE_READLINE = 1

# btree module using Berkeley DB 1.xx — off (no berkeley-db on the windows port)
MICROPY_PY_BTREE = 0

# _thread module using pthreads — off (no mpthreadport on the windows port)
MICROPY_PY_THREAD = 0

# Subset of CPython termios module — off (no termios on windows)
MICROPY_PY_TERMIOS = 0

# Subset of CPython socket module — off
MICROPY_PY_SOCKET = 0

# ffi module requires libffi
MICROPY_PY_FFI = 0

# ssl module — off
MICROPY_PY_SSL = 0
MICROPY_SSL_AXTLS = 0
MICROPY_SSL_MBEDTLS = 0

# jni module requires JVM/JNI
MICROPY_PY_JNI = 0

MICROPY_STANDALONE ?= 0

MICROPY_ROM_TEXT_COMPRESSION = 1

# VFS filesystems — off (windows port has no fatfs_port / littlefs plumbing here)
MICROPY_VFS_FAT = 0
MICROPY_VFS_LFS1 = 0
MICROPY_VFS_LFS2 = 0
