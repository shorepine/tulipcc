var amy_play_message = null;
var amy_live_start = null;
var audio_started = false;
var tulip_started = false;
var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;

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

async function restart_tulip() {
  location.reload(true);
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
  }
}
/*
async function get_test() {
    await mp.runPythonAsync(`
        async def test():
          import js
          url = "https://api.github.com/users/micropython"
          print(f"fetching {url}...")
          res = await js.fetch(url)
          json = await res.json()
          print(json['name'])
          #for i in dir(json):
          #  print(f"{i}: {json[i]}")
          return json['name']
      `);
}
*/


async function runCodeBlock() {
  var py = editor.getValue();
  amy_reset_sysclock();
  try {
    mp.runPythonAsync(py);
  } catch (e) {
    await print_error(e.message);
  }
}

function create_editor(element) {
  code = element.textContent;
  element.innerHTML = `
  <div>
    <section class="input">
      <div><textarea id="code" name="code"></textarea></div> 
    </section>
    <div class="align-self-center my-3"> 
      <button type="button" class="btn btn-sm btn-success" onclick="runCodeBlock()">►</button> 
      <button type="button" class="btn btn-sm btn-danger" onclick="resetAMY()">Reset</button> 
    </div>
  </div>`;

  const editor = CodeMirror.fromTextArea(document.getElementById(`code`), { 
    mode: { 
      name: "python", 
      version: 3, 
      singleLineStringErrors: false,
      lint: false
    }, 
    lineNumbers: true, 
    indentUnit: 4, 
    matchBrackets: true,
    spellCheck: false,
    autocorrect: false,
    theme: "solarized dark",
    lint: false,
  }); 

  editor.setSize(null,200);
  editor.setValue(code.trim()); 
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
  // Wait 1s for audio to start
  await sleep_ms(1000);
  // Play the bleep
  await mp.runPythonAsync(`midi.startup_bleep()`);
  audio_started = true;
}