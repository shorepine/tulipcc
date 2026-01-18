// Define all the knobs and their parameters here
window.addEventListener("DOMContentLoaded", function() {
  window.amy_knobs = [
    {
      section: "OSC A",
      display_name: "freq",
      knob_type: "log",
      default_value: 261.63,
      min_value: 50,
      max_value: 2000,
      onChange: function(value) {
        amy_add_message("i1v0f" + value);
      }
    },
    {
      section: "OSC A",
      knob_type: "selection",
      display_name: "WAVE",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v0w" + value);
      }
    },
    {
      section: "OSC A",
      display_name: "pulse w",
      default_value: 0.5,
      min_value: 0,
      max_value: 1,
      onChange: function(value) {
        amy_add_message("i1v0d" + value);
      }
    },
    {
      section: "OSC A",
      display_name: "level",
      knob_type: "log",
      default_value: 1.0,
      min_value: 0.001,
      max_value: 1.0,
      onChange: function(value) {
        amy_add_message("i1v0a" + value);
      }
    },
    {
      section: "OSC B",
      display_name: "freq",
      knob_type: "log",
      default_value: 261.63,
      min_value: 50,
      max_value: 2000,
      onChange: function(value) {
        amy_add_message("i1v1f" + value);
      }
    },
    {
      section: "OSC B",
      knob_type: "selection",
      display_name: "WAVE",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v1w" + value);
      }
    },
    {
      section: "OSC B",
      display_name: "pulse w",
      default_value: 0.5,
      min_value: 0,
      max_value: 1,
      onChange: function(value) {
        amy_add_message("i1v1d" + value);
      }
    },
    {
      section: "OSC B",
      display_name: "level",
      knob_type: "log",
      default_value: 1.0,
      min_value: 0.001,
      max_value: 1.0,
      onChange: function(value) {
        amy_add_message("i1v1a" + value);
      }
    },
    {
      section: "VCF",
      display_name: "freq_const",
      knob_type: "log",
      default_value: 1000,
      min_value: 20,
      max_value: 8000,
      onChange: function(value) {
        amy_add_message("i1F" + value);
      }
    },
    {
      section: "VCF",
      display_name: "resonance",
      knob_type: "log",
      default_value: 0.7,
      min_value: 0.5,
      max_value: 16,
      onChange: function(value) {
        amy_add_message("i1R" + value);
      }
    },
    {
      section: "VCF",
      display_name: "freq_kbd",
      default_value: 1.0,
      min_value: 0,
      max_value: 1,
      onChange: function(value) {
        amy_add_message("i1F," + value);
      }
    },
    {
      section: "VCF",
      display_name: "freq_env",
      default_value: 4.0,
      min_value: 0,
      max_value: 10,
      onChange: function(value) {
        amy_add_message("i1F,,,," + value);
      }
    },
    {
      section: "VCF",
      display_name: "f_attack",
      default_value: 0,
      min_value: 0,
      max_value: 1000,
      onChange: function(value) {
        amy_add_message("i1B"+Math.round(value)+",1,,,,0");
      }
    },
    {
      section: "VCF",
      knob_type: "log",
      display_name: "f_decay",
      default_value: 100,
      offset: 50,
      min_value: 0,
      max_value: 2000,
      onChange: function(value) {
        amy_add_message("i1B,1,"+value+",,,0");
      }
    },
    {
      section: "VCF",
      display_name: "f_sustain",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1B,1,,"+value+",,0");
      }
    },
    {
      section: "VCF",
      knob_type: "log",
      display_name: "f_release",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
      onChange: function(value) {
        amy_add_message("i1B,1,,,"+value+",0");
      }
    },
    {
      section: "LFO",
      display_name: "freq",
      knob_type: "log",
      default_value: 4,
      min_value: 0.1,
      max_value: 20,
      onChange: function(value) {
        amy_add_message("i1v2f" + value);
      }
    },
    {
      section: "LFO",
      knob_type: "selection",
      display_name: "WAVE",
      options: ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE"],
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v2w" + value);
      }
    },
    {
      section: "LFO",
      display_name: "level",
      knob_type: "log",
      default_value: 1.0,
      min_value: 0.001,
      max_value: 4.0,
      onChange: function(value) {
        amy_add_message("i1v2a" + value);
      }
    },
    {
      section: "LFO",
      display_name: "freq",
      knob_type: "pushbutton",
      color: "#d04437",
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v0f,,,,," + value);
        amy_add_message("i1v1f,,,,," + value);
      }
    },
    {
      section: "LFO",
      display_name: "pwm",
      knob_type: "pushbutton",
      color: "#d04437",
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v0d,,,,," + value);
        amy_add_message("i1v1d,,,,," + value);
      }
    },
    {
      section: "LFO",
      display_name: "filt",
      knob_type: "pushbutton",
      color: "#d04437",
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1v0F,,,,," + 4 * value);
      }
    },
    {
      section: "ADSR",
      display_name: "attack",
      min_value: 0,
      max_value: 1000,
      onChange: function(value) {
        bp_string = Math.round(value) + ",1,,,,0"
        amy_add_message("i1v0A" + bp_string);
        amy_add_message("i1v1A" + bp_string);
      }
    },
    {
      section: "ADSR",
      knob_type: "log",
      display_name: "decay",
      offset: 50,
      min_value: 0,
      max_value: 2000,
      default_value: 100,
      onChange: function(value) {
        bp_string = ",1," + value + ",,,0";
        amy_add_message("i1v0A" + bp_string);
        amy_add_message("i1v1A" + bp_string);
      }
    },
    {
      section: "ADSR",
      display_name: "sustain",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      onChange: function(value) {
        bp_string = ",1,," + value + ",,0";
        amy_add_message("i1v0A" + bp_string);
        amy_add_message("i1v1A" + bp_string);
      }
    },
    {
      section: "ADSR",
      knob_type: "log",
      display_name: "release",
      offset: 50,
      min_value: 0,
      max_value: 8000,
      default_value: 100,
      onChange: function(value) {
        bp_string = ",1,,," + value + ",0";
        amy_add_message("i1v0A" + bp_string);
        amy_add_message("i1v1A" + bp_string);
      }
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

  let filterEvent = null;
  let resonanceValue = null;
  let bp0Times = null;
  let bp0Values = null;

  for (const event of events) {
    if (!filterEvent && Array.isArray(event.filter_freq_coefs)) {
      const hasFilter = event.filter_freq_coefs.some((value) => Number.isFinite(value));
      if (hasFilter) {
        filterEvent = event;
      }
    }
    // DON'T pick up the envelope of the LFO (osc 4).
    if (!bp0Values && event.bp0 && event.osc != 4 && typeof event.bp0 === "string") {
      // split the comma separated bp0 values and check if any are finite numbers
      const values = event.bp0.split(",").map(parseFloat);
      // values is time,value pairs, we want the a list of bp0Values and bp0Times
      const filteredValues = [];
      const filteredTimes = [];
      let hasBP0 = false;
      for (let i = 0; i < values.length; i += 2) {
        const time = values[i];
        const value = values[i + 1];
        if (Number.isFinite(value)) {
          hasBP0 = true;
        }
        filteredTimes.push(Number.isFinite(time) ? time : 0);
        filteredValues.push(Number.isFinite(value) ? value : 0);
      }
      if (hasBP0) {
        bp0Times = filteredTimes;
        bp0Values= filteredValues;
      }
    }
    if (resonanceValue === null && Number.isFinite(event.resonance)) {
      resonanceValue = event.resonance;
    }
    if (filterEvent && resonanceValue !== null && bp0Values) {
      break;
    }
  }

  let didUpdate = false;

  if (filterEvent) {
    const coefs = filterEvent.filter_freq_coefs;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_const", coefs[0]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_kbd", coefs[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_env", coefs[3]) || didUpdate;
  }

  if (resonanceValue !== null) {
    didUpdate = set_amy_knob_value(window.amy_knobs, "resonance", resonanceValue) || didUpdate;
  }
  if (bp0Values) {
    const bp0V = bp0Values;
    const bp0T = bp0Times;
    didUpdate = set_amy_knob_value(window.amy_knobs, "attack", bp0T[0]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "decay", bp0T[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "sustain", bp0V[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "release", bp0T[2]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "f_attack", bp0T[0]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "f_decay", bp0T[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "f_sustain", bp0V[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "f_release", bp0T[2]) || didUpdate;
  }

  if (didUpdate && typeof init_knobs === "function") {
    init_knobs(window.amy_knobs);
  }
}

/* 1: Juno A12 Brass Swell */
/*"v4w4a1,,0,1Zv0w1c1L4G4Zv1w3c2L4Zv2w1c3L4Zv3w5L4Zv4f0.609A148,1.0,10000,0Zv0a,,0.001,1,0f261.63,1,,,,0,1d0.72,,,,,0m0Zv1a,,0.591,1,0f261.63,1,,,,0,1m0Zv2a,,0.326,1,0f130.81,1,,,,0,1m0Zv3a,,0.001,1,0Zv0F212.87,0.661,,2.252,,0R1.015Zv0A518,1,83561,0.299,310,0Zv1A518,1,83561,0.299,310,0Zv2A518,1,83561,0.299,310,0Zv3A518,1,83561,0.299,310,0Zx7,-3,-3k1,,0.5,0.5Z",
*/
