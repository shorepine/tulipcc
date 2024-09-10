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
    if(AMY_IS_UNSET(synth[osc].patch)) return 0;
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
    // Grab the samples and len from bytes
    mp_buffer_info_t bufinfo;
    mp_get_buffer(bytes, &bufinfo, MP_BUFFER_READ);
    memorypcm_map[patch]->length = bufinfo.len / 2;
    // Alloc the buffer and copy to Tulip RAM. The python alloc'd one will go away in gc
    //fprintf(stderr, "samplerate %d midinote %d, loopstart %d loopend %d length %lu\n", samplerate, midinote, loopstart, loopend, bufinfo.len/2);
    memorypcm_map[patch]->sample_ram = malloc_caps(bufinfo.len, MALLOC_CAP_SPIRAM);
    if(memorypcm_map[patch]->sample_ram  == NULL) {
        free_caps(memorypcm_map[patch]);
        return -1; // no ram for sample
    }

    if(loopend == 0) {  // loop whole sample
        memorypcm_map[patch]->loopend = memorypcm_map[patch]->length-1;
    } else {
        memorypcm_map[patch]->loopend = loopend;        
    }
    memcpy(memorypcm_map[patch]->sample_ram, bufinfo.buf, bufinfo.len);
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
#define PCM_INDEX_FRAC_BITS 8
#endif

#ifndef PCM_INDEX_BITS
// The number of bits used to hold the table index.
#define PCM_INDEX_BITS (31 - PCM_INDEX_FRAC_BITS)
#endif

void memorypcm_init(void) {
    for(uint8_t i=0;i<MAX_MEMORYPCM_PATCHES;i++) {
        memorypcm_map[i] = NULL;
    }   
}

void memorypcm_note_on(uint16_t osc, float freq) {
    if(osc_patch_exists(osc)) {
        // if no freq given, just play it at midinote
        if(synth[osc].logfreq_coefs[COEF_CONST] <= 0) {
            // This will result in PCM_SAMPLE_RATE when the midi_note == patch->midinote.
            synth[osc].logfreq_coefs[COEF_CONST] = memorypcm_map[synth[osc].patch]->log2sr - logfreq_for_midi_note(memorypcm_map[synth[osc].patch]->midinote);
        }
        synth[osc].phase = 0; // s16.15 index into the table; as if a PHASOR into a 16 bit sample table.
        // Special case: We use the msynth feedback flag to indicate note-off for looping PCM.  As a result, it's explicitly NOT set in amy:hold_and_modify for PCM voices.  Set it here.
        msynth[osc].feedback = synth[osc].feedback;
    }
}


void memorypcm_note_off(uint16_t osc) {
    if(osc_patch_exists(osc)) {
        if(msynth[osc].feedback == 0) {
            // Non-looping note: Set phase to the end to cause immediate stop.
            synth[osc].phase = F2P(memorypcm_map[synth[osc].patch]->length / (float)(1 << PCM_INDEX_FRAC_BITS));
        } else {
            // Looping is requested, disable future looping, sample will play through to end.
            // (sending a second note-off will stop it immediately).
            msynth[osc].feedback = 0;
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
        if (AMY_IS_UNSET(synth[osc].midi_note))  logfreq += logfreq_for_midi_note(patch->midinote);
        float playback_freq = freq_of_logfreq(logfreq);  // PCM_SAMPLE_RATE modified by

        SAMPLE max_value = 0;
        SAMPLE amp = F2S(msynth[osc].amp);
        PHASOR step = F2P((playback_freq / (float)AMY_SAMPLE_RATE) / (float)(1 << PCM_INDEX_BITS));
        const LUTSAMPLE* table = patch->sample_ram;
        uint32_t base_index = INT_OF_P(synth[osc].phase, PCM_INDEX_BITS);
        //fprintf(stderr, "render_pcm: time=%.3f patch=%d base_index=%d length=%d loopstart=%d loopend=%d fb=%f is_unset_note_off %d\n", total_samples / (float)AMY_SAMPLE_RATE, synth[osc].patch, base_index, patch->length, patch->loopstart, patch->loopend, msynth[osc].feedback, AMY_IS_UNSET(synth[osc].note_off_clock));
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
                if(msynth[osc].feedback > 0) { // still looping.  The feedback flag is cleared by pcm_note_off.
                    if(base_index >= patch->loopend) { // loopend
                        // back to loopstart
                        int32_t loop_len = patch->loopend - patch->loopstart;
                        synth[osc].phase -= F2P(loop_len / (float)(1 << PCM_INDEX_BITS));
                        base_index -= loop_len;
                    }
                }
            }
            SAMPLE value = buf[i] + MUL4_SS(amp, sample);
            buf[i] = value;
            if (value < 0) value = -value;
            if (value > max_value) max_value = value;        
        }
        //printf("render_pcm: osc %d patch %d len %d base_ix %d phase %f step %f tablestep %f amp %f\n",
        //       osc, synth[osc].patch, patch->length, base_index, P2F(synth[osc].phase), P2F(step), (1 << PCM_INDEX_BITS) * P2F(step), S2F(msynth[osc].amp));
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


