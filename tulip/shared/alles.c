// Alles multicast synthesizer
// Brian Whitman
// brian@variogr.am

#include "alles.h"

uint8_t board_level;
uint8_t status;
uint8_t mesh_local_playback  = 1; 
uint8_t * external_map;

#ifdef ESP_PLATFORM
// mutex that locks writes to the delta queue
SemaphoreHandle_t xQueueSemaphore;

#else
extern int get_first_ip_address(char*);
#endif

uint8_t board_level = ALLES_BOARD_V2;
uint8_t status = RUNNING;

char githash[8];

// AMY synth states
extern struct state global;
extern uint32_t event_counter;
extern uint32_t message_counter;
extern void mcast_send(char*, uint16_t);

uint8_t debug_on = 0;
char raw_file[1] = "";
char * alles_local_ip;
// sync.c -- keep track of mulitple synths
//extern uint8_t battery_mask;
extern uint8_t ipv4_quartet;
extern char githash[8];
int32_t clocks[255];
int32_t ping_times[255];
uint8_t alive = 1;
extern int32_t last_ping_time;

uint8_t mesh_flag = 0;

amy_err_t sync_init() {
    for(uint8_t i=0;i<255;i++) { clocks[i] = 0; ping_times[i] = 0; }
    return AMY_OK;
}



#ifdef ESP_PLATFORM
#include "driver/i2s_std.h"
#include "driver/gpio.h"
i2s_chan_handle_t tx_handle;


extern void mcast_listen_task(void *pvParameters);

// Render the second core
void esp_render_task( void * pvParameters) {
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        amy_render(0, AMY_OSCS/2, 1);
        xTaskNotifyGive(alles_fill_buffer_handle);
    }
}

// Make AMY's FABT run forever , as a FreeRTOS task 
void esp_fill_audio_buffer_task() {
    while(1) {
        AMY_PROFILE_START(AMY_ESP_FILL_BUFFER)

        // Get ready to render
        amy_prepare_buffer();
        // Tell the other core to start rendering
        xTaskNotifyGive(amy_render_handle);
        // Render me
        amy_render(AMY_OSCS/2, AMY_OSCS, 0);
        // Wait for the other core to finish
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Write to i2s
        int16_t *block = amy_fill_buffer();
        AMY_PROFILE_STOP(AMY_ESP_FILL_BUFFER)

        // We turn off writing to i2s on r10 when doing on chip debugging because of pins
        #ifndef TULIP_R10_DEBUG
        size_t written = 0;
        i2s_channel_write(tx_handle, block, AMY_BLOCK_SIZE * AMY_BYTES_PER_SAMPLE * AMY_NCHANS, &written, portMAX_DELAY);
        if(written != AMY_BLOCK_SIZE * AMY_BYTES_PER_SAMPLE * AMY_NCHANS) {
            fprintf(stderr,"i2s underrun: %d vs %d\n", written, AMY_BLOCK_SIZE * AMY_BYTES_PER_SAMPLE * AMY_NCHANS);
        }
        #endif


    }
}

#else
void *mcast_listen_task(void *vargp);
#endif


extern char *message_start_pointer;
extern int16_t message_length;
#include "py/compile.h"
#include "py/runtime.h"
#include "py/builtin.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/objstr.h"
#include "py/cstack.h"
#include "py/mphal.h"
#include "py/mpthread.h"
#include "py/objmodule.h"

extern mp_obj_t amy_block_done_callback;

void alles_send_message(char * message, uint16_t len) {
    //fprintf(stderr, "LOG: alles_send_message at time %lld\n", amy_sysclock());
    alles_parse_message(message, len);
}

void tulip_amy_block_done() {
    if(amy_block_done_callback != NULL && amy_block_done_callback != mp_const_none) {
        mp_sched_schedule(amy_block_done_callback, mp_const_none);
    }
}

#ifdef ESP_PLATFORM
// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 

#else


#endif


#ifdef ESP_PLATFORM

#ifndef AMYBOARD
amy_err_t setup_i2s(void) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    i2s_new_channel(&chan_cfg, &tx_handle, NULL);
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(AMY_SAMPLE_RATE),
        //#ifndef TDECK
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        //#else
        //.slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        //#endif
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = CONFIG_I2S_BCLK,
            .ws = CONFIG_I2S_LRCLK,
            .dout = CONFIG_I2S_DIN,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    /* Initialize the channel */
    i2s_channel_init_std_mode(tx_handle, &std_cfg);

    /* Before writing data, start the TX channel first */
    i2s_channel_enable(tx_handle);
    fprintf(stderr, "i2s started blck %d dout %d lrck %d\n",CONFIG_I2S_BCLK,CONFIG_I2S_DIN,CONFIG_I2S_LRCLK );
    return AMY_OK;
}
#endif

#endif



#ifdef ESP_PLATFORM
#include "driver/i2c.h"

// An AMY hook to send values out to a CV DAC over i2c, only on ESP 
uint8_t external_cv_render(uint16_t osc, SAMPLE * buf, uint16_t len) {
    if(external_map[osc]>0) {
        float volts = S2F(buf[0])*2.5f + 2.5f;
        // do the thing
        uint16_t value_int = (uint16_t)((volts/10.0) * 65535.0);
        uint8_t bytes[3];
        bytes[2] = (value_int & 0xff00) >> 8;
        bytes[1] = (value_int & 0x00ff);

        uint8_t ch = 0x02;
        uint8_t addr = 89;
        uint8_t channel = external_map[osc]-1;
        if(channel == 1) ch = 0x04;
        if(channel == 2) addr = 88;
        if(channel == 3) {ch = 0x04; addr=88; }
        bytes[0] = ch;
        i2c_master_write_to_device(I2C_NUM_0, addr, bytes, 3, pdMS_TO_TICKS(10));
        return 1;
    }
    return 0;

}
void run_alles() {
    check_init(&esp_event_loop_create_default, "Event");
    // We turn off writing to i2s on r10 when doing on chip debugging because of pins
    // clear the external map
    for(uint16_t i=0;i<AMY_OSCS;i++) external_map[i] = 0;
    amy_external_render_hook = external_cv_render;

    amy_config_t config = amy_default_config();
    config.cores = 2;
    config.has_audio_in = 0;
    config.lrc = CONFIG_I2S_LRCLK;
    config.bclk = CONFIG_I2S_BCLK;
    config.dout = CONFIG_I2S_DIN; // badly named
    amy_start(config);
    amy_live_start();

}

#else

/*
void * alles_start(void *vargs) {
    alles_local_ip = malloc(256);
    alles_local_ip[0] = 0;
    unix_amy_init();
    amy_reset_oscs();
    // Spin this core until the power off button is pressed, parsing events and making sounds
    while(status & RUNNING) {
        delay_ms(10);
    }
    return 0;
}
*/
#endif

#ifdef ESP_PLATFORM
// Make AMY's parse task run forever, as a FreeRTOS task (with notifications)
void esp_parse_task() {
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        alles_parse_message(message_start_pointer, message_length);
        xTaskNotifyGive(alles_receive_handle);
    }
}

#include "esp_wifi.h"
#endif

#ifndef ESP_PLATFORM
    pthread_t thread_id;
#endif

void alles_init_multicast() {
    if(!mesh_flag) {
    #ifdef ESP_PLATFORM
        fprintf(stderr, "creating socket\n");
        create_multicast_ipv4_socket();
        fprintf(stderr, "power save off\n");
        esp_wifi_set_ps(WIFI_PS_NONE);
        fprintf(stderr, "creating mcast task\n");
        xTaskCreatePinnedToCore(&mcast_listen_task, ALLES_RECEIVE_TASK_NAME, ALLES_RECEIVE_TASK_STACK_SIZE, NULL, ALLES_RECEIVE_TASK_PRIORITY, &alles_receive_handle, ALLES_RECEIVE_TASK_COREID);
        fprintf(stderr, "creating parse task\n");
        xTaskCreatePinnedToCore(&esp_parse_task, ALLES_PARSE_TASK_NAME, ALLES_PARSE_TASK_STACK_SIZE, NULL, ALLES_PARSE_TASK_PRIORITY, &alles_parse_handle, ALLES_PARSE_TASK_COREID);
    #else
        if(alles_local_ip[0]==0) {
            get_first_ip_address(alles_local_ip);
        }
        fprintf(stderr, "creating mcast task\n");
        pthread_create(&thread_id, NULL, mcast_listen_task, NULL);
    #endif
        mesh_flag = 1;
    }
}






void update_map(uint8_t client, uint8_t ipv4, int32_t time) {
    // I'm called when I get a sync response or a regular ping packet
    // I update a map of booted devices.
    //fprintf(stderr,"Got a sync response client %d ipv4 %d time %"PRIu32"\n",  client , ipv4, time);
    clocks[ipv4] = time;
    int32_t my_sysclock = amy_sysclock();
    ping_times[ipv4] = my_sysclock;

    uint8_t last_alive = alive;
    alive = 0;
    for(uint8_t i=0;i<255;i++) {
        if(clocks[i] > 0) { 
            if(my_sysclock < (ping_times[i] + (PING_TIME_MS * 2))) { // alive
                alive++;
            } else {
                clocks[i] = 0;
                ping_times[i] = 0;
            }
        }
    }
    if(last_alive != alive) {
        fprintf(stderr,"[alles] %d alive\n", alive);
    }
}

void handle_sync(int32_t time, int8_t index) {
    // Tulip doesn't respond to alles sync messages ever, as it does not boot as an alles node (it can control them only)
}



