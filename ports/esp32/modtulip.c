

#include <stdio.h>

#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "lodepng.h"
#include "extmod/vfs.h"
#include "display.h"
#include "py/stream.h"


// Graphics

STATIC mp_obj_t tulip_display_clock(size_t n_args, const mp_obj_t *args) {
    if(n_args==1) {
        uint16_t mhz = mp_obj_get_int(args[0]);
        display_set_clock(mhz);
        return mp_const_none;
    }
    uint8_t mhz = display_get_clock();
    return mp_obj_new_int(mhz);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_clock_obj, 0, 1, tulip_display_clock);


STATIC mp_obj_t tulip_bg_pixel(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);

    if(n_args == 5) {
        uint8_t r = mp_obj_get_int(args[2]);
        uint8_t g = mp_obj_get_int(args[3]);
        uint8_t b = mp_obj_get_int(args[4]);

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
        tuple[1] = mp_obj_new_int(b);
        return mp_obj_new_tuple(3, tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_pixel_obj, 2, 5, tulip_bg_pixel);


STATIC mp_obj_t tulip_bg_rect(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    if(n_args == 7) {
        uint8_t r = mp_obj_get_int(args[4]);
        uint8_t g = mp_obj_get_int(args[5]);
        uint8_t b = mp_obj_get_int(args[6]);
        // Set the rect with pixel color
        for(uint16_t y0=y;y0<y+h;y0++) {
            for(uint16_t x0=x;x0<x+w;x0++) {
                display_set_bg_pixel(x0, y0, r, g, b);
            }
        }
        return mp_const_none; 
    } else {
        // return a list of list of tuples, like .. ret[x][y] == (12,2,100)... will i ever need this? punt for now
        return mp_const_none; 
    }
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_rect_obj, 4, 7, tulip_bg_rect);

STATIC mp_obj_t tulip_bg_clear(size_t n_args, const mp_obj_t *args) {
    // Default to the default bg
    uint8_t r, g, b;
    unpack_rgb(ansi_pal[bg_pal_color]*2+0, ansi_pal[bg_pal_color]*2+1, &r, &g, &b);
    if(n_args == 3) {
        r = mp_obj_get_int(args[0]);
        g = mp_obj_get_int(args[1]);
        b = mp_obj_get_int(args[2]);
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

//bg_bitmap(x, y, w, h, bitmap)  --> sets or gets bitmap to fb ram
STATIC mp_obj_t tulip_bg_bitmap(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    if(n_args == 5) {
        // Set the rect with bitmap pixels
        //size_t len = w*h*2;
        //const char * bitmap = mp_obj_str_get_data(&args[4], len);
        //draw_bitmap(x,y,x+w,y+h,(uint8_t*)bitmap);
        return mp_const_none; 
    } else {
        // return a bitmap
        uint8_t bitmap[w*h*2];  
        display_get_bitmap(x,y,w,h, bitmap);
        return mp_obj_new_bytes(bitmap, w*h*2);
    }
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_bitmap_obj, 4, 5, tulip_bg_bitmap);



// bytes, x, y
STATIC mp_obj_t tulip_bg_png(size_t n_args, const mp_obj_t *args) {
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);

    mp_buffer_info_t bufinfo;
    mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
    error = lodepng_decode_memory(&image, &width, &height, (uint8_t*)bufinfo.buf, bufinfo.len, LCT_RGBA, 8);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    display_bg_bitmap(x,y,x+width,y+height,image);
    heap_caps_free(image);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_png_obj, 3, 3, tulip_bg_png);


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



STATIC mp_obj_t tulip_tfb_char(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    if(n_args > 2) {
        TFB[y*TFB_COLS+x] = mp_obj_get_int(args[2]);
        if(n_args == 4) TFBf[y*TFB_COLS+x] = mp_obj_get_int(args[3]);
        return mp_const_none; 
    } else {
        mp_obj_t tuple[2];
        tuple[0] = mp_obj_new_int(TFB[y*TFB_COLS+x]);
        tuple[1] = mp_obj_new_int(TFBf[y*TFB_COLS+x]);
        return mp_obj_new_tuple(2,tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_char_obj, 2, 4, tulip_tfb_char);



mp_obj_t frame_callback = NULL; 
mp_obj_t frame_arg = NULL; 
void tulip_frame_isr() {
    // Schedule the python callback given to run asap
    mp_sched_schedule(frame_callback, frame_arg);
    mp_hal_wake_main_task_from_isr();
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


//void load_sprite(uint32_t mem_pos, uint16_t w, uint16_t h, uint8_t* data) ;
//mem_len = sprite_png(pngdata, mem_pos) 
STATIC mp_obj_t tulip_sprite_png(size_t n_args, const mp_obj_t *args) {
    unsigned error;
    unsigned char* image;
    unsigned width, height;
    uint16_t mem_pos = mp_obj_get_int(args[1]);

    mp_buffer_info_t bufinfo;
    mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
    error = lodepng_decode_memory(&image, &width, &height, (uint8_t*)bufinfo.buf, bufinfo.len, LCT_RGBA, 8);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    display_load_sprite(mem_pos, width*height*2, image);
    heap_caps_free(image);
    return mp_obj_new_int(width*height*2);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_png_obj, 2, 2, tulip_sprite_png);

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
    sprite_vis[spriteno] = 1;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_on_obj, 1, 1, tulip_sprite_on);


STATIC mp_obj_t tulip_sprite_off(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    sprite_vis[spriteno] = 0;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_off_obj, 1, 1, tulip_sprite_off);


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


STATIC mp_obj_t tulip_screenshot(size_t n_args, const mp_obj_t *args) {
    char fn[50];
    strcpy(fn, mp_obj_str_get_str(args[0]));
    // Wait a frame or two 
    vTaskDelay(pdMS_TO_TICKS(100));
    display_screenshot(fn);
    return mp_const_none;

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_screenshot_obj, 1, 1, tulip_screenshot);


STATIC const mp_rom_map_elem_t tulip_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_tulip) },
    { MP_ROM_QSTR(MP_QSTR_display_clock), MP_ROM_PTR(&tulip_display_clock_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_pixel), MP_ROM_PTR(&tulip_bg_pixel_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_png), MP_ROM_PTR(&tulip_bg_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_clear), MP_ROM_PTR(&tulip_bg_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_scroll), MP_ROM_PTR(&tulip_bg_scroll_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_char), MP_ROM_PTR(&tulip_tfb_char_obj) },
    { MP_ROM_QSTR(MP_QSTR_frame_callback), MP_ROM_PTR(&tulip_frame_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_bitmap), MP_ROM_PTR(&tulip_bg_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_rect), MP_ROM_PTR(&tulip_bg_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_png), MP_ROM_PTR(&tulip_sprite_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_register), MP_ROM_PTR(&tulip_sprite_register_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_move), MP_ROM_PTR(&tulip_sprite_move_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_on), MP_ROM_PTR(&tulip_sprite_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_off), MP_ROM_PTR(&tulip_sprite_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_edit), MP_ROM_PTR(&tulip_edit_obj) },
    { MP_ROM_QSTR(MP_QSTR_screenshot), MP_ROM_PTR(&tulip_screenshot_obj) },

};

STATIC MP_DEFINE_CONST_DICT(tulip_module_globals, tulip_module_globals_table);

const mp_obj_module_t tulip_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&tulip_module_globals,
};


MP_REGISTER_MODULE(MP_QSTR_tulip, tulip_module, 1); // MICROPY_PY_TULIP);
