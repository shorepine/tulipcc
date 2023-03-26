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


delay_line_t *new_delay_line(int len, int feedback_delay) {
    // Check that len is a power of 2.
    if (!is_power_of_two(len)) { fprintf(stderr, "delay line len must be power of 2, not %d\n", len); abort(); }
    delay_line_t *delay_line = (delay_line_t*)malloc_caps(sizeof(delay_line_t), MALLOC_CAP_INTERNAL); 
    delay_line->samples = (float*)malloc_caps(len * sizeof(float), MALLOC_CAP_INTERNAL);
    delay_line->len = len;
    delay_line->feedback_delay = feedback_delay;
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
    // Read a block of samples out from the delay line.
    // "step" is a real-valued read-from sample index; "inc" is a real-valued step, so the resampling
    // can be non-constant delay.  Function returns the final value of step (to re-use in the next
    // block).
    int delay_len = delay_line->len;
    int index_mask = delay_len - 1; // will be all 1s because len is guaranteed 2**n.

    int index_in = delay_line->next_in;
    int index_feedback = (index_in - delay_line->feedback_delay) & index_mask;

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

void delay_line_in_out_fixed_delay(float *in, float *out, int n_samples, float mod_val, float mod_scale, delay_line_t *delay_line, float mix_level) {
    // Read a block of samples out from the delay line.
    // "step" is a real-valued read-from sample index; "inc" is a real-valued step, so the resampling
    // can be non-constant delay.  Function returns the final value of step (to re-use in the next
    // block).
    int delay_len = delay_line->len;
    int index_mask = delay_len - 1; // will be all 1s because len is guaranteed 2**n.

    int index_in = delay_line->next_in;

    float *delay = delay_line->samples;
    float total_delay = delay_line->max_delay * (1.f + mod_scale * mod_val) / 2.f;
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

void apply_fixed_delay(float *block, delay_line_t *delay_line, float delay_mod_val, float delay_scale, float mix_level) {
    delay_line_in_out_fixed_delay(block, block, BLOCK_SIZE, delay_mod_val, delay_scale, delay_line, mix_level);
}
