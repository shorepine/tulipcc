// xcc700w.h -- WebAssembly backend of Tulip's xcc700 fork (see xcc700w.c).
// Compiles a small-C source string to a complete wasm binary module whose
// undefined functions become env.* imports (wired to AMY module exports) and
// which places statics/rodata at imported base globals allocated by the host.
#pragma once

#include <stdint.h>

// Returns 0 and a malloc'd wasm module in *wasm_out/*wasm_size (caller frees),
// or -1 with a message in err. bss_size/rodata_size (may be NULL) report how
// much linear memory the host must allocate for env.__bss_base /
// env.__rodata_base. Not thread-safe (single global compiler state).
int xcc700w_compile(const char *source, const char *entry_name,
                    uint8_t **wasm_out, uint32_t *wasm_size,
                    uint32_t *bss_size, uint32_t *rodata_size,
                    char *err, int errlen);
