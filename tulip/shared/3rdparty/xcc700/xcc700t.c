// xcc700t.c - Tulip's fork of xcc700, a mini C compiler for esp32 / Xtensa
// Upstream: https://github.com/valdanylchuk/xcc700 (MIT, see LICENSE)
//
// Tulip changes, for compiling tulip.install_c_process()/install_c_osc()
// user DSP on-device (see docs/user_c_dsp_design.md):
//   - int16_t / short pointers and arrays (l16si/s16i, x2 indexing);
//     scalar int16_t locals/params are treated as int
//   - casts are accepted and ignored (stores truncate by element size)
//   - `static` locals become function-scoped .bss (zero-init, persistent --
//     how user DSP keeps state between render calls)
//   - callable API xcc700_compile() with in-memory ELF output and error
//     capture (setjmp) instead of main()/exit()/file IO; the entry symbol
//     name is a parameter and lands in e_entry, so the ELF loader hands
//     back a ready function pointer after relocation.
// Build the CLI for host testing with -DXCC700_CLI.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <setjmp.h>

#include "xcc700t.h"

static jmp_buf xcc_jmp;
static char *xcc_errbuf; static int xcc_errcap;
static void xcc_fail(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (xcc_errbuf != NULL && xcc_errcap > 0 && xcc_errbuf[0] == 0)
        vsnprintf(xcc_errbuf, xcc_errcap, fmt, ap);
    va_end(ap);
    longjmp(xcc_jmp, 1);
}

// --- Constants & Globals ---
enum {
    MAX_VARS=256, MAX_LOCAL_VARS=128, MAX_LITS=256, MAX_PATCHES=1200, STRTAB=2048,
    R_XTENSA_RELATIVE=5, R_XTENSA_JMP_SLOT=4,
    SP_REG=1, RES_REG=8, TMP_REG=9, ARG1_REG=10, ARG2_REG=11
};

enum { 
    T_EOF=0, T_INT=256, T_CHAR, T_VOID, T_IDENT, T_NUM, T_STR, T_EQ, T_NE, T_LE, T_GE,
    T_SHL, T_SHR, T_LAND, T_LOR, T_RETURN, T_ELLIPSIS, T_IF, T_ELSE, T_WHILE, T_ENUM, T_INC, T_DEC,
    T_SHORT, T_STATIC
};

enum { L_INT, L_STR, L_FUNC, L_BSS };
// tulip fork: TF_SHORT marks 16-bit *elements* (int16_t pointers/arrays use
// l16si/s16i and x2 indexing). Scalar int16_t locals/params are treated as int.
enum { TF_BYTE=1, TF_PTR=2, TF_ARR=4, TF_GLOBAL=8, TF_CONST=16, TF_SHORT=32 };
enum { TY_INT=0, TY_BYTE=1, TY_INTPTR=2, TY_BYTEPTR=3, TY_INTARR=4, TY_BYTEARR=5,
       TY_SHORTPTR=TF_PTR|TF_SHORT, TY_SHORTARR=TF_ARR|TF_SHORT };

// Global Context
char *src;
char *rodata; int rodata_sz; int rodata_cap;
int token; int num_val; int line; int token_cnt;
char str_val[256]; int str_len;
char *code_data; int code_size; int code_cap;
char *name_buf; int name_sz; int name_cap;

// Vars
int var_name_off[MAX_VARS];
int var_offsets[MAX_VARS];
int var_types[MAX_VARS];
int n_vars; int locals; int esp; int expr_type; int n_globals; int bss_size;

// Funcs
int func_name_off[MAX_VARS];
int func_addrs[MAX_VARS];
int n_funcs;

// Literals
int lit_vals[MAX_LITS];
int lit_types[MAX_LITS];
int n_lits;

// Patches
int patch_offs[MAX_PATCHES];
int patch_lits[MAX_PATCHES];
int n_patches;

void next(); void parse_func();
uint8_t *write_elf_mem(const char *entry_name, uint32_t *out_size);

int xcc700_compile(const char *source, const char *entry_name,
                   uint8_t **elf_out, uint32_t *elf_size, char *err, int errlen) {
    xcc_errbuf = err; xcc_errcap = errlen;
    if (err != NULL && errlen > 0) err[0] = 0;
    // Reset all compiler state so the translation unit is reusable.
    line = 1; token = 0; token_cnt = 0; num_val = 0; str_len = 0;
    n_vars = 0; locals = 0; esp = 0; expr_type = 0; n_globals = 0; bss_size = 0;
    n_funcs = 0; n_lits = 0; n_patches = 0;
    code_size = 0; rodata_sz = 0; name_sz = 0;
    code_cap = 32768; code_data = malloc(code_cap);
    rodata_cap = 2048; rodata = malloc(rodata_cap);
    name_cap = 4096; name_buf = malloc(name_cap);
    *elf_out = NULL; *elf_size = 0;
    if (code_data == NULL || rodata == NULL || name_buf == NULL) {
        free(code_data); free(rodata); free(name_buf);
        if (err != NULL && errlen > 0) snprintf(err, errlen, "out of memory");
        return -1;
    }
    src = (char *)source;
    if (setjmp(xcc_jmp)) {
        // A parse/codegen error longjmp'd here. (write_elf_mem temporaries can
        // leak on its rare failure paths; acceptable for a failed compile.)
        free(code_data); free(rodata); free(name_buf);
        return -1;
    }
    next(); while (token != T_EOF) parse_func();
    uint8_t *out = write_elf_mem(entry_name, elf_size);
    free(code_data); free(rodata); free(name_buf);
    *elf_out = out;
    return 0;
}

// Replace <ctype.h> macros to avoid ABI issues
static int xisdigit(int c) { return c >= '0' && c <= '9'; }
static int xisalpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static int xisspace(int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
static int xisalnum(int c) { return xisdigit(c) || xisalpha(c); }
#define isdigit xisdigit
#define isalpha xisalpha
#define isspace xisspace
#define isalnum xisalnum

// Helpers
void put32(char *b, int v) { b[0]=v; b[1]=v>>8; b[2]=v>>16; b[3]=v>>24; }
void put16(char *b, int v) { b[0]=v; b[1]=v>>8; }

// --- Lexer ---
void next() {
    while (isspace(*src) || (src[0]=='/' && src[1]=='/')) {
        if (*src == '\n') ++line;
        if (*src == '/') while (*src && *src != '\n') ++src; else ++src;
    }
    if (!*src) { token = T_EOF; return; }
    if (src[0]=='+' && src[1]=='+') { token=T_INC; src=src+2; return; }
    if (src[0]=='-' && src[1]=='-') { token=T_DEC; src=src+2; return; }
    if (src[0]=='=' && src[1]=='=') { token=T_EQ; src=src+2; return; }
    if (src[0]=='!' && src[1]=='=') { token=T_NE; src=src+2; return; }
    if (src[0]=='<' && src[1]=='=') { token=T_LE; src=src+2; return; }
    if (src[0]=='>' && src[1]=='=') { token=T_GE; src=src+2; return; }
    if (src[0]=='<' && src[1]=='<') { token=T_SHL; src=src+2; return; }
    if (src[0]=='>' && src[1]=='>') { token=T_SHR; src=src+2; return; }
    if (src[0]=='&' && src[1]=='&') { token=T_LAND; src=src+2; return; }
    if (src[0]=='|' && src[1]=='|') { token=T_LOR; src=src+2; return; }
    if (src[0]=='.' && src[1]=='.' && src[2]=='.') { token=T_ELLIPSIS; src=src+3; return; }

    if (isalpha(*src) || *src == '_') {
        char *p = str_val;
        while (isalnum(*src) || *src == '_') { *p = *src; ++p; ++src; }
        *p = 0;
        token = !strcmp(str_val,"int") ? T_INT : !strcmp(str_val,"char") ? T_CHAR :
            !strcmp(str_val,"void") ? T_VOID : !strcmp(str_val, "enum") ? T_ENUM :
            !strcmp(str_val,"if") ? T_IF : !strcmp(str_val,"else") ? T_ELSE :
            !strcmp(str_val,"while") ? T_WHILE : !strcmp(str_val,"return") ? T_RETURN :
            !strcmp(str_val,"int16_t") ? T_SHORT : !strcmp(str_val,"short") ? T_SHORT :
            !strcmp(str_val,"static") ? T_STATIC : T_IDENT;
    } else if (isdigit(*src)) {
        num_val = strtol(src, &src, 0); token = T_NUM;
    } else if (*src == '\'') {
        ++src;
        if (*src == '\\') {
            ++src;
            if (*src == 'n') num_val = '\n'; else if (*src == 't') num_val = '\t';
            else if (*src == 'r') num_val = '\r'; else if (*src == '0') num_val = 0; else num_val = *src;
        } else num_val = *src;
        ++src; if (*src == '\'') ++src;
        token = T_NUM;
    } else if (*src == '"') {
        char *p = str_val; ++src;
        while (*src && *src != '"') {
            if (*src == '\\') {
                ++src;
                if (*src == 'n') *p='\n'; else if (*src == 't') *p='\t';
                else if (*src == 'r') *p='\r'; else if (*src == '0') *p = '\0'; else *p=*src;
            } else *p=*src;
            ++src; ++p;
        }
        *p = 0; if (*src) ++src;
        str_len = p - str_val;
        token = T_STR;
    } else { token = *src; ++src; }
    ++token_cnt;
}

void expect(int tok) {
    if (token != tok) {
        if (tok < 256 && token < 256) xcc_fail("Line %d: expected '%c', got '%c'", line, tok, token);
        else if (tok < 256) xcc_fail("Line %d: expected '%c', got '%s'", line, tok, str_val);
        else if (token < 256) xcc_fail("Line %d: expected token %d, got '%c'", line, tok, token);
        else xcc_fail("Line %d: expected token %d, got '%s'", line, tok, str_val);
    }
    next();
}

// --- Code Emitter ---
void ensure_code_capacity(int n) {
    if (code_size + n > code_cap) { xcc_fail("Error: Out of memory (code)!"); }
}
void emit3(int b0, int b1, int b2) {
    ensure_code_capacity(3);
    code_data[code_size] = b0; code_data[code_size+1] = b1; code_data[code_size+2] = b2;
    code_size = code_size + 3;
}
void emit2(int b0, int b1) {
    ensure_code_capacity(2);
    code_data[code_size] = b0; ++code_size;
    code_data[code_size] = b1; ++code_size;
}

void emit_rrr(int op, int op0, int r, int s, int t) { emit3((t<<4)|op0, (r<<4)|s, op); }
void emit_l32i(int d, int b, int off) { emit3((d<<4)|2, (2<<4)|b, off/4); }
void emit_s32i(int s, int b, int off) { emit3((s<<4)|2, (6<<4)|b, off/4); }
void emit_l8ui(int d, int b, int off) { emit3((d<<4)|2, (0<<4)|b, off); }
void emit_s8i(int s, int b, int off) { emit3((s<<4)|2, (4<<4)|b, off); }
void emit_l16si(int d, int b, int off) { emit3((d<<4)|2, (9<<4)|b, off/2); }
void emit_s16i(int s, int b, int off) { emit3((s<<4)|2, (5<<4)|b, off/2); }
void emit_l32r(int r) { emit3((r<<4)|1, 0, 0); }
void emit_add_n(int d, int s1, int s2) { emit2((s2<<4)|0xa, (d<<4)|s1); }
void emit_mov_n(int d, int s) { emit2((d<<4)|0xd, s); }
void emit_movi_n(int d, int imm) { 
    int i=(((imm&0xf)<<12)|(d<<8)|(((imm&0x70)>>4)<<4)|0xc); 
    emit2(i&0xff, i>>8); 
}
void emit_load_lit(int val, int type);
void emit_movi(int d, int imm) {
    if (imm >= -2048 && imm < 2048) emit3((d<<4)|2, 0xa0|((imm>>8)&0xf), imm&0xff);
    else { emit_load_lit(imm, L_INT); if (d != RES_REG) emit_mov_n(d, RES_REG); }
}
void emit_addi(int d, int s, int imm) {
    if (imm >= -128 && imm < 128) emit3((d<<4)|2, (0xc<<4)|s, imm&0xff);
    else { emit_movi(TMP_REG, imm); emit_add_n(d, s, TMP_REG); }
}
void emit_op(int op, int d, int s1, int s2) { emit_rrr(op, 0, d, s1, s2); }
void emit_neg(int d, int s1) { emit_op(0x60, d, 0, s1); }
void emit_xor(int d, int s1, int s2) { emit_op(0x30, d, s1, s2); }
void emit_br(int op, int s, int t) { emit3((t<<4)|7, (op<<4)|s, 1); }
void emit_j(int off) { int i=0x06|((off&0x3ffff)<<6); emit3(i, i>>8, i>>16); }
void emit_beqz(int s, int off) { int i=((off&0xfff)<<12)|(s<<8)|0x16; emit3(i, i>>8, i>>16); }
void emit_callx8() { emit3(0xe0, 0x08, 0x00); }
void emit_retw_n() { emit2(0x1d, 0xf0); }
void emit_entry(int sz) { int imm12 = sz/8; emit3(0x36, (imm12<<4)|1, imm12>>4); }

void patch(int addr, int is_j) {
    int off = code_size - addr - 4;
    int i = 0; memcpy(&i, code_data + addr, 3);
    if (is_j) i = i | ((off & 0x3ffff) << 6);
    else      i = i | ((off & 0xfff)   << 12);
    memcpy(code_data + addr, &i, 3);
}

// --- Literals/symbols handling ---
int add_name(char *s) {
    int len = strlen(s) + 1;
    if (name_sz + len > name_cap) { xcc_fail("Out of memory (names)"); }
    int off = name_sz; strcpy(name_buf + off, s);
    name_sz = name_sz + len; return off;
}
int get_func(char *name) {
    int i=0;
    while (i<n_funcs) { if(!strcmp(name_buf + func_name_off[i], name)) return i; ++i; }
    func_name_off[n_funcs] = add_name(name);
    func_addrs[n_funcs] = -1;
    ++n_funcs;
    return n_funcs - 1;
}

void emit_load_lit(int val, int type) {
    int i=0;
    while(i<n_lits && (lit_vals[i] != val || lit_types[i] != type)) ++i;
    if (i == n_lits) {
        if (n_lits >= MAX_LITS) { xcc_fail("Error: Too many literals"); }
        lit_vals[n_lits] = val; lit_types[n_lits] = type; ++n_lits;
    }
    if (n_patches >= MAX_PATCHES) { xcc_fail("Error: Too many patches"); }
    patch_offs[n_patches] = code_size; patch_lits[n_patches] = i; ++n_patches;
    emit_l32r(RES_REG);
}

void push(int r) { emit_s32i(r, SP_REG, locals + esp); esp = esp + 4; }
void pop(int r) { esp = esp - 4; emit_l32i(r, SP_REG, locals + esp); }

// --- Parser ---
int get_prec(int t) {
    if(t=='?') return 1; if(t==T_LOR) return 2; if(t==T_LAND) return 3;
    if(t=='|') return 4; if(t=='^') return 5; if(t=='&') return 6;
    if(t==T_EQ||t==T_NE) return 7;
    if(t=='<'||t=='>'||t==T_LE||t==T_GE) return 8;
    if(t==T_SHL||t==T_SHR) return 9;
    if(t=='+'||t=='-') return 10; if(t=='*'||t=='/'||t=='%') return 11;
    return 0;
}

void emit_binop(int op) {
    if (op == T_LAND || op == T_LOR) {
        emit_movi_n(ARG1_REG, 0); emit_beqz(TMP_REG, 1); emit_movi_n(ARG1_REG, 1);
        emit_movi_n(ARG2_REG, 0); emit_beqz(RES_REG, 1); emit_movi_n(ARG2_REG, 1);
        emit_op(op==T_LAND ? 0x10 : 0x20, RES_REG, ARG2_REG, ARG1_REG);
    } else if (get_prec(op) >= 7 && get_prec(op) <= 8) {
        emit_mov_n(ARG1_REG, RES_REG); emit_movi_n(RES_REG, 0);
        if(op=='<') emit_br(0xa, TMP_REG, ARG1_REG); else if(op==T_LE) emit_br(0x2, ARG1_REG, TMP_REG);
        else if(op=='>') emit_br(0xa, ARG1_REG, TMP_REG); else if(op==T_GE) emit_br(0x2, TMP_REG, ARG1_REG);
        else if(op==T_EQ) emit_br(0x9, TMP_REG, ARG1_REG); else if(op==T_NE) emit_br(0x1, TMP_REG, ARG1_REG);
        emit_movi_n(RES_REG, 1);
    } else if(op=='+') emit_add_n(RES_REG, TMP_REG, RES_REG);
    else if(op=='-') emit_op(0xc0, RES_REG, TMP_REG, RES_REG);
    else if(op=='*') emit_op(0x82, RES_REG, TMP_REG, RES_REG);
    else if(op=='/') emit_op(0xd2, RES_REG, TMP_REG, RES_REG);
    else if(op=='%') emit_op(0xf2, RES_REG, TMP_REG, RES_REG);
    else if(op=='&') emit_op(0x10, RES_REG, TMP_REG, RES_REG);
    else if(op=='|') emit_op(0x20, RES_REG, TMP_REG, RES_REG);
    else if(op=='^') emit_op(0x30, RES_REG, TMP_REG, RES_REG);
    else if(op==T_SHL) { emit3(0,0x10|RES_REG,0x40); emit3(0,(RES_REG<<4)|TMP_REG,0xA1); }
    else if(op==T_SHR) { emit3(0,RES_REG,0x40); emit3(TMP_REG<<4,RES_REG<<4,0xB1); }
    expr_type = TY_INT;
}

void parse_expr(int limit);

void parse_call(char *name) {
    int arg_cnt = 0; next();
    if (token != ')') {
        parse_expr(1); push(RES_REG); ++arg_cnt;
        if (arg_cnt > 5) { xcc_fail("Error: L%d: Arg count exceeds the supported maximum of 5", line); }
        while (token == ',') { next(); parse_expr(1); push(RES_REG); ++arg_cnt; }
        while (arg_cnt > 0) { --arg_cnt; pop(arg_cnt <= 4 ? ARG1_REG + arg_cnt : TMP_REG); }
    }
    expect(')'); emit_load_lit(get_func(name), L_FUNC); emit_callx8();
}

int find_var(char *name) {
    int i = n_vars;
    while (i > 0) { --i; if (!strcmp(name_buf + var_name_off[i], name)) return i; }
    return -1;
}
void load_var_address(int i) {
    if (var_types[i] & TF_GLOBAL) emit_load_lit(var_offsets[i], L_BSS);
    else emit_addi(RES_REG, SP_REG, var_offsets[i]);
}
void load_var(int i) {
    int ty = var_types[i]; int is_byte = ((ty & ~TF_GLOBAL) == TY_BYTE);
    if (ty & TF_CONST) {
        emit_movi(RES_REG, var_offsets[i]); expr_type = TY_INT;
    } else if (ty & TF_ARR) {
        load_var_address(i);
        expr_type = (ty & TF_BYTE) ? TY_BYTEPTR : (ty & TF_SHORT) ? TY_SHORTPTR : TY_INTPTR;
    } else if (ty & TF_GLOBAL) {
        load_var_address(i);
        if (is_byte) emit_l8ui(RES_REG, RES_REG, 0); else emit_l32i(RES_REG, RES_REG, 0);
        expr_type = ty & ~TF_GLOBAL;
    } else {
        if (is_byte) emit_l8ui(RES_REG, SP_REG, var_offsets[i]);
        else emit_l32i(RES_REG, SP_REG, var_offsets[i]);
        expr_type = ty & ~TF_GLOBAL;
    }
}

void parse_index(int base_type) {
    next(); push(RES_REG);
    parse_expr(1); expect(']');
    if (base_type & TF_SHORT) { emit_add_n(RES_REG, RES_REG, RES_REG); }
    else if (!(base_type & TF_BYTE)) { emit_add_n(RES_REG, RES_REG, RES_REG); emit_add_n(RES_REG, RES_REG, RES_REG); }
    pop(TMP_REG); emit_add_n(RES_REG, TMP_REG, RES_REG);
}

// Element load/store by pointee flags (byte / short / int).
void emit_elem_load(int type) {
    if (type & TF_BYTE) emit_l8ui(RES_REG, RES_REG, 0);
    else if (type & TF_SHORT) emit_l16si(RES_REG, RES_REG, 0);
    else emit_l32i(RES_REG, RES_REG, 0);
}
void emit_elem_store(int type, int s, int b) {
    if (type & TF_BYTE) emit_s8i(s, b, 0);
    else if (type & TF_SHORT) emit_s16i(s, b, 0);
    else emit_s32i(s, b, 0);
}

void parse_factor() {
    if (token == T_INC || token == T_DEC) {
        int diff = (token == T_INC) ? 1 : -1; next();
        char name[64]; strcpy(name, str_val); expect(T_IDENT);
        int i = find_var(name);
        if (i < 0) { xcc_fail("Undef: %s", name); }
        load_var(i); emit_addi(RES_REG, RES_REG, diff);
        int is_byte = ((var_types[i] & ~TF_GLOBAL) == TY_BYTE);
        if (var_types[i] & TF_GLOBAL) {
            emit_mov_n(ARG1_REG, RES_REG); load_var_address(i);
            if (is_byte) emit_s8i(ARG1_REG, RES_REG, 0); else emit_s32i(ARG1_REG, RES_REG, 0);
            emit_mov_n(RES_REG, ARG1_REG);
        } else {
            if (is_byte) emit_s8i(RES_REG, SP_REG, var_offsets[i]);
            else emit_s32i(RES_REG, SP_REG, var_offsets[i]);
        }
        expr_type = TY_INT;
    } else if (token == '!' || token == '~' || token == '-') {
        int op = token; next(); parse_factor();
        if (op == '-') emit_neg(RES_REG, RES_REG);
        else if (op == '~') { emit_movi(TMP_REG, -1); emit_xor(RES_REG, RES_REG, TMP_REG); }
        else { emit_movi_n(TMP_REG, 1); emit_beqz(RES_REG, 1); emit_movi_n(TMP_REG, 0); emit_mov_n(RES_REG, TMP_REG); }
        expr_type = TY_INT;
    } else if (token == '*') {
        next(); parse_factor(); int pt = expr_type;
        emit_elem_load(pt);
        expr_type = (pt & TF_BYTE) ? TY_BYTE : TY_INT;
    } else if (token == '&') {
        next(); char name[64]; strcpy(name, str_val); expect(T_IDENT);
        int i = find_var(name);
        if (i < 0) { xcc_fail("Undef: %s", name); }
        load_var_address(i);
        expr_type = (var_types[i] & TF_BYTE) ? TY_BYTEPTR : TY_INTPTR;
    } else if (token == T_NUM) { 
        emit_movi(RES_REG, num_val); expr_type = TY_INT; next();
    } else if (token == T_STR) {
        if (rodata_sz + str_len + 1 > rodata_cap) { xcc_fail("Out of memory (rodata)"); }
        emit_load_lit(rodata_sz, L_STR);
        memcpy(rodata + rodata_sz, str_val, str_len + 1);
        rodata_sz = rodata_sz + str_len + 1;
        expr_type = TY_BYTEPTR; next();
    } else if (token == T_IDENT) {
        char name[64]; strcpy(name, str_val); next();
        if (token == '(') {
            parse_call(name); emit_mov_n(RES_REG, ARG1_REG); expr_type = TY_INT;
        } else {
            int i = find_var(name);
            if (i < 0) { xcc_fail("Undef: %s", name); }
            load_var(i);
            if (token == '[') {
                int bt = expr_type;
                parse_index(bt);
                emit_elem_load(bt);
                expr_type = (bt & TF_BYTE) ? TY_BYTE : TY_INT;
            }
        }
    } else if (token == '(') {
        next();
        if (token == T_INT || token == T_CHAR || token == T_SHORT || token == T_VOID) {
            // tulip fork: casts accepted and ignored (values are 32-bit registers;
            // stores truncate by element size anyway).
            next(); while (token == '*') next();
            expect(')');
            parse_factor();
        } else { parse_expr(1); expect(')'); }
    }
    else { xcc_fail("Error: Line %d: unexpected token %d", line, token); }
}

void parse_expr(int limit) {
    parse_factor();
    while (get_prec(token) >= limit) {
        int op = token; next();
        if (op == '?') {
            int patch_to_false = code_size; emit_beqz(RES_REG, 0);
            parse_expr(2);
            int patch_to_end = code_size; emit_j(0);
            expect(':'); patch(patch_to_false, 0);
            parse_expr(1);
            patch(patch_to_end, 1);
        } else {
            push(RES_REG); parse_expr(get_prec(op) + 1);
            pop(TMP_REG); emit_binop(op);
        }
    }
}

int align4(int x) { return (x + 3) & ~3; }

void parse_stmt() {
    esp = 0;
    if (token == T_WHILE) {
        next(); int loop_start = code_size;
        expect('('); parse_expr(1); expect(')');
        int exit_patch = code_size; emit_beqz(RES_REG, 0);
        parse_stmt();
        emit_j(loop_start - code_size - 4);
        patch(exit_patch, 0);
    } else if (token == T_IF) {
        next(); expect('('); parse_expr(1); expect(')');
        int p1 = code_size; emit_beqz(RES_REG, 0);
        parse_stmt();
        if (token == T_ELSE) {
            int p2 = code_size; emit_j(0);
            patch(p1, 0); next(); parse_stmt(); patch(p2, 1);
        } else patch(p1, 0);
    } else if (token == '{') {
        next(); while (token != '}' && token != T_EOF) parse_stmt(); expect('}');
    } else if (token == T_INT || token == T_CHAR || token == T_SHORT) {
        int is_byte = (token == T_CHAR); int is_short = (token == T_SHORT); next();
        int is_ptr = 0; while (token == '*') { is_ptr = 1; next(); }
        var_name_off[n_vars] = add_name(str_val);
        var_offsets[n_vars] = locals;
        if (++n_vars >= MAX_VARS) { xcc_fail("MAX_VARS exceeded"); }
        expect(T_IDENT);
        if (token == '[') {
            next(); int sz = num_val; expect(T_NUM); expect(']');
            var_types[n_vars-1] = is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR;
            locals = locals + (is_byte ? align4(sz) : is_short ? align4(sz * 2) : sz * 4);
        } else {
            var_types[n_vars-1] = is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT);
            locals = locals + 4;
            expect('='); parse_expr(1);
            if (is_byte && !is_ptr) emit_s8i(RES_REG, SP_REG, var_offsets[n_vars-1]);
            else emit_s32i(RES_REG, SP_REG, var_offsets[n_vars-1]);
        }
        if (locals >= MAX_LOCAL_VARS * 4) {
            xcc_fail("Error: Line %d: Function stack frame exceeded MAX_LOCAL_VARS", line);
        }
        expect(';');
    } else if (token == T_STATIC) {
        // tulip fork: static locals -> function-scoped .bss (zero-initialized,
        // persists across calls -- how user DSP keeps state). Optional "= 0"
        // initializer accepted; anything else is an error (no .data section).
        next();
        int is_byte = (token == T_CHAR); int is_short = (token == T_SHORT);
        if (token != T_INT && token != T_CHAR && token != T_SHORT) {
            xcc_fail("Error: Line %d: static must be int/char/int16_t", line);
        }
        next();
        int is_ptr = 0; while (token == '*') { is_ptr = 1; next(); }
        var_name_off[n_vars] = add_name(str_val);
        expect(T_IDENT);
        int ty = TF_GLOBAL | (is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT));
        if (token == '[') {
            next(); int sz = 0;
            if (token == T_NUM) { sz = num_val; next(); }
            else {
                int ci = find_var(str_val);
                if (ci < 0 || !(var_types[ci] & TF_CONST)) { xcc_fail("Error: Line %d: array size expected", line); }
                sz = var_offsets[ci]; next();
            }
            expect(']');
            ty = TF_GLOBAL | (is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR);
            var_offsets[n_vars] = bss_size;
            bss_size = bss_size + (is_byte ? align4(sz) : is_short ? align4(sz * 2) : sz * 4);
        } else {
            var_offsets[n_vars] = bss_size; bss_size = bss_size + 4;
            if (token == '=') {
                next();
                if (token != T_NUM || num_val != 0) { xcc_fail("Error: Line %d: static initializers must be 0 (.bss only)", line); }
                next();
            }
        }
        var_types[n_vars] = ty;
        // Registered past n_globals: visible in this function only (C static-local
        // scoping); the next parse_func resets n_vars back to n_globals.
        if (++n_vars >= MAX_VARS) { xcc_fail("MAX_VARS exceeded"); }
        expect(';');
    } else if (token == T_RETURN) {
        next(); if (token != ';') parse_expr(1); else emit_movi_n(RES_REG, 0);
        emit_mov_n(2, RES_REG); emit_retw_n(); expect(';');
    } else if (token == T_IDENT) {
        char name[64]; strcpy(name, str_val); next();
        if (token == '(') {
            parse_call(name); expect(';'); 
        } else {
            int i = find_var(name);
            if (i < 0) { xcc_fail("Undef: %s", name); }
            if (token == '[') {
                load_var(i);
                int bt = expr_type;
                parse_index(bt); push(RES_REG);
                expect('='); parse_expr(1);
                pop(ARG1_REG);
                emit_elem_store(bt, RES_REG, ARG1_REG);
            } else {
                expect('='); parse_expr(1);
                if (var_types[i] & TF_GLOBAL) {
                    emit_mov_n(ARG1_REG, RES_REG); 
                    emit_load_lit(var_offsets[i], L_BSS);
                    emit_s32i(ARG1_REG, RES_REG, 0);
                    emit_mov_n(RES_REG, ARG1_REG);
                } else emit_s32i(RES_REG, SP_REG, var_offsets[i]);
            }
            expect(';');
        }
    } else if (token == '*') {
        next(); parse_factor();
        int pt = expr_type;
        push(RES_REG); expect('='); parse_expr(1);
        pop(ARG1_REG);
        emit_elem_store(pt, RES_REG, ARG1_REG);
        expect(';');
    } else { parse_expr(1); expect(';'); }
}

void parse_func() {
    if (token == T_ENUM) {
        next(); if (token == T_IDENT) next();
        expect('{'); int val = 0;
        while (token == T_IDENT) {
            var_name_off[n_globals] = add_name(str_val);
            var_offsets[n_globals] = val;
            var_types[n_globals] = TF_CONST | TY_INT;
            ++n_globals; n_vars = n_globals; next();
            if (token == '=') { next(); val = num_val; var_offsets[n_globals - 1] = val; next(); }
            ++val; if (token == ',') next();
        }
        expect('}'); expect(';'); return;
    }

    int is_byte = (token == T_CHAR); int is_short = (token == T_SHORT);
    if (token == T_INT || token == T_CHAR || token == T_VOID || token == T_SHORT) next();
    int is_ptr = 0; while(token=='*') { is_ptr = 1; next(); }
    char name[64]; strcpy(name, str_val); expect(T_IDENT);

    if (token == ';' || token == '[') {
        int ty = TF_GLOBAL | (is_ptr ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT));
        if (token == '[') {
            next(); int sz=0;
            if (token == T_NUM) { sz = num_val; next(); }
            else if (token == T_IDENT) {
                int i = find_var(str_val);
                if (i < 0 || !(var_types[i] & TF_CONST)) {
                    xcc_fail("Error: Line %d: Undefined constant: %s", line, str_val);
                }
                sz = var_offsets[i]; next();
            } else { xcc_fail("Error: Line %d: Array size expected", line); }
            expect(']');
            ty = TF_GLOBAL | (is_byte ? TY_BYTEARR : is_short ? TY_SHORTARR : TY_INTARR);
            var_offsets[n_globals] = bss_size;
            bss_size = bss_size + ((ty & TF_BYTE) ? align4(sz) : (ty & TF_SHORT) ? align4(sz * 2) : sz * 4);
        } else {
            var_offsets[n_globals] = bss_size; bss_size = bss_size + 4;
        }
        var_name_off[n_globals] = add_name(name);
        var_types[n_globals] = ty;
        ++n_globals; n_vars = n_globals;
        expect(';'); return;
    }

    expect('('); n_vars = n_globals; locals = 32; int n_args = 0; // Reserve 32 bytes for base save area
    while (token != ')') {
        is_byte = 0; is_short = 0; int ptr_count = 0;
        if (token == T_CHAR) { is_byte = 1; next(); }
        else if (token == T_SHORT) { is_short = 1; next(); }
        else if (token == T_INT || token == T_VOID || token == T_IDENT || token == T_ELLIPSIS) next();
        while (token == '*') { ++ptr_count; next(); }
        if (token == T_IDENT) {
            var_name_off[n_vars] = add_name(str_val);
            var_offsets[n_vars] = locals;
            var_types[n_vars] = ptr_count >= 2 ? TY_INTPTR : ptr_count ? (is_byte ? TY_BYTEPTR : is_short ? TY_SHORTPTR : TY_INTPTR) : (is_byte ? TY_BYTE : TY_INT);
            locals = locals + 4; ++n_vars; ++n_args;
            if (n_vars >= MAX_VARS) { xcc_fail("MAX_VARS exceeded"); }
            next();
        }
        if (token == ',') next();
    }
    expect(')');
    if (token == ';') { next(); return; } // Prototype

    int i = get_func(name); int entry_addr = code_size; func_addrs[i] = entry_addr;
    expect('{');
    int stack_sz = (MAX_LOCAL_VARS*4 + 32 + 15) & ~15;
    emit_entry(stack_sz);
    int j=0; while(j<n_args) { emit_s32i(2+j, SP_REG, var_offsets[n_globals + j]); ++j; }
    while(token!='}' && token!=T_EOF) parse_stmt();
    int actual_stack_sz_imm12 = ((locals + 32 + 15) & ~15) / 8;
    code_data[entry_addr + 1] = (actual_stack_sz_imm12 << 4) | 1;
    code_data[entry_addr + 2] = actual_stack_sz_imm12 >> 4;
    emit_retw_n(); expect('}');
}

uint8_t *write_elf_mem(const char *entry_name, uint32_t *out_size) {
    int code_start = n_lits * 4;
    int off = 52; // Ehdr
    int text_off = off; int text_addr = off;
    off = off + align4(code_start + code_size);
    int rodata_off = off; int rodata_addr = off;
    off = off + align4(rodata_sz);
    int bss_off = off; int bss_addr = off;

    int n_syms = n_funcs + 1;
    char *syms = calloc(n_funcs+1, 16);
    char *strtab = calloc(1, STRTAB); int str_off = 1;
    char *lits = calloc(1, code_start); 
    char *rels = calloc(n_lits, 12);
    char *shdr=calloc(1, 320); // 8 sections * 40 bytes

    int fname_len = 0;
    int i = 0; while (i < n_funcs) {
        put32(syms + (i+1)*16, str_off);
        fname_len = strlen(name_buf + func_name_off[i]);
        if (str_off + fname_len >= STRTAB) { xcc_fail("strtab overflow on func %d len=%d name=%s", i, fname_len, name_buf + func_name_off[i]); }
        strcpy(strtab + str_off, name_buf + func_name_off[i]); str_off = str_off + fname_len+1;
        int is_ext = (func_addrs[i] == -1);
        syms[(i+1)*16 + 12] = (1<<4)|(is_ext ? 0 : 2); // ST_INFO
        put16(syms + (i+1)*16 + 14, is_ext ? 0 : 1);   // shndx
        if(!is_ext) put32(syms + (i+1)*16 + 4, code_start + func_addrs[i]);
        ++i;
    }

    int n_rels = 0;
    i=0; while(i<n_lits) {
        int val = 0; int r_offset = text_addr + i*4;
        if (lit_types[i] == L_INT) val = lit_vals[i];
        else if (lit_types[i] == L_STR) {
            val = rodata_addr + lit_vals[i];
            put32(rels + n_rels*12, r_offset); 
            put32(rels + n_rels*12 + 4, R_XTENSA_RELATIVE); 
            ++n_rels;
        } else if (lit_types[i] == L_FUNC) {
            int fidx = lit_vals[i];
            if (func_addrs[fidx] == -1) { 
                put32(rels + n_rels*12, r_offset); 
                put32(rels + n_rels*12 + 4, ((fidx+1)<<8)|R_XTENSA_JMP_SLOT); 
                ++n_rels;
            } else { 
                val = text_addr + code_start + func_addrs[fidx];
                put32(rels + n_rels*12, r_offset); 
                put32(rels + n_rels*12 + 4, R_XTENSA_RELATIVE); 
                ++n_rels;
            }
        } else if (lit_types[i] == L_BSS) {
            val = bss_addr + lit_vals[i];
            put32(rels + n_rels*12, r_offset);
            put32(rels + n_rels*12 + 4, R_XTENSA_RELATIVE);
            ++n_rels;
        }
        memcpy(lits + i*4, &val, 4); ++i;
    }

    i = 0; while (i < n_patches) {
        int target = text_addr + patch_lits[i] * 4;
        int pc = text_addr + code_start + patch_offs[i];
        int imm = (target - ((pc+3)&~3)) >> 2;
        code_data[patch_offs[i] + 1] = imm;
        code_data[patch_offs[i] + 2] = imm>>8;
        ++i;
    }

    // Section Headers
    int rela_off=off; off = off + align4(n_rels * 12);
    int symtab_off=off; off = off + n_syms * 16;
    int strtab_off=off; off = off + str_off;
    char *shstrtab = "\0.text\0.rodata\0.bss\0.rela\0.symtab\0.strtab\0.shstrtab\0";
    int shstrtab_off = off; off = off + 53; // sizeof(shstrtab) 
    
    // 1: .text
    put32(shdr+40, 1); put32(shdr+44, 1); put32(shdr+48, 6); put32(shdr+52, text_addr); 
    put32(shdr+56, text_off); put32(shdr+60, code_start+code_size); put32(shdr+72, 4);
    // 2: .rodata
    put32(shdr+80, 7); put32(shdr+84, 1); put32(shdr+88, 2); put32(shdr+92, rodata_addr);
    put32(shdr+96, rodata_off); put32(shdr+100, align4(rodata_sz)); put32(shdr+112, 4);
    // 3: .bss
    put32(shdr+120, 15); put32(shdr+124, 8); put32(shdr+128, 3); put32(shdr+132, bss_addr);
    put32(shdr+136, bss_off); put32(shdr+140, bss_size); put32(shdr+152, 4);
    // 4: .rela
    put32(shdr+160, 20); put32(shdr+164, 4); put32(shdr+168, 2); put32(shdr+176, rela_off);
    put32(shdr+180, n_rels*12); put32(shdr+184, 5); put32(shdr+188, 1); put32(shdr+192, 4); put32(shdr+196, 12);
    // 5: .symtab
    put32(shdr+200, 26); put32(shdr+204, 2); put32(shdr+216, symtab_off);
    put32(shdr+220, n_syms*16); put32(shdr+224, 6); put32(shdr+228, 1); put32(shdr+232, 4); put32(shdr+236, 16);
    // 6: .strtab
    put32(shdr+240, 34); put32(shdr+244, 3); put32(shdr+256, strtab_off);
    put32(shdr+260, str_off); put32(shdr+272, 1);
    // 7: .shstrtab
    put32(shdr+280, 42); put32(shdr+284, 3); put32(shdr+296, shstrtab_off);
    put32(shdr+300, 53); put32(shdr+312, 1); // 53 == sizeof(shstrtab)

    // tulip fork: e_entry is the caller-requested function ("process"/"render"),
    // so after esp_elf_relocate() the loader's entry pointer IS the user fn.
    int entry_func = get_func(entry_name);
    if (func_addrs[entry_func] == -1) xcc_fail("no %s() defined", entry_name);
    int entry_vaddr = text_addr + code_start + func_addrs[entry_func];

    char ehdr[52]; memset(ehdr, 0, 52);
    ehdr[0]=0x7f; ehdr[1]='E'; ehdr[2]='L'; ehdr[3]='F'; ehdr[4]=1; ehdr[5]=1; ehdr[6]=1;
    put16(ehdr+16, 1); put16(ehdr+18, 94); // e_type=ET_REL, e_machine=XTENSA
    put32(ehdr+20, 1); put32(ehdr+24, entry_vaddr);
    put32(ehdr+32, align4(off)); put32(ehdr+36, 0x300); put16(ehdr+40, 52);
    put16(ehdr+42, 0); put16(ehdr+44, 0); put16(ehdr+46, 40); put16(ehdr+48, 8); put16(ehdr+50, 7);

    // tulip fork: assemble the ELF in memory instead of writing a file.
    uint32_t total = align4(off) + 320;
    uint8_t *img = calloc(1, total);
    if (img == NULL) xcc_fail("out of memory (elf image)");
    memcpy(img, ehdr, 52);
    memcpy(img + 52, lits, code_start);
    memcpy(img + 52 + code_start, code_data, code_size);
    memcpy(img + rodata_off, rodata, rodata_sz);
    memcpy(img + rela_off, rels, n_rels * 12);
    memcpy(img + symtab_off, syms, n_syms * 16);
    memcpy(img + strtab_off, strtab, str_off);
    memcpy(img + shstrtab_off, shstrtab, 53); // 53 == sizeof(shstrtab)
    memcpy(img + align4(off), shdr, 320);

    free(lits); free(rels); free(syms); free(strtab); free(shdr);
    *out_size = total;
    return img;
}
#ifdef XCC700_CLI
// Host-side CLI for testing: xcc700t <in.c> -o <out.elf> [entry]
int main(int argc, char **argv) {
    const char *in = argc > 1 ? argv[1] : "input.c";
    const char *out = (argc > 3 && !strcmp(argv[2], "-o")) ? argv[3] : "output.elf";
    const char *entry = argc > 4 ? argv[4] : "process";
    FILE *f = fopen(in, "rb");
    if (f == NULL) { fprintf(stderr, "cannot open %s\n", in); return 1; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    if (fread(buf, 1, sz, f) != (size_t)sz) { fprintf(stderr, "read failed\n"); return 1; }
    buf[sz] = 0; fclose(f);
    uint8_t *elf; uint32_t elf_sz; char err[512];
    if (xcc700_compile(buf, entry, &elf, &elf_sz, err, sizeof(err)) != 0) {
        fprintf(stderr, "compile failed: %s\n", err); return 1;
    }
    FILE *o = fopen(out, "wb");
    fwrite(elf, 1, elf_sz, o); fclose(o);
    fprintf(stderr, "%s: %u bytes, entry %s\n", out, (unsigned)elf_sz, entry);
    return 0;
}
#endif
