#include "polyfills.h"

// time
// malloc/calloc
// free(?)
// thread / xtaskCreate 



#ifndef ESP_PLATFORM

void display_brightness(uint8_t amount) {
}

float compute_cpu_usage(uint8_t debug) {
    return 0;
}
#endif


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
    usleep(ms * 1000);
#endif
}


void take_semaphore() {
#ifdef ESP_PLATFORM
    ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(100));
#else
    // nothing? not sure yet
#endif
}


int64_t get_time_us() {
    return mp_hal_ticks_us();
}

extern int64_t get_sysclock();
int64_t get_time_ms() {
    return mp_hal_ticks_ms(); // a large number
}
int64_t get_ticks_ms() {
    return amy_sysclock(); // based on audio driver
}
    
void * malloc_caps(uint32_t size, uint32_t flags) {
#ifdef ESP_PLATFORM
    return heap_caps_malloc(size, flags);
#else
    // ignore flags
    return malloc(size);
#endif
}

void *calloc_caps(uint32_t align, uint32_t count, uint32_t size, uint32_t flags) {
#ifdef ESP_PLATFORM
    return heap_caps_aligned_calloc(align, count, size, flags); 
#else
    return (void*)malloc(size*count);
#endif
}

void *realloc_caps(void* ptr, uint32_t size, uint32_t caps) {
#ifdef ESP_PLATFORM
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
