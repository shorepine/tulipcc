#ifndef _DELAY_H

typedef struct delay_line {
    float *samples;
    int len;
    int feedback_delay;
    int next_in;
    float max_delay;
} delay_line_t;


delay_line_t *new_delay_line(int len, int feedback_delay);
void free_delay_line(delay_line_t *d);

void apply_variable_delay(float *block, delay_line_t *delay_line, float *delay_samples, float mod_scale, float mix_level, float feedback_level);


#endif // !_DELAY_H
