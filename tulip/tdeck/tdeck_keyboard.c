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
#include "usb_keyboard.h"

#define TIMEOUT_MS    10

int state = 0;
QueueHandle_t interruptQueue;

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

void run_tdeck_keyboard() {

    uint8_t rx_data[5];

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = TDECK_I2C_SDA,
        .scl_io_num = TDECK_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    i2c_param_config(I2C_NUM_0, &conf);

    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

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
        i2c_master_read_from_device(I2C_NUM_0, LILYGO_KB_SLAVE_ADDRESS, rx_data, 1, pdMS_TO_TICKS(TIMEOUT_MS));
        if(rx_data[0]>0) {
            // Send shift-$ or shift-volume as control C
            if(rx_data[0]==4)  { 
                send_key_to_micropython(3); // ctrl c
            // Send shift 0 or shift-microphone as control X
            }  else if(rx_data[0]==224) {
                send_key_to_micropython(24); 
            } else {
                // Send as is
                send_key_to_micropython(rx_data[0]);
            }
        }
    }
}


