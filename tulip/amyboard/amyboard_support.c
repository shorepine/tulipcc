#include <stdio.h>
#include <inttypes.h>
#include "amy.h"

// Stuff just for amyboard -- cv in/out direct , i2c in/out

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


#include "pins.h"


TaskHandle_t i2c_check_for_data_handle;


#define AMY_I2C_CHECK_FOR_DATA_TASK_COREID (1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_PRIORITY (ESP_TASK_PRIO_MAX-1)
#define ALLES_I2C_CHECK_FOR_DATA_TASK_NAME "amyboard_i2c"
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

esp_err_t i2c_follower_init() {
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
            amy_add_message((char*)i2c_buffer);
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


#endif // ESP_PLATFORM

extern uint8_t * external_map;
float cv_local_value[2];
uint8_t cv_local_override[2];

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
    if(external_map[osc]==1 || external_map[osc]==2) {
#ifdef ESP_PLATFORM
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
        // silence this output
        return 1;
#endif
        return 0;
    } else if(external_map[osc]>2) { // python audio buffer callback, WIP
        
        return 0;
    } 
    return 0;
}

