// Define all the knobs and their parameters here
window.addEventListener("DOMContentLoaded", function() {
  window.amy_knobs = [
    {
      section: "VCF",
      knob_type: "log",
      display_name: "freq_const",
      default_value: 0,
      offset: 200,
      range: 5,
      onChange: function(value) {
        amy_add_message("i1F" + value);
      }
    },
    {
      section: "VCF",
      knob_type: "log",
      display_name: "freq_note",
      default_value: 0,
      offset: 200,
      range: 5,
      onChange: function(value) {
        amy_add_message("i1F," + value);
      }
    },
    {
      section: "VCF",
      knob_type: "log",
      display_name: "freq_eg0",
      default_value: 0,
      offset: 200,
      range: 5,
      onChange: function(value) {
        amy_add_message("i1F,,," + value);
      }
    },
    {
      section: "VCF",
      min_value: 0,
      max_value: 4,
      display_name: "resonance",
      default_value: 0,
      onChange: function(value) {
        amy_add_message("i1R" + value);
      }
    },
    { section: "ADSR", knob_type: "log", display_name: "attack", range: 7, offset: 1, default_value: 0 },
    { section: "ADSR", knob_type: "log", display_name: "decay", range: 7, offset: 1, default_value: 0 },
    { section: "ADSR", knob_type: "log", display_name: "sustain", range: 7, offset: 1, default_value: 0 },
    { section: "ADSR", knob_type: "log", display_name: "release", range: 7, offset: 1, default_value: 0 },
    { section: "OSC", knob_type: "selection", display_name: "WAVE", options: ["SINE", "PULSE", "SAW_DOWN", "SAW_UP", "TRIANGLE", "NOISE", "KS", "PCM", "ALGO", "PARTIAL", "BYO_PARTIALS", "INTERP_PARTIALS", "AUDIO_IN0", "AUDIO_IN1", "AUDIO_EXT0", "AUDIO_EXT1", "AMY_MIDI", "PCM_LEFT", "PCM_RIGHT", "CUSTOM", "WAVE_OFF"], default_value: 0 }
  ];

  if (typeof init_knobs === "function") {
    init_knobs(window.amy_knobs);
  }
  if (typeof init_patches_select === "function") {
    init_patches_select("patch-select");
  }
});


function set_knobs_from_patch_number(patch_number) {
  amy_add_message("i1K"+patch_number);
  const events = get_events_for_patch_number(patch_number);
  if (!Array.isArray(window.amy_knobs) || events.length === 0) {
    return;
  }

  let filterEvent = null;
  let resonanceValue = null;

  for (const event of events) {
    if (!filterEvent && Array.isArray(event.filter_freq_coefs)) {
      const hasFilter = event.filter_freq_coefs.some((value) => Number.isFinite(value));
      if (hasFilter) {
        filterEvent = event;
      }
    }
    if (resonanceValue === null && Number.isFinite(event.resonance)) {
      resonanceValue = event.resonance;
    }
    if (filterEvent && resonanceValue !== null) {
      break;
    }
  }

  let didUpdate = false;

  if (filterEvent) {
    const coefs = filterEvent.filter_freq_coefs;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_const", coefs[0]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_note", coefs[1]) || didUpdate;
    didUpdate = set_amy_knob_value(window.amy_knobs, "freq_eg0", coefs[3]) || didUpdate;
  }

  if (resonanceValue !== null) {
    didUpdate = set_amy_knob_value(window.amy_knobs, "resonance", resonanceValue) || didUpdate;
  }

  if (didUpdate && typeof init_knobs === "function") {
    init_knobs(window.amy_knobs);
  }
}
