// midi.c
#include "midi.h"
#include "polyfills.h"
uint8_t last_midi[MIDI_QUEUE_DEPTH][MAX_MIDI_BYTES_PER_MESSAGE];
uint8_t last_midi_len[MIDI_QUEUE_DEPTH];
extern mp_obj_t midi_callback;

#define DEBUG_MIDI 0

#ifdef AMYBOARD
#include "tusb.h"
#endif
uint8_t current_midi_message[3] = {0,0,0};
uint8_t midi_message_slot = 0;

uint8_t midi_message_buffer[MAX_MIDI_BYTES_TO_PARSE];
uint8_t sysex_flag = 0;
int16_t midi_queue_head = 0;
int16_t midi_queue_tail = 0;


void midi_clock_received() {
    // do nothing
    // but one day update our sequencer
}

// This takes a fully formed (and with status byte) midi messages and puts it in a queue that Python reads from.
// We have to do this as python may be slower than the bytes come in.
void callback_midi_message_received(uint8_t *data, size_t len) {
    //fprintf(stderr, "adding midi message of %d bytes to queue: ", len);
    for(uint32_t i = 0; i < (uint32_t)len; i++) {
        if(i < MAX_MIDI_BYTES_PER_MESSAGE) {
            //fprintf(stderr, "%02x ", data[i]);
            last_midi[midi_queue_tail][i] = data[i];
        }
    }
    //fprintf(stderr, "\n");
    last_midi_len[midi_queue_tail] = (uint16_t)len;
    midi_queue_tail = (midi_queue_tail + 1) % MIDI_QUEUE_DEPTH;
    if (midi_queue_tail == midi_queue_head) {
        // Queue wrap, drop oldest item.
        midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
        //fprintf(stderr, "dropped midi message\n");
    }

    // We tell Python that a MIDI message has been received
    if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_false);
}

/*
    3 0x8X - note off    |   note number    |  velocity 
    3 0x9X - note on     |   note number    |  velocity
    3 0xAX - Paftertouch |   note number    |  pressure
    3 0xBX - CC          |   controller #   |  value 
    2 0xCX - PC          |   program        |  XXXX
    2 0xDX - Caftertouch |   pressure       |  XXXX
    3 0xEX - pitch bend  |    LSB.          |  MSB
    X 0xF0  - sysex start|  ... wait until F7
    2 0xF1  - time code  | data
    3 0xF2 song pos      | lsb              | msb
    2 0xF3 song sel      | data
    1 0xF4 reserved      | XXXX
    1 0xF5 reserved.     | XXXX
    1 0xF6 tune request  | XXXX
    X 0xF7 end of sysex  | XXXX
    1 0xF8 timing clock  | XXXX
    1 0xF9 reserved.     | XXXX
    1 0xFA start         | XXXX
    1 0xFB continue      | XXXX
    1 0xFC stop          | XXXX
    1 0xFD reserved      | XXXX
    1 0xFE sensing       | XXXX
    1 0xFF reset         | XXXX
*/

uint16_t sysex_len = 0;

extern void alles_send_message(char * message, uint16_t len);

#ifdef __EMSCRIPTEN__
EM_JS(void, call_amy_js, (const char *message, int len), {
    amy_play_message(UTF8ToString(message, len));
});
#endif

uint8_t * sysex_buffer;
void parse_sysex() {
    if(sysex_len>3) {
        // let's use 0x00 0x03 0x45 for SPSS
        if(sysex_buffer[0] == 0x00 && sysex_buffer[1] == 0x03 && sysex_buffer[2] == 0x45) {
            sysex_buffer[sysex_len] = 0;
            #ifndef __EMSCRIPTEN__
            alles_send_message((char*)(sysex_buffer+3), sysex_len-3);
            #else
            call_amy_js((char*)sysex_buffer+3, sysex_len-3);
            #endif
            sysex_len = 0; // handled
        } else {
            // alert python there's some sysex to read
            if(midi_callback!=NULL) mp_sched_schedule(midi_callback, mp_const_true);
            // will be handled when tulip.sysex_in is called
        }
    }
}

void convert_midi_bytes_to_messages(uint8_t * data, size_t len, uint8_t usb) {
    // i take any amount of bytes and add messages 
    // remember this can start in the middle of a midi message, so act accordingly
    // running status is handled by keeping the status byte around after getting a message.
    // remember that USB midi always comes in groups of 3 here, even if it's just a one byte message
    // so we have USB (and mac IAC) set a usb flag so we know to end the loop once a message is parsed
    for(size_t i=0;i<len;i++) {

        uint8_t byte = data[i];

        // Skip sysex in this parser until we get an F7. We do not pass sysex over to python (yet)
        if(sysex_flag) {
            if(byte == 0xF7) {
                sysex_flag = 0;
                parse_sysex();
            } else {
                sysex_buffer[sysex_len++] = byte;
            }
        } else {
            if(byte & 0x80) { // new status byte 
                sysex_flag = 0; sysex_len = 0;
                // Single byte message?
                current_midi_message[0] = byte;
                if(byte == 0xF4 || byte == 0xF5 || byte == 0xF6 || byte == 0xF9 || 
                    byte == 0xFA || byte == 0xFB || byte == 0xFC || byte == 0xFD || byte == 0xFE || byte == 0xFF) {
                    callback_midi_message_received(current_midi_message, 1); 
                    if(usb) i = len+1; // exit the loop if usb
                }  else if(byte == 0xF0) { // sysex start 
                    // if that's there we then assume everything is an AMY message until 0xF7
                    sysex_flag = 1;
                } else if(byte == 0xF8) { // clock. don't forward this on to Tulip userspace
                    midi_clock_received();
                    if(usb) i = len+1; // exit the loop if usb
                } else { // a new status message that expects at least one byte of message after
                    current_midi_message[0] = byte;
                }
            } else { // data byte of some kind
                uint8_t status = current_midi_message[0] & 0xF0;

                // a 2 bytes of data message
                if(status == 0x80 || status == 0x90 || status == 0xA0 || status == 0xB0 || status == 0xE0 || current_midi_message[0] == 0xF2) {
                    if(midi_message_slot == 0) {
                        current_midi_message[1] = byte;
                        midi_message_slot = 1;
                    } else {
                        current_midi_message[2] = byte;
                        midi_message_slot = 0;
                        callback_midi_message_received(current_midi_message, 3);
                    }
                // a 1 byte data message
                } else if (status == 0xC0 || status == 0xD0 || current_midi_message[0] == 0xF3 || current_midi_message[0] == 0xF1) {
                    current_midi_message[1] = byte;
                    callback_midi_message_received(current_midi_message, 2);
                    if(usb) i = len+1; // exit the loop if usb
                }
            }
        }
    }
    
}
void process_single_midi_byte(uint8_t byte) {
    uint8_t data[1];
    data[0] = byte;
    convert_midi_bytes_to_messages(data, 1, 1);
}


void midi_local(uint8_t * bytes, uint16_t len) {
    convert_midi_bytes_to_messages(bytes, len, 0);
}

#ifdef ESP_PLATFORM
QueueHandle_t uart_queue;

#ifndef TDECK
#ifndef AMYBOARD
extern void send_usb_midi_out(uint8_t * data, uint16_t len);
extern bool midi_has_out;
#endif
#endif

void midi_out(uint8_t * bytes, uint16_t len) {
    #if !defined (TDECK) && !defined(AMYBOARD)
        if(midi_has_out) { // usb midi HOST
            send_usb_midi_out(bytes,len);
        } else { // uart midi
            uart_write_bytes(UART_NUM_1, bytes, len);
        }
    #else
        uart_write_bytes(UART_NUM_1, bytes, len);
        #ifdef AMYBOARD
        tud_midi_stream_write(0, bytes, len);
        #endif
    #endif
}

void run_midi() {
    sysex_buffer = malloc_caps(MAX_SYSEX_BYTES, MALLOC_CAP_SPIRAM);
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

    const int uart_buffer_size = (MAX_MIDI_BYTES_TO_PARSE);
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size,  0, 0, NULL, 0));

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

    uint8_t data[MAX_MIDI_BYTES_TO_PARSE];
    size_t length = 0;
    while(1) {
        length = uart_read_bytes(uart_num, data, MAX_MIDI_BYTES_TO_PARSE /*MAX_MIDI_BYTES_PER_MESSAGE*MIDI_QUEUE_DEPTH*/, 1/portTICK_PERIOD_MS);
        if(length > 0) {
            //uart_flush(uart_num);
            //fprintf(stdout, "got raw bytes of %d length:  ", length);
            //for(uint8_t i=0;i<length;i++) {
            //    fprintf(stdout, "%02x ", data[i]);
            //}
            //fprintf(stdout, "\n");
            convert_midi_bytes_to_messages(data,length,0);
        }
        #ifdef AMYBOARD
        // check midi USB gadget
        while ( tud_midi_available() ) {
            uint8_t packet[4];
            tud_midi_packet_read(packet);
            //fprintf(stderr, "got usb bytes of %d length:  ", 4);
            //for(uint8_t i=0;i<4;i++) {
            //    fprintf(stderr, "%02x ", packet[i]);
            //}
            //fprintf(stderr, "\n");
            convert_midi_bytes_to_messages(packet+1, 3, 1);
        }
        //delay_ms(10);
        #endif

    } // end loop forever
}
#else
// midi out is in virtualmidi 
#ifdef MACOS
// defined in virtualmidi.m
#else

// This is MIDI out on tulip desktop that is NOT macos... web or linux
void midi_out(uint8_t * bytes, uint16_t len) {
    #ifdef __EMSCRIPTEN__
    EM_ASM(
            if(midiOutputDevice != null) {
                midiOutputDevice.send(HEAPU8.subarray($0, $0 + $1));
            }, bytes, len
        );
    #else
    // nothing yet for linux
    #endif
}
#endif

#endif
