// partials.c
// deal with partials presets

#include "amy.h"


typedef struct {
    uint32_t bp_offset;
    uint32_t bp_length;
    uint8_t midi_note;
    uint32_t sustain_ms;
    uint8_t oscs_alloc;
} partial_breakpoint_map_t;

typedef struct {
    uint32_t ms_offset;
    uint8_t osc;
    float freq;
    float amp;
    float bw;
    float phase;
    uint16_t ms_delta;
    float amp_delta;
    float freq_delta;
    float bw_delta;
} partial_breakpoint_t;

// Automatically generated from partials.generate_partials_header()
#include "partials.h"




// choose a patch from the .h file
void partials_note_on(uint8_t osc) {
    // just like PCM, start & end breakpoint are stored here
    partial_breakpoint_map_t patch = partial_breakpoint_map[synth[osc].patch];
    synth[osc].step = patch.bp_offset;
    synth[osc].substep = synth[osc].step + patch.bp_length;
    // Now let's start the oscillators (silently)
    uint8_t oscs = patch.oscs_alloc;
    if(osc + 1 + oscs > OSCS) {
        printf("Asking for more oscs than you have -- starting %d, + 1 + %d more\n", osc, oscs);
    }
}

void partials_note_off(uint8_t osc) {
    // todo; finish the sustain
    synth[osc].step = -1;
}

// render a full partial set at offset osc (with patch)
// freq controls pitch_ratio, amp amp_ratio, ratio controls time ratio
// do all patches have sustain point?
void render_partials(float *buf, uint8_t osc) {
    partial_breakpoint_map_t patch = partial_breakpoint_map[synth[osc].patch % PARTIALS_PATCHES];
    // If ratio is set (not 0 or -1), use it for a time stretch
    float time_ratio = 1;
    if(synth[osc].ratio > 0) time_ratio = synth[osc].ratio;
    uint32_t ms_since_started = (((total_samples - synth[osc].note_on_clock) / (float)SAMPLE_RATE)*1000.0)*time_ratio;
    if(synth[osc].step >= 0) {
        // do we either have no sustain, or are we past sustain? 
        // TODO: sustain is likely more complicated --we want to bounce between the closest bps for loopstart & loopend
        uint32_t sustain_ms = patch.sustain_ms;
        if((sustain_ms > 0 && (ms_since_started < sustain_ms)) || sustain_ms == 0) {
            partial_breakpoint_t pb = partial_breakpoints[(uint32_t)synth[osc].step];
            if(ms_since_started >= pb.ms_offset ) {
                // set up this oscillator
                uint8_t o = (pb.osc + 1 + osc) % OSCS; // just in case
    
                #ifdef ESP_PLATFORM
                    if(o % 2) o = o + 32; // scale
                #endif
                //printf("[%d %d] new pb: %d %d %f %f %f\n", total_samples, ms_since_started, pb.osc, pb.ms_offset, pb.amp, pb.freq, pb.phase);

                // Find our ratio using the midi note of the analyzed partial
                float freq_ratio = msynth[osc].freq / freq_for_midi_note(patch.midi_note); 

                // All the types share these params or are overwritten
                synth[o].wave = PARTIAL;
                synth[o].status = IS_ALGO_SOURCE;
                synth[o].amp = pb.amp;
                synth[o].note_on_clock = total_samples; // start breakpoints
                synth[o].freq = pb.freq * freq_ratio;
                synth[o].feedback = pb.bw * msynth[osc].feedback;
                synth[o].phase = pb.phase; // this can be a negative # to indicate continuation or end

                synth[o].breakpoint_times[0][0] = ms_to_samples((int)((float)pb.ms_delta/time_ratio));
                synth[o].breakpoint_values[0][0] = pb.amp_delta; 
                synth[o].breakpoint_times[0][1] = 0; 
                synth[o].breakpoint_values[0][1] = 0; 
                synth[o].breakpoint_target[0] = TARGET_AMP + TARGET_LINEAR;

                synth[o].breakpoint_times[1][0] = ms_to_samples((int)((float)pb.ms_delta/time_ratio));
                synth[o].breakpoint_values[1][0] = pb.freq_delta; 
                synth[o].breakpoint_times[1][1] = 0; 
                synth[o].breakpoint_values[1][1] = 0; 
                synth[o].breakpoint_target[1] = TARGET_FREQ + TARGET_LINEAR;
                
                if(synth[o].feedback > 0) {
                    synth[o].breakpoint_times[2][0] = ms_to_samples((int)((float)pb.ms_delta/time_ratio));
                    synth[o].breakpoint_values[2][0] = pb.bw_delta; 
                    synth[o].breakpoint_times[2][1] = 0; 
                    synth[o].breakpoint_values[2][1] = 0; 
                    synth[o].breakpoint_target[2] = TARGET_FEEDBACK + TARGET_LINEAR;
                }
                if(synth[o].phase==-1) { // continuation 
                    synth[o].freq = pb.freq * freq_ratio;
                    synth[o].feedback = pb.bw * msynth[osc].feedback;
                    //printf("[%d %d] o %d continue partial\n", total_samples, ms_since_started, o);
                } else if(synth[o].phase==-2) { // end partial
                    partial_note_off(o);
                } else { // start of a partial, 
                    //printf("[%d %d] o %d start partial\n", total_samples,ms_since_started, o);
                    partial_note_on(o);
                }
                synth[osc].step++;
                if(synth[osc].step == synth[osc].substep) {
                    partials_note_off(osc);
                }
            }
        }
    }
    // now, render everything, add it up
    uint8_t oscs = patch.oscs_alloc;
    float pbuf[BLOCK_SIZE];
    for(uint8_t i=osc+1;i<osc+1+oscs;i++) {
        uint8_t o = i % OSCS;
        #ifdef ESP_PLATFORM
            if(o % 2) o = o + 32; // scale
        #endif
        if(synth[o].status ==IS_ALGO_SOURCE) {
            hold_and_modify(o);
            //printf("[%d %d] %d amp %f (%f) freq %f (%f) on %d off %d bp0 %d %f bp1 %d %f wave %d\n", total_samples, ms_since_started, o, synth[o].amp, msynth[o].amp, synth[o].freq, msynth[o].freq, synth[o].note_on_clock, synth[o].note_off_clock, synth[o].breakpoint_times[0][0], 
            //    synth[o].breakpoint_values[0][0], synth[o].breakpoint_times[1][0], synth[o].breakpoint_values[1][0], synth[o].wave);
            for(uint16_t j=0;j<BLOCK_SIZE;j++) pbuf[j] = 0;
            if(synth[o].wave==SINE) render_sine(pbuf, o);
            if(synth[o].wave==PARTIAL) render_partial(pbuf, o); 
            for(uint16_t j=0;j<BLOCK_SIZE;j++) buf[j] = buf[j] + (pbuf[j] * msynth[osc].amp);
        }
    }
}

