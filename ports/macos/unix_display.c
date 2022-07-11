// GPU driver via SDL2 FB 
#include <SDL.h>
#include "polyfills.h"
#include "display.h"
#include "keyscan.h"
SDL_Window *window;
SDL_Surface *window_surface;
SDL_Surface *surface_332;
SDL_Renderer *fixed_fps_renderer;
uint8_t *pixels_332;
uint8_t *frame_bb;
#define BYTES_PER_PIXEL 1
int64_t frame_ticks = 0;
uint8_t unix_display_restart_flag = 0;

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
                    fprintf(stderr, "Returning %2.2f FPS for res %d %d clock %d\n", fps_map[fps_i], H_RES, V_RES, PIXEL_CLOCK_MHZ);
                    reported_fps = fps_map[fps_i];
                    // get out of search 
                    clock_i = 10; v_res_i = 10; h_res_i = 10; 
                }
                fps_i++;
            }
        }
    }
    if(fps_i == 20) {
        fprintf(stderr, "Problem: could not find matching fps from res %d, %d clock %d. returning 60fps.\n", H_RES, V_RES, PIXEL_CLOCK_MHZ);
        reported_fps = 60;
    }


}

void unix_display_set_clock(uint8_t mhz) {  
    PIXEL_CLOCK_MHZ = mhz;
    unix_set_fps_from_parameters();
}


void unix_display_timings(uint32_t t0, uint32_t t1, uint32_t t2, uint32_t t3) {
    fprintf(stderr, "Stopping display task\n");
    H_RES = t0;
    V_RES = t1; 
    OFFSCREEN_X_PX = t2; 
    OFFSCREEN_Y_PX = t3; 
    unix_display_restart_flag = 1;
}


void init_window(uint16_t w, uint16_t h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr,"SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    }
    if (window == NULL) {
        fprintf(stderr,"Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        window_surface = SDL_GetWindowSurface(window);
        surface_332 = SDL_ConvertSurfaceFormat(window_surface, SDL_PIXELFORMAT_RGB332, 0);
        fixed_fps_renderer = SDL_CreateSoftwareRenderer( window_surface);
        pixels_332 = (uint8_t*) surface_332->pixels;
    }
    memset(pixels_332, 0, H_RES * V_RES);

    SDL_SetWindowTitle(window, "Tulip Desktop");
    SDL_StartTextInput();
}

void destrow_window() {
    SDL_DestroyWindow(window);
    SDL_Quit();    
}

SDL_Keymod last_held_mod;
int check_key() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {

        if (e.type == SDL_QUIT) {
            fprintf(stderr, "quit detected\n");
            return -1;
        }
        if(e.type == SDL_KEYDOWN) {
            last_held_mod = SDL_GetModState();
            SDL_KeyboardEvent key = e.key; 
            if(key.keysym.scancode >= 0x04 && key.keysym.scancode <= 0x94) {
                send_key_to_micropython(scan_ascii(key.keysym.scancode, (uint32_t)last_held_mod));
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
            return 1;
        }
        if(e.type == SDL_KEYUP) {
            SDL_KeyboardEvent key = e.key; 
            for(uint8_t i=2;i<8;i++) {
                if(key.keysym.scancode == last_scan[i]) {
                    last_scan[i] = 0;
                }
            }
        }
        int x,y;
        uint32_t button = SDL_GetMouseState(&x, &y);
        if(button) {
            last_touch_x[0] = (int16_t)x;
            last_touch_y[0] = (int16_t)y;
        }
    }
    return 0;
}


void start_draw() { 
    SDL_LockSurface(surface_332); 

}

void end_draw() {
    SDL_UnlockSurface(surface_332);

    // Using our non-vsync software renderer, blit the surface into a texture and then out to the window
    SDL_Texture *blit_texture = SDL_CreateTextureFromSurface(fixed_fps_renderer, surface_332);
    SDL_RenderCopy(fixed_fps_renderer, blit_texture, NULL, NULL);
    SDL_RenderPresent(fixed_fps_renderer);

    // Clean up and show
    SDL_DestroyTexture(blit_texture);
    SDL_UpdateWindowSurface(window);

    int64_t ticks_per_frame_ms = (int64_t) (1000.0 / reported_fps);
    //If frame finished early according to our FPS clock, pause a bit until it's time
    if( get_ticks_ms()-frame_ticks < ticks_per_frame_ms ) {
        SDL_Delay( ticks_per_frame_ms - (get_ticks_ms()-frame_ticks) );
    }

}

int unix_display_draw() {
    // Start a timer here to count FPS
    frame_ticks = get_ticks_ms();
    int in_ch = check_key();
    if(in_ch >= 0) { // if not quit
        start_draw();
        uint32_t c = 0;
        for(uint16_t y=0;y<V_RES;y=y+FONT_HEIGHT) {
            display_bounce_empty(frame_bb, y*H_RES, H_RES*FONT_HEIGHT*BYTES_PER_PIXEL, NULL);
            for(uint16_t x=0;x<FONT_HEIGHT*H_RES*BYTES_PER_PIXEL;x=x+BYTES_PER_PIXEL) {
                pixels_332[c++] = frame_bb[x];
            }
        }
        end_draw();
        display_frame_done_generic();

        // Are we restarting the display for a mode change? 
        if(unix_display_restart_flag) {
            unix_display_restart_flag = 0;
            SDL_DestroyWindow(window);
            SDL_Quit();
            display_teardown();
            return -2;
        } 

        return 1; // fine
    }
    return -1; // quit
}
void unix_display_init() {
    display_init();
    unix_set_fps_from_parameters();
    init_window(H_RES,V_RES); 
    frame_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    fprintf(stderr, "display init done!!!\n");
}
