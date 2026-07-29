//amy_connector.h
#ifndef AMYCONNECTOR_H
#define AMYCONNECTOR_H

#include "py/obj.h"


#define MAX_MIDI_BYTES_TO_PARSE 1024
#define MAX_MIDI_BYTES_PER_MESSAGE 3
#define MIDI_QUEUE_DEPTH 1024
#define MAX_SYSEX_BYTES (16384)
extern uint8_t * sysex_buffer;

// amy.message() in C -- builds a wire string from kwargs, or returns None
// to send the caller back to the Python original. Defined in amy_connector.c,
// exported as tulip.amy_message by modtulip.c.
MP_DECLARE_CONST_FUN_OBJ_KW(tulip_amy_message_obj);
#ifdef __EMSCRIPTEN__
void midi_out(uint8_t * bytes, uint16_t len) ;

void midi_local(uint8_t * bytes, uint16_t len);
#endif

#endif