#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_task.h"
#include "driver/i2c.h"

#include "amy.h"
#include "examples.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"

#include "driver/i2s_std.h"

i2s_chan_handle_t tx_handle;
i2s_chan_handle_t rx_handle;

#include "pins.h"

#define I2S_SAMPLE_TYPE I2S_BITS_PER_SAMPLE_32BIT
typedef int32_t i2s_sample_type;



// mutex that locks writes to the delta queue
SemaphoreHandle_t xQueueSemaphore;
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
#define AMYCHIP_ADDR 0x58
#define PCM9211_ADDR 0x40

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



// AMY synth states
extern struct state amy_global;
extern uint32_t event_counter;
extern uint32_t message_counter;

void esp_show_debug(uint8_t t) {

}

// Render the second core
void esp_render_task( void * pvParameters) {
    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        amy_render(0, AMY_OSCS/2, 1);
        xTaskNotifyGive(alles_fill_buffer_handle);
    }
}

extern int16_t amy_in_block[AMY_BLOCK_SIZE * AMY_NCHANS];
i2s_sample_type my_int32_block[AMY_BLOCK_SIZE * AMY_NCHANS];

#include "esp_adc/adc_cali.h"
#include "driver/adc.h"
#include "esp_adc/adc_cali_scheme.h"
adc_cali_handle_t cali_scheme;
uint8_t external_map[AMY_OSCS];
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

    // Figure out which pin / ADC channel we're using
    uint8_t channel_id = ADC_CHANNEL_5;
    if(channel==1) channel_id = ADC_CHANNEL_4;
    
    // Read the ADC once
    int raw = 0;
    int v = 0;
    adc2_get_raw(channel_id,ADC_WIDTH_BIT_12, &raw);
    adc_cali_raw_to_voltage(cali_scheme, raw, &v);

    // Convert the voltage to the calibrated -10 to 10 scale
    float vf = (float)v;

    float lo = 427.0;
    float hi = 3104.0;
    float ret;
    if(vf < lo) {
        ret = -10;
    } else if (vf > hi) {
        ret = 10;
    } else {
        ret = (((vf-lo)/(hi-lo)) *20.0)-10.0;
    }

    // Now smooth the per-AMY block voltage reading to BLOCK_CV_READS 
    last_cv_reads[channel][cv_reads[channel]] = ret;
    float myret = 0;
    for(uint16_t n=0;n<BLOCK_CV_READS;n++) {
        myret += last_cv_reads[channel][n];
    }

    cv_reads[channel] = cv_reads[channel] + 1;
    if(cv_reads[channel] == BLOCK_CV_READS) {
        //if(channel==0) fprintf(stderr, "cv_reads %ld v %d ret %f avg %f\n", cv_reads[channel], v, ret, myret/(float)BLOCK_CV_READS);
        cv_reads[channel] = 0;
    }
    return myret/(float)BLOCK_CV_READS;
}



uint8_t cv_output_hook(uint16_t osc, SAMPLE * buf, uint16_t len) {
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
        i2c_master_write_to_device(I2C_NUM_0, addr, bytes, 3, pdMS_TO_TICKS(10));
        return 1;
    }
    return 0;

}

// Make AMY's FABT run forever , as a FreeRTOS task 
void esp_fill_audio_buffer_task() {
    size_t read = 0;
    size_t written = 0;
    while(1) {
        AMY_PROFILE_START(AMY_ESP_FILL_BUFFER)
        i2s_channel_read(rx_handle, my_int32_block, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS, &read, portMAX_DELAY);
        
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

        i2s_channel_write(tx_handle, my_int32_block, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS, &written, portMAX_DELAY);

        if(written != AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS || read != AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS) {
            fprintf(stderr,"i2s underrun: [w %d,r %d] vs %d\n", written, read, AMY_BLOCK_SIZE * sizeof(i2s_sample_type) * AMY_NCHANS);
        }

    }
}


// init AMY from the esp. wraps some amy funcs in a task to do multicore rendering on the ESP32 
amy_err_t esp_amy_init() {
    i2c_follower_init();

    cv_local_override[0] = 0;
    cv_local_override[1] = 0;
    amy_external_coef_hook = cv_input_hook;
    amy_external_render_hook = cv_output_hook;

    for(uint8_t n=0;n<BLOCK_CV_READS;n++) { last_cv_reads[0][n] = 0; last_cv_reads[1][n] = 0;  }
    adc2_config_channel_atten(ADC_CHANNEL_5, ADC_ATTEN_DB_11);
    adc2_config_channel_atten(ADC_CHANNEL_4, ADC_ATTEN_DB_11);

    #if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_WIDTH_BIT_12,
    };
    adc_cali_create_scheme_curve_fitting(&cali_config, &cali_scheme);
    #else
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_WIDTH_BIT_12,
    };
    adc_cali_create_scheme_line_fitting(&cali_config, &cali_scheme);
    #endif
    
    amy_start(2, 1, 1, 1);
    // We create a mutex for changing the event queue and pointers as two tasks do it at once
    xQueueSemaphore = xSemaphoreCreateMutex();

    // Create the second core rendering task
    xTaskCreatePinnedToCore(&esp_render_task, ALLES_RENDER_TASK_NAME, ALLES_RENDER_TASK_STACK_SIZE, NULL, ALLES_RENDER_TASK_PRIORITY, &amy_render_handle, ALLES_RENDER_TASK_COREID);

    // Wait for the render tasks to get going before starting the i2s task
    delay_ms(100);

    // And the fill audio buffer thread, combines, does volume & filters
    xTaskCreatePinnedToCore(&esp_fill_audio_buffer_task, ALLES_FILL_BUFFER_TASK_NAME, ALLES_FILL_BUFFER_TASK_STACK_SIZE, NULL, ALLES_FILL_BUFFER_TASK_PRIORITY, &alles_fill_buffer_handle, ALLES_FILL_BUFFER_TASK_COREID);

    // i2c listener task
    xTaskCreatePinnedToCore(&i2c_check_for_data, ALLES_I2C_CHECK_FOR_DATA_TASK_NAME, ALLES_I2C_CHECK_FOR_DATA_TASK_STACK_SIZE, NULL, ALLES_I2C_CHECK_FOR_DATA_TASK_PRIORITY, &i2c_check_for_data_handle, ALLES_I2C_CHECK_FOR_DATA_TASK_COREID);

    return AMY_OK;
}


// Setup I2S
amy_err_t setup_i2s(void) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_SLAVE);  // ************* I2S_ROLE_SLAVE - needs external I2S clock input.
    //fprintf(stderr, "creating I2S channel\n");
    i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle);

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
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
    i2s_channel_init_std_mode(rx_handle, &std_cfg);

    /* Before writing data, start the TX channel first */
    i2s_channel_enable(tx_handle);
    i2s_channel_enable(rx_handle);
    return AMY_OK;
}

void start_amyboard_amy() {
    check_init(&setup_i2s, "i2s");
    esp_amy_init();
    amy_reset_oscs();
}

