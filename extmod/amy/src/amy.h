
#ifndef __AMY_H
#define __AMY_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "polyfills.h"

#define OSCS 64              // # of simultaneous oscs to keep track of 
#define AMY_CORES 2
#define PCM_LARGE 2
#define PCM_SMALL 1
#define PCM_PATCHES_SIZE PCM_SMALL
#define BLOCK_SIZE 256       // buffer block size in samples
#define KS_OSCS 1            // # of Karplus-strong oscillators allowed at once, they're big RAM users so keep it small on esp
#define EVENT_FIFO_LEN 3000  // number of events the queue can store
#define MAX_DRIFT_MS 20000   // ms of time you can schedule ahead before synth recomputes time base
#define SAMPLE_RATE 44100    // playback sample rate
#define SAMPLE_MAX 32767
#define MAX_ALGO_OPS 6 
#define MAX_BREAKPOINTS 8
#define MAX_BREAKPOINT_SETS 3
#define THREAD_USLEEP 500
#define BYTES_PER_SAMPLE 2

//#define NCHANS 1             // Mono output, 'Q' (pan) ignored, saves memory.
#define NCHANS 2             // Enable 2-channel output, pan, etc.


typedef int16_t output_sample_type;

// D is how close the sample gets to the clip limit before the nonlinearity engages.  
// So D=0.1 means output is linear for -0.9..0.9, then starts clipping.
#define CLIP_D 0.1
#define MAX_VOLUME 11.0
#define MINIMUM_SCALE 0.000190 // computed from TRUE_EXPONENTIAL's end point after a while 
#define BREAKPOINT_EPS 0.0002

// center frequencies for the EQ
#define EQ_CENTER_LOW 800.0
#define EQ_CENTER_MED 2500.0
#define EQ_CENTER_HIGH 7000.0

#define LINEAR_INTERP        // use linear interp for oscs
// "The cubic stuff is just showing off.  One would only ever use linear in prod." -- dpwe, May 10 2021 
//#define CUBIC_INTERP         // use cubic interpolation for oscs

// Sample values for modulation sources
#define UP    32767
#define DOWN -32768

// modulation/breakpoint target mask (int16)
#define TARGET_AMP 1
#define TARGET_DUTY 2
#define TARGET_FREQ 4
#define TARGET_FILTER_FREQ 8
#define TARGET_RESONANCE 0x10
#define TARGET_FEEDBACK 0x20
#define TARGET_LINEAR 0x40 // default exp, linear as an option
#define TARGET_TRUE_EXPONENTIAL 0x80 // default exp, "true exp" for FM as an option
#define TARGET_DX7_EXPONENTIAL 0x100 // Asymmetric attack/decay behavior per DX7.

#define MAX_MESSAGE_LEN 255

#define FILTER_LPF 1
#define FILTER_BPF 2
#define FILTER_HPF 3
#define FILTER_NONE 0
#define SINE 0
#define PULSE 1
#define SAW_DOWN 2
#define SAW_UP 3
#define TRIANGLE 4
#define NOISE 5
#define KS 6
#define PCM 7
#define ALGO 8
#define PARTIAL 9
#define PARTIALS 10
#define OFF 11

#define EMPTY 0
#define SCHEDULED 1
#define PLAYED 2
#define AUDIBLE 3
#define IS_MOD_SOURCE 4
#define IS_ALGO_SOURCE 5

#define AMY_OK 0
typedef int amy_err_t;

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif


enum params{
    WAVE, PATCH, MIDI_NOTE, AMP, DUTY, FEEDBACK, FREQ, VELOCITY, PHASE, DETUNE, VOLUME, GAIN_L, GAIN_R, FILTER_FREQ,
    RATIO, RESONANCE, 
    MOD_SOURCE, MOD_TARGET, FILTER_TYPE, EQ_L, EQ_M, EQ_H, BP0_TARGET, BP1_TARGET, BP2_TARGET, ALGORITHM, LATENCY,
    ALGO_SOURCE_START=30, 
    ALGO_SOURCE_END=30+MAX_ALGO_OPS,
    BP_START=ALGO_SOURCE_END+1,   
    BP_END=BP_START + (MAX_BREAKPOINT_SETS * MAX_BREAKPOINTS * 2),
    NO_PARAM
};

// Delta holds the individual changes from an event, it's sorted in order of playback time 
// this is more efficient in memory than storing entire events per message 
struct delta {
    uint32_t data; // casted to the right thing later
    enum params param; // which parameter is being changed
    uint32_t time; // what time to play / change this parameter
    int8_t osc; // which oscillator it impacts
    struct delta * next; // the next event, in time 
};


// Events are used to parse from ASCII UDP strings into, and also as each oscillators current internal state 
struct event {
    // todo -- clean up types here - many don't need to be signed anymore, and time doesn't need to be int64
    int64_t time;
    int8_t osc;
    int16_t wave;
    int16_t patch;
    int16_t midi_note;
    float amp;
    float duty;
    float feedback;
    float freq;
    uint8_t status;
    float velocity;
    float phase;
    float detune;
    float step;
    float substep;
    float sample;
    float volume;
    float gain_l;   // Pan parameters.
    float gain_r;
    int16_t latency_ms;
    float filter_freq;
    float ratio;
    float resonance;
    int8_t mod_source;
    int8_t mod_target;
    int8_t algorithm;
    int8_t filter_type;
    int8_t algo_source[MAX_ALGO_OPS];

    int64_t note_on_clock;
    int64_t note_off_clock;
    int16_t breakpoint_target[MAX_BREAKPOINT_SETS];
    int32_t breakpoint_times[MAX_BREAKPOINT_SETS][MAX_BREAKPOINTS];
    float breakpoint_values[MAX_BREAKPOINT_SETS][MAX_BREAKPOINTS];
    float last_scale[MAX_BREAKPOINT_SETS];  // remembers current envelope level, to use as start point in release.
  
    // State variable for the impulse-integrating oscs.
    float lpf_state;
    // Constant offset to add to sawtooth before integrating.
    float dc_offset;
    // Decay alpha of LPF filter (e.g. 0.99 or 0.999).
    float lpf_alpha;
    // amplitude smoother
    float last_amp;
    // Selected lookup table and size.
    const float *lut;
    int16_t lut_size;
    float eq_l;
    float eq_m;
    float eq_h;
    // For ALGO feedback ops
    float last_two[2];
};

// events, but only the things that mods/env can change. one per osc
struct mod_event {
    float amp;
    float duty;
    float freq;
    float filter_freq;
    float resonance;
    float feedback;
};

// Callbacks, override if you'd like after calling amy_start()
void (*amy_parse_callback)(char,char*);

struct event amy_default_event();
void amy_add_event(struct event e);
void render_task(uint8_t start, uint8_t end, uint8_t core);
void show_debug(uint8_t type) ;
void oscs_deinit() ;
int64_t amy_sysclock();
float freq_for_midi_note(uint8_t midi_note);
int8_t check_init(amy_err_t (*fn)(), char *name);
void amy_increase_volume();
void amy_decrease_volume();


// global synth state
struct state {
    float volume;
    // State of fixed dc-blocking HPF
    float hpf_state;
    float eq[3];
    uint16_t event_qsize;
    int16_t next_event_write;
    struct delta * event_start; // start of the sorted list
    int16_t latency_ms;
};

// Shared structures
extern float coeffs[OSCS][5];
extern float delay[OSCS][2];
extern int64_t total_samples;
extern struct event *synth;
extern struct mod_event *msynth; // the synth that is being modified by modulations & envelopes
extern struct state global; 


int8_t oscs_init();
void parse_breakpoint(struct event * e, char* message, uint8_t bp_set) ;
void parse_algorithm(struct event * e, char* message) ;
void hold_and_modify(uint8_t osc) ;
int16_t * fill_audio_buffer_task();
int32_t ms_to_samples(int32_t ms) ;


// external functions
void amy_play_message(char *message);
struct event amy_parse_message(char * message);
void amy_start();
void amy_stop();
void amy_live_start();
void amy_live_stop();
void amy_reset_oscs();
void amy_print_devices(); 


extern float render_am_lut(float * buf, float step, float skip, float incoming_amp, float ending_amp, const float* lut, int16_t lut_size, float *mod, float bandwidth);
extern void ks_init();
extern void ks_deinit();
extern void algo_init();
extern void pcm_init();
extern void render_ks(float * buf, uint8_t osc); 
extern void render_sine(float * buf, uint8_t osc); 
extern void render_fm_sine(float *buf, uint8_t osc, float *mod, float feedback_level, uint8_t algo_osc);
extern void render_pulse(float * buf, uint8_t osc); 
extern void render_saw_down(float * buf, uint8_t osc);
extern void render_saw_up(float * buf, uint8_t osc);
extern void render_triangle(float * buf, uint8_t osc); 
extern void render_noise(float * buf, uint8_t osc); 
extern void render_pcm(float * buf, uint8_t osc);
extern void render_algo(float * buf, uint8_t osc) ;
extern void render_partial(float *buf, uint8_t osc) ;
extern void partials_note_on(uint8_t osc);
extern void partials_note_off(uint8_t osc);
extern void render_partials(float *buf, uint8_t osc);

extern float compute_mod_pulse(uint8_t osc);
extern float compute_mod_noise(uint8_t osc);
extern float compute_mod_sine(uint8_t osc);
extern float compute_mod_saw_up(uint8_t osc);
extern float compute_mod_saw_down(uint8_t osc);
extern float compute_mod_triangle(uint8_t osc);
extern float compute_mod_pcm(uint8_t osc);

extern void ks_note_on(uint8_t osc); 
extern void ks_note_off(uint8_t osc);
extern void sine_note_on(uint8_t osc); 
extern void fm_sine_note_on(uint8_t osc, uint8_t algo_osc); 
extern void saw_down_note_on(uint8_t osc); 
extern void saw_up_note_on(uint8_t osc); 
extern void triangle_note_on(uint8_t osc); 
extern void pulse_note_on(uint8_t osc); 
extern void pcm_note_on(uint8_t osc);
extern void pcm_note_off(uint8_t osc);
extern void partial_note_on(uint8_t osc);
extern void partial_note_off(uint8_t osc);
extern void algo_note_on(uint8_t osc);
extern void algo_note_off(uint8_t osc) ;
extern void sine_mod_trigger(uint8_t osc);
extern void saw_down_mod_trigger(uint8_t osc);
extern void saw_up_mod_trigger(uint8_t osc);
extern void triangle_mod_trigger(uint8_t osc);
extern void pulse_mod_trigger(uint8_t osc);
extern void pcm_mod_trigger(uint8_t osc);
extern float amy_get_random();

// filters
extern void filters_init();
extern void filters_deinit();
extern void filter_process(float * block, uint8_t osc);
extern void parametric_eq_process(float *block);
extern void update_filter(uint8_t osc);
extern float dsps_sqrtf_f32_ansi(float f);
extern int8_t dsps_biquad_gen_lpf_f32(float *coeffs, float f, float qFactor);
extern int8_t dsps_biquad_f32_ansi(const float *input, float *output, int len, float *coef, float *w);
// Use the esp32 optimized biquad filter if available
#ifdef ESP_PLATFORM
#include "esp_err.h"
esp_err_t dsps_biquad_f32_ae32(const float *input, float *output, int len, float *coef, float *w);
#endif



// envelopes
extern float compute_breakpoint_scale(uint8_t osc, uint8_t bp_set);
extern float compute_mod_scale(uint8_t osc);
extern void retrigger_mod_source(uint8_t osc);



#endif



