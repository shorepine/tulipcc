// usb_keyboard.c
#include "tdeck_keyboard.h"
#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "string.h"
#include "display.h"
#include "soc/io_mux_reg.h"
#include "ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "usb_host.h"

#define TIMEOUT_MS    10

int state = 0;
QueueHandle_t interruptQueue;
struct KeyMapping {
    // The following characters should be available to map to an alternative
    // Default: q w e r t y u i o p a s d f g h j k l z x c v b n m $  
    // Shift: Q W E R T Y U I O P A S D F G H J K L Z X C V B N M
    // Symbol: # 1 2 3 ( ) _ - + @ * 4 5 6 / : ; \ " 7 8 9 ? ! , . 0
    char original;
    char alternative;
};

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interruptQueue, &pinNumber, NULL);
}

// Send trackpall spins as arrow keys.
// The push current sends "enter"
void watch_trackball(void *params)
{
    uint8_t raw_data[40];
    for(uint8_t i=0;i<40;i++) raw_data[i] = 0;
    int pinNumber = 0;
    while (true)
    {
        if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
        {
            if(pinNumber == TDECK_TRACKBALL_UP) send_key_to_micropython(259);
            if(pinNumber == TDECK_TRACKBALL_DOWN) send_key_to_micropython(258);
            if(pinNumber == TDECK_TRACKBALL_LEFT) send_key_to_micropython(260);
            if(pinNumber == TDECK_TRACKBALL_RIGHT) send_key_to_micropython(261);
            if(pinNumber == TDECK_TRACKBALL_CLICK) send_key_to_micropython(13);
            //fprintf(stderr,"GPIO %d was pressed. The state is %d\n", pinNumber, gpio_get_level(pinNumber));
        }
    }
}

char get_alternative_char(struct KeyMapping mappings[], int size, char original) {
    for (int i = 0; i < size; ++i) {
        if (mappings[i].original == original) {
            return mappings[i].alternative;
        }
    }
    // Return the original character if no alternative is found
    return original;
}

#define KEYBOARD_BUFFER_SIZE 32

static char lvgl_kb_buf[KEYBOARD_BUFFER_SIZE];


void lvgl_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data) {
    (void) indev_drv;     // unused

    static bool dummy_read = false;
    const size_t len = strlen(lvgl_kb_buf);

    // Send a release manually
    if (dummy_read) {
        dummy_read = false;
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = len > 0;
    }
        // Send the pressed character 
    else if (len > 0) {
        dummy_read = true;
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = lvgl_kb_buf[0];
        memmove(lvgl_kb_buf, lvgl_kb_buf + 1, len);
        data->continue_reading = true;
    }
}


void run_tdeck_keyboard() {

    bool alt_char_mode = false;
    bool ctrl_toggle = false;
    uint8_t rx_data[5];
    uint8_t char_to_send[1];
    struct KeyMapping charMappings[] = {
        // Default keys
        {'q', '~'},
        {'w', '&'},
        {'e', '|'},
        {'r', '%'},
        {'t', '{'},
        {'y', '}'},
        {'u', '^'},
        {'i', '<'},
        {'o', '>'},
        {'p', '='},
        {'a', 172},     // Prints an empty character but should be a ¬ sign
        {'g', '\\'},    // Backslash needs escaping
        {'k', '`'},
        {'$', 163},     // Prints an empty character but should be a £ sign
        {' ', '\t'},    // Sends tab
        // Requires symbol to be pressed after alt+c
        {'(', '['},
        {')', ']'},
    };
    struct KeyMapping ctrlMappings[] = {
        // Default keys
        {'q', 17},      // Device Control 1
        {'w', 23},      // End of Transmission Block
        {'e', 5},       // Enquiry character
        {'r', 18},      // Device Control 2
        {'t', 20},      // Device Control 4
        {'y', 25},      // End of Medium
        {'u', 21},      // Negative-acknowledge character
        {'i', 9},       // Horizontal tab
        {'o', 15},      // Shift In
        {'p', 16},      // Data Link Escape
        {'a', 1},       // Start of Heading
        {'s', 19},      // Device Control 3
        {'d', 4},       // End-of-transmission character
        {'f', 6},       // Acknowledge character
        {'g', 7},       // Bell character
        {'h', 8},       // Backspace
        {'j', 10},      // Linefeed
        {'k', 11},      // Vertical tab
        {'l', 12},      // Formfeed
        {'z', 26},      // Substitute character
        {'x', 24},      // Cancel character
        {'c', 3},       // End-of-text character
        {'v', 22},      // Synchronous Idle
        {'b', 2},       // Start of Text
        {'n', 14},      // Shift Out
        {'m', 13},      // Carriage return
        {'$', 127},     // Delete (DEL)
        {' ', 27},      // Escape character
    };
    int charMappingsSize = sizeof(charMappings) / sizeof(charMappings[0]);
    int ctrlMappingsSize = sizeof(ctrlMappings) / sizeof(ctrlMappings[0]);

    // I2C already initialized on tdeck from the touchscreen

    gpio_set_direction(TDECK_TRACKBALL_UP, GPIO_MODE_INPUT);
    gpio_pulldown_dis(TDECK_TRACKBALL_UP);
    gpio_pullup_en(TDECK_TRACKBALL_UP);
    gpio_set_intr_type(TDECK_TRACKBALL_UP, GPIO_INTR_NEGEDGE);

    gpio_set_direction(TDECK_TRACKBALL_DOWN, GPIO_MODE_INPUT);
    gpio_pulldown_dis(TDECK_TRACKBALL_DOWN);
    gpio_pullup_en(TDECK_TRACKBALL_DOWN);
    gpio_set_intr_type(TDECK_TRACKBALL_DOWN, GPIO_INTR_NEGEDGE);

    gpio_set_direction(TDECK_TRACKBALL_LEFT, GPIO_MODE_INPUT);
    gpio_pulldown_dis(TDECK_TRACKBALL_LEFT);
    gpio_pullup_en(TDECK_TRACKBALL_LEFT);
    gpio_set_intr_type(TDECK_TRACKBALL_LEFT, GPIO_INTR_NEGEDGE);

    gpio_set_direction(TDECK_TRACKBALL_RIGHT, GPIO_MODE_INPUT);
    gpio_pulldown_dis(TDECK_TRACKBALL_RIGHT);
    gpio_pullup_en(TDECK_TRACKBALL_RIGHT);
    gpio_set_intr_type(TDECK_TRACKBALL_RIGHT, GPIO_INTR_NEGEDGE);

    gpio_set_direction(TDECK_TRACKBALL_CLICK, GPIO_MODE_INPUT);
    gpio_pulldown_dis(TDECK_TRACKBALL_CLICK);
    gpio_pullup_en(TDECK_TRACKBALL_CLICK);
    gpio_set_intr_type(TDECK_TRACKBALL_CLICK, GPIO_INTR_NEGEDGE);

    interruptQueue = xQueueCreate(10, sizeof(int));
    xTaskCreate(watch_trackball, "watch_trackball", 4096, NULL, 1, NULL);

    gpio_isr_handler_add(TDECK_TRACKBALL_UP, gpio_interrupt_handler, (void *)TDECK_TRACKBALL_UP);
    gpio_isr_handler_add(TDECK_TRACKBALL_DOWN, gpio_interrupt_handler, (void *)TDECK_TRACKBALL_DOWN);
    gpio_isr_handler_add(TDECK_TRACKBALL_LEFT, gpio_interrupt_handler, (void *)TDECK_TRACKBALL_LEFT);
    gpio_isr_handler_add(TDECK_TRACKBALL_RIGHT, gpio_interrupt_handler, (void *)TDECK_TRACKBALL_RIGHT);
    gpio_isr_handler_add(TDECK_TRACKBALL_CLICK, gpio_interrupt_handler, (void *)TDECK_TRACKBALL_CLICK);

    while (1) {
        i2c_master_read_from_device(I2C_NUM, LILYGO_KB_SLAVE_ADDRESS, rx_data, 1, pdMS_TO_TICKS(TIMEOUT_MS));
        if (rx_data[0] > 0) {
            if (rx_data[0] == 224) {
                // Toggle ctrl key (shift+microphone)
                ctrl_toggle = !ctrl_toggle;
            } else if (rx_data[0] == 12) {
                // Set alternate character mode
                alt_char_mode = !alt_char_mode;
            } else {
                if (alt_char_mode) {
                    // Send alternate characters if alternate character set is enabled, otherwise send as is
                    char_to_send[0] = get_alternative_char(charMappings, charMappingsSize, rx_data[0]);
                    alt_char_mode = false;
                } else {
                    char_to_send[0] = rx_data[0];
                }
                // Send as is, combining with ctrl if toggled
                if (ctrl_toggle) {
                    send_key_to_micropython(get_alternative_char(ctrlMappings, ctrlMappingsSize, char_to_send[0]));
                    ctrl_toggle = false;  // Reset toggle after sending
                } else {
                    send_key_to_micropython(char_to_send[0]);
                }
            }
        }
        delay_ms(10);
    }
}


