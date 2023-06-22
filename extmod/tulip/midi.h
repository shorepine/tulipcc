// midi.h
#include "alles.h"
#ifdef ESP_PLATFORM 
extern QueueHandle_t uart_queue;
#else
// virtualmidi Cocoa stubs
#endif


void tulip_midi_isr();
#define MAX_MIDI_BYTES_PER_MESSAGE 18
extern uint8_t last_midi[MAX_MIDI_BYTES_PER_MESSAGE];
extern uint8_t last_midi_len;

void midi_out(uint8_t * bytes, uint16_t len);
#ifdef ESP_PLATFORM
void run_midi();
#else
void *run_midi(void*vargp);
#endif
