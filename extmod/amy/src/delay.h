#ifndef _DELAY_H

typedef struct delay_line {
    float *samples;
    int len;
    int next_in;
    float next_out;
} delay_line_t;


delay_line_t *new_delay_line(int len, float initial_delay);

void apply_variable_delay(float *block, delay_line_t *delay_line, float *delay_samples, float mod_scale);


#endif // !_DELAY_H
