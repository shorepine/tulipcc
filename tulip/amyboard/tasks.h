#ifdef ESP_PLATFORM
#include "esp_task.h"
#ifndef TASKS_H
#define TASKS_H


#define SEQUENCER_TASK_PRIORITY (ESP_TASK_PRIO_MAX - 1) // Can be low because it sets its own timers?

//#define TULIP_MP_TASK_PRIORITY (ESP_TASK_PRIO_MAX - 2)
#define TULIP_MP_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 1)

#define MIDI_TASK_PRIORITY (ESP_TASK_PRIO_MAX - 2)

//#define ALLES_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 2)

#define ALLES_PARSE_TASK_PRIORITY (ESP_TASK_PRIO_MIN +2)
#define ALLES_RECEIVE_TASK_PRIORITY (ESP_TASK_PRIO_MIN + 3)
#define ALLES_RENDER_TASK_PRIORITY (ESP_TASK_PRIO_MAX )
#define ALLES_FILL_BUFFER_TASK_PRIORITY (ESP_TASK_PRIO_MAX )
#define TULIP_MP_TASK_COREID (1)
#define SEQUENCER_TASK_COREID (0)
#define MIDI_TASK_COREID (0)
#define ALLES_TASK_COREID (1)
#define ALLES_PARSE_TASK_COREID (0)
#define ALLES_RECEIVE_TASK_COREID (1)
#define ALLES_RENDER_TASK_COREID (0)
#define ALLES_FILL_BUFFER_TASK_COREID (1)

#define TULIP_MP_TASK_STACK_SIZE      (32 * 1024)
#define SEQUENCER_TASK_STACK_SIZE (2 * 1024)
#define MIDI_TASK_STACK_SIZE (16 * 1024)
#define ALLES_TASK_STACK_SIZE    (4 * 1024) 
#define ALLES_PARSE_TASK_STACK_SIZE (8 * 1024)
#define ALLES_RECEIVE_TASK_STACK_SIZE (4 * 1024)
#define ALLES_RENDER_TASK_STACK_SIZE (8 * 1024)
#define ALLES_FILL_BUFFER_TASK_STACK_SIZE (8 * 1024)

#define MP_TASK_HEAP_SIZE (2 * 1024 * 1024)

#define SEQUENCER_TASK_NAME         "seq_task"
#define TULIP_MP_TASK_NAME          "tulip_mp_task"
#define MIDI_TASK_NAME              "midi_task"
#define ALLES_TASK_NAME             "alles_task"
#define ALLES_PARSE_TASK_NAME       "alles_par_task"
#define ALLES_RECEIVE_TASK_NAME     "alles_rec_task"
#define ALLES_RENDER_TASK_NAME      "alles_r_task"
#define ALLES_FILL_BUFFER_TASK_NAME "alles_fb_task"

#define MAX_TASKS 18 // includes system tasks

extern TaskHandle_t sequencer_handle;
extern TaskHandle_t tulip_mp_handle;
extern TaskHandle_t midi_handle;
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
