// midi.c
#include "midi.h"
uint8_t last_midi[MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len;


void callback_midi_message_received(uint8_t *data, size_t len) {
    //fprintf(stderr,"got midi message len %d status %d -- ", (uint32_t)len, midi_status);
    for(uint32_t i=0;i<(uint32_t)len;i++) {
        if(i<MAX_MIDI_BYTES_PER_MESSAGE) {
            last_midi[i] = data[i];
        }
    }
    last_midi_len = (uint16_t)len;
    tulip_midi_isr();
}

#ifdef ESP_PLATFORM
QueueHandle_t uart_queue;

void midi_out(uint8_t * bytes, uint16_t len) {
    uart_write_bytes(UART_NUM_1, bytes, len);
}

void run_midi() {
    last_midi_len = 0;

    // Setup UART2 to listen for MIDI messages 
    const int uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate = 31250,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    // TX, RX, CTS/RTS -- Only care about RX here, pin 47 for now
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 11, 47, 10, 9));

    const int uart_buffer_size = (1024 * 2);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, uart_buffer_size, \
                                          uart_buffer_size, 10, &uart_queue, 0));


    printf("UART MIDI running on core %d\n", xPortGetCoreID());

    uint8_t data[128];
    size_t length = 0;
    while(1) {
        // Sleep 5ms to wait to get more MIDI data and avoid starving audio thread
        // I increased RTOS clock rate from 100hz to 500hz to go down to a 5ms delay here
        // https://www.esp32.com/viewtopic.php?t=7554
        vTaskDelay(10 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t*)&length));
        if(length) {
            length = uart_read_bytes(uart_num, data, length, 100);
            if(length > 1) {
                callback_midi_message_received(data, length);
            }
        }  // end was there any bytes at all 
    } // end loop forever


}
#else



#endif