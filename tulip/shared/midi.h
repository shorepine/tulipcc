// midi.h
#include "alles.h"

#ifdef ESP_PLATFORM 
extern QueueHandle_t uart_queue;
#else
// virtualmidi Cocoa stubs
#endif

void callback_midi_message_received(uint8_t *data, size_t len);

void tulip_midi_isr();
#define MAX_MIDI_BYTES_PER_MESSAGE 18
#define MIDI_QUEUE_DEPTH 8
extern uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
extern uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
extern int16_t midi_queue_tail;
extern int16_t midi_queue_head;

static inline void push_midi_message_into_fifo(uint8_t *data, int len) {
    for(uint32_t i = 0; i < (uint32_t)len; i++) {
        if(i < MAX_MIDI_BYTES_PER_MESSAGE) {
            last_midi[midi_queue_tail][i] = data[i];
        }
    }
    last_midi_len[midi_queue_tail] = (uint16_t)len;
    midi_queue_tail = (midi_queue_tail + 1) % MIDI_QUEUE_DEPTH;
    if (midi_queue_tail == midi_queue_head) {
        // Queue wrap, drop oldest item.
        midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
        fprintf(stderr, "dropped midi message\n");
    }
}

void midi_out(uint8_t * bytes, uint16_t len);
#ifdef ESP_PLATFORM
void run_midi();
#else
void *run_midi(void*vargp);
#endif
