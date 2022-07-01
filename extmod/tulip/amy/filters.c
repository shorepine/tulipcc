#include "amy.h"


// Filters tend to get weird under this ratio -- this corresponds to 4.4Hz 
#define LOWEST_RATIO 0.0001

float coeffs[OSCS][5];
float delay[OSCS][2];

float eq_coeffs[3][5];
float eq_delay[3][2];


float dsps_sqrtf_f32_ansi(float f)
{
    int* f_ptr = (int*)&f;
    const int result = 0x1fbb4000 + (*f_ptr >> 1);
    float* f_result = (float*)&result;
    return *f_result;   
}

int8_t dsps_biquad_gen_lpf_f32(float *coeffs, float f, float qFactor)
{
    if (qFactor <= 0.0001) {
        qFactor = 0.0001;
    }
    float Fs = 1;

    float w0 = 2 * M_PI * f / Fs;
    float c = cosf(w0);
    float s = sinf(w0);
    float alpha = s / (2 * qFactor);

    float b0 = (1 - c) / 2;
    float b1 = 1 - c;
    float b2 = b0;
    float a0 = 1 + alpha;
    float a1 = -2 * c;
    float a2 = 1 - alpha;

    coeffs[0] = b0 / a0;
    coeffs[1] = b1 / a0;
    coeffs[2] = b2 / a0;
    coeffs[3] = a1 / a0;
    coeffs[4] = a2 / a0;
    return 0;
}

int8_t dsps_biquad_gen_hpf_f32(float *coeffs, float f, float qFactor)
{
    if (qFactor <= 0.0001) {
        qFactor = 0.0001;
    }
    float Fs = 1;

    float w0 = 2 * M_PI * f / Fs;
    float c = cosf(w0);
    float s = sinf(w0);
    float alpha = s / (2 * qFactor);

    float b0 = (1 + c) / 2;
    float b1 = -(1 + c);
    float b2 = b0;
    float a0 = 1 + alpha;
    float a1 = -2 * c;
    float a2 = 1 - alpha;

    coeffs[0] = b0 / a0;
    coeffs[1] = b1 / a0;
    coeffs[2] = b2 / a0;
    coeffs[3] = a1 / a0;
    coeffs[4] = a2 / a0;
    return 0;
}

int8_t dsps_biquad_gen_bpf_f32(float *coeffs, float f, float qFactor)
{
    if (qFactor <= 0.0001) {
        qFactor = 0.0001;
    }
    float Fs = 1;

    float w0 = 2 * M_PI * f / Fs;
    float c = cosf(w0);
    float s = sinf(w0);
    float alpha = s / (2 * qFactor);

    float b0 = s / 2;
    float b1 = 0;
    float b2 = -b0;
    float a0 = 1 + alpha;
    float a1 = -2 * c;
    float a2 = 1 - alpha;

    coeffs[0] = b0 / a0;
    coeffs[1] = b1 / a0;
    coeffs[2] = b2 / a0;
    coeffs[3] = a1 / a0;
    coeffs[4] = a2 / a0;
    return 0;
}


int8_t dsps_biquad_f32_ansi(const float *input, float *output, int len, float *coef, float *w) {
    for (int i = 0 ; i < len ; i++) {
        float d0 = input[i] - coef[3] * w[0] - coef[4] * w[1];
        output[i] = coef[0] * d0 +  coef[1] * w[0] + coef[2] * w[1];
        w[1] = w[0];
        w[0] = d0;
    }
    return 0;
}

void update_filter(uint8_t osc) {
    // reset the delay for a filter
    // normal mod / adsr will just change the coeffs
    delay[osc][0] = 0; delay[osc][1] = 0;
}

void filters_init() {
    // update the parametric filters 
    dsps_biquad_gen_lpf_f32(eq_coeffs[0], EQ_CENTER_LOW /(float)SAMPLE_RATE, 0.707);
    dsps_biquad_gen_bpf_f32(eq_coeffs[1], EQ_CENTER_MED /(float)SAMPLE_RATE, 1.000);
    dsps_biquad_gen_hpf_f32(eq_coeffs[2], EQ_CENTER_HIGH/(float)SAMPLE_RATE, 0.707);
    for(uint8_t i=0;i<OSCS;i++) { delay[i][0] = 0; delay[i][1] = 0; }
    eq_delay[0][0] = 0; eq_delay[0][1] = 0;
    eq_delay[1][0] = 0; eq_delay[1][1] = 0;
    eq_delay[2][0] = 0; eq_delay[2][1] = 0;
}


void parametric_eq_process(float *block) {
    float output[3][BLOCK_SIZE];
#ifdef ESP_PLATFORM
    dsps_biquad_f32_ae32(block, output[0], BLOCK_SIZE, eq_coeffs[0], eq_delay[0]);
    dsps_biquad_f32_ae32(block, output[1], BLOCK_SIZE, eq_coeffs[1], eq_delay[1]);
    dsps_biquad_f32_ae32(block, output[2], BLOCK_SIZE, eq_coeffs[2], eq_delay[2]);
#else
    dsps_biquad_f32_ansi(block, output[0], BLOCK_SIZE, eq_coeffs[0], eq_delay[0]);
    dsps_biquad_f32_ansi(block, output[1], BLOCK_SIZE, eq_coeffs[1], eq_delay[1]);
    dsps_biquad_f32_ansi(block, output[2], BLOCK_SIZE, eq_coeffs[2], eq_delay[2]);
#endif
    for(uint16_t i=0;i<BLOCK_SIZE;i++)
        block[i] = (output[0][i] * global.eq[0]) - (output[1][i] * global.eq[1]) + (output[2][i] * global.eq[2]);
}



void filter_process(float * block, uint8_t osc) {
    float output[BLOCK_SIZE];
    float ratio = msynth[osc].filter_freq/(float)SAMPLE_RATE;
    if(ratio < LOWEST_RATIO) ratio = LOWEST_RATIO;
    if(synth[osc].filter_type==FILTER_LPF) dsps_biquad_gen_lpf_f32(coeffs[osc], ratio, msynth[osc].resonance);
    if(synth[osc].filter_type==FILTER_BPF) dsps_biquad_gen_bpf_f32(coeffs[osc], ratio, msynth[osc].resonance);
    if(synth[osc].filter_type==FILTER_HPF) dsps_biquad_gen_hpf_f32(coeffs[osc], ratio, msynth[osc].resonance);
#ifdef ESP_PLATFORM
    dsps_biquad_f32_ae32(block, output, BLOCK_SIZE, coeffs[osc], delay[osc]);
#else
    dsps_biquad_f32_ansi(block, output, BLOCK_SIZE, coeffs[osc], delay[osc]);
#endif
    for(uint16_t i=0;i<BLOCK_SIZE;i++) {
        block[i] = output[i];
    }
}

void filters_deinit() {
}

