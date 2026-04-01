
var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
var amyboard_started = false;
var amy_yield_patch_events = null;
var amy_yield_synth_commands = null;

var _python_error_buffer = "";
var _python_error_timer = null;

// Intercept console.log to catch Python errors printed by micropython WASM
var _orig_console_log = console.log.bind(console);
console.log = function() {
  _orig_console_log.apply(console, arguments);
  var msg = Array.from(arguments).join(" ");
  if (msg.indexOf("Traceback") !== -1 || msg.indexOf("sketch.py load failed") !== -1 ||
      msg.indexOf("sketch.loop() error") !== -1 || _python_error_buffer.length > 0) {
    _python_error_buffer += msg + "\n";
    if (_python_error_timer) clearTimeout(_python_error_timer);
    _python_error_timer = setTimeout(function() {
      if (_python_error_buffer.trim().length > 0 && typeof show_python_error === "function") {
        show_python_error(_python_error_buffer.trim());
      }
      _python_error_buffer = "";
      _python_error_timer = null;
    }, 500);
  }
};

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
var amyboard_world_files = [];
var amyboard_world_loading_index = null;
var amyboard_world_selected_tag = "";
var amyboard_world_tag_palette = {};
var amyboard_world_patches = [];
var amyboard_world_patch_loading_index = null;
var amyboard_world_patch_selected_tag = "";
var amyboard_world_patch_tag_palette = {};
var amyboard_world_patch_previewing_index = null;
var selected_environment_file = null;
var pending_environment_editor_load = false;
var environment_editor_dirty = false;
var environment_autosave_inflight = false;
const AMYBOARD_WORLD_API_BASE = (typeof window !== "undefined" && typeof window.AMYBOARD_WORLD_API_BASE === "string")
    ? String(window.AMYBOARD_WORLD_API_BASE).replace(/\/+$/, "")
    : String(window.location.origin || "").replace(/\/+$/, "");
const CURRENT_BASE_DIR = "/amyboard/user/current";
const CURRENT_ENV_DIR = CURRENT_BASE_DIR;
const DEFAULT_SKETCH_SOURCE = "# AMYboard Sketch\n# Top-level code runs once at boot. loop() is called every 32nd note.\n\ndef loop():\n    pass\n";
const EDITOR_ALLOWED_EXTENSIONS = [".py", ".txt", ".json", ".patch"];
const AMYBOARD_SYSEX_MFR_ID = [0x00, 0x03, 0x45];
const AMYBOARD_TRANSFER_CHUNK_BYTES = 188;
window.current_synth = 1;
window.active_channels = Array.isArray(window.active_channels) ? window.active_channels : new Array(17).fill(false);
window.active_channels[1] = true;
window.channel_control_mapping_sent = Array.isArray(window.channel_control_mapping_sent) ? window.channel_control_mapping_sent : new Array(17).fill(false);
window.channel_patch_names = Array.isArray(window.channel_patch_names) ? window.channel_patch_names : new Array(17).fill(null);
window.channel_patch_dirty = Array.isArray(window.channel_patch_dirty) ? window.channel_patch_dirty : new Array(17).fill(false);
window.suppress_knob_cc_send = false;
const EDITOR_STATE_FILENAME = "editor_state.json";
const DIRTY_AUTOSAVE_DELAY_MS = 180;
const channel_dirty_autosave_timers = new Array(17).fill(null);

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
  amy_yield_patch_events = am.cwrap(
    'yield_patch_events', 'number', ['number', 'number', 'number']
  );
  amy_yield_synth_commands = am.cwrap(
    'yield_synth_commands', 'number', ['number', 'number', 'number', 'number']
  );
  amy_start_web_no_synths();
  amy_module = am;
  res_ptr_in = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  res_ptr_out = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  // Expose on globalThis so MicroPython JS module calls can access them
  globalThis._amy_module_ref = amy_module;
  globalThis._amy_res_ptr_out = res_ptr_out;
});

function read_c_string_from_heap(ptr, maxLen) {
  if (!amy_module || !amy_module.HEAPU8) {
    return "";
  }
  const heap = amy_module.HEAPU8;
  const start = Number(ptr);
  const limit = Math.max(0, Number(maxLen) || 0);
  if (!Number.isInteger(start) || start <= 0 || limit <= 0) {
    return "";
  }
  const end = Math.min(heap.length, start + limit);
  let out = "";
  for (let i = start; i < end; i += 1) {
    const b = heap[i];
    if (b === 0) {
      break;
    }
    out += String.fromCharCode(b);
  }
  return out;
}

function read_editor_state_json() {
  let raw = "";
  try {
    raw = mp.FS.readFile(CURRENT_ENV_DIR + "/" + EDITOR_STATE_FILENAME, { encoding: "utf8" });
  } catch (e) {
    raw = "";
  }
  let parsed = {};
  try {
    parsed = raw ? JSON.parse(String(raw)) : {};
  } catch (e) {
    parsed = {};
  }
  if (!parsed || typeof parsed !== "object") {
    parsed = {};
  }
  if (!parsed.synths || typeof parsed.synths !== "object" || Array.isArray(parsed.synths)) {
    parsed.synths = {};
  }
  return parsed;
}

function normalize_synth_channel(channel) {
  const synth = Number(channel);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    return null;
  }
  return synth;
}

function remove_current_environment_file_if_exists(filename) {
  const name = String(filename || "").trim();
  if (!name) {
    return false;
  }
  try {
    const fullPath = CURRENT_ENV_DIR + "/" + name;
    const node = mp.FS.lookupPath(fullPath).node;
    if (mp.FS.isFile(node.mode)) {
      mp.FS.unlink(fullPath);
      return true;
    }
  } catch (e) {}
  return false;
}

function set_channel_patch_dirty_state(channel, dirty) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    return;
  }
  window.channel_patch_dirty[synth] = !!dirty;
  if (typeof window.refresh_save_patch_dirty_indicator === "function") {
    window.refresh_save_patch_dirty_indicator();
  }
}

function num_oscs_from_patch_file_content(source) {
  var maxOsc = -1;
  var lines = String(source || "").split(/\r?\n/);
  for (var i = 0; i < lines.length; i++) {
    var m = lines[i].match(/^v(\d+)/);
    if (m) {
      var osc = parseInt(m[1], 10);
      if (osc > maxOsc) maxOsc = osc;
    }
  }
  return maxOsc < 0 ? 3 : maxOsc + 1;
}

function get_wire_commands_for_juno_patch(patch) {
  console.log("get_wire_commands_for_juno_patch");

  // The new convention for AMYboard voice osc usage.
  const OSCA_OSC = 0;
  const FILTER_OSC = 0;  // Osc with filter commands.
  const OSCB_OSC = 2;
  const LFO_OSC = 1;

  function translate_juno_events_to_webeditor_wire_commands(events) {
    let filterFreq = null;
    let filterEnv = null;
    let filterLfo = null
    let filterKbd = null;
    let filter_osc = 0;  // The osc with the filter.  It's always 0.
    let resonanceValue = null;
    let adsr = [0, 0, 1, 0];
    let lfoFreq = null;
    let lfoDelay = null;
    let lfoWave = 4;  // always triangle
    let lfoOsc = 0;
    let lfoPwm = 0;
    // Track coefficients for all 5 oscs.  Not sure which one is LFO.
    let osc_freq = [null, null, null, null, null];
    let osc_wave = [0, 0, 0, 0, 0];
    let osc_duty = [0.5, 0.5, 0.5, 0.5, 0.5];
    let osc_gain = [0, 0, 0, 0, 0];
    let mod_source_osc = LFO_OSC;  // Start with Juno LFO osc, but may get updated if it's an amyboardsynth patch.
    // Which Juno oscs are used for oscA and B
    let oscAB_osc = [-1, -1];
    let oscAB_gain = [0, 0];
    // Do the oscs use ADSR, or the juno "gate"?
    let oscGate = 0;
    let eq = [null, null, null];
    let chorus = [0, 0.5, 0.5];
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

      // Remainder of block assumes osc is set.
      if (!Number.isFinite(event.osc)) continue;

      if (event.filter_freq) {
        if (Number.isFinite(event.filter_freq[0])) {
          filter_osc = event.osc;  // Assume we'll see this at least once
          filterFreq = event.filter_freq[0];
        }
        if (Number.isFinite(event.filter_freq[1])) {
          filterKbd = event.filter_freq[1];  // COEF_NOTE
        }
        if (Number.isFinite(event.filter_freq[3])) {
          filterEnv = event.filter_freq[3];  // COEF_EG0, Juno filter env
        }
        if (Number.isFinite(event.filter_freq[5])) {
          filterLfo = event.filter_freq[5];  // COEF_MOD
        }
      }
      if (Number.isFinite(event.resonance)) {
        resonanceValue = event.resonance;
      }
      if (event.osc == mod_source_osc) {
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
      } else if (event.osc >= 0) {
        // Non-LFO osc, don't assume what order they come in.
        const parsedModSource = Number(event.mod_source);
        if (Number.isInteger(parsedModSource) && parsedModSource >= 0 && parsedModSource < 64) {
          mod_source_osc = parsedModSource;  // Should never happen.
        }
        if (event.eg0_times) {
          if (bpTimeIsSet(event.eg0_times[0])) { adsr[0] = event.eg0_times[0]; }   // A time
          if (bpTimeIsSet(event.eg0_times[1])) { adsr[1] = event.eg0_times[1]; }   // D time
          if (bpTimeIsSet(event.eg0_times[2])) { adsr[3] = event.eg0_times[2]; }   // R time
        }
        if (event.eg0_values) {
          if (Number.isFinite(event.eg0_values[1])) { adsr[2] = event.eg0_values[1]; }  // S level
        }
        // Extract key parameters for each osc
        if (event.amp) {
          if (Number.isFinite(event.amp[2]) && event.amp[2] > 0) {
            osc_gain[event.osc] = event.amp[2];
          }
          if (Number.isFinite(event.amp[3]) && event.amp[3] == 0) {
            // Juno patches decouple amp from EG0 for "gate" mode.
            oscGate = 1;
          }
        }
        if (event.freq) {
          if (Number.isFinite(event.freq[0]) && event.freq[0] > 0) {
            osc_freq[event.osc] = event.freq[0];
          }
          if (Number.isFinite(event.freq[5]) && event.freq[5] > 0) {
            lfoOsc = event.freq[5];  // freq COEF_MOD == 5
          }
        }
        if (event.duty) {
          if (Number.isFinite(event.duty[0]) && event.duty[0] > 0) {
            osc_duty[event.osc] = event.duty[0];
          }
          if (Number.isFinite(event.duty[5]) && event.duty[5] > lfoPwm) {
            lfoPwm = event.duty[5];  // duty COEF_MOD == 5
          }
        }
        if (event.wave && event.wave >= 0 && event.wave < 127) {
          osc_wave[event.osc] = event.wave
        }
      }
    }

    if (oscGate) {
      adsr = [0, 0, 1, 0];
    }
    // Logic to choose juno oscs for osc A and osc B.
    // Remove the LFO from the lists
    osc_gain.splice(mod_source_osc, 1);
    osc_freq.splice(mod_source_osc, 1);
    osc_wave.splice(mod_source_osc, 1);
    osc_duty.splice(mod_source_osc, 1);
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

    // Format the final wire commands for the 3-osc config.
    // Helper: null/undefined → "" for safe wire code concatenation.
    const s = (v) => v != null ? v : "";

    const AMY_OSC_OF_LOGICAL_OSC = [OSCA_OSC, OSCB_OSC];
    wire_commands = [];
    for (const osc of [0, 1]) {
      let src_osc = oscAB_osc[osc];
      let amy_osc = AMY_OSC_OF_LOGICAL_OSC[osc];  // too many osc numbers.
	let command = "v" + amy_osc + "w" + osc_wave[src_osc] + "L" + LFO_OSC + "m0"
          + "f" + s(osc_freq[src_osc]) + ",1,,,," + lfoOsc + ",1"
          + "d" + osc_duty[src_osc] + ",,,,," + lfoPwm
          + "a,," + osc_gain[src_osc] + ",1,0"
          + "A" + adsr[0] + ",1," + adsr[1] + "," + adsr[2] + "," + adsr[3] + ",0";
      if (osc == 0) {
        // Osc 0 must chain to osc 1
        command += "c" + AMY_OSC_OF_LOGICAL_OSC[1];
      }
      if (amy_osc == FILTER_OSC) {
        // Osc 0 has the filter controls, including EG1, which is in fact the same as EG0
        command += "G" + AMY.FILTER_LPF24;
        if (resonanceValue != null) command += "R" + resonanceValue;
        command += "F" + s(filterFreq) + "," + s(filterKbd) + ",,," + s(filterEnv) + "," + s(filterLfo)
            + "B" + adsr[0] + ",1," + adsr[1] + "," + adsr[2] + "," + adsr[3] + ",0";
      }
      wire_commands.push(command + "Z");
    }
    // LFO command
    let command = "v" + LFO_OSC + "w" + lfoWave + "a1,0,0,1" + "f" + s(lfoFreq) + ",0,,0,0,0,0"
        + "A" + s(lfoDelay) + ",1,100,1,10000,0";
    wire_commands.push(command + "Z");
    // Global FX commands
    command = "x" + s(eq[0]) + "," + s(eq[1]) + "," + s(eq[2])
      + "k" + chorus[0] + ",," + chorus[1] + "," + chorus[2];
    wire_commands.push(command + "Z");
    return wire_commands;
  }

  // The concatenated Z-separated string defining the Juno patch.
  let message = patch_code_for_patch_number[patch];
  let events = events_from_wire_code_message(message);
  wire_commands = translate_juno_events_to_webeditor_wire_commands(events);
  return wire_commands;
}

function get_wire_commands_for_channel(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    throw new Error("Invalid channel.");
  }
  if (!amy_module || typeof amy_yield_synth_commands !== "function") {
    throw new Error("AMY patch command generator is unavailable.");
  }
  const maxMessageLen = 1024;
  const bufferPtr = amy_module._malloc(maxMessageLen);
  if (!bufferPtr) {
    throw new Error("Failed to allocate AMY message buffer.");
  }
  const lines = [];
  let state = 0;
  do {
    state = amy_yield_synth_commands(synth, bufferPtr, maxMessageLen, state);
    const wire = read_c_string_from_heap(bufferPtr, maxMessageLen).trim();
    if (wire) {
      lines.push(wire);
    }
  } while (state != 0);
  amy_module._free(bufferPtr);
  if (!lines.length) {
    throw new Error("No synth commands were generated for this channel.");
  }
  return lines;
}

function write_channel_dirty_patch_file(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    throw new Error("Invalid channel.");
  }
  const lines = get_wire_commands_for_channel(synth);
  const filename = String(synth) + ".dirty";
  mp.FS.writeFile(CURRENT_ENV_DIR + "/" + filename, lines.join("\n") + "\n");
  return filename;
}

const pending_channel_knob_sync = new Array(17).fill(false);

function schedule_channel_knob_sync_after_ui_ready(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    return false;
  }
  if (pending_channel_knob_sync[synth]) {
    return true;
  }
  pending_channel_knob_sync[synth] = true;
  const runSync = async function() {
    pending_channel_knob_sync[synth] = false;
    try {
      await sync_channel_knobs_from_synth_to_ui(synth);
    } catch (e) {}
  };
  if (document.readyState !== "complete") {
    window.addEventListener("DOMContentLoaded", runSync, { once: true });
  } else {
    setTimeout(runSync, 25);
  }
  return true;
}

async function sync_channel_knobs_from_synth_to_ui(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    return false;
  }
  if (typeof set_knobs_from_synth !== "function"
    || typeof window.refresh_knobs_for_channel !== "function"
    || typeof window.get_current_knobs !== "function") {
    schedule_channel_knob_sync_after_ui_ready(synth);
    return false;
  }
  const previousChannel = Number(window.current_synth || 1);
  const previousSuppress = !!window.suppress_knob_cc_send;
  window.current_synth = synth;
  window.suppress_knob_cc_send = true;
  try {
    let lastError = null;
    for (let attempt = 0; attempt < 24; attempt += 1) {
      // AMY applies queued wire messages asynchronously; keep retrying briefly during startup.
      await new Promise(function(resolve) { setTimeout(resolve, attempt === 0 ? 20 : 40); });
      try {
        set_knobs_from_synth(synth);
        window.refresh_knobs_for_channel();
        return true;
      } catch (e) {
        lastError = e;
      }
    }
    if (lastError) {
      console.warn("Failed to sync knobs from synth " + synth + ".", lastError);
    }
    return false;
  } finally {
    window.suppress_knob_cc_send = previousSuppress;
    window.current_synth = previousChannel;
  }
}

function schedule_dirty_autosave_for_channel(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth || window.suppress_knob_cc_send) {
    return;
  }
  set_channel_patch_dirty_state(synth, true);
  const existingTimer = channel_dirty_autosave_timers[synth];
  if (existingTimer) {
    clearTimeout(existingTimer);
  }
  channel_dirty_autosave_timers[synth] = setTimeout(async function() {
    channel_dirty_autosave_timers[synth] = null;
    try {
      ensure_current_environment_layout(true);
      write_channel_dirty_patch_file(synth);
      await sync_persistent_fs();
    } catch (e) {}
  }, DIRTY_AUTOSAVE_DELAY_MS);
}

function write_editor_state_json(state) {
  const payload = JSON.stringify(state || { synths: {} }, null, 2) + "\n";
  mp.FS.writeFile(CURRENT_ENV_DIR + "/" + EDITOR_STATE_FILENAME, payload);
}

function set_editor_state_patch_name(channel, patchName) {
  const synth = Number(channel);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    return;
  }
  const state = read_editor_state_json();
  if (!state.synths || typeof state.synths !== "object" || Array.isArray(state.synths)) {
    state.synths = {};
  }
  const key = String(synth);
  const name = (patchName == null) ? "" : String(patchName).trim();
  if (name) {
    state.synths[key] = name;
  } else {
    delete state.synths[key];
  }
  write_editor_state_json(state);
}


window.save_current_synth_patch_file = async function(rawName) {
  ensure_current_environment_layout(true);
  if (!amy_module || typeof amy_yield_synth_commands !== "function") {
    throw new Error("AMY patch command generator is unavailable.");
  }

  let name = String(rawName || "").trim();
  if (name.toLowerCase().endsWith(".patch")) {
    name = name.slice(0, -6);
  }
  if (!/^[A-Za-z0-9._-]{1,25}$/.test(name)) {
    throw new Error("Patch name must be 1-25 chars: letters, numbers, dot, underscore, dash.");
  }

  const synth = Number(window.current_synth || 1);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    throw new Error("Invalid channel.");
  }

  const lines = get_wire_commands_for_channel(synth);
  
  if (!lines.length) {
    throw new Error("No synth commands were generated for this channel.");
  }

  const filename = name + ".patch";
  mp.FS.writeFile(CURRENT_ENV_DIR + "/" + filename, lines.join("\n") + "\n");
  window.channel_patch_names[synth] = name;
  set_editor_state_patch_name(synth, name);
  remove_current_environment_file_if_exists(String(synth) + ".dirty");
  set_channel_patch_dirty_state(synth, false);
  await sync_persistent_fs();
  await fill_tree();
  return filename;
};

window.load_saved_patch_file_into_current_channel = async function(rawFilename) {
  ensure_current_environment_layout(true);

  let filename = String(rawFilename || "").trim();
  if (!filename) {
    throw new Error("No patch selected.");
  }
  if (!filename.toLowerCase().endsWith(".patch")) {
    filename += ".patch";
  }
  if (!/^[A-Za-z0-9._-]+\.patch$/i.test(filename)) {
    throw new Error("Invalid patch filename.");
  }

  const synth = Number(window.current_synth || 1);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    throw new Error("Invalid channel.");
  }

  let source = "";
  try {
    source = mp.FS.readFile(CURRENT_ENV_DIR + "/" + filename, { encoding: "utf8" });
  } catch (e) {
    throw new Error("Could not read patch file.");
  }

  var oscs_per_voice = num_oscs_from_patch_file_content(source);
  amy_send({synth: synth, midi_cc: "255"}, true);
  amy_send({synth: synth, num_voices: 6, oscs_per_voice: oscs_per_voice}, true);
  reset_global_effects();
  const lines = String(source || "").split(/\r?\n/);
  for (const line of lines) {
    const wire = String(line || "").trim();
    if (!wire) {
      continue;
    }
    amy_add_log_message("i" + synth + wire);
  }
  if (typeof window.apply_knob_cc_mappings_from_patch_source === "function") {
    window.apply_knob_cc_mappings_from_patch_source(synth, source);
  }
  send_all_knob_cc_mappings(synth);
  await sync_channel_knobs_from_synth_to_ui(synth);
  if (typeof window.set_section_disabled === "function") {
    var isDX7 = oscs_per_voice >= 8;
    window.set_section_disabled("Osc B", isDX7);
    window.set_section_disabled("ADSR", isDX7);
  }
  const patchName = filename.replace(/\.patch$/i, "");
  window.channel_patch_names[synth] = patchName;
  set_editor_state_patch_name(synth, patchName);
  remove_current_environment_file_if_exists(String(synth) + ".dirty");
  set_channel_patch_dirty_state(synth, false);
  await sync_persistent_fs();
  return filename;
};

function apply_channel_active_ui_from_loaded_map(loadedMap) {
  var anyActive = false;
  for (var ch = 1; ch <= 16; ch++) {
    var active = !!loadedMap[ch];
    if (typeof window.set_channel_active === "function") {
      window.set_channel_active(ch, active);
    } else if (Array.isArray(window.active_channels)) {
      window.active_channels[ch] = active;
    }
    if (active) {
      anyActive = true;
    }
  }
  if (!anyActive) {
    return;
  }
  var current = Number(window.current_synth || 1);
  if (!Number.isInteger(current) || current < 1 || current > 16 || !loadedMap[current]) {
    for (var pick = 1; pick <= 16; pick++) {
      if (loadedMap[pick]) {
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
}

async function restore_patches_from_editor_state_if_present(options) {
  const opts = options || {};
  const sendToAmy = opts.sendToAmy !== false;
  let hasEditorState = false;
  try {
    mp.FS.readFile(CURRENT_ENV_DIR + "/" + EDITOR_STATE_FILENAME, { encoding: "utf8" });
    hasEditorState = true;
  } catch (e) {
    hasEditorState = false;
  }
  const state = hasEditorState ? read_editor_state_json() : null;
  const synthMap = (state && state.synths && typeof state.synths === "object") ? state.synths : {};
  const loadedMap = new Array(17).fill(false);
  const loadedOscsMap = new Array(17).fill(0);
  const loadedSynths = [];
  let loadedCount = 0;
  for (let ch = 1; ch <= 16; ch += 1) {
    set_channel_patch_dirty_state(ch, false);
  }

  // Scan for .dirty files not covered by editor_state.json
  try {
    var envEntries = mp.FS.readdir(CURRENT_ENV_DIR);
    for (var ei = 0; ei < envEntries.length; ei++) {
      var entry = envEntries[ei];
      if (entry.endsWith(".dirty")) {
        var dsynth = parseInt(entry.slice(0, -6), 10);
        if (dsynth >= 1 && dsynth <= 16 && !synthMap[String(dsynth)]) {
          synthMap[String(dsynth)] = "";
        }
      }
    }
  } catch (e) {}

  if (!Object.keys(synthMap).length) {
    if (sendToAmy) {
      amy_send({synth: 1, midi_cc: "255"}, true);
      amy_send({synth: 1, patch: 257, num_voices: 6}, true);
      send_all_knob_cc_mappings(1);
    }
    await sync_channel_knobs_from_synth_to_ui(1);
    return { hasEditorState: hasEditorState, loadedCount: 0 };
  }

  for (const key in synthMap) {
    if (!Object.prototype.hasOwnProperty.call(synthMap, key)) {
      continue;
    }
    const synth = Number(key);
    if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
      continue;
    }
    const name = String(synthMap[key] || "").trim();
    if (name) {
      window.channel_patch_names[synth] = name.replace(/\.patch$/i, "");
    }
    const dirtyFilename = String(synth) + ".dirty";
    let dirtySource = "";
    try {
      dirtySource = mp.FS.readFile(CURRENT_ENV_DIR + "/" + dirtyFilename, { encoding: "utf8" });
    } catch (e) {
      dirtySource = "";
    }
    let filename = name ? (name.toLowerCase().endsWith(".patch") ? name : (name + ".patch")) : "";
    let source = "";
    if (dirtySource) {
      source = dirtySource;
      set_channel_patch_dirty_state(synth, true);
    } else if (filename) {
      try {
        source = mp.FS.readFile(CURRENT_ENV_DIR + "/" + filename, { encoding: "utf8" });
      } catch (e) {
        continue;
      }
      set_channel_patch_dirty_state(synth, false);
    } else {
      continue;
    }
    var oscs_per_voice = num_oscs_from_patch_file_content(source);
    if (sendToAmy) {
      amy_send({synth: synth, midi_cc: "255"}, true);
      amy_send({synth: synth, num_voices: 6, oscs_per_voice: oscs_per_voice}, true);
      reset_global_effects();
    }
    const lines = String(source || "").split(/\r?\n/);
    for (const line of lines) {
      const wire = String(line || "").trim();
      if (!wire || wire.startsWith("#")) {
        continue;
      }
      if (sendToAmy) {
        amy_add_log_message("i" + synth + wire);
      }
    }
    if (typeof window.apply_knob_cc_mappings_from_patch_source === "function") {
      window.apply_knob_cc_mappings_from_patch_source(synth, source);
    }
    if (sendToAmy) {
      send_all_knob_cc_mappings(synth);
    }
    await sync_channel_knobs_from_synth_to_ui(synth);
    loadedMap[synth] = true;
    loadedOscsMap[synth] = oscs_per_voice;
    loadedSynths.push(synth);
    loadedCount += 1;
  }

  if (!sendToAmy && loadedSynths.length > 0) {
    // On startup, Python may still be streaming patch messages into AMY while we read synth state.
    // Run a second sync pass shortly after to capture final settled values.
    await new Promise(function(resolve) { setTimeout(resolve, 350); });
    for (const synth of loadedSynths) {
      await sync_channel_knobs_from_synth_to_ui(synth);
    }
  }

  // If the active channel had no patch mapping, initialize it with a clean slate.
  const activeCh = Number(window.current_synth || 1);
  if (!loadedMap[activeCh] && sendToAmy) {
    amy_send({synth: activeCh, midi_cc: "255"}, true);
    amy_send({synth: activeCh, patch: 257, num_voices: 6}, true);
    send_all_knob_cc_mappings(activeCh);
  }

  // Apply section blur for the active channel only (once, after all syncing is done).
  if (typeof window.set_section_disabled === "function") {
    var activeOscs = loadedOscsMap[activeCh] || 0;
    var activeIsDX7 = activeOscs >= 8;
    window.set_section_disabled("Osc B", activeIsDX7);
    window.set_section_disabled("ADSR", activeIsDX7);
  }

  apply_channel_active_ui_from_loaded_map(loadedMap);
  return { hasEditorState: true, loadedCount: loadedCount };
}

window.clear_current_channel_patch = async function() {
  ensure_current_environment_layout(true);
  const synth = Number(window.current_synth || 1);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    throw new Error("Invalid channel.");
  }
  reset_global_effects();
  amy_send({synth: synth, midi_cc: "255"}, true);
  amy_send({synth: synth, patch: 257, num_voices: 6}, true);
  send_all_knob_cc_mappings(synth);
  await sync_channel_knobs_from_synth_to_ui(synth);
  if (typeof window.set_section_disabled === "function") {
    window.set_section_disabled("Osc B", false);
    window.set_section_disabled("ADSR", false);
  }
  window.channel_patch_names[synth] = null;
  set_editor_state_patch_name(synth, null);
  remove_current_environment_file_if_exists(String(synth) + ".dirty");
  set_channel_patch_dirty_state(synth, false);
  await sync_persistent_fs();
  return synth;
};

function onKnobCcChange(knob, previousCc) {
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
  var ccVal = build_knob_cc_value(knob);
  if (!ccVal) {
    return;
  }
  if (window.suppress_knob_cc_send) {
    return;
  }
  var synthChannel = Number(window.current_synth || 1);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = 1;
  }
  // If the CC number changed, remove the old CC mapping from AMY first.
  if (previousCc !== undefined && previousCc !== "" && previousCc !== knob.cc) {
    amy_send({synth: synthChannel, midi_cc: previousCc + ",0,0,0,0,"}, true);
  }
  amy_send({synth: synthChannel, midi_cc: ccVal}, true);
}

window.onKnobChange = function(_index, _value) {
  if (window.suppress_knob_cc_send) {
    return;
  }
  const synth = normalize_synth_channel(window.current_synth || 1);
  if (!synth) {
    return;
  }
  schedule_dirty_autosave_for_channel(synth);
};

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
  // Set the JS globals that amy.wasm reads in its audio loop
  if (index === 0) cv_1_voltage = value;
  else if (index === 1) cv_2_voltage = value;
  // Also update the Python/MicroPython side
  if (mp) mp.runPythonAsync("tulip.cv_local(" + index + ", " + value + ")");
};

// --- Rotary encoder emulation (24 pulses, wraps 0-23) ---
(function() {
  var encoderPos = 0;
  var ENCODER_STEPS = 24;
  var valueEl = null;

  function updateDisplay() {
    if (!valueEl) valueEl = document.getElementById('encoder-value');
    if (valueEl) valueEl.textContent = encoderPos;
  }

  function encoderTurn(delta) {
    encoderPos = ((encoderPos + delta) % ENCODER_STEPS + ENCODER_STEPS) % ENCODER_STEPS;
    updateDisplay();
    if (mp) mp.runPythonAsync("amyboard._web_encoder_turn(" + delta + ")");
  }

  function encoderPress(down) {
    if (mp) mp.runPythonAsync("amyboard._web_encoder_press(" + (down ? "True" : "False") + ")");
  }

  document.addEventListener('DOMContentLoaded', function() {
    var leftBtn = document.getElementById('encoder-left');
    var rightBtn = document.getElementById('encoder-right');
    var pushBtn = document.getElementById('encoder-push');
    if (leftBtn) leftBtn.addEventListener('click', function() { encoderTurn(-1); });
    if (rightBtn) rightBtn.addEventListener('click', function() { encoderTurn(1); });
    if (pushBtn) {
      pushBtn.addEventListener('mousedown', function() {
        pushBtn.classList.add('pressed');
        encoderPress(true);
      });
      pushBtn.addEventListener('mouseup', function() {
        pushBtn.classList.remove('pressed');
        encoderPress(false);
      });
      pushBtn.addEventListener('mouseleave', function() {
        if (pushBtn.classList.contains('pressed')) {
          pushBtn.classList.remove('pressed');
          encoderPress(false);
        }
      });
    }
  });
})();

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

function reset_global_effects() {
  // Reset all global effect knobs to 0 in AMY and refresh the UI.
  var globalKnobs = typeof window.get_global_knobs === "function"
    ? window.get_global_knobs() : [];
  for (var i = 0; i < globalKnobs.length; i++) {
    var knob = globalKnobs[i];
    if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half") {
      continue;
    }
    knob.default_value = (knob.amy_default !== undefined) ? knob.amy_default : 0;
    var payload = window.make_change_code(1, knob.default_value, knob, false);
    if (payload) {
      amy_add_log_message(payload);
    }
  }
  if (typeof window.refresh_knobs_for_channel === "function") {
    var previousSuppress = !!window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = true;
    window.refresh_knobs_for_channel();
    window.suppress_knob_cc_send = previousSuppress;
  }
}

async function apply_default_patch_state_from_clear_patches() {
  if (typeof window.reset_amy_knobs_to_defaults !== "function") {
    return false;
  }
  window.reset_amy_knobs_to_defaults();
  reset_global_effects();
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
  if (typeof window.refresh_knobs_for_active_channel === "function") {
    await window.refresh_knobs_for_active_channel({ sendControlMappings: true });
  } else if (typeof window.refresh_knobs_for_channel === "function") {
    window.refresh_knobs_for_channel();
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
  if (typeof window.get_channel_knobs === "function" && typeof window.get_global_knobs === "function") {
    return window.get_channel_knobs(ch).concat(window.get_global_knobs());
  }
  return window.get_current_knobs ? window.get_current_knobs() : [];
}

function build_knob_cc_value(knob) {
  if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half"
    || knob.knob_type === "pushbutton") {
    return "";
  }
  var log, min_val, max_val, offset;
  if (knob.knob_type === "selection") {
    log = 0;
    min_val = 0;
    max_val = Array.isArray(knob.options) ? knob.options.length : 0;
    offset = 0;
  } else {
    log = knob.knob_type === "log" ? 1 : 0;
    min_val = knob.min_value;
    max_val = knob.max_value;
    offset = (typeof knob.offset === "undefined") ? 0 : knob.offset;
  }
  return knob.cc + "," + log + "," + min_val + "," + max_val + "," + offset + "," + knob.change_code;
}

function send_all_knob_cc_mappings(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) ch = 1;
  var knobs = get_knobs_for_channel(ch);
  for (var i = 0; i < knobs.length; i++) {
    var ccVal = build_knob_cc_value(knobs[i]);
    if (ccVal) {
      amy_send({synth: ch, midi_cc: ccVal}, true);
    }
  }
  if (Array.isArray(window.channel_control_mapping_sent)) {
    window.channel_control_mapping_sent[ch] = true;
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
      // MIDI Thru: forward raw message to output device
      var thruToggle = document.getElementById("midi_thru_toggle");
      if (thruToggle && thruToggle.checked && midiOutputDevice && data.length > 0) {
        try { midiOutputDevice.send(data); } catch (err) {}
      }
      // Feed bytes to AMY audioworklet for sound processing
      for(byte in e.message.data) {
        if(audio_started) amy_process_single_midi_byte(e.message.data[byte], 1);
      }
      // Also forward complete message to MicroPython for Python midi callbacks.
      // AMY's EM_ASM hook can't cross the audioworklet boundary, so we do it here.
      if (audio_started && typeof mp !== 'undefined' && mp.midiInHook) {
        const sysex = (data.length > 0 && data[0] === 0xF0) ? 1 : 0;
        mp.midiInHook(new Uint8Array(data), data.length, sysex);
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
    show_python_error(e.message);
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


function show_python_error(message) {
    var existing = document.getElementById("python_error_modal");
    if (existing) existing.remove();
    var overlay = document.createElement("div");
    overlay.id = "python_error_modal";
    overlay.style.cssText = "position:fixed;inset:0;z-index:10000;background:rgba(0,0,0,0.6);display:flex;align-items:center;justify-content:center;";
    overlay.addEventListener("click", function(e) { if (e.target === overlay) overlay.remove(); });
    var box = document.createElement("div");
    box.style.cssText = "background:#1a1a1a;border:3px solid #ff4444;color:#fff;padding:1.5rem 2rem;max-width:700px;width:90vw;max-height:70vh;overflow:auto;font-family:monospace;font-size:0.85rem;position:relative;box-shadow:0 8px 32px rgba(0,0,0,0.5);";
    var header = document.createElement("div");
    header.style.cssText = "display:flex;justify-content:space-between;align-items:center;margin-bottom:1rem;";
    header.innerHTML = '<span style="color:#ff4444;font-weight:700;font-size:1.1rem;font-family:sans-serif;">Python Error</span>';
    var closeBtn = document.createElement("button");
    closeBtn.textContent = "✕";
    closeBtn.style.cssText = "background:none;border:none;color:#888;font-size:1.3rem;cursor:pointer;padding:0 0.25rem;";
    closeBtn.onclick = function() { overlay.remove(); };
    header.appendChild(closeBtn);
    box.appendChild(header);
    var pre = document.createElement("pre");
    pre.style.cssText = "margin:0;white-space:pre-wrap;word-break:break-word;color:#ffcccc;line-height:1.5;";
    pre.textContent = message;
    box.appendChild(pre);
    overlay.appendChild(box);
    document.body.appendChild(overlay);
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
    try {
        mp.FS.readFile(CURRENT_ENV_DIR + "/sketch.py", { encoding: "utf8" });
    } catch (e) {
        mp.FS.writeFile(CURRENT_ENV_DIR + "/sketch.py", DEFAULT_SKETCH_SOURCE);
    }
    if (seedDefaults) {
        // Reserved for future seed-only setup.
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
    for (const name of mp.FS.readdir(CURRENT_ENV_DIR)) {
        if (name === "." || name === "..") continue;
        var lower = String(name || "").toLowerCase();
        if (lower.endsWith(".dirty")) {
            continue;
        }
        var path = CURRENT_ENV_DIR + "/" + name;
        var mode = mp.FS.lookupPath(path).node.mode;
        if (mp.FS.isFile(mode)) {
            files.push(name);
        }
    }
    files.sort(function(a, b) { return String(a).localeCompare(String(b)); });
    return files;
}

function list_current_patch_files() {
    var files = list_environment_files().filter(function(filename) {
        return String(filename || "").toLowerCase().endsWith(".patch");
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

function should_initialize_default_patch_state(patchExecution) {
    if (!patchExecution || typeof patchExecution !== "object") {
        return false;
    }
    return (!patchExecution.hasPatchMessages) && (!patchExecution.hasActiveChannels);
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
    var targetFilename = selected_environment_file || "sketch.py";
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
    if (files.indexOf("sketch.py") !== -1) {
        target = "sketch.py";
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
        "Clear current code and create a fresh sketch.py?",
        "Clear"
    );
    if (!confirmed) {
        return;
    }
    try {
        clear_current_environment_dir();
        mp.FS.writeFile(CURRENT_ENV_DIR + "/sketch.py", DEFAULT_SKETCH_SOURCE);
        selected_environment_file = null;
        await sync_persistent_fs();
        await fill_tree();
        if (list_environment_files().indexOf("sketch.py") !== -1) {
            await select_environment_file("sketch.py", true);
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
    await runCodeBlock("import amyboard; amyboard.restart_sketch()");
}

async function stop_current_environment() {
    // Stop the sketch loop and reset the sequencer.
    await runCodeBlock("import amyboard, amy; amyboard.stop_sketch(); amy.send(reset=4096)");
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
    // Encode as UTF-8 bytes so multi-byte characters are preserved correctly.
    var encoded = new TextEncoder().encode(text);
    var n = Math.min(length, encoded.length);
    for (var j = 0; j < n; j++) {
        buffer[offset + j] = encoded[j];
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
        params.set("item_type", "environment");
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
        if (list_environment_files().indexOf("sketch.py") !== -1) {
            await select_environment_file("sketch.py", true);
        } else {
            var files = list_environment_files();
            if (files.length) {
                await select_environment_file(files[0], true);
            }
        }
        await restore_patches_from_editor_state_if_present({ sendToAmy: true });
        if (typeof window.refresh_patch_active_name_label === "function") {
            window.refresh_patch_active_name_label();
        }
        if (typeof window.refresh_save_patch_dirty_indicator === "function") {
            window.refresh_save_patch_dirty_indicator();
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



// Load an AMYboard World environment by username and environment name.
// Called from URL params: /editor/?env=woodpiano&user=bwhitman
async function load_world_environment_by_name(username, envName) {
    try {
        // Fetch all environments for this user and filter client-side
        // (the API does not reliably filter by filename)
        var params = new URLSearchParams();
        params.set("username", username);
        params.set("item_type", "environment");
        var response = await fetch(world_api_url("/api/amyboardworld/files?" + params.toString()));
        if (!response.ok) throw new Error("HTTP " + response.status.toString());
        var data = await response.json();
        if (!data || !Array.isArray(data.items) || !data.items.length) {
            show_alert("Environment '" + envName + "' by " + username + " not found.");
            return;
        }
        // Match by filename stem (e.g. "spacey" matches "spacey.tar")
        var item = null;
        for (var i = 0; i < data.items.length; i++) {
            var fn = data.items[i].filename || "";
            var stem = fn.replace(/\.tar$/, "");
            if (fn === envName || stem === envName || fn === envName + ".tar") {
                item = data.items[i];
                break;
            }
        }
        if (!item) {
            show_alert("Environment '" + envName + "' by " + username + " not found.");
            return;
        }
        var filename = normalize_world_filename(item.filename);
        var packageName = get_world_package_name(filename);

        // Show toast
        show_world_toast(item.description || envName, username);

        ensure_current_environment_layout(false);
        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) throw new Error("Missing file URL");
        var dlResponse = await fetch(downloadUrl);
        if (!dlResponse.ok) throw new Error("HTTP " + dlResponse.status.toString());
        var buffer = await dlResponse.arrayBuffer();
        var bytes = new Uint8Array(buffer);
        clear_current_environment_dir();
        extract_tar_buffer_to_fs(bytes, CURRENT_ENV_DIR, packageName + "/");
        if (!list_environment_files().length) {
            extract_tar_buffer_to_fs(bytes, CURRENT_ENV_DIR, "");
        }
        await fill_tree();
        var envNameInput = document.getElementById("editor_filename");
        if (envNameInput) envNameInput.value = packageName;
        if (list_environment_files().indexOf("sketch.py") !== -1) {
            await select_environment_file("sketch.py", true);
        } else {
            var files = list_environment_files();
            if (files.length) await select_environment_file(files[0], true);
        }
        await restore_patches_from_editor_state_if_present({ sendToAmy: true });
        if (typeof window.refresh_patch_active_name_label === "function") {
            window.refresh_patch_active_name_label();
        }
        if (typeof window.refresh_save_patch_dirty_indicator === "function") {
            window.refresh_save_patch_dirty_indicator();
        }
        await run_current_environment();
    } catch (e) {
        show_alert("Failed to load environment '" + envName + "' by " + username + ".");
    }
}

function show_world_toast_loading(envName) {
    var existing = document.getElementById("world_toast");
    if (existing) existing.remove();
    var toast = document.createElement("div");
    toast.id = "world_toast";
    toast.style.cssText = "position:fixed;top:80px;left:50%;transform:translateX(-50%);z-index:9999;background:#FFDD00;color:#000;padding:1rem 2rem;font-weight:700;font-size:1.1rem;border:3px solid #000;box-shadow:4px 4px 0 #000;max-width:90vw;text-align:center;transition:opacity 0.5s;";
    toast.innerHTML = "Loading <strong>" + envName + "</strong>&hellip;";
    document.body.appendChild(toast);
}

function show_world_toast(description, username) {
    var existing = document.getElementById("world_toast");
    if (existing) existing.remove();
    var toast = document.createElement("div");
    toast.id = "world_toast";
    toast.style.cssText = "position:fixed;top:80px;left:50%;transform:translateX(-50%);z-index:9999;background:#FFDD00;color:#000;padding:1rem 2rem;font-weight:700;font-size:1.1rem;border:3px solid #000;box-shadow:4px 4px 0 #000;max-width:90vw;text-align:center;transition:opacity 0.5s;";
    toast.innerHTML = "Loaded <strong>" + description + "</strong> by " + username;
    document.body.appendChild(toast);
    setTimeout(function() {
        toast.style.opacity = "0";
        setTimeout(function() { toast.remove(); }, 500);
    }, 4000);
}

// Check URL params on startup for environment loading
function check_url_env_params() {
    var params = new URLSearchParams(window.location.search);
    var env = params.get("env");
    var user = params.get("user");
    var tab = params.get("tab"); // "code" or "patch"
    if (env && user) {
        // Show immediate loading toast
        show_world_toast_loading(env);
        // Delay slightly to let micropython init complete
        setTimeout(async function() {
            await load_world_environment_by_name(user, env);
            // Switch to requested tab after environment loads
            if (tab === "code") {
                var codeTab = document.getElementById("environment-tab");
                if (codeTab) {
                    new bootstrap.Tab(codeTab).show();
                    // Re-select sketch.py and refresh CodeMirror after tab is visible
                    setTimeout(async function() {
                        if (list_environment_files().indexOf("sketch.py") !== -1) {
                            await select_environment_file("sketch.py", true);
                        }
                        if (editor) {
                            editor.refresh();
                        }
                    }, 300);
                }
            } else if (tab === "patch") {
                var patchTab = document.getElementById("patch-tab");
                if (patchTab) new bootstrap.Tab(patchTab).show();
            }
        }, 1500);
        // Clean up URL without reload
        window.history.replaceState({}, document.title, window.location.pathname);
    }
}

// Prompt for admin token via modal. Resolves with token string or null if cancelled.
function prompt_admin_token() {
    return new Promise(function(resolve) {
        var modalEl = document.getElementById("adminTokenModal");
        var input = document.getElementById("admin-token-input");
        var errorDiv = document.getElementById("admin_token_error");
        var submitBtn = document.getElementById("admin-token-submit-btn");
        if (!modalEl || !input || !submitBtn) { resolve(null); return; }
        input.value = "";
        errorDiv.style.display = "none";
        errorDiv.textContent = "";
        var modal = window.bootstrap.Modal.getOrCreateInstance(modalEl);
        var resolved = false;
        function cleanup() {
            freshBtn.removeEventListener("click", onSubmit);
            modalEl.removeEventListener("hidden.bs.modal", onDismiss);
        }
        function onSubmit() {
            var token = (input.value || "").trim();
            if (!token) {
                errorDiv.textContent = "Token cannot be empty.";
                errorDiv.style.display = "block";
                return;
            }
            resolved = true;
            cleanup();
            modal.hide();
            resolve(token);
        }
        function onDismiss() {
            cleanup();
            if (!resolved) resolve(null);
        }
        // Replace button to clear old listeners
        var freshBtn = submitBtn.cloneNode(true);
        submitBtn.parentNode.replaceChild(freshBtn, submitBtn);
        freshBtn.addEventListener("click", onSubmit);
        modalEl.addEventListener("hidden.bs.modal", onDismiss, { once: true });
        modal.show();
    });
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

    var adminToken = null;
    if (username.toLowerCase() === "shorepine") {
        adminToken = await prompt_admin_token();
        if (adminToken === null) return;
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
    var fetchOpts = { method: "POST", body: data };
    if (adminToken) fetchOpts.headers = { "X-Admin-Token": adminToken };
    try {
        var response = await fetch(world_api_url("/api/amyboardworld/upload"), fetchOpts);
        if (!response.ok) {
            var detail = "";
            try { detail = (await response.json()).detail || ""; } catch (_) {}
            if (response.status === 403) {
                show_alert(detail || "Wrong token.");
            } else {
                show_alert(detail || "Upload failed.");
            }
            return;
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
    // Pace sends to avoid overrunning the hardware USB-MIDI RX buffer.
    await sleep_ms(50);
}

async function open_send_to_amyboard_modal() {
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
        await setup_midi_devices();
        if (!midiOutputDevice) {
            throw new Error("No MIDI out port selected.");
        }

        var tarFilename = "environment.tar";
        var tarBytes = build_environment_tar_bytes();
        var fileSize = tarBytes.length;

        send_amyboard_progress_update(0, fileSize);
        await sysex_write_amy_message("zT" + tarFilename + "," + fileSize + "Z");
        // Wait for hardware to process zT and stop the sequencer/loop(),
        // even if loop() is blocking the scheduler for a while.
        await sleep_ms(1000);

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

 function get_output_audio_samples() {
     var am = globalThis._amy_module_ref;
     var ptr = globalThis._amy_res_ptr_out;
     if (!am || !am._amy_get_output_buffer || !ptr) {
         return new Uint8Array(1024);
     }
     var n = am._amy_get_output_buffer(ptr);
     var result = new Uint8Array(1024);
     if (n > 0) {
         result.set(am.HEAPU8.subarray(ptr, ptr + 1024));
     }
     return result;
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
  await mp.registerJsModule('amy_get_output_buffer', get_output_audio_samples);
//  await mp.registerJsModule('amy_get_input_buffer', get_audio_samples);
//  await mp.registerJsModule('amy_set_external_input_buffer', set_audio_samples);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import tulip, amy, amy_js_message, amy_sysclock, amy_get_output_buffer as _amy_get_output_buf_js
    amy.override_send = amy_js_message
    tulip.amy_ticks_ms = amy_sysclock
    tulip.amy_get_output_buffer = _amy_get_output_buf_js
  `);

  // If you don't have these sleeps we get a MemoryError with a locked heap. Not sure why yet.
  await sleep_ms(400);
  await mp.runFrozenAsync('_boot.py');
  ensure_current_environment_layout(true);
  // Skip restoring saved environment if ?env= param is present — the URL
  // environment will be loaded by check_url_env_params() after startup.
  var urlEnvPending = !!(new URLSearchParams(window.location.search).get("env"));
  if (!urlEnvPending) {
    try {
      await mp.runPythonAsync("import amyboard; amyboard.restore_patch_state_from_files(send_default_if_missing=True)");
    } catch (e) {
      // Fallback for environments where the shared helper is unavailable.
      await restore_patches_from_editor_state_if_present({ sendToAmy: true });
    }
    await restore_patches_from_editor_state_if_present({ sendToAmy: false });
    try {
      await mp.runPythonAsync("import amyboard; amyboard.run_sketch()");
    } catch (e) {
      // sketch.py may not exist or may fail — that's OK.
    }
  }
  await fill_tree();
  if (typeof window.refresh_patch_active_name_label === "function") {
    window.refresh_patch_active_name_label();
  }
  if (typeof window.refresh_save_patch_dirty_indicator === "function") {
    window.refresh_save_patch_dirty_indicator();
  }
  setTimeout(async function() {
    try {
      await restore_patches_from_editor_state_if_present({ sendToAmy: false });
      if (typeof window.refresh_patch_active_name_label === "function") {
        window.refresh_patch_active_name_label();
      }
      if (typeof window.refresh_save_patch_dirty_indicator === "function") {
        window.refresh_save_patch_dirty_indicator();
      }
    } catch (e) {}
  }, 800);
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
  // Allow a few audio callbacks to fire so amy_execute_deltas processes
  // any wire commands that were queued before audio started (e.g. from
  // restore_patch_state_from_files during boot).  Without this delay,
  // yield_synth_commands would read stale default values for global effects.
  await new Promise(function(resolve) { setTimeout(resolve, 150); });
  try {
    await restore_patches_from_editor_state_if_present({ sendToAmy: false });
    if (typeof window.refresh_patch_active_name_label === "function") {
      window.refresh_patch_active_name_label();
    }
    if (typeof window.refresh_save_patch_dirty_indicator === "function") {
      window.refresh_save_patch_dirty_indicator();
    }
  } catch (e) {}
  // Set up AnalyserNode to capture output waveform for Python
  try {
    if (typeof emAudio !== 'undefined') {
      var actx = null, workletNode = null;
      for (var k in emAudio) {
        var obj = emAudio[k];
        if (obj && obj.constructor && obj.constructor.name === 'AudioContext') actx = obj;
        if (obj && obj.constructor && obj.constructor.name === 'AudioWorkletNode') workletNode = obj;
      }
      console.log("emAudio scan: AudioContext=", !!actx, "WorkletNode=", !!workletNode);
      if (actx && workletNode) {
        var analyser = actx.createAnalyser();
        analyser.fftSize = 512;
        workletNode.connect(analyser);
        // Don't disconnect workletNode from destination - just add a parallel tap
        globalThis._amy_analyser = analyser;
        console.log("AnalyserNode connected for waveform capture");
      }
    }
  } catch(e) { console.warn("AnalyserNode setup failed:", e); }

  // Keep startup simple: knobs are local UI state until user moves them.
  audio_started = true;
}

// ---- wire_code_parser.js ----
function parse_wire_code(message) {

  const Lex = {
    END: 0,
    ALPHA: 1,
    NUMBER: 2,
    SEP: 3,
  };

  const Expecting = {
    END: 0,
    COMMAND: 1,
    NUMARG: 2,
    LISTSEP: 3,
    LISTARG: 4,
    STRARG: 5,
  };

  function numeric_prefix(s) {
    let len = s.length;
    let i = 0;
    while (i < len && ((s[i] >= "0" && s[i] <= "9") || s[i] == "." || s[i] == "-")) {
      ++i;
    }
    return s.slice(0, i);
  }

  function alpha_prefix(s) {
    let len = s.length;
    let i = 0;
    while (i < len && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))) {
      ++i;
    }
    return s.slice(0, i);
  }

  function next_token(message) {
    // Segment the next token - sequence of chars, numeric value, or comma.
    // returns [LexCode, token, remaining_string]
    let len = message.length;
    let index = 0;
    let result = "";
    var p;
    while ((message[index] == " " || message[index] == "\n") && index < len) {
      index++;
    }
    if (index >= len) return [Lex.END, "Z", ""];  // Virtual Z.
    if (message[index] == ",") return [Lex.SEP, ",", message.slice(index + 1)];
    // 'u' is a special case, meaning rest of input is a string
    if (message[index] == "u") return [Lex.ALPHA, "u", message.slice(index + 1)];
    if (message[index] == "Z") return [Lex.ALPHA, "Z", message.slice(index + 1)];
    p = numeric_prefix(message.slice(index));
    if (p.length > 0) return [Lex.NUMBER, p, message.slice(index + p.length)];
    p = alpha_prefix(message.slice(index));
    if (p.length > 0) return [Lex.ALPHA, p, message.slice(index + p.length)];
    throw new Error("Unable to find token in \"" + message + "\"");
  }

  function debug_message(message, token, rest) {
    // The parser had a problem working through message at tok, when the unprocessed part is rest.
    return message.slice(0, message.length - token.length - rest.length) + " ** " + token + " ** " + rest;
  }

  // Build reverse lookup (wire code → [fieldname, type]) from AMY_KW_MAP (source of truth from __init__.py).
  let codes = new Map();
  for (const [fieldname, mapping] of Object.entries(AMY_KW_MAP)) {
    codes.set(mapping.wire, [fieldname, mapping.type]);
  }
  codes.set("Z", ["end", "Z"]);
  var code, token;
  let fieldname = "?";
  let pending_list = [];
  let type = "?";
  let result = {};
  let expecting = Expecting.COMMAND;
  let rest = message;
  let pushback = null;
  let value = null;
  while (expecting != Expecting.END) {
    if (pushback != null) {
      [lexcode, token, rest] = pushback;
      pushback = null;
    } else {
      [lexcode, token, rest] = next_token(rest);0
    }
    switch (expecting) {
    case Expecting.COMMAND:
      if (lexcode == Lex.END) {
        expecting = Expecting.END;
        break;
      }
      if (!(lexcode == Lex.ALPHA && codes.has(token))) {
        throw new Error("Unrecognized wire code: " + debug_message(message, token, rest));
      }
      [fieldname, type] = codes.get(token);
      switch (type) {
      case "I":
      case "F":
        expecting = Expecting.NUMARG;
        break;
      case "L":
      case "C":
        expecting = Expecting.LISTARG;
        type = "F";  // without more annotation....
        break;
      case "S":
        expecting = Expecting.STRARG;
        result[fieldname] = rest;
        rest = "";
        expecting = Expecting.END;
        break;
      case "Z":
        expecting = Expecting.END;
        break;
      default:
        throw new Error("Unexpected code type " + type + " in " + debug_message(message, token, rest));
      }
      break;
    case Expecting.LISTARG:
      if (lexcode == Lex.SEP) {
        pending_list.push(null);
        break;
      }
      else if (lexcode == Lex.ALPHA) {
        // list is over, the ARG we expected was empty.
        pending_list.push(null);
        // handle end of list.
        result[fieldname] = pending_list;
        pending_list = [];
        expecting = Expecting.COMMAND;
        pushback = [lexcode, token, rest];
        break;
      }
      // else fall through
    case Expecting.NUMARG:
      if (!(lexcode == Lex.NUMBER)) {throw new Error("Expected number in: "  + debug_message(message, token, rest)); }
      if (type == "I")
        value = parseInt(token, 10);
      else if (type == "F")
        value = parseFloat(token);
      else throw new Error("Unrecognized type " + type);
      if (expecting == Expecting.NUMARG) {
        result[fieldname] = value;
        expecting = Expecting.COMMAND;
      } else {
        // LISTARG
        pending_list.push(value);
        expecting = Expecting.LISTSEP;
      }
      break;
    case Expecting.LISTSEP:
      if (lexcode == Lex.SEP) {
        // Hacky special case for string at end of 'ic'
        if (!(fieldname == "midi_cc" && pending_list.length == 5)) {
          expecting = Expecting.LISTARG;
          break;
        } else {
          pending_list.push(rest);
          rest = "";
          token = "Z";
          lexcode = Lex.END;
          // but fall through to list termination.
        }
      }
      result[fieldname] = pending_list;
      pending_list = [];
      expecting = Expecting.COMMAND;
      pushback = [lexcode, token, rest];
      break;
    default:
      throw new Error("Unknown expecting " + expecting);
    }
  }
  // Special-case handling of bp0, bp1
  for (const [field, timefield, valfield] of [["bp0", "eg0_times", "eg0_values"], ["bp1", "eg1_times", "eg1_values"]]) {
    if (field in result) {
      result[timefield] = result[field].filter((element, index) => {return index % 2 === 0;});
      result[valfield] = result[field].filter((element, index) => {return index % 2 === 1;});
      Reflect.deleteProperty(result, field);
    }
  }
  return [result, rest];
}

function events_from_wire_code_message(message) {
  // Message is a single wire-code string, but may contain multiple events with Z separators.
  events = [];
  let rest = message;
  while (rest != "") {
    [event, rest] = parse_wire_code(rest);
    events.push(event);
  }
  return events;
}

function events_from_wire_code_messages(messages) {
  // Messages is an array of wire code message strings; all the events contained are concatenated.
  var all_events = [];
  for (const message of messages) {
    let message_events = events_from_wire_code_message(message);
    for (const event of message_events) {
      all_events.push(event);
    }
  }
  return all_events;
}

// Test
if (false) {
  for (const message of [
    "v0w1c1L4G4ic10,13.3,,7",
    "v0w1c1L4G4ic10,13.3,,7Z",
    "v0w1c1L4G4d,,,,R0.913",
    "v0w1c1L4G4ic10,13.3,,7urest,of,itZ",
    "i1ic5,0,0,1,0",
    "i1ic5,0,0,1,0R123",
    "i1ic5,0,0,1,0,i%1d,,%v,0",
    "i1ic5,0,0,1,0R123Zi1ic5,0,0,1,0R123",
    "v0A0,1,100,0.5,1000,0Z",
    "v0A0,,,0.5,,Z",
    "v4w4a1,,0,1Zv0w1c1L4G4Zv1w3c2L4Zv2w1c3L4Zv3w5L4Zv4f0.609A148,1.0,10000,0Zv0a,,0.001,1,0f261.63,1,,,,0,1d0.72,,,,,0m0Zv1a,,0.591,1,0f261.63,1,,,,0,1m0Zv2a,,0.326,1,0f130.81,1,,,,0,1m0Zv3a,,0.001,1,0Zv0F212.87,0.661,,2.252,,0R1.015Zv0A518,1,83561,0.299,310,0Zv1A518,1,83561,0.299,310,0Zv2A518,1,83561,0.299,310,0Zv3A518,1,83561,0.299,310,0Zx7,-3,-3k1,,0.5,0.5Z",
  ]) {
    console.log(message, events_from_wire_code_message(message));
  };
}


// ──────────────────────────────────────────────────────────────────────────
// AMYboard World — Patches sub-tab
// ──────────────────────────────────────────────────────────────────────────

function get_world_patch_search_query() {
    var el = document.getElementById("amyboard_world_patch_search");
    return el ? el.value.trim() : "";
}

function get_world_patch_tag_query() {
    return amyboard_world_patch_selected_tag || "";
}

function get_world_patch_display_name(filename) {
    var name = normalize_world_filename(filename);
    return name.replace(/\.patch$/i, "");
}

function randomize_world_patch_tag_palette() {
    var tags = ["featured", "official", "popular", "wild"];
    var colors = ["bg-primary", "bg-success", "bg-danger", "bg-info", "bg-warning", "bg-secondary"];
    for (var i = colors.length - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var tmp = colors[i]; colors[i] = colors[j]; colors[j] = tmp;
    }
    amyboard_world_patch_tag_palette = {};
    for (var t = 0; t < tags.length; t++) {
        amyboard_world_patch_tag_palette[tags[t]] = colors[t % colors.length];
    }
}

function render_world_patch_tag_pills() {
    var container = document.getElementById("amyboard_world_patch_tag_pills");
    if (!container) return;
    if (!Object.keys(amyboard_world_patch_tag_palette).length) {
        randomize_world_patch_tag_palette();
    }
    var tags = ["featured", "official", "popular", "wild"];
    var html = "";
    for (var i = 0; i < tags.length; i++) {
        var tag = tags[i];
        var color = amyboard_world_patch_tag_palette[tag] || "bg-secondary";
        var active = (amyboard_world_patch_selected_tag === tag);
        html += '<button type="button" class="badge border-0 ' + color + (active ? "" : " opacity-50") + '"' +
            ' onclick="select_amyboard_world_patch_tag(\'' + tag + '\')">#' + tag + '</button>';
    }
    container.innerHTML = html;
}

function select_amyboard_world_patch_tag(tag) {
    tag = String(tag || "").toLowerCase();
    amyboard_world_patch_selected_tag = (amyboard_world_patch_selected_tag === tag) ? "" : tag;
    render_world_patch_tag_pills();
    refresh_amyboard_world_patches();
}

function render_amyboard_world_patch_list() {
    var list = document.getElementById("amyboard_world_patch_list");
    if (!list) return;

    if (!amyboard_world_patches.length) {
        list.innerHTML = '<div class="border rounded p-2 text-muted">No patches found.</div>';
        return;
    }

    var html = '<div class="row row-cols-2 g-2">';
    for (var i = 0; i < amyboard_world_patches.length; i++) {
        var item = amyboard_world_patches[i];
        var displayName = get_world_patch_display_name(item.filename);
        var description = (typeof item.content === "string") ? item.content.trim() : "";
        if (!description && typeof item.description === "string") {
            description = item.description.trim();
        }
        var when = format_world_file_timestamp(item.time);
        var meta = (item.username || "unknown") + (when ? " \u2022 " + when : "");
        var tags = Array.isArray(item.tags) ? item.tags : [];
        var isLoading = (amyboard_world_patch_loading_index === i);
        var isPreviewing = (amyboard_world_patch_previewing_index === i);

        html += '<div class="col">';
        html += '<div class="w-100 border rounded p-2 bg-white text-start">';
        html += '<div class="fw-bold">' + escape_html(displayName);
        if (description.length > 0) {
            html += ' <span class="fw-normal text-muted">- ' + escape_html(description) + '</span>';
        }
        html += '</div>';
        html += '<div class="small text-muted">' + escape_html(meta) + '</div>';
        if (tags.length > 0) {
            html += '<div class="mt-1">';
            for (var t = 0; t < tags.length; t++) {
                html += '<span class="badge bg-light text-secondary border me-1">#' + escape_html(String(tags[t])) + '</span>';
            }
            html += '</div>';
        }
        html += '<div class="mt-1 d-flex gap-1">';
        // Download button
        if (isLoading) {
            html += '<button type="button" class="btn btn-sm btn-outline-primary" disabled>' +
                '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span>Downloading...</button>';
        } else {
            html += '<button type="button" class="btn btn-sm btn-outline-primary" onclick="download_world_patch(' + i + ')">Download</button>';
        }
        // Preview button
        if (isPreviewing) {
            html += '<button type="button" class="btn btn-sm btn-outline-secondary" disabled>' +
                '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span></button>';
        } else {
            html += '<button type="button" class="btn btn-sm btn-outline-secondary" onclick="preview_world_patch(' + i + ')" title="Preview">' +
                '\u25B6</button>';
        }
        html += '</div>';
        html += '</div></div>';
    }
    html += '</div>';
    list.innerHTML = html;
}

async function refresh_amyboard_world_patches() {
    render_world_patch_tag_pills();
    var list = document.getElementById("amyboard_world_patch_list");
    if (list) {
        list.innerHTML = '<div class="border rounded p-2 text-muted">Loading...</div>';
    }

    try {
        var params = new URLSearchParams();
        params.set("limit", "100");
        params.set("latest_per_user_env", "true");
        params.set("item_type", "patch");
        var q = get_world_patch_search_query();
        var tag = get_world_patch_tag_query();
        if (q.length) {
            params.set("q", q);
        }
        if (tag.length) {
            params.set("tag", tag);
        }
        var response = await fetch(world_api_url("/api/amyboardworld/files?" + params.toString()));
        if (!response.ok) {
            throw new Error("HTTP " + response.status.toString());
        }
        var data = await response.json();
        if (!data || !Array.isArray(data.items)) {
            throw new Error("Unexpected response");
        }
        amyboard_world_patches = data.items
            .sort(function(a, b) {
                return Number(b.time || 0) - Number(a.time || 0);
            })
            .slice(0, 20);
        render_amyboard_world_patch_list();
    } catch (e) {
        amyboard_world_patches = [];
        if (list) {
            list.innerHTML = '<div class="border rounded p-2 text-danger">Could not load patches.</div>';
        }
    }
    // Always refresh the upload dropdown so the user sees their saved patches
    refresh_patch_upload_dropdown();
}

async function download_world_patch(index) {
    if (amyboard_world_patch_loading_index !== null) return;
    if (index < 0 || index >= amyboard_world_patches.length) {
        show_alert("That patch is no longer available.");
        return;
    }

    var item = amyboard_world_patches[index];
    amyboard_world_patch_loading_index = index;
    render_amyboard_world_patch_list();

    try {
        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) throw new Error("Missing file URL");
        var response = await fetch(downloadUrl);
        if (!response.ok) throw new Error("HTTP " + response.status.toString());
        var text = await response.text();

        var filename = normalize_world_filename(item.filename);
        if (!filename.toLowerCase().endsWith(".patch")) {
            filename = filename + ".patch";
        }
        ensure_current_environment_layout(false);
        mp.FS.writeFile(CURRENT_ENV_DIR + "/" + filename, text);
        await sync_persistent_fs();

        // Refresh the Load Patch dropdown so the new patch appears immediately.
        if (typeof window.refreshYourPatchesDropdown === "function") {
            window.refreshYourPatchesDropdown();
        }
        show_alert("Patch '" + filename.replace(/\.patch$/i, "") + "' saved. It's now available in Load Patch.");
    } catch (e) {
        show_alert("Failed to download patch.");
    } finally {
        amyboard_world_patch_loading_index = null;
        render_amyboard_world_patch_list();
    }
}

async function preview_world_patch(index) {
    if (amyboard_world_patch_previewing_index !== null) return;
    if (index < 0 || index >= amyboard_world_patches.length) return;

    amyboard_world_patch_previewing_index = index;
    render_amyboard_world_patch_list();

    try {
        var item = amyboard_world_patches[index];
        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) throw new Error("Missing file URL");
        var response = await fetch(downloadUrl);
        if (!response.ok) throw new Error("HTTP " + response.status.toString());
        var text = await response.text();

        // Init synth 32 with 6 voices, 3-note polyphony
        amy_send({synth: 32, num_voices: 6, oscs_per_voice: 3}, true);

        // Load each patch line prepended with i32
        var lines = String(text || "").split(/\r?\n/);
        for (var i = 0; i < lines.length; i++) {
            var wire = lines[i].trim();
            if (!wire || wire.startsWith("#")) continue;
            amy_add_log_message("i32" + wire);
        }

        // Brief pause to let the patch settle before playing
        await new Promise(function(r) { setTimeout(r, 250); });

        // Play 3 notes with 300ms spacing
        var notes = [58, 60, 62];
        for (var n = 0; n < notes.length; n++) {
            amy_send({synth: 32, vel: 1, note: notes[n]}, true);
            await new Promise(function(r) { setTimeout(r, 300); });
        }

        // Let notes ring briefly, then clear the synth
        await new Promise(function(r) { setTimeout(r, 800); });
        amy_send({synth: 32, num_voices: 6, oscs_per_voice: 3}, true);
    } catch (e) {
        show_alert("Preview failed.");
    } finally {
        amyboard_world_patch_previewing_index = null;
        render_amyboard_world_patch_list();
    }
}

function refresh_patch_upload_dropdown() {
    var select = document.getElementById("patch_upload_select");
    if (!select) return;
    var files = [];
    if (typeof list_current_patch_files === "function") {
        files = list_current_patch_files();
    }
    select.innerHTML = "";
    if (!files.length) {
        var opt = document.createElement("option");
        opt.value = "";
        opt.textContent = "No saved patches";
        select.appendChild(opt);
        select.disabled = true;
        return;
    }
    select.disabled = false;
    files.forEach(function(filename) {
        var opt = document.createElement("option");
        opt.value = filename;
        opt.textContent = String(filename).replace(/\.patch$/i, "");
        select.appendChild(opt);
    });
}

async function upload_patch_to_world() {
    var usernameInput = document.getElementById("patch_upload_username");
    var patchSelect = document.getElementById("patch_upload_select");
    var descriptionInput = document.getElementById("patch_upload_description");
    if (!usernameInput || !patchSelect || !descriptionInput) return;

    var username = (usernameInput.value || "").trim();
    var patchFilename = (patchSelect.value || "").trim();
    var description = sanitize_environment_description(descriptionInput.value || "");
    descriptionInput.value = description;

    if (!/^[A-Za-z0-9]{1,20}$/.test(username)) {
        show_alert("Username must be 1-20 chars: only A-Z, a-z, 0-9 (no spaces).");
        return;
    }
    if (!patchFilename) {
        show_alert("Select a patch to upload.");
        return;
    }
    if (description.length < 1 || description.length > 400) {
        show_alert("Description must be 1-400 characters.");
        return;
    }

    var adminToken = null;
    if (username.toLowerCase() === "shorepine") {
        adminToken = await prompt_admin_token();
        if (adminToken === null) return;
    }

    var uploadButton = document.getElementById("upload_patch_btn");
    if (uploadButton && uploadButton.disabled) return;
    if (uploadButton) {
        uploadButton.disabled = true;
        uploadButton.innerHTML = '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span>Uploading...';
    }

    try {
        ensure_current_environment_layout(false);
        var contents = mp.FS.readFile(CURRENT_ENV_DIR + "/" + patchFilename, { encoding: "utf8" });
        var file = new File([contents], patchFilename, { type: "text/plain" });
        var data = new FormData();
        data.append("file", file);
        data.append("username", username);
        data.append("description", description);
        var fetchOpts = { method: "POST", body: data };
        if (adminToken) fetchOpts.headers = { "X-Admin-Token": adminToken };

        var response = await fetch(world_api_url("/api/amyboardworld/upload"), fetchOpts);
        if (!response.ok) {
            var detail = "";
            try { detail = (await response.json()).detail || ""; } catch (_) {}
            if (response.status === 403) {
                show_alert(detail || "Wrong token.");
            } else {
                show_alert(detail || "Upload failed.");
            }
            return;
        }
        await refresh_amyboard_world_patches();
    } catch (e) {
        show_alert("Upload failed.");
    } finally {
        if (uploadButton) {
            uploadButton.disabled = false;
            uploadButton.textContent = "Upload";
        }
    }
}
