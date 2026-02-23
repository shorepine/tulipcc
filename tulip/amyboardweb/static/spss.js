
var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
var amyboard_started = false;
var amy_event_generator_for_patch_number = null;
var amy_size_of_amy_event = null;

function amy_add_log_message(message) {
  console.log(message);
  if (typeof amy_add_message === "function") {
    amy_add_message(message);
  }
}

var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;
var midiInputOptionIds = [];
var midiOutputOptionIds = [];
var editor = null;
var treeView = null;
var editor_shown = false;
var amy_audioin_toggle = false;
var editor_height = 200;
var term = null; 
var cv_1_voltage = 0;
var cv_2_voltage = 0;
var amy_module = null;
var pendingPatchKnobIndex = null;
var patchKnobSyncAttempts = 0;
var patches_file_rewrite_timer = null;
var amyboard_world_files = [];
var amyboard_world_loading_index = null;
var amyboard_world_selected_tag = "";
var amyboard_world_tag_palette = {};
var selected_environment_file = null;
var pending_environment_editor_load = false;
var environment_editor_dirty = false;
var environment_autosave_inflight = false;
const AMYBOARD_WORLD_API_BASE = (typeof window !== "undefined" && typeof window.AMYBOARD_WORLD_API_BASE === "string")
    ? String(window.AMYBOARD_WORLD_API_BASE).replace(/\/+$/, "")
    : String(window.location.origin || "").replace(/\/+$/, "");
const CURRENT_BASE_DIR = "/amyboard/user/current";
const CURRENT_ENV_DIR = "/amyboard/user/current/env";
const DEFAULT_BASE_DIR = "/amyboard/user/default";
const DEFAULT_ENV_DIR = "/amyboard/user/default/env";
const CURRENT_WEB_PATCH_FILE = CURRENT_ENV_DIR + "/web.patch";
const DEFAULT_WEB_PATCH_FILE = DEFAULT_ENV_DIR + "/web.patch";
const CURRENT_OTHER_PATCH_FILE = CURRENT_ENV_DIR + "/other.patch";
const DEFAULT_OTHER_PATCH_FILE = DEFAULT_ENV_DIR + "/other.patch";
const DEFAULT_ENV_SOURCE = "# Empty environment\nprint(\"Welcome to AMYboard!\")\n";
const WEB_PATCH_EDITOR_HEADER = "# Do not edit - Set by AMYboard web patch editor";
const DEFAULT_WEB_PATCH_SOURCE = WEB_PATCH_EDITOR_HEADER + "\n";
const DEFAULT_OTHER_PATCH_SOURCE = "";
const EDITOR_ALLOWED_EXTENSIONS = [".py", ".txt", ".json", ".patch"];
const AMYBOARD_SYSEX_MFR_ID = [0x00, 0x03, 0x45];
const AMYBOARD_TRANSFER_CHUNK_BYTES = 188;
window.current_synth = 1;
window.active_channels = Array.isArray(window.active_channels) ? window.active_channels : new Array(17).fill(false);
window.active_channels[1] = true;
window.channel_control_mapping_sent = Array.isArray(window.channel_control_mapping_sent) ? window.channel_control_mapping_sent : new Array(17).fill(false);
window.suppress_knob_cc_send = false;

function get_patch_number_for_channel(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) {
    ch = Number(window.current_synth || 1);
  }
  return ch + 1023;
}

function is_channel_active(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) {
    return false;
  }
  return !!(window.active_channels && window.active_channels[ch]);
}

window.set_channel_active = function(channel, active) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) {
    return;
  }
  window.active_channels[ch] = !!active;
};

function request_current_patches_file_rewrite() {
  if (patches_file_rewrite_timer) {
    clearTimeout(patches_file_rewrite_timer);
  }
  patches_file_rewrite_timer = setTimeout(async function() {
    patches_file_rewrite_timer = null;
    try {
      await write_current_patches_file_from_knobs();
    } catch (e) {}
  }, 80);
}
window.request_current_patches_file_rewrite = request_current_patches_file_rewrite;

// Once AMY module is loaded, register its functions and start AMY (not yet audio, you need to click for that)
amyModule().then(async function(am) {
  amy_live_start_web = am.cwrap(
    'amy_live_start_web', null, null, {async: true}    
  );
  amy_live_start_web_audioin = am.cwrap(
    'amy_live_start_web_audioin', null, null, {async: true}    
  );
  amy_live_stop = am.cwrap(
    'amy_live_stop', null,  null, {async: true}    
  );
  amy_bleep = am.cwrap(
    'amy_bleep', null, ['number']
  );
  amy_start_web_no_synths = am.cwrap(
    'amy_start_web_no_synths', null, null
  );
  amy_add_message = am.cwrap(
    'amy_add_message', null, ['string']
  );
  amy_reset_sysclock = am.cwrap(
    'amy_reset_sysclock', null, null
  );
  amy_ticks = am.cwrap(
    'sequencer_ticks', 'number', [null]
  );
  amy_sysclock = am.cwrap(
    'amy_sysclock', 'number', [null]
  );
  amy_process_single_midi_byte = am.cwrap(
    'amy_process_single_midi_byte', null, ['number, number']
  );
  amy_event_generator_for_patch_number = am.cwrap(
    'event_generator_for_patch_number', 'number', ['number', 'number', 'number']
  );
  amy_size_of_amy_event = am.cwrap(
    'size_of_amy_event', 'number',
  );
  amy_start_web_no_synths();
  amy_bleep(0); // won't play until live audio starts
  amy_module = am;
  res_ptr_in = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  res_ptr_out = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  attemptPatchKnobSync();
});

// Converts AMY patch number to list of (JS converted) amy_events
// This is to let us fill in the preset knobs
function get_events_for_patch_number(patch_number) {
  if (!amy_module || !amy_event_generator_for_patch_number) {
    return [];
  }

  const layout = window.AMY_EVENT_LAYOUT;
  if (!layout || !layout.constants || !layout.offsets) {
    return [];
  }

  const NUM_COMBO_COEFS = layout.constants.NUM_COMBO_COEFS;
  const MAX_BREAKPOINT_SETS = layout.constants.MAX_BREAKPOINT_SETS;
  const MAX_ALGO_OPS = layout.constants.MAX_ALGO_OPS;
  const MAX_VOICES_PER_INSTRUMENT = layout.constants.MAX_VOICES_PER_INSTRUMENT;
  const MAX_BPS = layout.constants.MAX_BPS || layout.constants.MAX_BREAKPOINTS;
  const MAX_EVENTS_PER_PATCH = layout.constants.MAX_EVENTS_PER_PATCH;
  const EVENT_EMPTY = 0;
  const offsets = layout.offsets;

  const heapU8 = amy_module.HEAPU8;
  if (!heapU8) {
    return [];
  }
  const heapU32 = amy_module.HEAPU32 || new Uint32Array(heapU8.buffer);
  const view = new DataView(heapU8.buffer);

  function readFloatArray(ptr, count) {
    const arr = new Array(count);
    for (let i = 0; i < count; i += 1) {
      arr[i] = view.getFloat32(ptr + i * 4, true);
    }
    return arr;
  }

  function readUint16Array(ptr, count) {
    const arr = new Array(count);
    for (let i = 0; i < count; i += 1) {
      arr[i] = view.getUint16(ptr + i * 2, true);
    }
    return arr;
  }

  function readInt16Array(ptr, count) {
    const arr = new Array(count);
    for (let i = 0; i < count; i += 1) {
      arr[i] = view.getInt16(ptr + i * 2, true);
    }
    return arr;
  }

  function readEvent(base) {
    return {
      time: view.getUint32(base + offsets.time, true),
      osc: view.getUint16(base + offsets.osc, true),
      wave: view.getUint16(base + offsets.wave, true),
      preset: view.getInt16(base + offsets.preset, true),
      midi_note: view.getFloat32(base + offsets.midi_note, true),
      patch_number: view.getUint16(base + offsets.patch_number, true),
      amp_coefs: readFloatArray(base + offsets.amp_coefs, NUM_COMBO_COEFS),
      freq_coefs: readFloatArray(base + offsets.freq_coefs, NUM_COMBO_COEFS),
      filter_freq_coefs: readFloatArray(base + offsets.filter_freq_coefs, NUM_COMBO_COEFS),
      duty_coefs: readFloatArray(base + offsets.duty_coefs, NUM_COMBO_COEFS),
      pan_coefs: readFloatArray(base + offsets.pan_coefs, NUM_COMBO_COEFS),
      feedback: view.getFloat32(base + offsets.feedback, true),
      velocity: view.getFloat32(base + offsets.velocity, true),
      phase: view.getFloat32(base + offsets.phase, true),
      detune: view.getFloat32(base + offsets.detune, true),
      volume: view.getFloat32(base + offsets.volume, true),
      pitch_bend: view.getFloat32(base + offsets.pitch_bend, true),
      tempo: view.getFloat32(base + offsets.tempo, true),
      latency_ms: view.getUint16(base + offsets.latency_ms, true),
      ratio: view.getFloat32(base + offsets.ratio, true),
      resonance: view.getFloat32(base + offsets.resonance, true),
      portamento_ms: view.getUint16(base + offsets.portamento_ms, true),
      chained_osc: view.getUint16(base + offsets.chained_osc, true),
      mod_source: view.getUint16(base + offsets.mod_source, true),
      algorithm: view.getUint8(base + offsets.algorithm),
      filter_type: view.getUint8(base + offsets.filter_type),
      eq_l: view.getFloat32(base + offsets.eq_l, true),
      eq_m: view.getFloat32(base + offsets.eq_m, true),
      eq_h: view.getFloat32(base + offsets.eq_h, true),
      bp_is_set: readUint16Array(base + offsets.bp_is_set, MAX_BREAKPOINT_SETS),
      algo_source: readInt16Array(base + offsets.algo_source, MAX_ALGO_OPS),
      voices: readUint16Array(base + offsets.voices, MAX_VOICES_PER_INSTRUMENT),
      eg0_times: readInt16Array(base + offsets.eg0_times, MAX_BPS),
      eg0_values: readFloatArray(base + offsets.eg0_values, MAX_BPS),
      eg1_times: readInt16Array(base + offsets.eg1_times, MAX_BPS),
      eg1_values: readFloatArray(base + offsets.eg1_values, MAX_BPS),
      eg_type: Array.from(amy_module.HEAPU8.subarray(base + offsets.eg_type, base + offsets.eg_type + MAX_BREAKPOINT_SETS)),
      synth: view.getUint8(base + offsets.synth),
      synth_flags: view.getUint32(base + offsets.synth_flags, true),
      synth_delay_ms: view.getUint16(base + offsets.synth_delay_ms, true),
      to_synth: view.getUint8(base + offsets.to_synth),
      grab_midi_notes: view.getUint8(base + offsets.grab_midi_notes),
      pedal: view.getUint8(base + offsets.pedal),
      num_voices: view.getUint16(base + offsets.num_voices, true),
      sequence: [
        view.getUint32(base + offsets.sequence, true),
        view.getUint32(base + offsets.sequence + 4, true),
        view.getUint32(base + offsets.sequence + 8, true)
      ],
      status: view.getUint8(base + offsets.status),
      note_source: view.getUint8(base + offsets.note_source),
      reset_osc: view.getUint32(base + offsets.reset_osc, true),
      echo_level: view.getFloat32(base + offsets.echo_level, true),
      echo_delay_ms: view.getFloat32(base + offsets.echo_delay_ms, true),
      echo_max_delay_ms: view.getFloat32(base + offsets.echo_max_delay_ms, true),
      echo_feedback: view.getFloat32(base + offsets.echo_feedback, true),
      echo_filter_coef: view.getFloat32(base + offsets.echo_filter_coef, true),
      chorus_level: view.getFloat32(base + offsets.chorus_level, true),
      chorus_max_delay: view.getFloat32(base + offsets.chorus_max_delay, true),
      chorus_lfo_freq: view.getFloat32(base + offsets.chorus_lfo_freq, true),
      chorus_depth: view.getFloat32(base + offsets.chorus_depth, true),
      reverb_level: view.getFloat32(base + offsets.reverb_level, true),
      reverb_liveness: view.getFloat32(base + offsets.reverb_liveness, true),
      reverb_damping: view.getFloat32(base + offsets.reverb_damping, true),
      reverb_xover_hz: view.getFloat32(base + offsets.reverb_xover_hz, true),
    };
  }

  const SIZEOF_EVENT_STRUCT = amy_size_of_amy_event();
  const eventPtr = amy_module._malloc(SIZEOF_EVENT_STRUCT);
  const events = [];
  let state = 0;
  do {
    state = amy_event_generator_for_patch_number(patch_number, eventPtr, state);
    events.push(readEvent(eventPtr));
  } while (state != 0);

  amy_module._free(eventPtr);

  return events;
}

function attemptPatchKnobSync() {
  if (pendingPatchKnobIndex === null) {
    return;
  }
  if (!audio_started) {
    return;
  }
  if (!amy_module || typeof window.get_current_knobs !== "function") {
    if (patchKnobSyncAttempts < 60) {
      patchKnobSyncAttempts += 1;
      setTimeout(attemptPatchKnobSync, 50);
    }
    return;
  }
  patchKnobSyncAttempts = 0;
  set_knobs_from_patch_number(pendingPatchKnobIndex);
}

function requestPatchKnobSync(patchIndex) {
  pendingPatchKnobIndex = patchIndex;
  if (audio_started) {
    attemptPatchKnobSync();
  }
}

window.onPatchChange = function(patchIndex) {
  requestPatchKnobSync(patchIndex);
};


function onKnobCcChange(knob) {
  /*
  ic<C>,<L>,<N>,<X>,<O>,<CODE>
  where C= MIDI control code (0-127), L = log-scale flag (so the mapping is exponential if L == 1), 
  N = min value for the control (corresponding to MIDI value 0), 
  X = max value (for MIDI value 127), 
  O = offset for log scale (so if L == 1, value = (min + offset) * exp(log(((max + offset)/(min + offset) * midi_value / 127) - offset), 
  and CODE is a wire code string with %v as a placeholder for the value to be substituted 
  (also %i for channel/synth number) and maybe %V to force an integer, 
  for things like selecting wave, and maybe more.
  */
  var m = build_knob_cc_message(window.current_synth, knob);
  if (!m) {
    return;
  }
  //console.log("Knob CC updated: " + knob.section + ": " + knob.display_name + " to " + knob.cc + ". Sending: " + m);
  if (window.suppress_knob_cc_send) {
    return;
  }
  if (typeof amy_add_message === "function") {
    amy_add_log_message(m);
  }
  if (typeof window.request_current_patches_file_rewrite === "function") {
    window.request_current_patches_file_rewrite();
  }
}

// Called from AMY to update AMYboard about what tick it is, for the sequencer
function amy_sequencer_js_hook(tick) {
    mp.tulipTick(tick);
}

async function clear_storage() {
  if(confirm("This will delete your AMYboard user folder and start again.\nAre you sure?")) {
    indexedDB.deleteDatabase('/amyboard/user');
    location.reload(true);
  }
}

window.amy_cv_knob_change = function(index, value) {
  
};

function move_knob(channel, cc, value) {
  // Hook for MIDI CC -> knob mapping.
  const knobList = window.get_current_knobs ? window.get_current_knobs() : [];
  if (window.cc_learn_handler && channel === window.current_synth) {
    window.cc_learn_handler(cc);
    return;
  }
  if (channel !== window.current_synth || !Array.isArray(knobList)) {
    return;
  }
  for (const knob of knobList) {
    if (knob.cc != cc) {
      continue;
    }
    let scaled_value = null;
    if (knob.knob_type === "selection") {
      const options = Array.isArray(knob.options) ? knob.options : [];
      if (options.length === 0) {
        return;
      }
      const selectedIndex = Math.round((options.length - 1) * (value / 127));
      const optionValues = Array.isArray(knob.option_values) ? knob.option_values : null;
      if (optionValues && optionValues[selectedIndex] !== undefined) {
        const mappedValue = Number(optionValues[selectedIndex]);
        scaled_value = Number.isFinite(mappedValue) ? mappedValue : selectedIndex;
      } else {
        scaled_value = selectedIndex;
      }
    } else if (knob.knob_type === "pushbutton") {
      scaled_value = value >= 64 ? 1 : 0;
    } else {
      const min = Number(knob.min_value);
      const max = Number(knob.max_value);
      const offset = Number(knob.offset || 0);
      if (!Number.isFinite(min) || !Number.isFinite(max)) {
        return;
      }
      if (knob.knob_type === "log") {
        const minShifted = min + offset;
        const maxShifted = max + offset;
        if (minShifted > 0 && maxShifted > 0) {
          const ratio = maxShifted / minShifted;
          scaled_value = minShifted * Math.exp(Math.log(ratio) * (value / 127)) - offset;
        } else {
          scaled_value = min;
        }
      } else {
        scaled_value = min + (max - min) * (value / 127);
      }
    }
    set_knob_ui_value(knob, scaled_value, false);
    return;
  }
}

function save_to_patch(patchNumber) {
  const channel = Number(window.current_synth || 1);
  const patch = get_patch_number_for_channel(channel);
  const messages = build_patch_save_messages(channel, patch);
  for (const message of messages) {
    amy_add_log_message(message);
  }
}

function load_from_patch(patchNumber) {
  // Hook for loading a saved memory patch number.
  const channel = Number(window.current_synth || 1);
  set_knobs_from_patch_number(get_patch_number_for_channel(channel));
}

function build_patch_save_messages(channel, patchNumber) {
  var synthChannel = Number(channel);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = Number(window.current_synth || 1);
  }
  var patch = Number(patchNumber);
  if (!Number.isFinite(patch)) {
    patch = get_patch_number_for_channel(synthChannel);
  }
  patch = Math.max(0, Math.floor(patch));

  var messages = [];
  messages.push("K" + patch + "S524288");

  if (typeof window.amyboard_patch_string === "string") {
    var chunks = window.amyboard_patch_string.split("Z");
    for (const chunk of chunks) {
      if (!chunk) {
        continue;
      }
      messages.push("K" + patch + chunk);
    }
  }

  const knobList = get_knobs_for_channel(synthChannel);
  if (Array.isArray(knobList) && typeof window.make_change_code === "function") {
    for (const knob of knobList) {
      if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half") {
        continue;
      }
      const value = Number(knob.default_value);
      if (!Number.isFinite(value)) {
        continue;
      }
      const payload = window.make_change_code(synthChannel, value, knob, true);
      if (!payload) {
        continue;
      }
      messages.push("K" + patch + payload);
    }
  }

  messages.push("i" + synthChannel + "iv6K" + patch);
  return messages;
}

function send_knob_value_messages_for_channel(channel) {
  var synthChannel = Number(channel);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = Number(window.current_synth || 1);
  }
  const knobList = get_knobs_for_channel(synthChannel);
  if (!Array.isArray(knobList) || typeof window.make_change_code !== "function") {
    return;
  }
  for (const knob of knobList) {
    if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half") {
      continue;
    }
    const value = Number(knob.default_value);
    if (!Number.isFinite(value)) {
      continue;
    }
    const payload = window.make_change_code(synthChannel, value, knob, false);
    if (!payload) {
      continue;
    }
    amy_add_log_message(payload);
  }
}

async function apply_default_patch_state_from_clear_patches() {
  if (typeof window.reset_amy_knobs_to_defaults !== "function") {
    return false;
  }
  window.reset_amy_knobs_to_defaults();
  if (typeof window.set_channel_active === "function") {
    window.set_channel_active(1, true);
    for (let ch = 2; ch <= 16; ch += 1) {
      window.set_channel_active(ch, false);
    }
  }
  window.current_synth = 1;
  var channelSelect = document.getElementById("midi-channel-select");
  if (channelSelect) {
    channelSelect.value = "1";
  }
  if (Array.isArray(window.channel_control_mapping_sent)) {
    for (let ch = 1; ch <= 16; ch += 1) {
      window.channel_control_mapping_sent[ch] = false;
    }
  }
  if (typeof set_knobs_from_patch_number === "function") {
    set_knobs_from_patch_number(0);
  }
  if (typeof window.refresh_knobs_for_active_channel === "function") {
    await window.refresh_knobs_for_active_channel({ sendControlMappings: true });
  } else if (typeof window.refresh_knobs_for_channel === "function") {
    window.refresh_knobs_for_channel();
  }
  if (typeof window.write_current_patches_file_from_knobs === "function") {
    await window.write_current_patches_file_from_knobs();
  }
  return true;
}

var clear_patch_state = async function() {
  var confirmed = await confirm_environment_action(
    "Clear patches",
    "Clear patch state and restore default knobs?",
    "Clear"
  );
  if (!confirmed) {
    return;
  }
  await apply_default_patch_state_from_clear_patches();
};
window.clear_patch_state = clear_patch_state;

function get_knobs_for_channel(channel) {
  var ch = Number(channel);
  if (Array.isArray(window.amy_knobs) && Array.isArray(window.amy_knobs[ch])) {
    return window.amy_knobs[ch];
  }
  return window.get_current_knobs ? window.get_current_knobs() : [];
}

function build_knob_cc_message(channel, knob) {
  if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half"
    || knob.knob_type === "selection" || knob.knob_type === "pushbutton") {
    return "";
  }
  var synthChannel = Number(channel);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = Number(window.current_synth || 1);
  }
  var log = knob.knob_type === "log" ? 1 : 0;
  var offset = (typeof knob.offset === "undefined") ? 0 : knob.offset;
  return "i" + synthChannel + "ic" + knob.cc + "," + log + "," + knob.min_value + "," + knob.max_value + "," + offset + "," + knob.change_code;
}

function build_channel_control_messages(channel) {
  const knobs = get_knobs_for_channel(channel);
  const messages = [];
  if (!Array.isArray(knobs)) {
    return messages;
  }
  for (const knob of knobs) {
    const message = build_knob_cc_message(channel, knob);
    if (message) {
      messages.push(message);
    }
  }
  return messages;
}

async function write_current_patches_file_from_knobs() {
  ensure_current_environment_layout(true);
  var allMessages = [];
  for (var channel = 1; channel <= 16; channel++) {
    if (!is_channel_active(channel)) {
      continue;
    }
    var patchNumber = get_patch_number_for_channel(channel);
    var messages = build_patch_save_messages(channel, patchNumber);
    for (const message of messages) {
      allMessages.push(message);
    }
    var controlMessages = build_channel_control_messages(channel);
    for (const message of controlMessages) {
      allMessages.push(message);
    }
  }
  if (!allMessages.length) {
    allMessages.push("# no active channels");
  }
  var outputLines = [WEB_PATCH_EDITOR_HEADER].concat(allMessages);
  mp.FS.writeFile(CURRENT_WEB_PATCH_FILE, outputLines.join("\n") + "\n");
  await sync_persistent_fs();
  refresh_editor_if_viewing_web_patch_file();
}
window.write_current_patches_file_from_knobs = write_current_patches_file_from_knobs;

function is_selected_file_web_patch_file() {
  if (!selected_environment_file || typeof selected_environment_file !== "string") {
    return false;
  }
  return selected_environment_file.toLowerCase() === "web.patch";
}

function refresh_editor_if_viewing_web_patch_file() {
  if (!editor || !is_selected_file_web_patch_file()) {
    return false;
  }
  try {
    var latest = mp.FS.readFile(CURRENT_WEB_PATCH_FILE, { encoding: "utf8" });
    if (editor.getValue() !== latest) {
      editor.setValue(latest);
    }
    environment_editor_dirty = false;
    editor.scrollTo(0, 0);
    setTimeout(function () { editor.save(); }, 50);
    setTimeout(function () {
      editor.refresh();
      editor.scrollTo(0, 0);
    }, 100);
    return true;
  } catch (e) {
    return false;
  }
}

window.refresh_knobs_for_active_channel = async function(options) {
  var opts = options || {};
  var channel = Number(window.current_synth || 1);
  var active = is_channel_active(channel);
  var sendControlMappings = !!opts.sendControlMappings && active && !window.channel_control_mapping_sent[channel];

  if (typeof window.refresh_knobs_for_channel === "function") {
    var previousSuppress = !!window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = !sendControlMappings;
    window.refresh_knobs_for_channel();
    window.suppress_knob_cc_send = previousSuppress;
  }
  if (sendControlMappings) {
    window.channel_control_mapping_sent[channel] = true;
  }
  await write_current_patches_file_from_knobs();
};


async function amy_external_midi_input_js_hook(bytes, len, sysex) {
    mp.midiInHook(bytes, len, sysex);
} 

function safe_midi_port_id(port) {
  try {
    return port && typeof port.id !== "undefined" ? port.id : null;
  } catch (e) {
    return null;
  }
}

function safe_midi_port_name(port) {
  try {
    if (!port) return "Unknown";
    return port.name || safe_midi_port_id(port) || "Unknown";
  } catch (e) {
    return "Unknown";
  }
}

function get_selected_midi_input_device() {
  if (!WebMidi || !Array.isArray(midiInputOptionIds) || midiInputOptionIds.length === 0) {
    return null;
  }
  var midi_in = document.amyboard_settings && document.amyboard_settings.midi_input
    ? document.amyboard_settings.midi_input
    : null;
  var selectedIndex = midi_in ? Number(midi_in.selectedIndex) : 0;
  if (!Number.isInteger(selectedIndex) || selectedIndex < 0 || selectedIndex >= midiInputOptionIds.length) {
    selectedIndex = 0;
  }
  var selectedId = midiInputOptionIds[selectedIndex];
  if (!selectedId) {
    return null;
  }
  var input = null;
  try {
    input = WebMidi.getInputById(selectedId);
  } catch (e) {
    input = null;
  }
  return input || null;
}

function get_selected_midi_output_device() {
  if (!WebMidi || !Array.isArray(midiOutputOptionIds) || midiOutputOptionIds.length === 0) {
    return null;
  }
  var midi_out = document.amyboard_settings && document.amyboard_settings.midi_output
    ? document.amyboard_settings.midi_output
    : null;
  var selectedIndex = midi_out ? Number(midi_out.selectedIndex) : 0;
  if (!Number.isInteger(selectedIndex) || selectedIndex < 0 || selectedIndex >= midiOutputOptionIds.length) {
    selectedIndex = 0;
  }
  var selectedId = midiOutputOptionIds[selectedIndex];
  if (!selectedId) {
    return null;
  }
  var output = null;
  try {
    output = WebMidi.getOutputById(selectedId);
  } catch (e) {
    output = null;
  }
  if (!output || (typeof output.sendSysex !== "function" && typeof output.send !== "function")) {
    return null;
  }
  return output;
}


async function setup_midi_devices() {
  var selectedInput = get_selected_midi_input_device();
  if (selectedInput) {
    if (midiInputDevice != null && typeof midiInputDevice.destroy === "function") {
      try { midiInputDevice.destroy(); } catch (e) {}
    }
    midiInputDevice = selectedInput;
    midiInputDevice.addListener("midimessage", e => {
      const data = e.message && e.message.data ? e.message.data : [];
      const status = data.length > 0 ? data[0] : null;
      if (Number.isFinite(status) && (status & 0xF0) === 0xB0 && data.length >= 3) {
        const channel = (status & 0x0F) + 1;
        const cc = data[1];
        const value = data[2];
        move_knob(channel, cc, value);
      }
      for(byte in e.message.data) {
        if(audio_started) amy_process_single_midi_byte(e.message.data[byte], 1);
      }
    });
  } else {
    midiInputDevice = null;
  }
  midiOutputDevice = get_selected_midi_output_device();
}

async function start_midi() {
  function onEnabled() {
    // Populate the dropdowns
    var midi_in = document.amyboard_settings.midi_input;
    var midi_out = document.amyboard_settings.midi_output;
    midiInputOptionIds = [];
    midiOutputOptionIds = [];

    if(WebMidi.inputs.length>0) {
      midi_in.options.length = 0;
      WebMidi.inputs.forEach(input => {
        var inputId = safe_midi_port_id(input);
        if (!inputId) return;
        midiInputOptionIds.push(inputId);
        midi_in.options[midi_in.options.length] = new Option("MIDI in: " + safe_midi_port_name(input));
      });
    }

    if(WebMidi.outputs.length>0) {
      midi_out.options.length = 0;
      WebMidi.outputs.forEach(output => {
        var outputId = safe_midi_port_id(output);
        if (!outputId) return;
        midiOutputOptionIds.push(outputId);
        midi_out.options[midi_out.options.length] = new Option("MIDI out: "+ safe_midi_port_name(output));
      });
    }
    // First run setup 
    setup_midi_devices();
  }

  if(WebMidi.supported) {
    WebMidi
      .enable({sysex:true})
      .then(onEnabled)
      .catch(err => console.log("MIDI: " + err));
  } else {
    document.getElementById('midi-input-col').style.display='none';
    document.getElementById('midi-output-col').style.display='none';
    document.getElementById('midi-warning-col').classList.remove('d-none');
  }
}

async function runCodeBlock(py) {
  // If audio hasn't yet started, the on-first-click audio starter is still running, so wait 1s so we don't glitch.
  if(!audio_started) await sleep_ms(1000);

  try {
    await mp.runPythonAsync(py);
  } catch (e) {
    console.log("Error in Python: " + e.message)
    // Print any error message to the REPL. Maybe there's a more direct way to raise JS errors to MPY
    await mp.runPythonAsync("print(\"\"\"" + e.message + "\"\"\")");
  }
}

async function runEditorBlock() {
  var py = editor.getValue();
  runCodeBlock(py);
}

async function shortio(url) {
    var data = {
        "domain":"share.tulip.computer",
        "originalURL": url
    }; 
    return fetch('https://api.short.io/links/public', {
        method: 'post',
        headers: {
            'accept': 'application/json',
            'Content-Type': 'application/json',
            'authorization': 'pk_C9ZPffijcRJnQMUq'
        },
        body: JSON.stringify(data)
    }).then(function(response){ 
        return response.json();
    }).then(function(data){
        return data.shortURL;
    })
}
async function shareCode() {
    // get the editor text, compress and base64 url safe it, and copy that + our base URL to the clipboard:
    var py = editor.getValue();
    const makeURLpromise = async() => {
        code = encodeURIComponent(await compress(py));
        var encoded_filename = encodeURIComponent(document.getElementById('editor_filename').value);
        var s =  window.location.protocol+"//"+window.location.host+"/run/?fn=" + encoded_filename + "&" + "share=" + code;
        var ss = await shortio(s);
        return await new Blob([ss], { type: 'text/plain'});
        //return await window.location.protocol+"//"+window.location.host+"/run/?fn=" + encoded_filename + "&" + "share=" + code;
    }

    navigator.clipboard.write([new ClipboardItem({ "text/plain": makeURLpromise() })])
      .then(function () {  })
      .catch(function (error) { console.log(error); });
    
    // Do a little button animation
    document.getElementById(`shareButton`).innerHTML = "Copied!"; 
    await sleep_ms(1000);
    document.getElementById(`shareButton`).innerHTML = "<i class='fas fa-share-square'></i>"; 
 }


// Comppreses string to GZIP. Retruns a Promise with Base64 string
const compress = string => {
    const blobToBase64 = blob => new Promise((resolve, _) => {
        const reader = new FileReader();
        reader.onloadend = () => resolve(reader.result.split(',')[1]);
        reader.readAsDataURL(blob);
    });
    const byteArray = new TextEncoder().encode(string);
    const cs = new CompressionStream('gzip');
    const writer = cs.writable.getWriter();
    writer.write(byteArray);
    writer.close();
    return new Response(cs.readable).blob().then(blobToBase64);
};

// Decompresses base64 encoded GZIP string. Retruns a string with original text.
const decompress = base64string => {
    const bytes = Uint8Array.from(atob(base64string), c => c.charCodeAt(0));
    const cs = new DecompressionStream('gzip');
    const writer = cs.writable.getWriter();
    writer.write(bytes);
    writer.close();
    return new Response(cs.readable).arrayBuffer().then(function (arrayBuffer) {
        return new TextDecoder().decode(arrayBuffer);
    });
}


async function sleep_ms(ms) {
    await new Promise((r) => setTimeout(r, ms));
}


async function show_alert(text) {
    const alertbox = document.querySelector("#alert_box");
    alertbox.innerHTML = text;
    alertbox.classList.remove('d-none'); //Remove hidden class
    alertbox.classList.add('alert-danger'); //Chance red color
    setTimeout(function() {alertbox.classList.add('d-none')}, 3000);

}

function ensure_current_environment_layout(seedDefaults) {
    try { mp.FS.mkdirTree(CURRENT_BASE_DIR); } catch (e) {}
    try { mp.FS.mkdirTree(CURRENT_ENV_DIR); } catch (e) {}
    try { mp.FS.mkdirTree(DEFAULT_BASE_DIR); } catch (e) {}
    try { mp.FS.mkdirTree(DEFAULT_ENV_DIR); } catch (e) {}
    if (seedDefaults) {
        try {
            mp.FS.readFile(DEFAULT_ENV_DIR + "/env.py", { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(DEFAULT_ENV_DIR + "/env.py", DEFAULT_ENV_SOURCE);
        }
        try {
            mp.FS.readFile(DEFAULT_WEB_PATCH_FILE, { encoding: "utf8" });
        } catch (e) {
            var defaultWebPatch = null;
            try {
                defaultWebPatch = mp.FS.readFile(DEFAULT_ENV_DIR + "/patches.txt", { encoding: "utf8" });
            } catch (legacyDefaultPatchErr) {}
            if (defaultWebPatch === null || defaultWebPatch === undefined) {
                defaultWebPatch = DEFAULT_WEB_PATCH_SOURCE;
            }
            mp.FS.writeFile(DEFAULT_WEB_PATCH_FILE, String(defaultWebPatch));
        }
        try {
            mp.FS.readFile(DEFAULT_OTHER_PATCH_FILE, { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(DEFAULT_OTHER_PATCH_FILE, DEFAULT_OTHER_PATCH_SOURCE);
        }
        try {
            mp.FS.readFile(CURRENT_ENV_DIR + "/env.py", { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(CURRENT_ENV_DIR + "/env.py", mp.FS.readFile(DEFAULT_ENV_DIR + "/env.py", { encoding: "utf8" }));
        }
        try {
            mp.FS.readFile(CURRENT_WEB_PATCH_FILE, { encoding: "utf8" });
        } catch (e) {
            var migrated = false;
            try {
                mp.FS.writeFile(CURRENT_WEB_PATCH_FILE, mp.FS.readFile("/amyboard/user/current/patches/patches.txt", { encoding: "utf8" }));
                migrated = true;
            } catch (legacyPatchesDirErr) {}
            try {
                if (!migrated) {
                    mp.FS.writeFile(CURRENT_WEB_PATCH_FILE, mp.FS.readFile("/amyboard/user/current/patch/patches.txt", { encoding: "utf8" }));
                    migrated = true;
                }
            } catch (legacyPatchErr) {}
            try {
                if (!migrated) {
                    mp.FS.writeFile(CURRENT_WEB_PATCH_FILE, mp.FS.readFile(CURRENT_ENV_DIR + "/patches.txt", { encoding: "utf8" }));
                    migrated = true;
                }
            } catch (legacyCurrentPatchFileErr) {}
            if (!migrated) {
                mp.FS.writeFile(CURRENT_WEB_PATCH_FILE, mp.FS.readFile(DEFAULT_WEB_PATCH_FILE, { encoding: "utf8" }));
            }
        }
        try {
            mp.FS.readFile(CURRENT_OTHER_PATCH_FILE, { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(CURRENT_OTHER_PATCH_FILE, mp.FS.readFile(DEFAULT_OTHER_PATCH_FILE, { encoding: "utf8" }));
        }
    }
}

async function sync_persistent_fs() {
    var fs = null;
    if (mp && mp.FS && typeof mp.FS.syncfs === "function") {
        fs = mp.FS;
    } else if (typeof FS !== "undefined" && FS && typeof FS.syncfs === "function") {
        fs = FS;
    }
    if (!fs) {
        return false;
    }
    return await new Promise(function(resolve) {
        fs.syncfs(false, function(err) {
            if (err) {
                console.warn("FS sync failed", err);
                resolve(false);
                return;
            }
            resolve(true);
        });
    });
}

function list_environment_files() {
    var files = [];
    function walk(dir, relPrefix) {
        for (const name of mp.FS.readdir(dir)) {
            if (name === "." || name === "..") continue;
            var path = dir + "/" + name;
            var relPath = relPrefix ? (relPrefix + "/" + name) : name;
            var mode = mp.FS.lookupPath(path).node.mode;
            if (mp.FS.isFile(mode)) {
                files.push(relPath);
            } else if (mp.FS.isDir(mode)) {
                walk(path, relPath);
            }
        }
    }
    walk(CURRENT_ENV_DIR, "");
    files.sort();
    return files;
}

function list_current_patch_files() {
    var files = list_environment_files().filter(function(filename) {
        return String(filename || "").toLowerCase().endsWith(".patch");
    });
    files.sort(function(a, b) {
        var al = String(a || "").toLowerCase();
        var bl = String(b || "").toLowerCase();
        if (al === "web.patch" && bl !== "web.patch") {
            return -1;
        }
        if (bl === "web.patch" && al !== "web.patch") {
            return 1;
        }
        return al < bl ? -1 : (al > bl ? 1 : 0);
    });
    return files;
}

function apply_active_channels_from_patch_map(channelPatchMap) {
    var anyActive = false;
    for (var channel = 1; channel <= 16; channel++) {
        if (Number.isInteger(channelPatchMap[channel])) {
            anyActive = true;
            break;
        }
    }
    if (!anyActive) {
        return false;
    }
    for (var ch = 1; ch <= 16; ch++) {
        var active = Number.isInteger(channelPatchMap[ch]);
        if (typeof window.set_channel_active === "function") {
            window.set_channel_active(ch, active);
        } else if (Array.isArray(window.active_channels)) {
            window.active_channels[ch] = active;
        }
    }
    var current = Number(window.current_synth || 1);
    if (!Number.isInteger(current) || current < 1 || current > 16 || !Number.isInteger(channelPatchMap[current])) {
        for (var pick = 1; pick <= 16; pick++) {
            if (Number.isInteger(channelPatchMap[pick])) {
                current = pick;
                break;
            }
        }
        window.current_synth = current;
    }
    var channelSelect = document.getElementById("midi-channel-select");
    if (channelSelect) {
        channelSelect.value = String(window.current_synth || 1);
    }
    var activeCheckbox = document.getElementById("channel-active-checkbox");
    if (activeCheckbox) {
        activeCheckbox.checked = !!(Array.isArray(window.active_channels) && window.active_channels[window.current_synth || 1]);
    }
    return true;
}

async function execute_current_patch_files() {
    var webPatchMissingBeforeLayout = false;
    try {
        mp.FS.readFile(CURRENT_WEB_PATCH_FILE, { encoding: "utf8" });
    } catch (missingWebPatchErr) {
        webPatchMissingBeforeLayout = true;
    }
    ensure_current_environment_layout(true);
    var patchFiles = list_current_patch_files();
    var channelPatchMap = new Array(17).fill(null);
    var combinedPatchSources = [];
    var webPatchHasMessages = false;
    for (var i = 0; i < patchFiles.length; i++) {
        var filename = patchFiles[i];
        var filenameLower = String(filename || "").toLowerCase();
        var source = "";
        try {
            source = mp.FS.readFile(CURRENT_ENV_DIR + "/" + filename, { encoding: "utf8" });
        } catch (readErr) {
            continue;
        }
        combinedPatchSources.push(String(source || ""));
        var lines = String(source || "").split("\n");
        for (var j = 0; j < lines.length; j++) {
            var message = String(lines[j] || "").trim();
            if (!message || message.startsWith("#")) {
                continue;
            }
            if (filenameLower === "web.patch") {
                webPatchHasMessages = true;
            }
            amy_add_log_message(message);
            var bindMatch = message.match(/^i(\d+)iv6K(\d+)/);
            if (bindMatch) {
                var bindChannel = Number(bindMatch[1]);
                var bindPatch = Number(bindMatch[2]);
                if (Number.isInteger(bindChannel) && bindChannel >= 1 && bindChannel <= 16 && Number.isInteger(bindPatch)) {
                    channelPatchMap[bindChannel] = bindPatch;
                }
                continue;
            }
            var patchMatch = message.match(/^K(\d+)/);
            if (patchMatch) {
                var patchNum = Number(patchMatch[1]);
                if (Number.isInteger(patchNum) && patchNum >= 1024 && patchNum <= 1055) {
                    var inferredChannel = patchNum - 1023;
                    if (!Number.isInteger(channelPatchMap[inferredChannel])) {
                        channelPatchMap[inferredChannel] = patchNum;
                    }
                }
            }
        }
    }

    var combinedPatchSource = combinedPatchSources.join("\n");
    var hasActiveChannels = apply_active_channels_from_patch_map(channelPatchMap);
    if (hasActiveChannels
        && typeof window.set_knobs_from_patch === "function"
        && combinedPatchSource) {
        for (var channel = 1; channel <= 16; channel++) {
            if (!Number.isInteger(channelPatchMap[channel])) {
                continue;
            }
            window.set_knobs_from_patch(channel, channelPatchMap[channel], combinedPatchSource);
        }
    }
    return {
        files: patchFiles,
        channelPatchMap: channelPatchMap,
        combinedPatchSource: combinedPatchSource,
        hasActiveChannels: hasActiveChannels,
        webPatchHasMessages: webPatchHasMessages,
        webPatchMissingBeforeLayout: webPatchMissingBeforeLayout,
    };
}

function should_initialize_default_patch_state(patchExecution) {
    if (!patchExecution || typeof patchExecution !== "object") {
        return false;
    }
    if (patchExecution.webPatchMissingBeforeLayout) {
        return true;
    }
    return (!patchExecution.webPatchHasMessages) && (!patchExecution.hasActiveChannels);
}

function is_editor_openable_file(filename) {
    if (!filename || typeof filename !== "string") {
        return false;
    }
    var lower = filename.toLowerCase();
    for (var i = 0; i < EDITOR_ALLOWED_EXTENSIONS.length; i++) {
        if (lower.endsWith(EDITOR_ALLOWED_EXTENSIONS[i])) {
            return true;
        }
    }
    return false;
}

function render_environment_file_list() {
    var container = document.getElementById("treecontainer");
    if (!container) return;
    var files = list_environment_files();
    if (!files.length) {
        selected_environment_file = null;
        container.innerHTML = '<div class="env-file-empty">No files in current environment.</div>';
        return;
    }
    if (!selected_environment_file || files.indexOf(selected_environment_file) === -1) {
        selected_environment_file = files[0];
    }
    var html = '<div class="env-file-list">';
    for (var i = 0; i < files.length; i++) {
        var filename = files[i];
        var active = filename === selected_environment_file;
        var encoded = encodeURIComponent(filename);
        html += '<button type="button" class="env-file-item' +
            (active ? " selected" : "") + '" onclick="select_environment_file(decodeURIComponent(\'' + encoded + '\'), true)">' +
            '<i class="fas fa-file"></i> ' + escape_html(filename) + "</button>";
    }
    html += "</div>";
    container.innerHTML = html;
}

async function select_environment_file(filename, loadEditor) {
    if (!filename) return;
    if (loadEditor && selected_environment_file && filename !== selected_environment_file) {
        await save_editor_if_dirty();
    }
    selected_environment_file = filename;
    render_environment_file_list();
    if (loadEditor && is_editor_openable_file(filename)) {
        await load_editor();
    }
}

async function download() {
    if (!selected_environment_file) {
        show_alert("Select an environment file first.");
        return;
    }
    var fullPath = CURRENT_ENV_DIR + "/" + selected_environment_file;
    var blob = new Blob([mp.FS.readFile(fullPath, { encoding: "binary" })], { type: "application/octet-stream" });
    var link = document.createElement("a");
    link.href = window.URL.createObjectURL(blob);
    link.download = selected_environment_file;
    link.click();
}

async function upload() {
    ensure_current_environment_layout(false);
    var input = document.createElement("input");
    input.type = "file";
    input.onchange = e => {
        var file = e.target.files[0];
        if (!file) return;
        var reader = new FileReader();
        reader.readAsArrayBuffer(file);
        reader.onload = async readerEvent => {
            var content = readerEvent.target.result;
            mp.FS.writeFile(CURRENT_ENV_DIR + "/" + file.name, new Uint8Array(content), { encoding: "binary" });
            selected_environment_file = file.name;
            await sync_persistent_fs();
            await fill_tree();
        };
    };
    input.click();
}

function next_untitled_environment_filename() {
    var files = list_environment_files();
    if (files.indexOf("untitled.py") === -1) {
        return "untitled.py";
    }
    var index = 2;
    while (files.indexOf("untitled" + index + ".py") !== -1) {
        index += 1;
    }
    return "untitled" + index + ".py";
}

async function create_new_environment_file() {
    ensure_current_environment_layout(false);
    var filename = next_untitled_environment_filename();
    try {
        mp.FS.writeFile(CURRENT_ENV_DIR + "/" + filename, "");
        await sync_persistent_fs();
        selected_environment_file = filename;
        await fill_tree();
        await load_editor();
    } catch (e) {
        show_alert("Could not create new file.");
    }
}

function environment_file_exists(filename) {
    if (!filename) return false;
    try {
        var mode = mp.FS.lookupPath(CURRENT_ENV_DIR + "/" + filename).node.mode;
        return mp.FS.isFile(mode);
    } catch (e) {
        return false;
    }
}

function open_rename_environment_file_modal() {
    if (!selected_environment_file) {
        show_alert("Select an environment file first.");
        return;
    }
    var input = document.getElementById("rename_environment_filename");
    if (!input) return;
    input.value = selected_environment_file;
    input.focus();
    input.select();
    var modalEl = document.getElementById("renameEnvironmentFileModal");
    if (!modalEl || !window.bootstrap) return;
    var modal = window.bootstrap.Modal.getOrCreateInstance(modalEl);
    modal.show();
}

async function confirm_environment_action(title, message, confirmLabel) {
    var modalEl = document.getElementById("confirmEnvironmentActionModal");
    var titleEl = document.getElementById("confirm_environment_action_title");
    var bodyEl = document.getElementById("confirm_environment_action_body");
    var confirmBtn = document.getElementById("confirm_environment_action_btn");
    if (!modalEl || !titleEl || !bodyEl || !confirmBtn || !window.bootstrap) {
        return window.confirm(message || title || "Are you sure?");
    }

    titleEl.textContent = title || "Confirm";
    bodyEl.textContent = message || "";
    confirmBtn.textContent = confirmLabel || "Confirm";

    return await new Promise(function(resolve) {
        var modal = window.bootstrap.Modal.getOrCreateInstance(modalEl);
        var accepted = false;

        function onConfirm() {
            accepted = true;
            modal.hide();
        }

        function onHidden() {
            confirmBtn.removeEventListener("click", onConfirm);
            resolve(accepted);
        }

        confirmBtn.addEventListener("click", onConfirm);
        modalEl.addEventListener("hidden.bs.modal", onHidden, { once: true });
        modal.show();
    });
}

async function submit_rename_environment_file() {
    if (!selected_environment_file) {
        show_alert("Select an environment file first.");
        return;
    }
    var input = document.getElementById("rename_environment_filename");
    if (!input) return;
    var newFilename = input.value.trim();
    if (!newFilename) {
        show_alert("Enter a file name.");
        return;
    }
    if (newFilename === selected_environment_file) {
        var unchangedModal = document.getElementById("renameEnvironmentFileModal");
        if (unchangedModal && window.bootstrap) {
            window.bootstrap.Modal.getOrCreateInstance(unchangedModal).hide();
        }
        return;
    }
    if (newFilename.startsWith("/") || newFilename.includes("..")) {
        show_alert("Invalid file name.");
        return;
    }
    if (environment_file_exists(newFilename)) {
        show_alert("A file with that name already exists.");
        return;
    }

    var oldPath = CURRENT_ENV_DIR + "/" + selected_environment_file;
    var newPath = CURRENT_ENV_DIR + "/" + newFilename;
    var slash = newFilename.lastIndexOf("/");
    if (slash > 0) {
        try { mp.FS.mkdirTree(CURRENT_ENV_DIR + "/" + newFilename.substring(0, slash)); } catch (e) {}
    }

    try {
        mp.FS.rename(oldPath, newPath);
        await sync_persistent_fs();
        selected_environment_file = newFilename;
        await fill_tree();
        await load_editor();
        var modalEl = document.getElementById("renameEnvironmentFileModal");
        if (modalEl && window.bootstrap) {
            window.bootstrap.Modal.getOrCreateInstance(modalEl).hide();
        }
    } catch (e) {
        show_alert("Could not rename that file.");
    }
}

async function delete_selected_environment_file() {
    if (!selected_environment_file) {
        show_alert("Select an environment file first.");
        return;
    }
    var confirmed = await confirm_environment_action(
        "Delete file",
        "Delete " + selected_environment_file + "?",
        "Delete"
    );
    if (!confirmed) {
        return;
    }
    try {
        mp.FS.unlink(CURRENT_ENV_DIR + "/" + selected_environment_file);
        await sync_persistent_fs();
        selected_environment_file = null;
        await fill_tree();
        var files = list_environment_files();
        if (files.length) {
            await select_environment_file(files[0], true);
        } else if (editor) {
            editor.setValue("");
            setTimeout(function () { editor.save(); }, 100);
            setTimeout(function () { editor.refresh(); }, 250);
        }
    } catch (e) {
        show_alert("Could not delete that file.");
    }
}

async function save_editor() {
    ensure_current_environment_layout(false);
    var targetFilename = selected_environment_file || "env.py";
    var slash = targetFilename.lastIndexOf("/");
    if (slash > 0) {
        try { mp.FS.mkdirTree(CURRENT_ENV_DIR + "/" + targetFilename.substring(0, slash)); } catch (e) {}
    }
    mp.FS.writeFile(CURRENT_ENV_DIR + "/" + targetFilename, editor.getValue());
    await sync_persistent_fs();
    selected_environment_file = targetFilename;
    environment_editor_dirty = false;
    await fill_tree();
}

async function save_editor_if_dirty() {
    if (!editor || !environment_editor_dirty || environment_autosave_inflight) {
        return false;
    }
    environment_autosave_inflight = true;
    try {
        await save_editor();
        return true;
    } finally {
        environment_autosave_inflight = false;
    }
}

async function load_editor() {
    if (!editor) {
        pending_environment_editor_load = true;
        return;
    }
    if (!selected_environment_file) {
        show_alert("Select an environment file first.");
        return;
    }
    if (!is_editor_openable_file(selected_environment_file)) {
        return;
    }
    var fullPath = CURRENT_ENV_DIR + "/" + selected_environment_file;
    try {
        editor.setValue(mp.FS.readFile(fullPath, { encoding: "utf8" }));
        environment_editor_dirty = false;
        setTimeout(function () { editor.save(); }, 100);
        setTimeout(function () { editor.refresh(); }, 250);
        pending_environment_editor_load = false;
    } catch (e) {
        show_alert("Could not load that file into the editor.");
    }
}

async function flush_pending_environment_editor_load() {
    if (pending_environment_editor_load && editor) {
        await load_editor();
    }
}

async function load_first_environment_file_into_editor() {
    ensure_current_environment_layout(false);
    var files = list_environment_files();
    if (!files.length) {
        return;
    }
    var target = null;
    if (files.indexOf("env.py") !== -1) {
        target = "env.py";
    }
    if (!target) {
        for (var i = 0; i < files.length; i++) {
            if (files[i].toLowerCase().endsWith(".py")) {
                target = files[i];
                break;
            }
        }
    }
    if (!target) {
        target = files[0];
    }
    await select_environment_file(target, true);
}

async function fill_tree() {
    ensure_current_environment_layout(false);
    render_environment_file_list();
}

function normalize_world_filename(filename) {
    if (!filename || typeof filename !== "string") {
        return "imported.py";
    }
    return filename.split("/").pop().split("\\").pop();
}

function is_world_tar_filename(filename) {
    return normalize_world_filename(filename).toLowerCase().endsWith(".tar");
}

function get_world_package_name(filename) {
    var normalized = normalize_world_filename(filename);
    if (is_world_tar_filename(normalized)) {
        return normalized.substring(0, normalized.length - 4);
    }
    return normalized;
}

function get_world_display_name(filename) {
    var normalized = normalize_world_filename(filename);
    return get_world_package_name(normalized);
}

function format_world_file_timestamp(time_ms) {
    if (!Number.isFinite(time_ms)) {
        return "";
    }
    try {
        return new Date(time_ms).toLocaleString();
    } catch (e) {
        return "";
    }
}

function world_api_url(pathAndQuery) {
    return AMYBOARD_WORLD_API_BASE + pathAndQuery;
}

function get_world_search_query() {
    var el = document.getElementById("amyboard_world_search");
    if (!el) return "";
    return String(el.value || "").trim();
}

function get_world_tag_query() {
    return String(amyboard_world_selected_tag || "").trim().toLowerCase();
}

function randomize_world_tag_palette() {
    var tags = ["featured", "official", "popular", "wild"];
    var classes = [
        "bg-primary",
        "bg-success",
        "bg-danger",
        "bg-info text-dark",
        "bg-warning text-dark",
        "bg-secondary",
    ];
    var shuffled = classes.slice();
    for (var i = shuffled.length - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var tmp = shuffled[i];
        shuffled[i] = shuffled[j];
        shuffled[j] = tmp;
    }
    amyboard_world_tag_palette = {};
    for (var t = 0; t < tags.length; t++) {
        amyboard_world_tag_palette[tags[t]] = shuffled[t % shuffled.length];
    }
}

function render_world_tag_pills() {
    var container = document.getElementById("amyboard_world_tag_pills");
    if (!container) return;
    if (!Object.keys(amyboard_world_tag_palette).length) {
        randomize_world_tag_palette();
    }
    var tags = ["featured", "official", "popular", "wild"];
    var html = "";
    for (var i = 0; i < tags.length; i++) {
        var tag = tags[i];
        var active = amyboard_world_selected_tag === tag;
        var colorClass = amyboard_world_tag_palette[tag] || "bg-secondary";
        html += '<button type="button" class="badge border-0 ' + colorClass + (active ? '' : ' opacity-50') + '"';
        html += ' onclick="select_amyboard_world_tag(\'' + tag + '\')">#' + escape_html(tag) + '</button>';
    }
    container.innerHTML = html;
}

function select_amyboard_world_tag(tag) {
    var normalized = String(tag || "").trim().toLowerCase();
    if (amyboard_world_selected_tag === normalized) {
        amyboard_world_selected_tag = "";
    } else {
        amyboard_world_selected_tag = normalized;
    }
    render_world_tag_pills();
    refresh_amyboard_world_files();
}

function resolve_world_download_url(item) {
    if (item && typeof item.download_url === "string" && item.download_url.length > 0) {
        if (/^https?:\/\//i.test(item.download_url)) {
            return item.download_url;
        }
        return world_api_url(item.download_url);
    }
    return "";
}

function format_size_kb(sizeBytes) {
    var bytes = Number(sizeBytes || 0);
    if (!Number.isFinite(bytes) || bytes < 0) {
        bytes = 0;
    }
    return (bytes / 1024).toFixed(1) + "KB";
}

function escape_html(text) {
    return String(text)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

function tar_header_string(buffer, offset, length) {
    var out = "";
    for (var i = 0; i < length; i++) {
        var byte = buffer[offset + i];
        if (!byte) break;
        out += String.fromCharCode(byte);
    }
    return out;
}

function tar_header_octal(buffer, offset, length) {
    var raw = tar_header_string(buffer, offset, length).trim();
    if (!raw) return 0;
    var parsed = parseInt(raw, 8);
    return Number.isFinite(parsed) ? parsed : 0;
}

function tar_is_zero_block(buffer, offset) {
    for (var i = 0; i < 512; i++) {
        if (buffer[offset + i] !== 0) return false;
    }
    return true;
}

function sanitize_tar_entry_path(path) {
    if (!path) return "";
    var p = path.replaceAll("\\", "/");
    while (p.startsWith("/")) p = p.substring(1);
    if (!p.length) return "";
    var parts = p.split("/");
    var safe = [];
    for (var i = 0; i < parts.length; i++) {
        var part = parts[i];
        if (!part || part === "." || part === "..") {
            continue;
        }
        safe.push(part);
    }
    return safe.join("/");
}

function extract_tar_buffer_to_fs(tarBytes, destinationRoot, stripPrefix) {
    var offset = 0;
    var normalizedStripPrefix = stripPrefix ? stripPrefix.replaceAll("\\", "/") : "";
    while (offset + 512 <= tarBytes.length) {
        if (tar_is_zero_block(tarBytes, offset)) break;

        var name = tar_header_string(tarBytes, offset + 0, 100);
        var size = tar_header_octal(tarBytes, offset + 124, 12);
        var typeflag = tar_header_string(tarBytes, offset + 156, 1) || "0";
        var prefix = tar_header_string(tarBytes, offset + 345, 155);
        var entryPath = sanitize_tar_entry_path((prefix ? prefix + "/" : "") + name);
        if (normalizedStripPrefix && entryPath.startsWith(normalizedStripPrefix)) {
            entryPath = sanitize_tar_entry_path(entryPath.substring(normalizedStripPrefix.length));
        }
        var dataOffset = offset + 512;

        if (entryPath) {
            var fullPath = destinationRoot + "/" + entryPath;
            if (typeflag === "5") {
                try { mp.FS.mkdirTree(fullPath); } catch (e) {}
            } else if (typeflag === "0" || typeflag === "\0") {
                var slash = fullPath.lastIndexOf("/");
                if (slash > 0) {
                    try { mp.FS.mkdirTree(fullPath.substring(0, slash)); } catch (e) {}
                }
                mp.FS.writeFile(fullPath, tarBytes.slice(dataOffset, dataOffset + size), { encoding: "binary" });
            }
        }

        var blocks = Math.ceil(size / 512);
        offset = dataOffset + (blocks * 512);
    }
}

function remove_fs_path(path) {
    try {
        var mode = mp.FS.lookupPath(path).node.mode;
        if (mp.FS.isDir(mode)) {
            for (const name of mp.FS.readdir(path)) {
                if (name === "." || name === "..") continue;
                remove_fs_path(path + "/" + name);
            }
            mp.FS.rmdir(path);
        } else {
            mp.FS.unlink(path);
        }
    } catch (e) {}
}

function copy_fs_path(sourcePath, destinationPath) {
    try {
        var mode = mp.FS.lookupPath(sourcePath).node.mode;
        if (mp.FS.isDir(mode)) {
            try { mp.FS.mkdirTree(destinationPath); } catch (e) {}
            for (const name of mp.FS.readdir(sourcePath)) {
                if (name === "." || name === "..") continue;
                copy_fs_path(sourcePath + "/" + name, destinationPath + "/" + name);
            }
        } else {
            var slash = destinationPath.lastIndexOf("/");
            if (slash > 0) {
                try { mp.FS.mkdirTree(destinationPath.substring(0, slash)); } catch (e) {}
            }
            mp.FS.writeFile(destinationPath, mp.FS.readFile(sourcePath, { encoding: "binary" }), { encoding: "binary" });
        }
    } catch (e) {}
}

function clear_current_environment_dir() {
    ensure_current_environment_layout(false);
    for (const name of mp.FS.readdir(CURRENT_ENV_DIR)) {
        if (name === "." || name === "..") continue;
        remove_fs_path(CURRENT_ENV_DIR + "/" + name);
    }
}

async function clear_current_environment_from_default() {
    ensure_current_environment_layout(true);
    var confirmed = await confirm_environment_action(
        "Clear code",
        "Clear current code and restore from default?",
        "Clear"
    );
    if (!confirmed) {
        return;
    }
    try {
        remove_fs_path(CURRENT_ENV_DIR);
        mp.FS.mkdirTree(CURRENT_ENV_DIR);
        copy_fs_path(DEFAULT_ENV_DIR, CURRENT_ENV_DIR);
        selected_environment_file = null;
        await sync_persistent_fs();
        await fill_tree();
        if (list_environment_files().indexOf("env.py") !== -1) {
            await select_environment_file("env.py", true);
        } else {
            var files = list_environment_files();
            if (files.length) {
                await select_environment_file(files[0], true);
            } else if (editor) {
                editor.setValue("");
            }
        }
    } catch (e) {
        show_alert("Could not clear environment.");
    }
}

async function run_current_environment() {
    await save_editor_if_dirty();
    await runCodeBlock(
        "from upysh import *\n" +
        "import sys\n" +
        "_amyboard_user_dir = '/amyboard/user'\n" +
        "_amyboard_env_dir = '/amyboard/user/current/env'\n" +
        "cd(_amyboard_env_dir)\n" +
        "try:\n" +
        "    execfile('env.py')\n" +
        "except Exception as e:\n" +
        "    print('Environment start failed:')\n" +
        "    sys.print_exception(e)\n" +
        "finally:\n" +
        "    cd(_amyboard_user_dir)\n"
    );
}

function add_octal_to_buffer(buffer, offset, length, value, digits, trailer) {
    var width = Math.max(1, Number(digits) || (length - 1));
    var oct = Math.max(0, Number(value) || 0).toString(8);
    var padded = oct.padStart(width, "0");
    if (padded.length > width) {
        padded = padded.slice(-width);
    }
    var text = padded + (typeof trailer === "string" ? trailer : " ");
    for (var i = 0; i < length; i++) {
        buffer[offset + i] = 0;
    }
    var n = Math.min(length, text.length);
    for (var j = 0; j < n; j++) {
        buffer[offset + j] = text.charCodeAt(j);
    }
}

function write_string_to_buffer(buffer, offset, length, text) {
    for (var i = 0; i < length; i++) {
        buffer[offset + i] = 0;
    }
    var n = Math.min(length, text.length);
    for (var j = 0; j < n; j++) {
        buffer[offset + j] = text.charCodeAt(j);
    }
}

function tar_build_header(name, size, isDirectory) {
    var header = new Uint8Array(512);
    write_string_to_buffer(header, 0, 100, name);
    // Match tulip/shared/py/utarfile.py addfile() field formatting.
    add_octal_to_buffer(header, 100, 7, isDirectory ? 0o755 : 0o644, 6, " ");
    add_octal_to_buffer(header, 108, 7, 0, 6, " ");
    add_octal_to_buffer(header, 116, 7, 0, 6, " ");
    add_octal_to_buffer(header, 124, 12, isDirectory ? 0 : size, 11, " ");
    add_octal_to_buffer(header, 136, 12, Math.floor(Date.now() / 1000), 11, " ");
    for (var i = 148; i < 156; i++) {
        header[i] = 32;
    }
    header[156] = isDirectory ? "5".charCodeAt(0) : "0".charCodeAt(0);
    write_string_to_buffer(header, 257, 6, "ustar");
    write_string_to_buffer(header, 263, 2, "00");
    var sum = 0;
    for (var k = 0; k < 512; k++) {
        sum += header[k];
    }
    add_octal_to_buffer(header, 148, 7, sum, 6, "\0");
    return header;
}

function build_environment_tar_bytes(environmentName) {
    var chunks = [];
    var totalSize = 0;
    function pushChunk(bytes) {
        chunks.push(bytes);
        totalSize += bytes.length;
    }
    function add_entry(relativePath) {
        var fullPath = CURRENT_ENV_DIR + "/" + relativePath;
        var mode = mp.FS.lookupPath(fullPath).node.mode;
        if (mp.FS.isDir(mode)) {
            var dirPath = relativePath.endsWith("/") ? relativePath : (relativePath + "/");
            pushChunk(tar_build_header(dirPath, 0, true));
            for (const child of mp.FS.readdir(fullPath)) {
                if (child === "." || child === "..") continue;
                add_entry(relativePath + "/" + child);
            }
            return;
        }
        var bytes = mp.FS.readFile(fullPath, { encoding: "binary" });
        pushChunk(tar_build_header(relativePath, bytes.length, false));
        pushChunk(bytes);
        var pad = (512 - (bytes.length % 512)) % 512;
        if (pad > 0) {
            pushChunk(new Uint8Array(pad));
        }
    }

    var entries = mp.FS.readdir(CURRENT_ENV_DIR).filter(function(name) {
        return name !== "." && name !== "..";
    }).sort();
    for (var i = 0; i < entries.length; i++) {
        add_entry(entries[i]);
    }
    pushChunk(new Uint8Array(1024));
    var out = new Uint8Array(totalSize);
    var cursor = 0;
    for (var c = 0; c < chunks.length; c++) {
        out.set(chunks[c], cursor);
        cursor += chunks[c].length;
    }
    return out;
}

function render_amyboard_world_file_list() {
    var list = document.getElementById("amyboard_world_file_list");
    if (!list) return;

    if (!amyboard_world_files.length) {
        list.innerHTML = '<div class="border rounded p-2 text-muted">No remote files found.</div>';
        return;
    }

    var html = '<div class="row row-cols-2 g-2">';
    for (var i = 0; i < amyboard_world_files.length; i++) {
        var item = amyboard_world_files[i];
        var filename = normalize_world_filename(item.filename);
        var displayName = get_world_display_name(filename);
        var description = (typeof item.content === "string") ? item.content.trim() : "";
        if (!description && typeof item.description === "string") {
            description = item.description.trim();
        }
        var when = format_world_file_timestamp(item.time);
        var meta = (item.username || "unknown") + (when ? " • " + when : "");
        var tags = Array.isArray(item.tags) ? item.tags : [];
        var isLoading = (amyboard_world_loading_index === i);
        html += '<div class="col">';
        html += '<button type="button" class="w-100 border rounded p-2 bg-white text-start d-flex justify-content-between align-items-start"' +
            ' onclick="import_amyboard_world_file(' + i + ')"' + (isLoading ? " disabled" : "") + ">";
        html += '<div class="me-3 text-start"><div class="fw-bold">' + escape_html(displayName);
        if (description.length > 0) {
            html += ' <span class="fw-normal text-muted">- ' + escape_html(description) + "</span>";
        }
        html += '</div><div class="small text-muted">' + escape_html(meta) + '</div>';
        if (tags.length > 0) {
            html += '<div class="mt-1">';
            for (var t = 0; t < tags.length; t++) {
                html += '<span class="badge bg-light text-secondary border me-1">#' + escape_html(String(tags[t])) + '</span>';
            }
            html += '</div>';
        }
        html += '</div>';
        if (isLoading) {
            html += '<span class="d-inline-flex align-items-center text-secondary small"><span class="spinner-border spinner-border-sm me-2" role="status" aria-hidden="true"></span>Downloading...</span>';
        } else {
            html += '<span class="badge bg-secondary rounded-pill">' + format_size_kb(item.size) + "</span>";
        }
        html += "</button>";
        html += "</div>";
    }
    html += "</div>";
    list.innerHTML = html;
}

async function refresh_amyboard_world_files() {
    render_world_tag_pills();
    var list = document.getElementById("amyboard_world_file_list");
    if (list) {
        list.innerHTML = '<div class="border rounded p-2 text-muted">Loading...</div>';
    }

    try {
        var params = new URLSearchParams();
        params.set("limit", "100");
        params.set("latest_per_user_env", "true");
        var q = get_world_search_query();
        var tag = get_world_tag_query();
        if (q.length) {
            params.set("q", q);
        }
        if (tag.length) {
            params.set("tag", tag);
        }
        var newApiResponse = await fetch(world_api_url("/api/amyboardworld/files?" + params.toString()));
        if (!newApiResponse.ok) {
            throw new Error("HTTP " + newApiResponse.status.toString());
        }
        var newApiData = await newApiResponse.json();
        if (!newApiData || !Array.isArray(newApiData.items)) {
            throw new Error("Unexpected response");
        }
        amyboard_world_files = newApiData.items
            .filter(function(item) {
                return item && is_world_tar_filename(item.filename);
            })
            .sort(function(a, b) {
                return Number(b.time || 0) - Number(a.time || 0);
            })
            .slice(0, 10);
        render_amyboard_world_file_list();
    } catch (e) {
        amyboard_world_files = [];
        if (list) {
            list.innerHTML = '<div class="border rounded p-2 text-danger">Could not load remote files.</div>';
        }
    }
}

async function import_amyboard_world_file(index) {
    if (amyboard_world_loading_index !== null) {
        return;
    }
    if (index < 0 || index >= amyboard_world_files.length) {
        show_alert("That remote file is no longer available.");
        return;
    }

    var item = amyboard_world_files[index];
    var filename = normalize_world_filename(item.filename);
    var packageName = get_world_package_name(filename);
    amyboard_world_loading_index = index;
    render_amyboard_world_file_list();

    try {
        ensure_current_environment_layout(false);
        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) {
            throw new Error("Missing file URL");
        }
        var response = await fetch(downloadUrl);
        if (!response.ok) {
            throw new Error("HTTP " + response.status.toString());
        }

        var buffer = await response.arrayBuffer();
        var bytes = new Uint8Array(buffer);
        clear_current_environment_dir();
        extract_tar_buffer_to_fs(bytes, CURRENT_ENV_DIR, packageName + "/");
        if (!list_environment_files().length) {
            // Support tarballs where files are already at tar root.
            extract_tar_buffer_to_fs(bytes, CURRENT_ENV_DIR, "");
        }
        await fill_tree();
        var envNameInput = document.getElementById("editor_filename");
        if (envNameInput) {
            envNameInput.value = packageName;
        }
        if (list_environment_files().indexOf("env.py") !== -1) {
            await select_environment_file("env.py", true);
        } else {
            var files = list_environment_files();
            if (files.length) {
                await select_environment_file(files[0], true);
            }
        }
        var importPatchExecution = await execute_current_patch_files();
        if (should_initialize_default_patch_state(importPatchExecution)) {
            await apply_default_patch_state_from_clear_patches();
        }
        await run_current_environment();

        await refresh_amyboard_world_files();
    } catch (e) {
        show_alert("Failed to import remote environment.");
    } finally {
        amyboard_world_loading_index = null;
        render_amyboard_world_file_list();
    }
}



// Create a js File object and upload it to the AMYboard World API.
async function amyboard_world_upload_file(pwd, filename, username, description) {
    var contents = await mp.FS.readFile(pwd+filename, {encoding:'binary'});
    var file = await new File([new Uint8Array(contents)], filename, {type: 'application/binary'})
    var data = new FormData();
    data.append('file',file);
    data.append('username', username);
    data.append('description', description);
    return fetch(world_api_url("/api/amyboardworld/upload"), {
        method: 'POST',
        body: data,
    });
}

function sanitize_environment_description(raw) {
    var input = String(raw || "");
    // Strip any HTML tags/content interpretation by converting through a detached element.
    var plain = input;
    try {
        var node = document.createElement("div");
        node.innerHTML = input;
        plain = node.textContent || node.innerText || "";
    } catch (e) {
        plain = input.replace(/<[^>]*>/g, "");
    }
    return plain.replace(/\s+/g, " ").trim();
}

async function upload_current_environment() {
    ensure_current_environment_layout(false);
    var envNameInput = document.getElementById("editor_filename");
    var usernameInput = document.getElementById("environment_username");
    var descriptionInput = document.getElementById("environment_description");
    if (!envNameInput || !usernameInput || !descriptionInput) return;

    var username = (usernameInput.value || "").trim();
    var environmentName = (envNameInput.value || "").trim();
    var description = sanitize_environment_description(descriptionInput.value || "");
    descriptionInput.value = description;

    if (!/^[A-Za-z0-9]{1,20}$/.test(username)) {
        show_alert("Username must be 1-20 chars: only A-Z, a-z, 0-9 (no spaces).");
        return;
    }

    if (!/^[A-Za-z0-9_-]{1,20}$/.test(environmentName)) {
        show_alert("Environment Name must be 1-20 chars: only A-Z, a-z, 0-9, - and _.");
        return;
    }

    if (description.length < 1 || description.length > 400) {
        show_alert("Description must be 1-400 characters.");
        return;
    }

    var uploadButton = document.getElementById("upload_current_environment_btn");
    if (uploadButton && uploadButton.disabled) {
        return;
    }
    if (uploadButton) {
        uploadButton.disabled = true;
        uploadButton.innerHTML = '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span>Uploading...';
    }

    var tarBytes = build_environment_tar_bytes(environmentName);
    var tarFilename = environmentName + ".tar";
    var file = new File([tarBytes], tarFilename, { type: "application/x-tar" });
    var data = new FormData();
    data.append("file", file);
    data.append("username", username);
    data.append("description", description);
    try {
        var response = await fetch(world_api_url("/api/amyboardworld/upload"), {
            method: "POST",
            body: data,
        });
        if (!response.ok) {
            throw new Error("HTTP " + response.status.toString());
        }
        await refresh_amyboard_world_files();
    } catch (e) {
        show_alert("Upload failed.");
    } finally {
        if (uploadButton) {
            uploadButton.disabled = false;
            uploadButton.textContent = "Upload";
        }
    }
}

function open_upload_environment_modal() {
    var modalEl = document.getElementById("uploadEnvironmentModal");
    if (!modalEl || !window.bootstrap) {
        return;
    }
    var modal = window.bootstrap.Modal.getOrCreateInstance(modalEl);
    modal.show();
    var usernameInput = document.getElementById("environment_username");
    if (usernameInput) {
        setTimeout(function() {
            usernameInput.focus();
            usernameInput.select();
        }, 150);
    }
}

function populate_send_to_amyboard_modal() {
    var unsupported = document.getElementById("send-amyboard-unsupported");
    var supported = document.getElementById("send-amyboard-supported");
    var sendButton = document.getElementById("send-amyboard-send-btn");
    if (!unsupported || !supported) return;
    if (!WebMidi.supported) {
        unsupported.classList.remove("d-none");
        supported.classList.add("d-none");
        if (sendButton) {
            sendButton.classList.add("d-none");
        }
        return;
    }
    unsupported.classList.add("d-none");
    supported.classList.remove("d-none");
    if (sendButton) {
        sendButton.classList.remove("d-none");
    }
}

function sanitize_environment_name_for_tar(rawName) {
    var name = String(rawName || "").trim();
    if (!name.length) {
        return "environment";
    }
    name = name.replace(/[^A-Za-z0-9_-]/g, "_");
    if (!name.length) {
        return "environment";
    }
    return name.slice(0, 20);
}

function send_amyboard_progress_reset() {
    var wrapper = document.getElementById("send-amyboard-progress-wrap");
    var bar = document.getElementById("send-amyboard-progress-bar");
    var text = document.getElementById("send-amyboard-progress-text");
    var status = document.getElementById("send-amyboard-status");
    if (wrapper) {
        wrapper.classList.add("d-none");
    }
    if (bar) {
        bar.style.width = "0%";
        bar.setAttribute("aria-valuenow", "0");
        bar.classList.add("progress-bar-striped", "progress-bar-animated");
    }
    if (text) {
        text.textContent = "";
    }
    if (status) {
        status.textContent = "";
        status.classList.add("d-none");
    }
}

function send_amyboard_progress_update(sentBytes, totalBytes) {
    var wrapper = document.getElementById("send-amyboard-progress-wrap");
    var bar = document.getElementById("send-amyboard-progress-bar");
    var text = document.getElementById("send-amyboard-progress-text");
    if (!wrapper || !bar || !text) {
        return;
    }
    var safeTotal = Math.max(0, Number(totalBytes) || 0);
    var safeSent = Math.max(0, Math.min(safeTotal, Number(sentBytes) || 0));
    var percent = safeTotal > 0 ? Math.floor((safeSent * 100) / safeTotal) : 100;
    wrapper.classList.remove("d-none");
    bar.style.width = percent + "%";
    bar.setAttribute("aria-valuenow", String(percent));
    if (percent >= 100) {
        bar.classList.remove("progress-bar-animated");
    } else {
        bar.classList.add("progress-bar-animated");
    }
    text.textContent = percent + "% (" + safeSent + " / " + safeTotal + " bytes)";
}

function set_send_amyboard_status(text, isError) {
    var status = document.getElementById("send-amyboard-status");
    if (!status) {
        return;
    }
    status.textContent = text || "";
    status.classList.remove("d-none", "text-danger", "text-success");
    status.classList.add(isError ? "text-danger" : "text-success");
}

function bytes_to_base64_ascii(bytes) {
    var chunk = "";
    for (var i = 0; i < bytes.length; i++) {
        chunk += String.fromCharCode(bytes[i]);
    }
    return btoa(chunk);
}

async function sysex_write_amy_message(message) {
    var outputDevice = get_selected_midi_output_device() || midiOutputDevice;
    if (!outputDevice) {
        throw new Error("No MIDI output is selected.");
    }
    midiOutputDevice = outputDevice;
    var payload = Array.from(new TextEncoder().encode(String(message || "")));
    if (typeof outputDevice.sendSysex === "function") {
        outputDevice.sendSysex(AMYBOARD_SYSEX_MFR_ID, payload);
    } else if (typeof outputDevice.send === "function") {
        outputDevice.send([0xF0].concat(AMYBOARD_SYSEX_MFR_ID, payload, [0xF7]));
    } else {
        throw new Error("Selected MIDI output does not support sysex send.");
    }
    // Matches amy.sysex_write pacing to avoid overrunning USB-MIDI consumers.
    await sleep_ms(25);
}

async function open_send_to_amyboard_modal() {
    try {
        await write_current_patches_file_from_knobs();
    } catch (e) {
        show_alert("Could not update web.patch");
    }
    send_amyboard_progress_reset();
    populate_send_to_amyboard_modal();
}

async function send_to_amyboard_now() {
    var sendButton = document.getElementById("send-amyboard-send-btn");
    if (sendButton && sendButton.disabled) {
        return;
    }
    send_amyboard_progress_reset();

    if (!WebMidi.supported) {
        show_alert("WebMIDI is not available in this browser.");
        return;
    }

    if (sendButton) {
        sendButton.disabled = true;
        sendButton.innerHTML = '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span>Sending...';
    }

    try {
        await save_editor_if_dirty();
        await write_current_patches_file_from_knobs();
        await setup_midi_devices();
        if (!midiOutputDevice) {
            throw new Error("No MIDI out port selected.");
        }

        var tarFilename = "environment.tar";
        var tarBytes = build_environment_tar_bytes();
        var fileSize = tarBytes.length;

        send_amyboard_progress_update(0, fileSize);
        await sysex_write_amy_message("zT" + tarFilename + "," + fileSize + "Z");

        var sentBytes = 0;
        for (var offset = 0; offset < fileSize; offset += AMYBOARD_TRANSFER_CHUNK_BYTES) {
            var chunk = tarBytes.slice(offset, offset + AMYBOARD_TRANSFER_CHUNK_BYTES);
            var b64 = bytes_to_base64_ascii(chunk);
            await sysex_write_amy_message(b64);
            sentBytes += chunk.length;
            send_amyboard_progress_update(sentBytes, fileSize);
        }
        set_send_amyboard_status("Environment sent to AMYboard.", false);
    } catch (e) {
        var bar = document.getElementById("send-amyboard-progress-bar");
        if (bar) {
            bar.classList.remove("progress-bar-animated");
        }
        set_send_amyboard_status("Send failed: " + (e && e.message ? e.message : "Unknown error"), true);
        show_alert("Failed to send to AMYboard.");
    } finally {
        if (sendButton) {
            sendButton.disabled = false;
            sendButton.textContent = "Send";
        }
    }
}

async function show_editor() {
    document.getElementById('showhideeditor').style.display='none'; 
    if(editor) editor.refresh();
    //document.getElementById('canvas').classList.remove("canvas-solo");
    //document.getElementById('canvas').classList.add("canvas-editor");
    editor_shown = true;
    //resize_tulip_grippie();
}

async function hide_editor() {
    document.getElementById('showhideeditor').style.display='';
    //document.getElementById('canvas').classList.remove("canvas-editor");
    //document.getElementById('canvas').classList.add("canvas-solo");
    editor_shown = false;
    //resize_tulip_grippie();
}

async function toggle_audioin() {
    if(!audio_started) await sleep_ms(1000);
    await amy_live_stop();
    if (document.getElementById('amy_audioin').checked) {
        amy_audioin_toggle = true;
        await amy_live_start_web_audioin();
    } else {
        amy_audioin_toggle = false;
        await amy_live_start_web();
    }
}

 function get_audio_samples() {
     var res_ptr = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
     amy_module.ccall('amy_get_input_buffer', null, ["number"], [res_ptr]);
     var view = amy_module.HEAPU8.subarray(res_ptr , res_ptr + 1024);
     amy_module._free(res_ptr);
     return view
 }

 function set_audio_samples(samples) {
     var res_ptr = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
     amy_module.HEAPU8.set(samples, res_ptr);
     amy_module.ccall('amy_set_external_input_buffer', null, ["number"], [res_ptr]);
     amy_module._free(res_ptr);
 }

async function amy_block_processed_js_hook() {
    mp.runPythonAsync(`if amy.block_cb is not None: amy.block_cb(None)`);
}

async function start_amyboard() {
  // Don't run this twice
  if(amyboard_started) return;

  // Start midi
  await start_midi();

  // Let micropython call an exported AMY function
  await mp.registerJsModule('amy_js_message', amy_add_message);
  await mp.registerJsModule('amy_sysclock', amy_sysclock);
  await mp.registerJsModule('amyboard_world_upload_file', amyboard_world_upload_file);
//  await mp.registerJsModule('amy_get_input_buffer', get_audio_samples);
//  await mp.registerJsModule('amy_set_external_input_buffer', set_audio_samples);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import tulip, amy, amy_js_message, amy_sysclock
    amy.override_send = amy_js_message
    tulip.amy_ticks_ms = amy_sysclock
  `);

  // If you don't have these sleeps we get a MemoryError with a locked heap. Not sure why yet.
  await sleep_ms(400);
  await mp.runFrozenAsync('_boot.py');

  await sleep_ms(400);
  await mp.runFrozenAsync('/amyboard/user/boot.py');
  ensure_current_environment_layout(true);
  var startupPatchExecution = await execute_current_patch_files();
  if (should_initialize_default_patch_state(startupPatchExecution)) {
      await apply_default_patch_state_from_clear_patches();
  }
  await run_current_environment();
  await fill_tree();
  if (list_environment_files().indexOf("env.py") !== -1) {
      await select_environment_file("env.py", true);
  } else {
      var envFiles = list_environment_files();
      if (envFiles.length) {
          await select_environment_file(envFiles[0], true);
      }
  }
  amyboard_started = true;
}

async function start_audio() {
  document.body.removeEventListener('click', start_audio, true); 
  document.body.removeEventListener('keydown', start_audio, true); 

  // Don't run this twice
  if(audio_started) return;
  // Start the audio worklet (miniaudio)
  if(amy_audioin_toggle) {
      await amy_live_start_web_audioin();
  } else {
      await amy_live_start_web();    
  }
  // Initialize default channel state, then sync preset/knobs.
  audio_started = true;
  attemptPatchKnobSync();
}
