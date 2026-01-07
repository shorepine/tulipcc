var amy_play_message = null;
var amy_live_start = null;
var audio_started = false;
var amy_sysclock = null;
var amy_get_input_buffer = null;
var amy_set_input_buffer = null;
var amyboard_started = false;
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
  amy_live_start = am.cwrap(
    'amy_live_start', null, ['number'], {async: true}    
  );
  amy_live_stop = am.cwrap(
    'amy_live_stop', null,  null, {async: true}    
  );
  amy_start = am.cwrap(
    'amy_start', null, ['number', 'number', 'number']
  );
  amy_play_message = am.cwrap(
    'amy_play_message', null, ['string']
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
  amy_get_input_buffer = am.cwrap(
    'amy_get_input_buffer', null, ['number']
  );
  amy_set_input_buffer = am.cwrap(
    'amy_set_external_input_buffer', null, ['number']
  );
  amy_start(1,1,1,1);
  amy_module = am;
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

async function setup_midi_devices() {
  var midi_in = document.amyboard_settings.midi_input;
  var midi_out = document.amyboard_settings.midi_output;
  if(WebMidi.inputs.length > midi_in.selectedIndex) {
    if(midiInputDevice != null) midiInputDevice.destroy();
    midiInputDevice = WebMidi.getInputById(WebMidi.inputs[midi_in.selectedIndex].id);
    midiInputDevice.addListener("midimessage", e => {
      for(byte in e.message.data) {
        mp.midiByte(e.message.data[byte]);
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


async function change_cv(which, value) {
    voltage = (value / 1000.0) * 10.0 - 5.0;
    voltage_string = voltage.toFixed(2).toString();
    if(which==1) {
        document.getElementById('cv-input-1-label').innerHTML = 'CV1: ' + voltage_string + 'V'
        cv_1_voltage = voltage;
    } else {
        document.getElementById('cv-input-2-label').innerHTML = 'CV2: ' + voltage_string + 'V'        
        cv_2_voltage = voltage;
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
        await amy_live_start(1);
    } else {
        amy_audioin_toggle = false;
        await amy_live_start(0);
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
  await mp.registerJsModule('amy_js_message', amy_play_message);
  await mp.registerJsModule('amy_sysclock', amy_sysclock);
  await mp.registerJsModule('amyboard_world_upload_file', amyboard_world_upload_file);
  await mp.registerJsModule('amy_get_input_buffer', get_audio_samples);
  await mp.registerJsModule('amy_set_external_input_buffer', set_audio_samples);
  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import tulip, amy, amy_js_message, amy_sysclock, amy_get_input_buffer, amy_set_external_input_buffer
    amy.override_send = amy_js_message
    tulip.amy_ticks_ms = amy_sysclock
    def amy_block_done_callback(f=None):
        amy.block_cb = f
    # put the js exported AMY shims in the spot tulip expects 
    tulip.amy_set_external_input_buffer = amy_set_external_input_buffer
    tulip.amy_get_input_buffer = amy_get_input_buffer
    tulip.amy_block_done_callback = amy_block_done_callback
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
      await amy_live_start(1);
  } else {
      await amy_live_start(0);    
  }
  audio_started = true;
}