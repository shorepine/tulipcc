// midi.c
#include "alles.h"
#ifdef ESP_PLATFORM // for now
QueueHandle_t uart_queue;
uint8_t midi_voice = 0;
uint8_t program_bank = 0;
uint8_t program = 0;
uint8_t note_map[OSCS];


void callback_midi_message_received(uint8_t source, uint16_t timestamp, uint8_t midi_status, uint8_t *remaining_message, size_t len) {
    // uart is 1 if this came in through uart, 0 if ble
    //printf("got midi message source %d ts %d status %d -- ", source, timestamp, midi_status);
    //for(int i=0;i<len;i++) fprintf(stderr, "MIDI %d ", remaining_message[i]);
    //printf("\n");
    uint8_t channel = midi_status & 0x0F;
    uint8_t message = midi_status & 0xF0;
    if(len > 0) {
        uint8_t data1 = remaining_message[0];
        if(message == 0x90) {  // note on 
            uint8_t data2 = remaining_message[1];
            struct event e = default_event();
            e.time = get_sysclock();
            if(program_bank > 0) {
                e.wave = ALGO;
                e.patch = ((program_bank-1) * 128) + program;
            } else {
                e.wave = program;
            }
            e.osc = midi_voice;
            e.midi_note = data1;
            e.velocity = data2;
            e.amp = 0.1; // for now
            note_map[midi_voice] = data1;
            if(channel == 0) {
                add_event(e);
            }
            midi_voice = (midi_voice + 1) % (OSCS);
        } else if (message == 0x80) { 
            // note off
            uint8_t data2 = remaining_message[1];
            // for now, only handle broadcast note offs... will have to refactor if i go down this path farther
            for(uint8_t v=0;v<OSCS;v++) {
                if(note_map[v] == data1) {
                    struct event e = default_event();
                    e.amp = 0;
                    e.osc = v;
                    e.time = get_sysclock();
                    e.velocity = data2; // note off velocity, not used... yet
                    add_event(e);
                }
            }                        
        } else if(message == 0xC0) { // program change 
            program = data1;
        } else if(message == 0xB0) {
            // control change
            uint8_t data2 = remaining_message[1];
            // Bank select for program change
            if(data1 == 0x00) { 
                program_bank = data2;
            }
            // feedback
            // duty cycle
            // pitch bend (?) 
            // amplitude / volume
        }
    }
}



void read_midi_uart() {
    printf("UART MIDI running on core %d\n", xPortGetCoreID());
    const int uart_num = UART_NUM_1;
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
            //fprintf(stderr, "got bytes %d\n", length);
            if(length > 1) {
                callback_midi_message_received(1,esp_timer_get_time() / 1000, data[0], data+1, length-1);
            }
        }  // end was there any bytes at all 
    } // end loop forever
}


void setup_midi_in() {
    // Setup UART2 to listen for MIDI messages 
    const int uart_num = UART_NUM_1;
    uart_config_t uart_config = {
        .baud_rate = 31250,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    // TX, RX, CTS/RTS -- Only care about RX here, pin 47 for now
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 11, 47, 10, 9));

    const int uart_buffer_size = (1024 * 2);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, uart_buffer_size, \
                                          uart_buffer_size, 10, &uart_queue, 0));

    xTaskCreatePinnedToCore(&read_midi_uart, "read_midi_task", 4096, NULL, 1, NULL, 0);


}
#endif