#include "tdeck_display.h"
#include "tasks.h"


uint8_t dumb_display_mutex = 0;

// lookup table for Tulip's "pallete" to the 16-bit colorspace needed by the TDeck display
const uint16_t rgb332_rgb565_i[256] = {
    0x0000, 0x0a00, 0x1500, 0x1f00, 0x2001, 0x2a01, 0x3501, 0x3f01, 
    0x4002, 0x4a02, 0x5502, 0x5f02, 0x6003, 0x6a03, 0x7503, 0x7f03, 
    0x8004, 0x8a04, 0x9504, 0x9f04, 0xa005, 0xaa05, 0xb505, 0xbf05, 
    0xc006, 0xca06, 0xd506, 0xdf06, 0xe007, 0xea07, 0xf507, 0xff07, 
    0x0020, 0x0a20, 0x1520, 0x1f20, 0x2021, 0x2a21, 0x3521, 0x3f21, 
    0x4022, 0x4a22, 0x5522, 0x5f22, 0x6023, 0x6a23, 0x7523, 0x7f23, 
    0x8024, 0x8a24, 0x9524, 0x9f24, 0xa025, 0xaa25, 0xb525, 0xbf25, 
    0xc026, 0xca26, 0xd526, 0xdf26, 0xe027, 0xea27, 0xf527, 0xff27, 
    0x0048, 0x0a48, 0x1548, 0x1f48, 0x2049, 0x2a49, 0x3549, 0x3f49, 
    0x404a, 0x4a4a, 0x554a, 0x5f4a, 0x604b, 0x6a4b, 0x754b, 0x7f4b, 
    0x804c, 0x8a4c, 0x954c, 0x9f4c, 0xa04d, 0xaa4d, 0xb54d, 0xbf4d, 
    0xc04e, 0xca4e, 0xd54e, 0xdf4e, 0xe04f, 0xea4f, 0xf54f, 0xff4f, 
    0x0068, 0x0a68, 0x1568, 0x1f68, 0x2069, 0x2a69, 0x3569, 0x3f69, 
    0x406a, 0x4a6a, 0x556a, 0x5f6a, 0x606b, 0x6a6b, 0x756b, 0x7f6b, 
    0x806c, 0x8a6c, 0x956c, 0x9f6c, 0xa06d, 0xaa6d, 0xb56d, 0xbf6d, 
    0xc06e, 0xca6e, 0xd56e, 0xdf6e, 0xe06f, 0xea6f, 0xf56f, 0xff6f, 
    0x0090, 0x0a90, 0x1590, 0x1f90, 0x2091, 0x2a91, 0x3591, 0x3f91, 
    0x4092, 0x4a92, 0x5592, 0x5f92, 0x6093, 0x6a93, 0x7593, 0x7f93, 
    0x8094, 0x8a94, 0x9594, 0x9f94, 0xa095, 0xaa95, 0xb595, 0xbf95, 
    0xc096, 0xca96, 0xd596, 0xdf96, 0xe097, 0xea97, 0xf597, 0xff97, 
    0x00b0, 0x0ab0, 0x15b0, 0x1fb0, 0x20b1, 0x2ab1, 0x35b1, 0x3fb1, 
    0x40b2, 0x4ab2, 0x55b2, 0x5fb2, 0x60b3, 0x6ab3, 0x75b3, 0x7fb3, 
    0x80b4, 0x8ab4, 0x95b4, 0x9fb4, 0xa0b5, 0xaab5, 0xb5b5, 0xbfb5, 
    0xc0b6, 0xcab6, 0xd5b6, 0xdfb6, 0xe0b7, 0xeab7, 0xf5b7, 0xffb7, 
    0x00d8, 0x0ad8, 0x15d8, 0x1fd8, 0x20d9, 0x2ad9, 0x35d9, 0x3fd9, 
    0x40da, 0x4ada, 0x55da, 0x5fda, 0x60db, 0x6adb, 0x75db, 0x7fdb, 
    0x80dc, 0x8adc, 0x95dc, 0x9fdc, 0xa0dd, 0xaadd, 0xb5dd, 0xbfdd, 
    0xc0de, 0xcade, 0xd5de, 0xdfde, 0xe0df, 0xeadf, 0xf5df, 0xffdf, 
    0x00f8, 0x0af8, 0x15f8, 0x1ff8, 0x20f9, 0x2af9, 0x35f9, 0x3ff9, 
    0x40fa, 0x4afa, 0x55fa, 0x5ffa, 0x60fb, 0x6afb, 0x75fb, 0x7ffb, 
    0x80fc, 0x8afc, 0x95fc, 0x9ffc, 0xa0fd, 0xaafd, 0xb5fd, 0xbffd, 
    0xc0fe, 0xcafe, 0xd5fe, 0xdffe, 0xe0ff, 0xeaff, 0xf5ff, 0xffff 
};
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

