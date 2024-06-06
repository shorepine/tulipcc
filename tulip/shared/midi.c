// midi.c
#include "midi.h"
#include "polyfills.h"
uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;
extern mp_obj_t midi_callback;

#define DEBUG_MIDI 0


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


uint8_t current_midi_status = 0;
uint8_t midi_message[3];
uint8_t midi_message_i = 0;

void callback_midi_message_received(uint8_t *data, size_t len) {
    push_midi_message_into_fifo(data, len);
    if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_none);
    current_midi_status = 0;
    midi_message_i = 0;
}


void convert_midi_bytes_to_messages(uint8_t * data, size_t len) {
    // i take any amount of bytes and add messages to the fifo
    for(size_t i=0;i<len;i++) {
        uint8_t byte = data[i];
        if(byte & 0x80) { // status byte 
            if(current_midi_status != 0) {
                // this means we got a new status byte before sending the last message. i guess this is fine... 
            }
            current_midi_status = byte;
            midi_message_i = 1;
            midi_message[0] = byte;
            if(byte == 0xF6 || byte == 0xF8 || byte == 0xFA || byte == 0xFB || byte == 0xFC || byte == 0xFF) {
                callback_midi_message_received(midi_message, 1);                
            } else {
                // skip.. sysex... 
            }
        } else { // data byte 
            uint8_t status = current_midi_status & 0xF0;
            if(status == 0x80 || status == 0x90 || status == 0xA0 || status == 0xB0 || status == 0xE0) {
                midi_message[midi_message_i++] = byte;
                if(midi_message_i >= 3) { 
                    callback_midi_message_received(midi_message, 3);
                }
            } else if(status == 0xC0 || status == 0xD0) {
                midi_message[midi_message_i++] = byte;
                if(midi_message_i >= 2) { 
                    callback_midi_message_received(midi_message, 2);
                }
            } else {
                // fs or a 0 -- skip. the only F that has data we don't care about right now
                // the 0 would happen if dan passed a 3 byte message for a 2 byte message from the USB MIDI 
            }
        }
    }
}


void midi_local(uint8_t * bytes, uint16_t len) {
    convert_midi_bytes_to_messages(bytes, len);
}

#ifdef ESP_PLATFORM
QueueHandle_t uart_queue;

void midi_out(uint8_t * bytes, uint16_t len) {
    uart_write_bytes(UART_NUM_1, bytes, len);
}

void run_midi() {
    // Setup UART2 to listen for MIDI messages 
    const int uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate = 31250,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        //.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };


    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    // TX, RX, CTS/RTS -- Only care about RX here, pin 47 for now
    ESP_ERROR_CHECK(uart_set_pin(uart_num, MIDI_OUT_PIN, MIDI_IN_PIN, UART_PIN_NO_CHANGE , UART_PIN_NO_CHANGE ));

    const int uart_buffer_size = (1024);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, \
                                          0, 0, NULL, 0));

    uart_intr_config_t uart_intr = {
          .intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M
                              | UART_RXFIFO_TOUT_INT_ENA_M
                              | UART_FRM_ERR_INT_ENA_M
                              | UART_RXFIFO_OVF_INT_ENA_M
                              | UART_BRK_DET_INT_ENA_M
                              | UART_PARITY_ERR_INT_ENA_M,
          .rxfifo_full_thresh = 1,
          .rx_timeout_thresh = 0,
          .txfifo_empty_intr_thresh = 10
    };

    uart_intr_config(uart_num, &uart_intr);

    printf("UART MIDI running on core %d\n", xPortGetCoreID());

    uint8_t data[128];
    size_t length = 0;
    while(1) {
        length = uart_read_bytes(uart_num, data, MAX_MIDI_BYTES_PER_MESSAGE*MIDI_QUEUE_DEPTH, 1/portTICK_PERIOD_MS);
        if(length > 0) {
            //uart_flush(uart_num);
            convert_midi_bytes_to_messages(data,length);
        }
    } // end loop forever
}
#else
void midi_out(uint8_t * bytes, uint16_t len) {
    // nothing yet
}

#endif
