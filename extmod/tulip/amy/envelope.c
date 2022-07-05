// envelope.c
// VCA -- handle modulation and ADSR

#include "amy.h"

extern struct event* synth;
extern struct mod_event* msynth;
extern struct mod_state mglobal;


// Breakpoints as/well (?) instead of adsr
// user defines BP as time(ms), ratio, time, ratio
// but also needs to ID a BP for key release 
// and each BP curve can be for any # of targets, there can be multiple
// (for now) the last BP is the release bp, default 0,0 (0ms after release, go to 0)

// how do i get aribitrary length BPs in alles?



// modulation scale is not like bp scale, it can also make a thing bigger, so return range is between -1 and 1, where 1 = 2x and 0 = 1x
float compute_mod_scale(uint8_t osc) {
    int8_t source = synth[osc].mod_source;
    if(synth[osc].mod_target >= 1 && source >= 0) {
        if(source != osc) {  // that would be weird
            msynth[source].amp = synth[source].amp;
            msynth[source].duty = synth[source].duty;
            msynth[source].freq = synth[source].freq;
            msynth[source].filter_freq = synth[source].filter_freq;
            msynth[source].feedback = synth[source].feedback;
            msynth[source].resonance = synth[source].resonance;
            if(synth[source].wave == NOISE) return compute_mod_noise(source);
            if(synth[source].wave == SAW_DOWN) return compute_mod_saw_down(source);
            if(synth[source].wave == SAW_UP) return compute_mod_saw_up(source);
            if(synth[source].wave == PULSE) return compute_mod_pulse(source);
            if(synth[source].wave == TRIANGLE) return compute_mod_triangle(source);
            if(synth[source].wave == SINE) return compute_mod_sine(source);
            if(synth[source].wave == PCM) return compute_mod_pcm(source);
        }
    }
    return 0; // 0 is no change, unlike bp scale
}
extern uint8_t debug_on;
float compute_breakpoint_scale(uint8_t osc, uint8_t bp_set) {
    // given a breakpoint list, compute the scale
    // we first see how many BPs are defined, and where we are in them?
    int8_t found = -1;
    int8_t release = 0;
    int32_t t1,t0;
    float v1,v0;
    int8_t bp_r = 0;
    t0 = 0; v0 = 1.0;
    float exponential_rate = 3.0;
    int64_t elapsed = 0;    

    // Find out which one is release (the last one)
    
    while(synth[osc].breakpoint_times[bp_set][bp_r] >= 0 && bp_r < MAX_BREAKPOINTS) bp_r++;
    bp_r--;
    //printf("[%d,%d] Found release BP at pos %d\n", bp_set, osc,bp_r);
    if(bp_r<0) {
        float scale = 1;
        //if(bp_set==0)printf("[%d,%d] returning scale=1\n",bp_set,osc); 
        if(synth[osc].note_off_clock >= 0) scale = 0;
	synth[osc].last_scale[bp_set] = scale;
        return scale; 
    }// no breakpoints

    // Find out which BP we're in
    if(synth[osc].note_on_clock >=0) {
        elapsed = (total_samples - synth[osc].note_on_clock) + 1; 
        for(uint8_t i=0;i<bp_r;i++) {
            if(elapsed < synth[osc].breakpoint_times[bp_set][i]) { found=i; i = bp_r; }
        }
        // We didn't find anything, so set it to the one before bp_r
        if(found<0) {
            found = bp_r - 1; // sustain
            //if(bp_set==0)printf("[%d,%d] elapsed %d. sustain, found %d returning %f\n", bp_set, osc, elapsed, found, synth[osc].breakpoint_values[bp_set][found] );
	    float scale = synth[osc].breakpoint_values[bp_set][found];
	    synth[osc].last_scale[bp_set] = scale;
	    return scale;
        }
        //printf("[%d,%d] elapsed %d. found %d at time %d\n", bp_set, osc, elapsed, found, synth[osc].breakpoint_times[bp_set][found] );

    } else if(synth[osc].note_off_clock >= 0) {
      if(debug_on)printf("%lld [%d,%d] last_scale=%f bp_r=%d v1=%f t1=%d\n", total_samples, bp_set, osc,
			 synth[osc].last_scale[bp_set], bp_r, synth[osc].breakpoint_values[bp_set][bp_r], synth[osc].breakpoint_times[bp_set][bp_r]);
        release = 1;
        elapsed = (total_samples - synth[osc].note_off_clock) + 1; 
        // Get the last t/v pair , for release
        found = bp_r;
        int8_t bp_rx = 0;
        t0 = 0; // start the elapsed clock again
        //if(found > 0) v0 = synth[osc].breakpoint_values[bp_set][found-1];
	// Release starts from wherever we got to
	v0 = synth[osc].last_scale[bp_set];
        if(debug_on)printf("elapsed %lld noc %lld total_samples %lld found %d v0 %f bpt %d\n", elapsed, synth[osc].note_off_clock, total_samples, found, v0, synth[osc].breakpoint_times[bp_set][bp_r]);
        if(elapsed > synth[osc].breakpoint_times[bp_set][bp_r]) {

            // are there multiple bp_sets? only turn off the note if it's the last one
            int32_t my_bt = synth[osc].breakpoint_times[bp_set][bp_r];
            // This is a mess but works, we could fix 
            for(uint8_t test_bp_set=0;test_bp_set<MAX_BREAKPOINT_SETS;test_bp_set++) {
                if(test_bp_set != bp_set) {
                    // Find the last bp in this bp set
                    bp_rx = 0; while(synth[osc].breakpoint_times[test_bp_set][bp_rx] >= 0 && bp_rx < MAX_BREAKPOINTS) bp_rx++; bp_rx--;
                    if(bp_rx >= 0) {
                        // If my breakpoint time is less than another breakpoint time from a different set, return 1.0 and don't end the note
		      if(my_bt < synth[osc].breakpoint_times[test_bp_set][bp_rx]) {
			float scale = 1;
			synth[osc].last_scale[bp_set] = scale;
			return scale;
		      }
		    }
                }
            }
            // OK. partials (et al) need a frame to fade out to avoid clicks. This is in conflict with the breakpoint release, 
            // which will set it to the bp end value before the fade out, often 0 so the fadeout never gets to hit. 
            // I'm not sure i love this solution, but PARTIAL is such a weird type that i guess having it called out like this is fine.
	    if(synth[osc].wave==PARTIAL) {
	      float scale = 1;
	      synth[osc].last_scale[bp_set] = scale;
	      return scale;
	    }
            synth[osc].status=OFF;
            synth[osc].note_off_clock = -1;
            float scale = synth[osc].breakpoint_values[bp_set][bp_r];
	    synth[osc].last_scale[bp_set] = scale;
	    return scale;
        }
    }

    t1 = synth[osc].breakpoint_times[bp_set][found]; 
    v1 = synth[osc].breakpoint_values[bp_set][found];
    if(found>0 && bp_r != found && !release) {
        t0 = synth[osc].breakpoint_times[bp_set][found-1];
        v0 = synth[osc].breakpoint_values[bp_set][found-1]; 
    }
    float scale = v0;
    if(t1 < 0 || v1 < 0) {
        scale = 0;
    } else if(t1==t0 || elapsed==t1) {
        // This way we return exact zero for v1 at the end of the segment, rather than BREAKPOINT_EPS
        scale = v1;
    } else {
	// OK, we are transition from v0 to v1 , and we're at elapsed time between t0 and t1
	float time_ratio = ((float)(elapsed - t0) / (float)(t1 - t0));
	// Compute scale based on which type we have
	if(synth[osc].breakpoint_target[bp_set] & TARGET_LINEAR) {
	    scale = v0 + ((v1-v0) * time_ratio);
	    if(debug_on)printf("%lld [%d,%d] LIN t0 %d v0 %f t1 %d v1 %f elapsed %lld tr %f scale %f\n", total_samples, bp_set, osc, t0, v0, t1, v1, elapsed, time_ratio, scale);
	} else if(synth[osc].breakpoint_target[bp_set] & TARGET_TRUE_EXPONENTIAL) {
	  v0 = MAX(v0, BREAKPOINT_EPS);
	  v1 = MAX(v1, BREAKPOINT_EPS);
	  float dx7_exponential_rate = -logf(v1/v0) / (t1 - t0);
	  scale = v0 * expf(-dx7_exponential_rate * (elapsed - t0)); 
	  if(debug_on)printf("%lld [%d,%d] TRUE t0 %d v0 %f t1 %d v1 %f elapsed %lld exprate %f scale %f \n", total_samples, bp_set, osc, t0, v0, t1, v1, elapsed, dx7_exponential_rate, scale);
	} else if(synth[osc].breakpoint_target[bp_set] & TARGET_DX7_EXPONENTIAL) {
	  // Somewhat complicated relationship, see https://colab.research.google.com/drive/1qZmOw4r24IDijUFlel_eSoWEf3L5VSok#scrollTo=F5zkeACrOlum
#define LINEAR_TO_DX7_LEVEL(linear) (MIN(99.0, logf(MAX(BREAKPOINT_EPS, linear)) * 8.0 + 99.0))
#define DX7_LEVEL_TO_LINEAR(level) (powf(2.0, (level - 99.0)/8.0))
#define MIN_LEVEL 34
#define ATTACK_RANGE 75
#define MAP_ATTACK_LEVEL(level) (1 - MAX(level - MIN_LEVEL, 0) / ATTACK_RANGE)
	  float mapped_current_level = MAP_ATTACK_LEVEL(LINEAR_TO_DX7_LEVEL(v0));
	  float mapped_target_level = MAP_ATTACK_LEVEL(LINEAR_TO_DX7_LEVEL(v1));
	  float t_const = (t1 - t0) / logf(mapped_current_level / mapped_target_level);
	  float my_t0 = -t_const * logf(mapped_current_level);
      if(debug_on) printf("mapped_current_level %f mapped_target_level %f t_const %f my_t0 %f\n", mapped_current_level, mapped_target_level, t_const, my_t0);
	  if (v1 > v0) {
            // This is the magic equation that shapes the DX7 attack envelopes.
            scale = DX7_LEVEL_TO_LINEAR(MIN_LEVEL + ATTACK_RANGE * (1 - expf(-(my_t0 + elapsed)/t_const)));
	    if(debug_on)printf("%lld [%d,%d] DX7 t0 %d v0 %f t1 %d v1 %f elapsed %lld t_const %f scale %f \n", total_samples, bp_set, osc, t0, v0, t1, v1, elapsed, t_const, scale);
	  } else {
	    // Decay is regular true_exponential
	    v0 = MAX(v0, BREAKPOINT_EPS);
	    v1 = MAX(v1, BREAKPOINT_EPS);
	    float dx7_exponential_rate = -logf(v1/v0) / (t1 - t0);
	    scale = v0 * expf(-dx7_exponential_rate * (elapsed - t0)); 
	  }
	} else { // "false exponential?"
	    if(debug_on)printf("target is %d\n",synth[osc].breakpoint_target[bp_set] & TARGET_TRUE_EXPONENTIAL );
	    scale = v0 + ((v1-v0) * (1.0 - expf(-exponential_rate*time_ratio)));
	    if(debug_on)printf("%lld [%d,%d] EXP t0 %d v0 %f t1 %d v1 %f elapsed %lld tr %f scale %f\n", total_samples, bp_set, osc, t0, v0, t1, v1, elapsed, time_ratio, scale);
	}
    }
    // Keep track of the most-recently returned non-release scale.
    if (!release) synth[osc].last_scale[bp_set] = scale;
    return scale;
}

// dpwe approved exp BP curves:
//def attack(t, attack):
//    return 1-exp(-3*(t/attack))
//def decay_and_sustain(t, attack, decay, S):
//    return S + (1-S)*exp(-(t - attack)/(decay / 3))
//def release(t, release, S):
//    return S*exp(-3 * t / release)
/*
float compute_adsr_scale(uint8_t osc) {
    // get the scale out of a osc
    float scale = 1.0; // the overall ratio to modify the thing
    int32_t t_a = synth[osc].adsr_a;
    int32_t t_d = synth[osc].adsr_d;
    float   S   = synth[osc].adsr_s;
    int32_t t_r = synth[osc].adsr_r;
    float curve = 3.0;
    if(synth[osc].adsr_on_clock >= 0) { 
        int64_t elapsed = (total_samples - synth[osc].adsr_on_clock) + 1; // +1 sample to avoid nans 
        if(elapsed > t_a) { // we're in sustain or decay
            scale = S + (1.0-S)*expf(-((float)elapsed - (float)t_a)/((float)t_d / curve));
        } else { // attack
            scale = 1.0 - expf(-curve * ((float)elapsed / (float)t_a));
        }
    } else if(synth[osc].adsr_off_clock >= 0) { // release
        int64_t elapsed = (total_samples - synth[osc].adsr_off_clock) + 1;
        scale = S * expf(-curve * (float)elapsed / (float)t_r);
        //printf("Release! osc %d t_r is %d S is %f elapsed is %d, so scale is %f\n", osc, t_r, S, elapsed, scale);
        if(elapsed > t_r) {
            // Turn off note
            //printf("Off! osc %d\n", osc);
            synth[osc].status=OFF;
            synth[osc].adsr_off_clock = -1;
        }
    }
    if(scale < 0) scale = 0;
    return scale;
}

*/
