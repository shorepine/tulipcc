// custom_memory_pcm.c
// AMY custom oscillator that reads from tulip RAM
#include "amy.h"
#include "py/runtime.h"
#include "polyfills.h"

// Tulip-side functions
// A map of parameters for a in memory PCM sample. You can set SR per sample, and looping
typedef struct {
    int16_t * sample_ram;
    uint32_t length;
    uint32_t loopstart;
    uint32_t loopend;
    uint8_t midinote;
    uint32_t samplerate;
    float log2sr;
} memorypcm_map_t;

// list of pointers,alloced as needed
#define MAX_MEMORYPCM_PATCHES 32

memorypcm_map_t *memorypcm_map[MAX_MEMORYPCM_PATCHES];


uint8_t osc_patch_exists(uint16_t osc) {
    if(memorypcm_map[synth[osc].patch] != NULL) return 1;
    return 0;
}

// load mono samples (let python parse wave files) into patch # 
// set loopstart, loopend, midinote, samplerate (and log2sr)
int8_t memorypcm_load(mp_obj_t bytes, uint32_t samplerate, uint8_t midinote, uint32_t loopstart, uint32_t loopend) {
    // find the next free patch #
    int8_t patch = -1;
    for(uint8_t i=0;i<MAX_MEMORYPCM_PATCHES;i++) {
        if(memorypcm_map[i]==NULL) {
            patch = i;
            i = MAX_MEMORYPCM_PATCHES+1;
        }
    }
    if(patch<0) return patch;

    // alloc the map
    memorypcm_map[patch] = malloc_caps(sizeof(memorypcm_map_t), MALLOC_CAP_SPIRAM);
    memorypcm_map[patch]->samplerate = samplerate;
    memorypcm_map[patch]->log2sr = log2f((float)samplerate / ZERO_LOGFREQ_IN_HZ);
    memorypcm_map[patch]->midinote = midinote;
    memorypcm_map[patch]->loopstart = loopstart;
    memorypcm_map[patch]->loopend = loopend;
    // Grab the samples and len from bytes
    mp_buffer_info_t bufinfo;
    mp_get_buffer(bytes, &bufinfo, MP_BUFFER_READ);
    memorypcm_map[patch]->length = bufinfo.len / 2;
    // Alloc the buffer and copy to Tulip RAM. The python alloc'd one will go away in gc
    memorypcm_map[patch]->sample_ram = malloc_caps(bufinfo.len, MALLOC_CAP_SPIRAM);
    if(memorypcm_map[patch]->sample_ram  == NULL) return -1; // no ram 
    memcpy(memorypcm_map[patch]->sample_ram, (int16_t*)bufinfo.buf, bufinfo.len);
    return patch; // patch number 
}

void memorypcm_unload_patch(uint8_t patch) {
    if(memorypcm_map[patch] == NULL) return;
    free_caps(memorypcm_map[patch]->sample_ram);
    free_caps(memorypcm_map[patch]);
    memorypcm_map[patch] = NULL;
}

//free all patches
void memorypcm_unload() {
    for(uint8_t i=0;i<MAX_MEMORYPCM_PATCHES;i++) {
        memorypcm_unload_patch(i);
    }
}



// AMY-side functions

#ifndef PCM_INDEX_FRAC_BITS
// How many bits used for fractional part of PCM table index.
#define PCM_INDEX_FRAC_BITS 15
#endif

#ifndef PCM_INDEX_BITS
// The number of bits used to hold the table index.
#define PCM_INDEX_BITS (31 - PCM_INDEX_FRAC_BITS)
#endif

void memorypcm_init(void) {
    printf("memorypcm init\n");
}

void memorypcm_note_on(uint16_t osc, float freq) {
    if(osc_patch_exists(osc)) {
        // if no freq given, just play it at midinote
        if(synth[osc].logfreq_coefs[0] <= 0) {
            synth[osc].logfreq_coefs[0] = memorypcm_map[synth[osc].patch]->log2sr - logfreq_for_midi_note(memorypcm_map[synth[osc].patch]->midinote);
        }
        synth[osc].phase = 0; // s16.15 index into the table; as if a PHASOR into a 16 bit sample table. 
    }
}

void memorypcm_note_off(uint16_t osc) {
    if(osc_patch_exists(osc)) {
        // if looping set, disable looping; sample should play through to end.
        if(msynth[osc].feedback > 0) {
            msynth[osc].feedback = 0;
        } else {
            // Set phase to the end to cause immediate stop.
            synth[osc].phase = F2P(memorypcm_map[synth[osc].patch]->length / (float)(1 << PCM_INDEX_FRAC_BITS));
        }
    }
}

void memorypcm_mod_trigger(uint16_t osc) {
    memorypcm_note_on(osc, 0);
}

SAMPLE memorypcm_render(SAMPLE* buf, uint16_t osc) {
    if(osc_patch_exists(osc)) {
        // Patches can be > 32768 samples long.
        // We need s16.15 fixed-point indexing.
        memorypcm_map_t* patch = memorypcm_map[synth[osc].patch];
        float logfreq = msynth[osc].logfreq;
        // If osc[midi_note] is unset, apply patch's default here.
        if (!AMY_IS_SET(synth[osc].midi_note))  logfreq += logfreq_for_midi_note(patch->midinote);
        float playback_freq = freq_of_logfreq(logfreq);  // PCM_SAMPLE_RATE modified by

        SAMPLE max_value = 0;
        SAMPLE amp = F2S(msynth[osc].amp);
        PHASOR step = F2P((playback_freq / (float)AMY_SAMPLE_RATE) / (float)(1 << PCM_INDEX_BITS));
        const LUTSAMPLE* table = patch->sample_ram;
        uint32_t base_index = INT_OF_P(synth[osc].phase, PCM_INDEX_BITS);
        for(uint16_t i=0; i < AMY_BLOCK_SIZE; i++) {
            SAMPLE frac = S_FRAC_OF_P(synth[osc].phase, PCM_INDEX_BITS);
            LUTSAMPLE b = table[base_index];
            LUTSAMPLE c = b;
            if (base_index < patch->length) c = table[base_index + 1];
            SAMPLE sample = L2S(b) + MUL0_SS(L2S(c - b), frac);
            synth[osc].phase = P_WRAPPED_SUM(synth[osc].phase, step);
            base_index = INT_OF_P(synth[osc].phase, PCM_INDEX_BITS);
            if(base_index >= patch->length) { // end
                synth[osc].status = STATUS_OFF;// is this right? 
                sample = 0;
            } else {
                if(msynth[osc].feedback > 0) { // loop       
                    if(base_index >= patch->loopend) { // loopend
                        // back to loopstart
                        int32_t loop_len = patch->loopend - patch->loopstart;
                        synth[osc].phase -= F2P(loop_len / (float)(1 << PCM_INDEX_FRAC_BITS));
                        base_index -= loop_len;
                    }
                }
            }
            SAMPLE value = buf[i] + MUL4_SS(amp, sample);
            buf[i] = value;
            if (value < 0) value = -value;
            if (value > max_value) max_value = value;        
        }
        return max_value;
    }
    return 0; // no patch here
}

SAMPLE memorypcm_compute_mod(uint16_t osc) {
    if(osc_patch_exists(osc)) {
        float mod_sr = (float)AMY_SAMPLE_RATE / (float)AMY_BLOCK_SIZE;
        memorypcm_map_t* patch = memorypcm_map[synth[osc].patch];
        PHASOR step = F2P(((float)patch->samplerate / mod_sr) / (1 << PCM_INDEX_BITS));
        const LUTSAMPLE* table = patch->sample_ram;
        uint32_t base_index = INT_OF_P(synth[osc].phase, PCM_INDEX_BITS);
        SAMPLE sample;
        if(base_index >= patch->length) { // end
            synth[osc].status = STATUS_OFF;// is this right? 
            sample = 0;
        } else {
            sample = L2S(table[base_index]);
            synth[osc].phase = P_WRAPPED_SUM(synth[osc].phase, step);
        }
        return MUL4_SS(F2S(msynth[osc].amp), sample);
    }
    return 0; // no patch 
}

struct custom_oscillator memorypcm = {
    memorypcm_init,
    memorypcm_note_on,
    memorypcm_note_off,
    memorypcm_mod_trigger,
    memorypcm_render,
    memorypcm_compute_mod
};


