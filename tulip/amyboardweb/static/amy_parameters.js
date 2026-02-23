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
  const WAVE_OPTIONS = ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE", "PCM", "WAVETABLE"];
  const WAVE_OPTION_VALUES = [0, 1, 3, 2, 4, 5, 7, 19];

  const amy_knob_definitions = [
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
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
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
      change_code: "i%iv0a,,%v",
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
      change_code: "i%iv0F%v",
      knob_type: "log",
      default_value: 1000,
      min_value: 20,
      max_value: 8000,
    },
    {
      section: "VCF",
      cc: 75,
      display_name: "resonance",
      change_code: "i%iv0R%v",
      knob_type: "log",
      default_value: 0.7,
      min_value: 0.5,
      max_value: 16,
    },
    {
      section: "VCF",
      cc: 76,
      display_name: "kbd",
      change_code: "i%iv0F,%v",
      default_value: 1.0,
      min_value: 0,
      max_value: 1,
    },
    {
      section: "VCF",
      cc: 77,
      display_name: "env",
      change_code: "i%iv0F,,,,%v",
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
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
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
      change_code: "i%iv1a,,%v",
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
      change_code: "i%iv0B%v,1,,,,0",
      default_value: 0,
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "VCF ENV",
      cc: 86,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iv0B,1,%v,,,0",
      default_value: 100,
      offset: 50,
      min_value: 0,
      max_value: 2000,
    },
    {
      section: "VCF ENV",
      cc: 87,
      display_name: "sustain",
      change_code: "i%iv0B,1,,%v,,0",
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
      change_code: "i%iv0B,1,,,%v,0",
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
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
      default_value: 0,
    },
    {
      section: "LFO",
      cc: 94,
      display_name: "osc",
      change_code: "i%iv0f,,,,,%vZi%iv1f,,,,,%v",
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
      change_code: "i%iv0d,,,,,%vZi%iv1d,,,,,%v",
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
      change_code: "i%iv0A%v,1,,,,0Zi%iv1A%v,1,,,,0",
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "ADSR",
      cc: 98,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iv0A,1,%v,,,0Zi%iv1A,1,%v,,,0",
      offset: 50,
      min_value: 0,
      max_value: 2000,
      default_value: 100,
    },
    {
      section: "ADSR",
      cc: 99,
      display_name: "sustain",
      change_code: "i%iv0A,1,,%v,,0Zi%iv1A,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
    },
    {
      section: "ADSR",
      cc: 100,
      knob_type: "log",
      display_name: "release",
      change_code: "i%iv0A,1,,,%v,0Zi%iv1A,1,,,%v,0",
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

  function cloneKnob(entry) {
    if (!entry || typeof entry !== "object") {
      return entry;
    }
    const clone = Object.assign({}, entry);
    if (Array.isArray(entry.options)) {
      clone.options = entry.options.slice();
    }
    if (entry.wave_preset_by_wave && typeof entry.wave_preset_by_wave === "object") {
      clone.wave_preset_by_wave = Object.assign({}, entry.wave_preset_by_wave);
    }
    return clone;
  }

  function cloneKnobList(list) {
    return list.map(cloneKnob);
  }

  function isRangeKnob(knob) {
    return !!knob
      && knob.knob_type !== "spacer"
      && knob.knob_type !== "spacer-half"
      && knob.knob_type !== "selection"
      && knob.knob_type !== "pushbutton";
  }

  function applyKnobCcMappingMessage(channel, message) {
    const match = String(message || "").match(/^i(\d+)ic([^,]*),([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/);
    if (!match) {
      return false;
    }
    const messageChannel = Number(match[1]);
    if (!Number.isInteger(messageChannel) || messageChannel !== channel) {
      return false;
    }
    if (!Array.isArray(window.amy_knobs) || !Array.isArray(window.amy_knobs[channel])) {
      return false;
    }
    const changeCode = String(match[7] || "");
    if (!changeCode) {
      return false;
    }
    const knob = window.amy_knobs[channel].find(function(entry) {
      return entry && entry.change_code === changeCode;
    });
    if (!knob) {
      return false;
    }

    const ccField = String(match[2] || "").trim();
    if (ccField === "") {
      knob.cc = "";
    } else {
      const ccValue = Number(ccField);
      if (Number.isInteger(ccValue) && ccValue >= 0 && ccValue <= 127) {
        knob.cc = ccValue;
      }
    }

    if (isRangeKnob(knob)) {
      const minValue = Number(match[4]);
      const maxValue = Number(match[5]);
      if (Number.isFinite(minValue) && Number.isFinite(maxValue) && minValue < maxValue) {
        knob.min_value = minValue;
        knob.max_value = maxValue;
      }
      const offsetValue = Number(match[6]);
      if (Number.isFinite(offsetValue)) {
        knob.offset = offsetValue;
      }
      if (Number(match[3]) === 1) {
        knob.knob_type = "log";
      } else if (knob.knob_type === "log") {
        knob.knob_type = undefined;
      }
    }
    return true;
  }

  function applyKnobCcMappingsFromPatchSource(channel, patchSource) {
    const ch = Number(channel);
    if (!Number.isInteger(ch) || ch < 1 || ch > 16) {
      return false;
    }
    if (!patchSource) {
      return false;
    }
    let appliedAny = false;
    const lines = String(patchSource).split("\n");
    for (const rawLine of lines) {
      const line = String(rawLine || "").trim();
      if (!line || line.startsWith("#")) {
        continue;
      }
      if (applyKnobCcMappingMessage(ch, line)) {
        appliedAny = true;
      }
    }
    return appliedAny;
  }
  window.apply_knob_cc_mappings_from_patch_source = applyKnobCcMappingsFromPatchSource;

  window.reset_amy_knobs_to_defaults = function() {
    window.amy_knobs = new Array(17);
    for (let i = 1; i <= 16; i += 1) {
      window.amy_knobs[i] = cloneKnobList(amy_knob_definitions);
    }
    window.has_restored_amy_knobs_state = false;
    return true;
  };

  window.amy_knobs = new Array(17);
  for (let i = 1; i <= 16; i += 1) {
    window.amy_knobs[i] = cloneKnobList(amy_knob_definitions);
  }
  window.has_restored_amy_knobs_state = false;

  window.get_current_knobs = function() {
    const idx = Number(window.current_synth);
    if (Array.isArray(window.amy_knobs) && window.amy_knobs[idx]) {
      return window.amy_knobs[idx];
    }
    if (Array.isArray(window.amy_knobs) && window.amy_knobs[1]) {
      return window.amy_knobs[1];
    }
    return [];
  };

  window.refresh_knobs_for_channel = function() {
    const knobs = window.get_current_knobs();
    if (typeof init_knobs === "function") {
      init_knobs(knobs);
    }
    if (typeof window.onKnobCcChange === "function") {
      for (const knob of knobs) {
        if (knob.knob_type !== "spacer" && knob.knob_type !== "spacer-half"
          && knob.knob_type !== "selection" && knob.knob_type !== "pushbutton") {
          window.onKnobCcChange(knob);
        }
      }
    }
  };

  if (typeof window.refresh_knobs_for_channel === "function") {
    var previousSuppress = !!window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = true;
    window.refresh_knobs_for_channel();
    window.suppress_knob_cc_send = previousSuppress;
  }
  if (typeof init_patches_select === "function") {
    init_patches_select("patch-select");
  }
});

function set_knobs_from_patch_number_impl(patch_number) {
  // if this is a memory patch, load it. if not, load the amyboard base patch
  if(patch_number >= 1024 && patch_number < 1024+32) {
    amy_add_log_message("i"+window.current_synth+"iv6K"+patch_number);
  } else {
    amy_add_log_message("i"+window.current_synth+"iv6K257");  // Default AMYboard patch state.
  }
  const events = get_events_for_patch_number(patch_number);
  const knobs = window.get_current_knobs ? window.get_current_knobs() : [];
  if (!Array.isArray(knobs) || events.length === 0) {
    return;
  }

  let filterFreq = null;
  let filterEnv = null;
  let filterLfo = null
  let filterKbd = null;
  let filter_eg = 0;  // Juno: filter env is eg0; updated to 1 for amyboardsynth.
  let filter_osc = 0;  // Default, but can be updated: which osc defines filter params.
  let resonanceValue = null;
  let adsr = [0, 0, 1, 0];
  let f_adsr = [0, 0, 1, 0];
  let lfoFreq = null;
  let lfoDelay = null;
  let lfoWave = 4;  // default triangle
  let lfoOsc = 0;
  let lfoPwm = 0;
  // Track coefficients for all 4 non-lfo oscs.
  let osc_freq = [null, null, null, null];
  let osc_wave = [null, null, null, null];
  let osc_preset = [null, null, null, null];
  let osc_duty = [null, null, null, null];
  let osc_gain = [0, 0, 0, 0];
  let mod_source_osc = 4;  // Start with Juno LFO osc, but may get updated if it's an amyboardsynth patch.
  // Which Juno oscs are used for oscA and B
  let oscAB_osc = [-1, -1];
  let oscAB_gain = [0, 0];
  // Do the oscs use ADSR, or the juno "gate"?
  let oscGate = 0;
  let eq = [null, null, null];
  let chorus = [null, null, null];
  const BP_UNSET = 32767;

  function bpTimeIsSet(v) {
    return Number.isFinite(v) && v !== BP_UNSET;
  }

  for (const event of events) {
    if (Number.isFinite(event.filter_freq_coefs[0])) {
      filter_osc = event.osc;  // Assume we'll see this at least once
      filterFreq = event.filter_freq_coefs[0];
    }
    if (Number.isFinite(event.filter_freq_coefs[1])) {
      filterKbd = event.filter_freq_coefs[1];  // COEF_NOTE
    }
    if (Number.isFinite(event.filter_freq_coefs[3])) {
      filterEnv = event.filter_freq_coefs[3];  // COEF_EG0, Juno filter env
    }
    if (Number.isFinite(event.filter_freq_coefs[4])) {
      filterEnv = event.filter_freq_coefs[4];  // COEF_EG1, amyboard filter env
      filter_eg = 1;
    }
    if (Number.isFinite(event.filter_freq_coefs[5])) {
      filterLfo = event.filter_freq_coefs[5];  // COEF_MOD
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
    if (event.osc == mod_source_osc) {
      // LFO
      if (Number.isFinite(event.freq_coefs[0])) {
        lfoFreq = event.freq_coefs[0];
      }
      if (event.wave >= 0 && event.wave < 127) {
        lfoWave = event.wave;
      }
      if (event.eg0_times && bpTimeIsSet(event.eg0_times[0])) {
        lfoDelay = event.eg0_times[0];
      }
    } else if (event.osc >= 0) {
      // Non-LFO osc, don't assume what order they come in.
      const parsedModSource = Number(event.mod_source);
      if (Number.isInteger(parsedModSource) && parsedModSource >= 0 && parsedModSource < 64) {
        mod_source_osc = parsedModSource;
      }
      if (event.eg0_times) {
        if (bpTimeIsSet(event.eg0_times[0])) { adsr[0] = event.eg0_times[0]; }   // A time
        if (bpTimeIsSet(event.eg0_times[1])) { adsr[1] = event.eg0_times[1]; }   // D time
        if (bpTimeIsSet(event.eg0_times[2])) { adsr[3] = event.eg0_times[2]; }   // R time
      }
      if (event.eg0_values) {
	if (Number.isFinite(event.eg0_values[1])) { adsr[2] = event.eg0_values[1]; }  // S level
      }
      if (event.eg1_times) {
        if (bpTimeIsSet(event.eg1_times[0])) { f_adsr[0] = event.eg1_times[0]; }   // A time
        if (bpTimeIsSet(event.eg1_times[1])) { f_adsr[1] = event.eg1_times[1]; }   // D time
        if (bpTimeIsSet(event.eg1_times[2])) { f_adsr[3] = event.eg1_times[2]; }   // R time
      }
      if (event.eg1_values) {
	if (Number.isFinite(event.eg1_values[1])) { f_adsr[2] = event.eg1_values[1]; }  // S level
      }
      // Extract key parameters for each osc
      if (Number.isFinite(event.amp_coefs[2]) && event.amp_coefs[2] > 0) {
        osc_gain[event.osc] = event.amp_coefs[2];
      }
      if (Number.isFinite(event.amp_coefs[3]) && event.amp_coefs[3] == 0) {
        // Juno patches decouple amp from EG0 for "gate" mode.
        oscGate = 1;
      }
      if (Number.isFinite(event.freq_coefs[0]) && event.freq_coefs[0] > 0) {
        osc_freq[event.osc] = event.freq_coefs[0];
      }
      if (Number.isFinite(event.freq_coefs[5]) && event.freq_coefs[5] > 0) {
        lfoOsc = event.freq_coefs[5];  // freq COEF_MOD == 5
      }
      if (Number.isFinite(event.duty_coefs[0]) && event.duty_coefs[0] > 0) {
        osc_duty[event.osc] = event.duty_coefs[0];
      }
      if (Number.isFinite(event.duty_coefs[5]) && event.duty_coefs[5] > lfoPwm) {
        lfoPwm = event.duty_coefs[5];  // duty COEF_MOD == 5
      }
      if (event.wave >= 0 && event.wave < 127) {
        osc_wave[event.osc] = event.wave
      }
      if (Number.isFinite(event.preset) && event.preset >= 0) {
        osc_preset[event.osc] = event.preset;
      }
    }
  }

  // Re-scan for LFO so event ordering does not force defaults.
  // Prefer osc 2 (the editor's dedicated LFO oscillator), then fall back to inferred mod source.
  var lfoSourceOsc = 2;
  if (!events.some(function(event) { return event && event.osc === 2; })) {
    lfoSourceOsc = mod_source_osc;
  }
  for (const event of events) {
    if (!event || event.osc !== lfoSourceOsc) {
      continue;
    }
    if (Number.isFinite(event.freq_coefs[0])) {
      lfoFreq = event.freq_coefs[0];
    }
    if (event.wave >= 0 && event.wave < 127) {
      lfoWave = event.wave;
    }
    if (event.eg0_times && bpTimeIsSet(event.eg0_times[0])) {
      lfoDelay = event.eg0_times[0];
    }
  }
  // If we didn't set up a separate filter ADSR, it follows the VCA
  if (filter_eg == 0) {
    f_adsr = adsr;
  }
  // Logic to choose juno oscs for osc A and osc B.
  if (osc_gain[2] == 0 && osc_gain[3] == 0) {
    // Only 2 oscs, let them be
    oscAB_osc[0] = 0;
    oscAB_osc[1] = 1;
  } else {
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
  }

  // Configure the patch.
  set_amy_knob_value(knobs, "Osc A", "freq", osc_freq[oscAB_osc[0]]);
  set_amy_knob_value(knobs, "Osc A", "wave", osc_wave[oscAB_osc[0]]);
  if (typeof set_amy_knob_wave_preset === "function"
    && Number.isFinite(osc_wave[oscAB_osc[0]]) && Number.isFinite(osc_preset[oscAB_osc[0]])) {
    set_amy_knob_wave_preset(knobs, "Osc A", osc_wave[oscAB_osc[0]], osc_preset[oscAB_osc[0]], false);
  }
  set_amy_knob_value(knobs, "Osc A", "duty", osc_duty[oscAB_osc[0]]);
  set_amy_knob_value(knobs, "Osc A", "level", osc_gain[oscAB_osc[0]]);

  set_amy_knob_value(knobs, "Osc B", "freq", osc_freq[oscAB_osc[1]]);
  set_amy_knob_value(knobs, "Osc B", "wave", osc_wave[oscAB_osc[1]]);
  if (typeof set_amy_knob_wave_preset === "function"
    && Number.isFinite(osc_wave[oscAB_osc[1]]) && Number.isFinite(osc_preset[oscAB_osc[1]])) {
    set_amy_knob_wave_preset(knobs, "Osc B", osc_wave[oscAB_osc[1]], osc_preset[oscAB_osc[1]], false);
  }
  set_amy_knob_value(knobs, "Osc B", "duty", osc_duty[oscAB_osc[1]]);
  set_amy_knob_value(knobs, "Osc B", "level", osc_gain[oscAB_osc[1]]);

  set_amy_knob_value(knobs, "VCF", "freq", filterFreq);
  set_amy_knob_value(knobs, "VCF", "resonance", resonanceValue);
  set_amy_knob_value(knobs, "VCF", "kbd", filterKbd);
  set_amy_knob_value(knobs, "VCF", "env", filterEnv);
  set_amy_knob_value(knobs, "VCF ENV", "attack", f_adsr[0]);
  set_amy_knob_value(knobs, "VCF ENV", "decay", f_adsr[1]);
  set_amy_knob_value(knobs, "VCF ENV", "sustain", f_adsr[2]);
  set_amy_knob_value(knobs, "VCF ENV", "release", f_adsr[3]);

  set_amy_knob_value(knobs, "LFO", "freq", lfoFreq);
  set_amy_knob_value(knobs, "LFO", "delay", lfoDelay);
  set_amy_knob_value(knobs, "LFO", "wave", lfoWave);
  set_amy_knob_value(knobs, "LFO", "osc", lfoOsc);
  set_amy_knob_value(knobs, "LFO", "pwm", lfoPwm);
  set_amy_knob_value(knobs, "LFO", "filt", filterLfo);

  if (oscGate) {
    adsr = [0, 0, 1, 0];
  }
  set_amy_knob_value(knobs, "ADSR", "attack", adsr[0]);
  set_amy_knob_value(knobs, "ADSR", "decay", adsr[1]);
  set_amy_knob_value(knobs, "ADSR", "sustain", adsr[2]);
  set_amy_knob_value(knobs, "ADSR", "release", adsr[3]);

  set_amy_knob_value(knobs, "EQ", "low", eq[0]);
  set_amy_knob_value(knobs, "EQ", "mid", eq[1]);
  set_amy_knob_value(knobs, "EQ", "high", eq[2]);

  set_amy_knob_value(knobs, "Chorus", "level", chorus[0]);
  set_amy_knob_value(knobs, "Chorus", "freq", chorus[1]);
  set_amy_knob_value(knobs, "Chorus", "depth", chorus[2]);

}

function set_knobs_from_patch(channel, patch_number, patch_source) {
  const requestedChannel = Number(channel);
  if (!Number.isInteger(requestedChannel) || requestedChannel < 1 || requestedChannel > 16) {
    return false;
  }
  const previousChannel = Number(window.current_synth || 1);
  const previousSuppress = !!window.suppress_knob_cc_send;
  window.current_synth = requestedChannel;
  window.suppress_knob_cc_send = false;
  try {
    set_knobs_from_patch_number_impl(patch_number);
    if (typeof window.apply_knob_cc_mappings_from_patch_source === "function") {
      window.apply_knob_cc_mappings_from_patch_source(requestedChannel, patch_source || "");
    }
    if (typeof window.refresh_knobs_for_channel === "function") {
      // Keep refresh inside suppression to avoid rewriting web.patch while loading patch files.
      window.refresh_knobs_for_channel();
    }
  } finally {
    window.suppress_knob_cc_send = previousSuppress;
    window.current_synth = previousChannel;
  }
  return true;
}
window.set_knobs_from_patch = set_knobs_from_patch;

function set_knobs_from_patch_number(patch_number) {
  const channel = Number(window.current_synth || 1);
  return set_knobs_from_patch(channel, patch_number, "");
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
