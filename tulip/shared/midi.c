// midi.c
#include "midi.h"
uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;


void callback_midi_message_received(uint8_t *data, size_t len) {
    //fprintf(stderr,"got midi message len %ld status %d -- ", (uint32_t)len, data[0]);
    for(uint32_t i=0;i<(uint32_t)len;i++) {
        if(i<MAX_MIDI_BYTES_PER_MESSAGE) {
            //fprintf(stderr, "%d ", data[i]);
            last_midi[midi_queue_tail][i] = data[i];
        }
    }
    //fprintf(stderr, " ## done\n");
    last_midi_len[midi_queue_tail] = (uint16_t)len;
    midi_queue_tail = (midi_queue_tail + 1) % MIDI_QUEUE_DEPTH;
    if (midi_queue_tail == midi_queue_head) {
        // Queue wrap, drop oldest item.
        midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
        fprintf(stderr, "dropped midi message\n");
    }
    tulip_midi_isr();
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
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 11, 47, 10, 9));

    const int uart_buffer_size = (512);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, \
                                          uart_buffer_size, 10, &uart_queue, 0));

    printf("UART MIDI running on core %d\n", xPortGetCoreID());

    uint8_t data[128];
    size_t length = 0;
    while(1) {
        // Sleep 5ms to wait to get more MIDI data and avoid starving audio thread
        // I increased RTOS clock rate from 100hz to 500hz to go down to a 5ms delay here
        // https://www.esp32.com/viewtopic.php?t=7554
        vTaskDelay(5 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
        if(length) {
            uart_read_bytes(uart_num, data, length, 0);
            callback_midi_message_received(data, length);
        }
    } // end loop forever


}
#else


#endif
