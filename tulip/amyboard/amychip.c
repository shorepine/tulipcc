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
    i2s_channel_init_std_mode(tx_handle, &std_cfg);
    i2s_channel_init_std_mode(rx_handle, &std_cfg);

    /* Before writing data, start the TX channel first */
    i2s_channel_enable(tx_handle);
    i2s_channel_enable(rx_handle);
    return AMY_OK;
}


#if 0

void app_main(void)
{
    printf("Welcome to the AMY chip implementation!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    check_init(&i2c_master_init, "i2c_master");
    check_init(&i2c_slave_init, "i2c_slave");
    check_init(&setup_pcm9211, "pcm9211");

    delay_ms(1000);
    

    // make this 1 if you want to actually turn on i2s... it is currently hanging as it can't find MCLK on the pin....
    if(0) {
        check_init(&setup_i2s, "i2s");
        esp_amy_init();
        amy_reset_oscs();

        struct event e = amy_default_event();
        e.time = amy_sysclock();
        e.freq_coefs[0] = 440;
        e.wave = SINE;
        e.osc = 0;
        e.velocity = 1;
        amy_add_event(e);
        //example_voice_chord(amy_sysclock(), 0);
    }
    while(1) {
        delay_ms(1000);
            fprintf(stderr, "register 0x39 is 0x%02x\n", pcm9211_readRegister(0x39));


    }
}
#endif
