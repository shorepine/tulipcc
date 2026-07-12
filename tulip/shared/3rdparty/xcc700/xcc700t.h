// xcc700t.h -- Tulip's fork of xcc700 (see xcc700t.c for the change list).
// Compiles a small-C source string to an Xtensa REL ELF image in memory,
// with e_entry pointing at entry_name, ready for the esp-idf elf_loader.
#pragma once

#include <stdint.h>

// Returns 0 and a malloc'd ELF image in *elf_out/*elf_size (caller frees),
// or -1 with a message in err. Not thread-safe (single global compiler state).
int xcc700_compile(const char *source, const char *entry_name,
                   uint8_t **elf_out, uint32_t *elf_size, char *err, int errlen);
