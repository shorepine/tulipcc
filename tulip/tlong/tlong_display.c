#include "tlong_display.h"
#include "tasks.h"

void axs15231_init(void);
void lcd_PushColors(uint16_t x, uint16_t y,uint16_t width,  uint16_t high, uint16_t *data);
void lcd_PushColors_whole(uint16_t *data, uint32_t len);
bool get_lcd_spi_dma_write(void);
// Defaults for esp32 display params that we don't need on TLong
uint32_t HSYNC_BACK_PORCH=0;
uint32_t HSYNC_FRONT_PORCH=0;
uint32_t HSYNC_PULSE_WIDTH=0;
uint32_t VSYNC_BACK_PORCH=0;
uint32_t VSYNC_FRONT_PORCH=0;
uint32_t VSYNC_PULSE_WIDTH=0;
int16_t touch_x_delta = 0;
int16_t touch_y_delta = 0;
uint8_t dumb_display_mutex = 0;

// lookup table for Tulip's "pallete" to the 16-bit colorspace needed by the TLong display
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
// Unused by TLong stubs
void esp32s3_display_stop() {
}

void esp32s3_display_start() {
}

void esp32s3_display_timings(uint32_t t0,uint32_t t1,uint32_t t2,uint32_t t3,uint32_t t4,uint32_t t5,uint32_t t6,uint32_t t7,uint32_t t8,uint32_t t9,uint32_t t10,uint32_t t11) {
}

void esp_display_set_clock(uint8_t mhz) {  
}


// TODO -- the trackball also responds to joy checks
uint16_t check_joy() {
    return 0;
}

void display_pwm_setup() {
}


void display_brightness(uint8_t amount) {
}


extern uint32_t transfer_num;
extern size_t lcd_PushColors_len;
// Task runner for the display, inits and then spins in a loop processing 
void run_tlong_display(void) {
    H_RES=640;
    V_RES=180;
    H_RES_D=640;
    V_RES_D=180;

    // First set up the memory
    display_init();

     // turn on backlight
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << TFT_BL
    };
    gpio_config(&bk_gpio_config);
    gpio_set_level(TFT_BL, 1);

    // And the display
    axs15231_init();

    // Time the frame sync and stay running forever 
    int64_t tic0 = esp_timer_get_time();
    uint16_t loop_count =0;
    uint8_t *frame_bb =(uint8_t*) malloc_caps(H_RES*FONT_HEIGHT, MALLOC_CAP_DMA);
    uint16_t *whole = (uint16_t*) malloc_caps(H_RES*V_RES*2, MALLOC_CAP_SPIRAM);
    for(uint32_t i=0;i<H_RES*V_RES;i++) whole[i] = 0;
    while(1)  { 
        if(loop_count++ >= 100) {
            float reported_fps = 1000000.0 / ((esp_timer_get_time() - tic0) / loop_count);
            if(gpu_log)fprintf(stderr,"past %d frames %2.2f FPS.\n", loop_count, reported_fps);
            loop_count = 0;
            tic0 = esp_timer_get_time();
        }        

        // bounce the entire screen at once to the display
        // We can't bounce line by line like on the T-Deck as this display is rotated
        for(uint16_t y=0;y<(V_RES-FONT_HEIGHT);y=y+FONT_HEIGHT) {
            // Get the pixel data for a row of screen from Tulip
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT, NULL);

            // Write it to the TLong screen via colorspace conversion and rotation
            uint16_t idx = 0;
            for(uint16_t sub_y=y;sub_y<y+FONT_HEIGHT;sub_y++) {
                for(uint16_t sub_x=0;sub_x<H_RES;sub_x++) {
                    uint32_t my_x = sub_y;
                    uint32_t my_y = (H_RES-1)-sub_x; 
                    whole[my_y*V_RES+my_x] = rgb332_rgb565_i[frame_bb[idx++]];
                }
            }

        }

        // Blit the entire display over. You have to do this flush dance or else it's corrupted
        if (transfer_num <= 0 && lcd_PushColors_len <= 0){
            #ifdef LCD_SPI_DMA
            while (get_lcd_spi_dma_write()) {
                lcd_PushColors(0, 0, 0, 0, NULL);
            }
            #endif
            // Push the display
            lcd_PushColors(0,0, V_RES, H_RES, whole);
        }

        // Wait for the last thing to finish
        if (transfer_num <= 1 && lcd_PushColors_len > 0) {
           lcd_PushColors(0, 0, 0, 0, NULL);
        }

        // Tell Tulip we're done with the frame
        display_frame_done_generic();

    }


}

