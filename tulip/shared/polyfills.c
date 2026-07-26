#include "polyfills.h"

// time
// malloc/calloc
// free(?)
// thread / xtaskCreate 

#ifdef AMY_IS_EXTERNAL
void * malloc_caps(uint32_t size, uint32_t flags) {
#ifdef ESP_PLATFORM
    //fprintf(stderr, "allocing size %ld flags %ld\n", size, flags);
    return heap_caps_malloc(size, flags);
#else
    // ignore flags
    return malloc(size);
#endif
}
#endif


#ifndef ESP_PLATFORM

void display_brightness(uint8_t amount) {
}

float compute_cpu_usage(uint8_t debug) {
    return 0;
}
#endif



void display_start() {
#ifdef ESP_PLATFORM
    esp32s3_display_start();
#endif
}


void display_stop() {
#ifdef ESP_PLATFORM
    esp32s3_display_stop();
#endif
}


uint8_t rand_uint8() {
#ifdef ESP_PLATFORM
    return rand() % 255; // i thought esp_random() would be faster but it's not 
#else
    return rand() % 255;
#endif
}

void delay_ms(uint32_t ms) {
#ifdef ESP_PLATFORM
    vTaskDelay(ms / portTICK_PERIOD_MS);
#else
    #ifndef __EMSCRIPTEN__
    usleep(ms * 1000);
    #else
    // nothing
    #endif
#endif
}


void take_semaphore() {
#ifdef ESP_PLATFORM
    ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(100));
#else
    // nothing? not sure yet
#endif
}


// Monotonic microseconds since startup, 64-bit. Not mp_hal_ticks_us(): that is
// mp_uint_t, i.e. 32 bits here, so it wraps every ~71 minutes.
int64_t get_time_us() {
#ifdef ESP_PLATFORM
    return esp_timer_get_time();  // already counts from chip boot
#else
    // CLOCK_MONOTONIC counts from machine boot, which can be weeks. Zero-base
    // it on first call so desktop and device agree on "microseconds since
    // Tulip started" and the numbers stay small.
    static int64_t base_us = -1;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int64_t now = (int64_t)ts.tv_sec * 1000000LL + ts.tv_nsec / 1000LL;
    if (base_us < 0) base_us = now;
    return now - base_us;
#endif
}

extern int32_t get_sysclock();

// Monotonic milliseconds since boot, 64-bit -- does not wrap. Anything that
// keeps an absolute timestamp rather than a short interval must use this;
// tulip.ticks_ms() hands it straight to Python.
int64_t get_time_ms() {
    return get_time_us() / 1000;
}

// 32-bit millisecond tick. Rolls over every 2^32 ms (49.7 days) by design: its
// only consumers are the LVGL tick shim and the desktop frame timer, which
// subtract these values modulo 2^32 -- correct across the rollover.
// This used to be declared int32_t, which made it go *negative* after 2^31 ms
// (24.9 days) and fed negative numbers to Python via tulip.ticks_ms().
uint32_t get_ticks_ms() {
    return (uint32_t)get_time_ms();
}

void *calloc_caps(uint32_t align, uint32_t count, uint32_t size, uint32_t flags) {
#ifdef ESP_PLATFORM
    //if(flags & MALLOC_CAP_SPIRAM) fprintf(stderr, "spiram callocing count %ld size %ld flags %ld\n", count, size, flags);
    return heap_caps_aligned_calloc(align, count, size, flags); 
#else
    return (void*)malloc(size*count);
#endif
}

void *realloc_caps(void* ptr, uint32_t size, uint32_t caps) {
#ifdef ESP_PLATFORM
  //fprintf(stderr, "re-allocing size %ld flags %ld\n", size, caps);
  return heap_caps_realloc(ptr, size, caps);
#else
  return (void*)realloc(ptr, size);
#endif
}

void free_caps(void *ptr) {
#ifdef ESP_PLATFORM
    heap_caps_free(ptr);
#else
    free(ptr);
#endif
}
