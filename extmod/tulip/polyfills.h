#ifndef POLYFILLS_H

#include <stdint.h>
#include "py/mphal.h"

#ifdef MICROPY_ESP_IDF_4

// not sure, maybe esp includes?


#else
#include <sys/time.h>
#include <stdlib.h>
#define MALLOC_CAP_INTERNAL 0
#define MALLOC_CAP_SPIRAM 0
#define MALLOC_CAP_8BIT 0 
#endif



void take_semaphore();
int64_t get_time_ms();
int64_t get_time_us();
void * malloc_caps(uint32_t size, uint32_t flags);
void *calloc_caps(uint32_t align, uint32_t count, uint32_t size, uint32_t flags);
void *realloc_caps(void* ptr, uint32_t size, uint32_t caps);
void free_caps(void *ptr);
float compute_cpu_usage(uint8_t debug);

#endif