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
  // "Global" sections are really per-BUS now (AMY #686): each of the
  // AMY_NUM_BUSES mix buses has its own independent EQ/Chorus/Reverb/Echo (and
  // a master volume). Their change_codes carry the i%i synth prefix so AMY
  // routes the command to the sending channel's bus (and so device-side MIDI-CC
  // templates keep working when a channel is moved to another bus). The knob
  // log records them bus-prefixed instead (y<bus>...) — see record_knob_value.
  const GLOBAL_SECTION_NAMES = ["Bus", "EQ", "Chorus", "Reverb", "Echo"];
  const WAVE_OPTIONS = ["SINE", "PULSE", "SAW_UP", "SAW_DOWN", "TRIANGLE", "NOISE", "PCM", "WAVETABLE", "ALGO"];
  const WAVE_OPTION_VALUES = [AMY.SINE, 1, 3, 2, 4, 5, 7, 19, 8];

  // amy_default: the value AMY's reset_osc() sets for this parameter.
  // Used to initialize knob state before applying patch changes, since
  // AMY's EVENT_FROM_OSC_ARRAY only emits values that differ from reset defaults.
  const amy_knob_definitions = [
    {
      // Per-channel overall level (VCA). Shown by the dedicated "Level" slider in
      // the channel strip (dedicated_slider -> kept out of the knob grid), but
      // registered as a channel knob so its MIDI CC rides the same
      // save/restore/bulk-send path as every other knob. Same AMY command as the
      // Level slider: i<ch>v0a<val> (amp on the control osc). Default CC 7 = the
      // MIDI standard "Channel Volume". The log range 0..9.9 with offset 0.1 is
      // the same pseudo-log curve as the UI slider: 10^(2(p-0.5)) - 0.1.
      // Drum channels have no control osc (amy#913: one dedicated osc per drum
      // sound), so there the level is an osc-less amp — broadcast to every
      // per-drum osc (drum_change_code, picked by make_change_code /
      // build_knob_cc_value when the channel holds a drum kit).
      section: "Synth",
      cc: 7,
      display_name: "level",
      change_code: "i%iv" + CTL_OSC + "a%v",
      drum_change_code: "i%ia%v",
      knob_type: "log",
      default_value: 1.0,
      amy_default: 1.0,
      min_value: 0,
      max_value: 9.9,
      offset: 0.1,
      dedicated_slider: true,
    },
    {
      section: "Osc A",
      cc: "",
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
      cc: "",
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
      cc: "",
      display_name: "duty",
      change_code: "i%iv" + OSCA_OSC + "d%v",
      default_value: 0.5,
      amy_default: 0.5,       // duty_coefs[CONST] = 0.5
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc A",
      cc: "",
      display_name: "level",
      change_code: "i%iv" + OSCA_OSC + "a%v",
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
      cc: 71,
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
      cc: "",
      display_name: "kbd",
      change_code: "i%iv" + CTL_OSC + "F,%v",
      default_value: 1.0,
      amy_default: 0,         // filter_logfreq_coefs[NOTE] = 0
      min_value: 0,
      max_value: 1,
    },
    {
      section: "VCF",
      cc: "",
      display_name: "env",
      change_code: "i%iv" + CTL_OSC + "F,,,,%v",
      default_value: 4.0,
      amy_default: 0,         // filter_logfreq_coefs[EG1] = 0
      min_value: -10,
      max_value: 10,
    },

    {
      // Per-bus master volume (final mixdown gain for this bus). Shown by the
      // dedicated "Level" slider in the FX column header (dedicated_slider ->
      // kept out of the knob grid), but registered as a bus knob so it can
      // carry a MIDI CC mapping like every other knob. The i%i prefix makes
      // AMY resolve the bus from the sending channel at execution time. The
      // log range 0..9.9 with offset 0.1 is the same pseudo-log curve as the
      // UI slider: 10^(2(p-0.5)) - 0.1.
      section: "Bus",
      cc: "",
      display_name: "level",
      change_code: "i%iV%v",
      knob_type: "log",
      default_value: 1.0,
      amy_default: 1.0,
      min_value: 0,
      max_value: 9.9,
      offset: 0.1,
      dedicated_slider: true,
    },
    {
      section: "EQ",
      cc: "",
      display_name: "low",
      change_code: "i%ix%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,         // EQ bands default to 0
    },
    {
      section: "EQ",
      cc: "",
      display_name: "mid",
      change_code: "i%ix,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,
    },
    {
      section: "EQ",
      cc: "",
      display_name: "high",
      change_code: "i%ix,,%v",
      min_value: -15,
      max_value: 15,
      default_value: 0,
      amy_default: 0,
    },

    {
      section: "Osc B",
      cc: "",
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
      cc: "",
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
      cc: "",
      display_name: "duty",
      change_code: "i%iv" + OSCB_OSC + "d%v",
      default_value: 0.5,
      amy_default: 0.5,       // duty_coefs[CONST] = 0.5
      min_value: 0.5,
      max_value: 0.99,
    },
    {
      section: "Osc B",
      cc: "",
      display_name: "level",
      change_code: "i%iv" + OSCB_OSC + "a%v",
      knob_type: "log",
      default_value: 1.0,
      amy_default: 1.0,       // amp_coefs[EG0] = 1.0
      min_value: 0.001,
      max_value: 1.0,
      offset: 0.1,
    },

    {
      section: "VCF ENV",
      cc: "",
      display_name: "attack",
      change_code: "i%iv" + CTL_OSC + "B%v,1,,,,0",
      default_value: 0,
      amy_default: 0,         // breakpoints UNSET → no envelope
      min_value: 0,
      max_value: 1000,
    },
    {
      section: "VCF ENV",
      cc: "",
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
      cc: "",
      display_name: "sustain",
      change_code: "i%iv" + CTL_OSC + "B,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 0,         // no filter envelope → sustain 0
    },
    {
      section: "VCF ENV",
      cc: "",
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
      cc: 93,
      knob_type: "log",
      display_name: "level",
      change_code: "i%ik%v",
      min_value: 0,
      max_value: 1,
      offset: 0.1,
      default_value: 0,
      amy_default: 0,         // chorus off
    },
    {
      section: "Chorus",
      cc: "",
      knob_type: "log",
      display_name: "freq",
      change_code: "i%ik,,%v",
      default_value: 0.5,
      amy_default: 0.5,       // CHORUS_DEFAULT_LFO_FREQ
      min_value: 0.1,
      max_value: 20,
    },
    {
      section: "Chorus",
      cc: "",
      display_name: "depth",
      change_code: "i%ik,,,%v",
      min_value: 0.01,
      max_value: 1,
      default_value: 0.5,
      amy_default: 0.5,       // chorus default depth
    },
    {
      section: "LFO",
      cc: 76,
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
      cc: "",
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
      cc: 77,
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
      cc: "",
      display_name: "pwm",
      change_code: "i%iv" + OSCA_OSC + "d,,,,,%vZi%iv" + OSCB_OSC + "d,,,,,%v",
      default_value: 0.0,
      amy_default: 0,         // duty_coefs[MOD] = 0
      min_value: 0.0,
      max_value: 0.49,
    },
    {
      section: "LFO",
      cc: "",
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
      cc: 73,
      display_name: "attack",
      change_code: "i%iv" + CTL_OSC + "A%v,1,,,,0",
      min_value: 0,
      max_value: 1000,
      default_value: 0,
      amy_default: 0,         // breakpoints UNSET → no envelope
    },
    {
      section: "ADSR",
      cc: 75,
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
      cc: 79,
      display_name: "sustain",
      change_code: "i%iv" + CTL_OSC + "A,1,,%v,,0",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 1,         // no breakpoints → EG0 = constant 1.0
    },
    {
      section: "ADSR",
      cc: 72,
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
      cc: 91,
      knob_type: "log",
      display_name: "level",
      change_code: "i%ih%v",
      min_value: 0,
      max_value: 1,
      default_value: 0,
      amy_default: 0,         // reverb off
      offset: 0.1,
    },
    {
      section: "Reverb",
      cc: "",
      display_name: "live",
      change_code: "i%ih,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.85,
      amy_default: 0.85,      // REVERB_DEFAULT_LIVENESS
    },
    {
      section: "Reverb",
      cc: "",
      display_name: "damp",
      change_code: "i%ih,,%v",
      min_value: 0,
      max_value: 1,
      default_value: 0.5,
      amy_default: 0.5,       // reverb default damping
    },
    {
      section: "Echo",
      cc: 94,
      display_name: "level",
      change_code: "i%iM%v",
      min_value: 0,
      max_value: 2,
      default_value: 0,
      amy_default: 0,         // echo off
    },
    {
      section: "Echo",
      cc: "",
      display_name: "delay",
      change_code: "i%iM,%v",
      min_value: 0,
      max_value: 5000,
      default_value: 500,
      amy_default: 500,       // ECHO_DEFAULT_DELAY_MS
    },
    {
      section: "Echo",
      cc: "",
      display_name: "feedback",
      change_code: "i%iM,,,%v",
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
        entry.bus_fx = true;  // scoped to a mix bus, not a channel
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
    let perChannelKnobs = (Array.isArray(window.amy_channel_knobs) && Array.isArray(window.amy_channel_knobs[channel]))
      ? window.amy_channel_knobs[channel] : [];
    // A DX7 channel's surface knobs take precedence: several share change
    // codes with the (disabled) Juno mirrors ("i%iv0F%v", ...), and the
    // mapping must land on the knob the user can actually see.
    if (typeof window.get_channel_dx7_patch === "function"
      && window.get_channel_dx7_patch(channel)
      && typeof window.get_dx7_knobs_for_channel === "function") {
      perChannelKnobs = window.get_dx7_knobs_for_channel(channel).concat(perChannelKnobs);
    }
    // FX (bus) knobs: apply the CC mapping to the bus this channel is on.
    const globalKnobs = window.get_bus_knobs(window.get_channel_bus(channel));
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

  // Number of independent mix buses (AMY_NUM_BUSES). Each has its own FX
  // (EQ/Chorus/Reverb/Echo) and master volume; each channel (synth) is
  // assigned to one bus (default 0).
  const NUM_BUSES = (typeof AMY !== "undefined" && Number.isInteger(AMY.AMY_NUM_BUSES)) ? AMY.AMY_NUM_BUSES : 4;
  window.AMY_UI_NUM_BUSES = NUM_BUSES;

  function resetBusState() {
    window.amy_bus_knobs = [];
    for (let b = 0; b < NUM_BUSES; b += 1) {
      window.amy_bus_knobs[b] = cloneKnobList(global_knob_definitions);
      restoreAmyDefaults(window.amy_bus_knobs[b]);
    }
    window.amy_bus_volumes = new Array(NUM_BUSES).fill(1);
    window.channel_bus = new Array(17).fill(0);
  }

  window.reset_amy_knobs_to_defaults = function() {
    window.amy_channel_knobs = new Array(17);
    for (let i = 1; i <= 16; i += 1) {
      window.amy_channel_knobs[i] = cloneKnobList(channel_knob_definitions);
      restoreAmyDefaults(window.amy_channel_knobs[i]);
    }
    resetBusState();
    if (typeof window.syncBusSelectForChannel === "function") window.syncBusSelectForChannel();
    if (typeof window.syncFxLevelForBus === "function") window.syncFxLevelForBus();
    // Every caller lands on channel 1 (K257 default) afterwards — clear any
    // DX7/drum section greying left over from the previous channel/patch.
    if (typeof window.update_knob_sections_for_patch === "function") {
      window.update_knob_sections_for_patch(257);
    }
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
  resetBusState();
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

  function clampBus(bus) {
    const b = Number(bus);
    if (!Number.isInteger(b) || b < 0) return 0;
    return Math.min(b, NUM_BUSES - 1);
  }
  window.clamp_bus = clampBus;

  window.get_bus_knobs = function(bus) {
    const b = clampBus(bus);
    if (Array.isArray(window.amy_bus_knobs) && window.amy_bus_knobs[b]) {
      return window.amy_bus_knobs[b];
    }
    return [];
  };

  // Which mix bus a channel (synth) is assigned to. Mirror-only setter; the
  // wire command (i<ch>y<bus>) is sent by window.set_channel_bus in spss.js.
  window.get_channel_bus = function(channel) {
    const ch = Number(channel);
    if (Array.isArray(window.channel_bus) && Number.isInteger(window.channel_bus[ch])) {
      return clampBus(window.channel_bus[ch]);
    }
    return 0;
  };

  window.set_channel_bus_local = function(channel, bus) {
    const ch = Number(channel);
    if (!Number.isInteger(ch) || ch < 1 || ch > 16) return;
    if (!Array.isArray(window.channel_bus)) window.channel_bus = new Array(17).fill(0);
    window.channel_bus[ch] = clampBus(bus);
  };

  // Update a bus's master volume mirror (and the Effects Level slider when
  // that bus is the one on display).
  window.setBusVolumeFromAmy = function(bus, val) {
    const b = clampBus(bus);
    val = Math.min(7, Math.max(0.001, Number(val) || 1));
    if (!Array.isArray(window.amy_bus_volumes)) window.amy_bus_volumes = new Array(NUM_BUSES).fill(1);
    window.amy_bus_volumes[b] = val;
    if (typeof window.syncFxLevelForBus === "function") window.syncFxLevelForBus();
  };

  // The Effects column shows the FX of the bus the CURRENT channel is on.
  window.get_global_knobs = function() {
    return window.get_bus_knobs(window.get_channel_bus(window.current_synth || 1));
  };

  window.get_current_knobs = function() {
    return window.get_channel_knobs(window.current_synth).concat(window.get_global_knobs());
  };

  window.refresh_knobs_for_channel = function() {
    const ch = Number(window.current_synth || 1);
    // Render the channel's knob SURFACE for its patch (see the surface plan
    // at update_knob_sections_for_patch in spss.js): drum kit channels get
    // the per-drum parameter grid, DX7 preset channels get the tabbed FM
    // surface (editor_dx7.js), everything else gets the Juno surface. The FX
    // (bus) grid renders the same either way.
    const drumKit = (typeof window.get_channel_drum_kit === "function")
      ? window.get_channel_drum_kit(ch) : null;
    const dx7Patch = (!drumKit && typeof window.get_channel_dx7_patch === "function")
      ? window.get_channel_dx7_patch(ch) : null;
    const channelGrid = document.getElementById("knob-grid-channel");
    if (channelGrid) {
      channelGrid.classList.toggle("knob-grid-drums", !!drumKit);
      channelGrid.classList.toggle("knob-grid-dx7", !!dx7Patch);
    }
    let channelKnobs;   // rendered in the channel grid
    if (drumKit && typeof window.get_drum_knobs_for_channel === "function") {
      window.refresh_drum_knob_values(ch);
      channelKnobs = window.get_drum_knobs_for_channel(ch);
    } else if (dx7Patch && typeof window.get_dx7_knobs_for_channel === "function") {
      window.refresh_dx7_knob_values(ch);
      channelKnobs = window.get_dx7_knobs_for_channel(ch);
    } else {
      channelKnobs = drumKit ? [] : window.get_channel_knobs(ch);
    }
    const globalKnobs = window.get_global_knobs();
    if (typeof init_knobs === "function") {
      // The DX7 grid gets an onChange hook so its envelope plots redraw live.
      init_knobs(channelKnobs, "knob-grid-channel",
        dx7Patch ? window.dx7_on_knob_change : undefined);
      init_knobs(globalKnobs, "knob-grid-global");
    }
    if (dx7Patch && typeof window.render_dx7_chrome === "function") {
      window.render_dx7_chrome(ch);  // the ENV PLOT row
    }
    if (typeof window.onKnobCcChange === "function") {
      const disabledSections = window._disabled_sections || {};
      for (const knob of channelKnobs.concat(globalKnobs)) {
        if (knob.drum) continue;  // drum knobs never emit device CC mappings
        if (disabledSections[knob.section]) continue;
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
// opts.onlyDirtyFxBuses: apply FX only to buses that appear in the events
// (used by synth readback, which carries ONE bus's FX line — the other buses'
// mirrors must be left alone). Default (sketch positioning) applies every bus:
// a bus with no lines is at its boot defaults.
function set_knobs_from_events(events, synth, opts) {
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
      var busZero = (typeof window.get_bus_knobs === "function") ? window.get_bus_knobs(0) : [];
      var allKnobs = busZero.concat(knobs || []);
      for (var i = 0; i < allKnobs.length; i++) {
        var k = allKnobs[i];
        if (k && k.section === section && k.display_name === name && k.amy_default !== undefined) return k.amy_default;
      }
      return 0;
    }
    // FX are per mix bus: one accumulator set per bus, initialized to AMY
    // defaults, overwritten by fx events routed by their y<bus> (default 0).
    const numBuses = Number.isInteger(window.AMY_UI_NUM_BUSES) ? window.AMY_UI_NUM_BUSES : 4;
    const onlyDirtyFxBuses = !!(opts && opts.onlyDirtyFxBuses);
    let fx = [];
    for (let b = 0; b < numBuses; b += 1) {
      fx.push({
        dirty: false,
        eq: [knobDefault("EQ","low"), knobDefault("EQ","mid"), knobDefault("EQ","high")],
        chorus: [knobDefault("Chorus","level"), knobDefault("Chorus","freq"), knobDefault("Chorus","depth")],
        reverb: [knobDefault("Reverb","level"), knobDefault("Reverb","live"), knobDefault("Reverb","damp")],
        echo: [knobDefault("Echo","level"), knobDefault("Echo","delay"), knobDefault("Echo","feedback")],
        volume: null,   // null = leave the mirror untouched
      });
    }
    const BP_UNSET = 32767;

    function bpTimeIsSet(v) {
      return Number.isFinite(v) && v !== BP_UNSET;
    }

    for (const event of events) {

      // non-osc (FX) values — routed to the bus named by y<bus>, default bus 0
      // (legacy pre-bus lines carry no y and always meant bus 0).
      const fxBus = Number.isFinite(event.bus) ? Math.min(Math.max(event.bus, 0), numBuses - 1) : 0;
      if (event.eq || event.chorus || event.reverb || event.echo) {
        fx[fxBus].dirty = true;
      }
      if (event.eq) {
        if (Number.isFinite(event.eq[0]))  { fx[fxBus].eq[0] = event.eq[0]; }
        if (Number.isFinite(event.eq[1]))  { fx[fxBus].eq[1] = event.eq[1]; }
        if (Number.isFinite(event.eq[2]))  { fx[fxBus].eq[2] = event.eq[2]; }
      }
      if (event.chorus) {
        if (Number.isFinite(event.chorus[0])) { fx[fxBus].chorus[0] = event.chorus[0]; }  // level
        if (Number.isFinite(event.chorus[2])) { fx[fxBus].chorus[1] = event.chorus[2]; }  // lfo_freq
        if (Number.isFinite(event.chorus[3])) { fx[fxBus].chorus[2] = event.chorus[3]; }  // depth
      }
      if (event.reverb) {
        if (Number.isFinite(event.reverb[0])) { fx[fxBus].reverb[0] = event.reverb[0]; }
        if (Number.isFinite(event.reverb[1])) { fx[fxBus].reverb[1] = event.reverb[1]; }
        if (Number.isFinite(event.reverb[2])) { fx[fxBus].reverb[2] = event.reverb[2]; }
      }
      if (event.echo) {
        if (Number.isFinite(event.echo[0])) { fx[fxBus].echo[0] = event.echo[0]; }
        if (Number.isFinite(event.echo[1])) { fx[fxBus].echo[1] = event.echo[1]; }
        if (Number.isFinite(event.echo[3])) { fx[fxBus].echo[2] = event.echo[3]; }
      }
      if (Array.isArray(event.volume)) {
        // V is a vector whose slots are RELATIVE to the event's bus:
        // "y1V0.4" = bus 1 volume 0.4, legacy "V,0.4" = bus 1 too.
        for (let vi = 0; vi < event.volume.length; vi += 1) {
          if (!Number.isFinite(event.volume[vi])) continue;
          const vBus = fxBus + vi;
          if (vBus >= 0 && vBus < numBuses) fx[vBus].volume = event.volume[vi];
        }
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

    for (let b = 0; b < numBuses; b += 1) {
      if (onlyDirtyFxBuses && !fx[b].dirty) {
        // Synth readback: this bus wasn't in the events — leave its mirror alone.
        if (Number.isFinite(fx[b].volume) && typeof window.setBusVolumeFromAmy === "function") {
          window.setBusVolumeFromAmy(b, fx[b].volume);
        }
        continue;
      }
      set_bus_knob_value(b, "EQ", "low", fx[b].eq[0]);
      set_bus_knob_value(b, "EQ", "mid", fx[b].eq[1]);
      set_bus_knob_value(b, "EQ", "high", fx[b].eq[2]);

      set_bus_knob_value(b, "Chorus", "level", fx[b].chorus[0]);
      set_bus_knob_value(b, "Chorus", "freq", fx[b].chorus[1]);
      set_bus_knob_value(b, "Chorus", "depth", fx[b].chorus[2]);

      set_bus_knob_value(b, "Reverb", "level", fx[b].reverb[0]);
      set_bus_knob_value(b, "Reverb", "live", fx[b].reverb[1]);
      set_bus_knob_value(b, "Reverb", "damp", fx[b].reverb[2]);

      set_bus_knob_value(b, "Echo", "level", fx[b].echo[0]);
      set_bus_knob_value(b, "Echo", "delay", fx[b].echo[1]);
      set_bus_knob_value(b, "Echo", "feedback", fx[b].echo[2]);

      if (Number.isFinite(fx[b].volume) && typeof window.setBusVolumeFromAmy === "function") {
        window.setBusVolumeFromAmy(b, fx[b].volume);
      }
    }

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
  // A synth readback identifies the synth's mix bus on its FX line (y<bus>);
  // adopt it into the UI mirror so the Bus pull-down and Effects column track.
  let busFound = null;
  for (const ev of events) {
    if (Number.isFinite(ev.bus)) busFound = ev.bus;
  }
  if (busFound !== null && typeof window.set_channel_bus_local === "function") {
    window.set_channel_bus_local(synth, busFound);
    if (typeof window.syncBusSelectForChannel === "function") window.syncBusSelectForChannel();
    if (typeof window.syncFxLevelForBus === "function") window.syncFxLevelForBus();
  }
  // Readback events describe ONE synth (and one bus's FX line) — don't reset
  // the other buses' FX mirrors to defaults.
  return set_knobs_from_events(events, synth, { onlyDirtyFxBuses: true });
}

