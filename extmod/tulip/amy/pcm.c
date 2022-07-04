// pcm.c

#include "amy.h"

#ifdef PCM_PATCHES_SIZE_LARGE
#include "pcm_samples_large.h"
#else
#include "pcm_samples_small.h"
#endif

typedef struct {
    uint32_t offset;
    uint32_t length;
    uint32_t loopstart;
    uint32_t loopend;
    uint8_t midinote;
} pcm_map_t;


#ifdef PCM_PATCHES_SIZE_LARGE
#include "pcm_large.h"
#else
#include "pcm_small.h"
#endif

void pcm_init() {
/*
    // For ESP, we mmap the PCM blob on the luts partition 
#ifdef ESP_PLATFORM
    spi_flash_mmap_handle_t mmap_handle;
    const esp_partition_t * pcm_part  = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS, "luts");
    esp_err_t err = esp_partition_mmap(pcm_part, 0, PCM_LENGTH*2, SPI_FLASH_MMAP_DATA, (const void**)&pcm, &mmap_handle);
    if(err != ESP_OK) {
        printf("err doing pcm mmap: %d %s\n", err, esp_err_to_name(err));
    }
#else
    pcm = pcm_desktop;
#endif
*/
}

void pcm_note_on(uint8_t osc) {
	// if no freq given, just play it at midinote
	if(synth[osc].patch<0) synth[osc].patch = 0;
	pcm_map_t patch = pcm_map[synth[osc].patch];
    if(synth[osc].freq <= 0) synth[osc].freq = PCM_SAMPLE_RATE; // / freq_for_midi_note(patch.midinote);
    synth[osc].step = (patch.offset/(float)PCM_SAMPLES); // normalized start sample
    // Use substep here as "end sample" so we don't have to add another field to the struct
    // and lpf_state for loopstart, and lpf_alpha for loopend.
    synth[osc].sample = patch.offset; // offset into table, needed as float32s don't have enough precision to index into big table
    synth[osc].step = 0; // start at the beginning of the sample
    synth[osc].substep = patch.length; // end sample
    synth[osc].lpf_state = patch.loopstart;
    synth[osc].lpf_alpha = patch.loopend;
}

void pcm_mod_trigger(uint8_t osc) {
    pcm_note_on(osc);
}

void pcm_note_off(uint8_t osc) {
    // if looping set, set loopend to the end of the sample, so it'll play through and die out
    if(msynth[osc].feedback > 0) {
        synth[osc].lpf_alpha = synth[osc].substep;
    } else {
        // just set step to the end
        synth[osc].step = synth[osc].substep;
    }
}

void render_pcm(float * buf, uint8_t osc) {
    pcm_map_t patch = pcm_map[synth[osc].patch];
    float playback_freq = PCM_SAMPLE_RATE;
    if(msynth[osc].freq < PCM_SAMPLE_RATE) { // user adjusted freq 
        float base_freq = freq_for_midi_note(patch.midinote); 
        playback_freq = (msynth[osc].freq / base_freq) * PCM_SAMPLE_RATE;
    }
    float skip = playback_freq / (float)SAMPLE_RATE;
    for(uint16_t i=0;i<BLOCK_SIZE;i++) {
        float float_index = synth[osc].step;
        uint32_t base_index = (uint32_t) float_index;
        float frac = float_index - (float)base_index;
        float b = (float)(pcm[base_index + (uint32_t)synth[osc].sample])/(float)SAMPLE_MAX;
        float c = b;
        if((base_index+1) < PCM_LENGTH) c = (float)(pcm[(base_index + 1) + (uint32_t)synth[osc].sample])/(float)SAMPLE_MAX;
        float sample = b + ((c - b) * frac);
        synth[osc].step = (synth[osc].step + skip);
        if(synth[osc].step >= synth[osc].substep ) { // end
            synth[osc].status=OFF;// is this right? 
            sample = 0;
        } else {
            if(msynth[osc].feedback > 0) { // loop       
                if(synth[osc].step > synth[osc].lpf_alpha) { // loopend
                    synth[osc].step = synth[osc].lpf_state; // back to loopstart
                }
            }
        }
        buf[i] = (sample * msynth[osc].amp);
    }

}

float compute_mod_pcm(uint8_t osc) {
    float mod_sr = (float)SAMPLE_RATE / (float)BLOCK_SIZE;
    float skip = msynth[osc].freq / mod_sr;
    float sample = pcm[(int)(synth[osc].step)];
    synth[osc].step = (synth[osc].step + skip);
    if(synth[osc].step >= synth[osc].substep ) { // end
        synth[osc].status=OFF;
        sample = 0;
    }
    return (sample * msynth[osc].amp) / SAMPLE_MAX; // -1 .. 1
    
}
