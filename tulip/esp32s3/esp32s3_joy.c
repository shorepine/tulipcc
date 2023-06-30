// esp32s3_joy.

#include <stdio.h>
#include "touchscreen.h"
#include "esp_log.h"
#include "string.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#define JOY_CLOCK_PIN 13
#define JOY_LATCH_PIN 48
#define JOY_DATA_PIN 45

// TODO, i wonder if setting this up as a task and constantly reading with a timer is a better approach

unsigned long IRAM_ATTR micros()
{
    return (unsigned long)(esp_timer_get_time());
}
void IRAM_ATTR delayMicroseconds(uint32_t us)
{
    uint32_t m = micros();
    if (us)
    {
        uint32_t e = (m + us);
        if (m > e)
        { //overflow
            while (micros() > e)
            {
            }
        }
        while (micros() < e)
        {
        }
    }
}

void init_esp_joy() {
    // Set DATA_CLOCK normally HIGH
    gpio_set_direction(JOY_CLOCK_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(JOY_CLOCK_PIN, 1);
  
    // Set DATA_LATCH normally LOW
    gpio_set_direction(JOY_LATCH_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(JOY_LATCH_PIN, 0);

    // Set DATA_SERIAL normally HIGH
    gpio_set_direction(JOY_DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(JOY_DATA_PIN, 1);
    gpio_set_direction(JOY_DATA_PIN, GPIO_MODE_INPUT);  
}

uint16_t check_joy() {
    // Latch for 12us
    gpio_set_level(JOY_LATCH_PIN, 1);
    delayMicroseconds(12);
    gpio_set_level(JOY_LATCH_PIN, 0);
    delayMicroseconds(6);
    uint16_t mask = 0;
    // One of my SNES controllers does not want to read R1 with this setup. Very strange
    for(int i = 0; i < 16; i++){
        gpio_set_level(JOY_CLOCK_PIN, 0);
        delayMicroseconds(6);
        if(i <= 11){
            uint8_t bit = gpio_get_level(JOY_DATA_PIN);
            mask = (mask | (!bit)) << 1;
        }
        gpio_set_level(JOY_CLOCK_PIN, 1);
        delayMicroseconds(6);
    }
    
    return mask;

}
