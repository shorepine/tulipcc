// Alles multicast synthesizer
// Brian Whitman
// brian@variogr.am
#include "alles.h"

uint8_t board_level;
uint8_t status;


#ifdef ESP_PLATFORM
// mutex that locks writes to the delta queue
SemaphoreHandle_t xQueueSemaphore;

#endif

uint8_t board_level = ALLES_BOARD_V2;
uint8_t status = RUNNING;

char githash[8];

#ifdef ESP_PLATFORM
// Button event
extern xQueueHandle gpio_evt_queue;

// Task handles for the renderers, multicast listener and main
TaskHandle_t renderTask[2]; // one per core
static TaskHandle_t fillbufferTask = NULL;
static TaskHandle_t idleTask0 = NULL;
static TaskHandle_t idleTask1 = NULL;

// Battery status for V2 board. If no v2 board, will stay at 0
uint8_t battery_mask = 0;

#endif

// AMY synth states
extern struct state global;
extern uint32_t event_counter;
extern uint32_t message_counter;


// Stuff for Alles the speaker not in here(yet)
uint8_t debug_on = 0;
void update_map(uint8_t client, uint8_t ipv4, int64_t time) {}
void handle_sync(int64_t time, int8_t index) {}
int16_t client_id = 0;
uint8_t alive =1;
char raw_file[1] = "";



#ifdef ESP_PLATFORM
// Wrap AMY's renderer into 2 FreeRTOS tasks, one per core
void esp_render_task( void * pvParameters) {
    uint8_t which = *((uint8_t *)pvParameters);
    uint8_t start = (OSCS/2); 
    uint8_t end = OSCS;
    if(which == 0) { start = 0; end = (OSCS/2); } 
    printf("I'm renderer #%d on core #%d and i'm handling oscs %d up until %d\n", which, xPortGetCoreID(), start, end);
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        render_task(start, end, which);
        xTaskNotifyGive(fillbufferTask);
    }
}


// Make AMY's FABT run forever , as a FreeRTOS task 
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
#endif


extern char *message_start_pointer;
extern int16_t message_length;


void alles_send_message(char * message, uint16_t len) {
    message_start_pointer = message;
    message_length = len;
    // tell the parse task, time to parse this message into deltas and add to the queue
    parse_task();
}

#ifdef ESP_PLATFORM
// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 
amy_err_t esp_amy_init() {
    start_amy();
    // We create a mutex for changing the event queue and pointers as two tasks do it at once
    xQueueSemaphore = xSemaphoreCreateMutex();

    // Create rendering threads, one per core so we can deal with dan ellis float math
    static uint8_t zero = 0;
    static uint8_t one = 1;
    xTaskCreatePinnedToCore(&esp_render_task, "render_task0", 4096, &zero, 4, &renderTask[0], 0);
    xTaskCreatePinnedToCore(&esp_render_task, "render_task1", 4096, &one, 4, &renderTask[1], 1);

    // Wait for the render tasks to get going before starting the i2s task
    delay_ms(100);

    // And the fill audio buffer thread, combines, does volume & filters
    xTaskCreatePinnedToCore(&esp_fill_audio_buffer_task, "fill_audio_buff", 4096, NULL, 22, &fillbufferTask, 0);

    // Grab the idle handles while we're here, we use them for CPU usage reporting
    idleTask0 = xTaskGetIdleTaskHandleForCPU(0);
    idleTask1 = xTaskGetIdleTaskHandleForCPU(1);
    return AMY_OK;
}
#else

extern void *soundio_run(void *vargp);
#include <pthread.h>
amy_err_t unix_amy_init() {
    //sync_init();
    start_amy();
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, soundio_run, NULL);
    return AMY_OK;
}
#endif


#ifdef ESP_PLATFORM
void esp_show_debug(uint8_t type) { 
    printf("debug\n");
    /*
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x, i;
    const char* const tasks[] = { "render_task0", "render_task1", "mcast_task", "parse_task", "main", "fill_audio_buff", "wifi", "idle0", "idle1", 0 }; 
    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );
    unsigned long counter_since_last[MAX_TASKS];
    unsigned long ulTotalRunTime = 0;
    TaskStatus_t xTaskDetails;
    // We have to check for the names we want to track
    for(i=0;i<MAX_TASKS;i++) { // for each name
        counter_since_last[i] = 0;
        for(x=0; x<uxArraySize; x++) { // for each task
            if(strcmp(pxTaskStatusArray[x].pcTaskName, tasks[i])==0) {
                counter_since_last[i] = pxTaskStatusArray[x].ulRunTimeCounter - last_task_counters[i];
                last_task_counters[i] = pxTaskStatusArray[x].ulRunTimeCounter;
                ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
            }
        }
        
        // Have to get these specially as the task manager calls them both "IDLE" and swaps their orderings around
        if(strcmp("idle0", tasks[i])==0) { 
            vTaskGetInfo(idleTask0, &xTaskDetails, pdFALSE, eRunning);
            counter_since_last[i] = xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            last_task_counters[i] = xTaskDetails.ulRunTimeCounter;
            ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
        }
        if(strcmp("idle1", tasks[i])==0) { 
            vTaskGetInfo(idleTask1, &xTaskDetails, pdFALSE, eRunning);
            counter_since_last[i] = xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            last_task_counters[i] = xTaskDetails.ulRunTimeCounter;
            ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
        }
        

    }
    printf("------ CPU usage since last call to debug()\n");
    for(i=0;i<MAX_TASKS;i++) {
        printf("%-15s\t%-15ld\t\t%2.2f%%\n", tasks[i], counter_since_last[i], (float)counter_since_last[i]/ulTotalRunTime * 100.0);
    }   
    printf("------\nEvent queue size %d / %d. Received %d events and %d messages\n", global.event_qsize, EVENT_FIFO_LEN, event_counter, message_counter);
    event_counter = 0;
    message_counter = 0;
    vPortFree(pxTaskStatusArray);
    */
}

   

// Setup I2S
amy_err_t setup_i2s(void) {
    //i2s configuration
    i2s_config_t i2s_config = {
         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
         .sample_rate = SAMPLE_RATE,
         .bits_per_sample = I2S_SAMPLE_TYPE,
         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, 
         .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
         .intr_alloc_flags = 0, //ESP_INTR_FLAG_LEVEL1, // high interrupt priority
         .dma_buf_count = 2, //I2S_BUFFERS,
         .dma_buf_len = 1024, //BLOCK_SIZE * BYTES_PER_SAMPLE,
         .tx_desc_auto_clear = true,
        };
        
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_I2S_BCLK, 
        .ws_io_num = CONFIG_I2S_LRCLK,  
        .data_out_num = CONFIG_I2S_DIN, 
        .data_in_num = -1   //Not used
    };
    //SET_PERI_REG_BITS(I2S_TX_TIMING_REG(0), 0x1, 1, I2S_TX_DSYNC_SW_S);

    i2s_driver_install((i2s_port_t)CONFIG_I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin((i2s_port_t)CONFIG_I2S_NUM, &pin_config);
    i2s_set_sample_rates((i2s_port_t)CONFIG_I2S_NUM, SAMPLE_RATE);
    return AMY_OK;
}

#endif




#ifdef ESP_PLATFORM
void alles_start() {
    check_init(&esp_event_loop_create_default, "Event");
    check_init(&setup_i2s, "i2s");
    esp_amy_init();
    reset_oscs();
    // Schedule a "turning on" sound
    bleep();

    // Spin this core until the power off button is pressed, parsing events and making sounds
    while(status & RUNNING) {
        delay_ms(10);
    }
}

#else

void * alles_start(void *vargs) {
    unix_amy_init();
    reset_oscs();
    // Schedule a "turning on" sound
    bleep();
    // Spin this core until the power off button is pressed, parsing events and making sounds
    while(status & RUNNING) {
        delay_ms(10);
    }
    return 0;
}

#endif
