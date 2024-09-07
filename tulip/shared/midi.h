// midi.h
#include "alles.h"
#ifdef ESP_PLATFORM 
#include "soc/uart_reg.h"
extern QueueHandle_t uart_queue;
#else
// virtualmidi Cocoa stubs
#endif
#define MIDI_SLOTS 4
#include "py/mphal.h"
#include "py/runtime.h"
void convert_midi_bytes_to_messages(uint8_t * data, size_t len, uint8_t usb);
extern mp_obj_t midi_callback;


//void tulip_midi_isr();
#define MAX_MIDI_BYTES_TO_PARSE 1024
#define MAX_MIDI_BYTES_PER_MESSAGE 3
#define MIDI_QUEUE_DEPTH 1024
extern uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
extern uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
extern int16_t midi_queue_tail;
extern int16_t midi_queue_head;

void midi_out(uint8_t * bytes, uint16_t len);
void midi_local(uint8_t * bytes, uint16_t len);

#ifdef ESP_PLATFORM
void run_midi();
#else
void *run_midi(void*vargp);
#endif
