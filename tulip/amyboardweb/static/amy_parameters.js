// Define all the knobs and their parameters here
window.addEventListener("DOMContentLoaded", function() {
  window.amy_sections = [
    { name: "Osc A", bg_color: "rgba(240, 180, 180, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "Osc B", bg_color: "rgba(240, 180, 180, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "LFO", bg_color: "rgba(240, 200, 180, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "VCF", bg_color: "rgba(150, 190, 240, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "VCF ENV", bg_color: "rgba(150, 190, 240, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "ADSR", bg_color: "rgba(150, 220, 240, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "EQ", bg_color: "rgba(150, 190, 240, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "Chorus", bg_color: "rgba(180, 225, 225, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
    { name: "Reverb", bg_color: "rgba(160, 200, 200, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
  ];

  window.amy_knobs = [
    {
      section: "Osc A",
      cc: 70,
      display_name: "freq",
      change_code: "i%iv0f%v",
      knob_type: "log",
      default_value: 261.63,
      min_value: 50,
      max_value: 2000,
    },
    {
      section: "Osc A",
      cc: 71,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv0w%v",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
    },
    {
      section: "Osc A",
      cc: 72,
      display_name: "duty",
      change_code: "i%iv0d%v",
      default_value: 0.5,
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc A",
      cc: 73,
      display_name: "level",
      change_code: "i%iv0a%v",
      knob_type: "log",
      default_value: 1.0,
      min_value: 0.001,
      max_value: 1.0,
      offset: 0.1,
    },
    { knob_type: "spacer" },
    { knob_type: "spacer-half" },
  
  {
      section: "VCF",
      cc: 74,
      display_name: "freq",
      change_code: "i%iF%v",
      knob_type: "log",
      default_value: 1000,
      min_value: 20,
      max_value: 8000,
    },
    {
      section: "VCF",
      cc: 75,
      display_name: "resonance",
      change_code: "i%iR%v",
      knob_type: "log",
      default_value: 0.7,
      min_value: 0.5,
      max_value: 16,
    },
    {
      section: "VCF",
      cc: 76,
      display_name: "kbd",
      change_code: "i%iF,%v",
      default_value: 1.0,
      min_value: 0,
      max_value: 1,
    },
    {
      section: "VCF",
      cc: 77,
      display_name: "env",
      change_code: "i%iF,,,,%v",
      default_value: 4.0,
      min_value: -10,
      max_value: 10,
    },

    { knob_type: "spacer-half" },
    {
      section: "EQ",
      cc: 78,
      display_name: "low",
      change_code: "x%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
    },
    {
      section: "EQ",
      cc: 79,
      display_name: "mid",
      change_code: "x,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
    },
    {
      section: "EQ",
      cc: 80,
      display_name: "high",
      change_code: "x,,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
    },

    {
      section: "Osc B",
      cc: 81,
      display_name: "freq",
      change_code: "i%iv1f%v",
      knob_type: "log",
      default_value: 261.63,
      min_value: 50,
      max_value: 2000,
    },
    {
      section: "Osc B",
      cc: 82,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv1w%v",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
    },
    {
      section: "Osc B",
      cc: 83,
      display_name: "duty",
      change_code: "i%iv1d%v",
      default_value: 0.5,
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc B",
      cc: 84,
      display_name: "level",
      change_code: "i%iv1a%v",
      knob_type: "log",
      default_value: 1.0,
      min_value: 0.001,
      max_value: 1.0,
      offset: 0.1,
    },
    { knob_type: "spacer" },
    { knob_type: "spacer-half" },

   {
      section: "VCF ENV",
      cc: 85,
      display_name: "attack",
      change_code: "i%iB%v,1,,,,0",
      default_value: 0,
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "VCF ENV",
      cc: 86,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iB,1,%v,,,0",
      default_value: 100,
      offset: 50,
      min_value: 0,
      max_value: 2000,
    },
    {
      section: "VCF ENV",
      cc: 87,
      display_name: "sustain",
      change_code: "i%iB,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
    },
    {
      section: "VCF ENV",
      cc: 88,
      knob_type: "log",
      display_name: "release",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
      change_code: "i%iB,1,,,%v,0",
    },  
    {knob_type: "spacer-half" },
    {
      section: "Chorus",
      cc: 89,
      knob_type: "log",
      display_name: "level",
      change_code: "k%v",
      min_value: 0,
      max_value: 1,
      offset: 0.1,
      default_value: 0,
    },
    {
      section: "Chorus",
      cc: 90,
      knob_type: "log",
      display_name: "freq",
      change_code: "k,,%v",
      default_value: 4,
      min_value: 0.1,
      max_value: 20,
    },
    {
      section: "Chorus",
      cc: 91,
      display_name: "depth",
      change_code: "k,,,%v",
      min_value: 0.01,
      max_value: 1,
      default_value: 0.5,
    },
   

    {
      section: "LFO",
      cc: 92,
      display_name: "freq",
      change_code: "i%iv2f%v",
      knob_type: "log",
      default_value: 4,
      min_value: 0.1,
      max_value: 20,
    },
    {
      section: "LFO",
      cc: 93,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv2w%v",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
    },
    {
      section: "LFO",
      cc: 94,
      display_name: "osc",
      change_code: "i%iv0f,,,,,%v",
      knob_type: "log",
      default_value: 0.0,
      min_value: 0.0,
      max_value: 4.0,
      offset: 0.001,
    },
    {
      section: "LFO",
      cc: 95,
      display_name: "pwm",
      change_code: "i%iv0d,,,,,%v",
      default_value: 0.0,
      min_value: 0.0,
      max_value: 0.49,
    },
    {
      section: "LFO",
      cc: 96,
      display_name: "filt",
      change_code: "i%iv0F,,,,,%v",
      default_value: 0.0,
      min_value: 0.0,
      max_value: 4.0,
      offset: 0.001,
    },
        {knob_type: "spacer-half" },

          {
      section: "ADSR",
      cc: 97,
      display_name: "attack",
      change_code: "i%iv0A%v,1,,,,0",
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "ADSR",
      cc: 98,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iv0A,1,%v,,,0",
      offset: 50,
      min_value: 0,
      max_value: 2000,
      default_value: 100,
    },
    {
      section: "ADSR",
      cc: 99,
      display_name: "sustain",
      change_code: "i%iv0A,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
    },
    {
      section: "ADSR",
      cc: 100,
      knob_type: "log",
      display_name: "release",
      change_code: "i%iv0A,1,,,%v,0",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
    },
  
        {knob_type: "spacer-half" },


    {
      section: "Reverb",
      cc: 101,
      knob_type: "log",
      display_name: "level",
      change_code: "h%v",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      offset: 0.1,
    },
    {
      section: "Reverb",
      cc: 102,
      display_name: "live",
      change_code: "h,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.5,
    },
    {
      section: "Reverb",
      cc: 103,
      display_name: "damp",
      change_code: "h,,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.5,
    },

  ];

  if (typeof init_knobs === "function") {
    init_knobs(window.amy_knobs);
  }
  if (typeof init_patches_select === "function") {
    init_patches_select("patch-select");
  }
});


function set_knobs_from_patch_number(patch_number) {
  amy_add_message("i1iv6K257");  // The amyboardsynth base patch - 3 oscs, bp2 for filter.
  const events = get_events_for_patch_number(patch_number);
  if (!Array.isArray(window.amy_knobs) || events.length === 0) {
    return;
  }

  let filterFreq = null;
  let filterEnv = null;
  let filterLfo = null
  let filterKbd = null;
  let resonanceValue = null;
  let adsr = null;
  let lfoFreq = null;
  let lfoDelay = null;
  let lfoOsc = 0;
  let lfoPwm = 0;
  // Track coefficients for all 4 non-lfo oscs.
  let osc_freq = [null, null, null, null];
  let osc_wave = [null, null, null, null];
  let osc_duty = [null, null, null, null];
  let osc_gain = [null, null, null, null];
  // Which Juno oscs are used for oscA and B
  let oscAB_osc = [-1, -1];
  let oscAB_gain = [0, 0];
  // Do the oscs use ADSR, or the juno "gate"?
  let oscGate = 0;
  let eq = [null, null, null];
  let chorus = [null, null, null];
  
  for (const event of events) {
    if (event.filter_freq_coefs.some((value) => Number.isFinite(value))) {
      filterFreq = event.filter_freq_coefs[0];
      filterKbd = event.filter_freq_coefs[1];  // COEF_NOTE == 1
      filterEnv = event.filter_freq_coefs[3];  // COEF_EG0 == 3
      filterLfo = event.filter_freq_coefs[5];  // COEF_MOD == 5
    }
    if (Number.isFinite(event.resonance)) {
      resonanceValue = event.resonance;
    }
    if (Number.isFinite(event.eq_l))  { eq[0] = event.eq_l; }
    if (Number.isFinite(event.eq_m))  { eq[1] = event.eq_m; }
    if (Number.isFinite(event.eq_h))  { eq[2] = event.eq_h; }
    if (Number.isFinite(event.chorus_level)) { chorus[0] = event.chorus_level; }
    if (Number.isFinite(event.chorus_lfo_freq)) { chorus[1] = event.chorus_lfo_freq; }
    if (Number.isFinite(event.chorus_depth)) { chorus[2] = event.chorus_depth; }
    if (event.osc == 4) {
      // LFO
      if (Number.isFinite(event.freq_coefs[0])) {
        lfoFreq = event.freq_coefs[0];
      }
      if (event.bp0) {
        lfoDelay = parseFloat(event.bp0.split(",")[0]);
      }
    } else if (event.osc >= 0 && event.osc < 4) {
      // Non-LFO osc, don't assume what order they come in.
      if (!adsr && event.bp0) {
        // split the comma separated bp0 values and check if any are finite numbers
        // values is time,value pairs.
        const values = event.bp0.split(",").map(parseFloat);
        adsr = [values[0], values[2], values[3], values[4]];  // times[0], times[1], level[1], times[2]
      }
      // Extract key parameters for each osc
      if (Number.isFinite(event.amp_coefs[2]) && event.amp_coefs[2] > 0) {
        osc_gain[event.osc] = event.amp_coefs[2];
        // Also, is the amp from ADSR or "gate", indicated by COEF_EG0 == 0.
        oscGate = (event.amp_coefs[3] == 0)? 1 : 0;
      }
      if (Number.isFinite(event.freq_coefs[0]) && event.freq_coefs[0] > 0) {
        osc_freq[event.osc] = event.freq_coefs[0];
        lfoOsc = event.freq_coefs[5];  // COEF_MOD == 5 always holds LFO coef.
      }
      if (Number.isFinite(event.duty_coefs[0]) && event.duty_coefs[0] > 0) {
        osc_duty[event.osc] = event.duty_coefs[0];
        if (Number.isFinite(event.duty_coefs[5]) && event.duty_coefs[5] > lfoPwm) {
          lfoPwm = event.duty_coefs[5];  // COEF_MOD == 5 always holds LFO coef.
        }
      }
      if (event.wave >= 0 && event.wave < 127) {
        osc_wave[event.osc] = event.wave
      }
    }
  }
  // Logic to choose juno oscs for osc A and osc B.
  for (let osc = 0; osc < 4; ++osc) {
    if (oscAB_osc[0] == -1 || osc_gain[osc] > osc_gain[oscAB_osc[0]]) {
      if (oscAB_osc[1] == -1 || osc_gain[oscAB_osc[0]] > osc_gain[oscAB_osc[1]]) {
        // Push oscA into oscB
        oscAB_osc[1] = oscAB_osc[0];
      }
      oscAB_osc[0] = osc;
    } else if (oscAB_osc[1] == -1 || osc_gain[osc] > osc_gain[oscAB_osc[1]]) {
      oscAB_osc[1] = osc;
    }
  }

  // Configure the patch.
  set_amy_knob_value(window.amy_knobs, "Osc A", "freq", osc_freq[oscAB_osc[0]]);
  set_amy_knob_value(window.amy_knobs, "Osc A", "wave", osc_wave[oscAB_osc[0]]);
  set_amy_knob_value(window.amy_knobs, "Osc A", "duty", osc_duty[oscAB_osc[0]]);
  set_amy_knob_value(window.amy_knobs, "Osc A", "level", osc_gain[oscAB_osc[0]]);

  set_amy_knob_value(window.amy_knobs, "Osc B", "freq", osc_freq[oscAB_osc[1]]);
  set_amy_knob_value(window.amy_knobs, "Osc B", "wave", osc_wave[oscAB_osc[1]]);
  set_amy_knob_value(window.amy_knobs, "Osc B", "duty", osc_duty[oscAB_osc[1]]);
  set_amy_knob_value(window.amy_knobs, "Osc B", "level", osc_gain[oscAB_osc[1]]);

  set_amy_knob_value(window.amy_knobs, "VCF", "freq", filterFreq);
  set_amy_knob_value(window.amy_knobs, "VCF", "resonance", resonanceValue);
  set_amy_knob_value(window.amy_knobs, "VCF", "kbd", filterKbd);
  set_amy_knob_value(window.amy_knobs, "VCF", "env", filterEnv);
  set_amy_knob_value(window.amy_knobs, "VCF ENV", "attack", adsr[0]);
  set_amy_knob_value(window.amy_knobs, "VCF ENV", "decay", adsr[1]);
  set_amy_knob_value(window.amy_knobs, "VCF ENV", "sustain", adsr[2]);
  set_amy_knob_value(window.amy_knobs, "VCF ENV", "release", adsr[3]);

  set_amy_knob_value(window.amy_knobs, "LFO", "freq", lfoFreq);
  set_amy_knob_value(window.amy_knobs, "LFO", "delay", lfoDelay);
  set_amy_knob_value(window.amy_knobs, "LFO", "wave", 4);   // always triangle
  set_amy_knob_value(window.amy_knobs, "LFO", "osc", lfoOsc);
  set_amy_knob_value(window.amy_knobs, "LFO", "pwm", lfoPwm);
  set_amy_knob_value(window.amy_knobs, "LFO", "filt", filterLfo);

  if (oscGate) {
    adsr = [0, 0, 1, 0];
  }
  set_amy_knob_value(window.amy_knobs, "ADSR", "attack", adsr[0]);
  set_amy_knob_value(window.amy_knobs, "ADSR", "decay", adsr[1]);
  set_amy_knob_value(window.amy_knobs, "ADSR", "sustain", adsr[2]);
  set_amy_knob_value(window.amy_knobs, "ADSR", "release", adsr[3]);

  set_amy_knob_value(window.amy_knobs, "EQ", "low", eq[0]);
  set_amy_knob_value(window.amy_knobs, "EQ", "mid", eq[1]);
  set_amy_knob_value(window.amy_knobs, "EQ", "high", eq[2]);

  set_amy_knob_value(window.amy_knobs, "Chorus", "level", chorus[0]);
  set_amy_knob_value(window.amy_knobs, "Chorus", "freq", chorus[1]);
  set_amy_knob_value(window.amy_knobs, "Chorus", "depth", chorus[2]);

  if (typeof init_knobs === "function") {
    init_knobs(window.amy_knobs);
  }

  // Set the CCs too
  for (const knob of window.amy_knobs) {
    if (knob.knob_type !== "spacer" && knob.knob_type !== "spacer-half" 
      && knob.knob_type !== 'selection' && knob.knob_type !== 'pushbutton') {
      if (typeof window.onKnobCcChange === "function") {
        window.onKnobCcChange(knob);
      }
    }
  }
}

/* 1: Juno A12 Brass Swell */
/*"
  v0
   w1c1L4G4Z  // PULSE, chain to 1, modsrc 4, LPF24, 
  v0
   a,,0.001,1,0  // off
   f261.63,1,,,,0,1
   d0.72,,,,,0
   m0Z  // no portamento
  v0
   F212.87,0.661,,2.252,,0  // filter freq
   R1.015Z  // resonance
  v0
   A518,1,83561,0.299,310,0Z  // ADSR

  v1
   w3c2L4Z  // SAW_UP, chain to 2, modsrc 4
  v1
   a,,0.591,1,0
   f261.63,1,,,,0,1
   m0Z
  v1
   A518,1,83561,0.299,310,0Z

  v2
   w1c3L4Z // PULSE
  v2
   a,,0.326,1,0
   f130.81,1,,,,0,1  // freq 1 octave below
   m0Z
  v2
   A518,1,83561,0.299,310,0Z

  v3
   w5L4Z  // NOISE, end of chain
  v3
   a,,0.001,1,0Z
  v3
   A518,1,83561,0.299,310,0Z

  v4  // LFO
   w4  // TRIANGLE
   a1,,0,1Z  // amp NOT from vel, YES from eg0
  v4f0.609  // LFO 0.609 Hz
   A148,1.0,10000,0Z  // 148 ms delay

  x7,-3,-3  // EQ high
  k1,,0.5,0.5Z // 0.5 Hz chorus
",
*/
