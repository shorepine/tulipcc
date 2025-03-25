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
#include "esp_event.h"
#include "esp_log.h"
#include "esp_psram.h"

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

#include "tsequencer.h"


#if MICROPY_BLUETOOTH_NIMBLE
#include "extmod/modbluetooth.h"
#endif

#if MICROPY_ESPNOW
#include "modespnow.h"
#endif

// Settings for memory-mapped location of SPIRAM.
#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define IDF_TARGET_PSRAM_ADDR_START (SOC_EXTRAM_DATA_LOW)
#define IDF_TARGET_PSRAM_SIZE (SOC_EXTRAM_DATA_SIZE)
#elif CONFIG_IDF_TARGET_ESP32S3
#define IDF_TARGET_PSRAM_ADDR_START (SOC_DROM_HIGH)
#define IDF_TARGET_PSRAM_SIZE (SOC_EXTRAM_DATA_HIGH - IDF_TARGET_PSRAM_ADDR_START)
#endif

// MicroPython runs as a task under FreeRTOS
#define MP_TASK_PRIORITY        (ESP_TASK_PRIO_MIN + 1)

// Set the margin for detecting stack overflow, depending on the CPU architecture.
#if CONFIG_IDF_TARGET_ESP32C3
#define MP_TASK_STACK_LIMIT_MARGIN (2048)
#else
#define MP_TASK_STACK_LIMIT_MARGIN (1024)
#endif

#include "alles.h"
#include "tasks.h"


TaskHandle_t tulip_mp_handle;
TaskHandle_t midi_handle;
//TaskHandle_t alles_handle;
//TaskHandle_t alles_parse_handle;
//TaskHandle_t alles_receive_handle;
//TaskHandle_t amy_render_handle;
//TaskHandle_t alles_fill_buffer_handle;
TaskHandle_t idle_0_handle;
TaskHandle_t idle_1_handle;
TaskHandle_t sequencer_handle;

// For CPU usage
unsigned long last_task_counters[MAX_TASKS];


typedef void (*esp_alloc_failed_hook_t) (size_t size, uint32_t caps, const char * function_name);
#include "esp_heap_caps.h"

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



float compute_cpu_usage(uint8_t debug) {
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x, i;

    const char* const tasks[] = {
         "IDLE0", "IDLE1", "Tmr Svc", "ipc0", "ipc1", "main", "wifi", "esp_timer", "sys_evt", "tiT",
         TULIP_MP_TASK_NAME, MIDI_TASK_NAME, ALLES_TASK_NAME,
         ALLES_PARSE_TASK_NAME, ALLES_RECEIVE_TASK_NAME, ALLES_RENDER_TASK_NAME, ALLES_FILL_BUFFER_TASK_NAME, SEQUENCER_TASK_NAME, 0
    };
    const uint8_t cores[] = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, TULIP_MP_TASK_COREID,
        MIDI_TASK_COREID, ALLES_TASK_COREID, ALLES_PARSE_TASK_COREID, ALLES_RECEIVE_TASK_COREID, ALLES_RENDER_TASK_COREID, ALLES_FILL_BUFFER_TASK_COREID, 
        SEQUENCER_TASK_COREID};

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
    unsigned long ulTotalRunTime_per_core[2];
    ulTotalRunTime_per_core[0] = 0;
    ulTotalRunTime_per_core[1] = 0;

    // We have to check for the names we want to track
    for(i=0;i<MAX_TASKS;i++) { // for each name
        counter_since_last[i] = 0;
        for(x=0; x<uxArraySize; x++) { // for each task
            if(strcmp(pxTaskStatusArray[x].pcTaskName, tasks[i])==0) {
                counter_since_last[i] = pxTaskStatusArray[x].ulRunTimeCounter - last_task_counters[i];
                last_task_counters[i] = pxTaskStatusArray[x].ulRunTimeCounter;
                ulTotalRunTime_per_core[cores[i]]= ulTotalRunTime_per_core[cores[i]] + counter_since_last[i];
            }
        }
    }
    if(debug) {
        printf("------ CPU usage since last call to tulip.cpu()\n");
        for(i=0;i<MAX_TASKS;i++) {
            printf("%d %-15s\t%-15ld\t\t%2.2f%%\n", cores[i], tasks[i], counter_since_last[i], ((float)counter_since_last[i])/ulTotalRunTime_per_core[cores[i]] * 100.0);
        }   
    }
    vPortFree(pxTaskStatusArray);

    // Also print heap info
    if(debug){
        fprintf(stderr, "SPIRAM:\n "); fflush(stderr);
        heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

        fprintf(stderr, "INTERNAL:\n "); fflush(stderr);
        heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
    }

    unsigned long freeTime = counter_since_last[0] + counter_since_last[1]; // add IDLE0 + IDLE1 
    unsigned long ulTotalRunTime = ulTotalRunTime_per_core[0] + ulTotalRunTime_per_core[1]; // add total counts 
    return 100.0 - (((float)freeTime/(float)ulTotalRunTime) * 100.0); // return CPU usage

}


int vprintf_null(const char *format, va_list ap) {
    // do nothing: this is used as a log target during raw repl mode
    return 0;
}

extern void setup_lvgl();
uint8_t lvgl_setup = 0;

void mp_task(void *pvParameter) {

    volatile uint32_t sp = (uint32_t)esp_cpu_get_sp();
    #if MICROPY_PY_THREAD
    mp_thread_init(pxTaskGetStackStart(NULL), MICROPY_TASK_STACK_SIZE / sizeof(uintptr_t));
    #endif
    #if MICROPY_HW_ESP_USB_SERIAL_JTAG
    usb_serial_jtag_init();
    #elif MICROPY_HW_ENABLE_USBDEV
    usb_init();
    #endif
    #if MICROPY_HW_ENABLE_UART_REPL
    uart_stdout_init();
    #endif
    machine_init();

    
    esp_err_t err = esp_event_loop_create_default();
    if (err != ESP_OK) {
        ESP_LOGE("esp_init", "can't create event loop: 0x%x\n", err);
    }
    
    heap_caps_register_failed_alloc_callback(esp_alloc_failed);
    uint32_t caps = MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM;
    size_t mp_task_heap_size = MP_TASK_HEAP_SIZE; 
    void *mp_task_heap = heap_caps_malloc(mp_task_heap_size, caps);

soft_reset:

    // initialise the stack pointer for the main thread
    mp_stack_set_top((void *)sp);
    mp_stack_set_limit(TULIP_MP_TASK_STACK_SIZE - MP_TASK_STACK_LIMIT_MARGIN);

    gc_init(mp_task_heap, mp_task_heap + mp_task_heap_size);
    mp_init();

    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));

    readline_init0();

    MP_STATE_PORT(native_code_pointers) = MP_OBJ_NULL;
    
    // initialise peripherals
    machine_pins_init();
    #if MICROPY_PY_MACHINE_I2S
    //machine_i2s_init0();
    #endif
    
    // run boot-up scripts
    pyexec_frozen_module("_boot.py", false);
    
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

    #if MICROPY_ESPNOW
    espnow_deinit(mp_const_none);
    MP_STATE_PORT(espnow_singleton) = NULL;
    #endif

    machine_timer_deinit_all();

    #if MICROPY_PY_THREAD
    mp_thread_deinit();
    #endif

    // Free any native code pointers that point to iRAM.
    if (MP_STATE_PORT(native_code_pointers) != MP_OBJ_NULL) {
        size_t len;
        mp_obj_t *items;
        mp_obj_list_get(MP_STATE_PORT(native_code_pointers), &len, &items);
        for (size_t i = 0; i < len; ++i) {
            heap_caps_free(MP_OBJ_TO_PTR(items[i]));
        }
    }

    gc_sweep_all();

    mp_hal_stdout_tx_str("MPY: soft reboot\r\n");
    esp_restart();
    // deinitialise peripherals
    machine_pwm_deinit_all();
    // TODO: machine_rmt_deinit_all();
    machine_pins_deinit();
    machine_deinit();
    #if MICROPY_PY_USOCKET_EVENTS
    usocket_events_deinit();
    #endif

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


extern void run_midi();



uint8_t * xStack;
StaticTask_t static_mp_handle;


//esp_err_t i2c_master_init(void);
//esp_err_t i2c_slave_init(void);
//esp_err_t setup_pcm9211(void);

void app_main(void) {
    // Hook for a board to run code at start up.
    // This defaults to initialising NVS.
    MICROPY_BOARD_STARTUP();
    for(uint8_t i=0;i<MAX_TASKS;i++) last_task_counters[i] = 0;

    // Grab the idle tasks
    idle_0_handle = xTaskGetIdleTaskHandleForCPU(0);
    idle_1_handle = xTaskGetIdleTaskHandleForCPU(1);




    // TODO -- USB gadget 

    /*
    fprintf(stderr,"Starting AMY on core %d\n", ALLES_TASK_COREID);
    check_init(&i2c_master_init, "i2c_master");
    check_init(&i2c_slave_init, "i2c_slave");
    check_init(&setup_pcm9211, "pcm9211");
    */

    //run_alles();
    fflush(stderr);
    delay_ms(500);
    

    fprintf(stderr,"Starting MicroPython on core %d\n", TULIP_MP_TASK_COREID);
    xTaskCreatePinnedToCore(mp_task, TULIP_MP_TASK_NAME, (TULIP_MP_TASK_STACK_SIZE) / sizeof(StackType_t), NULL, TULIP_MP_TASK_PRIORITY, &tulip_mp_handle, TULIP_MP_TASK_COREID);
    fflush(stderr);
    delay_ms(100);
    fprintf(stderr,"Starting MIDI on core %d\n", MIDI_TASK_COREID);
    xTaskCreatePinnedToCore(run_midi, MIDI_TASK_NAME, MIDI_TASK_STACK_SIZE / sizeof(StackType_t), NULL, MIDI_TASK_PRIORITY, &midi_handle, MIDI_TASK_COREID);
    fflush(stderr);
    delay_ms(100);
    tsequencer_init();
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
    if (MP_STATE_PORT(native_code_pointers) == MP_OBJ_NULL) {
        MP_STATE_PORT(native_code_pointers) = mp_obj_new_list(0, NULL);
    }
    mp_obj_list_append(MP_STATE_PORT(native_code_pointers), MP_OBJ_TO_PTR(p));
    if (reloc) {
        mp_native_relocate(reloc, buf, (uintptr_t)p);
    }
    memcpy(p, buf, len);
    return p;
}

MP_REGISTER_ROOT_POINTER(mp_obj_t native_code_pointers);
