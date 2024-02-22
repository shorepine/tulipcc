#include "esp32s3_display.h"

esp_lcd_panel_handle_t panel_handle;
esp_lcd_rgb_panel_config_t panel_config;
esp_lcd_rgb_panel_event_callbacks_t panel_callbacks;


void set_pin(uint16_t pin, uint8_t value) {
    gpio_config_t peri_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << pin
    };
    gpio_config(&peri_gpio_config);
    gpio_set_level(pin, value);
}

void esp32s3_display_restart() {
    esp32s3_display_stop();
    esp32s3_display_start();
}

// This gets called at vsync / frame done
static bool display_frame_done(esp_lcd_panel_handle_t panel_io, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)   {
    TaskHandle_t task_to_notify = (TaskHandle_t)user_ctx;
    BaseType_t high_task_wakeup;
    display_frame_done_generic();
    vTaskNotifyGiveFromISR(task_to_notify, &high_task_wakeup);
    return high_task_wakeup;
}


void esp32s3_display_stop() {
    #ifdef TULIP_DIY
    gpio_set_level(PIN_NUM_BK_LIGHT, !BK_LIGHT_ON_LEVEL);
    #endif
    esp_lcd_panel_del(panel_handle);
}

bool IRAM_ATTR esp32s3_display_bounce_empty(esp_lcd_panel_handle_t panel_io, void *bounce_buf, int pos_px, int len_bytes, void *user_ctx) {
    return display_bounce_empty(bounce_buf, pos_px, len_bytes, user_ctx);
}

void esp32s3_display_start() {
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &panel_callbacks, display_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    display_brightness(brightness); 
    #ifdef TULIP_DIY
    gpio_set_level(PIN_NUM_BK_LIGHT, BK_LIGHT_ON_LEVEL);
    #endif

}

void esp_display_set_clock(uint8_t mhz) {  
        PIXEL_CLOCK_MHZ = mhz;
        esp_lcd_rgb_panel_set_pclk(panel_handle, mhz*1000*1000);
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
        .gpio_num       = PIN_NUM_BK_PWM,
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
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1) ;
}


extern int64_t bounce_time;
extern uint32_t bounce_count ;

// Task runner for the display, inits and then spins in a loop processing frame done ISRs
void run_esp32s3_display(void) {
    // First set up the memory
    display_init();
    
    panel_handle = NULL;
    panel_config.data_width = BYTES_PER_PIXEL*8; 
    panel_config.psram_trans_align = 64;
    panel_config.clk_src = LCD_CLK_SRC_DEFAULT;
    panel_config.disp_gpio_num = PIN_NUM_DISP_EN;
    panel_config.pclk_gpio_num = PIN_NUM_PCLK;
    panel_config.vsync_gpio_num = PIN_NUM_VSYNC;
    panel_config.hsync_gpio_num = PIN_NUM_HSYNC;
    panel_config.de_gpio_num = PIN_NUM_DE;

    panel_config.data_gpio_nums[0] = PIN_NUM_DATA0;
    panel_config.data_gpio_nums[1] = PIN_NUM_DATA1;
    panel_config.data_gpio_nums[2] = PIN_NUM_DATA2;
    panel_config.data_gpio_nums[3] = PIN_NUM_DATA3;
    panel_config.data_gpio_nums[4] = PIN_NUM_DATA4;
    panel_config.data_gpio_nums[5] = PIN_NUM_DATA5;
    panel_config.data_gpio_nums[6] = PIN_NUM_DATA6;
    panel_config.data_gpio_nums[7] = PIN_NUM_DATA7;
    // Even though we'll only use 8 pins for RGB332 we keep the others to set them low
    panel_config.data_gpio_nums[8] = PIN_NUM_DATA8;
    panel_config.data_gpio_nums[9] = PIN_NUM_DATA9;
    panel_config.data_gpio_nums[10] = PIN_NUM_DATA10;
    panel_config.data_gpio_nums[11] = PIN_NUM_DATA11;
    panel_config.data_gpio_nums[12] = PIN_NUM_DATA12;
    panel_config.data_gpio_nums[13] = PIN_NUM_DATA13;
    panel_config.data_gpio_nums[14] = PIN_NUM_DATA14;
    panel_config.data_gpio_nums[15] = PIN_NUM_DATA15;

    #ifdef MAKERFABS
    // Set lower 8 bits to low
    set_pin(PIN_NUM_DATA8, 0);
    set_pin(PIN_NUM_DATA9, 0);
    set_pin(PIN_NUM_DATA10, 0);
    set_pin(PIN_NUM_DATA11, 0);
    set_pin(PIN_NUM_DATA12, 0);
    set_pin(PIN_NUM_DATA13, 0);
    set_pin(PIN_NUM_DATA14, 0);
    set_pin(PIN_NUM_DATA15, 0);
    #endif

    panel_config.timings.pclk_hz = PIXEL_CLOCK_MHZ*1000*1000;
    panel_config.timings.h_res = H_RES;
    panel_config.timings.v_res = V_RES;
    panel_config.timings.hsync_back_porch = HSYNC_BACK_PORCH;
    panel_config.timings.hsync_front_porch = HSYNC_FRONT_PORCH;
    panel_config.timings.hsync_pulse_width = HSYNC_PULSE_WIDTH;
    panel_config.timings.vsync_back_porch = VSYNC_BACK_PORCH;
    panel_config.timings.vsync_front_porch = VSYNC_FRONT_PORCH;
    panel_config.timings.vsync_pulse_width = VSYNC_PULSE_WIDTH;

    panel_config.num_fbs = 0; // we do this ourselves
    panel_config.flags.fb_in_psram = 0;
    panel_config.flags.no_fb = 1;
    panel_config.flags.refresh_on_demand = 0;
    panel_config.flags.bb_invalidate_cache = 0;

    panel_config.bounce_buffer_size_px = BOUNCE_BUFFER_SIZE_PX;
    brightness = DEFAULT_BRIGHTNESS;

    panel_callbacks.on_vsync = display_frame_done;
    panel_callbacks.on_bounce_empty = esp32s3_display_bounce_empty;

    // Start the RGB panel
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &panel_callbacks, display_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    display_pwm_setup();
    display_brightness(brightness); 
    #ifdef TULIP_DIY
    gpio_set_level(PIN_NUM_BK_LIGHT, BK_LIGHT_ON_LEVEL);
    #endif
    
    // Time the frame sync and stay running forever 
    int64_t free_time = 0;
    int64_t tic0 = esp_timer_get_time();
    uint16_t loop_count =1;
    bounce_count = 1;

    while(1)  { 
        int64_t tic1 = esp_timer_get_time();
        ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(100));

        free_time += (esp_timer_get_time() - tic1);
        if(loop_count++ >= 100) {
            reported_fps = 1000000.0 / ((esp_timer_get_time() - tic0) / loop_count);
            reported_gpu_usage = ((float)(bounce_time/bounce_count) / (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (reported_fps))))*100.0;
            if(gpu_log) {
                printf("past %d frames %2.2f FPS. free time %llduS. bounce time per is %llduS, %2.2f%% of max (%duS). bounce_count %ld\n", 
                    loop_count,
                    reported_fps, 
                    free_time / loop_count,
                    bounce_time / bounce_count,
                    reported_gpu_usage,
                    (int) (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (reported_fps))),
                    bounce_count
                );
                gpu_log = 0;
            }
            bounce_count = 1;
            bounce_time = 0;
            loop_count = 0;
            free_time = 0;
            tic0 = esp_timer_get_time();
        }        
    }


}

