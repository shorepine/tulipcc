// Alles multicast synthesizer
// Brian Whitman
// brian@variogr.am

#include "alles.h"

uint8_t board_level;
uint8_t status;
uint8_t mesh_local_playback  = 1; 

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
        i2s_channel_write(tx_handle, block, AMY_BLOCK_SIZE * BYTES_PER_SAMPLE * AMY_NCHANS, &written, portMAX_DELAY);
        if(written != AMY_BLOCK_SIZE * BYTES_PER_SAMPLE * AMY_NCHANS) {
            fprintf(stderr,"i2s underrun: %d vs %d\n", written, AMY_BLOCK_SIZE * BYTES_PER_SAMPLE * AMY_NCHANS);
        }
        #endif


    }
}

#else
void *mcast_listen_task(void *vargp);
#endif


extern char *message_start_pointer;
extern int16_t message_length;


void alles_send_message(char * message, uint16_t len) {
    //fprintf(stderr, "LOG: alles_send_message at time %lld\n", amy_sysclock());
    alles_parse_message(message, len);
}

#ifdef ESP_PLATFORM
// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 
amy_err_t esp_amy_init() {
    amy_start(2,1,1);
    // We create a mutex for changing the event queue and pointers as two tasks do it at once
    xQueueSemaphore = xSemaphoreCreateMutex();

    // Create the second core rendering task
    xTaskCreatePinnedToCore(&esp_render_task, ALLES_RENDER_TASK_NAME, ALLES_RENDER_TASK_STACK_SIZE, NULL, ALLES_RENDER_TASK_PRIORITY, &amy_render_handle, ALLES_RENDER_TASK_COREID);

    // Wait for the render tasks to get going before starting the i2s task
    delay_ms(100);

    // And the fill audio buffer thread, combines, does volume & filters
    xTaskCreatePinnedToCore(&esp_fill_audio_buffer_task, ALLES_FILL_BUFFER_TASK_NAME, ALLES_FILL_BUFFER_TASK_STACK_SIZE, NULL, ALLES_FILL_BUFFER_TASK_PRIORITY, &alles_fill_buffer_handle, ALLES_FILL_BUFFER_TASK_COREID);
    return AMY_OK;
}
#else

extern void *miniaudio_run(void *vargp);
#include <pthread.h>
amy_err_t unix_amy_init() {
    //sync_init();
    amy_start(1,1,1);
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, miniaudio_run, NULL);
    return AMY_OK;
}
#endif


#ifdef ESP_PLATFORM

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


extern struct custom_oscillator memorypcm;

#ifdef ESP_PLATFORM
void run_alles() {
    check_init(&esp_event_loop_create_default, "Event");
    // We turn off writing to i2s on r10 when doing on chip debugging because of pins
    #ifndef TULIP_R10_DEBUG
        check_init(&setup_i2s, "i2s");
    #endif
    esp_amy_init();
    amy_set_custom(&memorypcm);
    amy_reset_oscs();
    // Schedule a "turning on" sound
    bleep();

    // Spin this core until the power off button is pressed, parsing events and making sounds
    while(status & RUNNING) {
        delay_ms(10);
    }
}

#else

void * alles_start(void *vargs) {
    alles_local_ip = malloc(256);
    alles_local_ip[0] = 0;
    unix_amy_init();
    amy_set_custom(&memorypcm);
    amy_reset_oscs();
    // Schedule a "turning on" sound
    // We don't do this by default on tulip desktop as all threads start at once and it makes the bleep sound bad 
    bleep();
    // Spin this core until the power off button is pressed, parsing events and making sounds
    while(status & RUNNING) {
        delay_ms(10);
    }
    return 0;
}

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

void alles_init_multicast(uint8_t local_node) {
    mesh_local_playback = local_node;
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
    fprintf(stderr, "creating socket to ip %s\n", alles_local_ip);
    create_multicast_ipv4_socket();
    fprintf(stderr, "creating mcast task\n");
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, mcast_listen_task, NULL);
#endif
}



// sync.c -- keep track of mulitple synths
//extern uint8_t battery_mask;
extern uint8_t ipv4_quartet;
extern char githash[8];
int16_t client_id;
int32_t clocks[255];
int32_t ping_times[255];
uint8_t alive = 1;

extern int32_t computed_delta ; // can be negative no prob, but usually host is larger # than client
extern uint8_t computed_delta_set ; // have we set a delta yet?
extern int32_t last_ping_time;

amy_err_t sync_init() {
    client_id = -1; // for now
    for(uint8_t i=0;i<255;i++) { clocks[i] = 0; ping_times[i] = 0; }
    return AMY_OK;
}




void alles_parse_message(char *message, uint16_t length) {
    uint8_t mode = 0;
    int16_t client = -1;
    int32_t sync = -1;
    int8_t sync_index = -1;
    uint8_t ipv4 = 0;
    uint16_t start = 0;
    uint16_t c = 0;
    //char local_message[MAX_RECEIVE_LEN];
    //strncpy(local_message, message, length);
    // Parse the AMY stuff out of the message first
    struct event e = amy_parse_message(message);
    uint8_t sync_response = 0;
    //fprintf(stderr, "message is %s len is %d\n", message, length);
    // Then pull out any alles-specific modes in this message - c,i,r,s, _
    while(c < length+1) {
        uint8_t b = message[c];
        if(b == '_' && c==0) sync_response = 1;
        if( ((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z')) || b == 0) {  // new mode or end
            if(mode=='c') client = atoi(message + start); 
            if(mode=='i') sync_index = atoi(message + start);
            if(mode=='r') ipv4=atoi(message + start);
            if(mode=='s') sync = atol(message + start); 
            mode = b;
            start = c + 1;
        } 
        c++;
    }
    if(sync_response) {
        // If this is a sync response, let's update our local map of who is booted
        //fprintf(stderr, "sync response message was %s\n", message);
        update_map(client, ipv4, sync);
        length = 0; // don't need to do the rest
    }
    // Only do this if we got some data
    if(length >0) {
        // TODO -- not that it matters, but the below could probably be one or two lines long instead
        // Don't add sync messages to the event queue
        if(sync >= 0 && sync_index >= 0) {
            handle_sync(sync, sync_index);
        } else {
            // Assume it's for me
            uint8_t for_me = 1;
            // But wait, they specified, so don't assume
            if(client >= 0) {
                for_me = 0;
                if(client <= 255) {
                    // If they gave an individual client ID check that it exists
                    if(alive>0) { // alive may get to 0 in a bad situation
                        if(client >= alive) {
                            client = client % alive;
                        } 
                    }
                }
                // It's actually precisely for me
                if(client == client_id) for_me = 1;
                if(client > 255) {
                    // It's a group message, see if i'm in the group
                    if(client_id % (client-255) == 0) for_me = 1;
                }
            }
            //fprintf(stderr, "LOG: AMY message for time %lld received at time %lld (latency %lld ms) for_me %d\n", e.time, amy_sysclock(), amy_sysclock()-e.time, for_me);
            if(for_me && mesh_local_playback) amy_add_event(e);
        }
    }
}

void update_map(uint8_t client, uint8_t ipv4, int32_t time) {
    // I'm called when I get a sync response or a regular ping packet
    // I update a map of booted devices.

    //fprintf(stderr,"[%d %d] Got a sync response client %d ipv4 %d time %lld\n",  ipv4_quartet, client_id, client , ipv4, time);
    clocks[ipv4] = time;
    int32_t my_sysclock = amy_sysclock();
    ping_times[ipv4] = my_sysclock;

    // Now I basically see what index I would be in the list of booted synths (clocks[i] > 0)
    // And I set my client_id to that index
    uint8_t last_alive = alive;
    uint8_t my_new_client_id = 255;
    alive = 0;
    for(uint8_t i=0;i<255;i++) {
        if(clocks[i] > 0) { 
            if(my_sysclock < (ping_times[i] + (PING_TIME_MS * 2))) { // alive
                //printf("[%d %d] Checking my time %lld against ipv4 %d's of %lld, client_id now %d ping_time[%d] = %lld\n", 
                //    ipv4_quartet, client_id, my_sysclock, i, clocks[i], my_new_client_id, i, ping_times[i]);
                alive++;
            } else {
                //printf("[ipv4 %d client %d] clock %d is dead, ping time was %lld time now is %lld.\n", ipv4_quartet, client_id, i, ping_times[i], my_sysclock);
                clocks[i] = 0;
                ping_times[i] = 0;
            }
            // If this is not me....
            if(i != ipv4_quartet) {
                // predicted time is what we think the alive node should be at by now
                int32_t predicted_time = (my_sysclock - ping_times[i]) + clocks[i];
                if(my_sysclock >= predicted_time) my_new_client_id--;
            } else {
                my_new_client_id--;
            }
        } else {
            // if clocks[] is 0, no need to check
            my_new_client_id--;
        }
    }
    if(client_id != my_new_client_id || last_alive != alive) {
        fprintf(stderr,"[%d] my client_id is now %d. %d alive\n", ipv4_quartet, my_new_client_id, alive);
        client_id = my_new_client_id;
    }
}

void handle_sync(int32_t time, int8_t index) {
    // I am called when I get an s message, which comes along with host time and index
    int32_t sysclock = amy_sysclock();
    char message[100];
    // Before I send, i want to update the map locally
    //fprintf(stderr, "handle_sync %d %d\n", client_id, ipv4_quartet);
    update_map(client_id, ipv4_quartet, sysclock);
    // Send back sync message with my time and received sync index and my client id & battery status (if any)
    sprintf(message, "_s%" PRIi32 "i%dc%dr%dy%dZ", sysclock, index, client_id, ipv4_quartet, 0);
    //mcast_send(message, strlen(message));
    // Update computed delta (i could average these out, but I don't think that'll help too much)
    //int64_t old_cd = computed_delta;
    computed_delta = time - sysclock;
    computed_delta_set = 1;
    //if(old_cd != computed_delta) printf("Changed computed_delta from %lld to %lld on sync\n", old_cd, computed_delta);
}

void ping(int32_t sysclock) {
    char message[100];
    //printf("[%d %d] pinging with %lld\n", ipv4_quartet, client_id, sysclock);
    sprintf(message, "_s%" PRIi32 "i-1c%dr%dy%dZ", sysclock, client_id, ipv4_quartet, 0);
    //fprintf(stderr, "ping %d %d\n", client_id, ipv4_quartet);

    update_map(client_id, ipv4_quartet, sysclock);
    //mcast_send(message, strlen(message));
    last_ping_time = sysclock;
}

