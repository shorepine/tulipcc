// libsoundio-audio.c
// functions for running AMY on a computer

#ifndef ESP_PLATFORM
#include "libsoundio-audio.h"


struct SoundIo *soundio;
struct SoundIoDevice *device;
 struct SoundIoOutStream *outstream;
int16_t leftover_buf[BLOCK_SIZE]; 
uint16_t leftover_samples = 0;
int16_t amy_channel = -1;
int16_t amy_device_id = -1;
uint8_t amy_running = 0;
pthread_t amy_live_thread;



void amy_print_devices() {
    struct SoundIo *soundio2 = soundio_create();
    soundio_connect(soundio2);
    soundio_flush_events(soundio2);

    int output_count = soundio_output_device_count(soundio2);
    int default_output = soundio_default_output_device_index(soundio2);
    for (int i = 0; i < output_count; i += 1) {
        struct SoundIoDevice *device = soundio_get_output_device(soundio2, i);
        const char *default_str = (i==default_output) ? " (default)" : "";
        const char *raw_str = device->is_raw ? " (raw)" : "";
        int chans = device->layouts[0].channel_count;
        fprintf(stderr,"[%d]\t%s%s%s %d outputs, formats: ", i, device->name, default_str, raw_str, chans);
        for (int j = 0; j < device->format_count; j += 1) {
            const char *comma = (j == device->format_count - 1) ? "" : ", ";
            fprintf(stderr, "%s%s", soundio_format_string(device->formats[j]), comma);
        }
        fprintf(stderr,"\n");
        soundio_device_unref(device);
    }
}

static void soundio_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    struct SoundIoChannelArea *areas;
    int err;


    int frame_count = frame_count_max; // on mac they're always the same. on linux i guess it doesn't matter to choose max
    if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
        fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
        exit(1);
    }

    // Different audio devices on mac have wildly different frame_count_maxes, so we have to be ok with 
    // an audio buffer that is not an even multiple of BLOCK_SIZE. my iMac's speakers were always 512 frames, but
    // external headphones on a MBP is 432 or 431, and airpods were something like 1440.

    // First send over the leftover samples, if any
    for(uint16_t frame=0;frame<leftover_samples;frame++) {
        for(uint8_t c=0;c<layout->channel_count;c++) {
            if(c==amy_channel || amy_channel<0) {
                *((int16_t*)areas[c].ptr) = leftover_buf[frame];
            } else {
                *((int16_t*)areas[c].ptr) = 0;
            }
            areas[c].ptr += areas[c].step;
        }
    }
    frame_count -= leftover_samples;
    leftover_samples = 0;

    // Now send the bulk of the frames
    for(uint8_t i=0;i<(uint8_t)(frame_count / BLOCK_SIZE);i++) {
        int16_t *buf = fill_audio_buffer_task();
        for(uint16_t frame=0;frame<BLOCK_SIZE;frame++) {
            for(uint8_t c=0;c<layout->channel_count;c++) {
                if(c==amy_channel || amy_channel < 0) {
                    *((int16_t*)areas[c].ptr) = buf[frame];
                } else {
                    *((int16_t*)areas[c].ptr) = 0;
                }
                areas[c].ptr += areas[c].step;
            }
        }
    } 

    // If any leftover, let's put those in the outgoing buf and the rest in leftover_samples
    uint16_t still_need = frame_count % BLOCK_SIZE;
    if(still_need != 0) {
        int16_t * buf = fill_audio_buffer_task();
        for(uint16_t frame=0;frame<still_need;frame++) {
            for(uint8_t c=0;c<layout->channel_count;c++) {
                if(c==amy_channel || amy_channel < 0) {
                    *((int16_t*)areas[c].ptr) = buf[frame];
                } else {
                    *((int16_t*)areas[c].ptr) = 0;
                }
                areas[c].ptr += areas[c].step;
            }
        }
        memcpy(leftover_buf, buf+still_need, (BLOCK_SIZE - still_need)*2);
        leftover_samples = BLOCK_SIZE - still_need;
    }
    if ((err = soundio_outstream_end_write(outstream))) {
        if (err == SoundIoErrorUnderflow)
            return;
        fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
        exit(1);
    }
}

static void underflow_callback(struct SoundIoOutStream *outstream) {
    static int count = 0;
    fprintf(stderr, "underflow %d\n", count++);
}

// start soundio
amy_err_t soundio_init() {

    soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }
#ifdef __linux__
    int err;
    err = soundio_connect_backend(soundio, SoundIoBackendPulseAudio);
#else
    int err = soundio_connect(soundio);
#endif
    if (err) {
        fprintf(stderr, "Unable to connect to backend: %s\n", soundio_strerror(err));
        exit(1);
    }

    soundio_flush_events(soundio);
    int selected_device_index;
    if(amy_device_id < 0)  {
        selected_device_index = soundio_default_output_device_index(soundio);
    } else {
        selected_device_index = amy_device_id;
    }


    if (selected_device_index < 0) {
        fprintf(stderr, "Output device not found\n");
        return 1;
    }

    device = soundio_get_output_device(soundio, selected_device_index);
    if(amy_channel > device->layouts[0].channel_count-1) {
        printf("Requested channel number more than available, setting to -1\n");
        amy_channel = -1;
    }
    if (!device) {
        fprintf(stderr, "out of memory\n");
        return 1;
    } else {
        //const char *all_str = (amy_channel<0) ? " (all)" : "";
        //fprintf(stderr,"Using device ID %d, device %s, channel %d %s\n", selected_device_index, device->name, amy_channel, all_str);
    }

    if (device->probe_error) {
        fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(device->probe_error));
        return 1;
    }

    outstream = soundio_outstream_create(device);
    if (!outstream) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    outstream->write_callback = soundio_callback;
    outstream->underflow_callback = underflow_callback;
    outstream->name = NULL;
    outstream->software_latency = 0.0;
    outstream->sample_rate = SAMPLE_RATE;

    if (soundio_device_supports_format(device, SoundIoFormatS16NE)) {
        outstream->format = SoundIoFormatS16NE;
    } else {
        fprintf(stderr, "No suitable device format available.\n");
        return 1;
    }
    
    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }
    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s\n", soundio_strerror(err));
        return 1;
    }
    return AMY_OK;
}

void *soundio_run(void *vargp) {
    soundio_init();
    while(amy_running) {
        soundio_flush_events(soundio);
        usleep(THREAD_USLEEP);
    }
    return NULL;
}

void amy_live_start() {
    // kick off a thread running soundio_run
    amy_running = 1;
    pthread_create(&amy_live_thread, NULL, soundio_run, NULL);
}


void amy_live_stop() {
    amy_running = 0;
    pthread_join(amy_live_thread, NULL);
    soundio_outstream_destroy(outstream);
    soundio_device_unref(device);
    soundio_destroy(soundio);

}

#endif
