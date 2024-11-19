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
  amy_start(1,1,1,1);
});

async function start_term_and_repl() {
  // Clear the terminal
  await term.clear();
  // Tell MP to start serving a REPL
  await mp.replInit();
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

async function start_audio() {
  // Don't run this twice
  if(everything_started) return;
  // Start the audio worklet (miniaudio)
  await amy_live_start();
  await register_amy();
  // Wait 200ms on first run only before playing amy commands back to avoid clicks
  await new Promise((r) => setTimeout(r, 200));
  await mp.runPythonAsync('import tulip\nfrom upysh import *')
  everything_started = true;
}




