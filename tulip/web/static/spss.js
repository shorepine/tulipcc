var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
//var amy_get_input_buffer = null;
//var amy_set_input_buffer = null;
var tulip_started = false;
var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;
var editor = null;
var treeView = null;
var editor_shown = false;
var amy_audioin_toggle = false;
var editor_height = 200;
var tulip_height = null; // gets set on launch
var amy_module = null;
var amy_yield_synth_commands = null;
var res_ptr_in = null;
var res_ptr_out = null;

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
  amy_yield_synth_commands = am.cwrap(
    'yield_synth_commands', 'number', ['number', 'number', 'number', 'boolean', 'number']
  );
  amy_start_web_no_synths();
  amy_bleep(0); // won't play until live audio starts
  amy_module = am;
  res_ptr_in = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  res_ptr_out = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s

});

// Read a NUL-terminated C string out of the AMY WASM heap (used to read back the
// wirecode strings yielded by yield_synth_commands).
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

// Canonical JS bridge for AMY's get_synth_commands. The C convenience wrapper
// exists for CPython (amy/src/pyamy.c) and for MicroPython as
// tulip.amy_get_synth_commands (modtulip.c) -- but the MicroPython one is compiled
// out on web (#ifndef __EMSCRIPTEN__) because here micropython does not link AMY;
// AMY runs in a separate WASM worklet. So we bridge it from JS instead, driving
// AMY's exported low-level generator yield_synth_commands. Reads back the wirecode
// commands that reconstruct synth `synth` (0..63, AMY's default max_synths) from
// AMY's current state and returns them as an array of strings (empty if the synth
// has no state). include_fx (default true) also emits the global FX commands.
// Throws if AMY's WASM module is not loaded or `synth` is out of range.
function get_synth_commands(synth, include_fx = true) {
  const s = Number(synth);
  if (!Number.isInteger(s) || s < 0 || s > 63) {
    throw new Error("get_synth_commands: synth must be an integer 0..63.");
  }
  if (!amy_module || typeof amy_yield_synth_commands !== "function") {
    throw new Error("get_synth_commands: AMY WASM module is not loaded.");
  }
  const maxMessageLen = 1024;
  const bufferPtr = amy_module._malloc(maxMessageLen);
  if (!bufferPtr) {
    throw new Error("get_synth_commands: failed to allocate AMY message buffer.");
  }
  const lines = [];
  let state = 0;
  let iterations = 0;
  const MAX_ITERATIONS = 500;
  try {
    do {
      state = amy_yield_synth_commands(s, bufferPtr, maxMessageLen, !!include_fx, state);
      const wire = read_c_string_from_heap(bufferPtr, maxMessageLen).trim();
      if (wire) {
        lines.push(wire);
      }
      if (++iterations >= MAX_ITERATIONS) {
        console.error("get_synth_commands: bailed after " + MAX_ITERATIONS + " iterations (state=" + state + ")");
        break;
      }
    } while (state != 0);
  } finally {
    amy_module._free(bufferPtr);
  }
  return lines;
}
// spss.js is loaded as a plain <script>, so this is already a global, but be
// explicit that it is the intended JS entry point for reading synth commands.
globalThis.get_synth_commands = get_synth_commands;

// String-returning wrapper for the MicroPython bridge (one wire command per
// line, "" if the synth is unconfigured or AMY isn't loaded yet). Installed as
// tulip.amy_get_synth_commands in start_tulip so e.g. tulip.save_synth_state()
// works on web like it does on firmware.
function amy_get_synth_commands_js(synth, include_fx) {
  try {
    return get_synth_commands(synth, !!include_fx).join('\n');
  } catch (e) {
    return "";
  }
}

// Called from AMY to update Tulip about what tick it is, for the sequencer
function amy_sequencer_js_hook(tick) {
  mp.tulipTick(tick);
}

async function clear_storage() {
  if(confirm("This will delete your Tulip user folder and start again.\nAre you sure?")) {
    indexedDB.deleteDatabase('/tulip4/user');
    location.reload(true);
  }
}

async function amy_external_midi_input_js_hook(bytes, len, sysex) {
    mp.midiInHook(bytes, len, sysex);
} 

async function setup_midi_devices() {
  var midi_in = document.tulip_settings.midi_input;
  var midi_out = document.tulip_settings.midi_output;
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
    var midi_in = document.tulip_settings.midi_input;
    var midi_out = document.tulip_settings.midi_output;

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
                if(thing.fullpath.startsWith('/tulip4/sys')) {
                    show_alert("You can't write to /sys on Tulip. Try /user.")
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
            mp.FS.writeFile("/tulip4"+target_filename, content);
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
        // trim the `/tulip4` from here
        document.getElementById('editor_filename').value = file.fullpath.substring("/tulip4".length);
        setTimeout(function () { editor.save() }, 100);
        setTimeout(function () { editor.refresh() }, 250);
    }
}


async function fill_tree() {
    var root = new TreeNode(new DirItem("/tulip4", "tulip4"));
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
    impl('/tulip4', root);
    treeView = new TreeView(root, "#treecontainer");
    treeView.changeOption("leaf_icon", '<i class="fas fa-file"></i>');
    treeView.changeOption("parent_icon", '<i class="fas fa-folder"></i>');
    treeView.changeOption("show_root", false);
    TreeConfig.open_icon = '<i class="fas fa-angle-down"></i>';
    TreeConfig.close_icon = '<i class="fas fa-angle-right"></i>';
    treeView.collapseAllNodes();
    treeView.reload();
}


// Create a js File object and upload it to the TW proxy API. This is easier to do here than in python.
// Returns {ok, detail} — detail is a human-readable error when ok is false.
async function tulip_world_upload_file(pwd, filename, username, description) {
    var contents = await mp.FS.readFile(pwd+filename, {encoding:'binary'});
    var file = await new File([new Uint8Array(contents)], filename, {type: 'application/binary'})
    var data = new FormData();
    data.append('file',file);
    data.append('username', username);
    data.append('description', description);
    var resp;
    try {
        resp = await fetch('https://tulipcc-production.up.railway.app/api/tulipworld/upload', {
            method: 'POST',
            body: data,
        });
    } catch (e) {
        return { ok: false, detail: 'network error: ' + e };
    }
    var detail = '';
    if (!resp.ok) {
        try {
            var err = await resp.json();
            if (typeof err.detail === 'string') detail = err.detail;
            else if (err.detail !== undefined) detail = JSON.stringify(err.detail);
        } catch (e) {}
        if (!detail) detail = 'HTTP ' + resp.status;
    }
    return { ok: resp.ok, detail: detail };
}

async function resize_tulip_grippie() {
    var tulip_width = document.getElementById('canvas').offsetWidth;
    tulip_height = document.getElementById('canvas').offsetHeight;
    document.getElementById('tulip_grippierow').setAttribute("style","width:"+tulip_width.toString()+"px");
}

async function show_editor() {
    document.getElementById('showhideeditor').style.display='none'; 
    if(editor) editor.refresh();
    document.getElementById('canvas').classList.remove("canvas-solo");
    document.getElementById('canvas').classList.add("canvas-editor");
    editor_shown = true;
    resize_tulip_grippie();
}

async function hide_editor() {
    document.getElementById('showhideeditor').style.display='';
    document.getElementById('canvas').classList.remove("canvas-editor");
    document.getElementById('canvas').classList.add("canvas-solo");
    editor_shown = false;
    resize_tulip_grippie();
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
        if(example_snippets[i].t != "deprecated") {
            h += ' <a href="#" onclick="run_snippet('+i.toString()+');"><span class="badge rounded-pill ' + colors[example_snippets[i].t] + '">'+example_snippets[i].d+'</span></a>';
        }
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



//function get_audio_samples() {
//    amy_module.ccall('amy_get_input_buffer', null, ["number"], [res_ptr_in]);
//    var view = amy_module.HEAPU8.subarray(res_ptr_in , res_ptr_in + 1024);
//    return view
//}

//function set_audio_samples(samples) {
//    amy_module.HEAPU8.set(samples, res_ptr_out);
//    amy_module.ccall('amy_set_external_input_buffer', null, ["number"], [res_ptr_out]);
//}

//async function amy_block_processed_js_hook() {
//    mp.runPythonAsync(`if amy.block_cb is not None: amy.block_cb(None)`);
//}


async function start_tulip() {
  // Don't run this twice
  if(tulip_started) return;

  // Start midi
  await start_midi();

  // Let micropython call an exported AMY function
  await mp.registerJsModule('amy_js_message', amy_add_message);
  await mp.registerJsModule('amy_sysclock', amy_sysclock);
  await mp.registerJsModule('tulip_world_upload_file', tulip_world_upload_file);
  // AMY WASM lives in a separate module; bridge synth-state readback for Python via JS.
  await mp.registerJsModule('amy_get_synth_commands_js', amy_get_synth_commands_js);
//  await mp.registerJsModule('amy_get_input_buffer', get_audio_samples);
//  await mp.registerJsModule('amy_set_external_input_buffer', set_audio_samples);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import tulip, amy, amy_js_message, amy_sysclock
    import amy_get_synth_commands_js as _amy_gsc_js
    # import amy_get_input_buffer, amy_set_external_input_buffer
    amy.override_send = amy_js_message
    tulip.amy_ticks_ms = amy_sysclock
    # put the js synth-state readback in the spot tulip.py expects (compiled out of modtulip.c on web)
    tulip.amy_get_synth_commands = lambda synth, include_fx=True: [c for c in _amy_gsc_js(synth, include_fx).split('\\n') if c]
    #def amy_block_done_callback(f=None):
    #    amy.block_cb = f
    # put the js exported AMY shims in the spot tulip expects 
    #tulip.amy_set_external_input_buffer = amy_set_external_input_buffer
    #tulip.amy_get_input_buffer = amy_get_input_buffer
    #tulip.amy_block_done_callback = amy_block_done_callback
  `);
  // If you don't have these sleeps we get a MemoryError with a locked heap. Not sure why yet.
  await sleep_ms(400);
  await mp.runFrozenAsync('_boot.py');
  await sleep_ms(400);
  await mp.runFrozenAsync('/tulip4/user/boot.py');
  tulip_started = true;
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
