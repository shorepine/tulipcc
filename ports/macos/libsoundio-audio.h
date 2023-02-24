// libsoundio-audio.h

#ifndef __LIBSOUNDIO_AUDIO_H
#define __LIBSOUNDIO_AUDIO_H
#include "amy.h"
#include <soundio/soundio.h>
#include <pthread.h>

extern int16_t amy_channel;
extern int16_t amy_device_id;
extern uint8_t amy_running;

void amy_print_devices();
void amy_live_start();
void amy_live_stop();
#endif