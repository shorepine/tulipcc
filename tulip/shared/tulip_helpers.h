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

// Python callables (and their args) stored from C and invoked later via
// mp_sched_schedule. They live in MicroPython root-pointer state -- see the
// MP_REGISTER_ROOT_POINTER block in modtulip.c. A callable whose only
// reference is a plain C global is invisible to the GC; it gets collected
// and calling it later crashes (e.g. ctrl-Q after the bound method backing
// ui_quit_callback was swept).
#define midi_callback MP_STATE_VM(cb_midi)
#define amy_overload_callback MP_STATE_VM(cb_amy_overload)
#define amy_block_done_callback MP_STATE_VM(cb_amy_block_done)
#define frame_callback MP_STATE_VM(cb_frame)
#define frame_arg MP_STATE_VM(cb_frame_arg)
#define touch_callback MP_STATE_VM(cb_touch)
#define keyboard_callback MP_STATE_VM(cb_keyboard)
#define ui_quit_callback MP_STATE_VM(cb_ui_quit)
#define ui_switch_callback MP_STATE_VM(cb_ui_switch)
#define sequencer_callbacks MP_STATE_VM(cb_sequencer)
#define defer_callbacks MP_STATE_VM(cb_defer)
#define defer_args MP_STATE_VM(cb_defer_args)

uint8_t file_exists(const char *filename);
int32_t file_size(const char *filename);
uint32_t read_file(const char *filename, uint8_t *buf, int32_t len, uint8_t binary);
uint32_t write_file(const char *filename, uint8_t *buf, uint32_t len, uint8_t binary);
int check_rx_char();
void tx_char(int c);
mp_obj_t tulip_fopen(const char *filename, const char *mode);
uint32_t tulip_fwrite(mp_obj_t file, uint8_t * buf, uint32_t len);
uint32_t tulip_fread(mp_obj_t file, uint8_t * buf, uint32_t len);
void tulip_fclose(mp_obj_t file);
uint32_t tulip_fseek(mp_obj_t file, uint32_t seekpoint);
int32_t tulip_getline(char * line, uint32_t * len, mp_obj_t file );
uintptr_t get_stack_pointer();
#endif
