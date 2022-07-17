// i2c.c

#include "tulip_i2c.h"



const uint16_t wm8960_reg_defaults[] = {
  0x0, 0x0117,
  0x1, 0x0117,
  0x2, 0x01fd,
  0x3, 0x01fd,
  0x4, 0x0000,
  0x5, 0x0000,
  0x6, 0x0000,
  0x7, 0x0002,
  0x8, 0x01c0,
  0x9, 0x0000,
  0xa, 0x01ff,
  0xb, 0x01ff,

 0x10, 0x0000,
 0x11, 0x007b,
 0x12, 0x0100,
 0x13, 0x0032,
 0x14, 0x0000,
 0x15, 0x00c3,
 0x16, 0x00c3,
 0x17, 0x01c0,
 0x18, 0x0000,
 0x19, 0x0000,
 0x1a, 0x01e0,
 0x1b, 0x0000,
 0x1c, 0x0000,
 0x1d, 0x0000,

 0x20, 0x0100,
 0x21, 0x0100,
 0x22, 0x0150,

 0x25, 0x0150,
 0x26, 0x0000,
 0x27, 0x0000,
 0x28, 0x0000,
 0x29, 0x0000,
 0x2a, 0x0040,
 0x2b, 0x0000,
 0x2c, 0x0000,
 0x2d, 0x0050,
 0x2e, 0x0050,
 0x2f, 0x0000,
 0x30, 0x0002,
 0x31, 0x0037,

 0x33, 0x0080,
 0x34, 0x0008,
 0x35, 0x0031,
 0x36, 0x0026,
 0x37, 0x00e9,
};


void i2c_init()
{
    esp_err_t rc;
    i2c_config_t i2c_conf;
    memset(&i2c_conf, 0x00, sizeof(i2c_conf));
    i2c_conf.mode = I2C_MODE_MASTER;
    i2c_conf.sda_io_num = SDA_PIN;
    i2c_conf.scl_io_num = SCL_PIN;
    //i2c_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    //i2c_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_conf.master.clk_speed = 100 * 1000;
    fprintf(stderr, "Configuring I2C\n");
    rc = i2c_param_config(I2C_BUS_NO, &i2c_conf);
    fprintf(stderr, "I2C Param Config: %s\n", esp_err_to_name(rc));
    rc = i2c_driver_install(I2C_BUS_NO, I2C_MODE_MASTER, 0, 0, 0);
    fprintf(stderr, "I2C Driver Install; %s\n", esp_err_to_name(rc));
}

static esp_err_t i2c_trans(i2c_port_t i2c_num, uint8_t addr, void *txdata, uint8_t txlen)
{
    esp_err_t rc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // ESP_LOGV(TAG, "CMD Handle: %s", esp_err_to_name(rc));
    rc = i2c_master_start(cmd);
    // ESP_LOGV(TAG, "[W] Master_start: %s", esp_err_to_name(rc));
    rc = i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    // ESP_LOGV(TAG, "[W] Master_write_byte: %s", esp_err_to_name(rc));
    rc = i2c_master_write(cmd, txdata, txlen, ACK_CHECK_EN);
    // ESP_LOGV(TAG, "[W] Master_write: %s", esp_err_to_name(rc));
    rc = i2c_master_stop(cmd);
    // ESP_LOGV(TAG, "[W] Master stop: %s", esp_err_to_name(rc));
    rc = i2c_master_cmd_begin(I2C_BUS_NO, cmd, 10);
    // ESP_LOGI(TAG, "[W] CMD begin: %s", esp_err_to_name(rc));
    i2c_cmd_link_delete(cmd);
    return rc;
}

static esp_err_t i2c_recv(i2c_port_t i2c_num, uint8_t addr, void *rxdata, uint8_t rxlen)
{
    esp_err_t rc;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (CODEC_ADDR << 1) | I2C_MASTER_READ, ACK_CHECK_DIS);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1), ACK_CHECK_EN);
    if (rxlen > 1) {
        i2c_master_read(cmd, rxdata, rxlen - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, rxdata + rxlen - 1, NACK_VAL);
    i2c_master_stop(cmd);
    rc = i2c_master_cmd_begin(i2c_num, cmd, 10);
    printf("Recv: %d\n", rc);
    i2c_cmd_link_delete(cmd);
    return rc;
}

static esp_err_t write_register_i2c(uint8_t slave_id, uint32_t reg_addr, uint32_t reg_val)
{
    esp_err_t rc;
    uint8_t buff[3];
    buff[0] = (reg_addr << 1) | ((reg_val >> 8) & 0x0f);
    buff[1] = reg_val & 0xff;
    //printf("%x %x \t %x %x\n", reg_addr, reg_val, buff[0], buff[1]);
    rc = i2c_trans(I2C_BUS_NO, slave_id, buff, 2);
    return rc;
}

static esp_err_t read_register_i2c(uint8_t slave_id, uint8_t reg_addr)
{
    esp_err_t rc;
    uint8_t data = 0;
    rc = i2c_recv(I2C_BUS_NO, reg_addr, &data, 1);
    printf("Read Value: %x\n", data);
    return rc;

    esp_err_t res;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    res  = i2c_master_start(cmd);
    res |= i2c_master_write_byte(cmd, (slave_id << 1) | I2C_MASTER_WRITE, 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_write_byte(cmd, (reg_addr << 1), 1 /*ACK_CHECK_EN*/);
    res |= i2c_master_stop(cmd);
    res |= i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    printf("%d\n", res);
    i2c_cmd_link_delete(cmd);

}

esp_err_t wm8960_init()
{
    esp_err_t ret = 0;
    ret = write_register_i2c(CODEC_ADDR,0x0f, 0x0000);
    if(ret != 0) {
        fprintf(stderr, "ret returned %d\n", ret);
        return ret;
    }  else {
        fprintf(stderr, "WM8960 is reset\n");
    }

    //ret =  write_register_i2c(CODEC_ADDR,0x19, 1<<8 | 1<<7 | 1<<6);
    //ret += write_register_i2c(CODEC_ADDR,0x1A, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3);
    //ret += write_register_i2c(CODEC_ADDR,0x2F, 1<<3 | 1<<2);
    ret = write_register_i2c(CODEC_ADDR, 0x19, 0xFE);
    ret += write_register_i2c(CODEC_ADDR, 0x1A, 0x1F8);
    ret += write_register_i2c(CODEC_ADDR, 0x2F, 0x3C);

    if(ret != 0)  {
        fprintf(stderr,"Source set fail Error code: %d\r\n",ret);
        return ret;
    }


  
  //Configure clock
  //MCLK->div1->SYSCLK->DAC/ADC sample Freq = 25MHz(MCLK)/2*256 = 48.8kHz
  ret = write_register_i2c(CODEC_ADDR,0x04, 0x0000);
  
  //Configure ADC/DAC
  ret += write_register_i2c(CODEC_ADDR,0x05, 0x0000);
  
  //Configure audio interface
  //I2S format 16 bits word length
  ret += write_register_i2c(CODEC_ADDR,0x07, 0x0002);
  
  //Configure HP_L and HP_R OUTPUTS
  ret += write_register_i2c(CODEC_ADDR,0x02, 0x0061 | 0x0100);  //LOUT1 Volume Set
  ret += write_register_i2c(CODEC_ADDR,0x03, 0x0061 | 0x0100);  //ROUT1 Volume Set
  
  //Configure SPK_RP and SPK_RN
  ret += write_register_i2c(CODEC_ADDR,0x28, 0x0077 | 0x0100); //Left Speaker Volume
  ret += write_register_i2c(CODEC_ADDR,0x29, 0x0077 | 0x0100); //Right Speaker Volume
  
  ret += write_register_i2c(CODEC_ADDR,0x31, 0x00F7); //Enable Class D Speaker Outputs

  //Configure DAC volume
  ret += write_register_i2c(CODEC_ADDR,0x0a, 0x00FF | 0x0100);
  ret += write_register_i2c(CODEC_ADDR,0x0b, 0x00FF | 0x0100);
  

  //3D
  //  write_register_i2c(0x10, 0x000F);
  
  //Configure MIXER
  ret += write_register_i2c(CODEC_ADDR,0x22, 1<<8 | 1<<7);
  ret += write_register_i2c(CODEC_ADDR,0x25, 1<<8 | 1<<7);
  
  //Jack Detect
  ret += write_register_i2c(CODEC_ADDR,0x18, 1<<6 | 0<<5);
  ret += write_register_i2c(CODEC_ADDR,0x17, 0x01C3);
  ret += write_register_i2c(CODEC_ADDR,0x30, 0x0009);//0x000D,0x0005
  fprintf(stderr, "ret was %d\n", ret);
  return 0;
}

void wm8960_speaker_on() {
  //Enable the OUTPUTS
  write_register_i2c(CODEC_ADDR,0x31, 0x00F7); //Enable Class D Speaker Outputs
}


void wm8960_set_vol(uint16_t vol, uint8_t module) {
    uint16_t tbl[3][3] = { 
        { _MAX_VOLUME_DAC, _LDAC, 0x100},
        { _MAX_VOLUME_HEADPHONE, _LOUT1, 0x180},
        { _MAX_VOLUME_SPEAKER, _LOUT2, 0x180}};
    int16_t val = (int16_t)(vol * tbl[module][0] / 100 + 0.5) | tbl[module][2];
    int16_t ret;
    ret = write_register_i2c(CODEC_ADDR, tbl[module][1], val);
    ret +=write_register_i2c(CODEC_ADDR, tbl[module][1] + 1, val);

    fprintf(stderr, "ret was %d from wrote %d to reg %04x, module %d vol %d\n", ret, val, tbl[module][1], module, vol);
}

esp_err_t wm8960_set_vol2(uint16_t vol)
{
    esp_err_t ret = 0;
    int vol_to_set = 0;
    if (vol == 0) {
        vol_to_set = 0;
    } else {
        volume = vol;
        vol_to_set = (vol / 10) * 5 + 200;
    }
    ret |= write_register_i2c(CODEC_ADDR, 0xc, 0x100 | vol_to_set);
    ret |= write_register_i2c(CODEC_ADDR, 0xb, 0x100 | vol_to_set);

    return ret;
}


uint16_t wm8960_get_vol()
{
    return volume;
}



void run_i2c(void)
{
    i2c_init();
    wm8960_init();
 
}
