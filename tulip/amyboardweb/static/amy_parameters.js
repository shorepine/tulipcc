// amyboard default patch (K257) conventions.
const CTL_OSC = 0;  // Osc with filter & envelope commands.c
const LFO_OSC = 1;
const OSCA_OSC = 2;
const OSCB_OSC = 3;

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
    { name: "Echo", bg_color: "rgba(176, 208, 232, 0.75)", header_bg_color: "#000", header_fg_color: "#fff" },
  ];
  const GLOBAL_SECTION_NAMES = ["EQ", "Chorus", "Reverb", "Echo"];
  const WAVE_OPTIONS = ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE", "PCM", "WAVETABLE", "ALGO"];
  const WAVE_OPTION_VALUES = [AMY.SINE, 1, 3, 2, 4, 5, 7, 19, 8];

  // amy_default: the value AMY's reset_osc() sets for this parameter.
  // Used to initialize knob state before applying patch changes, since
  // AMY's EVENT_FROM_OSC_ARRAY only emits values that differ from reset defaults.
  const amy_knob_definitions = [
    {
      section: "Osc A",
      cc: 70,
      display_name: "freq",
      change_code: "i%iv" + OSCA_OSC + "f%v",
      knob_type: "log",
      default_value: 440.0,
      amy_default: 440.0,    // logfreq_coefs[CONST]=0; display as middle C
      min_value: 50,
      max_value: 2000,
    },
    {
      section: "Osc A",
      cc: 71,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv" + OSCA_OSC + "w%v",
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
      default_value: 0,
      amy_default: 0,         // wave = SINE
    },
    {
      section: "Osc A",
      cc: 72,
      display_name: "duty",
      change_code: "i%iv" + OSCA_OSC + "d%v",
      default_value: 0.5,
      amy_default: 0.5,       // duty_coefs[CONST] = 0.5
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc A",
      cc: 73,
      display_name: "level",
      change_code: "i%iv" + OSCA_OSC + "a%v,,0,0",
      knob_type: "log",
      default_value: 1.0,
      amy_default: 1.0,       // amp_coefs[EG0] = 1.0
      min_value: 0.001,
      max_value: 1.0,
      offset: 0.1,
    },
    {
      section: "VCF",
      cc: 74,
      display_name: "freq",
      change_code: "i%iv" + CTL_OSC + "F%v",
      knob_type: "log",
      default_value: 1000,
      amy_default: 1000,      // filter_type=NONE after reset; display default
      min_value: 20,
      max_value: 8000,
    },
    {
      section: "VCF",
      cc: 75,
      display_name: "resonance",
      change_code: "i%iv" + CTL_OSC + "R%v",
      knob_type: "log",
      default_value: 0.7,
      amy_default: 0.7,       // resonance = 0.7
      min_value: 0.5,
      max_value: 16,
    },
    {
      section: "VCF",
      cc: 76,
      display_name: "kbd",
      change_code: "i%iv" + CTL_OSC + "F,%v",
      default_value: 1.0,
      amy_default: 0,         // filter_logfreq_coefs[NOTE] = 0
      min_value: 0,
      max_value: 1,
    },
    {
      section: "VCF",
      cc: 77,
      display_name: "env",
      change_code: "i%iv" + CTL_OSC + "F,,,,%v",
      default_value: 4.0,
      amy_default: 0,         // filter_logfreq_coefs[EG1] = 0
      min_value: -10,
      max_value: 10,
    },

    {
      section: "EQ",
      cc: 78,
      display_name: "low",
      change_code: "x%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,         // EQ bands default to 0
    },
    {
      section: "EQ",
      cc: 79,
      display_name: "mid",
      change_code: "x,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,
    },
    {
      section: "EQ",
      cc: 80,
      display_name: "high",
      change_code: "x,,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,
    },

    {
      section: "Osc B",
      cc: 81,
      display_name: "freq",
      change_code: "i%iv" + OSCB_OSC + "f%v",
      knob_type: "log",
      default_value: 440.0,
      amy_default: 440.0,
      min_value: 50,
      max_value: 2000,
    },
    {
      section: "Osc B",
      cc: 82,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv" + OSCB_OSC + "w%v",
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
      default_value: 0,
      amy_default: 0,         // wave = SINE
    },
    {
      section: "Osc B",
      cc: 83,
      display_name: "duty",
      change_code: "i%iv" + OSCB_OSC + "d%v",
      default_value: 0.5,
      amy_default: 0.5,       // duty_coefs[CONST] = 0.5
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc B",
      cc: 84,
      display_name: "level",
      change_code: "i%iv" + OSCB_OSC + "a%v,,0,0",
      knob_type: "log",
      default_value: 1.0,
      amy_default: 1.0,       // amp_coefs[EG0] = 1.0
      min_value: 0.001,
      max_value: 1.0,
      offset: 0.1,
    },

    {
      section: "VCF ENV",
      cc: 85,
      display_name: "attack",
      change_code: "i%iv" + CTL_OSC + "B%v,1,,,,0",
      default_value: 0,
      amy_default: 0,         // breakpoints UNSET → no envelope
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "VCF ENV",
      cc: 86,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iv" + CTL_OSC + "B,1,%v,,,0",
      default_value: 100,
      amy_default: 0,         // breakpoints UNSET → no envelope
      offset: 50,
      min_value: 0,
      max_value: 2000,
    },
    {
      section: "VCF ENV",
      cc: 87,
      display_name: "sustain",
      change_code: "i%iv" + CTL_OSC + "B,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 0,         // no filter envelope → sustain 0
    },
    {
      section: "VCF ENV",
      cc: 88,
      knob_type: "log",
      display_name: "release",
      change_code: "i%iv" + CTL_OSC + "B,1,,,%v,0",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
      amy_default: 0,         // breakpoints UNSET → no envelope
    },
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
      amy_default: 0,         // chorus off
    },
    {
      section: "Chorus",
      cc: 90,
      knob_type: "log",
      display_name: "freq",
      change_code: "k,,%v",
      default_value: 0.5,
      amy_default: 0.5,       // CHORUS_DEFAULT_LFO_FREQ
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
      amy_default: 0.5,       // chorus default depth
    },
    {
      section: "LFO",
      cc: 92,
      display_name: "freq",
      change_code: "i%iv" + LFO_OSC + "f%v",
      knob_type: "log",
      default_value: 4,
      amy_default: 4,         // LFO display default (logfreq_coefs[CONST]=0)
      min_value: 0.1,
      max_value: 20,
    },
    {
      section: "LFO",
      cc: 93,
      knob_type: "selection",
      display_name: "wave",
      change_code: "i%iv" + LFO_OSC + "w%v",
      options: WAVE_OPTIONS,
      option_values: WAVE_OPTION_VALUES,
      default_value: 0,
      amy_default: 0,         // wave = SINE
    },
    {
      section: "LFO",
      cc: 94,
      display_name: "osc",
      change_code: "i%iv" + OSCA_OSC + "f,,,,,%vZi%iv" + OSCB_OSC + "f,,,,,%v",
      knob_type: "log",
      default_value: 0.0,
      amy_default: 0,         // logfreq_coefs[MOD] = 0
      min_value: 0.0,
      max_value: 4.0,
      offset: 0.001,
    },
    {
      section: "LFO",
      cc: 95,
      display_name: "pwm",
      change_code: "i%iv" + OSCA_OSC + "d,,,,,%vZi%iv" + OSCB_OSC + "d,,,,,%v",
      default_value: 0.0,
      amy_default: 0,         // duty_coefs[MOD] = 0
      min_value: 0.0,
      max_value: 0.49,
    },
    {
      section: "LFO",
      cc: 96,
      display_name: "filt",
      change_code: "i%iv" + CTL_OSC + "F,,,,,%v",
      default_value: 0.0,
      amy_default: 0,         // filter_logfreq_coefs[MOD] = 0
      min_value: 0.0,
      max_value: 4.0,
      offset: 0.001,
    },

    {
      section: "ADSR",
      cc: 97,
      display_name: "attack",
      change_code: "i%iv" + CTL_OSC + "A%v,1,,,,0",
      min_value: 0,
      max_value: 1000,
      default_value: 0,
      amy_default: 0,         // breakpoints UNSET → no envelope
    },
    {
      section: "ADSR",
      cc: 98,
      knob_type: "log",
      display_name: "decay",
      change_code: "i%iv" + CTL_OSC + "A,1,%v,,,0",
      offset: 50,
      min_value: 0,
      max_value: 2000,
      default_value: 100,
      amy_default: 0,         // breakpoints UNSET → no envelope
    },
    {
      section: "ADSR",
      cc: 99,
      display_name: "sustain",
      change_code: "i%iv" + CTL_OSC + "A,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 1,         // no breakpoints → EG0 = constant 1.0
    },
    {
      section: "ADSR",
      cc: 100,
      knob_type: "log",
      display_name: "release",
      change_code: "i%iv" + CTL_OSC + "A,1,,,%v,0",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
      amy_default: 0,         // breakpoints UNSET → no envelope
    },



    {
      section: "Reverb",
      cc: 101,
      knob_type: "log",
      display_name: "level",
      change_code: "h%v",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 0,         // reverb off
      offset: 0.1,
    },
    {
      section: "Reverb",
      cc: 102,
      display_name: "live",
      change_code: "h,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.85,
      amy_default: 0.85,      // REVERB_DEFAULT_LIVENESS
    },
    {
      section: "Reverb",
      cc: 103,
      display_name: "damp",
      change_code: "h,,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.5,
      amy_default: 0.5,       // reverb default damping
    },
    {
      section: "Echo",
      cc: 104,
      display_name: "level",
      change_code: "M%v",
      min_value: 0,
      max_value: 2,
      default_value: 0,
      amy_default: 0,         // echo off
    },
    {
      section: "Echo",
      cc: 105,
      display_name: "delay",
      change_code: "M,%v",
      min_value: 0,
      max_value: 5000,
      default_value: 500,
      amy_default: 500,       // ECHO_DEFAULT_DELAY_MS
    },
    {
      section: "Echo",
      cc: 106,
      display_name: "feedback",
      change_code: "M,,,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 0,
    },

  ];

  function isGlobalSection(sectionName) {
    return GLOBAL_SECTION_NAMES.indexOf(String(sectionName || "")) !== -1;
  }

  function nearestSectionName(list, start, direction) {
    for (let i = start; i >= 0 && i < list.length; i += direction) {
      const entry = list[i];
      if (!entry || typeof entry !== "object") {
        continue;
      }
      if (entry.knob_type === "spacer" || entry.knob_type === "spacer-half") {
        continue;
      }
      if (typeof entry.section === "string") {
        return entry.section;
      }
      return null;
    }
    return null;
  }

  function build_channel_knob_definitions(list) {
    const out = [];
    for (let i = 0; i < list.length; i += 1) {
      const entry = list[i];
      if (!entry || typeof entry !== "object") {
        continue;
      }
      if (entry.knob_type === "spacer" || entry.knob_type === "spacer-half") {
        const prevSection = nearestSectionName(list, i - 1, -1);
        const nextSection = nearestSectionName(list, i + 1, 1);
        if (prevSection && nextSection && !isGlobalSection(prevSection) && !isGlobalSection(nextSection)) {
          out.push(entry);
        }
        continue;
      }
      if (!isGlobalSection(entry.section)) {
        out.push(entry);
      }
    }
    return out;
  }

  function build_global_knob_definitions(list) {
    const out = [];
    for (let i = 0; i < list.length; i += 1) {
      const entry = list[i];
      if (!entry || typeof entry !== "object") {
        continue;
      }
      if (entry.knob_type === "spacer" || entry.knob_type === "spacer-half") {
        continue;
      }
      if (isGlobalSection(entry.section)) {
        out.push(entry);
      }
    }
    return out;
  }

  const channel_knob_definitions = build_channel_knob_definitions(amy_knob_definitions);
  const global_knob_definitions = build_global_knob_definitions(amy_knob_definitions);

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
    // Match both prefixed (i1ic70,...) and unprefixed (ic70,...) formats.
    const match = String(message || "").match(/^(?:i(\d+))?ic([^,]*),([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/);
    if (!match) {
      return false;
    }
    if (match[1] !== undefined) {
      const messageChannel = Number(match[1]);
      if (!Number.isInteger(messageChannel) || messageChannel !== channel) {
        return false;
      }
    }
    // Strip trailing Z (AMY wire protocol terminator) from the template.
    const changeCode = String(match[7] || "").replace(/Z+$/, "");
    if (!changeCode) {
      return false;
    }
    const perChannelKnobs = (Array.isArray(window.amy_channel_knobs) && Array.isArray(window.amy_channel_knobs[channel]))
      ? window.amy_channel_knobs[channel] : [];
    const globalKnobs = Array.isArray(window.amy_global_knobs) ? window.amy_global_knobs : [];
    let knob = perChannelKnobs.find(function(entry) {
      return entry && entry.change_code === changeCode;
    });
    if (!knob) {
      knob = globalKnobs.find(function(entry) {
        return entry && entry.change_code === changeCode;
      });
    }
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

  function restoreAmyDefaults(knobList) {
    for (let i = 0; i < knobList.length; i += 1) {
      var knob = knobList[i];
      if (knob && typeof knob === "object" && knob.amy_default !== undefined) {
        knob.default_value = knob.amy_default;
      }
    }
  }

  window.reset_amy_knobs_to_defaults = function() {
    window.amy_channel_knobs = new Array(17);
    for (let i = 1; i <= 16; i += 1) {
      window.amy_channel_knobs[i] = cloneKnobList(channel_knob_definitions);
      restoreAmyDefaults(window.amy_channel_knobs[i]);
    }
    window.amy_global_knobs = cloneKnobList(global_knob_definitions);
    restoreAmyDefaults(window.amy_global_knobs);
    window.has_restored_amy_knobs_state = false;
    return true;
  };

  window.reset_channel_knobs_to_defaults = function(channel) {
    const idx = Number(channel);
    if (!Number.isInteger(idx) || idx < 1 || idx > 16) return;
    if (!Array.isArray(window.amy_channel_knobs)) {
      window.amy_channel_knobs = new Array(17);
    }
    window.amy_channel_knobs[idx] = cloneKnobList(channel_knob_definitions);
    restoreAmyDefaults(window.amy_channel_knobs[idx]);
  };

  window.amy_channel_knobs = new Array(17);
  for (let i = 1; i <= 16; i += 1) {
    window.amy_channel_knobs[i] = cloneKnobList(channel_knob_definitions);
    restoreAmyDefaults(window.amy_channel_knobs[i]);
  }
  window.amy_global_knobs = cloneKnobList(global_knob_definitions);
  restoreAmyDefaults(window.amy_global_knobs);
  window.has_restored_amy_knobs_state = false;

  window.get_channel_knobs = function(channel) {
    const idx = Number(channel);
    if (Array.isArray(window.amy_channel_knobs) && window.amy_channel_knobs[idx]) {
      return window.amy_channel_knobs[idx];
    }
    if (Array.isArray(window.amy_channel_knobs) && window.amy_channel_knobs[1]) {
      return window.amy_channel_knobs[1];
    }
    return [];
  };

  window.get_global_knobs = function() {
    if (Array.isArray(window.amy_global_knobs)) {
      return window.amy_global_knobs;
    }
    return [];
  };

  window.get_current_knobs = function() {
    return window.get_channel_knobs(window.current_synth).concat(window.get_global_knobs());
  };

  window.refresh_knobs_for_channel = function() {
    const channelKnobs = window.get_channel_knobs(window.current_synth);
    const globalKnobs = window.get_global_knobs();
    const knobs = channelKnobs.concat(globalKnobs);
    if (typeof init_knobs === "function") {
      init_knobs(channelKnobs, "knob-grid-channel");
      init_knobs(globalKnobs, "knob-grid-global");
    }
    if (typeof window.onKnobCcChange === "function") {
      for (const knob of knobs) {
        if (knob.knob_type !== "spacer" && knob.knob_type !== "spacer-half"
          && knob.knob_type !== "pushbutton") {
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

const PatchType = {
  AMYBOARD: 0,
  JUNO: 1,   // not used - Juno patches are translated to AMYBOARD.
  DX7: 2,
};

// Called from editor/index.html as part of servicing the Load Preset button.
// Also called from spss.sync_channel_knobs_from_synth_to_ui called in many places.
function set_knobs_from_events(events, synth) {
  {
    const knobs = window.get_current_knobs ? window.get_current_knobs() : [];
    if (!Array.isArray(knobs) || events.length === 0) {
      return;
    }
    // Initialize all working variables to AMY reset defaults (from reset_osc in amy.c).
    // AMY's EVENT_FROM_OSC_ARRAY only emits values that differ from reset defaults,
    // so any field not present in the events should remain at the AMY default.
    let filterFreq = 1000;      // filter_type=NONE after reset; use display default
    let filterEnv = 0;          // filter_logfreq_coefs[EG1] = 0
    let filterLfo = 0;          // filter_logfreq_coefs[MOD] = 0
    let filterKbd = 0;          // filter_logfreq_coefs[NOTE] = 0
    let resonanceValue = 0.7;   // resonance = 0.7
    let adsr = [0, 0, 1, 0];   // no breakpoints → EG0 = constant 1.0 (sustain=1)
    let f_adsr = [0, 0, 0, 0]; // no breakpoints → no filter envelope (sustain=0)
    let lfoFreq = 4;            // LFO display default (logfreq_coefs[CONST]=0)
    let lfoDelay = null;        // no knob for this
    let lfoWave = 0;            // wave = SINE (AMY reset default)
    let lfoOsc = 0;             // logfreq_coefs[MOD] = 0
    let lfoPwm = 0;             // duty_coefs[MOD] = 0
    // Track coefficients for the 2 non-lfo oscs.  Because LFO is now osc 1, we need index [2]
    let osc_freq = [440.0, 440.0, 440.0, 440.0]; // display default (logfreq_coefs[CONST]=0)
    let osc_wave = [0, 0, 0];      // wave = SINE = 0
    let osc_preset = [null, null, null];
    let osc_duty = [0.5, 0.5, 0.5];  // duty_coefs[CONST] = 0.5
    let osc_gain = [1, 1, 1];      // amp_coefs[EG0] = 1.0
    let synthLevel = 1;            // amp_coefs[CONST] on osc 0 (CTL_OSC)
    function knobDefault(section, name) {
      var allKnobs = (window.amy_global_knobs || []).concat(knobs || []);
      for (var i = 0; i < allKnobs.length; i++) {
        var k = allKnobs[i];
        if (k && k.section === section && k.display_name === name && k.amy_default !== undefined) return k.amy_default;
      }
      return 0;
    }
    let eq = [knobDefault("EQ","low"), knobDefault("EQ","mid"), knobDefault("EQ","high")];
    let chorus = [knobDefault("Chorus","level"), knobDefault("Chorus","freq"), knobDefault("Chorus","depth")];
    let reverb = [knobDefault("Reverb","level"), knobDefault("Reverb","live"), knobDefault("Reverb","damp")];
    let echo = [knobDefault("Echo","level"), knobDefault("Echo","delay"), knobDefault("Echo","feedback")];
    const BP_UNSET = 32767;

    function bpTimeIsSet(v) {
      return Number.isFinite(v) && v !== BP_UNSET;
    }

    for (const event of events) {

      // non-osc values.
      if (event.eq) {
        if (Number.isFinite(event.eq[0]))  { eq[0] = event.eq[0]; }
        if (Number.isFinite(event.eq[1]))  { eq[1] = event.eq[1]; }
        if (Number.isFinite(event.eq[2]))  { eq[2] = event.eq[2]; }
      }
      if (event.chorus) {
        if (Number.isFinite(event.chorus[0])) { chorus[0] = event.chorus[0]; }  // level
        if (Number.isFinite(event.chorus[2])) { chorus[1] = event.chorus[2]; }  // lfo_freq
        if (Number.isFinite(event.chorus[3])) { chorus[2] = event.chorus[3]; }  // depth
      }
      if (event.reverb) {
        if (Number.isFinite(event.reverb[0])) { reverb[0] = event.reverb[0]; }
        if (Number.isFinite(event.reverb[1])) { reverb[1] = event.reverb[1]; }
        if (Number.isFinite(event.reverb[2])) { reverb[2] = event.reverb[2]; }
      }
      if (event.echo) {
        if (Number.isFinite(event.echo[0])) { echo[0] = event.echo[0]; }
        if (Number.isFinite(event.echo[1])) { echo[1] = event.echo[1]; }
        if (Number.isFinite(event.echo[3])) { echo[2] = event.echo[3]; }
      }

      // Remainder of block assumes osc is set.
      if (!Number.isFinite(event.osc)) continue;

      if (event.osc == LFO_OSC) {
        // LFO
        if (event.freq && Number.isFinite(event.freq[0])) {
          lfoFreq = event.freq[0];
        }
        if (event.wave >= 0 && event.wave < 127) {
          lfoWave = event.wave;
        }
        if (event.eg0_times && bpTimeIsSet(event.eg0_times[0])) {
          lfoDelay = event.eg0_times[0];
        }
      }
      if (event.osc == CTL_OSC) {
        if (event.amp && Number.isFinite(event.amp[0])) {
          synthLevel = event.amp[0];
        }
        if (event.filter_freq) {
          if (Number.isFinite(event.filter_freq[0])) {
            filterFreq = event.filter_freq[0];
          }
          if (Number.isFinite(event.filter_freq[1])) {
            filterKbd = event.filter_freq[1];  // COEF_NOTE
          }
          if (Number.isFinite(event.filter_freq[4])) {
            filterEnv = event.filter_freq[4];  // COEF_EG1, amyboard filter env
          }
          if (Number.isFinite(event.filter_freq[5])) {
            filterLfo = event.filter_freq[5];  // COEF_MOD
          }
        }
        if (Number.isFinite(event.resonance)) {
          resonanceValue = event.resonance;
        }
        if (event.eg1_times) {
          if (bpTimeIsSet(event.eg1_times[0])) { f_adsr[0] = event.eg1_times[0]; }   // A time
          if (bpTimeIsSet(event.eg1_times[1])) { f_adsr[1] = event.eg1_times[1]; }   // D time
          if (bpTimeIsSet(event.eg1_times[2])) { f_adsr[3] = event.eg1_times[2]; }   // R time
        }
        if (event.eg1_values) {
          if (Number.isFinite(event.eg1_values[1])) { f_adsr[2] = event.eg1_values[1]; }  // S level
        }
        // Pull ADSR env values
        if (event.eg0_times) {
          if (bpTimeIsSet(event.eg0_times[0])) { adsr[0] = event.eg0_times[0]; }   // A time
          if (bpTimeIsSet(event.eg0_times[1])) { adsr[1] = event.eg0_times[1]; }   // D time
          if (bpTimeIsSet(event.eg0_times[2])) { adsr[3] = event.eg0_times[2]; }   // R time
        }
        if (event.eg0_values) {
          if (Number.isFinite(event.eg0_values[1])) { adsr[2] = event.eg0_values[1]; }  // S level
        }
      }
      if (event.osc == OSCA_OSC || event.osc == OSCB_OSC) {
        // Extract key parameters for each osc
        if (event.amp && Number.isFinite(event.amp[0])) {
          osc_gain[event.osc] = event.amp[0];
        }
        if (event.freq && Number.isFinite(event.freq[0]) && event.freq[0] > 0) {
          osc_freq[event.osc] = event.freq[0];
        }
        if (event.duty) {
          if (Number.isFinite(event.duty[0]) && event.duty[0] > 0) {
            osc_duty[event.osc] = event.duty[0];
          }
          if (Number.isFinite(event.duty[5]) && event.duty[5] > lfoPwm) {
            lfoPwm = event.duty[5];  // duty COEF_MOD == 5
          }
        }
        if (Number.isFinite(event.wave) && event.wave >= 0 && event.wave < 127) {
          osc_wave[event.osc] = event.wave;
        }
        if (Number.isFinite(event.preset) && event.preset >= 0) {
          osc_preset[event.osc] = event.preset;
        }
        if (event.freq && Number.isFinite(event.freq[5]) && event.freq[5] > 0) {
          lfoOsc = event.freq[5];  // freq COEF_MOD == 5
        }
      }
    }
    // Configure the patch.
    set_amy_knob_value(knobs, "Osc A", "freq", osc_freq[OSCA_OSC]);
    set_amy_knob_value(knobs, "Osc A", "wave", osc_wave[OSCA_OSC]);
    if (typeof set_amy_knob_wave_preset === "function"
        && Number.isFinite(osc_wave[OSCA_OSC]) && Number.isFinite(osc_preset[OSCA_OSC])) {
      set_amy_knob_wave_preset(knobs, "Osc A", osc_wave[OSCA_OSC], osc_preset[OSCA_OSC], false);
    }
    set_amy_knob_value(knobs, "Osc A", "duty", osc_duty[OSCA_OSC]);
    set_amy_knob_value(knobs, "Osc A", "level", osc_gain[OSCA_OSC]);

    set_amy_knob_value(knobs, "Osc B", "freq", osc_freq[OSCB_OSC]);
    set_amy_knob_value(knobs, "Osc B", "wave", osc_wave[OSCB_OSC]);
    if (typeof set_amy_knob_wave_preset === "function"
      && Number.isFinite(osc_wave[OSCB_OSC]) && Number.isFinite(osc_preset[OSCB_OSC])) {
      set_amy_knob_wave_preset(knobs, "Osc B", osc_wave[OSCB_OSC], osc_preset[OSCB_OSC], false);
    }
    set_amy_knob_value(knobs, "Osc B", "duty", osc_duty[OSCB_OSC]);
    set_amy_knob_value(knobs, "Osc B", "level", osc_gain[OSCB_OSC]);

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

    set_amy_knob_value(knobs, "Reverb", "level", reverb[0]);
    set_amy_knob_value(knobs, "Reverb", "live", reverb[1]);
    set_amy_knob_value(knobs, "Reverb", "damp", reverb[2]);

    set_amy_knob_value(knobs, "Echo", "level", echo[0]);
    set_amy_knob_value(knobs, "Echo", "delay", echo[1]);
    set_amy_knob_value(knobs, "Echo", "feedback", echo[2]);

    // Update synth level slider from osc 0 amp[CONST]
    if (typeof window.setSynthLevelFromAmy === "function") {
      window.setSynthLevelFromAmy(synth, synthLevel);
    }
  }
}

window.set_knobs_from_events = set_knobs_from_events;

function set_knobs_from_synth(synth) {
  let wire_commands = get_wire_commands_for_channel(synth);
  let events = events_from_wire_code_messages(wire_commands);
  return set_knobs_from_events(events, synth);
}

