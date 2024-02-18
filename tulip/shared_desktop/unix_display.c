// GPU driver via SDL2 FB 
#include <SDL.h>
#include "polyfills.h"
#include "display.h"
#include "keyscan.h"
#include "ui.h"
#include "lvgl.h"
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
uint8_t sdl_ready = 0;
SDL_Rect button_bar;
SDL_Rect btn_ctrl, btn_tab, btn_esc, btn_l, btn_r, btn_u, btn_d;

#ifdef __TULIP_IOS__
extern int get_keyboard_y();
extern uint8_t is_iphone();
extern uint8_t is_ipad();
extern void ios_draw_text(float x, float y, float w, float h, char *text) ;
#define BUTTON_BAR_TEXT "  ⌃    ⇥    ␛    ◁    △    ▷    ▽"
#endif

void show_frame(void*d);
int unix_display_draw();
void check_key();
void destroy_window();
void unix_display_init();



void unix_display_set_clock(uint8_t mhz) {  
    PIXEL_CLOCK_MHZ = mhz;
    reported_fps = 30;
}


void unix_display_timings(uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3) {
    unix_display_flag = -2; // restart display with new timings
}

void screen_size(int *w, int *h, float *scale);

// Compute the viewport and optionally new optimal Tulip size for this display 
int8_t compute_viewport(uint16_t tw, uint16_t th, int8_t resize_tulip) {
    int sw = tw;
    int sh = th;
    viewport_scale = 1;
    #ifdef __TULIP_IOS__
        screen_size(&drawable_w, &drawable_h, &viewport_scale);
        sw = drawable_w * viewport_scale;
        sh = drawable_h * viewport_scale;
    #endif

    tulip_rect.x = 0; 
    tulip_rect.y = 0; 
    tulip_rect.w = tw; 
    tulip_rect.h = th; 
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
        } else {
            button_bar.w = 0;
            button_bar.h = 0;
        }
        sh = sh - keyboard_top_y - button_bar.h; // leave room for the keyboard
    #endif
    if(resize_tulip) {
        // given the sw / sh, find a better H_RES/tw than what we have. 
        tulip_rect.w = (int)((float)sw / viewport_scale);
        tulip_rect.h = (int)((float)sh / viewport_scale); 
        //H_RES = tulip_rect.w;
        //V_RES = tulip_rect.h;
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


int unix_display_draw() {
    check_key();
    lv_task_handler();

    frame_ticks = get_ticks_ms();



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


    // Copy the framebuffer (and stretch if needed into the renderer)
    SDL_RenderCopy(fixed_fps_renderer, framebuffer, &tulip_rect, &viewport);

    SDL_UnlockTexture(framebuffer);

    // If iOS, draw the button bar
    // TODO -- every time, or just once?
    SDL_RenderPresent(fixed_fps_renderer);

    // Clean up and show
    SDL_UpdateWindowSurface(window);

    display_frame_done_generic();

    #ifndef __TULIP_IOS__
        int64_t ticks_per_frame_ms = (int64_t) (1000.0 / reported_fps);
        while(get_ticks_ms() - frame_ticks < ticks_per_frame_ms) {
            SDL_Delay(1);
            check_key();
        }
    #endif

    // Are we restarting the display for a mode change, or quitting
    if(unix_display_flag < 0) {
        destroy_window();
        display_teardown();

        if(unix_display_flag == -2){
            unix_display_flag = 0;
            unix_display_init();
        }

    }    
    return 1;
}

void show_frame(void*d) {
    unix_display_draw();
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
        framebuffer= SDL_CreateTexture(fixed_fps_renderer,SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, tulip_rect.w,tulip_rect.h);

        #ifdef __TULIP_IOS__
            //SDL_iPhoneSetAnimationCallback(window, 1, show_frame, NULL);
        #endif
    }
    // If this is not set it prevents sleep on a mac (at least)
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    SDL_SetWindowTitle(window, "Tulip Desktop");

}



void destroy_window() {
    free_caps(frame_bb);
    SDL_DestroyTexture(framebuffer);
    SDL_DestroyRenderer(fixed_fps_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();    
    sdl_ready = 0;
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
                if(e.text.text[0]==-30) { // utf8, either ' or "
                    if(e.text.text[2]==-99 || e.text.text[2]==-100) { // double quote
                        send_key_to_micropython(34);
                    }
                    if(e.text.text[2]==-103 || e.text.text[2]==-104) { // single quote 
                        send_key_to_micropython(39);
                    }
                }
            #endif

            // In SDL all non ascii stuff only comes in through textinput                    
            uint8_t start = 0;
            while(e.text.text[start] != 0) {
                send_key_to_micropython((uint8_t) e.text.text[start] & 0xff);
                start++;
            }
            
        } else if(e.type == SDL_KEYDOWN) {
            last_held_mod = SDL_GetModState();
            SDL_KeyboardEvent key = e.key; 
            if(key.keysym.scancode == 225 || key.keysym.scancode == 229) {
                #ifdef __TULIP_IOS__
                    store_mod = store_mod | KMOD_LSHIFT;
                #endif
            } else if(key.keysym.scancode >= 0x04 && key.keysym.scancode <= 0x94) {
                uint16_t ascii_key = scan_ascii(key.keysym.scancode, (uint32_t)(last_held_mod | store_mod));
                if(ascii_key < 32 || ascii_key > 255) {
                    send_key_to_micropython(ascii_key);
                    store_mod = 0;
                }
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
            int kby = 0;
            #ifdef __TULIP_IOS__
                kby = get_keyboard_y();
            #endif

            if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED) {
                if(kby != keyboard_top_y || (e.window.data1 != drawable_w || e.window.data2 != drawable_h)) {
                    keyboard_top_y = kby;
                    drawable_w = e.window.data1;
                    drawable_h = e.window.data2;
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
    #ifdef __TULIP_IOS__
        if(button_bar.w > 0) {
            SDL_SetRenderDrawColor(fixed_fps_renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(fixed_fps_renderer, &button_bar);
            ios_draw_text(0,button_bar.y/viewport_scale - 20,800,100,BUTTON_BAR_TEXT);
        }
    #endif
}


int HandleAppEvents(void *userdata, SDL_Event *event) {
    switch (event->type)
    {
    case SDL_APP_TERMINATING:
        /* Terminate the app.
           Shut everything down before returning from this function.
        */
        return 0;
    case SDL_APP_LOWMEMORY:
        /* You will get this when your app is paused and iOS wants more memory.
           Release as much memory as possible.
        */
        return 0;
    case SDL_APP_WILLENTERBACKGROUND:
        /* Prepare your app to go into the background.  Stop loops, etc.
           This gets called when the user hits the home button, or gets a call.
        */
        return 0;
    case SDL_APP_DIDENTERBACKGROUND:
        /* This will get called if the user accepted whatever sent your app to the background.
           If the user got a phone call and canceled it, you'll instead get an SDL_APP_DIDENTERFOREGROUND event and restart your loops.
           When you get this, you have 5 seconds to save all your state or the app will be terminated.
           Your app is NOT active at this point.
        */
        return 0;
    case SDL_APP_WILLENTERFOREGROUND:
        /* This call happens when your app is coming back to the foreground.
           Restore all your state here.
        */
        return 0;
    case SDL_APP_DIDENTERFOREGROUND:
        /* Restart your loops here.
           Your app is interactive and getting CPU again.
        */
        return 0;
    default:
        /* No special processing, add it to the event queue */
        return 1;
    }
}




void unix_display_init() {
    // on iOS we need to get the display size before computing display sizes
    if(!sdl_ready) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
            fprintf(stderr,"SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        } 
        SDL_SetEventFilter(HandleAppEvents, NULL);
        SDL_StartTextInput();

        sdl_ready = 1;
    }  else {
        fprintf(stderr, "sdl already ready\n");
    }

    #ifdef __TULIP_IOS__
        compute_viewport(H_RES,V_RES,1);
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

    #else
        compute_viewport(H_RES,V_RES,0);
    #endif        

    display_init();

    init_window(); 

    for(uint8_t i=0;i<MAX_KEY_REMAPS;i++) {
        key_remaps[i].scan = 0;
        key_remaps[i].mod = 0;
        key_remaps[i].code = 0;
    }


    SDL_StartTextInput();

    #ifdef __TULIP_IOS__
        draw_button_bar();
    #endif


    frame_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    gp = SDL_GameControllerOpen(0);
    if(!gp) {
        fprintf(stderr, "No gamepad detected. That's ok\n");
    } else {
        fprintf(stderr, "Gamepad detected\n");
    }
    SDL_StartTextInput();



}


