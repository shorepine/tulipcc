
var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
var amyboard_started = false;
var amy_parse_patch_number_to_events = null;

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
  const MAX_PARAM_LEN = layout.constants.MAX_PARAM_LEN;
  const MAX_EVENTS_PER_PATCH = layout.constants.MAX_EVENTS_PER_PATCH;
  const EVENT_EMPTY = 0;
  const offsets = layout.offsets;

  const heapU8 = amy_module.HEAPU8;
  if (!heapU8) {
    return [];
  }
  const heapU32 = amy_module.HEAPU32 || new Uint32Array(heapU8.buffer);
  const view = new DataView(heapU8.buffer);

  function readString(ptr, maxLen) {
    let end = 0;
    while (end < maxLen) {
      if (view.getUint8(ptr + end) === 0) break;
      end += 1;
    }
    let out = "";
    for (let i = 0; i < end; i += 1) {
      out += String.fromCharCode(view.getUint8(ptr + i));
    }
    return out;
  }

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
      bp0: readString(base + offsets.bp0, MAX_PARAM_LEN),
      bp1: readString(base + offsets.bp1, MAX_PARAM_LEN),
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
      reset_osc: view.getUint32(base + offsets.reset_osc, true)
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
    if (status === EVENT_EMPTY) {
      break;
    }
    events.push(readEvent(base));
  }

  amy_module._free(eventsPtr);
  return events;
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
      for(byte in e.message.data) {
        amy_process_single_midi_byte(e.message.data[byte], 1);
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
    document.getElementById('midi-input-panel').style.display='none';
    document.getElementById('midi-output-panel').style.display='none';
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

class DirItem {
    constructor(fullpath, showpath, is_dir) {
        this.fullpath = fullpath;
        this.showpath = showpath;
        this.is_dir = is_dir
      }
    
    getContents(encoding) {
        if(!this.is_dir) {
            return mp.FS.readFile(this.fullpath, {encoding:encoding});
        }
        return null;
    }
    toString() {
        return this.showpath;
    }
}

// Ask for a selected FS item in the treeview and return it, showing errors if not
async function request_file_or_folder(want_folder) {
    if(treeView.getSelectedNodes().length > 0) {
        thing = treeView.getSelectedNodes()[0].getUserObject();
        if(want_folder) {
            if(thing.is_dir) { 
                if(thing.fullpath.startsWith('/amyboard/sys')) {
                    show_alert("You can't write to /sys on AMYboard. Try /user.")
                    return null;
                }
                return thing; 
            } 
            show_alert("Please select a destination folder (not a file).");
            return null;
        } else {
            if(!thing.is_dir) { 
                return thing;
            }
            show_alert("Please select a file (not a folder).");
            return null;
        }       
    }
    var t = "file"
    if(want_folder) t = "folder;"
    show_alert("Please select a " + t + " first.")
    return null;
}

async function download() {
    file = await request_file_or_folder(false);
    if(file != null) {
        var blob=new Blob([file.getContents('binary')], {type: "application/binary"});
        var link=document.createElement('a');
        link.href=window.URL.createObjectURL(blob);
        link.download=file.showpath;
        link.click();
    }
}

async function upload() {
    upload_folder = await request_file_or_folder(true);
    if(upload_folder != null) {
        // Get a file upload window, read the data and write to FS
        var input = document.createElement('input');
        input.type = 'file';
        input.onchange = e => { 
            var file = e.target.files[0]; 
            var reader = new FileReader();
            reader.readAsArrayBuffer(file); //,'UTF-8');
            reader.onload = readerEvent => {
                var content = readerEvent.target.result; // this is the content!
                mp.FS.writeFile(upload_folder.fullpath + '/' + file.name, new Uint8Array(content), {encoding:'binary'});
                fill_tree();
            }
        }
        input.click();
    }
}

async function save_editor() {
    target_filename = document.getElementById('editor_filename').value;
    if(target_filename.startsWith("/sys")) {
        show_alert("You can't write to the /sys folder. Save it to /user.");
    } else {
        if(target_filename.length > 0) {
            if(!target_filename.startsWith("/user")) {
                target_filename = "/user/" + target_filename;
            }
            content = editor.getValue();
            mp.FS.writeFile("/amyboard"+target_filename, content);
            document.getElementById('editor_filename').value = target_filename;
        } else {
            show_alert('You need to provide a filename before saving.');
        }
    }
}


async function load_editor() {
    file = await request_file_or_folder(false);
    if(file != null) {
        editor.setValue(file.getContents('utf8'));
        // trim the `/amyboard` from here
        document.getElementById('editor_filename').value = file.fullpath.substring("/amyboard".length); 
        setTimeout(function () { editor.save() }, 100);
        setTimeout(function () { editor.refresh() }, 250);
    }
}


async function fill_tree() {
    var root = new TreeNode(new DirItem("/amyboard", "amyboard"));
    function impl(curFolder, curNode) {
        for (const name of mp.FS.readdir(curFolder)) {
            if (name === '.' || name === '..') continue;
            const path = `${curFolder}/${name}`;
            const { mode, timestamp } = mp.FS.lookupPath(path).node;
            if (mp.FS.isFile(mode)) {
                var file = new TreeNode(new DirItem(path, name, false));
                curNode.addChild(file);
            } else if (mp.FS.isDir(mode)) {
                var newdir = new TreeNode(new DirItem(path, name, true));
                curNode.addChild(newdir);
                impl(path, newdir);
            }
        }
    }
    impl('/amyboard', root);
    treeView = new TreeView(root, "#treecontainer");
    treeView.changeOption("leaf_icon", '<i class="fas fa-file"></i>');
    treeView.changeOption("parent_icon", '<i class="fas fa-folder"></i>');
    treeView.changeOption("show_root", false);
    TreeConfig.open_icon = '<i class="fas fa-angle-down"></i>';
    TreeConfig.close_icon = '<i class="fas fa-angle-right"></i>';
    treeView.collapseAllNodes();
    treeView.reload();
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
  audio_started = true;
}
