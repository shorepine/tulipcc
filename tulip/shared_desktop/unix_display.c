// GPU driver via SDL2 FB 
#include <SDL.h>
#include "polyfills.h"
#include "display.h"
#include "keyscan.h"
#include "ui.h"
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Renderer *fixed_fps_renderer;
SDL_GameController *gp;
SDL_Rect viewport;
SDL_Rect tulip_rect;
SDL_Texture *framebuffer;
uint8_t *frame_bb;
#define BYTES_PER_PIXEL 1
int64_t frame_ticks = 0;
int8_t unix_display_flag = 0;
SDL_Keymod last_held_mod;
int drawable_w = 0;
int drawable_h = 0;
int keyboard_top_y = 0;
float viewport_scale = 1.0;

SDL_Rect button_bar;
SDL_Rect btn_ctrl, btn_tab, btn_esc, btn_l, btn_r, btn_u, btn_d;

extern int get_keyboard_y();
extern uint8_t is_iphone();
extern uint8_t is_ipad();


void unix_set_fps_from_parameters() {
    // use the screen res and clock to discern a new FPS, based on real life measurements on tulip cc
    float fps_map[20] = {14.98, 18.55, 23.19, 30.91, 46.37, 21.47, 34.36, 42.95, 57.26, 85.90,
                         19.91, 30.26, 37.82, 50.43, 75.65, 35.03, 56.05, 70.07, 93.45, 140.13};
    uint8_t clocks[5]= {10,14,18,22,28};
    uint16_t h_reses[2] = {1024, 512};
    uint16_t v_reses[2] = {600, 300};
    uint8_t fps_i = 0;
    for(uint8_t h_res_i = 0;h_res_i < 2;h_res_i++) {
        for(uint8_t v_res_i = 0;v_res_i < 2;v_res_i++) {
            for(uint8_t clock_i = 0;clock_i < 5;clock_i++) {
                if(H_RES==h_reses[h_res_i] && V_RES==v_reses[v_res_i] && PIXEL_CLOCK_MHZ==clocks[clock_i]) {
                    //fprintf(stderr, "Returning %2.2f FPS for res %d %d clock %d\n", fps_map[fps_i], H_RES, V_RES, PIXEL_CLOCK_MHZ);
                    reported_fps = fps_map[fps_i];
                    // get out of search 
                    clock_i = 10; v_res_i = 10; h_res_i = 10; 
                }
                fps_i++;
            }
        }
    }
    if(fps_i == 20) {
        fprintf(stderr, "Problem: could not find matching fps from res %d, %d clock %d. returning 30fps.\n", H_RES, V_RES, PIXEL_CLOCK_MHZ);
        reported_fps = 30;
    }


}

void unix_display_set_clock(uint8_t mhz) {  
    PIXEL_CLOCK_MHZ = mhz;
    unix_set_fps_from_parameters();
}


void unix_display_timings(uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3, uint32_t t4, uint32_t t5) {
    fprintf(stderr, "Stopping display task\n");
    H_RES = t0;
    V_RES = t1; 
    OFFSCREEN_X_PX = t2; 
    OFFSCREEN_Y_PX = t3; 
    H_RES_D = t4;
    V_RES_D = t5;
    TFB_ROWS = (V_RES_D/FONT_HEIGHT);
    TFB_COLS = (H_RES_D/FONT_WIDTH);
    BOUNCE_BUFFER_SIZE_PX = (H_RES*FONT_HEIGHT) ;
    unix_display_flag = -2; // restart display with new timings
}

// Compute the viewport and optionally new optimal Tulip size for this display 
int8_t compute_viewport(uint16_t tw, uint16_t th, int8_t resize_tulip) {
    int sw = tw;
    int sh = th;
    #ifdef __TULIP_IOS__
        // Boot up an SDL window so we can get the device size
        SDL_Window *query_window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, tw, th,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_Surface *query_surface = SDL_GetWindowSurface(query_window);
        SDL_Renderer *query_renderer = SDL_CreateSoftwareRenderer(query_surface);
        SDL_GetRendererOutputSize(query_renderer, &sw, &sh);
        SDL_GetWindowSize(query_window, &drawable_w, &drawable_h);
        fprintf(stderr, "From setup I get drawable %d %d and screen %d %d.\n", drawable_w, drawable_h, sw, sh);
        SDL_DestroyRenderer(query_renderer);
        SDL_FreeSurface(query_surface);
        SDL_DestroyWindow(query_window);
    #else
        sw = tw;
        drawable_w = tw;
        sh = th;
        drawable_h = th;
    #endif
    
    

    viewport_scale = (float)sw/(float)drawable_w;
    tulip_rect.x = 0; 
    tulip_rect.y = 0; 
    tulip_rect.w = tw; 
    tulip_rect.h = th; 

    if(drawable_w < 10 || drawable_h < 10) {
        fprintf(stderr, "problem reading screen size\n");
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = tw;
        viewport.h = th;
        drawable_w = tw;
        drawable_h = th;
        viewport_scale = 1;
        return 0;
    }

    // Adjust y for bezels
    viewport.y = 0;
    #ifdef __TULIP_IOS__
        int orig_sh = sh;
        // Is this landscape or portrait
        if(sw > sh) {
            sh = sh - 100; // leave room for the bezel
            viewport.y = 100;
        } else {
            sh = sh-200; // leave room for the notch
            viewport.y = 200;
        }
        // If no keyboard, don't draw button bar
        if(keyboard_top_y>0) {
            button_bar.w = sw;
            button_bar.h = (int)(50.0 * viewport_scale);
            button_bar.x = 0;
            button_bar.y = orig_sh - keyboard_top_y - button_bar.h;
            fprintf(stderr, "setting button bar to %d %d %d %d\n", button_bar.x,button_bar.y,button_bar.w,button_bar.h);
        } else {
            button_bar.w = 0;
            button_bar.h = 0;
        }
        sh = sh - keyboard_top_y - button_bar.h; // leave room for the keyboard
    #endif
    fprintf(stderr, "before resize: scale %f. sw %d sh %d dw %d dh %d tw %d th %d kty %d\n", 
        viewport_scale, sw, sh, drawable_w, drawable_h, tw, th, keyboard_top_y);
    if(resize_tulip) {
        // given the sw / sh, find a better H_RES/tw than what we have. 
        tulip_rect.w = (int)((float)sw / viewport_scale);
        tulip_rect.h = (int)((float)sh / viewport_scale); 
        H_RES = tulip_rect.w;
        V_RES = tulip_rect.h;
        H_RES_D = H_RES;
        V_RES_D = V_RES;

    } else {
        // just keep it
    }

    float w_ratio = (float)sw / (float)tulip_rect.w;
    float h_ratio = (float)sh / (float)tulip_rect.h;
    if(w_ratio > h_ratio) {
        w_ratio = h_ratio;
    } else {
        h_ratio = w_ratio;
    }
    viewport.w = (int)((float)tulip_rect.w * w_ratio);
    viewport.h = (int)((float)tulip_rect.h * h_ratio);
    viewport.x = (sw - viewport.w) / 2;
    return 1; // ok
}


void init_window() {
#ifdef __TULIP_IOS__
    window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, tulip_rect.w, tulip_rect.h,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
#else
    window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, tulip_rect.w, tulip_rect.h,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#endif
    if (window == NULL) {
        fprintf(stderr,"Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        window_surface = SDL_GetWindowSurface(window);
        fixed_fps_renderer = SDL_CreateSoftwareRenderer( window_surface);
        fprintf(stderr, "setting viewport to x=%d y=%d w=%d h=%d and tulip to w=%d h=%d\n", viewport.x, viewport.y, viewport.w, viewport.h, tulip_rect.w, tulip_rect.h);
        framebuffer= SDL_CreateTexture(fixed_fps_renderer,SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, tulip_rect.w,tulip_rect.h);
    }
    // If this is not set it prevents sleep on a mac (at least)
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    SDL_SetWindowTitle(window, "Tulip Desktop");
    SDL_StartTextInput();

}



void destroy_window() {
    fprintf(stderr, "destroy_window1\n");
    free_caps(frame_bb);
    fprintf(stderr, "destroy_window2\n");
    SDL_DestroyWindow(window);
    fprintf(stderr, "destroy_window3\n");

    SDL_Quit();    
}

uint16_t last_held_joy_mask = 0;

void update_joy(SDL_Event e) {
    if(e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP) {
        last_held_joy_mask = 0;
        for(uint8_t b=0;b<SDL_CONTROLLER_BUTTON_MAX;b++) {
            if(SDL_GameControllerGetButton(gp, b)) {
                if(b == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) last_held_joy_mask |= 2;
                if(b == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) last_held_joy_mask |= 4;
                if(b == SDL_CONTROLLER_BUTTON_X) last_held_joy_mask |= 8;
                if(b == SDL_CONTROLLER_BUTTON_A) last_held_joy_mask |= 16;
                if(b == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) last_held_joy_mask |= 32;
                if(b == SDL_CONTROLLER_BUTTON_DPAD_LEFT) last_held_joy_mask |= 64;
                if(b == SDL_CONTROLLER_BUTTON_DPAD_DOWN) last_held_joy_mask |= 128;
                if(b == SDL_CONTROLLER_BUTTON_DPAD_UP) last_held_joy_mask |= 256;
                if(b == SDL_CONTROLLER_BUTTON_START) last_held_joy_mask |= 512;
                if(b == SDL_CONTROLLER_BUTTON_BACK) last_held_joy_mask |= 1024;
                if(b == SDL_CONTROLLER_BUTTON_Y) last_held_joy_mask |= 2048;
                if(b == SDL_CONTROLLER_BUTTON_B) last_held_joy_mask |= 4096;
            }
        }
    }    
}

uint16_t check_joy() {
    return last_held_joy_mask;
}

uint8_t store_mod = 0;

// it looks like from the perspective of tulip on iOS only ' and " come out as textinput only. 
// so we can use keydown always (storing the shift when we get it) 225/229
// and then only accept unicode from textinput , ' '
void check_key() {
#ifndef MONITOR_APPLE 
    SDL_Event e;
    uint8_t was_touch = 0;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            // TODO - not sure what to do here for iOS
            unix_display_flag = -1; // tell main to quit
        } else if(e.type == SDL_TEXTINPUT) {
            #ifdef __TULIP_IOS__
                //fprintf(stderr, "textinput %d %d %d\n", e.text.text[0], e.text.text[1], e.text.text[2]);
                if(e.text.text[0]==-30) { // utf8, either ' or "
                    if(e.text.text[2]==-99 || e.text.text[2]==-100) { // double quote
                        send_key_to_micropython(34);
                    }
                    if(e.text.text[2]==-103 || e.text.text[2]==-104) { // single quote 
                        send_key_to_micropython(39);
                    }
                }
            #endif
        } else if(e.type == SDL_KEYDOWN) {
            last_held_mod = SDL_GetModState();
            SDL_KeyboardEvent key = e.key; 
            //fprintf(stderr, "keydown %d %d = %d\n", key.keysym.scancode, last_held_mod, scan_ascii(key.keysym.scancode, (uint32_t)last_held_mod));
            if(key.keysym.scancode == 225 || key.keysym.scancode == 229) {
                #ifdef __TULIP_IOS__
                    store_mod = store_mod | KMOD_LSHIFT;
                #endif
            } else if(key.keysym.scancode >= 0x04 && key.keysym.scancode <= 0x94) {
                send_key_to_micropython(scan_ascii(key.keysym.scancode, (uint32_t)(last_held_mod | store_mod)));
                store_mod = 0;
            }
            uint8_t skip = 0;
            uint8_t pos = 10;
            for(uint8_t i=2;i<8;i++) {
                if(last_scan[i] == key.keysym.scancode) { skip = 1; }
                if(pos == 10 && last_scan[i] == 0) { pos = i; }
            }
            if(!skip && pos < 8) {
                last_scan[pos] = key.keysym.scancode;
            }
        } else if( e.type == SDL_WINDOWEVENT ) {
            //Window resize/orientation change
            #ifdef __TULIP_IOS__
                int kby = get_keyboard_y();
                if(kby != keyboard_top_y) {
                    keyboard_top_y = kby;
                    fprintf(stderr, "keyboard y has changed. now %d\n", keyboard_top_y);
                    unix_display_flag = -2;
                }
            #endif
            if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED) {
                fprintf(stderr, "window size changed to %d %d\n", e.window.data1, e.window.data2);
                if(e.window.data1 != drawable_w || e.window.data2 != drawable_h) {
                    fprintf(stderr, "different than existing %d %d\n", drawable_w, drawable_h);
                    drawable_w = e.window.data1;
                    drawable_h = e.window.data2;
                    // restart display
                    unix_display_flag = -2;
                }
            }
        } else if(e.type == SDL_KEYUP) {
            SDL_KeyboardEvent key = e.key; 
            for(uint8_t i=2;i<8;i++) {
                if(key.keysym.scancode == last_scan[i]) {
                    last_scan[i] = 0;
                }
            }
        } 
        // Deal with the button bar in iOS
        #ifdef __TULIP_IOS__
            else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                if(button_bar.w > 0) {
                    if(e.button.x >= button_bar.x/viewport_scale && e.button.y >= button_bar.y/viewport_scale && e.button.x < button_bar.x/viewport_scale+button_bar.w/viewport_scale && e.button.y < button_bar.y/viewport_scale + button_bar.h/viewport_scale) {
                        uint16_t button_x = e.button.x - button_bar.x;
                        button_x = button_x * viewport_scale;
                        if(e.type == SDL_MOUSEBUTTONUP) {
                            fprintf(stderr, "button x is %d\n", button_x);
                            if(button_x < btn_ctrl.x + btn_ctrl.w) { 
                                store_mod = store_mod | KMOD_LCTRL;
                            } else if(button_x < btn_tab.x + btn_tab.w) { 
                                send_key_to_micropython(9);
                            } else if(button_x < btn_esc.x + btn_esc.w) { 
                                send_key_to_micropython(27); 
                            } else if(button_x < btn_l.x + btn_l.w) { 
                                send_key_to_micropython(260);
                            } else if(button_x < btn_u.x + btn_u.w) { 
                                send_key_to_micropython(259);                  
                            } else if(button_x < btn_r.x + btn_r.w) { 
                                send_key_to_micropython(261);
                            } else if(button_x < btn_d.x + btn_d.w) { 
                                send_key_to_micropython(258);
                            }
                        }
                    }
                }
            }
        #endif
        int x,y;
        uint32_t button = SDL_GetMouseState(&x, &y);
        if(button) {
            last_touch_x[0] = (int16_t)x-(int16_t)(viewport.x/viewport_scale);
            last_touch_y[0] = (int16_t)y-(int16_t)(viewport.y/viewport_scale);
            was_touch = 1;
        } else { // release
            last_touch_x[0] = (int16_t)x-(int16_t)(viewport.x/viewport_scale);
            last_touch_y[0] = (int16_t)y-(int16_t)(viewport.y/viewport_scale);
            was_touch = 2;
        }
        update_joy(e);
    }
    if(was_touch) {
        send_touch_to_micropython(last_touch_x[0], last_touch_y[0], was_touch-1);
    }
#endif
}

void draw_button_bar() {
    if(button_bar.w > 0) {
        SDL_SetRenderDrawColor(fixed_fps_renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(fixed_fps_renderer, &button_bar);
    }
}

int unix_display_draw() {
    frame_ticks = get_ticks_ms();
    check_key();
    uint8_t *pixels;
    int pitch;
    SDL_LockTexture(framebuffer, NULL, (void**)&pixels, &pitch);

    // bounce the entire screen at once to the 332 color framebuffer
    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        if(y+FONT_HEIGHT <= V_RES) {
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT, NULL);
            for (uint16_t row=0;row<FONT_HEIGHT;row++) {
                for(uint16_t x=0;x<H_RES;x++) {
                    pixels[((y+row)*pitch)+x] = frame_bb[H_RES*row + x];
                }
            }
        }
    }

    SDL_UnlockTexture(framebuffer);

    // Copy the framebuffer (and stretch if needed into the renderer)
    SDL_RenderCopy(fixed_fps_renderer, framebuffer, &tulip_rect, &viewport);

    // If iOS, draw the button bar
    // TODO -- every time, or just once?
    SDL_RenderPresent(fixed_fps_renderer);

    // Clean up and show
    SDL_UpdateWindowSurface(window);

    display_frame_done_generic();

    int64_t ticks_per_frame_ms = (int64_t) (1000.0 / reported_fps);

    //If frame finished early according to our FPS clock, pause a bit (still processing keys) until it's time
    while(get_ticks_ms() - frame_ticks < ticks_per_frame_ms) {
        SDL_Delay(1);
        check_key();
    }

    // Are we restarting the display for a mode change, or quitting
    if(unix_display_flag < 0) {
        fprintf(stderr, "shutting down because of flag %d\n", unix_display_flag);
        destroy_window();
        display_teardown();
        if(unix_display_flag==-2) {
            unix_display_flag = 0;
            return -2;
        } else {
            unix_display_flag = 0;
            return -1;
        }
    }    
    return 1;
}

void unix_display_init() {
    // on iOS we need to get the display size before computing display sizes
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr,"SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } 
    #ifdef __TULIP_IOS__
        fprintf(stderr, "computing viewport\n");
        if(!compute_viewport(H_RES,V_RES,1)) {
            // problem

        }
        if(button_bar.w > 0) {
            btn_ctrl.x = 0;
            btn_ctrl.y = button_bar.y;
            btn_ctrl.w = 150;
            btn_ctrl.h = button_bar.h;

            btn_tab.x = 150;
            btn_tab.y = button_bar.y;
            btn_tab.w = 150;
            btn_tab.h = button_bar.h;

            btn_esc.x = 300;
            btn_esc.y = button_bar.y;
            btn_esc.w = 150;
            btn_esc.h = button_bar.h;

            btn_l.x = 450;
            btn_l.y = button_bar.y;
            btn_l.w = 150;
            btn_l.h = button_bar.h;

            btn_u.x = 600;
            btn_u.y = button_bar.y;
            btn_u.w = 150;
            btn_u.h = button_bar.h;


            btn_r.x = 750;
            btn_r.y = button_bar.y;
            btn_r.w = 150;
            btn_r.h = button_bar.h;

            btn_d.x = 900;
            btn_d.y = button_bar.y;
            btn_d.w = 150;
            btn_d.h = button_bar.h;
        }
        fprintf(stderr, "computed viewport\n");

    #else
        compute_viewport(H_RES,V_RES,0);
    #endif        

    fprintf(stderr, "unix_display1\n");

    display_init();
    fprintf(stderr, "unix_display2\n");
    unix_set_fps_from_parameters();
    fprintf(stderr, "unix_display3\n");

    init_window(); 
    fprintf(stderr, "unix_display4\n");

    #ifdef __TULIP_IOS__
        draw_button_bar();
    #endif
    fprintf(stderr, "unix_display5\n");


    frame_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    gp = SDL_GameControllerOpen(0);
    if(!gp) {
        fprintf(stderr, "No gamepad detected. That's ok\n");
    } else {
        fprintf(stderr, "Gamepad detected\n");
    }
}
