#include "amy.h"
#include "delay.h"

int is_power_of_two(int val) {
    while(val) {
        if (val == 1) return true;
        if (val & 1) return false;
        val >>= 1;
    }
    return false;  // zero is not a power of 2.
}


delay_line_t *new_delay_line(int len, int fixed_delay, int ram_type) {
    // Check that len is a power of 2.
    if (!is_power_of_two(len)) { fprintf(stderr, "delay line len must be power of 2, not %d\n", len); abort(); }
    delay_line_t *delay_line = (delay_line_t*)malloc_caps(sizeof(delay_line_t), ram_type); 
    delay_line->samples = (float*)malloc_caps(len * sizeof(float), ram_type);
    delay_line->len = len;
    delay_line->fixed_delay = fixed_delay;
    delay_line->next_in = 0;
    delay_line->max_delay = len;
    for (int i = 0; i < len; ++i) {
        delay_line->samples[i] = 0;
    }
    return delay_line;
}

void free_delay_line(delay_line_t *delay_line) {
    free(delay_line->samples);
    free(delay_line);
}

static float FRACTIONAL_SAMPLE(float step, const float *delay, int index_mask) {
    // Interpolated sample copied from oscillators.c:render_lut
    uint32_t base_index = (uint32_t)step;
    float frac = step - (float)base_index;
    float b = delay[(base_index + 0) & index_mask];
    float c = delay[(base_index + 1) & index_mask];
#ifdef LINEAR_INTERP
    // linear interpolation.
    float sample = b + ((c - b) * frac);
#else /* !LINEAR_INTERP => CUBIC_INTERP */
    float a = delay[(base_index - 1) & index_mask];
    float d = delay[(base_index + 2) & index_mask];
    // Miller's optimization - https://github.com/pure-data/pure-data/blob/master/src/d_array.c#L440
    float cminusb = c - b;
    float sample = b + frac * (cminusb - 0.1666667f * (1.0f-frac) * ((d - a - 3.0f * cminusb) * frac + (d + 2.0f*a - 3.0f*b)));
#endif /* LINEAR_INTERP */
    return sample;
}

void delay_line_in_out(float *in, float *out, int n_samples, float* mod_in, float mod_scale, delay_line_t *delay_line, float mix_level, float feedback_level) {
    // Read and write the next n_samples from/to the delay line.
    // mod_in is a per-sample modulation of the maximum delay, where 1 gives 
    // the max delay, -1 gives no delay, and 0 gives max_delay/2.
    // mod_scale is a constant scale factor applied to each value in mod_in, 
    // used e.g. to flip the sign of the delay.
    // Also supports input feedback from a non-modulated feedback delay output.
    int delay_len = delay_line->len;
    int index_mask = delay_len - 1; // will be all 1s because len is guaranteed 2**n.

    int index_in = delay_line->next_in;
    int index_feedback = (index_in - delay_line->fixed_delay) & index_mask;

    float *delay = delay_line->samples;
    float max_delay_on_two = delay_line->max_delay / 2.f;
    float scaled_mod_scale = max_delay_on_two * mod_scale;
    while(n_samples-- > 0) {
        float next_in = *in++ + feedback_level * delay[index_feedback++];
        index_feedback &= index_mask;

        float index_out = index_in - max_delay_on_two - scaled_mod_scale * *mod_in++;
        //if(index_out >= delay_len) index_out -= delay_len;
        if(index_out < 0) index_out += delay_len;
        float sample = FRACTIONAL_SAMPLE(index_out, delay, index_mask);
        *out++ += mix_level * sample;  // mix delayed + original.
        delay[index_in++] = next_in;
        index_in &= index_mask;
    }
    delay_line->next_in = index_in;
}

void delay_line_in_out_fixed_delay(float *in, float *out, int n_samples, float mod_val, delay_line_t *delay_line, float mix_level) {
    // Read and write the next n_samples from/to the delay line.
    // Simplified version of delay_line_in_out() that uses a single, fixed (but fractional)
    // mod_val for the whole block.
    int delay_len = delay_line->len;
    int index_mask = delay_len - 1; // will be all 1s because len is guaranteed 2**n.

    int index_in = delay_line->next_in;

    float *delay = delay_line->samples;
    float total_delay = delay_line->max_delay * (1.f + mod_val) / 2.f;
    while(n_samples-- > 0) {
        float next_in = *in++;

        float index_out = index_in - total_delay;
        //if(index_out >= delay_len) index_out -= delay_len;
        if(index_out < 0) index_out += delay_len;
        float sample = FRACTIONAL_SAMPLE(index_out, delay, index_mask);
        *out++ += mix_level * sample;  // mix delayed + original.
        delay[index_in++] = next_in;
        index_in &= index_mask;
    }
    delay_line->next_in = index_in;
}


void apply_variable_delay(float *block, delay_line_t *delay_line, float *delay_mod, float delay_scale, float mix_level, float feedback_level) {
    delay_line_in_out(block, block, BLOCK_SIZE, delay_mod, delay_scale, delay_line, mix_level, feedback_level);
}

void apply_fixed_delay(float *block, delay_line_t *delay_line, float delay_mod_val, float mix_level) {
    delay_line_in_out_fixed_delay(block, block, BLOCK_SIZE, delay_mod_val, delay_line, mix_level);
}


static inline float DEL_OUT(delay_line_t *delay_line) {
    int out_index =
        (delay_line->next_in - delay_line->fixed_delay) & (delay_line->len - 1);
    return delay_line->samples[out_index];
}

static inline void DEL_IN(delay_line_t *delay_line, float val) {
    delay_line->samples[delay_line->next_in++] = val;
    delay_line->next_in &= (delay_line->len - 1);
}

static inline float LPF(float samp, float state, float lpcoef, float lpgain, float gain) {
    // 1-pole lowpass filter (exponential smoothing).
    // Smoothing. lpcoef=1 => no smoothing; lpcoef=0.001 => much smoothing.
    state += lpcoef * (samp - state);
    // Cross-fade between smoothed and original.  lpgain=0 => all smoothed, 1 => all dry.
    return 0.5f * gain * (state + lpgain * (samp - state));
}

float f1state = 0, f2state = 0, f3state = 0, f4state = 0;

delay_line_t *delay_1 = NULL, *delay_2 = NULL, *delay_3 = NULL, *delay_4 = NULL;

float lpfcoef = 0.4;
float lpfgain = 0.5;
float liveness = 0.85;

void config_stereo_reverb(float a_liveness, float crossover_hz, float damping) {
    // liveness (0..1) controls how much energy is preserved (larger = longer reverb).
    liveness = a_liveness;
    // crossover_hz is 3dB point of 1-pole lowpass freq.
    lpfcoef = 6.2832f * crossover_hz / SAMPLE_RATE;
    if (lpfcoef > 1.f)  lpfcoef = 1.f;
    if (lpfcoef < 0.f)  lpfcoef = 0.f;
    lpfgain = damping;
}

// Delay 1 is 58.6435 ms
#define DELAY1SAMPS 2586
// Delay 2 is 69.4325 ms
#define DELAY2SAMPS 3062
// Delay 3 is 74.5234 ms
#define DELAY3SAMPS 3286
// Delay 4 is 86.1244 ms
#define DELAY4SAMPS 3798

// Power of 2 that encloses all the delays.
#define DELAY_POW2 4096

// Reverb delays go into SPIRAM.
#define DELAYRAM MALLOC_CAP_SPIRAM

void init_stereo_reverb(void) {
    if (delay_1 == NULL) {
        delay_1 = new_delay_line(DELAY_POW2, DELAY1SAMPS, DELAYRAM);
        delay_2 = new_delay_line(DELAY_POW2, DELAY2SAMPS, DELAYRAM);
        delay_3 = new_delay_line(DELAY_POW2, DELAY3SAMPS, DELAYRAM);
        delay_4 = new_delay_line(DELAY_POW2, DELAY4SAMPS, DELAYRAM);
        config_stereo_reverb(0.85f, 3000.0f, 0.5);
    }
}

void stereo_reverb(float *r_in, float *l_in, float *r_out, float *l_out, int n_samples, float level) {
    // Stereo reverb.  *{r,l}_in each point to n_samples input samples.
    // n_samples are written to {r,l}_out.
    // Recreate
    // https://github.com/duvtedudug/Pure-Data/blob/master/extra/rev2%7E.pd
    // an instance of the Stautner-Puckette multichannel reverberator from
    // https://www.ee.columbia.edu/~dpwe/e4896/papers/StautP82-reverb.pdf
    while(n_samples--) {
        float d1 = DEL_OUT(delay_1);
        d1 = LPF(d1, f1state, lpfcoef, lpfgain, liveness);
        d1 += *r_in;
        *r_out++ = *r_in++ + level * d1;

        float d2 = DEL_OUT(delay_2);
        d2 = LPF(d2, f2state, lpfcoef, lpfgain, liveness);
        if (l_in != NULL)  d2 += *l_in;
        if (l_out != NULL)  *l_out++ = *l_in++ + level * d2;

        float d3 = DEL_OUT(delay_3);
        d3 = LPF(d3, f3state, lpfcoef, lpfgain, liveness);

        float d4 = DEL_OUT(delay_4);
        d4 = LPF(d4, f3state, lpfcoef, lpfgain, liveness);

        // Mixing and feedback.
        DEL_IN(delay_1, d1 + d2 + d3 + d4);
        DEL_IN(delay_2, d1 - d2 + d3 - d4);
        DEL_IN(delay_3, d1 + d2 - d3 - d4);
        DEL_IN(delay_4, d1 - d2 - d3 + d4);
    }
}
