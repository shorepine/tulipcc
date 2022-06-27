#include "display.h"

static TaskHandle_t xDisplayTask = NULL;
esp_lcd_panel_handle_t panel_handle;
esp_lcd_rgb_panel_config_t panel_config;


// RRRGGGBB
void unpack_rgb_332(uint8_t px0, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = px0 & 0xe0;
    *g = (px0 << 3) & 0xe0;
    *b = (px0 << 6) & 0xc0;
}

// Given a single uint (0-255 for RGB332, 0-65535 for RGB565), return r, g, b
void unpack_pal_idx(uint16_t pal_idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    unpack_rgb_332(pal_idx & 0xff, r, g, b);
}

// Given an ansi pal index (0-255 right now), return r g b
void unpack_ansi_idx(uint8_t ansi_idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    unpack_rgb_332(ansi_idx, r, g, b);
}

// Return a packed 8-bit number for RRRGGGBB
uint8_t color_332(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t ret = 0;
    ret |= (red&0xe0);
    ret |= (green&0xe0) >> 3;
    ret |= (blue&0xc0) >> 6;
    return ret;
}



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

// Timers / counters for perf
int64_t bounce_time = 0;
uint32_t bounce_count = 0;
int32_t desync = 0;
// Two buffers are filled by this function, one gets filled while the other is drawn (via GDMA to the LCD.) 
// Each call fills a certain number of lines, set by BOUNCE_BUFFER_SIZE_PX in setup (it's currently 12 lines / 1 row of text)
static bool display_bounce_empty(void *bounce_buf, int pos_px, int len_bytes, void *user_ctx) {
    int64_t tic=esp_timer_get_time(); // start the timer

    // Which pixel row and TFB row is this
    uint16_t starting_display_row_px = pos_px / H_RES;
    uint8_t bounce_total_rows_px = len_bytes / H_RES / BYTES_PER_PIXEL;
    // compute the starting TFB row offset 
    uint8_t * b = (uint8_t*)bounce_buf;

    // Copy in the BG, line by line 
    // 208uS per call at 6 lines RGB565
    // 209uS per call at 12 lines RGB332
    // 416uS per call at 12 lines RGB565
    for(uint8_t rows_relative_px=0;rows_relative_px<bounce_total_rows_px;rows_relative_px++) {
        memcpy(b+(H_RES*BYTES_PER_PIXEL*rows_relative_px), bg_lines[(starting_display_row_px+rows_relative_px) % V_RES], H_RES*BYTES_PER_PIXEL); 
    }
    
    // Now per row (N (now 12) pixel rows per call), draw the text frame buffer and sprites on top of the BG
    for(uint8_t bounce_row_px=0;bounce_row_px<bounce_total_rows_px;bounce_row_px++) {
        uint8_t tfb_row = (starting_display_row_px+bounce_row_px) / FONT_HEIGHT;
        uint8_t tfb_row_offset_px = (starting_display_row_px+bounce_row_px) % FONT_HEIGHT; 

        uint8_t tfb_col = 0;
        while(TFB[tfb_row*TFB_COLS+tfb_col]!=0 && tfb_col < TFB_COLS) {
            uint8_t data = font_8x12_r[TFB[tfb_row*TFB_COLS+tfb_col]][tfb_row_offset_px];
            uint8_t format = TFBf[tfb_row*TFB_COLS+tfb_col];
            uint8_t fg_color = TFBfg[tfb_row*TFB_COLS+tfb_col];
            uint8_t bg_color = TFBbg[tfb_row*TFB_COLS+tfb_col];

            // If you're looking at this code just know the unrolled versions were 1.5x faster than loops
            // I'm sure there's more to do but this is the best we could get it for now
            uint8_t * bptr = b + (bounce_row_px*H_RES + tfb_col*FONT_WIDTH);
            if(bg_color == ALPHA) {
                if(format & FORMAT_INVERSE) {
                    if(!((data) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                } else {
                    if((data) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                }
            } else {
                if(format & FORMAT_INVERSE) {
                    if(!((data) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                } else {
                    if((data) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                }

            }
            tfb_col++;
        }
        // Add in the sprites
        uint16_t row_px = starting_display_row_px + bounce_row_px; 
        for(uint8_t s=0;s<SPRITES;s++) {
            if(sprite_vis[s]) {
                if(sprite_vis[s] & SPRITE_IS_SPRITE) {
                    if(row_px >= sprite_y_px[s] && row_px < sprite_y_px[s]+sprite_h_px[s]) {
                        // this sprite is on this line 
                        // compute x and y (relative to the sprite!)
                        uint8_t * sprite_data = &sprite_ram[sprite_mem[s]];
                        uint16_t relative_sprite_y_px = row_px - sprite_y_px[s];
                        for(uint16_t col_px=sprite_x_px[s]; col_px < sprite_x_px[s] + sprite_w_px[s]; col_px++) {
                            if(col_px < H_RES) {
                                uint16_t relative_sprite_x_px = col_px - sprite_x_px[s];
                                uint8_t b0 = sprite_data[relative_sprite_y_px * sprite_w_px[s] + relative_sprite_x_px  ] ;
                                if(b0 != ALPHA) {
                                    b[bounce_row_px*H_RES + col_px] = b0;
                                }
                            }
                        } // end for each column
                    } // end if this row has a sprite on it 
                } else if(sprite_vis[s] & SPRITE_IS_LINE) {
                    // draw a line using sprite data; sprite_w contains the # of points
                } else if(sprite_vis[s] & SPRITE_IS_BEZIER) {
                    // draw a bezier using sprite data; sprite_w contains the # of points
                } else if(sprite_vis[s] & SPRITE_IS_ELLIPSE) {
                    // draw an ellipse
                }
            } // end if sprite vis
        } // for each sprite
    } // per each row
    bounce_time += (esp_timer_get_time() - tic); // stop timer
    bounce_count++;

    return false; 
}



void display_reset_bg() {
    bg_pal_color = TULIP_TEAL;
    for(int i=0;i<(H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX)*BYTES_PER_PIXEL;i=i+BYTES_PER_PIXEL) { 
        (bg)[i+0] = bg_pal_color; 
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
    tfb_fg_pal_color = color_332(255,255,255);
    tfb_bg_pal_color = ALPHA;

    for(uint i=0;i<TFB_ROWS*TFB_COLS;i++) {
        TFB[i]=0;
        TFBfg[i]=tfb_fg_pal_color;
        TFBbg[i]=tfb_bg_pal_color;
        TFBf[i]=0;
    }
    tfb_y_row = 0;
    tfb_x_col = 0;
    ansi_active_format = -1; // no override
    ansi_active_fg_color = tfb_fg_pal_color; 
    ansi_active_bg_color = tfb_bg_pal_color;

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


void display_set_clock(uint8_t mhz) {  
    if(mhz > 1 && mhz < 50) {
        display_mhz = mhz;
        display_stop();
        panel_config.timings.pclk_hz = mhz*1000*1000;
        //vTaskDelay(pdMS_TO_TICKS(1000));
        display_start();
    }
}




void display_set_bg_bitmap_rgba(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data) {
    for (int j = y; j < y+h; j++) {
        for (int i = x; i < x+w; i++) {
            uint8_t r = *data++;
            uint8_t g = *data++;
            uint8_t b = *data++;
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL))] = color_332(r,g,b);
            // don't use alpha for bg bitmaps, so assign to to r so the compiler doesn't complain
            r = *data++;
        }
    }
}

void display_set_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data) {
    uint32_t c = 0;
    for (int j = y; j < y+h; j++) {
        for (int i = x; i < x+w; i++) {
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = data[c++];
        }
    }
}

void display_get_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data) {
    uint32_t c = 0;
    for (int j = y; j < y+h; j++) {
        for (int i = x; i < x+w; i++) {
            data[c++] = (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)];
        }
    }
}

void display_bg_bitmap_blit(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t x1,uint16_t y1) {
    for (uint16_t j = y1; j < y1+h; j++) {
        for (uint16_t i = x1; i < x1+w; i++) {
            uint16_t src_y = y+(j-y1);
            uint16_t src_x = x+(i-x1);
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = (bg)[(((src_y*(H_RES+OFFSCREEN_X_PX) + src_x)*BYTES_PER_PIXEL) + 0)];
        }
    }    
}



//mem_len = sprite_load(bitmap, mem_pos, [x,y,w,h]) # returns mem_len (w*h*2)
// load a bitmap into fast sprite ram
void display_load_sprite_rgba(uint32_t mem_pos, uint32_t len, uint8_t* data) {
    for (int j = mem_pos; j < mem_pos + len; j=j+BYTES_PER_PIXEL) {
        uint8_t r = *data++;
        uint8_t g = *data++;
        uint8_t b = *data++;
        uint8_t a = *data++;
        if(a==0) { // only full transparent counts
            sprite_ram[j] = ALPHA;
        } else {
            sprite_ram[j] = color_332(r,g,b);
        }
    }

}

void display_load_sprite_raw(uint32_t mem_pos, uint32_t len, uint8_t* data) {
    for (int j = mem_pos; j < mem_pos + len; j=j+BYTES_PER_PIXEL) {
        sprite_ram[j] = *data++;
    }
}


// Palletized version of screenshot. about 3x as fast, RGB332 only
void display_screenshot(char * screenshot_fn) {
    // Blank the display
    display_stop();

    // 778ms total without blanking, pretty good
    uint8_t * screenshot_bb = (uint8_t *) heap_caps_malloc(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    uint8_t * full_pic = (uint8_t*) heap_caps_malloc(H_RES*V_RES*1,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    uint32_t c = 0;
    uint8_t r,g,b,a;
    // Generate the pal
    LodePNGState state;
    lodepng_state_init(&state);
    a = 255; // todo, we could use BG alpha colors? but it doesn't matter
    int err;
    for(uint16_t i=0;i<256;i++) {
        unpack_pal_idx(i, &r, &g, &b);
        // You make the same entry in both the input image and the output image 
        err = lodepng_palette_add(&state.info_png.color, r,g,b,a);
        err = lodepng_palette_add(&state.info_raw, r,g,b,a);        
    }
    state.info_png.color.colortype = LCT_PALETTE; 
    state.info_png.color.bitdepth = 8;
    state.info_raw.colortype = LCT_PALETTE;
    state.info_raw.bitdepth = 8;
    state.encoder.auto_convert = 0;
    int64_t tic = esp_timer_get_time();
    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        display_bounce_empty(screenshot_bb, y*H_RES, H_RES*FONT_HEIGHT*BYTES_PER_PIXEL, NULL);
        for(uint16_t x=0;x<FONT_HEIGHT*H_RES*BYTES_PER_PIXEL;x=x+BYTES_PER_PIXEL) {
            full_pic[c++] = screenshot_bb[x];
        }
    }
    // 45ms
    printf("Took %lld uS to bounce entire screen\n", esp_timer_get_time() - tic);
    tic = esp_timer_get_time();
    uint32_t outsize = 0;
    uint8_t *out;
    err = lodepng_encode(&out, &outsize,full_pic, H_RES, V_RES, &state);
    // 456ms , 4223 b frame
    printf("Took %lld uS to encode as PNG to memory. err %d\n", esp_timer_get_time() - tic, err);
    tic = esp_timer_get_time();
    printf("PNG done encoding. writing %d bytes to file %s\n", outsize, screenshot_fn);
    write_file(screenshot_fn, out, outsize, 1);
    // 268ms 
    printf("Took %lld uS to write to disk\n", esp_timer_get_time() - tic);
    heap_caps_free(out);
    heap_caps_free(screenshot_bb);
    heap_caps_free(full_pic);
    // Restart the display
    display_start();
}


void display_set_bg_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL] = color_332(r,g,b);
}


void display_get_bg_pixel(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t px0 = bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0];
    unpack_rgb_332(px0, r, g, b);
}


void display_tfb_cursor(uint16_t x, uint16_t y) {
    // Put a space char in the TFB if there's nothing here; makes the system draw it
    if(TFB[tfb_y_row*TFB_COLS+tfb_x_col] == 0) TFB[tfb_y_row*TFB_COLS+tfb_x_col] = 32;
    uint8_t f = TFBf[tfb_y_row*TFB_COLS + tfb_x_col];
    f = f | FORMAT_FLASH;
    f = f | FORMAT_INVERSE;
    TFBf[tfb_y_row*TFB_COLS + tfb_x_col] = f;
    TFBfg[tfb_y_row*TFB_COLS + tfb_x_col] = tfb_fg_pal_color;
    TFBbg[tfb_y_row*TFB_COLS + tfb_x_col] = tfb_bg_pal_color;
}

void display_tfb_uncursor(uint16_t x, uint16_t y) {
    if(tfb_x_col < TFB_COLS) {
        uint8_t f = TFBf[tfb_y_row*TFB_COLS + tfb_x_col];
        if(f & FORMAT_FLASH) f = f - FORMAT_FLASH;
        if(f & FORMAT_INVERSE) f = f - FORMAT_INVERSE;
        TFBf[tfb_y_row*TFB_COLS + tfb_x_col] = f;
    }
}

void display_tfb_new_row() {
    display_tfb_uncursor(tfb_x_col, tfb_y_row);
    // Move the pointer to a new row, and scroll the view if necessary
    if(tfb_y_row == TFB_ROWS-1) {
        // We were in the last row, let's scroll the buffer up by moving the TFB up
        for(uint8_t i=0;i<TFB_ROWS-1;i++) {
            memcpy(&TFB[i*TFB_COLS], &TFB[(i+1)*TFB_COLS], TFB_COLS);
            memcpy(&TFBf[i*TFB_COLS], &TFBf[(i+1)*TFB_COLS], TFB_COLS);
            memcpy(&TFBfg[i*TFB_COLS], &TFBfg[(i+1)*TFB_COLS], TFB_COLS);
            memcpy(&TFBbg[i*TFB_COLS], &TFBbg[(i+1)*TFB_COLS], TFB_COLS);
        }
        for(uint8_t i=0;i<TFB_COLS;i++) { 
            TFB[tfb_y_row*TFB_COLS+i] = 0; 
            TFBf[tfb_y_row*TFB_COLS+i] = 0;
            TFBfg[tfb_y_row*TFB_COLS+i] = tfb_fg_pal_color;
            TFBbg[tfb_y_row*TFB_COLS+i] = tfb_bg_pal_color;
        }
    } else {
        // Still got space, just increase the row counter
        tfb_y_row++;
    }
    // No matter what, go back to 0 on cols
    tfb_x_col = 0;
}

uint8_t ansi_parse_digits(char*str, uint16_t j, uint16_t k, uint16_t * digits) {
    uint8_t d = 0;
    uint16_t last_pos = j;
    for(uint16_t i=j; i<k; i++) {
        if(str[i]==';' || i == k-1) {
            if(i==k-1) i++; // this is to make a pretend delimeter at the end
            if(i-last_pos == 3) {
                digits[d++] = ((str[i-3]-'0') * 100) + ((str[i-2]-'0') * 10) + ((str[i-1]-'0'));
            } else if(i-last_pos == 2) {
                digits[d++] = ((str[i-2]-'0') * 10) + ((str[i-1]-'0'));
            } else if(i-last_pos == 1) {
                digits[d++] = (str[i-1]-'0');
            }
            if(d==5) { printf("Warning, more than 5 ANSI format commands in a row\n"); d = 4; }
            last_pos = i+1;
        }
    }
    return d;
}

uint8_t supress_lf = 0;
void display_tfb_str(char*str, uint16_t len, uint8_t format, uint8_t fg_color, uint8_t bg_color) {
    //printf("str len %d format %d is ###", len, format);
    //for(uint16_t i=0;i<len;i++) printf("%c", str[i]);
    //printf("###\n");
    // For each character incoming from micropython
    for(uint16_t i=0;i<len;i++) {
        if(str[i] == 8)  { // backspace , go backwards (don't delete)
            display_tfb_uncursor(tfb_x_col, tfb_y_row);
            if(tfb_x_col > 0) tfb_x_col--;
        } else if(str[i] == 27) { // ANSI
            // we see an esc coming in on stream at i
            // we check if i+1 is [, save i+2 as j, if not goto B
            // we then scan ahead from j until we find a character F at pos k within a-zA-Z. 
            // if F==K: clear to end of line, set stream to k, continue
            // if F==D: get digits between j and k, move cursor backwards that many, set stream to k, continue
            // if F==m: foreach item in delimeter by ; between j and k, process format, set stream to k, continue
            // if F==J: get digit between j and k, do erase per digit code, set stream to k, continue
            // if F==H: see if digits bwetween j and k, if, move cursor to line;column, if not, move to 0,0, set stream to k, continue
            // if F==anything else: printf unsupported, set stream to k, continue
            // B: get next char, print unsupported, set stream to j+1, continue
            if(str[i+1]=='[') {
                uint16_t j=i+2;
                for(uint16_t scan=j;scan<len;scan++) {
                    if((str[scan]>='A' && str[scan]<='Z') || (str[scan]>='a' && str[scan]<='z')) {
                        uint16_t digits[5] = {0};
                        uint16_t k = scan; char F=str[k];
                        if(F == 'K') { // clear to end of line
                            for(uint8_t col=tfb_x_col;col<TFB_COLS;col++) { 
                                TFB[tfb_y_row*TFB_COLS+col] = 0; 
                                TFBf[tfb_y_row*TFB_COLS+col] = 0; 
                                TFBfg[tfb_y_row*TFB_COLS+col] = tfb_fg_pal_color; 
                                TFBbg[tfb_y_row*TFB_COLS+col] = tfb_bg_pal_color ;
                            }    
                            i = k;
                            scan = len;
                        } else if(F=='D') { // move cursor backwards
                            uint8_t d = ansi_parse_digits(str, j, k, digits);
                            if(d==1) { 
                                tfb_x_col = tfb_x_col - digits[0];
                            }
                            i = k;
                            scan = len;
                        } else if(F=='J') { // erase
                            uint8_t d = ansi_parse_digits(str, j, k, digits);
                            if(d==1) {
                                if(digits[0] == 0) { // erase from cursor until end of screen 
                                    printf("nyi , erase from cursor until end of screen\n");
                                } else if(digits[0] == 1) { // erase from cursor to beginning of screen 
                                    printf("nyi, erase from cursor to beginning of screen\n");
                                } else if(digits[0] == 2) { // erase entire screen 
                                    display_reset_tfb();
                                }
                            }
                            i = k;
                            scan = len;

                        } else if(F=='H') { 
                            uint8_t d = ansi_parse_digits(str, j, k, digits); 
                            if(d==2) {
                                // move cursor to line digits[0] and column digits[1]
                                tfb_x_col = digits[1];
                                tfb_y_row = digits[0];
                            } else if(d==0) {
                                // move cursor to 0,0
                                tfb_x_col = 0;
                                tfb_y_row = 0;
                                // Perhaps supress the oncoming LF too? 
                                supress_lf = 1;
                            }
                            i = k;
                            scan = len;
                        } else if(F=='m') { // formatting
                            uint8_t d = ansi_parse_digits(str, j, k, digits);
                            uint8_t ansi_color_idx = 0;
                            // Check to see if the message is a 256 color setting, as it will confuse the other codes below
                            uint8_t c256 = 0;
                            if(digits[0] == 38 && digits[1] == 5) c256 = 1;
                            if(digits[0] == 48 && digits[1] == 5) c256 = 2;
                            for(uint8_t l=0;l<d;l++) {
                                uint8_t code = digits[l];
                                // 256 color mode was sent, so just get the last number in the digits and set color
                                if(c256==1) { 
                                    if(ansi_active_format < 0) ansi_active_format = 0;
                                    if(l==2) {
                                        ansi_active_fg_color = ansi_pal[code];
                                    }
                                } else if(c256==2) {
                                    if(ansi_active_format < 0) ansi_active_format = 0;
                                    if(l==2) {
                                        ansi_active_bg_color = ansi_pal[code];                                    
                                    }
                                } else if(code==0)  { 
                                    // Everything off
                                    ansi_active_format = -1; 
                                    ansi_active_bg_color = tfb_bg_pal_color;  
                                    ansi_active_fg_color = tfb_fg_pal_color; 
                                } else {
                                    // Get ready
                                    if(ansi_active_format < 0) ansi_active_format = 0;
                                    if(code==1)  if (ansi_color_idx < 8) ansi_color_idx += 8; // "bold" color (not font!)
                                    if(code==4)  ansi_active_format = ansi_active_format | FORMAT_UNDERLINE;
                                    if(code==5)  { if(d==1) { ansi_active_format = ansi_active_format | FORMAT_FLASH; } } // check d=1 because of 256 color guy
                                    if(code==6)  ansi_active_format = ansi_active_format | FORMAT_BOLD; // hidden
                                    if(code==7)  ansi_active_format = ansi_active_format | FORMAT_INVERSE;
                                    if(code==9)  ansi_active_format = ansi_active_format | FORMAT_STRIKE;
                                    if(code==22) if (ansi_color_idx >= 8) ansi_color_idx =- 8;
                                    if(code==24) if(ansi_active_format | FORMAT_UNDERLINE) ansi_active_format =- FORMAT_UNDERLINE;
                                    if(code==25) if(ansi_active_format | FORMAT_FLASH) ansi_active_format =- FORMAT_FLASH;
                                    if(code==26) if(ansi_active_format | FORMAT_BOLD) ansi_active_format =- FORMAT_BOLD;
                                    if(code==27) if(ansi_active_format | FORMAT_INVERSE) ansi_active_format =- FORMAT_INVERSE;
                                    if(code==29) if(ansi_active_format | FORMAT_STRIKE) ansi_active_format =- FORMAT_STRIKE;

                                    if(code>=30 && code<=37)  ansi_active_fg_color = ansi_pal[ansi_color_idx + (code-30)]; // color, not including bold color
                                    if(code==39) ansi_active_fg_color = tfb_fg_pal_color;

                                    if(code>=40 && code<=47) ansi_active_bg_color = ansi_pal[ansi_color_idx + (code-40)];
                                    if(code==49) ansi_active_bg_color = tfb_bg_pal_color; // reset   
                                }
                                //printf("code was %d. aaf is now %d, fg now %d bg now %d. color_idx %d\n", code, ansi_active_format, ansi_active_fg_color, ansi_active_bg_color, ansi_color_idx);
                            }
                            i = k;
                            scan = len;
                        } else {
                            printf("Unsupported ANSI code %c\n", F);
                            i = k;
                            scan = len;
                        }
                    } // end if found character
                } // end scan 
            } else {
                printf("Unsupported no CSI ansi %c\n", str[i+1]);
            }



        } else if(str[i] == 10) {
            // If an LF, start a new row
            if(!supress_lf) {
                display_tfb_new_row();
            } else { supress_lf = 0; }
        } else if(str[i]<32) {
            // do nothing with other non-printable chars
        } else { // printable chars
            TFB[tfb_y_row*TFB_COLS+tfb_x_col] = str[i];    
            if(ansi_active_format >= 0 ) {
                TFBf[tfb_y_row*TFB_COLS+tfb_x_col] =ansi_active_format;        
                TFBfg[tfb_y_row*TFB_COLS+tfb_x_col] =ansi_active_fg_color ;      
                TFBbg[tfb_y_row*TFB_COLS+tfb_x_col] =ansi_active_bg_color;        

            } else {
                TFBf[tfb_y_row*TFB_COLS+tfb_x_col] = format;        
                TFBfg[tfb_y_row*TFB_COLS+tfb_x_col] = fg_color;        
                TFBbg[tfb_y_row*TFB_COLS+tfb_x_col] = bg_color;        
            }
            tfb_x_col++;
            if(tfb_x_col == TFB_COLS) {
                display_tfb_new_row();
            }
        }
    }
    // Update the cursor
    display_tfb_cursor(tfb_x_col, tfb_y_row);    
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


// Task runner for the display, inits and then spins in a loop processing frame done ISRs
void display_run(void) {
    // Backlight on GPIO for the 10.1 panel
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BK_LIGHT
    };
    xDisplayTask = xTaskGetCurrentTaskHandle();
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    panel_handle = NULL;
    panel_config.data_width = BYTES_PER_PIXEL*8; 
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
    // Even though we'll only use 8 pins for RGB332 we keep the others to set them low
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
    brightness = DEFAULT_BRIGHTNESS;

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
    TFBfg = (uint8_t*)heap_caps_malloc(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    TFBbg = (uint8_t*)heap_caps_malloc(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);

    x_offsets = (uint16_t*)heap_caps_malloc(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    y_offsets = (uint16_t*)heap_caps_malloc(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    x_speeds = (int16_t*)heap_caps_malloc(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);
    y_speeds = (int16_t*)heap_caps_malloc(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);

    bg_lines = (uint32_t**)heap_caps_malloc(V_RES*sizeof(uint32_t*), MALLOC_CAP_INTERNAL);


    // Init the BG, TFB and sprite layers
    display_reset_bg();
    display_reset_tfb();
    display_reset_sprites();
    
    py_callback = 0;
    vsync_count = 1;
    reported_fps = 1;
    reported_gpu_usage = 0;

    // Start the RGB panel
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    display_pwm_setup();
    display_brightness(brightness); 
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
            reported_fps = 1000000.0 / ((esp_timer_get_time() - tic0) / loop_count);
            reported_gpu_usage = ((float)(bounce_time/bounce_count) / (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (reported_fps))))*100.0;
            if(desync) {
                printf("Desync warning: past %d frames: %2.2f FPS. free time %llduS. bounce time per is %llduS, %2.2f%% of max (%duS). %d desyncs [bc %d]\n", 
                    loop_count,
                    reported_fps, 
                    free_time / loop_count,
                    bounce_time / bounce_count,
                    reported_gpu_usage,
                    (int) (1000000.0 / ((H_RES*V_RES / BOUNCE_BUFFER_SIZE_PX) * (reported_fps))),
                    desync,
                    bounce_count
                );
            }
            bounce_count = 1;
            bounce_time = 0;
            loop_count = 0;
            free_time = 0;
            desync = 0;
            tic0 = esp_timer_get_time();
        }        
    }


}

