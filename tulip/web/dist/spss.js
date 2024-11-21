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

function amy_sequencer_js_hook(tick) {
  mp.tulipTick(tick);
}

function setup_fs() {
  mp.FS.mkdir("/tulip4");
  mp.FS.mount(IDBFS, {autoPersist:true}, "/tulip4");
  mp.FS.syncfs(true);
}
async function register_amy() {
  // Let micropython call an exported AMY function
  await mp.registerJsModule('amy_js_message', amy_play_message);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("time", {
    sleep: async (s) => await new Promise((r) => setTimeout(r, s * 1000)),
  });

  // Set up the micropython context, like _boot.py. 
  await mp.runPythonAsync(`
    import amy, amy_js_message, time
    amy.override_send = amy_js_message
  `);
}

async function start_midi() {
  function onEnabled() {    
    // Inputs
    WebMidi.inputs.forEach(input => console.log(input.manufacturer, input.name));
    
    // Outputs
    WebMidi.outputs.forEach(output => console.log(output.manufacturer, output.name));

    const myInput = WebMidi.getInputByName("Q25");
    myInput.addListener("midimessage", e => {
      for(byte in e.message.data) {
        mp.midiByte(e.message.data[byte]);
      }
    });
  }

  WebMidi
    .enable({sysex:true})
    .then(onEnabled)
    .catch(err => alert(err));
}

async function start_audio() {
  // Don't run this twice
  if(everything_started) return;
  // Start midi
  await start_midi();
  // Start the audio worklet (miniaudio)
  await amy_live_start();
  await register_amy();
  // Wait 200ms on first run only before playing amy commands back to avoid clicks
  await new Promise((r) => setTimeout(r, 200));
  await mp.runFrozenAsync('_boot.py');
  await mp.runFrozenAsync('/tulip4/user/boot.py');
  everything_started = true;
}

async function tulip_tick() {
  //if(everything_started) await mp.tulipTick(amy_ticks());
}
