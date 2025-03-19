#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_task.h"
//#include "driver/i2c_master.h"

#include "amy.h"
#include "examples.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"

#include "driver/i2s_std.h"

static const char *TAG = "amy-chip";

i2s_chan_handle_t tx_handle;
i2s_chan_handle_t rx_handle;

#include "pins.h"

#define I2S_SAMPLE_TYPE I2S_BITS_PER_SAMPLE_32BIT
typedef int32_t i2s_sample_type;



// mutex that locks writes to the delta queue
SemaphoreHandle_t xQueueSemaphore;
void delay_ms(uint32_t);
/*
void delay_ms(uint32_t ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
*/
// Task handles for the renderers, multicast listener and main
TaskHandle_t amy_render_handle;
TaskHandle_t alles_fill_buffer_handle;


#define ALLES_TASK_COREID (1)
#define ALLES_RENDER_TASK_COREID (0)
#define ALLES_FILL_BUFFER_TASK_COREID (1)
#define ALLES_RENDER_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1 )
#define ALLES_FILL_BUFFER_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1)
#define ALLES_TASK_NAME             "alles_task"
#define ALLES_RENDER_TASK_NAME      "alles_r_task"
#define ALLES_FILL_BUFFER_TASK_NAME "alles_fb_task"
#define ALLES_TASK_STACK_SIZE    (8 * 1024) 
#define ALLES_RENDER_TASK_STACK_SIZE (8 * 1024)
#define ALLES_FILL_BUFFER_TASK_STACK_SIZE (8 * 1024)


// i2c stuff
#define I2C_CLK_FREQ 400000
//#include "esp32-hal-i2c-slave.h"
#define DATA_LENGTH 255
#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_SLAVE_NUM I2C_NUMBER(1) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */
#define AMYCHIP_ADDR 0x58
#define PCM9211_ADDR 0x40
#define I2C_MASTER_NUM I2C_NUMBER(0) /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0  

//i2c_master_bus_handle_t tool_bus_handle;
//i2c_master_dev_handle_t pcm9211_handle;
#define I2C_TOOL_TIMEOUT_VALUE_MS (500)


// Commenting out all the i2c code here because it conflicts with Micropython's i2c driver.
// So instead, we can set up PCM9211 over micropython i2c





//esp_err_t i2c_master_write(uint8_t device_addr, uint8_t *data_wr, size_t size_wr);
/*
esp_err_t i2c_master_write(uint8_t device_addr, uint8_t *data_wr, size_t size_wr) {
    esp_err_t ret = i2c_master_transmit(pcm9211_handle, data_wr, size_wr, I2C_TOOL_TIMEOUT_VALUE_MS);
    if (ret == ESP_OK) {
        //ESP_LOGI(TAG, "Write OK");
    } else if (ret == ESP_ERR_TIMEOUT) {
        ESP_LOGW(TAG, "Bus is busy");
    } else {
        ESP_LOGW(TAG, "Write Failed");
    }
    return 0;
}
*/

/*
uint8_t pcm9211_readRegister(uint8_t reg) {
    uint8_t buf[1] = {reg};
    uint8_t buffer[1] = { 0};
    ESP_ERROR_CHECK(i2c_master_transmit_receive(pcm9211_handle, buf, 1, buffer, 1, I2C_TOOL_TIMEOUT_VALUE_MS));
    return(buffer[0]);
}

void pcm9211_writeRegister(uint8_t reg, uint16_t value) {
  uint8_t data[2];
  data[0] = reg; 
  data[1] = value; 
  if (i2c_master_write(PCM9211_ADDR, data, 2))
      fprintf(stderr, "bad write to pcm9211\n");
}

uint8_t pcm9211_registers[10][2] = {
    { 0x40, 0x33 }, // Power down ADC, power down DIR, power down DIT, power down OSC
    { 0x40, 0xc0 }, // Normal operation for all
    { 0x34, 0x00 }, // Initialize DIR - both biphase amps on, input from RXIN0
    { 0x26, 0x01 }, // Main Out is DIR/ADC if no DIR sync (these match power-on default, repeated for clarity).
    { 0x6B, 0x00 }, // Main output pins are DIR/ADC AUTO
    { 0x30, 0x04 }, // PLL sends 512fs as SCK
    { 0x31, 0x0A }, // XTI SCK as 512fs too
    { 0x60, 0x44 }, // Initialize PCM9211 DIT to send SPDIF from AUXIN1 through MPO0 (pin15).  MPO1 (pin16) is VOUT (Valid)
    { 0x78, 0x3d }, // MPO0 = 0b1101 = TXOUT, MPO1 = 0b0011 = VOUT
    { 0x6F, 0x40 }  // MPIO_A = CLKST etc / MPIO_B = AUXIN2 / MPIO_C = AUXIN1
};

esp_err_t setup_pcm9211(void) {
    for(uint8_t i=0;i<10; i++) {
        delay_ms(100);
        fprintf(stderr, "[pcm9211] setting register 0x%02x to 0x%02x: ", pcm9211_registers[i][0], pcm9211_registers[i][1]);
        pcm9211_writeRegister(pcm9211_registers[i][0], pcm9211_registers[i][1]);
        delay_ms(100);
        uint8_t read_back = pcm9211_readRegister(pcm9211_registers[i][0]);
        if(read_back == pcm9211_registers[i][1]) {
            fprintf(stderr, "success\n");
        } else {
            fprintf(stderr, "failed?: read back is 0x%02x\n", read_back);
        }
    }
    return ESP_OK;
}

esp_err_t i2c_master_init(void) {
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_io_num = I2C_MASTER_SDA,
        .flags.enable_internal_pullup = true,
    };

    if (i2c_new_master_bus(&i2c_bus_config, &tool_bus_handle) != ESP_OK) {
        return 1;
    }
    i2c_device_config_t i2c_dev_conf = {
        .scl_speed_hz = I2C_CLK_FREQ,
        .device_address = PCM9211_ADDR, 
        .flags.disable_ack_check = true,
    };
    if (i2c_master_bus_add_device(tool_bus_handle, &i2c_dev_conf, &pcm9211_handle) != ESP_OK) {
        return 1;
    }
    return ESP_OK;
}



void i2c_slave_request_cb(uint8_t num, uint8_t *cmd, uint8_t cmd_len, void * arg) {
    if (cmd_len > 0) {
        // first write to master
        i2cSlaveWrite(I2C_SLAVE_NUM, cmd, cmd_len, 0);
    } else {
        // cmd_len == 0 means master want more data from slave
        // we just send one byte 0 each time to master here
        uint8_t extra_data = 0x00;
        i2cSlaveWrite(I2C_SLAVE_NUM, &extra_data, 1, 0);
    }
}

void i2c_slave_receive_cb(uint8_t num, uint8_t * data, size_t len, bool stop, void * arg) {
    if (len > 0) {
        data[len]= 0;
        amy_play_message((char*)data);
    }
}

esp_err_t i2c_slave_init(void) {
    i2cSlaveAttachCallbacks(I2C_SLAVE_NUM, i2c_slave_request_cb, i2c_slave_receive_cb, NULL);
    return i2cSlaveInit(I2C_SLAVE_NUM, I2C_SLAVE_SDA, I2C_SLAVE_SCL, AMYCHIP_ADDR, I2C_CLK_FREQ, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN);
}


*/



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

float cv_input_hook(uint16_t channel) {
    /*
    pin = 15 if channel==1 else 16
    pot = ADC(Pin(pin))
    pot.atten(ADC.ATTN_11DB) 

    # Read it n times to smooth it
    x = 0
    for i in range(n):
        x = x + pot.read_uv()
    x = (x / (float(n))) 

    # measured uV with a CV pot module that goes from -5.4V to +5.4V (10vpp)
    (lo, hi) = 755000,2568000
    rge = hi-lo
    return (((x-lo) / rge) * 10.0) - 5.0
    */
    if(cv_local_override[channel]) {
        return cv_local_value[channel];
    }
    uint8_t channel_id = ADC_CHANNEL_5;
    if(channel==1) channel_id = ADC_CHANNEL_4;
    int raw = 0;
    int uv = 0;
    adc2_get_raw(channel_id,ADC_WIDTH_BIT_12, &raw);
    adc_cali_raw_to_voltage(cali_scheme, raw, &uv);
    // latest curve was 427000 is -10, 3044200 is 9.5
    float uvf = (float)uv;
    float lo = 427000.0;
    float hi = 3104200.0;
    if(uvf < lo) return -1;
    if(uvf > hi) return 1;
    return (((uvf-lo)/(hi-lo)) *10.0)-5.0;
}


#include "driver/i2c.h"

// An AMY hook to send values out to a CV DAC over i2c, only on ESP 
/*
    """Output -10.0v to +10.0v (nominal) on CV1 (channel=0) or 2 (channel=1)"""
    addr = 88 # GP8413
    # With rev1 scaling, 0x0000 -> -10v, 0x7fff -> +10v
    val = int(((volts + 10)/20.0) * 0x8000)
    if(val < 0):
        val = 0
    if(val > 0x7fff):
        val = 0x7fff
    b1 = (val & 0xff00) >> 8
    b0 = (val & 0x00ff)
    ch = 0x02
    if(channel == 1):
        ch = 0x04
    get_i2c().writeto_mem(addr, ch, bytes([b0,b1]))
*/

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
    cv_local_override[0] = 0;
    cv_local_override[1] = 0;
    amy_external_coef_hook = cv_input_hook;
    amy_external_render_hook = cv_output_hook;
    //{{&machine_adc_type}, ADCBLOCK2, ADC_CHANNEL_4, GPIO_NUM_15},
    //{{&machine_adc_type}, ADCBLOCK2, ADC_CHANNEL_5, GPIO_NUM_16},
        
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

    return AMY_OK;
}


// Setup I2S
amy_err_t setup_i2s(void) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_SLAVE);  // ************* I2S_ROLE_SLAVE - needs external I2S clock input.
    fprintf(stderr, "creating I2S channel\n");
    i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle);

     i2s_std_config_t std_cfg = {
        .clk_cfg = {
            .sample_rate_hz = 48000,
            .clk_src = I2S_CLK_SRC_EXTERNAL,
            .ext_clk_freq_hz = 48000 * 512,
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
    fprintf(stderr, "initializing I2S TX channel\n");
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
    fprintf(stderr, "initializing I2S RX channel\n");
    i2s_channel_init_std_mode(rx_handle, &std_cfg);

    /* Before writing data, start the TX channel first */
    fprintf(stderr, "enabling I2S TX channel\n");
    i2s_channel_enable(tx_handle);
    fprintf(stderr, "enabling I2S RX channel\n");
    i2s_channel_enable(rx_handle);
    return AMY_OK;
}

void start_amyboard_amy() {
    fprintf(stderr, "attempting to start I2S\n");
    check_init(&setup_i2s, "i2s");
    fprintf(stderr, "initializing AMY\n");
    esp_amy_init();
    amy_reset_oscs();
    fprintf(stderr, "done initializing AMY\n");
}

