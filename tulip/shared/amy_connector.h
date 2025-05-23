//amy_connector.h
#ifndef AMYCONNECTOR_H
#define AMYCONNECTOR_H


#define MAX_MIDI_BYTES_TO_PARSE 1024
#define MAX_MIDI_BYTES_PER_MESSAGE 3
#define MIDI_QUEUE_DEPTH 1024
#define MAX_SYSEX_BYTES (16384)
extern uint8_t * sysex_buffer;
#ifdef __EMSCRIPTEN__
void midi_out(uint8_t * bytes, uint16_t len) ;

void midi_local(uint8_t * bytes, uint16_t len);
#endif

#endif