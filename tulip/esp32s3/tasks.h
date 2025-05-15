#ifdef ESP_PLATFORM
#include "esp_task.h"
#ifndef TASKS_H
#define TASKS_H
#include "alles.h"

// All of the ESP tasks that we create in one place
#define DISPLAY_TASK_PRIORITY (ESP_TASK_PRIO_MIN)
#define USB_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define TOUCHSCREEN_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define SEQUENCER_TASK_PRIORITY (ESP_TASK_PRIO_MAX - 1) // Can be low because it sets its own timers?
#define TULIP_MP_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define AMY_TASK_PRIORITY (ESP_TASK_PRIO_MAX)

#define ALLES_PARSE_TASK_PRIORITY (ESP_TASK_PRIO_MIN +2)
#define ALLES_RECEIVE_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 3)
#define ALLES_RENDER_TASK_PRIORITY (ESP_TASK_PRIO_MAX )
#define ALLES_FILL_BUFFER_TASK_PRIORITY (ESP_TASK_PRIO_MAX )

// Since display is on core0, things on core0 will be slower than things on core1
#define DISPLAY_TASK_COREID (0)
#define USB_TASK_COREID (1)
#define TOUCHSCREEN_TASK_COREID  (0)
#define TULIP_MP_TASK_COREID (1)
#define SEQUENCER_TASK_COREID (0)
#define AMY_TASK_COREID (1)

#define DISPLAY_TASK_STACK_SIZE    (4 * 1024) 
#define USB_TASK_STACK_SIZE    (8 * 1024) 
#define TOUCHSCREEN_TASK_STACK_SIZE (4 * 1024)
#define TULIP_MP_TASK_STACK_SIZE      (32 * 1024)
#define SEQUENCER_TASK_STACK_SIZE (2 * 1024)
#define AMY_TASK_STACK_SIZE    (8 * 1024) 

#define MP_TASK_HEAP_SIZE (2 * 1024 * 1024)

#define DISPLAY_TASK_NAME           "display_task"
#define USB_TASK_NAME               "usb_task"
#define TOUCHSCREEN_TASK_NAME       "tscreen_task"
#define SEQUENCER_TASK_NAME         "seq_task"
#define TULIP_MP_TASK_NAME          "tulip_mp_task"
#define AMY_TASK_NAME                "amy_task"

#define MAX_TASKS 18 // includes system tasks

extern TaskHandle_t display_handle;
extern TaskHandle_t usb_handle;
extern TaskHandle_t sequencer_handle;
extern TaskHandle_t touchscreen_handle;
extern TaskHandle_t tulip_mp_handle;
extern TaskHandle_t alles_handle;
extern TaskHandle_t alles_parse_handle;
extern TaskHandle_t alles_receive_handle;
extern TaskHandle_t amy_render_handle;
extern TaskHandle_t alles_fill_buffer_handle;
extern TaskHandle_t idle_0_handle;
extern TaskHandle_t idle_1_handle;
// For CPU usage
extern unsigned long last_task_counters[MAX_TASKS];
#endif

#endif
