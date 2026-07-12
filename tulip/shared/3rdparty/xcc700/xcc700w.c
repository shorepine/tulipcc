// xcc700w.c - WebAssembly backend for Tulip's xcc700 fork (see xcc700t.c).
// Same small-C front end (lexer/symbols/parse structure, int + int16_t
// pointers/arrays, enum, static locals -> bss, casts as no-ops), emitting a
// complete wasm binary module instead of an Xtensa ELF, for
// tulip.install_c_process() on Tulip Web / AMYboard Web.
//
// Model (kept deliberately identical to the Xtensa backend):
//   - all C locals live in a shadow stack in linear MEMORY (imported from the
//     AMY module, so user code reads/writes AMY's buffers directly); wasm
//     locals only implement the "registers" res/tmp/arg1/arg2
//   - statics/.bss and .rodata are placed at imported base globals
//     (env.__bss_base, env.__rodata_base), allocated by the host from AMY's
//     heap; env.__stack_base seeds the module's private $sp global
//   - calls to undeclared-here functions become wasm imports (env.<name>),
//     wired by the host straight to AMY module exports (e.g. cos_lut) --
//     no JS in the per-sample path
//   - fxmul / to_int16 / from_int16 are inlined as intrinsics
//   - control flow is structured wasm (block/loop/br_if) 1:1 with the C
//
// API: xcc700w_compile(src, entry_name, &wasm_out, &wasm_size, err, errlen).
// The entry function and every defined function are exported by name.
// Build the CLI for host testing with -DXCC700W_CLI.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>

#include "xcc700w.h"

static jmp_buf w_jmp;
static char *w_errbuf; static int w_errcap;
static void w_fail(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (w_errbuf != NULL && w_errcap > 0 && w_errbuf[0] == 0)
        vsnprintf(w_errbuf, w_errcap, fmt, ap);
    va_end(ap);
    longjmp(w_jmp, 1);
}

enum {
    MAX_VARS=256, MAX_LOCAL_VARS=128, MAX_FUNCS=64, MAX_ARGS=5
};

enum {
    T_EOF=0, T_INT=256, T_CHAR, T_VOID, T_IDENT, T_NUM, T_STR, T_EQ, T_NE, T_LE, T_GE,
    T_SHL, T_SHR, T_LAND, T_LOR, T_RETURN, T_ELLIPSIS, T_IF, T_ELSE, T_WHILE, T_ENUM, T_INC, T_DEC,
    T_SHORT, T_STATIC
};

enum { TF_BYTE=1, TF_PTR=2, TF_ARR=4, TF_GLOBAL=8, TF_CONST=16, TF_SHORT=32 };
enum { TY_INT=0, TY_BYTE=1, TY_INTPTR=2, TY_BYTEPTR=3, TY_INTARR=4, TY_BYTEARR=5,
       TY_SHORTPTR=TF_PTR|TF_SHORT, TY_SHORTARR=TF_ARR|TF_SHORT };

// wasm opcodes used
enum {
    W_BLOCK=0x02, W_LOOP=0x03, W_IF=0x04, W_ELSE=0x05, W_END=0x0b,
    W_BR=0x0c, W_BRIF=0x0d, W_RETURN=0x0f, W_CALL=0x10, W_DROP=0x1a,
    W_LGET=0x20, W_LSET=0x21, W_LTEE=0x22, W_GGET=0x23, W_GSET=0x24,
    W_I32LOAD=0x28, W_I32LOAD8U=0x2d, W_I32LOAD16S=0x2e,
    W_I32STORE=0x36, W_I32STORE8=0x3a, W_I32STORE16=0x3b,
    W_I32CONST=0x41, W_I64CONST=0x42,
    W_I32EQZ=0x45, W_I32EQ=0x46, W_I32NE=0x47, W_I32LTS=0x48, W_I32GTS=0x4a, W_I32LES=0x4c, W_I32GES=0x4e,
    W_I32ADD=0x6a, W_I32SUB=0x6b, W_I32MUL=0x6c, W_I32DIVS=0x6d, W_I32REMS=0x6f,
    W_I32AND=0x71, W_I32OR=0x72, W_I32XOR=0x73, W_I32SHL=0x74, W_I32SHRS=0x75,
    W_I64MUL=0x7e, W_I64SHRS=0x87,
    W_I32WRAPI64=0xa7, W_I64EXTENDI32S=0xac
};

// Global Context (front end -- mirrors xcc700t)
char *w_src;
char *w_rodata; int w_rodata_sz; int w_rodata_cap;
int w_token; int w_num_val; int w_line; int w_token_cnt;
char w_str_val[256]; int w_str_len;
uint8_t *w_code; int w_code_sz; int w_code_cap;   // current function's body
char *w_names; int w_names_sz; int w_names_cap;

int w_var_name_off[MAX_VARS];
int w_var_offsets[MAX_VARS];
int w_var_types[MAX_VARS];
int w_n_vars; int w_locals; int w_esp; int w_expr_type; int w_n_globals; int w_bss_size;

// functions: defined and imported
int w_func_name_off[MAX_FUNCS];
int w_func_arity[MAX_FUNCS];   // -1 = unknown yet
int w_func_defined[MAX_FUNCS]; // 1 if defined in this module
uint8_t *w_func_body[MAX_FUNCS]; int w_func_body_sz[MAX_FUNCS];
int w_func_nargs[MAX_FUNCS];
int w_n_funcs;

// call fixups: call sites emit a placeholder func index (padded LEB), patched
// once import/define split is known
int w_callpatch_off[1024]; int w_callpatch_fn[1024]; int w_n_callpatch;
uint8_t *w_callpatch_body[1024];

// "registers" as wasm locals, appended after params
#define NLOCAL_REGS 4
static int w_nargs_cur;
#define LOC_RES (w_nargs_cur + 0)
#define LOC_TMP (w_nargs_cur + 1)
#define LOC_A (w_nargs_cur + 2)
#define LOC_B (w_nargs_cur + 3)

// globals: 0 = $sp (mut). imported const globals: 0 __stack_base, 1 __bss_base, 2 __rodata_base
// After imports, defined global $sp gets index 3.
#define G_STACKBASE 0
#define G_BSSBASE 1
#define G_RODATABASE 2
#define G_SP 3

void w_next(); void w_parse_func();

// --- Emit helpers ---
static void wb(int b) {
    if (w_code_sz + 1 > w_code_cap) { w_code_cap *= 2; w_code = realloc(w_code, w_code_cap); if (!w_code) w_fail("oom"); }
    w_code[w_code_sz++] = (uint8_t)b;
}
static void wleb(uint32_t v) { do { int b = v & 0x7f; v >>= 7; wb(v ? b | 0x80 : b); } while (v); }
static void wsleb(int32_t v) {
    int more = 1;
    while (more) {
        int b = v & 0x7f; v >>= 7;
        if ((v == 0 && !(b & 0x40)) || (v == -1 && (b & 0x40))) more = 0; else b |= 0x80;
        wb(b);
    }
}
static void wleb5(uint32_t v) {  // padded 5-byte LEB (patchable)
    for (int i = 0; i < 4; i++) { wb((v & 0x7f) | 0x80); v >>= 7; }
    wb(v & 0x7f);
}
static void w_i32c(int32_t v) { wb(W_I32CONST); wsleb(v); }
static void w_lget(int i) { wb(W_LGET); wleb(i); }
static void w_lset(int i) { wb(W_LSET); wleb(i); }
static void w_gget(int i) { wb(W_GGET); wleb(i); }
static void w_gset(int i) { wb(W_GSET); wleb(i); }
static void w_load(int op) { wb(op); wleb(op == W_I32LOAD ? 2 : op == W_I32LOAD16S ? 1 : 0); wleb(0); }
static void w_store(int op) { wb(op); wleb(op == W_I32STORE ? 2 : op == W_I32STORE16 ? 1 : 0); wleb(0); }

// address of frame slot [sp + off] on stack
static void w_frameaddr(int off) { w_gget(G_SP); if (off) { w_i32c(off); wb(W_I32ADD); } }
// res <- [sp+off]; [sp+off] <- res etc.
static void w_ld_frame(int loc, int off) { w_frameaddr(off); w_load(W_I32LOAD); w_lset(loc); }
static void w_st_frame(int loc, int off) { w_frameaddr(off); w_lget(loc); w_store(W_I32STORE); }

// virtual expression stack (memory, above locals -- same as xtensa backend)
static void w_push(int loc) { w_st_frame(loc, w_locals + w_esp); w_esp += 4; }
static void w_pop(int loc) { w_esp -= 4; w_ld_frame(loc, w_locals + w_esp); }

static int w_add_name(char *s) {
    int len = (int)strlen(s) + 1;
    if (w_names_sz + len > w_names_cap) w_fail("out of memory (names)");
    int off = w_names_sz; strcpy(w_names + off, s);
    w_names_sz += len; return off;
}

static int w_get_func(char *name, int arity_hint) {
    for (int i = 0; i < w_n_funcs; i++)
        if (!strcmp(w_names + w_func_name_off[i], name)) {
            if (w_func_arity[i] < 0 && arity_hint >= 0) w_func_arity[i] = arity_hint;
            return i;
        }
    if (w_n_funcs >= MAX_FUNCS) w_fail("too many functions");
    w_func_name_off[w_n_funcs] = w_add_name(name);
    w_func_arity[w_n_funcs] = arity_hint;
    w_func_defined[w_n_funcs] = 0;
    w_func_body[w_n_funcs] = NULL; w_func_body_sz[w_n_funcs] = 0;
    w_func_nargs[w_n_funcs] = 0;
    return w_n_funcs++;
}

// --- Lexer (identical grammar to xcc700t) ---
static int xisdigit(int c) { return c >= '0' && c <= '9'; }
static int xisalpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int xisspace(int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
static int xisalnum(int c) { return xisdigit(c) || xisalpha(c); }

void w_next() {
    while (xisspace(*w_src) || (w_src[0]=='/' && w_src[1]=='/')) {
        if (*w_src == '\n') ++w_line;
        if (*w_src == '/') while (*w_src && *w_src != '\n') ++w_src; else ++w_src;
    }
    if (!*w_src) { w_token = T_EOF; return; }
    if (w_src[0]=='+' && w_src[1]=='+') { w_token=T_INC; w_src+=2; return; }
    if (w_src[0]=='-' && w_src[1]=='-') { w_token=T_DEC; w_src+=2; return; }
    if (w_src[0]=='=' && w_src[1]=='=') { w_token=T_EQ; w_src+=2; return; }
    if (w_src[0]=='!' && w_src[1]=='=') { w_token=T_NE; w_src+=2; return; }
    if (w_src[0]=='<' && w_src[1]=='=') { w_token=T_LE; w_src+=2; return; }
    if (w_src[0]=='>' && w_src[1]=='=') { w_token=T_GE; w_src+=2; return; }
    if (w_src[0]=='<' && w_src[1]=='<') { w_token=T_SHL; w_src+=2; return; }
    if (w_src[0]=='>' && w_src[1]=='>') { w_token=T_SHR; w_src+=2; return; }
    if (w_src[0]=='&' && w_src[1]=='&') { w_token=T_LAND; w_src+=2; return; }
    if (w_src[0]=='|' && w_src[1]=='|') { w_token=T_LOR; w_src+=2; return; }
    if (w_src[0]=='.' && w_src[1]=='.' && w_src[2]=='.') { w_token=T_ELLIPSIS; w_src+=3; return; }

    if (xisalpha(*w_src) || *w_src == '_') {
        char *p = w_str_val;
        while (xisalnum(*w_src) || *w_src == '_') { *p = *w_src; ++p; ++w_src; }
        *p = 0;
        w_token = !strcmp(w_str_val,"int") ? T_INT : !strcmp(w_str_val,"char") ? T_CHAR :
            !strcmp(w_str_val,"void") ? T_VOID : !strcmp(w_str_val, "enum") ? T_ENUM :
            !strcmp(w_str_val,"if") ? T_IF : !strcmp(w_str_val,"else") ? T_ELSE :
            !strcmp(w_str_val,"while") ? T_WHILE : !strcmp(w_str_val,"return") ? T_RETURN :
            !strcmp(w_str_val,"int16_t") ? T_SHORT : !strcmp(w_str_val,"short") ? T_SHORT :
            !strcmp(w_str_val,"static") ? T_STATIC : T_IDENT;
    } else if (xisdigit(*w_src)) {
        w_num_val = (int)strtol(w_src, &w_src, 0); w_token = T_NUM;
    } else if (*w_src == '\'') {
        ++w_src;
        if (*w_src == '\\') {
            ++w_src;
            if (*w_src == 'n') w_num_val = '\n'; else if (*w_src == 't') w_num_val = '\t';
            else if (*w_src == 'r') w_num_val = '\r'; else if (*w_src == '0') w_num_val = 0; else w_num_val = *w_src;
        } else w_num_val = *w_src;
        ++w_src; if (*w_src == '\'') ++w_src;
        w_token = T_NUM;
    } else if (*w_src == '"') {
        char *p = w_str_val; ++w_src;
        while (*w_src && *w_src != '"') {
            if (*w_src == '\\') {
                ++w_src;
                if (*w_src == 'n') *p='\n'; else if (*w_src == 't') *p='\t';
                else if (*w_src == 'r') *p='\r'; else if (*w_src == '0') *p='\0'; else *p=*w_src;
            } else *p=*w_src;
            ++w_src; ++p;
        }
        *p = 0; if (*w_src) ++w_src;
        w_str_len = (int)(p - w_str_val);
        w_token = T_STR;
    } else { w_token = *w_src; ++w_src; }
    ++w_token_cnt;
}

static void w_expect(int tok) {
    if (w_token != tok) {
        if (tok < 256 && w_token < 256) w_fail("Line %d: expected '%c', got '%c'", w_line, tok, w_token);
        else if (tok < 256) w_fail("Line %d: expected '%c', got '%s'", w_line, tok, w_str_val);
        else if (w_token < 256) w_fail("Line %d: expected token %d, got '%c'", w_line, tok, w_token);
        else w_fail("Line %d: expected token %d, got '%s'", w_line, tok, w_str_val);
    }
    w_next();
}

// --- Parser + wasm codegen ---
static int w_get_prec(int t) {
    if(t=='?') return 1; if(t==T_LOR) return 2; if(t==T_LAND) return 3;
    if(t=='|') return 4; if(t=='^') return 5; if(t=='&') return 6;
    if(t==T_EQ||t==T_NE) return 7;
    if(t=='<'||t=='>'||t==T_LE||t==T_GE) return 8;
    if(t==T_SHL||t==T_SHR) return 9;
    if(t=='+'||t=='-') return 10; if(t=='*'||t=='/'||t=='%') return 11;
    return 0;
}

static void w_parse_expr(int limit);

static int w_find_var(char *name) {
    for (int i = w_n_vars; i > 0; ) { --i; if (!strcmp(w_names + w_var_name_off[i], name)) return i; }
    return -1;
}

// leave the variable's ADDRESS on the wasm stack
static void w_var_addr_on_stack(int i) {
    if (w_var_types[i] & TF_GLOBAL) { w_gget(G_BSSBASE); w_i32c(w_var_offsets[i]); wb(W_I32ADD); }
    else w_frameaddr(w_var_offsets[i]);
}

// res <- variable value (or address for arrays / const value for enums)
static void w_load_var(int i) {
    int ty = w_var_types[i];
    if (ty & TF_CONST) { w_i32c(w_var_offsets[i]); w_lset(LOC_RES); w_expr_type = TY_INT; return; }
    if (ty & TF_ARR) {
        w_var_addr_on_stack(i); w_lset(LOC_RES);
        w_expr_type = (ty & TF_BYTE) ? TY_BYTEPTR : (ty & TF_SHORT) ? TY_SHORTPTR : TY_INTPTR;
        return;
    }
    int is_byte = ((ty & ~TF_GLOBAL) == TY_BYTE);
    w_var_addr_on_stack(i);
    w_load(is_byte ? W_I32LOAD8U : W_I32LOAD);
    w_lset(LOC_RES);
    w_expr_type = ty & ~TF_GLOBAL;
}

static void w_elem_load(int type) {  // res holds address -> res = *address
    w_lget(LOC_RES);
    if (type & TF_BYTE) w_load(W_I32LOAD8U);
    else if (type & TF_SHORT) w_load(W_I32LOAD16S);
    else w_load(W_I32LOAD);
    w_lset(LOC_RES);
}
static void w_elem_store(int type, int val_loc, int addr_loc) {  // *addr = val
    w_lget(addr_loc); w_lget(val_loc);
    if (type & TF_BYTE) w_store(W_I32STORE8);
    else if (type & TF_SHORT) w_store(W_I32STORE16);
    else w_store(W_I32STORE);
}

// res = base(tmp-slot on virt stack) + index(res) scaled
static void w_index(int base_type) {
    w_next(); w_push(LOC_RES);
    w_parse_expr(1); w_expect(']');
    if (base_type & TF_SHORT) { w_lget(LOC_RES); w_lget(LOC_RES); wb(W_I32ADD); w_lset(LOC_RES); }
    else if (!(base_type & TF_BYTE)) { w_lget(LOC_RES); w_i32c(2); wb(W_I32SHL); w_lset(LOC_RES); }
    w_pop(LOC_TMP);
    w_lget(LOC_TMP); w_lget(LOC_RES); wb(W_I32ADD); w_lset(LOC_RES);
}

// intrinsics: return 1 if handled (result in res)
static int w_intrinsic_call(const char *name, int arg_cnt) {
    // args are in virt-stack slots; arg values popped into locals by caller
    if (!strcmp(name, "fxmul") && arg_cnt == 2) {
        // res = (int32)(((int64)a * b) >> 23)
        w_lget(LOC_A); wb(W_I64EXTENDI32S);
        w_lget(LOC_B); wb(W_I64EXTENDI32S);
        wb(W_I64MUL);
        wb(W_I64CONST); wsleb(23);
        wb(W_I64SHRS);
        wb(W_I32WRAPI64);
        w_lset(LOC_RES);
        return 1;
    }
    if (!strcmp(name, "from_int16") && arg_cnt == 1) {
        w_lget(LOC_A); w_i32c(8); wb(W_I32SHL); w_lset(LOC_RES);
        return 1;
    }
    if (!strcmp(name, "to_int16") && arg_cnt == 1) {
        // s >>= 8; clamp to +/-32767   (structured: nested ifs)
        w_lget(LOC_A); w_i32c(8); wb(W_I32SHRS); w_lset(LOC_RES);
        w_lget(LOC_RES); w_i32c(32767); wb(W_I32GTS);
        wb(W_IF); wb(0x40); w_i32c(32767); w_lset(LOC_RES); wb(W_END);
        w_lget(LOC_RES); w_i32c(-32767); wb(W_I32LTS);
        wb(W_IF); wb(0x40); w_i32c(-32767); w_lset(LOC_RES); wb(W_END);
        return 1;
    }
    return 0;
}

static void w_parse_call(char *name) {
    int arg_cnt = 0; w_next();
    if (w_token != ')') {
        w_parse_expr(1); w_push(LOC_RES); ++arg_cnt;
        while (w_token == ',') {
            w_next(); w_parse_expr(1); w_push(LOC_RES); ++arg_cnt;
            if (arg_cnt > MAX_ARGS) w_fail("Line %d: max %d args", w_line, MAX_ARGS);
        }
    }
    w_expect(')');
    // pop args into locals a,b (intrinsics) or push onto wasm stack (calls)
    if ((!strcmp(name,"fxmul") || !strcmp(name,"to_int16") || !strcmp(name,"from_int16"))
        && (arg_cnt == 1 || arg_cnt == 2)) {
        if (arg_cnt == 2) { w_pop(LOC_B); w_pop(LOC_A); }
        else w_pop(LOC_A);
        if (w_intrinsic_call(name, arg_cnt)) return;
    }
    // real call: fetch args from virt stack in order
    int fi = w_get_func(name, arg_cnt);
    if (w_func_arity[fi] >= 0 && w_func_arity[fi] != arg_cnt)
        w_fail("Line %d: %s takes %d args", w_line, name, w_func_arity[fi]);
    for (int k = arg_cnt; k > 0; k--) {
        // load [sp + locals + esp - 4k .. ] in call order
        w_frameaddr(w_locals + w_esp - 4*k); w_load(W_I32LOAD);
    }
    w_esp -= 4*arg_cnt;
    wb(W_CALL);
    if (w_n_callpatch >= 1024) w_fail("too many calls");
    w_callpatch_body[w_n_callpatch] = NULL;  // fixed after body copy
    w_callpatch_off[w_n_callpatch] = w_code_sz;
    w_callpatch_fn[w_n_callpatch] = fi;
    w_n_callpatch++;
    wleb5(0);
    w_lset(LOC_RES);
}

static void w_parse_factor() {
    if (w_token == T_INC || w_token == T_DEC) {
        int diff = (w_token == T_INC) ? 1 : -1; w_next();
        char name[64]; strcpy(name, w_str_val); w_expect(T_IDENT);
        int i = w_find_var(name);
        if (i < 0) w_fail("Line %d: undefined: %s", w_line, name);
        w_load_var(i);
        w_lget(LOC_RES); w_i32c(diff); wb(W_I32ADD); w_lset(LOC_RES);
        int is_byte = ((w_var_types[i] & ~TF_GLOBAL) == TY_BYTE);
        w_var_addr_on_stack(i); w_lget(LOC_RES);
        w_store(is_byte ? W_I32STORE8 : W_I32STORE);
        w_expr_type = TY_INT;
    } else if (w_token == '!' || w_token == '~' || w_token == '-') {
        int op = w_token; w_next(); w_parse_factor();
        if (op == '-') { w_i32c(0); w_lget(LOC_RES); wb(W_I32SUB); w_lset(LOC_RES); }
        else if (op == '~') { w_lget(LOC_RES); w_i32c(-1); wb(W_I32XOR); w_lset(LOC_RES); }
        else { w_lget(LOC_RES); wb(W_I32EQZ); w_lset(LOC_RES); }
        w_expr_type = TY_INT;
    } else if (w_token == '*') {
        w_next(); w_parse_factor(); int pt = w_expr_type;
        w_elem_load(pt);
        w_expr_type = (pt & TF_BYTE) ? TY_BYTE : TY_INT;
    } else if (w_token == '&') {
        w_next(); char name[64]; strcpy(name, w_str_val); w_expect(T_IDENT);
        int i = w_find_var(name);
        if (i < 0) w_fail("Line %d: undefined: %s", w_line, name);
        w_var_addr_on_stack(i); w_lset(LOC_RES);
        w_expr_type = (w_var_types[i] & TF_BYTE) ? TY_BYTEPTR : (w_var_types[i] & TF_SHORT) ? TY_SHORTPTR : TY_INTPTR;
    } else if (w_token == T_NUM) {
        w_i32c(w_num_val); w_lset(LOC_RES); w_expr_type = TY_INT; w_next();
    } else if (w_token == T_STR) {
        if (w_rodata_sz + w_str_len + 1 > w_rodata_cap) w_fail("out of memory (rodata)");
        w_gget(G_RODATABASE); w_i32c(w_rodata_sz); wb(W_I32ADD); w_lset(LOC_RES);
        memcpy(w_rodata + w_rodata_sz, w_str_val, w_str_len + 1);
        w_rodata_sz += w_str_len + 1;
        w_expr_type = TY_BYTEPTR; w_next();
    } else if (w_token == T_IDENT) {
        char name[64]; strcpy(name, w_str_val); w_next();
        if (w_token == '(') {
            w_parse_call(name); w_expr_type = TY_INT;
        } else {
            int i = w_find_var(name);
            if (i < 0) w_fail("Line %d: undefined: %s", w_line, name);
            w_load_var(i);
            if (w_token == '[') {
                int bt = w_expr_type;
                w_index(bt);
                w_elem_load(bt);
                w_expr_type = (bt & TF_BYTE) ? TY_BYTE : TY_INT;
            }
        }
    } else if (w_token == '(') {
        w_next();
        if (w_token == T_INT || w_token == T_CHAR || w_token == T_SHORT || w_token == T_VOID) {
            // cast: accepted and ignored
            w_next(); while (w_token == '*') w_next();
            w_expect(')');
            w_parse_factor();
        } else { w_parse_expr(1); w_expect(')'); }
    }
    else w_fail("Line %d: unexpected token", w_line);
}

static void w_binop(int op) {
    // operands: tmp (left), res (right) -> res
    if (op == T_LAND || op == T_LOR) {
        w_lget(LOC_TMP); wb(W_I32EQZ); wb(W_I32EQZ);   // !!tmp
        w_lget(LOC_RES); wb(W_I32EQZ); wb(W_I32EQZ);   // !!res
        wb(op == T_LAND ? W_I32AND : W_I32OR);
        w_lset(LOC_RES);
    } else {
        w_lget(LOC_TMP); w_lget(LOC_RES);
        if(op=='+') wb(W_I32ADD);
        else if(op=='-') wb(W_I32SUB);
        else if(op=='*') wb(W_I32MUL);
        else if(op=='/') wb(W_I32DIVS);
        else if(op=='%') wb(W_I32REMS);
        else if(op=='&') wb(W_I32AND);
        else if(op=='|') wb(W_I32OR);
        else if(op=='^') wb(W_I32XOR);
        else if(op==T_SHL) wb(W_I32SHL);
        else if(op==T_SHR) wb(W_I32SHRS);
        else if(op=='<') wb(W_I32LTS);
        else if(op=='>') wb(W_I32GTS);
        else if(op==T_LE) wb(W_I32LES);
        else if(op==T_GE) wb(W_I32GES);
        else if(op==T_EQ) wb(W_I32EQ);
        else if(op==T_NE) wb(W_I32NE);
        else w_fail("Line %d: bad op", w_line);
        w_lset(LOC_RES);
    }
    w_expr_type = TY_INT;
}

static void w_parse_expr(int limit) {
    w_parse_factor();
    while (w_get_prec(w_token) >= limit) {
        int op = w_token; w_next();
        if (op == '?') {
            // res ? expr : expr  -- structured if/else with i32 result
            w_lget(LOC_RES);
            wb(W_IF); wb(0x7f);           // if (result i32)
            w_parse_expr(2); w_lget(LOC_RES);
            w_expect(':');
            wb(W_ELSE);
            w_parse_expr(1); w_lget(LOC_RES);
            wb(W_END);
            w_lset(LOC_RES);
        } else {
            w_push(LOC_RES); w_parse_expr(w_get_prec(op) + 1);
            w_pop(LOC_TMP); w_binop(op);
        }
    }
}

static int w_align4(int x) { return (x + 3) & ~3; }

static void w_parse_stmt() {
    w_esp = 0;
    if (w_token == T_WHILE) {
        w_next();
        wb(W_BLOCK); wb(0x40);            // block (exit)
        wb(W_LOOP); wb(0x40);             //   loop
        w_expect('('); w_parse_expr(1); w_expect(')');
        w_lget(LOC_RES); wb(W_I32EQZ); wb(W_BRIF); wleb(1);   // exit if !cond
        w_parse_stmt();
        wb(W_BR); wleb(0);                //   continue loop
        wb(W_END); wb(W_END);
    } else if (w_token == T_IF) {
        w_next(); w_expect('('); w_parse_expr(1); w_expect(')');
        w_lget(LOC_RES);
        wb(W_IF); wb(0x40);
        w_parse_stmt();
        if (w_token == T_ELSE) { wb(W_ELSE); w_next(); w_parse_stmt(); }
        wb(W_END);
    } else if (w_token == '{') {
        w_next(); while (w_token != '}' && w_token != T_EOF) w_parse_stmt(); w_expect('}');
    } else if (w_token == T_INT || w_token == T_CHAR || w_token == T_SHORT) {
        int is_byte = (w_token == T_CHAR); int is_short = (w_token == T_SHORT); w_next();
        int is_ptr = 0; while (w_token == '*') { is_ptr = 1; w_next(); }
        w_var_name_off[w_n_vars] = w_add_name(w_str_val);
        w_var_offsets[w_n_vars] = w_locals;
        if (++w_n_vars >= MAX_VARS) w_fail("MAX_VARS exceeded");
        w_expect(T_IDENT);
        if (w_token == '[') {
            w_next(); int sz = w_num_val; w_expect(T_NUM); w_expect(']');
            w_var_types[w_n_vars-1] = is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR;
            w_locals += (is_byte ? w_align4(sz) : is_short ? w_align4(sz * 2) : sz * 4);
        } else {
            w_var_types[w_n_vars-1] = is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT);
            w_locals += 4;
            w_expect('='); w_parse_expr(1);
            int off = w_var_offsets[w_n_vars-1];
            if (is_byte && !is_ptr) { w_frameaddr(off); w_lget(LOC_RES); w_store(W_I32STORE8); }
            else { w_frameaddr(off); w_lget(LOC_RES); w_store(W_I32STORE); }
        }
        if (w_locals >= MAX_LOCAL_VARS * 4) w_fail("Line %d: frame too large", w_line);
        w_expect(';');
    } else if (w_token == T_STATIC) {
        // static locals -> function-scoped bss (same as xcc700t)
        w_next();
        int is_byte = (w_token == T_CHAR); int is_short = (w_token == T_SHORT);
        if (w_token != T_INT && w_token != T_CHAR && w_token != T_SHORT)
            w_fail("Line %d: static must be int/char/int16_t", w_line);
        w_next();
        int is_ptr = 0; while (w_token == '*') { is_ptr = 1; w_next(); }
        w_var_name_off[w_n_vars] = w_add_name(w_str_val);
        w_expect(T_IDENT);
        int ty = TF_GLOBAL | (is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT));
        if (w_token == '[') {
            w_next(); int sz = 0;
            if (w_token == T_NUM) { sz = w_num_val; w_next(); }
            else {
                int ci = w_find_var(w_str_val);
                if (ci < 0 || !(w_var_types[ci] & TF_CONST)) w_fail("Line %d: array size expected", w_line);
                sz = w_var_offsets[ci]; w_next();
            }
            w_expect(']');
            ty = TF_GLOBAL | (is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR);
            w_var_offsets[w_n_vars] = w_bss_size;
            w_bss_size += (is_byte ? w_align4(sz) : is_short ? w_align4(sz * 2) : sz * 4);
        } else {
            w_var_offsets[w_n_vars] = w_bss_size; w_bss_size += 4;
            if (w_token == '=') {
                w_next();
                if (w_token != T_NUM || w_num_val != 0) w_fail("Line %d: static initializers must be 0", w_line);
                w_next();
            }
        }
        w_var_types[w_n_vars] = ty;
        if (++w_n_vars >= MAX_VARS) w_fail("MAX_VARS exceeded");
        w_expect(';');
    } else if (w_token == T_RETURN) {
        w_next();
        if (w_token != ';') w_parse_expr(1); else { w_i32c(0); w_lset(LOC_RES); }
        // epilogue: restore sp, return res
        w_gget(G_SP); w_i32c(MAX_LOCAL_VARS * 4 + 32); wb(W_I32ADD); w_gset(G_SP);
        w_lget(LOC_RES); wb(W_RETURN);
        w_expect(';');
    } else if (w_token == T_IDENT) {
        char name[64]; strcpy(name, w_str_val); w_next();
        if (w_token == '(') {
            w_parse_call(name); w_expect(';');
        } else {
            int i = w_find_var(name);
            if (i < 0) w_fail("Line %d: undefined: %s", w_line, name);
            if (w_token == '[') {
                w_load_var(i);
                int bt = w_expr_type;
                w_index(bt); w_push(LOC_RES);
                w_expect('='); w_parse_expr(1);
                w_pop(LOC_TMP);
                w_elem_store(bt, LOC_RES, LOC_TMP);
            } else {
                w_expect('='); w_parse_expr(1);
                int is_byte = ((w_var_types[i] & ~TF_GLOBAL) == TY_BYTE);
                w_var_addr_on_stack(i); w_lget(LOC_RES);
                w_store(is_byte ? W_I32STORE8 : W_I32STORE);
            }
            w_expect(';');
        }
    } else if (w_token == '*') {
        w_next(); w_parse_factor();
        int pt = w_expr_type;
        w_push(LOC_RES); w_expect('='); w_parse_expr(1);
        w_pop(LOC_TMP);
        w_elem_store(pt, LOC_RES, LOC_TMP);
        w_expect(';');
    } else { w_parse_expr(1); w_expect(';'); }
}

void w_parse_func() {
    if (w_token == T_ENUM) {
        w_next(); if (w_token == T_IDENT) w_next();
        w_expect('{'); int val = 0;
        while (w_token == T_IDENT) {
            w_var_name_off[w_n_globals] = w_add_name(w_str_val);
            w_var_offsets[w_n_globals] = val;
            w_var_types[w_n_globals] = TF_CONST | TY_INT;
            ++w_n_globals; w_n_vars = w_n_globals; w_next();
            if (w_token == '=') { w_next(); val = w_num_val; w_var_offsets[w_n_globals - 1] = val; w_next(); }
            ++val; if (w_token == ',') w_next();
        }
        w_expect('}'); w_expect(';'); return;
    }

    int is_byte = (w_token == T_CHAR); int is_short = (w_token == T_SHORT);
    if (w_token == T_INT || w_token == T_CHAR || w_token == T_VOID || w_token == T_SHORT) w_next();
    int is_ptr = 0; while (w_token=='*') { is_ptr = 1; w_next(); }
    char name[64]; strcpy(name, w_str_val); w_expect(T_IDENT);

    if (w_token == ';' || w_token == '[') {
        int ty = TF_GLOBAL | (is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT));
        if (w_token == '[') {
            w_next(); int sz = 0;
            if (w_token == T_NUM) { sz = w_num_val; w_next(); }
            else if (w_token == T_IDENT) {
                int i = w_find_var(w_str_val);
                if (i < 0 || !(w_var_types[i] & TF_CONST)) w_fail("Line %d: undefined constant: %s", w_line, w_str_val);
                sz = w_var_offsets[i]; w_next();
            } else w_fail("Line %d: array size expected", w_line);
            w_expect(']');
            ty = TF_GLOBAL | (is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR);
            w_var_offsets[w_n_globals] = w_bss_size;
            w_bss_size += ((ty & TF_BYTE) ? w_align4(sz) : (ty & TF_SHORT) ? w_align4(sz * 2) : sz * 4);
        } else {
            w_var_offsets[w_n_globals] = w_bss_size; w_bss_size += 4;
        }
        w_var_name_off[w_n_globals] = w_add_name(name);
        w_var_types[w_n_globals] = ty;
        ++w_n_globals; w_n_vars = w_n_globals;
        w_expect(';'); return;
    }

    w_expect('('); w_n_vars = w_n_globals; w_locals = 32; int n_args = 0;
    while (w_token != ')') {
        is_byte = 0; is_short = 0; int ptr_count = 0;
        if (w_token == T_CHAR) { is_byte = 1; w_next(); }
        else if (w_token == T_SHORT) { is_short = 1; w_next(); }
        else if (w_token == T_INT || w_token == T_VOID || w_token == T_IDENT || w_token == T_ELLIPSIS) w_next();
        while (w_token == '*') { ++ptr_count; w_next(); }
        if (w_token == T_IDENT) {
            w_var_name_off[w_n_vars] = w_add_name(w_str_val);
            w_var_offsets[w_n_vars] = w_locals;
            w_var_types[w_n_vars] = ptr_count >= 2 ? TY_INTPTR : ptr_count ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT);
            w_locals += 4; ++w_n_vars; ++n_args;
            if (w_n_vars >= MAX_VARS) w_fail("MAX_VARS exceeded");
            w_next();
        }
        if (w_token == ',') w_next();
    }
    w_expect(')');
    int fi = w_get_func(name, n_args);
    if (w_token == ';') { w_next(); return; }  // prototype

    if (w_func_defined[fi]) w_fail("Line %d: %s redefined", w_line, name);
    w_func_defined[fi] = 1;
    w_func_nargs[fi] = n_args;
    w_nargs_cur = n_args;

    // fresh body buffer
    w_code_cap = 4096; w_code = malloc(w_code_cap); w_code_sz = 0;
    if (!w_code) w_fail("oom");
    int patch_base = w_n_callpatch;

    w_expect('{');
    // prologue: sp -= frame; store params into frame
    w_gget(G_SP); w_i32c(MAX_LOCAL_VARS * 4 + 32); wb(W_I32SUB); w_gset(G_SP);
    for (int j = 0; j < n_args; j++) {
        w_frameaddr(w_var_offsets[w_n_globals + j]); w_lget(j); w_store(W_I32STORE);
    }
    while (w_token != '}' && w_token != T_EOF) w_parse_stmt();
    // implicit epilogue
    w_gget(G_SP); w_i32c(MAX_LOCAL_VARS * 4 + 32); wb(W_I32ADD); w_gset(G_SP);
    w_i32c(0);
    wb(W_END);
    w_expect('}');

    w_func_body[fi] = w_code;
    w_func_body_sz[fi] = w_code_sz;
    // mark which body the patches belong to
    for (int p = patch_base; p < w_n_callpatch; p++) w_callpatch_body[p] = w_code;
    w_code = NULL;
}

// --- Module writer ---
typedef struct { uint8_t *b; int sz; int cap; } wbuf_t;
static void bput(wbuf_t *o, const void *p, int n) {
    if (o->sz + n > o->cap) { while (o->sz + n > o->cap) o->cap *= 2; o->b = realloc(o->b, o->cap); if (!o->b) w_fail("oom"); }
    memcpy(o->b + o->sz, p, n); o->sz += n;
}
static void bbyte(wbuf_t *o, int v) { uint8_t b = (uint8_t)v; bput(o, &b, 1); }
static void bleb(wbuf_t *o, uint32_t v) { do { int b = v & 0x7f; v >>= 7; bbyte(o, v ? b | 0x80 : b); } while (v); }
static void bsleb(wbuf_t *o, int32_t v) {
    int more = 1;
    while (more) {
        int b = v & 0x7f; v >>= 7;
        if ((v == 0 && !(b & 0x40)) || (v == -1 && (b & 0x40))) more = 0; else b |= 0x80;
        bbyte(o, b);
    }
}
static void bname(wbuf_t *o, const char *s) { int n = (int)strlen(s); bleb(o, n); bput(o, s, n); }
static void bsection(wbuf_t *o, int id, wbuf_t *content) {
    bbyte(o, id); bleb(o, content->sz); bput(o, content->b, content->sz);
}
static wbuf_t bnew(void) { wbuf_t o; o.cap = 1024; o.sz = 0; o.b = malloc(o.cap); if (!o.b) w_fail("oom"); return o; }

static uint8_t *w_write_module(const char *entry_name, uint32_t *out_size) {
    // function index space: imports first (undefined funcs that are actually
    // called -- prototyped-but-unused helpers don't become imports), then defined
    int import_idx[MAX_FUNCS], defined_idx[MAX_FUNCS], referenced[MAX_FUNCS];
    int n_imports = 0, n_defined = 0;
    memset(referenced, 0, sizeof(referenced));
    for (int p = 0; p < w_n_callpatch; p++) referenced[w_callpatch_fn[p]] = 1;
    for (int i = 0; i < w_n_funcs; i++) {
        import_idx[i] = -1;
        if (w_func_defined[i]) defined_idx[i] = -1;  // fill later
        else if (referenced[i]) {
            if (w_func_arity[i] < 0) w_func_arity[i] = 0;
            import_idx[i] = n_imports++;
        } else if (!w_func_defined[i]) {
            // declared but never called: drop it
        }
    }
    for (int i = 0; i < w_n_funcs; i++)
        if (w_func_defined[i]) defined_idx[i] = n_imports + n_defined++;

    int entry_fi = -1;
    for (int i = 0; i < w_n_funcs; i++)
        if (w_func_defined[i] && !strcmp(w_names + w_func_name_off[i], entry_name)) entry_fi = i;
    if (entry_fi < 0) w_fail("no %s() defined", entry_name);

    // patch call sites with final indices
    for (int p = 0; p < w_n_callpatch; p++) {
        int fi = w_callpatch_fn[p];
        uint32_t idx = w_func_defined[fi] ? (uint32_t)defined_idx[fi] : (uint32_t)import_idx[fi];
        uint8_t *at = w_callpatch_body[p] + w_callpatch_off[p];
        for (int k = 0; k < 4; k++) { at[k] = (idx & 0x7f) | 0x80; idx >>= 7; }
        at[4] = idx & 0x7f;
    }

    // types: one per arity 0..MAX_ARGS, all (i32^n) -> i32
    wbuf_t ty = bnew();
    bleb(&ty, MAX_ARGS + 1);
    for (int n = 0; n <= MAX_ARGS; n++) {
        bbyte(&ty, 0x60); bleb(&ty, n);
        for (int k = 0; k < n; k++) bbyte(&ty, 0x7f);
        bleb(&ty, 1); bbyte(&ty, 0x7f);
    }

    // imports: memory, 3 const globals, undefined funcs
    wbuf_t im = bnew();
    bleb(&im, 4 + n_imports);
    bname(&im, "env"); bname(&im, "memory"); bbyte(&im, 0x02); bbyte(&im, 0x00); bleb(&im, 1);
    bname(&im, "env"); bname(&im, "__stack_base"); bbyte(&im, 0x03); bbyte(&im, 0x7f); bbyte(&im, 0x00);
    bname(&im, "env"); bname(&im, "__bss_base"); bbyte(&im, 0x03); bbyte(&im, 0x7f); bbyte(&im, 0x00);
    bname(&im, "env"); bname(&im, "__rodata_base"); bbyte(&im, 0x03); bbyte(&im, 0x7f); bbyte(&im, 0x00);
    for (int i = 0; i < w_n_funcs; i++) {
        if (w_func_defined[i] || import_idx[i] < 0) continue;
        bname(&im, "env"); bname(&im, w_names + w_func_name_off[i]);
        bbyte(&im, 0x00); bleb(&im, w_func_arity[i]);   // type index == arity
    }

    // functions (type of each defined func)
    wbuf_t fn = bnew();
    bleb(&fn, n_defined);
    for (int i = 0; i < w_n_funcs; i++)
        if (w_func_defined[i]) bleb(&fn, w_func_nargs[i]);

    // globals: $sp (mut i32) = __stack_base
    wbuf_t gl = bnew();
    bleb(&gl, 1);
    bbyte(&gl, 0x7f); bbyte(&gl, 0x01);
    bbyte(&gl, W_GGET); bleb(&gl, G_STACKBASE); bbyte(&gl, W_END);

    // exports: every defined function by name
    wbuf_t ex = bnew();
    bleb(&ex, n_defined);
    for (int i = 0; i < w_n_funcs; i++) {
        if (!w_func_defined[i]) continue;
        bname(&ex, w_names + w_func_name_off[i]);
        bbyte(&ex, 0x00); bleb(&ex, defined_idx[i]);
    }

    // code
    wbuf_t co = bnew();
    bleb(&co, n_defined);
    for (int i = 0; i < w_n_funcs; i++) {
        if (!w_func_defined[i]) continue;
        wbuf_t body = bnew();
        bleb(&body, 1); bleb(&body, NLOCAL_REGS); bbyte(&body, 0x7f);  // 4 scratch i32 locals
        bput(&body, w_func_body[i], w_func_body_sz[i]);
        bleb(&co, body.sz); bput(&co, body.b, body.sz);
        free(body.b);
    }

    // data: rodata at __rodata_base
    wbuf_t da = bnew();
    if (w_rodata_sz > 0) {
        bleb(&da, 1);
        bleb(&da, 0);  // active, memory 0
        bbyte(&da, W_GGET); bleb(&da, G_RODATABASE); bbyte(&da, W_END);
        bleb(&da, w_rodata_sz); bput(&da, w_rodata, w_rodata_sz);
    } else bleb(&da, 0);

    wbuf_t mod = bnew();
    static const uint8_t hdr[8] = { 0, 'a', 's', 'm', 1, 0, 0, 0 };
    bput(&mod, hdr, 8);
    bsection(&mod, 1, &ty);
    bsection(&mod, 2, &im);
    bsection(&mod, 3, &fn);
    bsection(&mod, 6, &gl);
    bsection(&mod, 7, &ex);
    bsection(&mod, 10, &co);
    if (w_rodata_sz > 0) bsection(&mod, 11, &da);

    free(ty.b); free(im.b); free(fn.b); free(gl.b); free(ex.b); free(co.b); free(da.b);
    *out_size = (uint32_t)mod.sz;
    return mod.b;
}

int xcc700w_compile(const char *source, const char *entry_name,
                    uint8_t **wasm_out, uint32_t *wasm_size,
                    uint32_t *bss_size, uint32_t *rodata_size,
                    char *err, int errlen) {
    w_errbuf = err; w_errcap = errlen;
    if (err != NULL && errlen > 0) err[0] = 0;
    w_line = 1; w_token = 0; w_token_cnt = 0; w_num_val = 0; w_str_len = 0;
    w_n_vars = 0; w_locals = 0; w_esp = 0; w_expr_type = 0; w_n_globals = 0; w_bss_size = 0;
    w_n_funcs = 0; w_n_callpatch = 0;
    w_rodata_sz = 0; w_names_sz = 0;
    w_rodata_cap = 2048; w_rodata = malloc(w_rodata_cap);
    w_names_cap = 4096; w_names = malloc(w_names_cap);
    w_code = NULL; w_code_sz = 0; w_code_cap = 0;
    *wasm_out = NULL; *wasm_size = 0;
    if (w_rodata == NULL || w_names == NULL) {
        free(w_rodata); free(w_names);
        if (err && errlen > 0) snprintf(err, errlen, "out of memory");
        return -1;
    }
    for (int i = 0; i < MAX_FUNCS; i++) { w_func_body[i] = NULL; }
    w_src = (char *)source;
    if (setjmp(w_jmp)) {
        for (int i = 0; i < w_n_funcs; i++) free(w_func_body[i]);
        free(w_code); free(w_rodata); free(w_names);
        return -1;
    }
    w_next(); while (w_token != T_EOF) w_parse_func();
    uint8_t *mod = w_write_module(entry_name, wasm_size);
    if (bss_size) *bss_size = (uint32_t)w_bss_size;
    if (rodata_size) *rodata_size = (uint32_t)w_rodata_sz;
    for (int i = 0; i < w_n_funcs; i++) free(w_func_body[i]);
    free(w_rodata); free(w_names);
    *wasm_out = mod;
    return 0;
}

#ifdef XCC700W_CLI
// Host CLI: xcc700w <in.c> -o <out.wasm> [entry]
int main(int argc, char **argv) {
    const char *in = argc > 1 ? argv[1] : "input.c";
    const char *out = (argc > 3 && !strcmp(argv[2], "-o")) ? argv[3] : "output.wasm";
    const char *entry = argc > 4 ? argv[4] : "process";
    FILE *f = fopen(in, "rb");
    if (f == NULL) { fprintf(stderr, "cannot open %s\n", in); return 1; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    if (fread(buf, 1, sz, f) != (size_t)sz) { fprintf(stderr, "read failed\n"); return 1; }
    buf[sz] = 0; fclose(f);
    uint8_t *wasm; uint32_t wasm_sz, bss, ro; char err[512];
    if (xcc700w_compile(buf, entry, &wasm, &wasm_sz, &bss, &ro, err, sizeof(err)) != 0) {
        fprintf(stderr, "compile failed: %s\n", err); return 1;
    }
    FILE *o = fopen(out, "wb");
    fwrite(wasm, 1, wasm_sz, o); fclose(o);
    fprintf(stderr, "%s: %u bytes wasm, %u bss, %u rodata, entry %s\n", out, (unsigned)wasm_sz, (unsigned)bss, (unsigned)ro, entry);
    return 0;
}
#endif
