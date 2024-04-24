

#include <stdio.h>
#include "polyfills.h"

#include "py/runtime.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "display.h"
#include "bresenham.h"
#include "extmod/vfs.h"
#include "py/stream.h"
#include "alles.h"
#include "midi.h"
#include "ui.h"
#include "sequencer.h"
#include "keyscan.h"
#include "genhdr/mpversion.h"

#ifdef ESP_PLATFORM
#include "tasks.h"
#include "driver/rtc_io.h"
#endif


// Graphics

// tulip.display_clock(18)
// clock = tulip.display_clock()
extern float unix_display_set_clock();
extern void display_start();
extern void display_stop();
extern void save_tfb();
extern void restore_tfb();
extern uint8_t tfb_active;



STATIC mp_obj_t tulip_display_clock(size_t n_args, const mp_obj_t *args) {
    if(n_args==1) {
        uint16_t mhz = mp_obj_get_int(args[0]);
        display_set_clock(mhz);
        return mp_const_none;
    }
    return mp_obj_new_int(PIXEL_CLOCK_MHZ);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_clock_obj, 0, 1, tulip_display_clock);

extern void esp32s3_display_restart();
STATIC mp_obj_t tulip_display_restart(size_t n_args, const mp_obj_t *args) {
    #ifdef ESP_PLATFORM
    esp32s3_display_restart();
    #endif
    //display_set_clock(PIXEL_CLOCK_MHZ);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_restart_obj, 0, 0, tulip_display_restart);

STATIC mp_obj_t tulip_display_stop(size_t n_args, const mp_obj_t *args) {
    display_stop();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_stop_obj, 0, 0, tulip_display_stop);


STATIC mp_obj_t tulip_display_start(size_t n_args, const mp_obj_t *args) {
    display_start();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_display_start_obj, 0, 0, tulip_display_start);


STATIC mp_obj_t tulip_tfb_stop(size_t n_args, const mp_obj_t *args) {
    tfb_active = 0;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_stop_obj, 0, 0, tulip_tfb_stop);


STATIC mp_obj_t tulip_tfb_start(size_t n_args, const mp_obj_t *args) {
    tfb_active = 1;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_start_obj, 0, 0, tulip_tfb_start);

STATIC mp_obj_t tulip_tfb_log_start(size_t n_args, const mp_obj_t *args) {
    tfb_log = 1;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_log_start_obj, 0, 0, tulip_tfb_log_start);


STATIC mp_obj_t tulip_tfb_log_stop(size_t n_args, const mp_obj_t *args) {
    tfb_log = 0;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_log_stop_obj, 0, 0, tulip_tfb_log_stop);



STATIC mp_obj_t tulip_gpu_log(size_t n_args, const mp_obj_t *args) {
    gpu_log = 1;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_gpu_log_obj, 0, 0, tulip_gpu_log);



STATIC mp_obj_t tulip_tfb_save(size_t n_args, const mp_obj_t *args) {
    save_tfb();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_save_obj, 0, 0, tulip_tfb_save);

STATIC mp_obj_t tulip_tfb_restore(size_t n_args, const mp_obj_t *args) {
    restore_tfb();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_restore_obj, 0, 0, tulip_tfb_restore);

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


//tulip.bg_swap()
STATIC mp_obj_t tulip_bg_swap(size_t n_args, const mp_obj_t *args) {
    display_swap();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_swap_obj, 0, 0, tulip_bg_swap);



STATIC mp_obj_t tulip_ticks_ms(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_int(get_ticks_ms());
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_ticks_ms_obj, 0, 0, tulip_ticks_ms);

// tulip.bg_pixel(x,y, pal_idx)
// (r,g,b) = tulip.bg_pixel(x,y)
STATIC mp_obj_t tulip_bg_pixel(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    if(n_args == 3) { // set
        // Set the pixel
        uint8_t pal_idx = mp_obj_get_int(args[2]);
        display_set_bg_pixel_pal(x,y,pal_idx);
        return mp_const_none; 
    } else { // get the pixel
        return mp_obj_new_int( display_get_bg_pixel_pal(x,y)); 
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_pixel_obj, 2, 3, tulip_bg_pixel);



// This version of bg_clear is 2.5x as fast as the old one, as it's just copying within SPIRAM.
// tulip.bg_clear(pal_idx)
// tulip.bg_clear() # uses default
STATIC mp_obj_t tulip_bg_clear(size_t n_args, const mp_obj_t *args) {
    uint8_t pal_idx = bg_pal_color;
    if(n_args == 1) {
        pal_idx = mp_obj_get_int(args[0]);
    }
    // Set a single pixel
    display_set_bg_pixel_pal(0,0,pal_idx);
    // Copy that pixel
    for (uint16_t j = 0; j < V_RES+OFFSCREEN_Y_PX; j++) {
        for (uint16_t i = 0; i < H_RES+OFFSCREEN_X_PX; i++) {
            (bg)[(((j*(H_RES+OFFSCREEN_X_PX) + i)*BYTES_PER_PIXEL) + 0)] = (bg)[0];
        }
    }    
    return mp_const_none; 
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_clear_obj, 0, 1, tulip_bg_clear);


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
    if(n_args > 6) {
        display_bg_bitmap_blit_alpha(x,y,w,h,x1,y1);
    } else {
        display_bg_bitmap_blit(x,y,w,h,x1,y1);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_blit_obj, 6, 7, tulip_bg_blit);


mp_obj_t load_obj_file_into_ram(const char *fn);
void draw_sprite_wire(uint16_t sprite_no);

STATIC mp_obj_t tulip_wire_load(size_t n_args, const mp_obj_t *args) {
    mp_obj_t bytes = load_obj_file_into_ram(mp_obj_str_get_str(args[0]));
    return bytes;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_wire_load_obj, 1, 1, tulip_wire_load);


extern mp_obj_t render_wire_to_lines(uint8_t *buf, uint16_t x, uint16_t y, uint16_t scale, uint16_t theta,  int16_t max_faces);


STATIC mp_obj_t tulip_wire_to_lines(size_t n_args, const mp_obj_t *args) {
    mp_buffer_info_t bufinfo;
    mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
    if(n_args > 5) {
        return render_wire_to_lines(bufinfo.buf, mp_obj_get_int(args[1]), mp_obj_get_int(args[2]), mp_obj_get_int(args[3]), mp_obj_get_int(args[4]), mp_obj_get_int(args[5]));
    }
    return render_wire_to_lines(bufinfo.buf, mp_obj_get_int(args[1]), mp_obj_get_int(args[2]), mp_obj_get_int(args[3]), mp_obj_get_int(args[4]), -1);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_wire_to_lines_obj, 5, 6, tulip_wire_to_lines);


extern int8_t memorypcm_load(mp_obj_t bytes, uint32_t samplerate, uint8_t midinote, uint32_t loopstart, uint32_t loopend);
extern void memorypcm_unload();

STATIC mp_obj_t tulip_call_load_sample(size_t n_args, const mp_obj_t *args) {
    uint32_t samplerate = 44100;
    uint8_t midinote = 60;
    uint32_t loopstart = 0;
    uint32_t loopend = 0;
    if(n_args > 1) samplerate = mp_obj_get_int(args[1]);
    if(n_args > 2) midinote = mp_obj_get_int(args[2]);
    if(n_args > 3) loopstart = mp_obj_get_int(args[3]);
    if(n_args > 4) loopend = mp_obj_get_int(args[4]);
    int8_t patch = memorypcm_load(args[0], samplerate, midinote, loopstart, loopend);
    return mp_obj_new_int(patch);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_call_load_sample_obj, 1, 5, tulip_call_load_sample);

STATIC mp_obj_t tulip_unload_samples(size_t n_args, const mp_obj_t *args) {
    memorypcm_unload();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_unload_samples_obj, 0, 0, tulip_unload_samples);


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
                for(uint16_t i=0;i<strlen(str);i++) {
                    TFBf[y*TFB_COLS+x+i] = mp_obj_get_int(args[3]);
                }
            }
        }
        if(n_args > 4 ) {
            if(mp_obj_get_int(args[4])>=0) {
                for(uint16_t i=0;i<strlen(str);i++) {
                    TFBfg[y*TFB_COLS+x+i] = mp_obj_get_int(args[4]);
                }
            }
        }
        if(n_args > 5 ) {
            if(mp_obj_get_int(args[5])>=0) {
                for(uint16_t i=0;i<strlen(str);i++) {
                    TFBbg[y*TFB_COLS+x+i] = mp_obj_get_int(args[5]);
                }
            }
        }
        return mp_const_none; 
    } else {
        mp_obj_t tuple[5];
        tuple[0] = mp_obj_new_str((const char*)(TFB + (y*TFB_COLS+x)), 1);
        tuple[1] = mp_obj_new_int(TFBf[y*TFB_COLS+x]);
        tuple[2] = mp_obj_new_int(TFBfg[y*TFB_COLS+x]);
        tuple[3] = mp_obj_new_int(TFBbg[y*TFB_COLS+x]);
        tuple[4] = mp_obj_new_int(TFB[y*TFB_COLS+x]);
        return mp_obj_new_tuple(5,tuple);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_tfb_str_obj, 2, 6, tulip_tfb_str);





STATIC mp_obj_t tulip_screen_size(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[2];
    tuple[0] = mp_obj_new_int(H_RES);
    tuple[1] = mp_obj_new_int(V_RES);
    return mp_obj_new_tuple(2,tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_screen_size_obj, 0, 0, tulip_screen_size);


STATIC mp_obj_t tulip_board(size_t n_args, const mp_obj_t *args) {
#ifdef TULIP4_R10
    return mp_obj_new_str("TULIP4_R10", strlen("TULIP4_R10"));
#elif defined MATOUCH7
    return mp_obj_new_str("MATOUCH7", strlen("MATOUCH7"));
#elif defined TDECK
    return mp_obj_new_str("TDECK", strlen("TDECK"));
#elif defined N16R8
    return mp_obj_new_str("N16R8", strlen("N16R8"));
#elif defined N32R8
    return mp_obj_new_str("N32R8", strlen("N32R8"));
#elif defined TULIP_DESKTOP
    return mp_obj_new_str("DESKTOP", strlen("DESKTOP"));    
#else
    return mp_obj_new_str("OTHER", strlen("OTHER"));
#endif
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_board_obj, 0, 0, tulip_board);


mp_obj_t frame_callback = NULL; 
mp_obj_t frame_arg = NULL; 
mp_obj_t touch_callback = NULL; 
mp_obj_t keyboard_callback = NULL;

STATIC mp_obj_t mp_lv_task_handler(mp_obj_t arg)
{  
    lv_task_handler();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mp_lv_task_handler_obj, mp_lv_task_handler);

void tulip_frame_isr() {
    // schedule lvgl task
    mp_sched_schedule((mp_obj_t)&mp_lv_task_handler_obj, mp_const_none);

    if(frame_callback != NULL) {
        // Schedule the python callback given to run asap
        mp_sched_schedule(frame_callback, frame_arg);
#ifdef ESP_PLATFORM
        //mp_hal_wake_main_task_from_isr();
#endif
    }
}

/*void tulip_midi_isr() {
    if(midi_callback != NULL) {
        //fprintf(stderr, "LOG: midi_callback at time %lld\n", get_ticks_ms());
        mp_sched_schedule(midi_callback, mp_const_none); 
    }
}
*/

void tulip_touch_isr(uint8_t up) {
    if(touch_callback != NULL) {
        mp_sched_schedule(touch_callback, mp_obj_new_int(up)); 
    }
}




STATIC mp_obj_t tulip_midi_add_callback(size_t n_args, const mp_obj_t *args) {
    uint8_t index = 5;
    if(midi_callbacks[0]==NULL) {
        index = 0;
    } else if(midi_callbacks[1]==NULL) {
        index = 1;
    } else if(midi_callbacks[2]==NULL) {
        index = 2;
    } else if(midi_callbacks[3]==NULL) {
        index = 3;
    }
    if(index<4) {
        midi_callbacks[index] = args[0];
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("No more MIDI slots available"));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_add_callback_obj, 1, 2, tulip_midi_add_callback);

STATIC mp_obj_t tulip_midi_remove_callback(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<MIDI_SLOTS;i++) {
        if(midi_callbacks[i] == args[0]) {
            midi_callbacks[i] = NULL;
        }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_remove_callback_obj, 1, 1, tulip_midi_remove_callback);

STATIC mp_obj_t tulip_midi_remove_callbacks(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<MIDI_SLOTS;i++) {
        midi_callbacks[i] = NULL;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_remove_callbacks_obj, 0, 0, tulip_midi_remove_callbacks);






STATIC mp_obj_t tulip_seq_add_callback(size_t n_args, const mp_obj_t *args) {
    uint8_t index = 5;
    if(sequencer_callbacks[0]==NULL) {
        index = 0;
    } else if(sequencer_callbacks[1]==NULL) {
        index = 1;
    } else if(sequencer_callbacks[2]==NULL) {
        index = 2;
    } else if(sequencer_callbacks[3]==NULL) {
        index = 3;
    }
    if(index<4) {
        sequencer_callbacks[index] = args[0];
        if(n_args == 2) {
            sequencer_dividers[index] = mp_obj_get_int(args[1]);
        } else {
            sequencer_dividers[index] = sequencer_ppq;            
        }
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("No more sequencer slots available"));
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_add_callback_obj, 1, 2, tulip_seq_add_callback);

STATIC mp_obj_t tulip_seq_remove_callback(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
        if(sequencer_callbacks[i] == args[0]) {
            sequencer_callbacks[i] = NULL;
            sequencer_dividers[i] = 0;
        }
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_remove_callback_obj, 1, 1, tulip_seq_remove_callback);

STATIC mp_obj_t tulip_seq_remove_callbacks(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<SEQUENCER_SLOTS;i++) {
        sequencer_callbacks[i] = NULL;
        sequencer_dividers[i] = 0;
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_remove_callbacks_obj, 0, 0, tulip_seq_remove_callbacks);


STATIC mp_obj_t tulip_seq_bpm(size_t n_args, const mp_obj_t *args) {
    if(n_args == 1) {
        sequencer_bpm = mp_obj_get_float(args[0]);
        sequencer_recompute();
    } else {
        return mp_obj_new_float(sequencer_bpm);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_bpm_obj, 0, 1, tulip_seq_bpm);


STATIC mp_obj_t tulip_seq_ppq(size_t n_args, const mp_obj_t *args) {
    if(n_args == 1) {
        sequencer_ppq = mp_obj_get_int(args[0]);
        sequencer_recompute();
    } else {
        return mp_obj_new_int(sequencer_ppq);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_ppq_obj, 0, 1, tulip_seq_ppq);

STATIC mp_obj_t tulip_seq_latency(size_t n_args, const mp_obj_t *args) {
    if(n_args == 1) {
        sequencer_latency_ms = mp_obj_get_int(args[0]);
    } else {
        return mp_obj_new_int(sequencer_latency_ms);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_latency_obj, 0, 1, tulip_seq_latency);


STATIC mp_obj_t tulip_seq_stop(size_t n_args, const mp_obj_t *args) {
    sequencer_stop();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_stop_obj, 0, 0, tulip_seq_stop);


STATIC mp_obj_t tulip_seq_start(size_t n_args, const mp_obj_t *args) {
    sequencer_start();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_start_obj, 0, 0, tulip_seq_start);


STATIC mp_obj_t tulip_seq_ticks(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_int(sequencer_tick_count);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_seq_ticks_obj, 0, 0, tulip_seq_ticks);


// tulip.frame_callback(cb, arg)
// tulip.frame_callback() -- stops 
STATIC mp_obj_t tulip_frame_callback(size_t n_args, const mp_obj_t *args) {
    if(n_args == 0) {
        frame_callback = NULL;
    } else {
        frame_callback = args[0];
    }
    if(n_args > 1) {
        frame_arg = args[1];
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_frame_callback_obj, 0, 2, tulip_frame_callback);



STATIC mp_obj_t tulip_touch_callback(size_t n_args, const mp_obj_t *args) {
    if(n_args == 0) {
        touch_callback = NULL;
    } else {
        touch_callback = args[0];
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_touch_callback_obj, 0, 1, tulip_touch_callback);

STATIC mp_obj_t tulip_keyboard_callback(size_t n_args, const mp_obj_t *args) {
    if(n_args == 0) {
        keyboard_callback = NULL;
    } else {
        keyboard_callback = args[0];
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_keyboard_callback_obj, 0, 1, tulip_keyboard_callback);



STATIC mp_obj_t tulip_midi_in(size_t n_args, const mp_obj_t *args) {
    if(midi_queue_head != midi_queue_tail) {
        int16_t prev_head = midi_queue_head;
        // Step on the head, hope no-one notices before we pop it.
        midi_queue_head = (midi_queue_head + 1) % MIDI_QUEUE_DEPTH;
        return mp_obj_new_bytes(last_midi[prev_head],
                                last_midi_len[prev_head]);
    } 
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_in_obj, 0, 0, tulip_midi_in);


STATIC mp_obj_t tulip_midi_out(size_t n_args, const mp_obj_t *args) {
    if(mp_obj_get_type(args[0]) == &mp_type_bytes) {
        mp_buffer_info_t bufinfo;
        mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
        midi_out((uint8_t*)bufinfo.buf, bufinfo.len);
    } else {
        mp_obj_t *items;
        size_t len;
        mp_obj_get_array(args[0], &len, &items);
        uint8_t *b = malloc_caps(len, MALLOC_CAP_INTERNAL);
        for(uint16_t i=0;i<(uint16_t)len;i++) {
            b[i] = mp_obj_get_int(items[i]);
        }
        midi_out(b, len);
        free_caps(b);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_out_obj, 1, 1, tulip_midi_out);


// Send a message on the "local bus", as if it was received from physical midi in
STATIC mp_obj_t tulip_midi_local(size_t n_args, const mp_obj_t *args) {
    if(mp_obj_get_type(args[0]) == &mp_type_bytes) {
        mp_buffer_info_t bufinfo;
        mp_get_buffer(args[0], &bufinfo, MP_BUFFER_READ);
        midi_local((uint8_t*)bufinfo.buf, bufinfo.len);
    } else {
        mp_obj_t *items;
        size_t len;
        mp_obj_get_array(args[0], &len, &items);
        uint8_t *b = malloc_caps(len, MALLOC_CAP_INTERNAL);
        for(uint16_t i=0;i<(uint16_t)len;i++) {
            b[i] = mp_obj_get_int(items[i]);
        }
        midi_local(b, len);
        free_caps(b);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_midi_local_obj, 1, 1, tulip_midi_local);


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

extern uint8_t spriteno_activated; // tells the drawing loop to look at sprites
//sprite_register(34, mem_pos, w,h, type) # 34 = sprite number, can be up to ...
STATIC mp_obj_t tulip_sprite_register(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    if(spriteno_activated < spriteno+1)
        spriteno_activated = spriteno+1;
    uint32_t mem_pos = mp_obj_get_int(args[1]);
    if(spriteno < SPRITES) {
        sprite_mem[spriteno] = mem_pos;
    }
    if(n_args > 2) {
        uint16_t width = mp_obj_get_int(args[2]);
        uint16_t height = mp_obj_get_int(args[3]);
        if(spriteno < SPRITES) {
            sprite_w_px[spriteno] = width;
            sprite_h_px[spriteno] = height;
        }
    } else {
        sprite_vis[spriteno] = SPRITE_IS_WIREFRAME;
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_register_obj, 2, 4, tulip_sprite_register);


//sprite_move(34, x,y)
STATIC mp_obj_t tulip_sprite_move(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);
    if(spriteno < SPRITES) {
        if(check_dim_xy(x,y)) {
            sprite_x_px[spriteno] = x;
            sprite_y_px[spriteno] = y;
        } else {
            fprintf(stderr, "bad sprite xy %d %d\n", x,y);
        }
    } else {
        fprintf(stderr, "move bad spriteno %d\n", spriteno);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_move_obj, 3, 3, tulip_sprite_move);

STATIC mp_obj_t tulip_sprite_on(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    if(spriteno < SPRITES) sprite_vis[spriteno] = SPRITE_IS_SPRITE;        
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_on_obj, 1, 1, tulip_sprite_on);


STATIC mp_obj_t tulip_sprite_off(size_t n_args, const mp_obj_t *args) {
    uint16_t spriteno = mp_obj_get_int(args[0]);
    if(spriteno < SPRITES) sprite_vis[spriteno] = 0;
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_off_obj, 1, 1, tulip_sprite_off);


STATIC mp_obj_t tulip_sprite_clear(size_t n_args, const mp_obj_t *args) {
    display_reset_sprites();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_sprite_clear_obj, 0, 0, tulip_sprite_clear);


STATIC mp_obj_t tulip_collisions(size_t n_args, const mp_obj_t *args) {
    mp_obj_t list = mp_obj_new_list(0, NULL);
    // iterate through all fields
    for(uint8_t a=0;a<32;a++) {
        for(uint8_t b=a+1;b<32;b++) {
            if(collide_mask_get(a,b)) {
                mp_obj_t tuple[2];
                tuple[0] = mp_obj_new_int(a);
                tuple[1] = mp_obj_new_int(b);
                mp_obj_list_append(list, mp_obj_new_tuple(2, tuple));
            }
        }
    }
    // clear collision
    // TODO -- divine 62 a better way 
    for(uint8_t i=0;i<62;i++) collision_bitfield[i] = 0;
    return list;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_collisions_obj, 0, 0, tulip_collisions);



extern void editor(const char * filename);
STATIC mp_obj_t tulip_run_editor(size_t n_args, const mp_obj_t *args) {
    if(n_args) {
        editor(mp_obj_str_get_str(args[0]));
    } else {
        editor(NULL);
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_run_editor_obj, 0, 1, tulip_run_editor);

extern void unix_display_timings(uint16_t, uint16_t, uint16_t, uint16_t);
STATIC mp_obj_t tulip_gpu_reset(size_t n_args, const mp_obj_t *args) {
    display_reset_bg();
    display_reset_sprites();
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





extern void mcast_send(char*, uint16_t len);

STATIC mp_obj_t tulip_alles_send(size_t n_args, const mp_obj_t *args) {
    if(n_args > 1) {
        if(mp_obj_get_int(args[1])) { // mesh
            mcast_send( (char*)mp_obj_str_get_str(args[0]), strlen(mp_obj_str_get_str(args[0])));
            return mp_const_none;
        }
    }
    alles_send_message((char*)mp_obj_str_get_str(args[0]), strlen(mp_obj_str_get_str(args[0])));
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_alles_send_obj, 1, 2, tulip_alles_send);

extern char * alles_local_ip;
STATIC mp_obj_t tulip_multicast_start(size_t n_args, const mp_obj_t *args) {
    const char * local_ip = mp_obj_str_get_str(args[0]);
    uint8_t local_node = mp_obj_get_int(args[1]);
    if(strlen(local_ip)>2) {
        strcpy(alles_local_ip, local_ip);
    }
    alles_init_multicast(local_node);
    return mp_const_none;

}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_multicast_start_obj, 2, 2, tulip_multicast_start);


extern uint8_t alive;
extern int16_t client_id;
extern int64_t clocks[255];
extern int64_t ping_times[255];

STATIC mp_obj_t tulip_alles_map(size_t n_args, const mp_obj_t *args) {
    mp_obj_t list = mp_obj_new_list(0, NULL);
    for(uint8_t i=0;i<255;i++) {
        mp_obj_t tuple[3];
        if(clocks[i]>0) {
            tuple[0] = mp_obj_new_int(i);
            tuple[1] = mp_obj_new_int(clocks[i]);
            tuple[2] = mp_obj_new_int(ping_times[i]);
            mp_obj_list_append(list, mp_obj_new_tuple(3, tuple));
        }
    }
    // TODO - sort this so that client 0 is the earliest, etc. Maybe do the sort in python 
    return list;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_alles_map_obj, 0, 0, tulip_alles_map);

extern uint8_t ipv4_quartet;
STATIC mp_obj_t tulip_set_quartet(size_t n_args, const mp_obj_t *args) {
    ipv4_quartet = mp_obj_get_int(args[0]);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_set_quartet_obj, 1, 1, tulip_set_quartet);

STATIC mp_obj_t tulip_brightness(size_t n_args, const mp_obj_t *args) {
    if(n_args > 0) {
        display_brightness(mp_obj_get_int(args[0]));
        return mp_const_none;
    } else {
        return mp_obj_new_int(brightness);
    }

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_brightness_obj, 0, 1, tulip_brightness);


extern const uint16_t rgb332_rgb565_i[256];

STATIC mp_obj_t tulip_rgb332_565(size_t n_args, const mp_obj_t *args) {
    return mp_obj_new_int(rgb332_rgb565_i[mp_obj_get_int(args[0])]);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_rgb332_565_obj, 1, 1, tulip_rgb332_565);



extern uint8_t last_scan[8];
STATIC mp_obj_t tulip_keys(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[7];
    tuple[0] = mp_obj_new_int(last_scan[0]);
    for(uint8_t i=0;i<6;i++) tuple[i+1] = mp_obj_new_int(last_scan[i+2]);
    return mp_obj_new_tuple(7, tuple);

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_keys_obj, 0, 0, tulip_keys);

extern int16_t last_touch_x[3];
extern int16_t last_touch_y[3];
STATIC mp_obj_t tulip_touch(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[6];
    for(uint8_t i=0;i<3;i++) {
        tuple[i*2] = mp_obj_new_int(last_touch_x[i]);
        tuple[i*2+1] = mp_obj_new_int(last_touch_y[i]);
    }
    return mp_obj_new_tuple(6, tuple);

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_touch_obj, 0, 0, tulip_touch);

#ifdef TULIP_DIY
#include "ft5x06_touchscreen.h"
#elif defined MAKERFABS
#include "gt911_touchscreen.h"
#endif

#ifdef ESP_PLATFORM
extern int16_t touch_x_delta;
extern int16_t touch_y_delta;
#else
int16_t touch_x_delta = 0;
int16_t touch_y_delta = 0;
#endif

STATIC mp_obj_t tulip_touch_delta(size_t n_args, const mp_obj_t *args) {
    if(n_args > 0) {
        touch_x_delta = mp_obj_get_int(args[0]);
        touch_y_delta = mp_obj_get_int(args[1]);
    } else {
        mp_obj_t tuple[2];
        tuple[0] = mp_obj_new_int(touch_x_delta);
        tuple[1] = mp_obj_new_int(touch_y_delta);
        return mp_obj_new_tuple(2, tuple);
    }
    return mp_const_none;

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_touch_delta_obj, 0, 2, tulip_touch_delta);


STATIC mp_obj_t tulip_key_remap(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<MAX_KEY_REMAPS;i++) {
        if(key_remaps[i].scan==0) {
            key_remaps[i].scan = mp_obj_get_int(args[0]);
            key_remaps[i].mod = mp_obj_get_int(args[1]);
            key_remaps[i].code = mp_obj_get_int(args[2]);
            i = MAX_KEY_REMAPS + 1;
        }
    }
    return mp_const_none;

}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_remap_obj, 3, 3, tulip_key_remap);

STATIC mp_obj_t tulip_key_remaps_clear(size_t n_args, const mp_obj_t *args) {
    for(uint8_t i=0;i<MAX_KEY_REMAPS;i++) {
        key_remaps[i].scan =0 ;
        key_remaps[i].mod =0 ;
        key_remaps[i].code =0 ;
    }
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_remaps_clear_obj, 0, 0, tulip_key_remaps_clear);


STATIC mp_obj_t tulip_key_wait(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_int(mp_hal_stdin_rx_chr());
    tuple[1] = mp_obj_new_int(last_held_code);
    tuple[2] = mp_obj_new_int(last_held_modifier);
    return mp_obj_new_tuple(3, tuple);
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

void send_key_to_micropython(uint16_t key);
STATIC mp_obj_t tulip_key_send(size_t n_args, const mp_obj_t *args) {
    send_key_to_micropython(mp_obj_get_int(args[0]));
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_key_send_obj, 1, 1, tulip_key_send);


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

#ifndef ESP_PLATFORM
extern char* get_tulip_home_path();
STATIC mp_obj_t tulip_app_path(size_t n_args, const mp_obj_t *args) {
#ifndef __linux__
    char * path = get_tulip_home_path();
    return mp_obj_new_str(path, strlen(path));
#endif
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_app_path_obj, 0, 0, tulip_app_path);
#endif


STATIC mp_obj_t tulip_bg_bezier(size_t n_args, const mp_obj_t *args) {
    uint16_t x0 = mp_obj_get_int(args[0]);
    uint16_t y0 = mp_obj_get_int(args[1]);
    uint16_t x1 = mp_obj_get_int(args[2]);
    uint16_t y1 = mp_obj_get_int(args[3]);
    uint16_t x2 = mp_obj_get_int(args[4]);
    uint16_t y2 = mp_obj_get_int(args[5]);
    uint16_t pal_idx = mp_obj_get_int(args[6]);
    plotQuadBezier(x0,y0,x1,y1,x2,y2,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_bezier_obj, 7, 7, tulip_bg_bezier);



STATIC mp_obj_t tulip_bg_line(size_t n_args, const mp_obj_t *args) {
    int16_t x0 = mp_obj_get_int(args[0]);
    int16_t y0 = mp_obj_get_int(args[1]);
    int16_t x1 = mp_obj_get_int(args[2]);
    int16_t y1 = mp_obj_get_int(args[3]);
    uint8_t pal_idx = mp_obj_get_int(args[4]);
    drawLine_scanline(x0,y0,x1,y1,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_line_obj, 5, 5, tulip_bg_line);

STATIC mp_obj_t tulip_bg_roundrect(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    uint16_t r = mp_obj_get_int(args[4]);
    uint16_t pal_idx = mp_obj_get_int(args[5]);
    if(n_args > 6) {
        if(mp_obj_get_int(args[6])>0) {
            fillRoundRect(x,y,w,h,r,pal_idx);
            return mp_const_none;
        }
    }
    drawRoundRect(x,y,w,h,r,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_roundrect_obj, 6, 7, tulip_bg_roundrect);


STATIC mp_obj_t tulip_bg_rect(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t w = mp_obj_get_int(args[2]);
    uint16_t h = mp_obj_get_int(args[3]);
    uint16_t pal_idx = mp_obj_get_int(args[4]);
    if(n_args > 5) {
        if(mp_obj_get_int(args[5])>0) {
            fillRect(x,y,w,h,pal_idx);
            return mp_const_none;
        } 
    }
    drawRect(x,y,w,h,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_rect_obj, 5, 6, tulip_bg_rect);

STATIC mp_obj_t tulip_bg_circle(size_t n_args, const mp_obj_t *args) {
    uint16_t x = mp_obj_get_int(args[0]);
    uint16_t y = mp_obj_get_int(args[1]);
    uint16_t r = mp_obj_get_int(args[2]);
    uint16_t pal_idx = mp_obj_get_int(args[3]);
    if(n_args > 4) {
        if(mp_obj_get_int(args[4])>0) {
            fillCircle(x,y,r,pal_idx);
            return mp_const_none;
        } 
    }
    drawCircle(x,y,r,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_circle_obj, 4, 5, tulip_bg_circle);


STATIC mp_obj_t tulip_bg_triangle(size_t n_args, const mp_obj_t *args) {
    uint16_t x0 = mp_obj_get_int(args[0]);
    uint16_t y0 = mp_obj_get_int(args[1]);
    uint16_t x1 = mp_obj_get_int(args[2]);
    uint16_t y1 = mp_obj_get_int(args[3]);
    uint16_t x2 = mp_obj_get_int(args[4]);
    uint16_t y2 = mp_obj_get_int(args[5]);
    uint16_t pal_idx = mp_obj_get_int(args[6]);
    if(n_args > 7) {
        if(mp_obj_get_int(args[7])>0) {
            fillTriangle(x0, y0, x1, y1, x2, y2, pal_idx);
            return mp_const_none;
        }
    }
    drawTriangle(x0, y0, x1, y1, x2, y2, pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_triangle_obj, 7, 8, tulip_bg_triangle);

STATIC mp_obj_t tulip_bg_fill(size_t n_args, const mp_obj_t *args) {
    uint16_t x0 = mp_obj_get_int(args[0]);
    uint16_t y0 = mp_obj_get_int(args[1]);
    uint16_t pal_idx = mp_obj_get_int(args[2]);
    fill(x0,y0,pal_idx);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_fill_obj, 3, 3, tulip_bg_fill);

STATIC mp_obj_t tulip_bg_char(size_t n_args, const mp_obj_t *args) {
    uint16_t c = mp_obj_get_int(args[0]);
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);
    uint16_t pal_idx = mp_obj_get_int(args[3]);
    uint16_t font_no = mp_obj_get_int(args[4]);
    return mp_obj_new_int(draw_new_char(c, x, y, pal_idx, font_no));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_char_obj, 5, 5, tulip_bg_char);

STATIC mp_obj_t tulip_bg_str(size_t n_args, const mp_obj_t *args) {
    const char *str =  mp_obj_str_get_str(args[0]);
    uint16_t x = mp_obj_get_int(args[1]);
    uint16_t y = mp_obj_get_int(args[2]);
    uint16_t pal_idx = mp_obj_get_int(args[3]);
    uint16_t font_no = mp_obj_get_int(args[4]);
    if(n_args>5) {
        uint16_t w = mp_obj_get_int(args[5]);
        uint16_t h = mp_obj_get_int(args[6]);        
        return mp_obj_new_int(draw_new_str(str, x, y, pal_idx, font_no, w, h, 1));
    }
    return mp_obj_new_int(draw_new_str(str, x, y, pal_idx, font_no, 0, 0, 0));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_bg_str_obj, 5, 7, tulip_bg_str);


STATIC mp_obj_t tulip_build_strings(size_t n_args, const mp_obj_t *args) {
    mp_obj_t tuple[3];
    tuple[0] = mp_obj_new_str(MICROPY_GIT_TAG, strlen(MICROPY_GIT_TAG));
    tuple[1] = mp_obj_new_str(MICROPY_GIT_HASH, strlen(MICROPY_GIT_HASH));
    tuple[2] = mp_obj_new_str(MICROPY_BUILD_DATE, strlen(MICROPY_BUILD_DATE));
    return mp_obj_new_tuple(3, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_build_strings_obj, 0,0, tulip_build_strings);

extern int16_t lvgl_is_repl;
STATIC mp_obj_t tulip_set_screen_as_repl(size_t n_args, const mp_obj_t *args) {
    // set a glob var of the current lvgl screen to test in C for the repl screen later
    // called by boot ui.py 
    int16_t x = mp_obj_get_int(args[0]);
    lvgl_is_repl = x;
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(tulip_set_screen_as_repl_obj, 1,1, tulip_set_screen_as_repl);



STATIC const mp_rom_map_elem_t tulip_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR__tulip) },
    { MP_ROM_QSTR(MP_QSTR_display_clock), MP_ROM_PTR(&tulip_display_clock_obj) },
    { MP_ROM_QSTR(MP_QSTR_display_restart), MP_ROM_PTR(&tulip_display_restart_obj) },
    { MP_ROM_QSTR(MP_QSTR_display_stop), MP_ROM_PTR(&tulip_display_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_display_start), MP_ROM_PTR(&tulip_display_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_swap), MP_ROM_PTR(&tulip_bg_swap_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_stop), MP_ROM_PTR(&tulip_tfb_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_log_start), MP_ROM_PTR(&tulip_tfb_log_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_log_stop), MP_ROM_PTR(&tulip_tfb_log_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_start), MP_ROM_PTR(&tulip_tfb_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_save), MP_ROM_PTR(&tulip_tfb_save_obj) },
    { MP_ROM_QSTR(MP_QSTR_tfb_restore), MP_ROM_PTR(&tulip_tfb_restore_obj) },
    { MP_ROM_QSTR(MP_QSTR_fps), MP_ROM_PTR(&tulip_fps_obj) },
    { MP_ROM_QSTR(MP_QSTR_gpu), MP_ROM_PTR(&tulip_gpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&tulip_ticks_ms_obj) },
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
    { MP_ROM_QSTR(MP_QSTR_touch_callback), MP_ROM_PTR(&tulip_touch_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_keyboard_callback), MP_ROM_PTR(&tulip_keyboard_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_add_callback), MP_ROM_PTR(&tulip_seq_add_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_remove_callback), MP_ROM_PTR(&tulip_seq_remove_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_remove_callbacks), MP_ROM_PTR(&tulip_seq_remove_callbacks_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_add_callback), MP_ROM_PTR(&tulip_midi_add_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_remove_callback), MP_ROM_PTR(&tulip_midi_remove_callback_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_remove_callbacks), MP_ROM_PTR(&tulip_midi_remove_callbacks_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_bpm), MP_ROM_PTR(&tulip_seq_bpm_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_ppq), MP_ROM_PTR(&tulip_seq_ppq_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_latency), MP_ROM_PTR(&tulip_seq_latency_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_stop), MP_ROM_PTR(&tulip_seq_stop_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_start), MP_ROM_PTR(&tulip_seq_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_seq_ticks), MP_ROM_PTR(&tulip_seq_ticks_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_in), MP_ROM_PTR(&tulip_midi_in_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_out), MP_ROM_PTR(&tulip_midi_out_obj) },
    { MP_ROM_QSTR(MP_QSTR_midi_local), MP_ROM_PTR(&tulip_midi_local_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_bitmap), MP_ROM_PTR(&tulip_bg_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_blit), MP_ROM_PTR(&tulip_bg_blit_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_png), MP_ROM_PTR(&tulip_sprite_png_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_bitmap), MP_ROM_PTR(&tulip_sprite_bitmap_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_register), MP_ROM_PTR(&tulip_sprite_register_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_move), MP_ROM_PTR(&tulip_sprite_move_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_on), MP_ROM_PTR(&tulip_sprite_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_off), MP_ROM_PTR(&tulip_sprite_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_sprite_clear), MP_ROM_PTR(&tulip_sprite_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_wire_to_lines), MP_ROM_PTR(&tulip_wire_to_lines_obj) },
    { MP_ROM_QSTR(MP_QSTR_wire_load), MP_ROM_PTR(&tulip_wire_load_obj) },
    { MP_ROM_QSTR(MP_QSTR_collisions), MP_ROM_PTR(&tulip_collisions_obj) },
    { MP_ROM_QSTR(MP_QSTR_run_editor), MP_ROM_PTR(&tulip_run_editor_obj) },
    { MP_ROM_QSTR(MP_QSTR_int_screenshot), MP_ROM_PTR(&tulip_int_screenshot_obj) },
    { MP_ROM_QSTR(MP_QSTR_multicast_start), MP_ROM_PTR(&tulip_multicast_start_obj) },
    { MP_ROM_QSTR(MP_QSTR_alles_send), MP_ROM_PTR(&tulip_alles_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_alles_map), MP_ROM_PTR(&tulip_alles_map_obj) },
    { MP_ROM_QSTR(MP_QSTR_brightness), MP_ROM_PTR(&tulip_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_rgb332_565), MP_ROM_PTR(&tulip_rgb332_565_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_quartet), MP_ROM_PTR(&tulip_set_quartet_obj) },
    { MP_ROM_QSTR(MP_QSTR_keys), MP_ROM_PTR(&tulip_keys_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch), MP_ROM_PTR(&tulip_touch_obj) },
    { MP_ROM_QSTR(MP_QSTR_touch_delta), MP_ROM_PTR(&tulip_touch_delta_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_remap), MP_ROM_PTR(&tulip_key_remap_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_remaps_clear), MP_ROM_PTR(&tulip_key_remaps_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_wait), MP_ROM_PTR(&tulip_key_wait_obj) },
    { MP_ROM_QSTR(MP_QSTR_key), MP_ROM_PTR(&tulip_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_scan), MP_ROM_PTR(&tulip_key_scan_obj) },
    { MP_ROM_QSTR(MP_QSTR_key_send), MP_ROM_PTR(&tulip_key_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_cpu), MP_ROM_PTR(&tulip_cpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_gpu_reset), MP_ROM_PTR(&tulip_gpu_reset_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_circle), MP_ROM_PTR(&tulip_bg_circle_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_bezier), MP_ROM_PTR(&tulip_bg_bezier_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_line), MP_ROM_PTR(&tulip_bg_line_obj) },
    { MP_ROM_QSTR(MP_QSTR_call_load_sample), MP_ROM_PTR(&tulip_call_load_sample_obj) },
    { MP_ROM_QSTR(MP_QSTR_unload_samples), MP_ROM_PTR(&tulip_unload_samples_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_roundrect), MP_ROM_PTR(&tulip_bg_roundrect_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_triangle), MP_ROM_PTR(&tulip_bg_triangle_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_fill), MP_ROM_PTR(&tulip_bg_fill_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_rect), MP_ROM_PTR(&tulip_bg_rect_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_char), MP_ROM_PTR(&tulip_bg_char_obj) },
    { MP_ROM_QSTR(MP_QSTR_bg_str), MP_ROM_PTR(&tulip_bg_str_obj) },
    { MP_ROM_QSTR(MP_QSTR_gpu_log), MP_ROM_PTR(&tulip_gpu_log_obj) },
    { MP_ROM_QSTR(MP_QSTR_screen_size), MP_ROM_PTR(&tulip_screen_size_obj) },
    { MP_ROM_QSTR(MP_QSTR_board), MP_ROM_PTR(&tulip_board_obj) }, 
    { MP_ROM_QSTR(MP_QSTR_set_screen_as_repl), MP_ROM_PTR(&tulip_set_screen_as_repl_obj) },
    { MP_ROM_QSTR(MP_QSTR_build_strings), MP_ROM_PTR(&tulip_build_strings_obj) },


// Special platform specific things
#ifndef ESP_PLATFORM
    { MP_ROM_QSTR(MP_QSTR_app_path), MP_ROM_PTR(&tulip_app_path_obj) },
#endif

};

STATIC MP_DEFINE_CONST_DICT(tulip_module_globals, tulip_module_globals_table);

const mp_obj_module_t tulip_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&tulip_module_globals,
};


MP_REGISTER_MODULE(MP_QSTR__tulip, tulip_module); // MICROPY_PY_TULIP);
