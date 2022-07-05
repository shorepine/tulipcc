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

void init_window(uint16_t w, uint16_t h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr,"SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, w, h,
                                SDL_WINDOW_SHOWN);

    }
    if (window == NULL) {
        fprintf(stderr,"Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
        // A non-vsynced renderer, to cap to usual Tulip CC frame rates
        //fixed_fps_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
        window_surface = SDL_GetWindowSurface(window);
        surface_332 = SDL_ConvertSurfaceFormat(window_surface, SDL_PIXELFORMAT_RGB332, 0);
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
    }
    return 0;
}


void start_draw() { 
    SDL_LockSurface(surface_332); 

}

void end_draw() {

    SDL_UnlockSurface(surface_332);
    SDL_BlitSurface(surface_332, NULL, window_surface, NULL);

    //Update screen
    //SDL_RenderPresent( fixed_fps_renderer );
    //If frame finished early
    //if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
    //    SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
    //}


    SDL_UpdateWindowSurface(window);
}

int unix_display_draw() {
    //Calculate and correct fps    
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
        return 1; // fine
    }
    return -1; // quit
}
void unix_display_init() {
    display_init();
    init_window(H_RES,V_RES); 
    frame_bb = (uint8_t *) malloc_caps(FONT_HEIGHT*H_RES*BYTES_PER_PIXEL,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    fprintf(stderr, "display init done\n");


}
