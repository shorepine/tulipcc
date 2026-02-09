
var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
var amyboard_started = false;
var amy_parse_patch_number_to_events = null;

function amy_add_log_message(message) {
  console.log(message);
  if (typeof amy_add_message === "function") {
    amy_add_message(message);
  }
}

var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;
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
var amyboard_world_files = [];
var amyboard_world_loading_index = null;
var selected_environment_file = null;
var pending_environment_editor_load = false;
const AMYBOARD_MODAL_MESSAGES_URL = "https://bwhitman--tulipworldapi-messages.modal.run";
const AMYBOARD_MODAL_URLGET_URL = "https://bwhitman--tulipworldapi-urlget.modal.run";
const CURRENT_ENV_DIR = "/amyboard/user/current/env";
const CURRENT_PATCH_DIR = "/amyboard/user/current/patch";
const CURRENT_PATCH_FILE = CURRENT_PATCH_DIR + "/patches.txt";
const DEFAULT_ENV_SOURCE = "# Empty environment\nprint(\"Welcome to AMYboard!\")\n";
const DEFAULT_PATCH_SOURCE = "# patches.txt, ,AMY messages to load on boot\n";
window.current_synth = 1;

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
  amy_parse_patch_number_to_events = am.cwrap(
    'parse_patch_number_to_events', null, ['number', 'number', 'number']
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
  if (!amy_module || !amy_parse_patch_number_to_events) {
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

  const outPtr = amy_module._malloc(4);
  const countPtr = amy_module._malloc(2);
  if (!outPtr) {
    return [];
  }
  if (!countPtr) {
    amy_module._free(outPtr);
    return [];
  }
  heapU32[outPtr >> 2] = 0;
  view.setUint16(countPtr, 0, true);
  amy_parse_patch_number_to_events(patch_number, outPtr, countPtr);
  const eventsPtr = heapU32[outPtr >> 2];
  const eventCount = view.getUint16(countPtr, true);
  amy_module._free(outPtr);
  amy_module._free(countPtr);
  if (!eventsPtr) {
    return [];
  }

  const events = [];
  const limit = eventCount;
  for (let i = 0; i < limit; i += 1) {
    const base = eventsPtr + i * layout.size;
    const status = view.getUint8(base + offsets.status);
    events.push(readEvent(base));
  }

  amy_module._free(eventsPtr);
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
  log = 0;
  if(knob.knob_type=='log') {
    log = 1;
  }
  // if knob.offset doesn't exist, make it 0
  if(typeof knob.offset === "undefined") {
    knob.offset = 0;
  }
  var m = "i"+window.current_synth+"ic"+knob.cc+","+log+","+knob.min_value+","+knob.max_value+","+knob.offset+","+knob.change_code;
  //console.log("Knob CC updated: " + knob.section + ": " + knob.display_name + " to " + knob.cc + ". Sending: " + m);
  if (typeof amy_add_message === "function") {
    amy_add_log_message(m);
  }
}


window.addEventListener("DOMContentLoaded", function() {
  requestPatchKnobSync(0);
});

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
      scaled_value = Math.round((options.length - 1) * (value / 127));
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
  // reset patch
  amy_add_log_message("K"+patchNumber+"S524288");
  if (typeof window.amyboard_patch_string !== "string") {
    return;
  }
  const chunks = window.amyboard_patch_string.split("Z");
  for (const chunk of chunks) {
    if (!chunk) {
      continue;
    }
    amy_add_log_message("K" + patchNumber + chunk);
  }

  const knobList = window.get_current_knobs ? window.get_current_knobs() : [];
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
    const payload = window.make_change_code(window.current_synth || 1, value, knob, true);
    if (!payload) {
      continue;
    }
    amy_add_log_message("K" + patchNumber + payload);
  }
  amy_add_log_message("i" + window.current_synth + "K" + patchNumber);
}

function load_from_patch(patchNumber) {
  // Hook for loading a saved memory patch number.
  set_knobs_from_patch_number(patchNumber);
}


async function amy_external_midi_input_js_hook(bytes, len, sysex) {
    mp.midiInHook(bytes, len, sysex);
} 


async function setup_midi_devices() {
  var midi_in = document.amyboard_settings.midi_input;
  var midi_out = document.amyboard_settings.midi_output;
  if(WebMidi.inputs.length > midi_in.selectedIndex) {
    if(midiInputDevice != null) midiInputDevice.destroy();
    midiInputDevice = WebMidi.getInputById(WebMidi.inputs[midi_in.selectedIndex].id);
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
  }
  if(WebMidi.outputs.length > midi_out.selectedIndex) {
    if(midiOutputDevice != null) midiOutputDevice.destroy();
    midiOutputDevice = WebMidi.getOutputById(WebMidi.outputs[midi_out.selectedIndex].id);
  }
}

async function start_midi() {
  function onEnabled() {
    // Populate the dropdowns
    var midi_in = document.amyboard_settings.midi_input;
    var midi_out = document.amyboard_settings.midi_output;

    if(WebMidi.inputs.length>0) {
      midi_in.options.length = 0;
      WebMidi.inputs.forEach(input => {
        midi_in.options[midi_in.options.length] = new Option("MIDI in: " + input.name);
      });
    }

    if(WebMidi.outputs.length>0) {
      midi_out.options.length = 0;
      WebMidi.outputs.forEach(output => {
        midi_out.options[midi_out.options.length] = new Option("MIDI out: "+ output.name);
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

  // Reset AMY's timebase every run here, so people can use absolute sequence / timestamps in AMY code.
  amy_reset_sysclock();
  await sleep_ms(50);
  try {
    mp.runPythonAsync(py);
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
    try { mp.FS.mkdirTree("/amyboard/user/current"); } catch (e) {}
    try { mp.FS.mkdirTree(CURRENT_ENV_DIR); } catch (e) {}
    try { mp.FS.mkdirTree(CURRENT_PATCH_DIR); } catch (e) {}
    if (seedDefaults) {
        try {
            mp.FS.readFile(CURRENT_ENV_DIR + "/env.py", { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(CURRENT_ENV_DIR + "/env.py", DEFAULT_ENV_SOURCE);
        }
        try {
            mp.FS.readFile(CURRENT_PATCH_FILE, { encoding: "utf8" });
        } catch (e) {
            mp.FS.writeFile(CURRENT_PATCH_FILE, DEFAULT_PATCH_SOURCE);
        }
    }
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
    selected_environment_file = filename;
    render_environment_file_list();
    if (loadEditor) {
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
        reader.onload = readerEvent => {
            var content = readerEvent.target.result;
            mp.FS.writeFile(CURRENT_ENV_DIR + "/" + file.name, new Uint8Array(content), { encoding: "binary" });
            selected_environment_file = file.name;
            fill_tree();
        };
    };
    input.click();
}

async function save_editor() {
    ensure_current_environment_layout(false);
    var targetFilename = selected_environment_file || "env.py";
    var slash = targetFilename.lastIndexOf("/");
    if (slash > 0) {
        try { mp.FS.mkdirTree(CURRENT_ENV_DIR + "/" + targetFilename.substring(0, slash)); } catch (e) {}
    }
    mp.FS.writeFile(CURRENT_ENV_DIR + "/" + targetFilename, editor.getValue());
    selected_environment_file = targetFilename;
    await fill_tree();
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
    var fullPath = CURRENT_ENV_DIR + "/" + selected_environment_file;
    try {
        editor.setValue(mp.FS.readFile(fullPath, { encoding: "utf8" }));
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
    for (var i = 0; i < files.length; i++) {
        if (files[i].toLowerCase().endsWith(".py")) {
            target = files[i];
            break;
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

function clear_current_environment_dir() {
    ensure_current_environment_layout(false);
    for (const name of mp.FS.readdir(CURRENT_ENV_DIR)) {
        if (name === "." || name === "..") continue;
        remove_fs_path(CURRENT_ENV_DIR + "/" + name);
    }
}

async function run_current_environment() {
    await runCodeBlock(
        "from upysh import *\n" +
        "cd('/amyboard/user/current/env')\n" +
        "execfile('env.py')\n"
    );
}

function add_octal_to_buffer(buffer, offset, length, value) {
    var oct = Math.max(0, value).toString(8);
    var padded = oct.padStart(length - 1, "0");
    for (var i = 0; i < length; i++) {
        buffer[offset + i] = 0;
    }
    for (var j = 0; j < padded.length && j < (length - 1); j++) {
        buffer[offset + j] = padded.charCodeAt(j);
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
    add_octal_to_buffer(header, 100, 8, isDirectory ? 0o755 : 0o644);
    add_octal_to_buffer(header, 108, 8, 0);
    add_octal_to_buffer(header, 116, 8, 0);
    add_octal_to_buffer(header, 124, 12, isDirectory ? 0 : size);
    add_octal_to_buffer(header, 136, 12, Math.floor(Date.now() / 1000));
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
    var checksum = sum.toString(8).padStart(6, "0");
    write_string_to_buffer(header, 148, 6, checksum);
    header[154] = 0;
    header[155] = 32;
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
            pushChunk(tar_build_header(environmentName + "/" + dirPath, 0, true));
            for (const child of mp.FS.readdir(fullPath)) {
                if (child === "." || child === "..") continue;
                add_entry(relativePath + "/" + child);
            }
            return;
        }
        var bytes = mp.FS.readFile(fullPath, { encoding: "binary" });
        pushChunk(tar_build_header(environmentName + "/" + relativePath, bytes.length, false));
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
        var when = format_world_file_timestamp(item.time);
        var meta = (item.username || "unknown") + (when ? " • " + when : "");
        var isLoading = (amyboard_world_loading_index === i);
        html += '<div class="col">';
        html += '<button type="button" class="w-100 border rounded p-2 bg-white text-start d-flex justify-content-between align-items-start"' +
            ' onclick="import_amyboard_world_file(' + i + ')"' + (isLoading ? " disabled" : "") + ">";
        html += '<div class="me-3 text-start"><div class="fw-bold">' + escape_html(displayName);
        if (description.length > 0) {
            html += ' <span class="fw-normal text-muted">- ' + escape_html(description) + "</span>";
        }
        html += '</div><div class="small text-muted">' + escape_html(meta) + '</div></div>';
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
    var list = document.getElementById("amyboard_world_file_list");
    if (list) {
        list.innerHTML = '<div class="border rounded p-2 text-muted">Loading...</div>';
    }

    try {
        var response = await fetch(AMYBOARD_MODAL_MESSAGES_URL + "?mtype=amyboard&n=100");
        if (!response.ok) {
            throw new Error("HTTP " + response.status.toString());
        }
        var data = await response.json();
        if (!Array.isArray(data)) {
            throw new Error("Unexpected response");
        }
        amyboard_world_files = data.filter(function(item) {
            return item && is_world_tar_filename(item.filename);
        });
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
        var response = await fetch(AMYBOARD_MODAL_URLGET_URL + "?url=" + encodeURIComponent(item.url));
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
        await run_current_environment();

        await refresh_amyboard_world_files();
    } catch (e) {
        show_alert("Failed to import remote environment.");
    } finally {
        amyboard_world_loading_index = null;
        render_amyboard_world_file_list();
    }
}



// Create a js File object and upload it to the TW proxy API. This is easier to do here than in python
async function amyboard_world_upload_file(pwd, filename, username, description) {
    var contents = await mp.FS.readFile(pwd+filename, {encoding:'binary'});
    var file = await new File([new Uint8Array(contents)], filename, {type: 'application/binary'})
    var data = new FormData();
    data.append('file',file);
    data.append('username', username);
    data.append('description', description);
    data.append('which', 'amyboard');
    return fetch('https://bwhitman--tulipworldapi-upload.modal.run', {
        method: 'POST',
        body: data,
    });
}

async function upload_current_environment() {
    ensure_current_environment_layout(false);
    var envNameInput = document.getElementById("editor_filename");
    var usernameInput = document.getElementById("environment_username");
    var descriptionInput = document.getElementById("environment_description");
    if (!envNameInput || !usernameInput || !descriptionInput) return;

    var username = (usernameInput.value || "").trim();
    var environmentName = (envNameInput.value || "").trim();
    var description = (descriptionInput.value || "").trim();

    if (!/^[A-Za-z0-9]{1,20}$/.test(username)) {
        show_alert("Username must be 1-20 chars: only A-Z, a-z, 0-9 (no spaces).");
        return;
    }

    if (!/^[A-Za-z0-9_-]{1,20}$/.test(environmentName)) {
        show_alert("Environment Name must be 1-20 chars: only A-Z, a-z, 0-9, - and _.");
        return;
    }

    if (description.length < 1 || description.length > 40) {
        show_alert("Description must be 1-40 characters.");
        return;
    }

    var tarBytes = build_environment_tar_bytes(environmentName);
    var tarFilename = environmentName + ".tar";
    var file = new File([tarBytes], tarFilename, { type: "application/x-tar" });
    var data = new FormData();
    data.append("file", file);
    data.append("username", username);
    data.append("description", description);
    data.append("which", "amyboard");
    try {
        var response = await fetch("https://bwhitman--tulipworldapi-upload.modal.run", {
            method: "POST",
            body: data,
        });
        if (!response.ok) {
            throw new Error("HTTP " + response.status.toString());
        }
        await refresh_amyboard_world_files();
    } catch (e) {
        show_alert("Upload failed.");
    }
}

function populate_send_to_amyboard_modal() {
    var unsupported = document.getElementById("send-amyboard-unsupported");
    var supported = document.getElementById("send-amyboard-supported");
    var midiOut = document.getElementById("send-amyboard-midi-out");
    var sendButton = document.getElementById("send-amyboard-send-btn");
    if (!unsupported || !supported || !midiOut) return;
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
    midiOut.options.length = 0;
    if (WebMidi.outputs.length === 0) {
        midiOut.options[midiOut.options.length] = new Option("No MIDI outputs found");
        return;
    }
    WebMidi.outputs.forEach(function(output) {
        midiOut.options[midiOut.options.length] = new Option(output.name, output.id);
    });
}

function open_send_to_amyboard_modal() {
    populate_send_to_amyboard_modal();
    var modalEl = document.getElementById("sendToAmyboardModal");
    if (!modalEl) return;
    var modal = bootstrap.Modal.getOrCreateInstance(modalEl);
    modal.show();
}

function send_to_amyboard_now() {
    // Placeholder only: transfer logic comes next.
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


async function run_snippet(i) {
    var code = example_snippets[i].c.trimStart();
    if(!editor_shown) {
        var editorElement = document.getElementById('collapseEditor');
        var bsCollapse = new bootstrap.Collapse(editorElement, { toggle: true });
        show_editor();
    }
    editor.setValue(code);
    setTimeout(function () { editor.save() }, 100);
    setTimeout(function () { editor.refresh() }, 250);
    setTimeout(function () { window.scrollTo(0,0) }, 300);
    runEditorBlock();

}

async function fill_examples() {
    colors = {
        'music':'bg-success',
        'network':'bg-warning text-dark',
        'games':'bg-primary',
        'other':'bg-danger'
    } // 'bg-info text-dark', 'bg-secondary', 'bg-dark'
    h = '';
    var i = 0;
    for (i=0;i<example_snippets.length;i++) { 
        h += ' <a href="#" onclick="run_snippet('+i.toString()+');"><span class="badge rounded-pill ' + colors[example_snippets[i].t] + '">'+example_snippets[i].d+'</span></a>';
    } 
    document.getElementById('tutorials').innerHTML = h;
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
  clear_current_environment_dir();
  mp.FS.writeFile(CURRENT_ENV_DIR + "/env.py", DEFAULT_ENV_SOURCE);
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
  // Initialize to the "amyboardweb" preset (patch 257), 6 voices.
  audio_started = true;
  amy_add_log_message("i1iv6K257")
  attemptPatchKnobSync();
}
