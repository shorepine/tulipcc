#ifdef ESP_PLATFORM
#include "esp_task.h"
#ifndef TASKS_H
#define TASKS_H
#define MP_TASK_PRIORITY        (ESP_TASK_PRIO_MIN + 1)
#define DISPLAY_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 5)
#define USB_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define ALLES_TASK_PRIORITY (ESP_TASK_PRIO_MIN+2)
#define TOUCHSCREEN_TASK_PRIORITY (ESP_TASK_PRIO_MIN)

//#define MP_TASK_COREID (1) // forced to 1 downstream
#define DISPLAY_TASK_COREID (0)
#define USB_TASK_COREID (1)
#define ALLES_TASK_COREID (1)
#define TOUCHSCREEN_TASK_COREID  (0)

#define MP_TASK_STACK_SIZE      (16 * 1024)
#define DISP_TASK_STACK_SIZE    (8 * 1024) 
#define USB_TASK_STACK_SIZE    (4 * 1024) 
#define ALLES_TASK_STACK_SIZE    (4 * 1024) 
#define TOUCHSCREEN_TASK_STACK_SIZE (2 * 1024)

#define MAX_TASKS 17

TaskHandle_t display_main_task_handle;
TaskHandle_t usb_main_task_handle;
TaskHandle_t alles_main_task_handle;
TaskHandle_t touch_main_task_handle;
TaskHandle_t idle_task_0;
TaskHandle_t idle_task_1;

// For CPU usage
unsigned long last_task_counters[MAX_TASKS];
#endif
#endif
