#include "tdeck_display.h"
#include "tasks.h"


// Defaults for esp32 display params
uint32_t HSYNC_BACK_PORCH=0;
uint32_t HSYNC_FRONT_PORCH=0;
uint32_t HSYNC_PULSE_WIDTH=0;
uint32_t VSYNC_BACK_PORCH=0;
uint32_t VSYNC_FRONT_PORCH=0;
uint32_t VSYNC_PULSE_WIDTH=0;
int16_t touch_x_delta = 0;
int16_t touch_y_delta = 0;

const uint16_t RGB332to565lookupTable[256] = {
    0x0000, 0x000a, 0x0015, 0x001f, 0x0120, 0x012a, 0x0135, 0x013f, 
    0x0240, 0x024a, 0x0255, 0x025f, 0x0360, 0x036a, 0x0375, 0x037f, 
    0x0480, 0x048a, 0x0495, 0x049f, 0x05a0, 0x05aa, 0x05b5, 0x05bf, 
    0x06c0, 0x06ca, 0x06d5, 0x06df, 0x07e0, 0x07ea, 0x07f5, 0x07ff, 
    0x2000, 0x200a, 0x2015, 0x201f, 0x2120, 0x212a, 0x2135, 0x213f, 
    0x2240, 0x224a, 0x2255, 0x225f, 0x2360, 0x236a, 0x2375, 0x237f, 
    0x2480, 0x248a, 0x2495, 0x249f, 0x25a0, 0x25aa, 0x25b5, 0x25bf, 
    0x26c0, 0x26ca, 0x26d5, 0x26df, 0x27e0, 0x27ea, 0x27f5, 0x27ff, 
    0x4800, 0x480a, 0x4815, 0x481f, 0x4920, 0x492a, 0x4935, 0x493f, 
    0x4a40, 0x4a4a, 0x4a55, 0x4a5f, 0x4b60, 0x4b6a, 0x4b75, 0x4b7f, 
    0x4c80, 0x4c8a, 0x4c95, 0x4c9f, 0x4da0, 0x4daa, 0x4db5, 0x4dbf, 
    0x4ec0, 0x4eca, 0x4ed5, 0x4edf, 0x4fe0, 0x4fea, 0x4ff5, 0x4fff, 
    0x6800, 0x680a, 0x6815, 0x681f, 0x6920, 0x692a, 0x6935, 0x693f, 
    0x6a40, 0x6a4a, 0x6a55, 0x6a5f, 0x6b60, 0x6b6a, 0x6b75, 0x6b7f, 
    0x6c80, 0x6c8a, 0x6c95, 0x6c9f, 0x6da0, 0x6daa, 0x6db5, 0x6dbf, 
    0x6ec0, 0x6eca, 0x6ed5, 0x6edf, 0x6fe0, 0x6fea, 0x6ff5, 0x6fff, 
    0x9000, 0x900a, 0x9015, 0x901f, 0x9120, 0x912a, 0x9135, 0x913f, 
    0x9240, 0x924a, 0x9255, 0x925f, 0x9360, 0x936a, 0x9375, 0x937f, 
    0x9480, 0x948a, 0x9495, 0x949f, 0x95a0, 0x95aa, 0x95b5, 0x95bf, 
    0x96c0, 0x96ca, 0x96d5, 0x96df, 0x97e0, 0x97ea, 0x97f5, 0x97ff, 
    0xb000, 0xb00a, 0xb015, 0xb01f, 0xb120, 0xb12a, 0xb135, 0xb13f, 
    0xb240, 0xb24a, 0xb255, 0xb25f, 0xb360, 0xb36a, 0xb375, 0xb37f, 
    0xb480, 0xb48a, 0xb495, 0xb49f, 0xb5a0, 0xb5aa, 0xb5b5, 0xb5bf, 
    0xb6c0, 0xb6ca, 0xb6d5, 0xb6df, 0xb7e0, 0xb7ea, 0xb7f5, 0xb7ff, 
    0xd800, 0xd80a, 0xd815, 0xd81f, 0xd920, 0xd92a, 0xd935, 0xd93f, 
    0xda40, 0xda4a, 0xda55, 0xda5f, 0xdb60, 0xdb6a, 0xdb75, 0xdb7f, 
    0xdc80, 0xdc8a, 0xdc95, 0xdc9f, 0xdda0, 0xddaa, 0xddb5, 0xddbf, 
    0xdec0, 0xdeca, 0xded5, 0xdedf, 0xdfe0, 0xdfea, 0xdff5, 0xdfff, 
    0xf800, 0xf80a, 0xf815, 0xf81f, 0xf920, 0xf92a, 0xf935, 0xf93f, 
    0xfa40, 0xfa4a, 0xfa55, 0xfa5f, 0xfb60, 0xfb6a, 0xfb75, 0xfb7f, 
    0xfc80, 0xfc8a, 0xfc95, 0xfc9f, 0xfda0, 0xfdaa, 0xfdb5, 0xfdbf, 
    0xfec0, 0xfeca, 0xfed5, 0xfedf, 0xffe0, 0xffea, 0xfff5, 0xffff 
};


void esp32s3_display_stop() {
}


void esp32s3_display_start() {
}

void esp32s3_display_timings(uint32_t t0,uint32_t t1,uint32_t t2,uint32_t t3,uint32_t t4,uint32_t t5,uint32_t t6,uint32_t t7,uint32_t t8,uint32_t t9,uint32_t t10,uint32_t t11) {

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

void esp_display_set_clock(uint8_t mhz) {  
    // maybe set a new clock, but TBD
}


uint16_t check_joy() {
    return 0;
    // TODO , use that dingus
}


static bool tdeck_trans_done(esp_lcd_panel_io_handle_t panel_io,
                            esp_lcd_panel_io_event_data_t* edata,
                            void* user_ctx) {
    BaseType_t high_task_wakeup;
    vTaskNotifyGiveFromISR(display_handle, &high_task_wakeup);
    return high_task_wakeup;
}
// Task runner for the display, inits and then spins in a loop processing frame done ISRs
void run_tdeck_display(void) {
    H_RES=320;
    V_RES=240;
    H_RES_D=320;
    V_RES_D=240;
    // First set up the memory
    fprintf(stderr, "display set up %d x %d\n", H_RES, V_RES);

    display_init();
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;

    // turn off backlight
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << TDECK_LCD_BK_LIGHT_GPIO
    };
    gpio_config(&bk_gpio_config);
    gpio_set_level(TDECK_LCD_BK_LIGHT_GPIO, 0);

    // turn on peri
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
        .on_color_trans_done = tdeck_trans_done, // could be on_bounce_empty(?) 
        .user_ctx = NULL,
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = TDECK_LCD_RST_GPIO,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
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
    
    uint8_t * clear = malloc_caps(H_RES*V_RES*2, MALLOC_CAP_DMA);
    memset(clear, 0, H_RES*V_RES*2 );
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, H_RES, V_RES, clear);
    free(clear);

    brightness = DEFAULT_BRIGHTNESS;

    // Time the frame sync and stay running forever 
    int64_t tic0 = esp_timer_get_time();
    uint16_t loop_count =0;
    uint8_t *frame_bb =(uint8_t*) malloc_caps(H_RES*FONT_HEIGHT, MALLOC_CAP_INTERNAL);
    uint16_t *bb565 = (uint16_t*) malloc_caps(H_RES*FONT_HEIGHT*2, MALLOC_CAP_INTERNAL);
    while(1)  { 
        if(loop_count++ >= 100) {
            float reported_fps = 1000000.0 / ((esp_timer_get_time() - tic0) / loop_count);
            if(gpu_log)fprintf(stderr,"past %d frames %2.2f FPS.\n", loop_count, reported_fps);
            loop_count = 0;
            tic0 = esp_timer_get_time();
        }        


        // bounce the entire screen at once to the 332 color framebuffer
        for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100));
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT, NULL);
            for(uint16_t i=0;i<H_RES*FONT_HEIGHT;i++) {
                bb565[i] = RGB332to565lookupTable[frame_bb[i]];
            }
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, H_RES, y+FONT_HEIGHT, bb565);
        }
        
        display_frame_done_generic();

    }


}

