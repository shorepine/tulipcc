#include "tdeck_display.h"
#include "tasks.h"


uint8_t dumb_display_mutex = 0;

// Unused by TDeck stubs
void esp32s3_display_stop() {
}

void esp32s3_display_start() {
}

void esp32s3_display_restart() {
}


void esp_display_set_clock(uint8_t mhz) {  
}


// TODO -- the trackball also responds to joy checks
uint16_t check_joy() {
    return 0;
}

void display_pwm_setup() {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_1,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = 300,  // Set output frequency at 300Hz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_1,
        .timer_sel      = LEDC_TIMER_1,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = TDECK_LCD_BK_LIGHT_GPIO,
        .duty           = 4095, // Set duty to 50%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}


void display_brightness(uint8_t amount) {
    if(amount < 1) amount = 1;
    if(amount > 9) amount = 9;
    brightness = amount;
    uint16_t duty = (9-brightness)*1000;
    fprintf(stderr, "setting duty to %d\n", duty);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1) ;
}

// interrupt called when the SPI transmission is complete. we use this to tell the display code
// it is ok to start writing to our framebuffer RAM again. without this setup you'll get corruption in drawing
static bool tdeck_trans_done(esp_lcd_panel_io_handle_t panel_io,
                            esp_lcd_panel_io_event_data_t* edata,
                            void* user_ctx) {
    BaseType_t high_task_wakeup;
    vTaskNotifyGiveFromISR(display_handle, &high_task_wakeup);
    return high_task_wakeup;
}

// Task runner for the display, inits and then spins in a loop processing 
void run_tdeck_display(void) {
    // Only one res for the TDeck

    // First set up the memory
    display_init();

    // Make an ESP_LCD object
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;

    // turn off backlight
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << TDECK_LCD_BK_LIGHT_GPIO
    };
    gpio_config(&bk_gpio_config);
    gpio_set_level(TDECK_LCD_BK_LIGHT_GPIO, 0);

    // turn on TDeck peripheral 
    gpio_config_t peri_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << TDECK_PERI_GPIO
    };
    gpio_config(&peri_gpio_config);
    gpio_set_level(TDECK_PERI_GPIO, 1);

    // set up SPI
    spi_bus_config_t buscfg = {
        .sclk_io_num = TDECK_LCD_PCLK_GPIO,
        .mosi_io_num = TDECK_LCD_MOSI_GPIO,
        .miso_io_num = TDECK_LCD_MISO_GPIO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };
    spi_bus_initialize(TDECK_SPI_HOST_ID, &buscfg, SPI_DMA_CH_AUTO);
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = TDECK_LCD_DC_GPIO,
        .cs_gpio_num = TDECK_LCD_CS_GPIO,
        .pclk_hz = TDECK_LCD_PIXEL_CLOCK_HZ,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8, 
        .lcd_param_bits = 8, 
        .on_color_trans_done = tdeck_trans_done,
        .user_ctx = NULL,
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TDECK_LCD_RST_GPIO,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)TDECK_SPI_HOST_ID, &io_config, &io_handle);
    esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle);
    esp_lcd_panel_swap_xy(panel_handle, true);
    esp_lcd_panel_mirror(panel_handle, true, false);
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_invert_color(panel_handle, true);
    esp_lcd_panel_disp_on_off(panel_handle, true);
    gpio_set_level(TDECK_LCD_BK_LIGHT_GPIO, 1);
    
    // If you don't clear the screen it'll still show what was left there after reboots. 
    uint8_t * clear = malloc_caps(H_RES*V_RES*2, MALLOC_CAP_DMA);
    memset(clear, 0, H_RES*V_RES*2 );
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, H_RES, V_RES, clear);
    free(clear);

    brightness = DEFAULT_BRIGHTNESS;

    // Time the frame sync and stay running forever 
    int64_t tic0 = esp_timer_get_time();
    uint16_t loop_count =0;
    uint8_t *frame_bb =(uint8_t*) malloc_caps(H_RES*FONT_HEIGHT, MALLOC_CAP_DMA);
    uint16_t *bb565 = (uint16_t*) malloc_caps(H_RES*FONT_HEIGHT*2, MALLOC_CAP_DMA);
    while(1)  { 
        if(loop_count++ >= 100) {
            float reported_fps = 1000000.0 / ((esp_timer_get_time() - tic0) / loop_count);
            if(gpu_log)fprintf(stderr,"past %d frames %2.2f FPS.\n", loop_count, reported_fps);
            loop_count = 0;
            tic0 = esp_timer_get_time();
        }        

        // bounce the entire screen at once to the display
        for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
            // Get the pixel data for a row of screen from Tulip
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT, NULL);
            // Wait for the last write to be done before starting
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
            // Write it to the TDeck screen via colorspace conversion
            for(uint16_t i=0;i<H_RES*FONT_HEIGHT;i++) {
                bb565[i] = rgb332_rgb565_i[frame_bb[i]];
            }
            dumb_display_mutex = 1;
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, H_RES, y+FONT_HEIGHT, bb565);
            dumb_display_mutex = 0;
        }
        // Tell Tulip we're done with the frame
        display_frame_done_generic();

    }


}

