
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
var selected_environment_file = null;
var pending_environment_editor_load = false;
var environment_editor_dirty = false;
var environment_autosave_inflight = false;
const AMYBOARD_WORLD_API_BASE = (typeof window !== "undefined" && typeof window.AMYBOARD_WORLD_API_BASE === "string")
    ? String(window.AMYBOARD_WORLD_API_BASE).replace(/\/+$/, "")
    : String(window.location.origin || "").replace(/\/+$/, "");
const CURRENT_BASE_DIR = "/amyboard/user/current";
const CURRENT_ENV_DIR = CURRENT_BASE_DIR;
const DEFAULT_ENV_SOURCE = "# Put your own code here to run in your environment\n";
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
  amy_bleep(0); // won't play until live audio starts
  amy_module = am;
  res_ptr_in = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  res_ptr_out = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
});

// Converts AMY patch number to list of (JS converted) amy_events
// This is to let us fill in the preset knobs
function get_events_for_patch_number(patch_number) {
  if (!amy_module || !amy_yield_patch_events) {
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

function get_wire_commands_for_juno_patch(patch) {

  function translate_juno_events_to_webeditor_wire_commands(events) {
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
    let osc_duty = [0.5, 0.5, 0.5, 0.5];
    let osc_gain = [0, 0, 0, 0];
    let mod_source_osc = 4;  // Start with Juno LFO osc, but may get updated if it's an amyboardsynth patch.
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
        if (Number.isFinite(event.chorus[0])) { chorus[0] = event.chorus[0]; }
        if (Number.isFinite(event.chorus[1])) { chorus[1] = event.chorus[1]; }
        if (Number.isFinite(event.chorus[2])) { chorus[2] = event.chorus[2]; }
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
        if (Number.isFinite(event.filter_freq[4])) {
          filterEnv = event.filter_freq[4];  // COEF_EG1, amyboard filter env
          filter_eg = 1;
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
        if (event.Freq && Number.isFinite(event.freq[0])) {
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
    // If we didn't set up a separate filter ADSR, it follows the VCA
    if (filter_eg == 0) {
      f_adsr = [adsr[0], adsr[1], adsr[2], adsr[3]];
    }
    if (oscGate) {
      adsr = [0, 0, 1, 0];
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

    // Format the final wire commands for the 3-osc config.
    wire_commands = [];
    const PCM = 7;  // from amy/src/amy.h
    const WAVETABLE = 19;  // from amy/src/amy.h
    for (const osc of [0, 1]) {
      let src_osc = oscAB_osc[osc];
      let command = "v" + osc + "w" + osc_wave[src_osc] + "L2" + "m0"
          + "f" + osc_freq[src_osc] + ",1,,,," + lfoOsc + ",1"
          + "d" + osc_duty[src_osc] + ",,,,," + lfoPwm
          + "a,," + osc_gain[src_osc] + ",1,0"
          + "A" + adsr[0] + ",1," + adsr[1] + "," + adsr[2] + "," + adsr[3] + ",0";
      if (osc_wave[src_osc] == WAVETABLE || osc_wave[src_osc] == PCM) {
        command += "p" + osc_preset[src_osc];
      }
      if (osc == 0) {
        // Osc 0 must chain to osc 1
        command += "c1";
        // Osc 0 has the filter controls.
        command += "G4R" + resonanceValue
            + "F" + filterFreq + "," + filterKbd + ",,," + filterEnv + "," + filterLfo
            + "B" + f_adsr[0] + ",1," + f_adsr[1] + "," + f_adsr[2] + "," + f_adsr[3] + ",0";
      }
      wire_commands.push(command + "Z");
    }
    // LFO command
    let command = "v2w" + lfoWave + "a1,0,0,1" + "f" + lfoFreq + ",0,,0,0,0,0"
        + "A" + lfoDelay + ",1,100,1,10000,0";
    wire_commands.push(command + "Z");
    // Global FX commands
    command = "x" + eq[0] + "," + eq[1] + "," + eq[2]
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

  amy_add_log_message("i" + synth + "K257iv6");
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
  if (!hasEditorState) {
    if (sendToAmy) {
      amy_add_log_message("i1K257iv6");
      send_all_knob_cc_mappings(1);
    }
    return { hasEditorState: false, loadedCount: 0 };
  }

  const state = read_editor_state_json();
  const synthMap = (state && state.synths && typeof state.synths === "object") ? state.synths : {};
  const loadedMap = new Array(17).fill(false);
  const loadedSynths = [];
  let loadedCount = 0;
  for (let ch = 1; ch <= 16; ch += 1) {
    set_channel_patch_dirty_state(ch, false);
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
    if (!name) {
      continue;
    }
    window.channel_patch_names[synth] = name.replace(/\.patch$/i, "");
    const dirtyFilename = String(synth) + ".dirty";
    let dirtySource = "";
    try {
      dirtySource = mp.FS.readFile(CURRENT_ENV_DIR + "/" + dirtyFilename, { encoding: "utf8" });
    } catch (e) {
      dirtySource = "";
    }
    let filename = name.toLowerCase().endsWith(".patch") ? name : (name + ".patch");
    let source = "";
    if (dirtySource) {
      source = dirtySource;
      set_channel_patch_dirty_state(synth, true);
    } else {
      try {
        source = mp.FS.readFile(CURRENT_ENV_DIR + "/" + filename, { encoding: "utf8" });
      } catch (e) {
        continue;
      }
      set_channel_patch_dirty_state(synth, false);
    }
    if (sendToAmy) {
      amy_add_log_message("i" + synth + "K257iv6");
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
    amy_add_log_message("i" + activeCh + "K257iv6");
    send_all_knob_cc_mappings(activeCh);
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
  amy_add_log_message("i" + synth + "K257iv6");
  send_all_knob_cc_mappings(synth);
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
  var m = build_knob_cc_message(window.current_synth, knob);
  if (!m) {
    return;
  }
  if (window.suppress_knob_cc_send) {
    return;
  }
  // If the CC number changed, remove the old CC mapping from AMY first.
  if (previousCc !== undefined && previousCc !== "" && previousCc !== knob.cc) {
    var synthChannel = Number(window.current_synth || 1);
    if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
      synthChannel = 1;
    }
    amy_add_log_message("i" + synthChannel + "ic" + previousCc + ",0,0,0,0,");
  }
  if (typeof amy_add_message === "function") {
    amy_add_log_message(m);
  }
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
  const channel = Number(window.current_synth || 1);
  const patch = Number(patchNumber);
  if (!Number.isInteger(patch) || patch < 0) {
    return;
  }
  amy_add_log_message("i" + channel + "iv6K" + patch);
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

function build_knob_cc_message(channel, knob) {
  if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half"
    || knob.knob_type === "pushbutton") {
    return "";
  }
  var synthChannel = Number(channel);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = Number(window.current_synth || 1);
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
  return "i" + synthChannel + "ic" + knob.cc + "," + log + "," + min_val + "," + max_val + "," + offset + "," + knob.change_code;
}

function send_all_knob_cc_mappings(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) ch = 1;
  var knobs = get_knobs_for_channel(ch);
  for (var i = 0; i < knobs.length; i++) {
    var m = build_knob_cc_message(ch, knobs[i]);
    if (m) {
      amy_add_log_message(m);
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
    try {
        mp.FS.readFile(CURRENT_ENV_DIR + "/env.py", { encoding: "utf8" });
    } catch (e) {
        mp.FS.writeFile(CURRENT_ENV_DIR + "/env.py", DEFAULT_ENV_SOURCE);
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
    files.sort();
    return files;
}

function list_current_patch_files() {
    var files = list_environment_files().filter(function(filename) {
        return String(filename || "").toLowerCase().endsWith(".patch");
    });
    files.sort();
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
    ensure_current_environment_layout(true);
    var patchFiles = list_current_patch_files();
    var channelPatchMap = new Array(17).fill(null);
    var combinedPatchSources = [];
    var hasPatchMessages = false;
    for (var i = 0; i < patchFiles.length; i++) {
        var filename = patchFiles[i];
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
            hasPatchMessages = true;
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
        hasPatchMessages: hasPatchMessages,
    };
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
        "Clear current code and create a fresh env.py?",
        "Clear"
    );
    if (!confirmed) {
        return;
    }
    try {
        clear_current_environment_dir();
        mp.FS.writeFile(CURRENT_ENV_DIR + "/env.py", DEFAULT_ENV_SOURCE);
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
        "_amyboard_env_dir = '/amyboard/user/current'\n" +
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
  ensure_current_environment_layout(true);
  try {
    await mp.runPythonAsync("import amyboard; amyboard.restore_patch_state_from_files(send_default_if_missing=True)");
  } catch (e) {
    // Fallback for environments where the shared helper is unavailable.
    await restore_patches_from_editor_state_if_present({ sendToAmy: true });
  }
  await restore_patches_from_editor_state_if_present({ sendToAmy: false });
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
  try {
    await restore_patches_from_editor_state_if_present({ sendToAmy: false });
    if (typeof window.refresh_patch_active_name_label === "function") {
      window.refresh_patch_active_name_label();
    }
    if (typeof window.refresh_save_patch_dirty_indicator === "function") {
      window.refresh_save_patch_dirty_indicator();
    }
  } catch (e) {}
  // Keep startup simple: knobs are local UI state until user moves them.
  audio_started = true;
}

// ---- wire_code_parser.js ----
function parse_wire_code(message) {

  // Local variables.
  let _KW_MAP_LIST = [   // Order matters because patch_string must come last.
      ['osc', 'vI'], ['wave', 'wI'], ['note', 'nF'], ['vel', 'lF'], ['amp', 'aC'], ['freq', 'fC'], ['duty', 'dC'], 
      ['feedback', 'bF'], ['time', 'tI'],  ['reset', 'SI'], ['phase', 'PF'], ['pan', 'QC'], ['client', 'gI'], 
      ['volume', 'VF'], ['pitch_bend', 'sF'], ['filter_freq', 'FC'], ['resonance', 'RF'], ['bp0', 'AL'], 
      ['bp1', 'BL'], ['eg0_type', 'TI'], ['eg1_type', 'XI'], ['debug', 'DI'], ['chained_osc', 'cI'], 
      ['mod_source', 'LI'],  ['eq', 'xL'], ['filter_type', 'GI'], ['ratio', 'IF'], ['latency_ms', 'NI'],
      ['algo_source', 'OL'], ['load_sample', 'zL'], ['transfer_file', 'zTL'], ['disk_sample', 'zFL'], 
      ['algorithm', 'oI'], ['chorus', 'kL'], ['reverb', 'hL'], ['echo', 'ML'], ['patch', 'KI'], ['voices', 'rL'],
      ['external_channel', 'WI'], ['portamento', 'mI'], ['sequence', 'HL'], ['tempo', 'jF'],
      ['synth', 'iI'], ['pedal', 'ipI'], ['synth_flags', 'ifI'], ['num_voices', 'ivI'], ['oscs_per_voice', 'inI'],
      ['to_synth', 'itI'], ['grab_midi_notes', 'imI'],  ['synth_delay', 'idI'],
      ['preset', 'pI'], ['num_partials', 'pI'], // note aliasing
      ['start_sample', 'zSL'], ['stop_sample', 'zOI'],
      ['midi_cc', 'icL'],
      ['patch_string', 'uS'],  // patch_string MUST be last because we can't identify when it ends except by end-of-message.
      ['end', 'ZZ'],
  ]

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

  // Build the rule set.
  let codes = new Map();
  for (const [fieldname, code] of _KW_MAP_LIST) {
    let type = code.slice(-1);  
    let wirecode = code.slice(0, -1);
    codes.set(wirecode, [fieldname, type]);
  }
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
    //console.log("pushback= ", pushback, " fieldname=", fieldname, " pending_list=", pending_list);
    if (pushback != null) {
      [lexcode, token, rest] = pushback;
      pushback = null;
    } else {
      [lexcode, token, rest] = next_token(rest);0
    }
    //console.log("expecting: ", expecting, " lexcode: " + lexcode + " " + debug_message(message, token, rest));
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
