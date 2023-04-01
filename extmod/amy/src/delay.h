#ifndef _DELAY_H

typedef struct delay_line {
    float *samples;
    int len;
    int fixed_delay;
    int next_in;
    float max_delay;
} delay_line_t;


delay_line_t *new_delay_line(int len, int fixed_delay, int ram_type /* e.g. MALLOC_CAP_INTERNAL */);
void free_delay_line(delay_line_t *d);

void apply_variable_delay(float *block, delay_line_t *delay_line, float *delay_samples, float mod_scale, float mix_level, float feedback_level);
void apply_fixed_delay(float *block, delay_line_t *delay_line, float delay_mod_val, float mix_level);

void config_stereo_reverb(float a_liveness, float crossover_hz, float damping);
void init_stereo_reverb(void);
void stereo_reverb(float *r_in, float *l_in, float *r_out, float *l_out, int n_samples, float level);

#endif // !_DELAY_H
