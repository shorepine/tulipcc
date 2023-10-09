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
uint8_t tfb_log;

uint8_t tfb_active;
uint8_t tfb_y_row; 
uint8_t tfb_x_col; 
uint8_t task_screenshot;
uint8_t task_start;
uint8_t task_stop;
int32_t vsync_count;
uint8_t brightness;
float reported_fps;
float reported_gpu_usage;

uint8_t *collision_bitfield;
// RAM for sprites and background FB
uint8_t *sprite_ram; // in IRAM
uint8_t * bg; // in SPIRAM

uint8_t * sprite_ids;
uint16_t *sprite_x_px;//[SPRITES]; 
uint16_t *sprite_y_px;//[SPRITES]; 
uint16_t *sprite_w_px;//[SPRITES]; 
uint16_t *sprite_h_px;//[SPRITES]; 
uint8_t *sprite_vis;//[SPRITES];
uint32_t *sprite_mem;//[SPRITES];

uint8_t *TFB;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBfg;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBbg;//[TFB_ROWS][TFB_COLS];
uint8_t *TFBf;//[TFB_ROWS][TFB_COLS];
int16_t *x_offsets;//[V_RES];
int16_t *y_offsets;//[V_RES];
int16_t *x_speeds;//[V_RES];
int16_t *y_speeds;//[V_RES];

uint32_t **bg_lines;//[V_RES];

// Defaults for runtime display params
uint16_t H_RES = DEFAULT_H_RES;
uint16_t V_RES = DEFAULT_V_RES;
uint16_t H_RES_D = DEFAULT_H_RES;
uint16_t V_RES_D = DEFAULT_V_RES;
uint16_t OFFSCREEN_X_PX = DEFAULT_OFFSCREEN_X_PX;
uint16_t OFFSCREEN_Y_PX = DEFAULT_OFFSCREEN_Y_PX;
uint16_t PIXEL_CLOCK_MHZ = DEFAULT_PIXEL_CLOCK_MHZ;
uint16_t BOUNCE_BUFFER_SIZE_PX;
uint16_t TFB_ROWS, TFB_COLS;
uint8_t tfb_active = 1;
uint8_t tfb_log = 0;
uint8_t gpu_log = 0;


uint8_t check_dim_xy(uint16_t x, uint16_t y) {
    if(x >= OFFSCREEN_X_PX + H_RES || y >= OFFSCREEN_Y_PX+V_RES) return 0;
    return 1;
}

uint8_t check_dim_xywh(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if(!check_dim_xy(x,y)) return 0;
    if(!check_dim_xy(x+w-1, y+h-1)) return 0;
    return 1;
}

// RRRGGGBB
void unpack_rgb_332(uint8_t px0, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = px0 & 0xe0;
    *g = (px0 << 3) & 0xe0;
    *b = (px0 << 6) & 0xc0;
}

// RRRGGGBB , but expand any non-zero colors with 1111s (won't matter for display as there's no wires there)
void unpack_rgb_332_wide(uint8_t px0, uint16_t *r, uint16_t *g, uint16_t *b) {
    *r = (px0 & 0xe0);
    if(*r != 0) *r |= 0x1f;
    *g = ((px0 << 3) & 0xe0);
    if(*g != 0) *g |= 0x1f;
    *b = ((px0 << 6) & 0xc0);
    if(*b != 0) *b |= 0x3f;
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


bool display_frame_done_generic() {
    // Update the scroll
    for(uint16_t i=0;i<V_RES;i++) {
        x_offsets[i] = x_offsets[i] + x_speeds[i];
        y_offsets[i] = y_offsets[i] + y_speeds[i];
        x_offsets[i] = x_offsets[i] % (H_RES+OFFSCREEN_X_PX);
        y_offsets[i] = y_offsets[i] % (V_RES+OFFSCREEN_Y_PX);
        bg_lines[i] = (uint32_t*)&bg[(H_RES+OFFSCREEN_X_PX)*BYTES_PER_PIXEL*y_offsets[i] + x_offsets[i]*BYTES_PER_PIXEL];
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
int64_t bounce_time = 0;
uint32_t bounce_count = 0;
int32_t desync = 0;

// Two buffers are filled by this function, one gets filled while the other is drawn (via GDMA to the LCD.) 
// Each call fills a certain number of lines, set by BOUNCE_BUFFER_SIZE_PX in setup (it's currently 12 lines / 1 row of text)

 bool display_bounce_empty(void *bounce_buf, int pos_px, int len_bytes, void *user_ctx) {
    int64_t tic=get_time_us(); // start the timer
    // Which pixel row and TFB row is this

    uint16_t starting_display_row_px = pos_px / H_RES;
    uint8_t bounce_total_rows_px = len_bytes / H_RES / BYTES_PER_PIXEL;
    // compute the starting TFB row offset 
    uint8_t * b = (uint8_t*)bounce_buf;
    int16_t touch_x = last_touch_x[0];
    int16_t touch_y = last_touch_y[0];
    uint8_t touch_held_local = touch_held;
    uint16_t start_col_px = 0;

    // We want to (first vertically) center our visible window.
    // compute starting row given V_RES_D..
    // TODO -- this cuts off the top by half a line with ipad pro resolution
    if(V_RES != V_RES_D) {
        if(starting_display_row_px < ((V_RES-V_RES_D)/2)) {
            // not yet in the window
            memset(bounce_buf, 0, len_bytes); 
            goto bounce_end;
        } else if(starting_display_row_px >= ((V_RES-V_RES_D)/2)+V_RES_D) {
            // past the window
            memset(bounce_buf, 0, len_bytes); 
            goto bounce_end;            
        } else {
            // it's me. adjust starting row px
            starting_display_row_px = starting_display_row_px - ((V_RES-V_RES_D)/2);
            // Also update touch_y
            touch_y = touch_y - ((V_RES-V_RES_D)/2);
        }
    }

    // And likewise, center horizontally 
    if(H_RES != H_RES_D) {
        start_col_px = ((H_RES-H_RES_D)/2);
    }


    // Copy in the BG, line by line 
    // 208uS per call at 6 lines RGB565
    // 209uS per call at 12 lines RGB332
    // 416uS per call at 12 lines RGB565
    for(uint8_t rows_relative_px=0;rows_relative_px<bounce_total_rows_px;rows_relative_px++) {
        memcpy(b+start_col_px+(H_RES*BYTES_PER_PIXEL*rows_relative_px), bg_lines[(starting_display_row_px+rows_relative_px) % V_RES], H_RES_D*BYTES_PER_PIXEL); 
    }

    // Now per row (N (now 12) pixel rows per call), draw the text frame buffer and sprites on top of the BG
    for(uint8_t bounce_row_px=0;bounce_row_px<bounce_total_rows_px;bounce_row_px++) {
        memset(sprite_ids, 255, H_RES);
        if(tfb_active) {
            uint8_t tfb_row = (starting_display_row_px+bounce_row_px) / FONT_HEIGHT;
            uint8_t tfb_row_offset_px = (starting_display_row_px+bounce_row_px) % FONT_HEIGHT; 

            uint8_t tfb_col = 0;
            while(TFB[tfb_row*TFB_COLS+tfb_col]!=0 && tfb_col < TFB_COLS) {

                #ifndef TULIP_REPL_FONT_8X6
                    uint8_t data = font_8x12_r[TFB[tfb_row*TFB_COLS+tfb_col]][tfb_row_offset_px];
                #else
                    uint8_t data = portfolio_glyph_bitmap[(TFB[tfb_row*TFB_COLS+tfb_col] -32) * 8 + tfb_row_offset_px];
                #endif
                uint8_t format = TFBf[tfb_row*TFB_COLS+tfb_col];
                uint8_t fg_color = TFBfg[tfb_row*TFB_COLS+tfb_col];
                uint8_t bg_color = TFBbg[tfb_row*TFB_COLS+tfb_col];

                // If you're looking at this code just know the unrolled versions were 1.5x faster than loops on esp32s3
                // I'm sure there's more to do but this is the best we could get it for now
                uint8_t * bptr = b + start_col_px + (bounce_row_px*H_RES + tfb_col*FONT_WIDTH);
                if(bg_color == ALPHA) {
                    if(format & FORMAT_INVERSE) {
                        if(!((data) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        bptr++; if(!((data <<= 1) & 0x80)) *(bptr) = fg_color; 
                        #ifndef TULIP_REPL_FONT_8X6
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
                        #ifndef TULIP_REPL_FONT_8X6
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
                        #ifndef TULIP_REPL_FONT_8X6
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
                        #ifndef TULIP_REPL_FONT_8X6
                            bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                            bptr++; if((data <<=1) & 0x80) *(bptr) = fg_color; else *(bptr) = bg_color;
                        #endif
                    }

                }
                tfb_col++;
            }
        }
        // Add in the sprites
        uint16_t row_px = starting_display_row_px + bounce_row_px; 
        // Add touch in as a fake colliison, if it exists
        if(touch_held_local && touch_y == row_px) {
            if(touch_x >= 0 && touch_x < H_RES) {
                sprite_ids[touch_x] = SPRITES-1;
            }
        }

        

        for(uint8_t s=0;s<SPRITES;s++) {
            if(sprite_vis[s]==SPRITE_IS_SPRITE) {
                if(row_px >= sprite_y_px[s] && row_px < sprite_y_px[s]+sprite_h_px[s]) {
                    // this sprite is on this line 
                    // compute x and y (relative to the sprite!)
                    uint8_t * sprite_data = &sprite_ram[sprite_mem[s]];
                    uint16_t relative_sprite_y_px = row_px - sprite_y_px[s];
                    for(uint16_t col_px=sprite_x_px[s]; col_px < sprite_x_px[s] + sprite_w_px[s]; col_px++) {
                        if(col_px < H_RES_D) {
                            uint16_t relative_sprite_x_px = col_px - sprite_x_px[s];
                            uint8_t b0 = sprite_data[relative_sprite_y_px * sprite_w_px[s] + relative_sprite_x_px  ] ;
                            if(b0 != ALPHA) {
                                b[bounce_row_px*H_RES + col_px + start_col_px] = b0;
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
            } else if(sprite_vis[s] == SPRITE_IS_WIREFRAME) {
                // draw this as a bresenham list
                // find where to start
                uint16_t * line_data = (uint16_t*) (&sprite_ram[sprite_mem[s]]);
                uint16_t line_c = 0;
                uint16_t y0 = line_data[line_c * 4 + 1];
                uint16_t y1 = line_data[line_c * 4 + 3];
                // todo, we could get smarter and sort lines by length secondarily after y0 to save time here
                while(y0 != 65535) {
                    if(row_px >= y0 && row_px <= y1) {
                        // get a int representation of how far down we are 
                        uint16_t x_midpoint = 0;
                        uint16_t x_line_width = 0;
                        uint16_t x0 = line_data[line_c * 4 + 0];
                        uint16_t x1 = line_data[line_c * 4 + 2];

                        uint8_t color = ((x0 & 0xF000) >> 8) | ((x1 & 0xF000) >> 12);
                        x0 = x0 & 0x0FFF;
                        x1 = x1 & 0x0FFF;
                        if(x1 > x0) {
                            x_line_width =  ((H_RES) / (((y1-y0)*H_RES)/(x1-x0)));
                            x_midpoint = x0 + (((row_px-y0) * (x1-x0)) / (y1-y0));
                            if(x_line_width < 2) {
                                b[bounce_row_px*H_RES+x_midpoint] = color;
                            } else {
                                for(uint16_t i=x_midpoint-(x_line_width/2);i<x_midpoint+(x_line_width/2);i++) {
                                    if(i <= x1 && i >= x0) { 
                                        b[bounce_row_px*H_RES + i] = color;
                                    }
                                }
                            }
                        } else if (x1<x0) {
                            x_line_width =  ((H_RES) / (((y1-y0)*H_RES)/(x0-x1)));
                            x_midpoint = x0 - (((row_px-y0) * (x0-x1)) / (y1-y0));
                            if(x_line_width < 2) {
                                b[bounce_row_px*H_RES+x_midpoint] = color;
                            } else {
                                for(uint16_t i=x_midpoint-(x_line_width/2);i<x_midpoint+(x_line_width/2);i++) {
                                    if(i <= x0 && i >= x1) { 
                                        b[bounce_row_px*H_RES + i] = color;
                                    }
                                }
                            }
                        } else {
                            // A straight line up and down
                            b[bounce_row_px*H_RES+x1] = color;
                        }
                    }
                    line_c++;
                    y0 = line_data[line_c * 4 + 1];
                    y1 = line_data[line_c * 4 + 3];
                }

            }

        } // for each sprite
    } // per each row
    bounce_end:
        bounce_time += (get_time_us() - tic); // stop timer
        bounce_count++;

        return false; 
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


// Palletized version of screenshot. about 3x as fast, RGB332 only
void display_screenshot(char * screenshot_fn) {
    // Blank the display
    //display_stop();

    // 778ms total without blanking, pretty good
    uint8_t * screenshot_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    uint8_t * full_pic = (uint8_t*) malloc_caps(H_RES*V_RES*1,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
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
    (void)err;
    state.info_png.color.colortype = LCT_PALETTE; 
    state.info_png.color.bitdepth = 8;
    state.info_raw.colortype = LCT_PALETTE;
    state.info_raw.bitdepth = 8;
    state.encoder.auto_convert = 0;
    //int64_t tic = get_time_us();
    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        display_bounce_empty(screenshot_bb, y*H_RES, H_RES*FONT_HEIGHT*BYTES_PER_PIXEL, NULL);
        for(uint16_t x=0;x<FONT_HEIGHT*H_RES*BYTES_PER_PIXEL;x=x+BYTES_PER_PIXEL) {
            full_pic[c++] = screenshot_bb[x];
        }
    }
    // 45ms
    //fprintf(stderr,"Took %lld uS to bounce entire screen\n", get_time_us() - tic);
    //tic = get_time_us();
    uint32_t outsize = 0;
    uint8_t *out;
    err = lodepng_encode(&out, (size_t*)&outsize,full_pic, H_RES, V_RES, &state);
    // 456ms , 4223 b frame
    //fprintf(stderr,"Took %lld uS to encode as PNG to memory. err %d\n", get_time_us() - tic, err);
    //tic = get_time_us();
    //fprintf(stderr,"PNG done encoding. writing %" PRIu32" bytes to file %s\n", outsize, screenshot_fn);
    write_file(screenshot_fn, out, outsize, 1);
    // 268ms 
    //fprintf(stderr,"Took %lld uS to write to disk\n", get_time_us() - tic);
    free_caps(out);
    free_caps(screenshot_bb);
    free_caps(full_pic);
    // Restart the display
    //display_start();
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
        unpack_rgb_332(px0, r, g, b);
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
            if(d==5) { fprintf(stderr,"Warning, more than 5 ANSI format commands in a row\n"); d = 4; }
            last_pos = i+1;
        }
    }
    return d;
}

uint8_t supress_lf = 0;
void display_tfb_str(char*str, uint16_t len, uint8_t format, uint8_t fg_color, uint8_t bg_color) {
    if(tfb_log) {
        fprintf(stderr, "%.*s", len, str);
        fflush(stderr);
    }

    //fprintf(stderr,"str len %d format %d is ### ", len, format);
    //for(uint16_t i=0;i<len;i++) fprintf(stderr, "[%c] [%d] ", str[i], str[i]);
    //fprintf(stderr, "###\n");
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
    fprintf(stderr, "freeing bg\n");
    free_caps(bg); bg = NULL;
    free_caps(sprite_ids);
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


void display_init(void) {
    TFB_ROWS = (V_RES_D/FONT_HEIGHT);
    TFB_COLS = (H_RES_D/FONT_WIDTH);
    // 12 divides into 600, 480, 240
    BOUNCE_BUFFER_SIZE_PX = (H_RES*12) ;
    // Create the background FB
    bg = (uint8_t*)calloc_caps(32, 1, (H_RES+OFFSCREEN_X_PX)*(V_RES+OFFSCREEN_Y_PX)*BYTES_PER_PIXEL, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);


    // And various ptrs
    sprite_ids = (uint8_t*)malloc_caps(H_RES_D *  sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_ram = (uint8_t*)malloc_caps(SPRITE_RAM_BYTES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_x_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_y_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_w_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_h_px = (uint16_t*)malloc_caps(SPRITES*sizeof(uint16_t), MALLOC_CAP_INTERNAL);
    sprite_vis = (uint8_t*)malloc_caps(SPRITES*sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    sprite_mem = (uint32_t*)malloc_caps(SPRITES*sizeof(uint32_t), MALLOC_CAP_INTERNAL);
    collision_bitfield = (uint8_t*)malloc_caps(128, MALLOC_CAP_INTERNAL);

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
    ui_init();

    vsync_count = 1;
    reported_fps = 30;
    reported_gpu_usage = 0;
    touch_held = 0;


}

