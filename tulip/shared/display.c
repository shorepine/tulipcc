#include "display.h"
uint8_t bg_pal_color;
uint8_t tfb_fg_pal_color;
uint8_t tfb_bg_pal_color;
uint8_t ansi_active_bg_color; 
uint8_t ansi_active_fg_color; 
int16_t ansi_active_format;

int16_t last_touch_x[3];
int16_t last_touch_y[3];
uint8_t touch_held;

uint8_t mouse_pointer_status = 0;

uint8_t tfb_active;
uint8_t tfb_y_row; 
uint8_t tfb_x_col;
int32_t vsync_count;
uint8_t brightness;
float reported_fps;
float reported_gpu_usage;

uint8_t *collision_bitfield;
// RAM for sprites and background FB
uint8_t *sprite_ram; // in IRAM
uint8_t * bg; // in SPIRAM
uint8_t * bg_tfb;

uint8_t * sprite_ids;
uint16_t *sprite_x_px;//[SPRITES]; 
uint16_t *sprite_y_px;//[SPRITES]; 
uint16_t *sprite_w_px;//[SPRITES]; 
uint16_t *sprite_h_px;//[SPRITES]; 
uint8_t *sprite_vis;//[SPRITES];
uint32_t *sprite_mem;//[SPRITES];

uint8_t * lv_buf;

uint8_t *TFB;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBfg;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBbg;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBf;//[TFB_ROWS][TFB_COLS];
uint16_t *TFB_pxlen;
int16_t *x_offsets;//[V_RES];
int16_t *y_offsets;//[V_RES];
int16_t *x_speeds;//[V_RES];
int16_t *y_speeds;//[V_RES];

uint32_t **bg_lines;//[V_RES];

// Defaults for runtime display params
uint16_t PIXEL_CLOCK_MHZ = DEFAULT_PIXEL_CLOCK_MHZ;
uint8_t tfb_active = 1;
uint8_t gpu_log = 0;

int16_t lvgl_is_repl = 0;

// lookup table for Tulip's "pallete" to the 16-bit colorspace needed by LVGL and T-deck

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



uint8_t check_dim_xy(uint16_t x, uint16_t y) {
    if(x >= OFFSCREEN_X_PX + H_RES || y >= OFFSCREEN_Y_PX+V_RES) return 0;
    return 1;
}

uint8_t check_dim_xywh(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if(!check_dim_xy(x,y)) return 0;
    if(!check_dim_xy(x+w-1, y+h-1)) return 0;
    return 1;
}

// RRRGGGBB -> 
void unpack_rgb_332_repeat(uint8_t px0, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r=0; *g=0; *b=0;
    *r = (px0 & 0xe0) | ((px0 & 0xe0)>>3) | ((px0&0xc0)>>6);
    *g = ((px0 & 0x1c) << 3) | (px0 & 0x1c) | ((px0&0x18) >> 3); 
    *b = (px0 & 0x03) | ((px0 & 0x03) << 2) | ((px0 & 0x03) << 4) | ((px0 & 0x03) << 6);
}

// Given a single uint (0-255 for RGB332, 0-65535 for RGB565), return r, g, b
void unpack_pal_idx(uint16_t pal_idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    unpack_rgb_332_repeat(pal_idx & 0xff, r, g, b);
}

// Given a single uint (0-255 for RGB332, 0-65535 for RGB565), return r, g, b
void unpack_pal_idx_wide(uint16_t pal_idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    unpack_rgb_332_repeat(pal_idx & 0xff, r, g, b);
}

// Given an ansi pal index (0-255 right now), return r g b
void unpack_ansi_idx(uint8_t ansi_idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    unpack_rgb_332_repeat(ansi_idx, r, g, b);
}

// Return a packed 8-bit number for RRRGGGBB
uint8_t color_332(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t ret = 0;
    ret |= (red&0xe0);
    ret |= (green&0xe0) >> 3;
    ret |= (blue&0xc0) >> 6;
    return ret;
}


// RRRRRGGG GGGBBBBB -> RRRGGGBB
// >> 6

uint8_t rgb565to332(uint16_t rgb565) {
    return (rgb565 >> 6 & 0xe0)  | (rgb565 >> 6 & 0x1c) | (rgb565 >> 3 & 0x3);
}

// Python callback
extern void tulip_frame_isr(); 

uint8_t spriteno_activated;

bool display_frame_done_generic() {
    // Update the scroll
    for(uint16_t i=0;i<V_RES;i++) {
        x_offsets[i] = x_offsets[i] + x_speeds[i];
        y_offsets[i] = y_offsets[i] + y_speeds[i];
        x_offsets[i] = x_offsets[i] % (H_RES+OFFSCREEN_X_PX);
        y_offsets[i] = y_offsets[i] % (V_RES+OFFSCREEN_Y_PX);
        bg_lines[i] = (uint32_t*)&bg[(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL*y_offsets[i] + x_offsets[i]*BYTES_PER_PIXEL];
    }
    if(mouse_pointer_status) {
        sprite_x_px[0] = mouse_x_pos;
        sprite_y_px[0] = mouse_y_pos;
    }
    tulip_frame_isr();
    vsync_count++; 
    return true;
}

void display_swap() {
    for(uint16_t i=0;i<V_RES;i++) x_offsets[i] = (x_offsets[i] + H_RES) % (H_RES+OFFSCREEN_X_PX);
}


// Thanks dan for this code... packs a 32x32 hit matrix into 62 bytes
uint8_t collide_mask_get(uint8_t a, uint8_t b) {
    uint16_t field = 0;
    if(a==b) return 1;
    if(a>b) {
         field = a * (a - 1) / 2 + b;
    } else {
         field = b * (b - 1) / 2 + a;
    }
    if(field/8 > 61) {
        fprintf(stderr, "get bad field %d a %d b %d \n", field, a, b);
        return 0;
    }
    return collision_bitfield[field / 8] & 1 << (field % 8) ;
}

// Timers / counters for perf

// Two buffers are filled by this function, one gets filled while the other is drawn (via GDMA to the LCD.) 
// Each call fills a certain number of lines, set by BOUNCE_BUFFER_SIZE_PX in setup (it's currently 12 lines / 1 row of text)
int64_t bounce_time = 0;
uint32_t bounce_count = 1;

bool IRAM_ATTR display_bounce_empty(void *bounce_buf, int pos_px, int len_bytes, void *user_ctx) {
    int64_t tic=get_time_us(); // start the timer
    int16_t touch_x = last_touch_x[0];
    int16_t touch_y = last_touch_y[0];
    uint8_t touch_held_local = touch_held;

    uint16_t starting_display_row_px = pos_px / H_RES;
    uint8_t bounce_total_rows_px = len_bytes / H_RES;
    uint8_t * b = (uint8_t*)bounce_buf;
    // Copy the bg then the TFB over 
    for(uint8_t rows_relative_px=0;rows_relative_px<bounce_total_rows_px;rows_relative_px++) {
        uint8_t * b_ptr = b+(H_RES*rows_relative_px);
        uint16_t y = (starting_display_row_px + rows_relative_px) % V_RES;
        memcpy(b_ptr, bg_lines[y], H_RES); 
        if(tfb_active) memcpy(b_ptr, bg_tfb + (y * H_RES),TFB_pxlen[y]);
    
        if(spriteno_activated) {
            memset(sprite_ids, 255, H_RES);
            if(touch_held_local && touch_y == y) {
                if(touch_x >= 0 && touch_x < H_RES) {
                    sprite_ids[touch_x] = SPRITES-1;
                }
            }
            for(uint8_t s=0;s<spriteno_activated;s++) {
                if(sprite_vis[s]==SPRITE_IS_SPRITE) {
                    if(y >= sprite_y_px[s] && y < sprite_y_px[s]+sprite_h_px[s]) {
                        // this sprite is on this line 
                        // compute x and y (relative to the sprite!)
                        uint8_t * sprite_data = &sprite_ram[sprite_mem[s]];
                        uint16_t relative_sprite_y_px = y - sprite_y_px[s];
                        for(uint16_t col_px=sprite_x_px[s]; col_px < sprite_x_px[s] + sprite_w_px[s]; col_px++) {
                            if(col_px < H_RES) {
                                uint16_t relative_sprite_x_px = col_px - sprite_x_px[s];
                                uint8_t b0 = sprite_data[relative_sprite_y_px * sprite_w_px[s] + relative_sprite_x_px  ] ;
                                if(b0 != ALPHA) {
                                    b[rows_relative_px*H_RES + col_px] = b0;
                                    // Only update collisions on non-alpha pixels
                                    uint8_t overlap_sprite = sprite_ids[col_px];
                                    if(overlap_sprite!=255) { // sprite already here!
                                        uint16_t field = s * (s - 1) / 2 + overlap_sprite;
                                        collision_bitfield[field / 8] |= 1 << (field % 8);
                                    }
                                    sprite_ids[col_px] = s;
                                }
                            }
                        } // end for each column
                    } // end if this row has a sprite on it 
                }
            } // for each sprite
        } // end if any sprites on
    } // for each row
    bounce_time += (get_time_us() - tic); // stop timer
    bounce_count++;

    return false;
}

// set tfb_row_hint to -1 for everything
void display_tfb_update(int8_t tfb_row_hint) { 
    if(!tfb_active) { return; }

    uint16_t bounce_row_start = 0;
    uint16_t bounce_row_end = V_RES;
    if(tfb_row_hint >= 0) {
        bounce_row_start = tfb_row_hint * FONT_HEIGHT;
        bounce_row_end = bounce_row_start + FONT_HEIGHT;
    }
    for(uint16_t bounce_row_px=bounce_row_start;bounce_row_px<bounce_row_end;bounce_row_px++) {
        memset(bg_tfb + (bounce_row_px*H_RES), 0, H_RES);

        uint8_t tfb_row = bounce_row_px / FONT_HEIGHT;
        uint8_t tfb_row_offset_px = bounce_row_px % FONT_HEIGHT; 
        uint8_t tfb_col = 0;
        while(TFB[tfb_row*TFB_COLS+tfb_col]!=0 && tfb_col < TFB_COLS) {
            #ifndef TDECK
                uint8_t data = font_8x12_r[TFB[tfb_row*TFB_COLS+tfb_col]][tfb_row_offset_px];
            #else
                uint8_t data = portfolio_glyph_bitmap[(TFB[tfb_row*TFB_COLS+tfb_col] -32) * 8 + tfb_row_offset_px];
            #endif
            uint8_t format = TFBf[tfb_row*TFB_COLS+tfb_col];
            uint8_t fg_color = TFBfg[tfb_row*TFB_COLS+tfb_col];
            uint8_t bg_color = TFBbg[tfb_row*TFB_COLS+tfb_col];

            // If you're looking at this code just know the unrolled versions were 1.5x faster than loops on esp32s3
            // I'm sure there's more to do but this is the best we could get it for now
            uint8_t * bptr = bg_tfb + (bounce_row_px*H_RES + tfb_col*FONT_WIDTH);
            if(bg_color == ALPHA) {
                if(format & FORMAT_INVERSE) {
                    if(!((data) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    #ifndef TDECK
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                    #endif
                } else {
                    if((data) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    #ifndef TDECK
                        bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                        bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; 
                    #endif
                }
            } else {
                if(format & FORMAT_INVERSE) {
                    if(!((data) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    #ifndef TDECK
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; else *(bptr) = bg_color;
                    #endif
                } else {
                    if((data) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    #ifndef TDECK
                        bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                        bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                    #endif
                }
            }
            tfb_col++;
        }
        TFB_pxlen[bounce_row_px] = tfb_col*FONT_WIDTH;
    }

}
void display_reset_bg() {
    bg_pal_color = TULIP_TEAL;
    for(int i=0;i<(H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX);i++) { 
        bg[i] = bg_pal_color; 
    }
    
    // init the scroll pointer to the top left of the fb 
    for(int i=0;i<V_RES;i++) {
        bg_lines[i] = (uint32_t*)&bg[(H_RES+OFFSCREEN_X_PX)*i];
        x_offsets[i] = 0;
        y_offsets[i] = i;
        x_speeds[i] = 0;
        y_speeds[i] = 0;
    }

}

void display_reset_tfb() {
    // Clear out the TFB
    tfb_fg_pal_color = color_332(255,255,255);
    tfb_bg_pal_color = TULIP_TEAL;
    for(uint i=0;i<TFB_ROWS*TFB_COLS;i++) {
        TFB[i]=0;
        TFBfg[i]=tfb_fg_pal_color;
        TFBbg[i]=tfb_bg_pal_color;
        TFBf[i]=0;
    }
    for(uint16_t i=0;i<V_RES;i++) TFB_pxlen[i] = 0;
    tfb_y_row = 0;
    tfb_x_col = 0;
    ansi_active_format = -1; // no override
    ansi_active_fg_color = tfb_fg_pal_color; 
    ansi_active_bg_color = tfb_bg_pal_color;
    tfb_active = 1;
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
    for(uint8_t i=0;i<62;i++) collision_bitfield[i] = 0;
    for(uint32_t i=0;i<SPRITE_RAM_BYTES;i++) sprite_ram[i] = 0;
    spriteno_activated = 0;
}


void display_reset_touch() {
    for(uint8_t i=0;i<3;i++) {
        last_touch_x[i] = -1;
        last_touch_y[i] = -1;
    }
}


void display_invert_bg(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if(check_dim_xywh(x,y,w,h)) {
        for (int j = y; j < y+h; j++) {
            for (int i = x; i < x+w; i++) {
                if(j<V_RES+OFFSCREEN_Y_PX && i < H_RES+OFFSCREEN_X_PX) {
                    (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = 255 - (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)];
                }
            }
        }
    } else { fprintf(stderr, "invert_bg %d %d %d %d\n", x,y,w,h); }
}

void display_set_bg_bitmap_rgba(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data) {
    if(1) { // check_dim_xywh(x,y,w,h)) {
        for (int j = y; j < y+h; j++) {
            for (int i = x; i < x+w; i++) {
                uint8_t r = *data++;
                uint8_t g = *data++;
                uint8_t b = *data++;
                uint8_t a = *data++; 
                if(j<V_RES+OFFSCREEN_Y_PX && i < H_RES+OFFSCREEN_X_PX) {
                    if(a!=0) {
                        (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL))] = color_332(r,g,b);
                    }
                }
            }
        }
    } else { fprintf(stderr, "bg_bitmap_rgba %d %d %d %d\n", x,y,w,h); }
}

void display_set_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data) {
    if(check_dim_xywh(x,y,w,h)) {
        uint32_t c = 0;
        for (int j = y; j < y+h; j++) {
            for (int i = x; i < x+w; i++) {
                uint8_t pixel = data[c++];
                if(j<V_RES+OFFSCREEN_Y_PX && i < H_RES+OFFSCREEN_X_PX) {
                    if(pixel != ALPHA) {
                        (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = pixel;
                    }
                }
            }
        }
    } else { fprintf(stderr, "bg_bitmap_raw %d %d %d %d\n", x,y,w,h); }
}

void display_get_bg_bitmap_raw(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t * data) {
    if(check_dim_xywh(x,y,w,h)) {
        uint32_t c = 0;
        for (int j = y; j < y+h; j++) {
            for (int i = x; i < x+w; i++) {
                data[c++] = (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)];
            }
        }
    } else { fprintf(stderr, "get_bitmap_raw %d %d %d %d\n", x,y,w,h); }
}

void display_bg_bitmap_blit(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t x1,uint16_t y1) {
    if(check_dim_xywh(x,y,w,h)) {
        for (uint16_t j = y1; j < y1+h; j++) {
            for (uint16_t i = x1; i < x1+w; i++) {
                uint16_t src_y = y+(j-y1);
                uint16_t src_x = x+(i-x1);
                if(j<V_RES+OFFSCREEN_Y_PX && i < H_RES+OFFSCREEN_X_PX) {
                    (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = (bg)[(((src_y*(H_RES+OFFSCREEN_X_PX) + src_x)*BYTES_PER_PIXEL) + 0)];
                }
            }
        }    
    } else { fprintf(stderr, "bg_bitmap_blit %d %d %d %d %d %d\n", x,y,w,h, x1, y1); }
}

void display_bg_bitmap_blit_alpha(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t x1,uint16_t y1) {
    if(check_dim_xywh(x,y,w,h) && check_dim_xywh(x1,y1, w, h)) {
        for (uint16_t j = y1; j < y1+h; j++) {
            for (uint16_t i = x1; i < x1+w; i++) {
                uint16_t src_y = y+(j-y1);
                uint16_t src_x = x+(i-x1);
                if(j<V_RES+OFFSCREEN_Y_PX && i < H_RES+OFFSCREEN_X_PX) {
                    uint8_t c = (bg)[(((src_y*(H_RES+OFFSCREEN_X_PX) + src_x)*BYTES_PER_PIXEL) + 0)];
                    if(c != ALPHA) {
                        (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = (bg)[(((src_y*(H_RES+OFFSCREEN_X_PX) + src_x)*BYTES_PER_PIXEL) + 0)];
                    }
                }
            }
        }    
    } else { fprintf(stderr, "bg_bitmap_blit_alpha %d %d %d %d %d %d\n", x,y,w,h, x1, y1); }
}



//mem_len = sprite_load(bitmap, mem_pos, [x,y,w,h]) # returns mem_len (w*h*2)
// load a bitmap into fast sprite ram
void display_load_sprite_rgba(uint32_t mem_pos, uint32_t len, uint8_t* data) {
    if(mem_pos < SPRITE_RAM_BYTES && mem_pos+len < SPRITE_RAM_BYTES) {
        for (uint32_t j = mem_pos; j < mem_pos + len; j=j+BYTES_PER_PIXEL) {
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
}

void display_load_sprite_raw(uint32_t mem_pos, uint32_t len, uint8_t* data) {
    if(mem_pos < SPRITE_RAM_BYTES && mem_pos+len < SPRITE_RAM_BYTES) {
        for (uint32_t j = mem_pos; j < mem_pos + len; j=j+BYTES_PER_PIXEL) {
            sprite_ram[j] = *data++;
        }
    }    
}


const uint8_t pointer_bitmap_xys[96] = {
    0,0, 0,1, 1,1, 0,2, 2,2, 0,3, 3,3, 0,4, 4,4, 0,5, 5,5, 0,6, 6,6, 0,7, 7,7, 0,8, 8,8,
    0,9, 9,9, 0,10, 10,10, 0,11, 11,11, 0,12, 7,12, 8,12, 9,12, 10,12, 11,12, 0,13, 4,13, 7,13,
    0,14, 3,14, 5,14, 8,14, 0,15, 2,15, 5,15, 8,14, 0,16, 1,16, 6,16, 9,16, 7,17, 10,17, 8,18, 9,18
};

void enable_mouse_pointer() {
    // just overwrite sprite ram for this, near the end of the ram slice ? 
    uint8_t w=12;
    uint8_t h=19;
    for(uint32_t i=SPRITE_RAM_BYTES-(w*h); i<SPRITE_RAM_BYTES; i++) {
        sprite_ram[i] = ALPHA;
    }
    for(uint8_t i=0;i<96;i=i+2) {
        uint8_t x0 = pointer_bitmap_xys[i];
        uint8_t y0 = pointer_bitmap_xys[i+1];
        if(i<93 && pointer_bitmap_xys[i+3]==y0) {
            if(x0!=3 && x0!=2 && x0!=1) { // skip the end of the tail
                for(uint8_t j=x0; j<pointer_bitmap_xys[i+2]; j++) {
                    sprite_ram[(SPRITE_RAM_BYTES-(w*h)) + (y0*w + j)] = 244;
                }
            }
        }
        sprite_ram[(SPRITE_RAM_BYTES-(w*h)) + (y0*w + x0)] = 162;
    }
    uint8_t spriteno = 0;
    spriteno_activated++;
    sprite_w_px[spriteno] = w;
    sprite_h_px[spriteno] = h;
    sprite_mem[spriteno] = SPRITE_RAM_BYTES-(w*h);
    sprite_vis[spriteno] = SPRITE_IS_SPRITE;
    mouse_pointer_status = 1;
}

void disable_mouse_pointer() {
    mouse_pointer_status = 0;
    sprite_vis[0] = 0;
    spriteno_activated--;
}

// Palletized version of screenshot. about 3x as fast, RGB332 only
void display_screenshot(char * screenshot_fn) {
    // Blank the display
    display_stop();

    uint8_t * screenshot_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    uint32_t c = 0;
    uint8_t r,g,b,a;

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

    (void)err;
    state.info_png.color.colortype = LCT_PALETTE; 
    state.info_png.color.bitdepth = 8;
    state.info_raw.colortype = LCT_PALETTE;
    state.info_raw.bitdepth = 8;
    state.encoder.auto_convert = 0;

    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        c=0;
        display_bounce_empty(screenshot_bb, y*H_RES, H_RES*FONT_HEIGHT*BYTES_PER_PIXEL, NULL);
        for(uint8_t sy=0;sy<FONT_HEIGHT;sy++) {
            for(uint16_t x=0;x<H_RES;x++) {
                bg_tfb[(y+sy)*H_RES + x] = screenshot_bb[c++];
            }
        }
    }
    // now bg_tfb has rendered sprites/tfb/etc on screen

    // encode png
    uint32_t outsize = 0;
    uint8_t *out;
    err = lodepng_encode(&out, (size_t*)&outsize,bg_tfb, H_RES, V_RES, &state);
    write_file(screenshot_fn, out, outsize, 1);
    free_caps(out);
    free_caps(screenshot_bb);

    // redraw the tfb
    display_tfb_update(-1);
    // Restart the display
    display_start();
}

void display_set_bg_pixel_pal(uint16_t x, uint16_t y, uint8_t pal_idx) {
    if(check_dim_xy(x,y)) {
        bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL] = pal_idx;    
    }
}

void display_set_bg_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    if(check_dim_xy(x,y)) {
        bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL] = color_332(r,g,b);
    }
}


void display_get_bg_pixel(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b) {
    if(check_dim_xy(x,y)) {
        uint8_t px0 = bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0];
        unpack_rgb_332_repeat(px0, r, g, b);
    } else {
        *r = 0; *g =0; *b = 0;
    }
}

uint8_t display_get_bg_pixel_pal(uint16_t x, uint16_t y) {
    if(check_dim_xy(x,y)) {
        return bg[y*(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL + x*BYTES_PER_PIXEL + 0];
    }
    return 0;
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
        // update the whole screen
        display_tfb_update(-1);
    } else {
        // Still got space, just increase the row counter
        display_tfb_update(tfb_y_row);
        tfb_y_row++;
    }
    // No matter what, go back to 0 on cols
    tfb_x_col = 0;
}

uint8_t ansi_parse_digits( unsigned char*str, uint16_t j, uint16_t k, uint16_t * digits) {
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
            if(d==5) { fprintf(stderr,"Warning, more than 5 ANSI format commands in a row\n"); d = 4; }
            last_pos = i+1;
        }
    }
    return d;
}

uint32_t utf8_esc = 0;
uint8_t supress_lf = 0;
void display_tfb_str(unsigned char*str, uint16_t len, uint8_t format, uint8_t fg_color, uint8_t bg_color) {

    //fprintf(stderr,"str len %d format %d is ### ", len, format);
    //for(uint16_t i=0;i<len;i++) fprintf(stderr, "[%c/%d] ", str[i], str[i]);
    //fprintf(stderr, "###\n");
    // For each character incoming from micropython
    for(uint16_t i=0;i<len;i++) {
        if(str[i] == 8)  { // backspace , go backwards (don't delete)
            display_tfb_uncursor(tfb_x_col, tfb_y_row);
            if(tfb_x_col > 0) tfb_x_col--;
        }
        if(str[i]>127) { // unicode
            uint32_t esc = 0;
            uint8_t code= convert_utf8_to_cp437(str[i], &esc);
            while(code == 0) {
                i++;
                code = convert_utf8_to_cp437(str[i], &esc);
            }
            str[i] = code;
        }
        if(str[i] == 27) { // ANSI
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
                        uint16_t k = scan;  unsigned char F=str[k];
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
                                    fprintf(stderr,"nyi , erase from cursor until end of screen\n");
                                } else if(digits[0] == 1) { // erase from cursor to beginning of screen 
                                    fprintf(stderr,"nyi, erase from cursor to beginning of screen\n");
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
                                    if(code==22) if (ansi_color_idx >= 8) ansi_color_idx = ansi_color_idx - 8;
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
                            fprintf(stderr,"Unsupported ANSI code %c\n", F);
                            i = k;
                            scan = len;
                        }
                    } // end if found character
                } // end scan 
            } else {
                fprintf(stderr,"Unsupported no CSI ansi %c\n", str[i+1]);
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
    display_tfb_update(tfb_y_row);
}


extern float unix_display_set_clock(uint8_t mhz);
void display_set_clock(uint8_t mhz) {  
    if(mhz > 1 && mhz < 50) {
#ifdef ESP_PLATFORM
        esp_display_set_clock(mhz);
#else
        unix_display_set_clock(mhz);
#endif
    }
}

void display_teardown(void) {
    free_caps(bg); bg = NULL;
    free_caps(bg_tfb); bg_tfb = NULL;
    free_caps(TFB_pxlen); TFB_pxlen = NULL;
    free_caps(sprite_ids); sprite_ids = NULL;
    free_caps(lv_buf); lv_buf = NULL;
    free_caps(sprite_ram); sprite_ram = NULL; 
    free_caps(sprite_x_px); sprite_x_px = NULL;
    free_caps(sprite_y_px); sprite_y_px = NULL;
    free_caps(sprite_w_px); sprite_w_px = NULL;
    free_caps(sprite_h_px); sprite_h_px = NULL;
    free_caps(sprite_vis); sprite_vis = NULL;
    free_caps(sprite_mem); sprite_mem = NULL;
    free_caps(collision_bitfield); collision_bitfield = NULL;
    free_caps(TFB); TFB = NULL;
    free_caps(TFBf); TFBf = NULL; 
    free_caps(TFBfg); TFBfg = NULL;
    free_caps(TFBbg); TFBbg = NULL;
    free_caps(x_offsets); x_offsets = NULL;
    free_caps(y_offsets); y_offsets = NULL;
    free_caps(x_speeds); x_speeds = NULL;
    free_caps(y_speeds); y_speeds = NULL;
    free_caps(bg_lines); bg_lines = NULL;
}


void lv_flush_cb_8b(lv_display_t * display, const lv_area_t * area, unsigned char * px_map)
{
    // Inform LVGL that you are ready with the flushing and buf is not used anymore
    lv_display_flush_ready(display);
}


// Shim for lvgl to read ticks
uint32_t u32_ticks_ms() {
    return (uint32_t) get_ticks_ms();
}

void lvgl_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data);


void lvgl_input_kb_read_cb(lv_indev_t * indev, lv_indev_data_t*data) {
    lvgl_keyboard_read(indev, data);
}

void lvgl_input_read_cb(lv_indev_t * indev, lv_indev_data_t*data) {
    if(touch_held) {
        if(last_touch_x[0] >= 0 && last_touch_x[0] < H_RES && last_touch_y[0] >= 0 && last_touch_y[0] < V_RES) {
            data->point.x = last_touch_x[0];
            data->point.y = last_touch_y[0];
            data->state = LV_INDEV_STATE_PRESSED;
        }
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}


void my_log_cb(lv_log_level_t level, const char * buf)
{
    fprintf(stderr, "%s\n", buf);
}

/*
extern void get_lvgl_font_from_tulip(uint8_t font_no, lv_font_t *outfont);

lv_font_t lv_font_tulip_0;
lv_font_t lv_font_tulip_1;
lv_font_t lv_font_tulip_2;
lv_font_t lv_font_tulip_3;
lv_font_t lv_font_tulip_4;
lv_font_t lv_font_tulip_5;
lv_font_t lv_font_tulip_6;
lv_font_t lv_font_tulip_7;
lv_font_t lv_font_tulip_8;
lv_font_t lv_font_tulip_9;
lv_font_t lv_font_tulip_10;
lv_font_t lv_font_tulip_11;
lv_font_t lv_font_tulip_12;
lv_font_t lv_font_tulip_13;
lv_font_t lv_font_tulip_14;
lv_font_t lv_font_tulip_15;
lv_font_t lv_font_tulip_16;
lv_font_t lv_font_tulip_17;
lv_font_t lv_font_tulip_18;
*/

lv_indev_t * indev;
lv_indev_t * indev_kb;
lv_display_t * lv_display;

void setup_lvgl() {
    // Setup LVGL for UI etc
    lv_init();

    //lv_log_register_print_cb(my_log_cb);
    lv_display = lv_display_create(H_RES+OFFSCREEN_X_PX, V_RES+OFFSCREEN_Y_PX);
    lv_display_set_physical_resolution(lv_display, H_RES, V_RES); // for touchpad
    lv_display_set_offset(lv_display,0,0);
    lv_display_set_antialiasing(lv_display, 0);
    lv_display_set_color_format(lv_display, LV_COLOR_FORMAT_RGB332);
    lv_display_set_flush_cb(lv_display, lv_flush_cb_8b);
    lv_display_set_buffers(lv_display, bg, NULL, (H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX), LV_DISPLAY_RENDER_MODE_DIRECT);
    
    lv_tick_set_cb(u32_ticks_ms);

    // Create a input device (uses tulip.touch())
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);   
    lv_indev_set_read_cb(indev, lvgl_input_read_cb);  

    // Also create a keyboard input device 
    indev_kb = lv_indev_create();
    lv_indev_set_type(indev_kb, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev_kb, lvgl_input_kb_read_cb);  
/*
    get_lvgl_font_from_tulip(0, &lv_font_tulip_0);
    get_lvgl_font_from_tulip(1, &lv_font_tulip_1);
    get_lvgl_font_from_tulip(2, &lv_font_tulip_2);
    get_lvgl_font_from_tulip(3, &lv_font_tulip_3);
    get_lvgl_font_from_tulip(4, &lv_font_tulip_4);
    get_lvgl_font_from_tulip(5, &lv_font_tulip_5);
    get_lvgl_font_from_tulip(6, &lv_font_tulip_6);
    get_lvgl_font_from_tulip(7, &lv_font_tulip_7);
    get_lvgl_font_from_tulip(8, &lv_font_tulip_8);
    get_lvgl_font_from_tulip(9, &lv_font_tulip_9);
    get_lvgl_font_from_tulip(10, &lv_font_tulip_10);
    get_lvgl_font_from_tulip(11, &lv_font_tulip_11);
    get_lvgl_font_from_tulip(12, &lv_font_tulip_12);
    get_lvgl_font_from_tulip(13, &lv_font_tulip_13);
    get_lvgl_font_from_tulip(14, &lv_font_tulip_14);
    get_lvgl_font_from_tulip(15, &lv_font_tulip_15);
    get_lvgl_font_from_tulip(16, &lv_font_tulip_16);
    get_lvgl_font_from_tulip(17, &lv_font_tulip_17);
    get_lvgl_font_from_tulip(18, &lv_font_tulip_18);
    */
}




void display_init(void) {
    // 12 divides into 600, 480, 240
    // Create the background FB
    // 1536000 bytes
    bg = (uint8_t*)calloc_caps(32, 1, (H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX)*BYTES_PER_PIXEL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    // 614400 bytes
    bg_tfb = (uint8_t*)calloc_caps(32, 1, (H_RES*V_RES), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    // And various ptrs
    sprite_ids = (uint8_t*)malloc_caps(H_RES *  sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_ram = (uint8_t*)malloc_caps(SPRITE_RAM_BYTES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_x_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_y_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_w_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_h_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_vis = (uint8_t*)malloc_caps(SPRITES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_mem = (uint32_t*)malloc_caps(SPRITES*sizeof(uint32_t), MALLOC_CAP_INTERNAL);
    collision_bitfield = (uint8_t*)malloc_caps(128, MALLOC_CAP_INTERNAL);
    TFB_pxlen = (uint16_t*)malloc_caps(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);


    TFB = (uint8_t*)malloc_caps(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    TFBf = (uint8_t*)malloc_caps(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    TFBfg = (uint8_t*)malloc_caps(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    TFBbg = (uint8_t*)malloc_caps(TFB_ROWS*TFB_COLS*sizeof(uint8_t), MALLOC_CAP_INTERNAL);


    x_offsets = (int16_t*)malloc_caps(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    y_offsets = (int16_t*)malloc_caps(V_RES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    x_speeds = (int16_t*)malloc_caps(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);
    y_speeds = (int16_t*)malloc_caps(V_RES*sizeof(int16_t), MALLOC_CAP_INTERNAL);

    bg_lines = (uint32_t**)malloc_caps(V_RES*sizeof(uint32_t*), MALLOC_CAP_INTERNAL);


    // Init the BG, TFB and sprite and UI layers
    display_reset_bg();
    display_reset_tfb();
    display_reset_sprites();
    display_reset_touch();

    vsync_count = 1;
    reported_fps = TARGET_DESKTOP_FPS;
    reported_gpu_usage = 0;
    touch_held = 0;


}

