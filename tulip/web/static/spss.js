var amy_play_message = null;
var amy_live_start = null;
var audio_started = false;
var tulip_started = false;
var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;
var editor = null;
var treeView = null;

// Once AMY module is loaded, register its functions and start AMY (not yet audio, you need to click for that)
amyModule().then(async function(am) {
  amy_live_start = am.cwrap(
    'amy_live_start', null, null, {async: true}    
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
  amy_start(1,1,1,1);
});

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

async function setup_midi_devices() {
  var midi_in = document.tulip_settings.midi_input;
  var midi_out = document.tulip_settings.midi_output;
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
    var midi_in = document.tulip_settings.midi_input;
    var midi_out = document.tulip_settings.midi_output;

    if(WebMidi.inputs.length>0) {
      midi_in.options.length = 0;
      WebMidi.inputs.forEach(input => {
        midi_in.options[midi_in.options.length] = new Option(input.manufacturer + " " + input.name);
      });
    }

    if(WebMidi.outputs.length>0) {
      midi_out.options.length = 0;
      WebMidi.outputs.forEach(output => {
        midi_out.options[midi_out.options.length] = new Option(output.manufacturer + " " + output.name);
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
    document.getElementById('midi_settings').style.display='none';
  }
}

async function runCodeBlock() {
  // If audio hasn't yet started, the on-first-click audio starter is still running, so wait 1s so we don't glitch.
  if(!audio_started) await sleep_ms(1000);

  var py = editor.getValue();
  // Reset AMY's timebase every run here, so people can use absolute sequence / timestamps in AMY code.
  amy_reset_sysclock();
  try {
    mp.runPythonAsync(py);
  } catch (e) {
    console.log("Error in Python: " + e.message)
    // Print any error message to the REPL. Maybe there's a more direct way to raise JS errors to MPY
    await mp.runPythonAsync("print(\"\"\"" + e.message + "\"\"\")");
  }
}

async function shareCode() {
  // get the editor text, compress and base64 url safe it, and copy that + our base URL to the clipboard:
  var py = editor.getValue();
  code = encodeURIComponent(await compress(py));
  var encoded_filename = encodeURIComponent(document.getElementById('editor_filename').value);
  url = window.location.host+"/run/?fn=" + encoded_filename + "&" + "share=" + code;
  navigator.clipboard.writeText(url);
  // Do a little button animation
  document.getElementById(`shareButton`).innerHTML = "Copied to clipboard!"; 
  await sleep_ms(2500);
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
            reader.readAsText(file,'UTF-8');
            reader.onload = readerEvent => {
                var content = readerEvent.target.result; // this is the content!
                mp.FS.writeFile(upload_folder.fullpath + '/' + file.name, content);
                fill_tree();
            }
        }
        input.click();
    }
}

async function save_editor() {
    upload_folder = await request_file_or_folder(true);
    target_filename = document.getElementById('editor_filename').value;
    if(upload_folder != null) {
        if(target_filename.length > 0) {
            content = editor.getValue();
            mp.FS.writeFile(upload_folder.fullpath + '/' + target_filename, content);
            fill_tree();
        } else {
            show_alert('You need to provide a filename before saving.');
        }
    }
}


async function load_editor() {
    file = await request_file_or_folder(false);
    if(file != null) {
        editor.setValue(file.getContents('utf8'));
        document.getElementById('editor_filename').value = file.showpath;
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


async function start_tulip() {
  // Don't run this twice
  if(tulip_started) return;

  // Start midi
  await start_midi();

  // Let micropython call an exported AMY function
  await mp.registerJsModule('amy_js_message', amy_play_message);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import amy, amy_js_message
    amy.override_send = amy_js_message
  `);
  // If you don't have these sleeps we get a MemoryError with a locked heap. Not sure why yet.
  await sleep_ms(200);
  await mp.runFrozenAsync('_boot.py');
  await sleep_ms(200);
  await mp.runFrozenAsync('/tulip4/user/boot.py');
  tulip_started = true;
}

async function start_audio() {
  document.body.removeEventListener('click', start_audio, true); 
  document.body.removeEventListener('keydown', start_audio, true); 
  // Don't run this twice
  if(audio_started) return;

  // Start the audio worklet (miniaudio)
  await amy_live_start();
  audio_started = true;
}