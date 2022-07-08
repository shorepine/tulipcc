/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * Development of the code in this file was sponsored by Microbric Pty Ltd
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_task.h"
#include "soc/cpu.h"
#include "esp_log.h"

#if CONFIG_IDF_TARGET_ESP32
#include "esp32/spiram.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/spiram.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/spiram.h"
#endif

#include "py/stackctrl.h"
#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/persistentcode.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mphal.h"
#include "shared/readline/readline.h"
#include "shared/runtime/pyexec.h"
#include "uart.h"
#include "usb.h"
#include "usb_serial_jtag.h"
#include "modmachine.h"
#include "modnetwork.h"
#include "mpthreadport.h"
#include "display.h"
#include "alles.h"
#include "touchscreen.h"
#include "tasks.h"
#if MICROPY_BLUETOOTH_NIMBLE
#include "extmod/modbluetooth.h"
#endif






// there's also mp_main_task_handle

// Set the margin for detecting stack overflow, depending on the CPU architecture.
#if CONFIG_IDF_TARGET_ESP32C3
#define MP_TASK_STACK_LIMIT_MARGIN (2048)
#else
#define MP_TASK_STACK_LIMIT_MARGIN (1024)
#endif

extern void usb_keyboard_start();

typedef void (*esp_alloc_failed_hook_t) (size_t size, uint32_t caps, const char * function_name);

void esp_alloc_failed(size_t size, uint32_t caps, const char *function_name) {
    printf("alloc failed size %d function %s caps: ", size, function_name);
    if(caps & MALLOC_CAP_SPIRAM) printf("spiram ");
    if(caps & MALLOC_CAP_INTERNAL) printf("internal ");
    if(caps & MALLOC_CAP_32BIT) printf("32bit ");
    if(caps & MALLOC_CAP_DEFAULT) printf("default ");
    if(caps & MALLOC_CAP_IRAM_8BIT) printf("iram8bit ");
    if(caps & MALLOC_CAP_RTCRAM) printf("rtcram ");
    if(caps & MALLOC_CAP_8BIT) printf("8bit ");
    if(caps & MALLOC_CAP_EXEC) printf("exec ");
    if(caps & MALLOC_CAP_DMA) printf("dma ");
    printf("\n");
}


/*
alles_task
disp_task
usb_task
Tmr Svc
sys_evt
ipc1
ipc0
esp_timer
fill_audio_buff
render_task0
mp_task
*/


float compute_cpu_usage(uint8_t debug) {
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x, i;
    const char* const tasks[] = {
         "render_task0", "render_task1", "esp_timer", "sys_evt", "Tmr Svc", "ipc0", "ipc1", "mp_task", "disp_task", 
         "usb_task", "alles_task", "main", "fill_audio_buff", "wifi", "idle0", "idle1", "touch_task", 0 
    }; 
    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );
    if(debug) {
        printf("%d tasks running now\n", uxArraySize);
        for(x=0; x<uxArraySize; x++) { // for each task
            printf("_%s_ ", pxTaskStatusArray[x].pcTaskName);
        }
        printf("\n");
    }
    
    unsigned long counter_since_last[MAX_TASKS];
    unsigned long ulTotalRunTime = 0;
    unsigned long freeTime = 0;
    TaskStatus_t xTaskDetails;
    // We have to check for the names we want to track
    for(i=0;i<MAX_TASKS;i++) { // for each name
        counter_since_last[i] = 0;
        for(x=0; x<uxArraySize; x++) { // for each task
            if(strcmp(pxTaskStatusArray[x].pcTaskName, tasks[i])==0) {
                counter_since_last[i] = pxTaskStatusArray[x].ulRunTimeCounter - last_task_counters[i];
                last_task_counters[i] = pxTaskStatusArray[x].ulRunTimeCounter;
                ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
            }
        }
        
        // Have to get these specially as the task manager calls them both "IDLE" and swaps their orderings around
        if(strcmp("idle0", tasks[i])==0) { 
            vTaskGetInfo(idle_task_0, &xTaskDetails, pdFALSE, eRunning);
            counter_since_last[i] = xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            freeTime = freeTime + xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            last_task_counters[i] = xTaskDetails.ulRunTimeCounter;
            ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
        }
        if(strcmp("idle1", tasks[i])==0) { 
            vTaskGetInfo(idle_task_1, &xTaskDetails, pdFALSE, eRunning);
            counter_since_last[i] = xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            freeTime = freeTime + xTaskDetails.ulRunTimeCounter - last_task_counters[i];
            last_task_counters[i] = xTaskDetails.ulRunTimeCounter;
            ulTotalRunTime = ulTotalRunTime + counter_since_last[i];
        }
        

    }
    if(debug) {
        printf("------ CPU usage since last call to tulip.cpu()\n");
        for(i=0;i<MAX_TASKS;i++) {
            printf("%-15s\t%-15ld\t\t%2.2f%%\n", tasks[i], counter_since_last[i], (float)counter_since_last[i]/ulTotalRunTime * 100.0);
        }   
    }
    vPortFree(pxTaskStatusArray);
    return 100.0 - ((float)freeTime/ulTotalRunTime * 100.0);

}

int vprintf_null(const char *format, va_list ap) {
    // do nothing: this is used as a log target during raw repl mode
    return 0;
}

void mp_task(void *pvParameter) {
    volatile uint32_t sp = (uint32_t)get_sp();
    #if MICROPY_PY_THREAD
    mp_thread_init(pxTaskGetStackStart(NULL), MP_TASK_STACK_SIZE / sizeof(uintptr_t));
    #endif
    #if CONFIG_USB_ENABLED
    usb_init();
    #elif CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG
    //usb_serial_jtag_init();
    #endif
    #if MICROPY_HW_ENABLE_UART_REPL
    uart_stdout_init();
    #endif
    machine_init();

    size_t mp_task_heap_size;
    void *mp_task_heap = NULL;

    #if CONFIG_SPIRAM_USE_MALLOC
    // SPIRAM is issued using MALLOC, fallback to normal allocation rules
    mp_task_heap = NULL;
    #elif CONFIG_ESP32_SPIRAM_SUPPORT
    // Try to use the entire external SPIRAM directly for the heap
    mp_task_heap = (void *)SOC_EXTRAM_DATA_LOW;
    switch (esp_spiram_get_chip_size()) {
        case ESP_SPIRAM_SIZE_16MBITS:
            mp_task_heap_size = 2 * 1024 * 1024;
            break;
        case ESP_SPIRAM_SIZE_32MBITS:
        case ESP_SPIRAM_SIZE_64MBITS:
            printf("64mbit ram\n");
            mp_task_heap_size = 4 * 1024 * 1024;
            break;
        default:
            // No SPIRAM, fallback to normal allocation
            mp_task_heap = NULL;
            break;
    }
    #elif CONFIG_ESP32S2_SPIRAM_SUPPORT || CONFIG_ESP32S3_SPIRAM_SUPPORT

    // Try to use the entire external SPIRAM directly for the heap
    size_t esp_spiram_size = esp_spiram_get_size();

    heap_caps_register_failed_alloc_callback(esp_alloc_failed);

    // save ram for screen -- currently a 1280x750 buffer, so 1.92MB
    // Save another 2MB for working buffers for PNG decoding?!? 
    esp_spiram_size = esp_spiram_size - (1024*1024*4);

    if (esp_spiram_size > 0) {
        mp_task_heap = (void *)SOC_EXTRAM_DATA_HIGH - esp_spiram_size;
        mp_task_heap_size = esp_spiram_size;
    }
    #endif

    if (mp_task_heap == NULL) {

        // Allocate the uPy heap using malloc and get the largest available region,
        // limiting to 1/2 total available memory to leave memory for the OS.
        #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 1, 0)
        size_t heap_total = heap_caps_get_total_size(MALLOC_CAP_8BIT);
        #else
        multi_heap_info_t info;
        heap_caps_get_info(&info, MALLOC_CAP_8BIT);
        size_t heap_total = info.total_free_bytes + info.total_allocated_bytes;
        #endif
        mp_task_heap_size = MIN(heap_caps_get_largest_free_block(MALLOC_CAP_8BIT), heap_total / 2);
        mp_task_heap = malloc(mp_task_heap_size);
    }
soft_reset:
    // initialise the stack pointer for the main thread
    mp_stack_set_top((void *)sp);
    mp_stack_set_limit(MP_TASK_STACK_SIZE - MP_TASK_STACK_LIMIT_MARGIN);
    gc_init(mp_task_heap, mp_task_heap + mp_task_heap_size);
    mp_init();
    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
    readline_init0();

    // initialise peripherals
    machine_pins_init();

    #if MICROPY_PY_MACHINE_I2S
    machine_i2s_init0();
    #endif

    // run boot-up scripts
    pyexec_frozen_module("_boot.py");
    pyexec_file_if_exists("boot.py");
    if (pyexec_mode_kind == PYEXEC_MODE_FRIENDLY_REPL) {
        int ret = pyexec_file_if_exists("main.py");
        if (ret & PYEXEC_FORCED_EXIT) {
            goto soft_reset_exit;
        }
    }

    for (;;) {
        if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
            //vprintf_like_t vprintf_log = esp_log_set_vprintf(vprintf_null);
            if (pyexec_raw_repl() != 0) {
                break;
            }
            //esp_log_set_vprintf(vprintf_log);
        } else {
            if (pyexec_friendly_repl() != 0) {
                break;
            }
        }
    }

soft_reset_exit:

    #if MICROPY_BLUETOOTH_NIMBLE
    mp_bluetooth_deinit();
    #endif

    machine_timer_deinit_all();

    #if MICROPY_PY_THREAD
    mp_thread_deinit();
    #endif

    gc_sweep_all();

    mp_hal_stdout_tx_str("MPY: soft reboot\r\n");

    // deinitialise peripherals
    machine_pwm_deinit_all();
    // TODO: machine_rmt_deinit_all();
    machine_pins_deinit();
    machine_deinit();
    usocket_events_deinit();

    mp_deinit();
    fflush(stdout);
    goto soft_reset;
}


void boardctrl_startup(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

extern void ft5x06_init();
extern void ft5x06_test_task();
extern void setup_midi_in();

void app_main(void) {
    // Hook for a board to run code at start up.
    // This defaults to initialising NVS.
    MICROPY_BOARD_STARTUP();

    for(uint8_t i=0;i<MAX_TASKS;i++) last_task_counters[i] = 0;

    // Grab the idle tasks
    idle_task_0 = xTaskGetIdleTaskHandleForCPU(0);
    idle_task_1 = xTaskGetIdleTaskHandleForCPU(1);

        printf("Starting MIDI\n");
    setup_midi_in();

    printf("Starting USB keyboard host on core %d\n", USB_TASK_COREID);
    xTaskCreatePinnedToCore(usb_keyboard_start, "usb_task", (USB_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, DISPLAY_TASK_PRIORITY, &usb_main_task_handle, USB_TASK_COREID);

    printf("Starting display on core %d\n", DISPLAY_TASK_COREID);
    xTaskCreatePinnedToCore(esp32s3_display_run, "disp_task", (DISP_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, DISPLAY_TASK_PRIORITY, &display_main_task_handle, DISPLAY_TASK_COREID);

    printf("Init touchscreen on core %d \n", TOUCHSCREEN_TASK_COREID);
    ft5x06_init();
    xTaskCreatePinnedToCore(ft5x06_test_task, "touch_task", (TOUCHSCREEN_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, TOUCHSCREEN_TASK_PRIORITY, &touch_main_task_handle, TOUCHSCREEN_TASK_COREID);

    printf("Starting Alles on core %d (dual core)\n", ALLES_TASK_COREID);
    xTaskCreatePinnedToCore(alles_start, "alles_task", (ALLES_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, ALLES_TASK_PRIORITY, &alles_main_task_handle, ALLES_TASK_COREID);

    printf("Starting MicroPython on core %d\n", MP_TASK_COREID);
    // Force MP core to 1
    xTaskCreatePinnedToCore(mp_task, "mp_task", (MP_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, MP_TASK_PRIORITY, &mp_main_task_handle, 1);
    

}

void nlr_jump_fail(void *val) {
    printf("NLR jump failed, val=%p\n", val);
    esp_restart();
}

// modussl_mbedtls uses this function but it's not enabled in ESP IDF
void mbedtls_debug_set_threshold(int threshold) {
    (void)threshold;
}

void *esp_native_code_commit(void *buf, size_t len, void *reloc) {
    len = (len + 3) & ~3;
    uint32_t *p = heap_caps_malloc(len, MALLOC_CAP_EXEC);
    if (p == NULL) {
        m_malloc_fail(len);
    }
    if (reloc) {
        mp_native_relocate(reloc, buf, (uintptr_t)p);
    }
    memcpy(p, buf, len);
    return p;
}
