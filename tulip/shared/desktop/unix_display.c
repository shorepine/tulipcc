// GPU driver via SDL2 FB 
#include <SDL.h>
#include "polyfills.h"
#include "display.h"
#include "keyscan.h"
#include "ui.h"
#include "lvgl.h"
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Renderer *default_renderer;
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


void show_frame(void*d);
int unix_display_draw();
void check_key();
void destroy_window();
void unix_display_init();

#ifdef __EMSCRIPTEN__
    uint8_t display_size_changed = 0;
    #define SDL_WINDOW_NAME "Tulip Web"
#else
    #define SDL_WINDOW_NAME "Tulip Desktop"
#endif    


// LVGL/SDL connectors for keyboard here

#define KEYBOARD_BUFFER_SIZE SDL_TEXTINPUTEVENT_TEXT_SIZE

static char lvgl_kb_buf[KEYBOARD_BUFFER_SIZE];


void lvgl_keyboard_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    (void) indev_drv;      /*Unused*/

    static bool dummy_read = false;
    const size_t len = strlen(lvgl_kb_buf);

    /*Send a release manually*/
    if (dummy_read) {
        dummy_read = false;
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = len > 0;
    }
        /*Send the pressed character*/
    else if (len > 0) {
        dummy_read = true;
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = lvgl_kb_buf[0];
        memmove(lvgl_kb_buf, lvgl_kb_buf + 1, len);
        data->continue_reading = true;
    }
}


void force_rescale() {
    int rw, rh;
    SDL_GetRendererOutputSize(default_renderer, &rw, &rh);
    //fprintf(stderr, "renderer output size %d %d\n", rw,rh);
    float widthScale = (float)rw / (float) tulip_rect.w;
    float heightScale = (float)rh / (float) tulip_rect.h;
    SDL_RenderSetScale(default_renderer, widthScale, heightScale);
}



/**
 * Convert a SDL key code to it's LV_KEY_* counterpart or return '\0' if it's not a control character.
 * @param sdl_key the key code
 * @return LV_KEY_* control character or '\0'
 */
uint32_t sdl_keycode_to_ctrl_key(SDL_Keycode sdl_key)
{
    /*Remap some key to LV_KEY_... to manage groups*/
    
    SDL_Keymod mode = SDL_GetModState();
    
    switch(sdl_key) {
        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            return LV_KEY_RIGHT;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            return LV_KEY_LEFT;

        case SDLK_UP:
            return LV_KEY_UP;

        case SDLK_DOWN:
            return LV_KEY_DOWN;

        case SDLK_ESCAPE:
            return LV_KEY_ESC;

        case SDLK_BACKSPACE:
            return LV_KEY_BACKSPACE;

        case SDLK_DELETE:
            return LV_KEY_DEL;

        case SDLK_KP_ENTER:
        case '\r':
            return LV_KEY_ENTER;

        case SDLK_TAB:
            return (mode & KMOD_SHIFT)? LV_KEY_PREV: LV_KEY_NEXT;
            
        case SDLK_PAGEDOWN:
            return LV_KEY_NEXT;

        case SDLK_PAGEUP:
            return LV_KEY_PREV;

        default:
            return '\0';
    }
}


void unix_display_set_clock(uint8_t mhz) {  
    PIXEL_CLOCK_MHZ = mhz;
    reported_fps = TARGET_DESKTOP_FPS;
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

    tulip_rect.x = 0; 
    tulip_rect.y = 0; 
    tulip_rect.w = tw; 
    tulip_rect.h = th; 
    // Adjust y for bezels
    viewport.y = 0;
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
    //fprintf(stderr, "viewport is %d,%d,%d,%d\n", viewport.x, viewport.y, viewport.w, viewport.h);
    return 1; // ok
}


int unix_display_draw() {
    check_key();

    frame_ticks = get_ticks_ms();


    

    uint8_t *pixels;
    int pitch;
    SDL_LockTexture(framebuffer, NULL, (void**)&pixels, &pitch);
    // bounce the entire screen at once to the ARGB8888 color framebuffer
    for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
        if(y+FONT_HEIGHT <= V_RES) {
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT, NULL);
            for (uint16_t row=0;row<FONT_HEIGHT;row++) {
                for(uint16_t x=0;x<H_RES*4;x=x+4) {
                    uint8_t r,g,b;
                    unpack_rgb_332_repeat(frame_bb[H_RES*row + (x/4)], &r, &g, &b);
                    pixels[((y+row)*pitch)+x+0] = b;
                    pixels[((y+row)*pitch)+x+1] = g;
                    pixels[((y+row)*pitch)+x+2] = r;
                    pixels[((y+row)*pitch)+x+3] = 0;
                }
            }
        }
    }


    // Copy the framebuffer (and stretch if needed into the renderer)
    SDL_RenderCopy(default_renderer, framebuffer, &tulip_rect, &viewport);

    SDL_UnlockTexture(framebuffer);

#ifndef __EMSCRIPTEN__
    SDL_RenderPresent(default_renderer);
#endif

    // Clean up and show
    SDL_UpdateWindowSurface(window);

    display_frame_done_generic();

    // Are we restarting the display for a mode change, or quitting
    if(unix_display_flag < 0) {
        destroy_window();
        #ifndef __EMSCRIPTEN__
        display_teardown();
        #endif

        if(unix_display_flag == -2){
            unix_display_flag = 0;
            unix_display_init();
        }

    }    



    return get_ticks_ms() - frame_ticks;
}

void show_frame(void*d) {
    unix_display_draw();
}

#ifdef __EMSCRIPTEN__
#include "emscripten/html5.h"
static EM_BOOL on_web_display_size_changed( int event_type, 
    const EmscriptenUiEvent *event, void *user_data ) {
    display_size_changed = 1;  // custom global flag
    return 0;
}
#endif

void init_window() {
#ifdef __EMSCRIPTEN__
    // We keep a hidden textinput on the page to capture keypresses for mobile devices & web
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#textinput");
    // This sets the scale to nearest neighbor -- "0", the default, makes our pixel font very jaggy
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
#endif
    window = SDL_CreateWindow(SDL_WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, tulip_rect.w, tulip_rect.h,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        fprintf(stderr,"Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        default_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
        framebuffer= SDL_CreateTexture(default_renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tulip_rect.w,tulip_rect.h);
        #ifndef __EMSCRIPTEN__
        force_rescale();
        #endif
    }
    // If this is not set it prevents sleep on a mac (at least)
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");
    SDL_SetWindowTitle(window, SDL_WINDOW_NAME);

#ifdef __EMSCRIPTEN__ // Tulip web deskop
    emscripten_set_resize_callback(
        EMSCRIPTEN_EVENT_TARGET_WINDOW,
        0, 0, on_web_display_size_changed
    );

#endif
}



void destroy_window() {
    free_caps(frame_bb);
    SDL_DestroyTexture(framebuffer);
    SDL_DestroyRenderer(default_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();    
    sdl_ready = 0;
}


uint8_t store_mod = 0;

void check_key() {
#ifndef MONITOR_APPLE 
    SDL_Event e;
    uint8_t was_touch = 0;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            unix_display_flag = -1; // tell main to quit
        } else if(e.type == SDL_TEXTINPUT) {
            // lvgl stuff
           const size_t len = strlen(lvgl_kb_buf) + strlen(e.text.text);
           if (len < KEYBOARD_BUFFER_SIZE - 1)
                strcat(lvgl_kb_buf, e.text.text);

            // In SDL all non ascii stuff only comes in through textinput                    
            uint8_t start = 0;
            while(e.text.text[start] != 0) {
                send_key_to_micropython((uint8_t) e.text.text[start] & 0xff);
                start++;
            }
            
        } else if(e.type == SDL_KEYDOWN) {
            // do LVGL stuff first
            const uint32_t ctrl_key = sdl_keycode_to_ctrl_key(e.key.keysym.sym);
            if (ctrl_key == '\0') {
                // do nothing?
            } else {
                const size_t len = strlen(lvgl_kb_buf);
                if (len < KEYBOARD_BUFFER_SIZE - 1) {
                    lvgl_kb_buf[len] = ctrl_key;
                    lvgl_kb_buf[len + 1] = '\0';
                }
            }


            last_held_mod = SDL_GetModState();
            SDL_KeyboardEvent key = e.key; 
            if(key.keysym.scancode == 225 || key.keysym.scancode == 229) {
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

            if( e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_RESIZED) {
                if(kby != keyboard_top_y || (e.window.data1 != drawable_w || e.window.data2 != drawable_h)) {
                    keyboard_top_y = kby;
                    drawable_w = e.window.data1;
                    drawable_h = e.window.data2;
                    #ifndef __EMSCRIPTEN__
                    unix_display_flag = -2;
                    #else
                    force_rescale();
                    #endif
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
        int x,y;
        uint32_t button = SDL_GetMouseState(&x, &y);
        #ifdef __EMSCRIPTEN__
        float newx, newy;
        SDL_RenderWindowToLogical(default_renderer, x,y , &newx, &newy);
        //if(button) fprintf(stderr, "x,y was %d,%d is now %d,%d vs is %f\n", x,y, (int)newx, (int)newy, viewport_scale);
        x = (int16_t) newx;
        y = (int16_t) newy;
        #endif
        if(button) {
            //fprintf(stderr, "button is at %d,%d. vp is %d,%d. scale is %f\n", x,y, viewport.x, viewport.y, viewport_scale);
            last_touch_x[0] = (int16_t)x-(int16_t)(viewport.x/viewport_scale);
            last_touch_y[0] = (int16_t)y-(int16_t)(viewport.y/viewport_scale);
            was_touch = 1;
        } else { // release
            last_touch_x[0] = (int16_t)x-(int16_t)(viewport.x/viewport_scale);
            last_touch_y[0] = (int16_t)y-(int16_t)(viewport.y/viewport_scale);
            was_touch = 2;
        }
    }
    if(was_touch) {
        send_touch_to_micropython(last_touch_x[0], last_touch_y[0], was_touch-1);
    }
#endif
}



void unix_display_init() {
    // on iOS we need to get the display size before computing display sizes
    if(!sdl_ready) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr,"SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        } 
        SDL_StartTextInput();

        sdl_ready = 1;
    }  else {
        fprintf(stderr, "sdl already ready\n");
    }

    compute_viewport(H_RES,V_RES,0);
   
    display_init();

    init_window(); 

    for(uint8_t i=0;i<MAX_KEY_REMAPS;i++) {
        key_remaps[i].scan = 0;
        key_remaps[i].mod = 0;
        key_remaps[i].code = 0;
    }


    SDL_StartTextInput();

    frame_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    SDL_StartTextInput();



}


