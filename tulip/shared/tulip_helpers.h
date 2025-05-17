// tulip_helpers.h
#ifndef TULIPHELPERS_H
#define TULIPHELPERS_H

#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "extmod/vfs.h"
#include "py/stream.h"
#include "py/ringbuf.h"
#include <string.h>
#include <stdio.h>

extern ringbuf_t stdin_ringbuf;

uint8_t file_exists(const char *filename);
int32_t file_size(const char *filename);
uint32_t read_file(const char *filename, uint8_t *buf, int32_t len, uint8_t binary);
uint32_t write_file(const char *filename, uint8_t *buf, uint32_t len, uint8_t binary);
int check_rx_char();
void tx_char(int c);
mp_obj_t tulip_fopen(const char *filename, const char *mode);
uint32_t tulip_fwrite(mp_obj_t file, uint8_t * buf, uint32_t len);
void tulip_fclose(mp_obj_t file);
uint32_t tulip_fseek(mp_obj_t file, uint32_t seekpoint, int32_t whence);
int32_t tulip_getline(char * line, uint32_t * len, mp_obj_t file );
uint64_t get_stack_pointer();
#endif
