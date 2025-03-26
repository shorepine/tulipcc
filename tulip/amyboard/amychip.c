#include <stdio.h>
#include <inttypes.h>
#include "amy.h"



#ifdef ESP_PLATFORM
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_task.h"
#include "driver/i2c.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2s_std.h"


i2s_chan_handle_t amyboard_tx_handle;
i2s_chan_handle_t amyboard_rx_handle;

#include "pins.h"

#define I2S_SAMPLE_TYPE I2S_BITS_PER_SAMPLE_32BIT
typedef int32_t i2s_sample_type;



// mutex that locks writes to the delta queue
extern SemaphoreHandle_t xQueueSemaphore;
void delay_ms(uint32_t);

// Task handles for the renderers, multicast listener and main
TaskHandle_t amy_render_handle;
TaskHandle_t alles_fill_buffer_handle;
TaskHandle_t i2c_check_for_data_handle;


#define ALLES_TASK_COREID (1)
#define ALLES_RENDER_TASK_COREID (0)
#define ALLES_FILL_BUFFER_TASK_COREID (1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_COREID (1)

#define ALLES_RENDER_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1 )
#define ALLES_FILL_BUFFER_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1)

#define ALLES_TASK_NAME             "alles_task"
#define ALLES_RENDER_TASK_NAME      "alles_r_task"
#define ALLES_FILL_BUFFER_TASK_NAME "alles_fb_task"
#define ALLES_I2C_CHECK_FOR_DATA_TASK_NAME "alles_i2c"

#define ALLES_TASK_STACK_SIZE    (8 * 1024) 
#define ALLES_RENDER_TASK_STACK_SIZE (8 * 1024)
#define ALLES_FILL_BUFFER_TASK_STACK_SIZE (8 * 1024)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_STACK_SIZE (16 * 1024)


// i2c stuff
#define I2C_CLK_FREQ 400000
#define DATA_LENGTH MAX_MESSAGE_LEN
#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_FOLLOWER_NUM I2C_NUMBER(1) /*!< I2C port number for follower dev */
#define I2C_FOLLOWER_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C follower tx buffer size */
#define I2C_FOLLOWER_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C follower rx buffer size */

#define AMYCHIP_ADDR 0x3F
#define PCM9211_ADDR 0x40
#define ADS1115_ADDR 0x48
#define GP8413_ADDR  0x58

static esp_err_t i2c_follower_init() {
    i2c_port_t i2c_follower_port = I2C_FOLLOWER_NUM;
    i2c_config_t conf_follower;
    conf_follower.sda_io_num = I2C_FOLLOWER_SDA;
    conf_follower.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_follower.scl_io_num = I2C_FOLLOWER_SCL;
    conf_follower.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_follower.mode = I2C_MODE_SLAVE;
    conf_follower.slave.addr_10bit_en = 0;
    conf_follower.slave.slave_addr = AMYCHIP_ADDR;
    conf_follower.slave.maximum_speed = I2C_CLK_FREQ; // expected maximum clock speed
    conf_follower.clk_flags =0;
    i2c_param_config(i2c_follower_port, &conf_follower);
    return i2c_driver_install(i2c_follower_port, conf_follower.mode,
                            I2C_FOLLOWER_RX_BUF_LEN, I2C_FOLLOWER_TX_BUF_LEN, 0);
}


uint8_t i2c_buffer[MAX_MESSAGE_LEN];

void i2c_check_for_data() {
    while(1) {
        size_t size = i2c_slave_read_buffer(I2C_FOLLOWER_NUM, i2c_buffer, MAX_MESSAGE_LEN, 10 / portTICK_PERIOD_MS);
        if(size>0) {
            i2c_buffer[size] = 0;
            //fprintf(stderr, "%s\n", i2c_buffer);
            amy_play_message((char*)i2c_buffer);
        }
    }
}



#define ADS1115_REGISTER_CONFIG (0x01)
#define ADS1115_CQUE_NONE (0x0003)
#define ADS1115_CLAT_NONLAT (0x0000)
#define ADS1115_CPOL_ACTVLOW (0x0000)
#define ADS1115_CMODE_TRAD (0x0000)
#define ADS1115_DR_1600SPS (0x0080)
#define ADS1115_MODE_SINGLE (0x0100)
#define ADS1115_OS_SINGLE (0x8000)
#define ADS1115_PGA_2_048V (0x0400)
#define ADS1115_PGA_4_096V (0x0200)
#define ADS1115_MUX_SINGLE_0 (0x4000) 
#define ADS1115_MUX_SINGLE_1 (0x5000) 
#define ADS1115_MUX_SINGLE_2 (0x6000) 
#define ADS1115_MUX_SINGLE_3 (0x7000) 
#define ADS1115_REGISTER_CONVERT (0x00)

static esp_err_t ads1115_write_register(uint8_t reg, uint16_t data) {
  i2c_cmd_handle_t cmd;
  esp_err_t ret;
  uint8_t out[2];

  out[0] = data >> 8; // get 8 greater bits
  out[1] = data & 0xFF; // get 8 lower bits
  cmd = i2c_cmd_link_create();
  i2c_master_start(cmd); // generate a start command
  i2c_master_write_byte(cmd,(ADS1115_ADDR<<1) | I2C_MASTER_WRITE,1); // specify address and write command
  i2c_master_write_byte(cmd,reg,1); // specify register
  i2c_master_write(cmd,out,2,1); // write it
  i2c_master_stop(cmd); // generate a stop command
  ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10)); // send the i2c command
  i2c_cmd_link_delete(cmd);
  return ret;
}

static esp_err_t ads1115_read_register(uint8_t reg, uint8_t* data, uint8_t len) {
    i2c_cmd_handle_t cmd;
    esp_err_t ret;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd,(ADS1115_ADDR<<1) | I2C_MASTER_WRITE,1);
    i2c_master_write_byte(cmd,reg,1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10));
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd); // generate start command
    i2c_master_write_byte(cmd,(ADS1115_ADDR<<1) | I2C_MASTER_READ,1); // specify address and read command
    i2c_master_read(cmd, data, len, 0); // read all wanted data
    i2c_master_stop(cmd); // generate stop command
    ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(10)); // send the i2c command
    i2c_cmd_link_delete(cmd);
    return ret;
}

uint16_t read_ads1115_raw(uint8_t channel) {
    uint16_t data = 0;
    uint16_t channel_mux = ADS1115_MUX_SINGLE_0;
    if(channel==1) channel_mux = ADS1115_MUX_SINGLE_1;
    if(channel==2) channel_mux = ADS1115_MUX_SINGLE_2;
    if(channel==3) channel_mux = ADS1115_MUX_SINGLE_3;
    data =  (ADS1115_CQUE_NONE |ADS1115_CLAT_NONLAT |
             ADS1115_CPOL_ACTVLOW | ADS1115_CMODE_TRAD | ADS1115_DR_1600SPS |
             ADS1115_MODE_SINGLE | ADS1115_OS_SINGLE | ADS1115_PGA_4_096V |
             channel_mux);
    ads1115_write_register(ADS1115_REGISTER_CONFIG, data);
    uint8_t output[2];
    ads1115_read_register(ADS1115_REGISTER_CONVERT, output, 2);
    return ((uint16_t)output[0] << 8) | (uint16_t)output[1];
}


extern int16_t amy_in_block[AMY_BLOCK_SIZE * AMY_NCHANS];
i2s_sample_type my_int32_block[AMY_BLOCK_SIZE * AMY_NCHANS];

#endif // ESP_PLATFORM

extern uint8_t external_map[AMY_OSCS];
float cv_local_value[2];
uint8_t cv_local_override[2];

#define BLOCK_CV_READS 100
float last_cv_reads[2][BLOCK_CV_READS];
uint32_t cv_reads[2] = {0,0};

float cv_input_hook(uint16_t channel) {

    // If they used cv_local, just return that, skip everything else
    if(cv_local_override[channel]) {
        return cv_local_value[channel];
    }
    // Read the ADS1115
    #ifdef ESP_PLATFORM
    uint16_t raw = read_ads1115_raw(channel);
    uint16_t min = 1058; // -10V
    uint16_t max = 21312; // 10V
    float v = ((((float)raw - (float)min)/((float)max-(float)min))*20.0)-10.0;
    return v;
    #else
    return 0;
    #endif
}


// Write to the GP8413
uint8_t cv_output_hook(uint16_t osc, SAMPLE * buf, uint16_t len) {
#ifdef ESP_PLATFORM
    if(external_map[osc]>0) {
        // -5v to +5v? 
        float volts = S2F(buf[0])*5.0;
        int32_t val = (int32_t)(((volts + 10)/20.0) * 0x8000);
        if(val < 0) val = 0;
        if(val > 0x7fff) val = 0x7fff;
        uint8_t bytes[3];
        bytes[2] = (val & 0xff00) >> 8;
        bytes[1] = (val & 0x00ff);
        uint8_t ch = 0x02;
        uint8_t addr = 88;
        uint8_t channel = external_map[osc]-1;
        if(channel == 1) ch = 0x04;
        bytes[0] = ch;
        //fprintf(stderr, "writing %f volts [%ld] to channel %d\n", volts, val, channel);
        i2c_master_write_to_device(I2C_NUM_0, addr, bytes, 3, pdMS_TO_TICKS(10));
        return 1;
    }
#endif
    return 0;

}

#ifdef ESP_PLATFORM
// Make AMY's FABT run forever , as a FreeRTOS task 
void amyboard_fill_audio_buffer_task() {
    size_t read = 0;
    size_t written = 0;
    while(1) {
        AMY_PROFILE_START(AMY_ESP_FILL_BUFFER)
        i2s_channel_read(amyboard_rx_handle, my_int32_block, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS, &read, portMAX_DELAY);
        
        for (int i = 0; i < AMY_BLOCK_SIZE * AMY_NCHANS; ++i)
            amy_in_block[i] = (i2s_sample_type)(my_int32_block[i] >> 16);

        
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
        for (int i = 0; i < AMY_BLOCK_SIZE * AMY_NCHANS; ++i)
            my_int32_block[i] = ((i2s_sample_type)block[i]) << 16;


        AMY_PROFILE_STOP(AMY_ESP_FILL_BUFFER)

        i2s_channel_write(amyboard_tx_handle, my_int32_block, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS, &written, portMAX_DELAY);

        if(written != AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS || read != AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS) {
            fprintf(stderr,"i2s underrun: [w %d,r %d] vs %d\n", written, read, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS);
        }

    }
}
extern void esp_render_task( void * pvParameters);

// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 
amy_err_t amyboard_amy_init() {
    i2c_follower_init();

    cv_local_override[0] = 0;
    cv_local_override[1] = 0;
    amy_external_coef_hook = cv_input_hook;
    amy_external_render_hook = cv_output_hook;

    amy_start(2, 1, 1, 1);
    // We create a mutex for changing the event queue and pointers as two tasks do it at once
    xQueueSemaphore = xSemaphoreCreateMutex();

    // Create the second core rendering task
    xTaskCreatePinnedToCore(&esp_render_task, ALLES_RENDER_TASK_NAME, ALLES_RENDER_TASK_STACK_SIZE, NULL, ALLES_RENDER_TASK_PRIORITY, &amy_render_handle, ALLES_RENDER_TASK_COREID);

    // Wait for the render tasks to get going before starting the i2s task
    delay_ms(100);

    // And the fill audio buffer thread, combines, does volume & filters
    xTaskCreatePinnedToCore(&amyboard_fill_audio_buffer_task, ALLES_FILL_BUFFER_TASK_NAME, ALLES_FILL_BUFFER_TASK_STACK_SIZE, NULL, ALLES_FILL_BUFFER_TASK_PRIORITY, &alles_fill_buffer_handle, ALLES_FILL_BUFFER_TASK_COREID);

    // i2c listener task
    xTaskCreatePinnedToCore(&i2c_check_for_data, ALLES_I2C_CHECK_FOR_DATA_TASK_NAME, ALLES_I2C_CHECK_FOR_DATA_TASK_STACK_SIZE, NULL, ALLES_I2C_CHECK_FOR_DATA_TASK_PRIORITY, &i2c_check_for_data_handle, ALLES_I2C_CHECK_FOR_DATA_TASK_COREID);

    return AMY_OK;
}


// Setup I2S
amy_err_t amyboard_setup_i2s(void) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_SLAVE);  // ************* I2S_ROLE_SLAVE - needs external I2S clock input.
    //fprintf(stderr, "creating I2S channel\n");
    i2s_new_channel(&chan_cfg, &amyboard_tx_handle, &amyboard_rx_handle);

     i2s_std_config_t std_cfg = {
        .clk_cfg = {
            .sample_rate_hz = AMY_SAMPLE_RATE,
            .clk_src = I2S_CLK_SRC_EXTERNAL,
            .ext_clk_freq_hz = AMY_SAMPLE_RATE * 512,
            .mclk_multiple = 512, 
        },
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_32BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT,
            .slot_mode = I2S_SLOT_MODE_STEREO,
            .slot_mask = I2S_STD_SLOT_BOTH,
            .ws_width = 32,
            .ws_pol = false, // false in STD_PHILIPS macro
            .bit_shift = false, // true for STD_PHILIPS macro, but that results in *2* bits delay of dout vs lrclk in Follower mode. false gives 1 bit delay, as expected for i2s.
            .left_align = false,
            .big_endian = false,
            .bit_order_lsb = false,
        },
        .gpio_cfg = {
            .mclk = I2S_MCLK, 
            .bclk = I2S_BCLK,
            .ws = I2S_LRCLK,
            .dout = AMYOUT,
            .din = AMYIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = true, // invert bclk for pcm9211 
                .ws_inv = false,
            },
        },
    };
    /* Initialize the channel */
    i2s_channel_init_std_mode(amyboard_tx_handle, &std_cfg);
    i2s_channel_init_std_mode(amyboard_rx_handle, &std_cfg);

    /* Before writing data, start the TX channel first */
    i2s_channel_enable(amyboard_tx_handle);
    i2s_channel_enable(amyboard_rx_handle);
    return AMY_OK;
}

void start_amyboard_amy() {
    check_init(&amyboard_setup_i2s, "i2s");
    amyboard_amy_init();
    amy_reset_oscs();
}
#endif // ESP_PLATFORM
