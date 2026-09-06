#ifdef ESP_PLATFORM
#include "esp_task.h"
#ifndef TASKS_H
#define TASKS_H


#define TULIP_MP_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)
#define TULIP_MP_TASK_COREID (1)
#define TULIP_MP_TASK_STACK_SIZE      (32 * 1024)
#define MP_TASK_HEAP_SIZE (2 * 1024 * 1024)
#define TULIP_MP_TASK_NAME          "tulip_mp_task"

// One step above i2c_bg_task (the OLED flush, also PRIO_MIN+1 on core 0) so an
// in-flight display refresh can't preempt the CV task in the middle of its
// back-to-back ADS1015 conversions and stretch the inter-channel skew. The
// task is short -- ~1.3ms of every 6ms period -- so this doesn't starve the
// flush. The I2C follower task at priority 20 still outranks us.
#define CV_READ_TASK_PRIORITY   (ESP_TASK_PRIO_MIN + 2)
#define CV_READ_TASK_COREID     (0)
#define CV_READ_TASK_STACK_SIZE (2 * 1024)
#define CV_READ_TASK_NAME       "cv_read_task"

#define MAX_TASKS 17 // includes system tasks

extern TaskHandle_t tulip_mp_handle;
extern TaskHandle_t idle_0_handle;
extern TaskHandle_t idle_1_handle;
extern TaskHandle_t cv_read_handle;
// For CPU usage
extern unsigned long last_task_counters[MAX_TASKS];
#endif

#endif
