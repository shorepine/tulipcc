// amy-example-esp32.c
// a minimal example of running AMY from an ESP32

#ifdef ESP_PLATFORM


#include "amy.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_sleep.h"
#include "driver/i2s.h"

#define I2S_SAMPLE_TYPE I2S_BITS_PER_SAMPLE_16BIT

#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_I2S_LRCLK 4
#define CONFIG_I2S_BCLK 1
#define CONFIG_I2S_DIN 2
#define CONFIG_I2S_NUM 0 

#define AMY_EXAMPLE_RENDER_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define AMY_EXAMPLE_FILL_BUFFER_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 4)

#define AMY_EXAMPLE_RENDER_0_TASK_COREID (1)
#define AMY_EXAMPLE_RENDER_1_TASK_COREID (0)
#define AMY_EXAMPLE_FILL_BUFFER_TASK_COREID (1)

#define AMY_EXAMPLE_RENDER_TASK_STACK_SIZE (8 * 1024)
#define AMY_EXAMPLE_FILL_BUFFER_TASK_STACK_SIZE (4 * 1024)

#define AMY_EXAMPLE_RENDER_0_TASK_NAME      "amy_r0_task"
#define AMY_EXAMPLE_RENDER_1_TASK_NAME      "amy_r1_task"
#define AMY_EXAMPLE_FILL_BUFFER_TASK_NAME   "amy_fb_task"

SemaphoreHandle_t xQueueSemaphore;
TaskHandle_t amy_render_handle[AMY_CORES]; // one per core
TaskHandle_t amy_fill_buffer_handle;



// Wrap AMY's renderer into 2 FreeRTOS tasks, one per core
void esp_render_task( void * pvParameters) {
    uint8_t which = *((uint8_t *)pvParameters);
    uint8_t start = 0; 
    uint8_t end = OSCS;
    if (AMY_CORES == 2) {
        start = (OSCS/2); 
        end = OSCS;
        if(which == 0) { start = 0; end = (OSCS/2); } 
    }
    fprintf(stderr,"I'm renderer #%d on core #%d and i'm handling oscs %d up until %d\n", which, xPortGetCoreID(), start, end);
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        render_task(start, end, which);
        xTaskNotifyGive(amy_fill_buffer_handle);
    }
}

// Make AMY's FABT run forever , as a FreeRTOS task, writing to i2s
void esp_fill_audio_buffer_task() {
    while(1) {
        int16_t *block = fill_audio_buffer_task();
        size_t written = 0;
        i2s_write((i2s_port_t)CONFIG_I2S_NUM, block, BLOCK_SIZE * BYTES_PER_SAMPLE, &written, portMAX_DELAY); 
        if(written != BLOCK_SIZE*BYTES_PER_SAMPLE) {
            fprintf(stderr,"i2s underrun: %d vs %d\n", written, BLOCK_SIZE*BYTES_PER_SAMPLE);
        }
    }
}


// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 
amy_err_t esp_amy_init() {
    amy_start();
    // We create a mutex for changing the event queue and pointers as two tasks do it at once
    xQueueSemaphore = xSemaphoreCreateMutex();

    // Create rendering threads, one per core so we can deal with dan ellis float math
    static uint8_t zero = 0;
    static uint8_t one = 1;
    xTaskCreatePinnedToCore(&esp_render_task, AMY_EXAMPLE_RENDER_0_TASK_NAME, AMY_EXAMPLE_RENDER_TASK_STACK_SIZE, &zero, AMY_EXAMPLE_RENDER_TASK_PRIORITY, &amy_render_handle[0], AMY_EXAMPLE_RENDER_0_TASK_COREID);
    if(AMY_CORES>1) xTaskCreatePinnedToCore(&esp_render_task, AMY_EXAMPLE_RENDER_1_TASK_NAME, AMY_EXAMPLE_RENDER_TASK_STACK_SIZE, &one, AMY_EXAMPLE_RENDER_TASK_PRIORITY, &amy_render_handle[1], AMY_EXAMPLE_RENDER_1_TASK_COREID);
    // Wait for the render tasks to get going before starting the i2s task
    delay_ms(100);

    // And the fill audio buffer thread, combines, does volume & filters
    xTaskCreatePinnedToCore(&esp_fill_audio_buffer_task, AMY_EXAMPLE_FILL_BUFFER_TASK_NAME, AMY_EXAMPLE_FILL_BUFFER_TASK_STACK_SIZE, NULL, AMY_EXAMPLE_FILL_BUFFER_TASK_PRIORITY, &amy_fill_buffer_handle, AMY_EXAMPLE_FILL_BUFFER_TASK_COREID);
    return AMY_OK;
}


// Setup I2S on esp32
amy_err_t setup_i2s(void) {
    //i2s configuration
    i2s_config_t i2s_config = {
         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
         .sample_rate = SAMPLE_RATE,
         .bits_per_sample = I2S_SAMPLE_TYPE,
         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, 
         .communication_format = I2S_COMM_FORMAT_STAND_MSB,
         .intr_alloc_flags = 0, 
         .dma_buf_count = 2, 
         .dma_buf_len = 1024, 
         .tx_desc_auto_clear = true,
        };
        
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_I2S_BCLK, 
        .ws_io_num = CONFIG_I2S_LRCLK,  
        .data_out_num = CONFIG_I2S_DIN, 
        .data_in_num = -1   //Not used
    };

    i2s_driver_install((i2s_port_t)CONFIG_I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin((i2s_port_t)CONFIG_I2S_NUM, &pin_config);
    i2s_set_sample_rates((i2s_port_t)CONFIG_I2S_NUM, SAMPLE_RATE);
    return AMY_OK;
}


void app_main() {
    esp_event_loop_create_default();
    setup_i2s();
    esp_amy_init();

    // Play a few notes in FM
    struct event e = amy_default_event();
    int64_t start = amy_sysclock();
    e.time = start;
    e.velocity = 1;
    e.wave = ALGO;
    e.patch = 15;
    e.midi_note = 60;
    amy_add_event(e);

    e.time = start + 500;
    e.osc += 9; // remember that an FM patch takes up 9 oscillators
    e.midi_note = 64;
    amy_add_event(e);
    
    e.time = start + 1000;
    e.osc += 9;
    e.midi_note = 68;
    amy_add_event(e);


    // Run forever
    while(1) {
        delay_ms(10);
    }
}


#endif