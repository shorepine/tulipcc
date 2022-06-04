#include "display.h"


static const char *TAG = "display";
static TaskHandle_t xDisplayTask = NULL;
esp_lcd_panel_handle_t panel_handle;
esp_lcd_rgb_panel_config_t panel_config;

// RAM for sprites and background FB
uint8_t *sprite_ram;//[SPRITE_RAM_BYTES];
uint8_t * bg;

int64_t flash_start =0;
uint8_t flash_on = 0;






// Python callback
extern void tulip_frame_isr(); 
// This gets called at vsync / frame done
static bool IRAM_ATTR display_frame_done(esp_lcd_panel_handle_t panel_io, esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx)   {
    TaskHandle_t task_to_notify = (TaskHandle_t)user_ctx;
    BaseType_t high_task_wakeup;
    // Update the scroll
    for(uint16_t i=0;i<V_RES;i++) {
        x_offsets[i] = x_offsets[i] + x_speeds[i];
        y_offsets[i] = y_offsets[i] + y_speeds[i];
        x_offsets[i] = x_offsets[i] % (H_RES+OFFSCREEN_X_PX);
        y_offsets[i] = y_offsets[i] % (V_RES+OFFSCREEN_Y_PX);
        bg_lines[i] = (uint32_t*)&bg[(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL*y_offsets[i] + x_offsets[i]*BYTES_PER_PIXEL];
    }

    if(py_callback) tulip_frame_isr();
    vsync_count++;
    vTaskNotifyGiveFromISR(task_to_notify, &high_task_wakeup);
    return high_task_wakeup;
}

int64_t bounce_time = 0;
uint32_t bounce_count = 0;
int32_t desync = 0;
//3ms between frame done and calling this on the 2nd line 
// Gets called at N hsyncs, and fills in the next N line
static bool display_bounce_empty(void *bounce_buf, int pos_px, int len_bytes, void *user_ctx) {
    // Which pixel row and TFB row is this
    int64_t tic=esp_timer_get_time();
    uint16_t starting_display_row_px = pos_px / H_RES;
    uint8_t bounce_total_rows_px = len_bytes / H_RES / BYTES_PER_PIXEL;
    // compute the starting TFB row offset 
    uint8_t * b = (uint8_t*)bounce_buf;

    // Copy in the BG, line by line 
    // 208uS per call at 6 lines
    for(uint8_t rows_relative_px=0;rows_relative_px<bounce_total_rows_px;rows_relative_px++) {
        memcpy(b+(H_RES*BYTES_PER_PIXEL*rows_relative_px), bg_lines[(starting_display_row_px+rows_relative_px) % V_RES], H_RES*BYTES_PER_PIXEL); 
    }

    uint8_t test_bits[16]= {0x80,0x80,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x01};
    // Now per row (N pixel rows per call), do the TFB then sprites
    for(uint8_t bounce_row_px=0;bounce_row_px<bounce_total_rows_px;bounce_row_px++) {
        uint8_t tfb_row = (starting_display_row_px+bounce_row_px) / FONT_HEIGHT;
        uint8_t tfb_row_offset_px = (starting_display_row_px+bounce_row_px) % FONT_HEIGHT; 

        // Add in the TFB
        uint8_t tfb_col = 0;
        while(TFB[tfb_row*TFB_COLS+tfb_col]!=0 && tfb_col < TFB_COLS) {
            uint8_t data = font_8x12_r[TFB[tfb_row*TFB_COLS+tfb_col]][tfb_row_offset_px];
            uint8_t format = TFBf[tfb_row*TFB_COLS+tfb_col];
            uint8_t fg_color0 = ansi_pal[(format & 0x0f)*BYTES_PER_PIXEL+0];
            uint8_t fg_color1 = ansi_pal[(format & 0x0f)*BYTES_PER_PIXEL+1];
            // Skip drawing if flash is active
 
            /*
            if(format & FORMAT_FLASH) {
                if(vsync_count - flash_start > FLASH_FRAMES) {
                    flash_on = !flash_on;
                    flash_start = vsync_count;
                }
                if(!flash_on) { tfb_col++; continue; }
            }
          */
           uint32_t byte_start = bounce_row_px*H_RES*BYTES_PER_PIXEL + tfb_col*FONT_WIDTH*BYTES_PER_PIXEL;
            if(format & FORMAT_INVERSE) {
                for(uint8_t k=0;k<16;k=k+2) {
                    if(!(data & test_bits[k])) {
                        b[byte_start + k] = fg_color0;
                        b[byte_start + k + 1] = fg_color1;
                    }
                }
            } else {
                for(uint8_t k=0;k<16;k=k+2) {
                    if(data & test_bits[k]) {
                        b[byte_start + k] = fg_color0;
                        b[byte_start + k + 1] = fg_color1;
                    }
                }
            }

            tfb_col++;

        }

        // Add in the sprites
        uint16_t row_px = starting_display_row_px + bounce_row_px; 
        for(uint8_t s=0;s<SPRITES;s++) {
            if(sprite_vis[s]) {
                if(row_px >= sprite_y_px[s] && row_px < sprite_y_px[s]+sprite_h_px[s]) {
                    // this sprite is on this line 
                    // compute x and y (relative to the sprite!)
                    uint8_t * sprite_data = &sprite_ram[sprite_mem[s]];
                    uint16_t relative_sprite_y_px = row_px - sprite_y_px[s];
                    for(uint16_t col_px=sprite_x_px[s]; col_px < sprite_x_px[s] + sprite_w_px[s]; col_px++) {
                        if(col_px < H_RES) {
                            uint16_t relative_sprite_x_px = col_px - sprite_x_px[s];
                            uint8_t b0 = sprite_data[relative_sprite_y_px * sprite_w_px[s] * BYTES_PER_PIXEL + relative_sprite_x_px * BYTES_PER_PIXEL + 0 ] ;
                            uint8_t b1 = sprite_data[relative_sprite_y_px * sprite_w_px[s] * BYTES_PER_PIXEL + relative_sprite_x_px * BYTES_PER_PIXEL + 1 ] ;
                            if(!(b0 == ALPHA0 && b1 == ALPHA1)) {
                                b[bounce_row_px*H_RES*BYTES_PER_PIXEL + col_px*BYTES_PER_PIXEL + 0] = b0;
                                b[bounce_row_px*H_RES*BYTES_PER_PIXEL + col_px*BYTES_PER_PIXEL + 1] = b1;
                            }
                        }
                    } // end for each column
                } // end if this row has a sprite on it 
            } // end if sprite vis
        } // for each sprite
      
    } // per each row
    bounce_time += (esp_timer_get_time() - tic)    ;

    bounce_count++;
    return false; 
}



void display_reset_bg() {
    bg_pal_color = TULIP_TEAL;
    for(int i=0;i<(H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX)*BYTES_PER_PIXEL;i=i+2) { 
        (bg)[i+0] = ansi_pal[bg_pal_color*BYTES_PER_PIXEL+0]; 
        (bg)[i+1] = ansi_pal[bg_pal_color*BYTES_PER_PIXEL+1]; 
    }
    
    // init the scroll pointer to the top left of the fb 
    for(int i=0;i<V_RES;i++) {
        bg_lines[i] = (uint32_t*)&bg[(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL*i];
        x_offsets[i] = 0;
        y_offsets[i] = i;
        x_speeds[i] = 0;
        y_speeds[i] = 0;
    }

}

void display_reset_tfb() {
    // Clear out the TFB
    tfb_pal_color = ANSI_BOLD_WHITE;
    for(uint i=0;i<TFB_ROWS*TFB_COLS;i++) {
        TFB[i]=0;
        TFBf[i]=tfb_pal_color;
    }
    tfb_y_row = 0;
    tfb_x_col = 0;
    ansi_active_format =-1; // no override
    ansi_active_bg_color = FORMAT_BG_COLOR_NONE;

}

void display_reset_sprites() {
    // Set the sprite positions and speeds to 0
    for(int i=0;i<SPRITES;i++) { 
        sprite_mem[i] = 0;
        sprite_x_px[i] = 0; 
        sprite_y_px[i] = 0; 
        sprite_w_px[i] = 0; 
        sprite_h_px[i] = 0; 
        sprite_vis[i] = 0;
    }
    for(uint32_t i=0;i<SPRITE_RAM_BYTES;i++) sprite_ram[i] = 0;
}

void display_stop() {
    esp_lcd_panel_del(panel_handle);
}


void display_start() {
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
}

uint8_t display_get_clock() {
    return display_mhz;
}
void display_set_clock(uint8_t mhz) {  
    if(mhz > 1 && mhz < 50) {
        display_mhz = mhz;
        display_stop();
        panel_config.timings.pclk_hz = mhz*1000*1000;
        vTaskDelay(pdMS_TO_TICKS(1000));
        display_start();
    }
}


void display_get_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * bitmap) {
    uint32_t c = 0;
    for (int j = y; j < y+h; j++) {
        for (int i = x; i < x+w; i++) {
            bitmap[c++] = (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)];
            bitmap[c++] = (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 1)];
        }
    }
}

void unpack_rgb(uint8_t px0, uint8_t px1, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = px1 & 0xf8;
    *g = ((px1 << 5) | (px0 >> 5)) & 0xfc;
    *b = (px0 << 3) & 0xf8;
}

// GGGBBBBB RRRRRGGG, this returns GGGBBBBB
uint8_t color0(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t ret = 0;
    ret |= (blue & 0xf8) >> 3;
    ret |= (green & 0x1c) << 3;
    return ret;
}

// GGGBBBBB RRRRRGGG, this returns RRRRRGGG
uint8_t color1(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t ret = 0;
    ret |= green >> 5;
    ret |= (red & 0xf8);
    return ret;    
}

// input is RGBA data, but we skip alpha
void display_bg_bitmap(int x_start, int y_start, int x_end, int y_end, uint8_t* data) {
    //GGGBBBBB RRRRRGGG
    x_start = MIN(x_start, H_RES+OFFSCREEN_X_PX);
    x_end = MIN(x_end, H_RES+OFFSCREEN_X_PX);
    y_start = MIN(y_start, V_RES+OFFSCREEN_Y_PX);
    y_end = MIN(y_end, V_RES+OFFSCREEN_Y_PX);
    for (int j = y_start; j < y_end; j++) {
        for (int i = x_start; i < x_end; i++) {
            uint8_t r = *data++;
            uint8_t g = *data++;
            uint8_t b = *data++;
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = color0(r,g,b);
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 1)] = color1(r,g,b);
            // don't use alpha for bg bitmaps, so assign to to r so the compiler doesn't complain
            r = *data++;
        }
    }
}

//mem_len = sprite_load(bitmap, mem_pos, [x,y,w,h]) # returns mem_len (w*h*2)
// load a bitmap into fast sprite ram
void display_load_sprite(uint32_t mem_pos, uint32_t len, uint8_t* data) {
    for (int j = mem_pos; j < mem_pos + len; j=j+2) {
        uint8_t r = *data++;
        uint8_t g = *data++;
        uint8_t b = *data++;
        uint8_t a = *data++;
        if(a==0) { // only full transparent counts
            sprite_ram[j+0] = ALPHA0;
            sprite_ram[j+1] = ALPHA1;
        } else {
            sprite_ram[j+0] = color0(r,g,b);
            sprite_ram[j+1] = color1(r,g,b);
        }
    }
}


void display_screenshot(char * screenshot_fn) {
    // this will pause rendering, maybe set a flag to pause the screen? 
    // but basically calls bounce_empty with my own bounce_buf and write it to a png file line by line
    uint8_t * screenshot_bb = (uint8_t *) heap_caps_malloc(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    int err= 0;
    uint8_t * full_pic = (uint8_t*) heap_caps_malloc(H_RES*V_RES*3,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    uint32_t c = 0;
    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        display_bounce_empty(screenshot_bb, y*H_RES, H_RES*FONT_HEIGHT*BYTES_PER_PIXEL, NULL);
        for(uint16_t x=0;x<FONT_HEIGHT*H_RES*BYTES_PER_PIXEL;x=x+2) {
            uint8_t r=0;
            uint8_t g=0;
            uint8_t b=0;
            unpack_rgb(screenshot_bb[x+0], screenshot_bb[x+1], &r, &g, &b);
            full_pic[c++] = r;
            full_pic[c++] = g;
            full_pic[c++] = b;
        }
    }
    uint32_t outsize = 0;
    uint8_t *out;
    err= lodepng_encode_memory(&out, &outsize,full_pic, H_RES, V_RES, LCT_RGB, 8);

    printf("PNG done encoding. writing %d bytes to file %s\n", outsize, screenshot_fn);
    write_file(screenshot_fn, out, outsize, 1);
    heap_caps_free(out);
    heap_caps_free(screenshot_bb);
    heap_caps_free(full_pic);
}


void display_set_bg_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0] = color0(r,g,b);
    bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 1] = color1(r,g,b);
}


void display_get_bg_pixel(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t px0 = bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0];
    uint8_t px1 = bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0];
    unpack_rgb(px0, px1, r, g, b);
}


void display_tfb_new_row() {
    // Move the pointer to a new row, and scroll the view if necessary
    if(tfb_y_row == TFB_ROWS-1) {
        // We were in the last row, let's scroll the buffer up by moving the TFB up
        for(uint8_t i=0;i<TFB_ROWS-1;i++) {
            memcpy(&TFB[i*TFB_COLS], &TFB[(i+1)*TFB_COLS], TFB_COLS);
        }
        for(uint8_t i=0;i<TFB_COLS;i++) { TFB[tfb_y_row*TFB_COLS+i] = 0; TFBf[tfb_y_row*TFB_COLS+i] = tfb_pal_color;}
    } else {
        // Still got space, just increase the row counter
        // Also delete the previous cursor

        // were we at the end? if so don't reset cursor, it was never shown
        if(tfb_x_col != TFB_COLS) {
            TFB[tfb_y_row*TFB_COLS+tfb_x_col] = 0;
            TFBf[tfb_y_row*TFB_COLS+tfb_x_col] = tfb_pal_color;
        }
        tfb_y_row++;
    }
    // No matter what, go back to 0 on cols
    tfb_x_col = 0;
}


void display_tfb_str(char*str, uint16_t len, uint8_t format) {
    //printf("str len %d format %d is ###", len, format);
    //for(uint16_t i=0;i<len;i++) printf("%c", str[i]);
    //printf("###\n");
    // For each character incoming from micropython
    for(uint16_t i=0;i<len;i++) {
        if(str[i] == 8)  { // backspace , go backwards (don't delete)
            if(tfb_x_col > 0) tfb_x_col--;
        } else if(str[i] == 27) { // ANSI
            if(str[len-1] == 'K') { // clear to end of the line
                for(uint8_t i=tfb_x_col;i<TFB_COLS;i++) { TFB[tfb_y_row*TFB_COLS+i] = 0; TFBf[tfb_y_row*TFB_COLS+i] = tfb_pal_color; } 
                // We're done. set len to end 
                i= len;
            } else if(str[len-1] == 'D') { // move cursor backwards str[i+1] spaces
                uint8_t spaces = 0;
                if(len==5) { // two digits
                    spaces = (str[2]-'0')*10 + (str[3]-'0');
                } else {
                    spaces = (str[2]-'0');                    
                }
                tfb_x_col = tfb_x_col - spaces;
                i = len;
            } else if(str[len-1] == 'm') { // incoming ANSI formatting
                uint8_t code = 0;
                uint8_t last_pos = i+2; // first digit
                for(uint8_t pos=i+1;pos<len;pos++) {
                    if(str[pos]==';' || str[pos]=='m') { // for each combination
                        if(pos - last_pos == 1) { // single digit code
                            code = (str[pos-1]-'0'); 
                        }
                        if(pos - last_pos == 2) { // double digit code
                            code =  (str[pos-2]-'0')*10 + (str[pos-1]-'0');
                        }
                        last_pos = pos+1;
        
                        if(ansi_active_format<0) ansi_active_format = 0;
                        if(code==0)  ansi_active_format = -1;
                        if(code==1)  if ((ansi_active_format & 0x0f) < 8) ansi_active_format += 8; // "bold" color (not font!)
                        if(code==4)  ansi_active_format = ansi_active_format | FORMAT_UNDERLINE;
                        if(code==5)  ansi_active_format = ansi_active_format | FORMAT_FLASH;
                        if(code==7)  ansi_active_format = ansi_active_format | FORMAT_INVERSE;
                        if(code==22) if ((ansi_active_format & 0x0f) >= 8) ansi_active_format =- 8;
                        if(code==24) if(ansi_active_format | FORMAT_UNDERLINE) ansi_active_format =- FORMAT_UNDERLINE;
                        if(code==25) if(ansi_active_format | FORMAT_FLASH) ansi_active_format =- FORMAT_FLASH;
                        if(code==27) if(ansi_active_format | FORMAT_FLASH) ansi_active_format =- FORMAT_FLASH;
                        if(code>=30 && code<=37)  ansi_active_format = ((ansi_active_format & 0xf0) | (code-30)); // color
                        if(code==39) ansi_active_format = ((ansi_active_format & 0xf0) | tfb_pal_color);
                        // bg colors per char, not implemented yet
                        if(code>=40 && code<=47) ansi_active_bg_color = code-40; 
                        if(code==49) ansi_active_bg_color = FORMAT_BG_COLOR_NONE; // reset
                    } // end if found a ;/m
                } // end if test each char 
                i = last_pos; // skip the rest
            }
        } else if(str[i] == 10) {
            // If an LF, start a new row
            display_tfb_new_row();
        } else if(str[i]<32) {
            // do nothing with other non-printable chars
        } else {
            TFB[tfb_y_row*TFB_COLS+tfb_x_col] = str[i];    
            if(ansi_active_format >= 0 ) {
                TFBf[tfb_y_row*TFB_COLS+tfb_x_col] =ansi_active_format;        
            } else {
                TFBf[tfb_y_row*TFB_COLS+tfb_x_col] = format;        
            }
            tfb_x_col = (tfb_x_col + 1);
            if(tfb_x_col == TFB_COLS) {
                display_tfb_new_row();
            }
        }
    }
    // cursor
    TFB[tfb_y_row*TFB_COLS+tfb_x_col] = 32;
    TFBf[tfb_y_row*TFB_COLS+tfb_x_col] = FORMAT_INVERSE|FORMAT_FLASH|tfb_pal_color;
    
}



void display_run(void) {
    // Backlight on GPIO for the 10.1 panel
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BK_LIGHT
    };
    xDisplayTask = xTaskGetCurrentTaskHandle();
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    panel_handle = NULL;

    panel_config.data_width = 16; // RGB565 in parallel mode, thus 16bit in width
    panel_config.psram_trans_align = 64;
    panel_config.clk_src = LCD_CLK_SRC_PLL160M;
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
    panel_config.data_gpio_nums[8] = PIN_NUM_DATA8;
    panel_config.data_gpio_nums[9] = PIN_NUM_DATA9;
    panel_config.data_gpio_nums[10] = PIN_NUM_DATA10;
    panel_config.data_gpio_nums[11] = PIN_NUM_DATA11;
    panel_config.data_gpio_nums[12] = PIN_NUM_DATA12;
    panel_config.data_gpio_nums[13] = PIN_NUM_DATA13;
    panel_config.data_gpio_nums[14] = PIN_NUM_DATA14;
    panel_config.data_gpio_nums[15] = PIN_NUM_DATA15;
    panel_config.timings.pclk_hz = PIXEL_CLOCK_MHZ*1000*1000;
    panel_config.timings.h_res = H_RES;
    panel_config.timings.v_res = V_RES;
    panel_config.timings.hsync_back_porch = HSYNC_BACK_PORCH;
    panel_config.timings.hsync_front_porch = HSYNC_FRONT_PORCH;
    panel_config.timings.hsync_pulse_width = HSYNC_PULSE_WIDTH;
    panel_config.timings.vsync_back_porch = VSYNC_BACK_PORCH;
    panel_config.timings.vsync_front_porch = VSYNC_FRONT_PORCH;
    panel_config.timings.vsync_pulse_width = VSYNC_PULSE_WIDTH;
    panel_config.flags.relax_on_idle = 0;
    panel_config.flags.fb_in_psram = 0;
    panel_config.on_frame_trans_done = display_frame_done;
    panel_config.on_bounce_empty = display_bounce_empty;
    panel_config.user_ctx = xDisplayTask;
    panel_config.bounce_buffer_size_px = BOUNCE_BUFFER_SIZE_PX;

    display_mhz = PIXEL_CLOCK_MHZ;

    // Create the background FB
    bg = (uint8_t*)heap_caps_aligned_calloc(64, 1, (H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX)*BYTES_PER_PIXEL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    // And various ptrs
    sprite_ram = (uint8_t*)heap_caps_malloc(SPRITE_RAM_BYTES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_x_px = (uint16_t*)heap_caps_malloc(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_y_px = (uint16_t*)heap_caps_malloc(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_w_px = (uint16_t*)heap_caps_malloc(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_h_px = (uint16_t*)heap_caps_malloc(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_vis = (uint8_t*)heap_caps_malloc(SPRITES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_mem = (uint32_t*)heap_caps_malloc(SPRITES*sizeof(uint32_t), MALLOC_CAP_INTERNAL);

    TFB = (uint8_t*)heap_caps_malloc(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    TFBf = (uint8_t*)heap_caps_malloc(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);

    x_offsets = (uint16_t*)heap_caps_malloc(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    y_offsets = (uint16_t*)heap_caps_malloc(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    x_speeds = (int16_t*)heap_caps_malloc(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);
    y_speeds = (int16_t*)heap_caps_malloc(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);

    bg_lines = (uint32_t**)heap_caps_malloc(V_RES*sizeof(uint32_t*), MALLOC_CAP_INTERNAL);

    ansi_pal = (uint8_t*)heap_caps_malloc(2*17*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    

    ansi_pal = (uint8_t[]){
        // normal
        0, 0,       // ANSI_BLACK
        0, 168,     // ANSI_RED
        64, 5,      // ANSI_GREEN
        64, 173,    // ANSI_YELLOW
        21, 0,      // ANSI_BLUE
        21, 168,    // ANSI_MAGENTA
        85, 5,      // ANSI_CYAN
        85, 173,    // ANSI_WHITE
        // bold
        170, 82,    // ANSI_BOLD_BLACK
        170, 250,   // ANSI_BOLD_RED
        234, 87,    // ANSI_BOLD_GREEN
        234, 255,   // ANSI_BOLD_YELLOW
        191, 82,    // ANSI_BOLD_BLUE
        191, 250,   // ANSI_BOLD_MAGENTA
        255, 87,    // ANSI_BOLD_CYAN
        255, 255,   // ANSI_BOLD_WHITE
        8,   2      // TULIP_TEAL
    };

    // Init the BG, TFB and sprite layers
    display_reset_bg();
    display_reset_tfb();
    display_reset_sprites();
    
    py_callback = 0;
    vsync_count = 1;


    // Start the RGB panel
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    gpio_set_level(PIN_NUM_BK_LIGHT, BK_LIGHT_ON_LEVEL);


    // Time the frame sync and stay running forever 
    int64_t free_time = 0;
    int64_t tic0 = esp_timer_get_time();
    uint16_t loop_count =0;
    while(1)  { 
        int64_t tic1 = esp_timer_get_time();
        ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(100));
        free_time += (esp_timer_get_time() - tic1);
        if(loop_count++ >= 100) {
            printf("Past %d frames: %2.2f FPS. free time %llduS. bounce time per is %llduS, %2.2f%% of max (%duS). %d desyncs \n", 
                loop_count,
                1000000.0 / ((esp_timer_get_time() - tic0) / loop_count), 
                free_time / loop_count,
                bounce_time / bounce_count,
                ((float)(bounce_time/bounce_count) / (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (1000000.0 / ((esp_timer_get_time() - tic0) / loop_count)))))*100.0,
                (int) (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (1000000.0 / ((esp_timer_get_time() - tic0) / loop_count)))),
                desync
            );
            bounce_count = 1;
            bounce_time = 0;
            loop_count = 0;
            free_time = 0;
            desync = 0;
            tic0 = esp_timer_get_time();
        }        
    }


}

