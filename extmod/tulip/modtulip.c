

#include <stdio.h>
#include "polyfills.h"

#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "display.h"
#include "extmod/vfs.h"
#include "py/stream.h"
#include "alles_tulip.h"

// Graphics

// tulip.display_clock(18)
// clock = tulip.display_clock()
STATIC mp_obj_t tulip_display_clock(size_t n_args, const mp_obj_t *args) {
    if(n_args==1) {
        uint16_t mhz = mp_obj_get_int(args[0]);
        display_set_clock(mhz);
        return mp_const_none;
    }
    return mp_obj_new_int(display_mhz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_clock_obj, 0, 1, tulip_display_clock);

STATIC mp_obj_t tulip_display_restart(size_t n_args, const mp_obj_t *args) {
    display_set_clock(display_mhz);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_restart_obj, 0, 0, tulip_display_restart);


// fps = tulip.fps()
STATIC mp_obj_t tulip_fps(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_float_from_f(reported_fps);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_fps_obj, 0, 0, tulip_fps);

// usage = tulip.gpu()
STATIC mp_obj_t tulip_gpu(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_float_from_f(reported_gpu_usage);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_gpu_obj, 0, 0, tulip_gpu);


// tulip.bg_pixel(x,y, r,g,b)
// tulip.bg_pixel(x,y, pal_idx)
// (r,g,b) = tulip.bg_pixel(x,y)
STATIC mp_obj_t tulip_bg_pixel(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint8_t r,g,b;
    if(n_args == 5) { // r,g,b
        r = mp_obj_get_int(args[2]);
        g = mp_obj_get_int(args[3]);
        b = mp_obj_get_int(args[4]);
        // Set the pixel
        display_set_bg_pixel(x,y,r,g,b);
        return mp_const_none; 
    } else  if(n_args == 3) { // pallete index
        unpack_pal_idx(mp_obj_get_int(args[2]), &r, &g, &b);
        // Set the pixel
        display_set_bg_pixel(x,y,r,g,b);
        return mp_const_none; 
    } else {
        // return a tuple of (r,g,b) for x,y
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        mp_obj_t tuple[3];
        display_get_bg_pixel(x,y, &r,&g,&b); 
        tuple[0] = mp_obj_new_int(r); 
        tuple[1] = mp_obj_new_int(g);
        tuple[2] = mp_obj_new_int(b);
        return mp_obj_new_tuple(3, tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_pixel_obj, 2, 5, tulip_bg_pixel);

// tulip.bg_rect(x,y,w,h, r,g,b)
// tulip.bg_rect(x,y,w,h, pal_idx)
STATIC mp_obj_t tulip_bg_rect(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    uint8_t r,g,b;        
    if(n_args == 5) {
        unpack_pal_idx(mp_obj_get_int(args[4]), &r, &g, &b);
    }
    if(n_args == 7) {
        r = mp_obj_get_int(args[4]);
        g = mp_obj_get_int(args[5]);
        b = mp_obj_get_int(args[6]);
    }
    // Set the rect with pixel color
    for(uint16_t y0=y;y0<y+h;y0++) {
        for(uint16_t x0=x;x0<x+w;x0++) {
            display_set_bg_pixel(x0, y0, r, g, b);
        }
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_rect_obj, 5, 7, tulip_bg_rect);

// tulip.bg_clear(r,g,b)
// tulip.bg_clear(pal_idx)
// tulip.bg_clear() # uses default
STATIC mp_obj_t tulip_bg_clear(size_t n_args, const mp_obj_t *args) {
    uint8_t r, g, b;
    if(n_args == 1) {
        unpack_pal_idx(mp_obj_get_int(args[0]), &r, &g, &b);
    } else if(n_args == 3) {
        r = mp_obj_get_int(args[0]);
        g = mp_obj_get_int(args[1]);
        b = mp_obj_get_int(args[2]);
    } else {
        // Default to the default bg
        unpack_ansi_idx(bg_pal_color, &r, &g, &b);
    }
    // Set the rect with pixel color
    for(uint16_t y0=0;y0<V_RES+OFFSCREEN_Y_PX;y0++) {
        for(uint16_t x0=0;x0<H_RES+OFFSCREEN_X_PX;x0++) {
            display_set_bg_pixel(x0, y0, r, g, b);
        }
    }
    return mp_const_none; 
}


STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_clear_obj, 0, 3, tulip_bg_clear);

// tulip.bg_bitmap(x, y, w, h, bitmap)  --> sets or gets bitmap to fb ram
STATIC mp_obj_t tulip_bg_bitmap(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    if(n_args == 5) {
        // Set the rect with bitmap pixels
        mp_buffer_info_t bufinfo;
        if (mp_obj_get_type(args[4]) == &mp_type_bytes) {
            mp_get_buffer(args[4], &bufinfo, MP_BUFFER_READ);
            display_set_bg_bitmap_raw(x, y, w, h, (uint8_t*)bufinfo.buf);
        }
        return mp_const_none; 
    } else {
        // return a bitmap
        uint8_t bitmap[w*h*BYTES_PER_PIXEL];  
        display_get_bg_bitmap_raw(x,y,w,h, bitmap);
        return mp_obj_new_bytes(bitmap, w*h*BYTES_PER_PIXEL);
    }
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_bitmap_obj, 4, 5, tulip_bg_bitmap);

// tulip.bg_blit(x, y, w, h, x1, y1)  --> copies bitmap ram
STATIC mp_obj_t tulip_bg_blit(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    uint16_t x1 = mp_obj_get_int(args[4]);
    uint16_t y1 = mp_obj_get_int(args[5]);
    display_bg_bitmap_blit(x,y,w,h,x1,y1);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_blit_obj, 6, 6, tulip_bg_blit);



// tulip.bg_png(bytes, x,y)
// tulip.bg_png(filename, x,y)
STATIC mp_obj_t tulip_bg_png(size_t n_args, const mp_obj_t *args) {
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);

    mp_buffer_info_t bufinfo;
    uint8_t file = 0;
    if (mp_obj_get_type(args[0]) == &mp_type_bytes) {
        mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
    } else {
        uint32_t fs = file_size(mp_obj_str_get_str(args[0]));
        bufinfo.buf = malloc_caps(fs, MALLOC_CAP_SPIRAM);
        bufinfo.len = fs;
        read_file(mp_obj_str_get_str(args[0]), (uint8_t *)bufinfo.buf, -1, 1);
        file = 1;
    }
    error = lodepng_decode_memory(&image, &width, &height, (uint8_t*)bufinfo.buf, bufinfo.len, LCT_RGBA, 8);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    display_set_bg_bitmap_rgba(x,y,width,height,image);
    free_caps(image);
    if(file) {
        free_caps(bufinfo.buf);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_png_obj, 3, 3, tulip_bg_png);

//tulip.bg_scroll(line, x_offset, y_offset, x_speed, y_speed)
//tulip.bg_scroll() # resets
STATIC mp_obj_t tulip_bg_scroll(size_t n_args, const mp_obj_t *args) {
    if(n_args<5) {
        // Reset
        for(uint16_t line_no=0;line_no<V_RES;line_no++) {
            x_offsets[line_no] = 0;
            y_offsets[line_no] = line_no;
            x_speeds[line_no] = 0;
            y_speeds[line_no] = 0;
        }
    } else {
        uint16_t line_no = mp_obj_get_int(args[0]);
        uint16_t x_offset = mp_obj_get_int(args[1]);
        uint16_t y_offset = mp_obj_get_int(args[2]);
        int16_t x_speed = mp_obj_get_int(args[3]);
        int16_t y_speed = mp_obj_get_int(args[4]);
        x_offsets[line_no] = x_offset;
        y_offsets[line_no] = y_offset;
        x_speeds[line_no] = x_speed;
        y_speeds[line_no] = y_speed;
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_scroll_obj, 0, 5, tulip_bg_scroll);

STATIC mp_obj_t tulip_bg_scroll_x_speed(size_t n_args, const mp_obj_t *args) {
    uint16_t line_no = mp_obj_get_int(args[0]);
    int16_t x_speed = mp_obj_get_int(args[1]);
    x_speeds[line_no] = x_speed;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_scroll_x_speed_obj, 2, 2, tulip_bg_scroll_x_speed);


STATIC mp_obj_t tulip_bg_scroll_y_speed(size_t n_args, const mp_obj_t *args) {
    uint16_t line_no = mp_obj_get_int(args[0]);
    int16_t y_speed = mp_obj_get_int(args[1]);
    y_speeds[line_no] = y_speed;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_scroll_y_speed_obj, 2, 2, tulip_bg_scroll_y_speed);


STATIC mp_obj_t tulip_bg_scroll_x_offset(size_t n_args, const mp_obj_t *args) {
    uint16_t line_no = mp_obj_get_int(args[0]);
    int16_t x_offset = mp_obj_get_int(args[1]);
    x_offsets[line_no] = x_offset;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_scroll_x_offset_obj, 2, 2, tulip_bg_scroll_x_offset);


STATIC mp_obj_t tulip_bg_scroll_y_offset(size_t n_args, const mp_obj_t *args) {
    uint16_t line_no = mp_obj_get_int(args[0]);
    int16_t y_offset = mp_obj_get_int(args[1]);
    y_offsets[line_no] = y_offset;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_scroll_y_offset_obj, 2, 2, tulip_bg_scroll_y_offset);



// tulip.tfb_str(x,y, str, [format], [fg_color], [bg_color])
// (str, format, fg, bg) = tulip.tfb_str(x,y)
STATIC mp_obj_t tulip_tfb_str(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint8_t set = 0;
    if(n_args > 2) set = 1;
    if(set) {
        const char * str = mp_obj_str_get_str(args[2]);
        for(uint16_t i=0;i<strlen(str);i++) {
            TFB[y*TFB_COLS+x+i] = str[i];
        }
        if(n_args > 3) {
            if(mp_obj_get_int(args[3])>=0) {
                TFBf[y*TFB_COLS+x] = mp_obj_get_int(args[3]);
            }
        }
        if(n_args > 4 ) {
            if(mp_obj_get_int(args[4])>=0) {
                TFBfg[y*TFB_COLS+x] = mp_obj_get_int(args[4]);
            }
        }
        if(n_args > 5 ) {
            if(mp_obj_get_int(args[5])>=0) {
                TFBbg[y*TFB_COLS+x] = mp_obj_get_int(args[5]);
            }
        }
        return mp_const_none; 
    } else {
        mp_obj_t tuple[4];
        tuple[0] = mp_obj_new_str((const char*)(TFB + (y*TFB_COLS+x)), 1);
        tuple[1] = mp_obj_new_int(TFBf[y*TFB_COLS+x]);
        tuple[2] = mp_obj_new_int(TFBfg[y*TFB_COLS+x]);
        tuple[3] = mp_obj_new_int(TFBbg[y*TFB_COLS+x]);
        return mp_obj_new_tuple(4,tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_str_obj, 2, 6, tulip_tfb_str);



mp_obj_t frame_callback = NULL; 
mp_obj_t frame_arg = NULL; 
void tulip_frame_isr() {
    // Schedule the python callback given to run asap
    mp_sched_schedule(frame_callback, frame_arg);
#ifdef ESP_PLATFORM
    mp_hal_wake_main_task_from_isr();
#endif
}

// tulip.frame_callback(cb, arg)
// tulip.frame_callback() -- stops 
STATIC mp_obj_t tulip_frame_callback(size_t n_args, const mp_obj_t *args) {
    if(n_args == 0) {
        py_callback = 0;
    } else {
        frame_callback = args[0];
        py_callback = 1;
    }
    if(n_args > 1) {
        frame_arg = args[1];
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_frame_callback_obj, 0, 2, tulip_frame_callback);


//(w,h,bytes) = sprite_png(pngdata, mem_pos) 
//(w,h,bytes) = sprite_png("filename.png", mem_pos)
STATIC mp_obj_t tulip_sprite_png(size_t n_args, const mp_obj_t *args) {
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    uint16_t mem_pos = mp_obj_get_int(args[1]);
    mp_buffer_info_t bufinfo;
    uint8_t file = 0;
    if (mp_obj_get_type(args[0]) == &mp_type_bytes) {
        mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
    } else {
        uint32_t fs = file_size(mp_obj_str_get_str(args[0]));
        bufinfo.buf = malloc_caps(fs, MALLOC_CAP_SPIRAM);
        bufinfo.len = fs;
        read_file(mp_obj_str_get_str(args[0]), (uint8_t *)bufinfo.buf, -1, 1);
        file = 1;
    }
    error = lodepng_decode_memory(&image, &width, &height, (uint8_t*)bufinfo.buf, bufinfo.len, LCT_RGBA, 8);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    display_load_sprite_rgba(mem_pos, width*height*BYTES_PER_PIXEL, image);
    free_caps(image);
    if(file) free_caps(bufinfo.buf);
    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_int(width);
    tuple[1] = mp_obj_new_int(height);
    tuple[2] = mp_obj_new_int(width*height*BYTES_PER_PIXEL);
    return mp_obj_new_tuple(3, tuple);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_png_obj, 2, 2, tulip_sprite_png);


//bytes = sprite_bitmap(bitmap, mem_pos) 
//buffer_of_bytes = sprite_bitmap(mem_pos, length)
STATIC mp_obj_t tulip_sprite_bitmap(size_t n_args, const mp_obj_t *args) {
    if (mp_obj_get_type(args[0]) == &mp_type_bytes) {
        uint16_t mem_pos = mp_obj_get_int(args[1]);
        mp_buffer_info_t bufinfo;
        mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
        display_load_sprite_raw(mem_pos, bufinfo.len, bufinfo.buf);
        return mp_obj_new_int(bufinfo.len);
    } 
    uint16_t mem_pos = mp_obj_get_int(args[0]);
    uint16_t length = mp_obj_get_int(args[1]);
    return mp_obj_new_bytes(sprite_ram+mem_pos, length);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_bitmap_obj, 2, 2, tulip_sprite_bitmap);

//sprite_register(34, mem_pos, w,h) # 34 = sprite number, can be up to ...
STATIC mp_obj_t tulip_sprite_register(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    uint32_t mem_pos = mp_obj_get_int(args[1]);
    uint16_t width = mp_obj_get_int(args[2]);
    uint16_t height = mp_obj_get_int(args[3]);
    sprite_mem[spriteno] = mem_pos;
    sprite_w_px[spriteno] = width;
    sprite_h_px[spriteno] = height;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_register_obj, 4, 4, tulip_sprite_register);


//sprite_move(34, x,y)
STATIC mp_obj_t tulip_sprite_move(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);
    sprite_x_px[spriteno] = x;
    sprite_y_px[spriteno] = y;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_move_obj, 3, 3, tulip_sprite_move);

STATIC mp_obj_t tulip_sprite_on(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    sprite_vis[spriteno] = SPRITE_IS_SPRITE;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_on_obj, 1, 1, tulip_sprite_on);


STATIC mp_obj_t tulip_sprite_off(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    sprite_vis[spriteno] = 0;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_off_obj, 1, 1, tulip_sprite_off);


STATIC mp_obj_t tulip_sprite_clear(size_t n_args, const mp_obj_t *args) {
    display_reset_sprites();
    py_callback = 0;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_clear_obj, 0, 0, tulip_sprite_clear);


extern void editor(const char * filename);
STATIC mp_obj_t tulip_edit(size_t n_args, const mp_obj_t *args) {
    if(n_args) {
        editor(mp_obj_str_get_str(args[0]));
    } else {
        editor(NULL);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_edit_obj, 0, 1, tulip_edit);


STATIC mp_obj_t tulip_gpu_reset(size_t n_args, const mp_obj_t *args) {
    display_reset_bg();
    display_reset_sprites();
    py_callback = 0;
    display_reset_tfb();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_gpu_reset_obj, 0, 0, tulip_gpu_reset);



STATIC mp_obj_t tulip_int_screenshot(size_t n_args, const mp_obj_t *args) {
    char fn[50];
    strcpy(fn, mp_obj_str_get_str(args[0]));
    display_screenshot(fn);
    return mp_const_none;

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_int_screenshot_obj, 1, 1, tulip_int_screenshot);

STATIC mp_obj_t tulip_alles(size_t n_args, const mp_obj_t *args) {
    alles_send_message((char*)mp_obj_str_get_str(args[0]), strlen(mp_obj_str_get_str(args[0])));
    return mp_const_none;

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_alles_obj, 1, 1, tulip_alles);

STATIC mp_obj_t tulip_brightness(size_t n_args, const mp_obj_t *args) {
    if(n_args > 0) {
        display_brightness(mp_obj_get_int(args[0]));
        return mp_const_none;
    } else {
        return mp_obj_new_int(brightness);
    }

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_brightness_obj, 0, 1, tulip_brightness);

extern uint8_t last_scan[8];
STATIC mp_obj_t tulip_keys(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[7];
    tuple[0] = mp_obj_new_int(last_scan[0]);
    for(uint8_t i=0;i<6;i++) tuple[i+1] = mp_obj_new_int(last_scan[i+2]);
    return mp_obj_new_tuple(7, tuple);

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_keys_obj, 0, 0, tulip_keys);

STATIC mp_obj_t tulip_key_wait(size_t n_args, const mp_obj_t *args) {
    int16_t ch = mp_hal_stdin_rx_chr();
    return mp_obj_new_int(ch);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_wait_obj, 0, 0, tulip_key_wait);


STATIC mp_obj_t tulip_key(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_int(check_rx_char());
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_obj, 0, 0, tulip_key);

extern uint8_t keyboard_send_keys_to_micropython;
STATIC mp_obj_t tulip_key_scan(size_t n_args, const mp_obj_t *args) {
    keyboard_send_keys_to_micropython = !(mp_obj_get_int(args[0]));
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_scan_obj, 1, 1, tulip_key_scan);


extern float compute_cpu_usage(uint8_t debug);
STATIC mp_obj_t tulip_cpu(size_t n_args, const mp_obj_t *args) {
    // for now just printf to uart
    float idle;
    if(n_args > 0) {
        idle = compute_cpu_usage(1);        
    } else {
        idle = compute_cpu_usage(0);
    }
    return mp_obj_new_float_from_f(idle);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_cpu_obj, 0, 1, tulip_cpu);


STATIC const mp_rom_map_elem_t tulip_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR__tulip) },
    { MP_ROM_QSTR(MP_QSTR_display_clock), MP_ROM_PTR(&tulip_display_clock_obj) },
    { MP_ROM_QSTR(MP_QSTR_display_restart), MP_ROM_PTR(&tulip_display_restart_obj) },
    { MP_ROM_QSTR(MP_QSTR_fps), MP_ROM_PTR(&tulip_fps_obj) },
    { MP_ROM_QSTR(MP_QSTR_gpu), MP_ROM_PTR(&tulip_gpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_pixel), MP_ROM_PTR(&tulip_bg_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_png), MP_ROM_PTR(&tulip_bg_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_clear), MP_ROM_PTR(&tulip_bg_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll), MP_ROM_PTR(&tulip_bg_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll_x_speed), MP_ROM_PTR(&tulip_bg_scroll_x_speed_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll_y_speed), MP_ROM_PTR(&tulip_bg_scroll_y_speed_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll_x_offset), MP_ROM_PTR(&tulip_bg_scroll_x_offset_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll_y_offset), MP_ROM_PTR(&tulip_bg_scroll_y_offset_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_str), MP_ROM_PTR(&tulip_tfb_str_obj) },
    { MP_ROM_QSTR(MP_QSTR_frame_callback), MP_ROM_PTR(&tulip_frame_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_bitmap), MP_ROM_PTR(&tulip_bg_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_blit), MP_ROM_PTR(&tulip_bg_blit_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_rect), MP_ROM_PTR(&tulip_bg_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_png), MP_ROM_PTR(&tulip_sprite_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_bitmap), MP_ROM_PTR(&tulip_sprite_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_register), MP_ROM_PTR(&tulip_sprite_register_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_move), MP_ROM_PTR(&tulip_sprite_move_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_on), MP_ROM_PTR(&tulip_sprite_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_off), MP_ROM_PTR(&tulip_sprite_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_clear), MP_ROM_PTR(&tulip_sprite_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_edit), MP_ROM_PTR(&tulip_edit_obj) },
    { MP_ROM_QSTR(MP_QSTR_int_screenshot), MP_ROM_PTR(&tulip_int_screenshot_obj) },
    { MP_ROM_QSTR(MP_QSTR_alles), MP_ROM_PTR(&tulip_alles_obj) },
    { MP_ROM_QSTR(MP_QSTR_brightness), MP_ROM_PTR(&tulip_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_keys), MP_ROM_PTR(&tulip_keys_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_wait), MP_ROM_PTR(&tulip_key_wait_obj) },
    { MP_ROM_QSTR(MP_QSTR_key), MP_ROM_PTR(&tulip_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_scan), MP_ROM_PTR(&tulip_key_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_cpu), MP_ROM_PTR(&tulip_cpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_gpu_reset), MP_ROM_PTR(&tulip_gpu_reset_obj) },

};

STATIC MP_DEFINE_CONST_DICT(tulip_module_globals, tulip_module_globals_table);

const mp_obj_module_t tulip_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&tulip_module_globals,
};


MP_REGISTER_MODULE(MP_QSTR__tulip, tulip_module, 1); // MICROPY_PY_TULIP);
