var amy_play_message = null;
var amy_live_start = null;
var amy_reset_sysclock = null
var amy_module = null;
var everything_started = false;
var mp = null;
var term = null;


// Once AMY module is loaded, register its functions and start AMY (not yet audio, you need to click for that)
amyModule().then(async function(am) {
  amy_module = am;
  amy_live_start = amy_module.cwrap(
    'amy_live_start', null, null, {async: true}    
  );
  amy_start = amy_module.cwrap(
    'amy_start', null, ['number', 'number', 'number']
  );
  amy_play_message = amy_module.cwrap(
    'amy_play_message', null, ['string']
  );
  amy_reset_sysclock = amy_module.cwrap(
    'amy_reset_sysclock', null, null
  );
  amy_ticks = amy_module.cwrap(
    'sequencer_ticks', 'number', [null]
  );
  amy_start(1,1,1,1);
});

// Called from AMY to update Tulip about what tick it is, for the sequencer
function amy_sequencer_js_hook(tick) {
  mp.tulipTick(tick);
}

async function register_amy() {
  // Let micropython call an exported AMY function
  await mp.registerJsModule('amy_js_message', amy_play_message);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("time", {
    sleep: async (s) => await new Promise((r) => setTimeout(r, s * 1000)),
  });

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import amy, amy_js_message, time
    amy.override_send = amy_js_message
  `);
}

var midiOutputDevice = null;
var midiInputDevice = null;

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

async function start_audio() {
  // Don't run this twice
  if(everything_started) return;
  // Start midi
  await start_midi();
  // Start the audio worklet (miniaudio)
  await amy_live_start();
  // Tell Tulip about this external AMY
  await register_amy();

  // Wait 200ms on first run only before playing amy commands back to avoid clicks
  await new Promise((r) => setTimeout(r, 200));

  await mp.runFrozenAsync('_boot.py');
  await mp.runFrozenAsync('/tulip4/user/boot.py');
  everything_started = true;
}

