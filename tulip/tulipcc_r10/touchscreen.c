
#include "display.h"
#include "touchscreen.h"

// Default for my MaTouch 7" 1024x600
int16_t touch_x_delta = -4;
int16_t touch_y_delta = -6;
float y_scale = 0.8f;

#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_INT -1
#define TOUCH_RST 38
#define I2C_NUM I2C_NUM_0
esp_lcd_touch_handle_t tp;


void touch_init(void)
{
    tp = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;

    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, i2c_conf.mode, 0, 0, 0));

    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_NUM, &io_config, &io_handle));


    esp_lcd_touch_config_t tp_cfg = {
        .x_max = H_RES,
        .y_max = V_RES,
        .rst_gpio_num = TOUCH_RST,
        .int_gpio_num = TOUCH_INT,
        .levels = {
            .reset = 1,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };

    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_gt911(io_handle, &tp_cfg, &tp));
}
uint8_t gt911_held = 0;

void run_gt911(void *param) {
    uint16_t touch_x[3];
    uint16_t touch_y[3];
    uint16_t touch_strength[3];
    uint8_t touch_cnt = 0;
    
    touch_init();
    while (1) {
        esp_lcd_touch_read_data(tp);
        if(esp_lcd_touch_get_coordinates(tp, touch_x, touch_y, touch_strength, &touch_cnt, 3)) {
            //fprintf(stderr, "TP pressed %d,%d str %d count %d\n", touch_x[0], touch_y[0], touch_strength[0], touch_cnt);
            for(uint8_t i=0;i<touch_cnt;i++) {
                last_touch_x[i] = touch_x[i] + touch_x_delta;
                last_touch_y[i] = (touch_y[i] + touch_y_delta)*y_scale;
            }
            //fprintf(stderr, "touch DOWN %d %d\n", last_touch_x[0], last_touch_y[0]);
            send_touch_to_micropython(last_touch_x[0], last_touch_y[0], 0);
            gt911_held = 1;
        } else {
            if(gt911_held) {
                //fprintf(stderr, "touch UP   %d %d\n", last_touch_x[0], last_touch_y[0]);
                send_touch_to_micropython(last_touch_x[0], last_touch_y[0], 1);
                gt911_held = 0;
            }
        }
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}
