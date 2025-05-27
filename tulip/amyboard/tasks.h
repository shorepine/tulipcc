#ifdef ESP_PLATFORM
#include "esp_task.h"
#ifndef TASKS_H
#define TASKS_H


#define TULIP_MP_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define TULIP_MP_TASK_COREID (1)
#define TULIP_MP_TASK_STACK_SIZE      (32 * 1024)
#define MP_TASK_HEAP_SIZE (2 * 1024 * 1024)
#define TULIP_MP_TASK_NAME          "tulip_mp_task"

#define MAX_TASKS 18 // includes system tasks

extern TaskHandle_t tulip_mp_handle;
extern TaskHandle_t idle_0_handle;
extern TaskHandle_t idle_1_handle;
// For CPU usage
extern unsigned long last_task_counters[MAX_TASKS];
#endif

#endif
