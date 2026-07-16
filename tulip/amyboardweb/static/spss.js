console.log('[spss.js] build sentinel: knobs-dedup-v3 loaded');
var amy_add_message = null;
var amy_live_start_web = null;
var amy_bleep = null;
var amy_live_start_web_audioin = null;
var amy_process_single_midi_byte = null;
var audio_started = false;
var amy_sysclock = null;
var amyboard_started = false;
// Deep-link guard: set in start_amyboard() when the initial URL has ?env=... ,
// read later in start_audio() after check_url_env_params() has mutated the URL.
// Keeps the default run_sketch() path from racing the deferred world-load.
var _url_env_pending = false;
// Deep-link autoplay (?play=1, used by the marketing page's sketch links):
// once BOTH the deep-link sketch has loaded and audio has started, auto-write
// it to the simulator so it plays without pressing "Write to Simulator".
// Simulate mode only — never auto-writes to real hardware.
var _url_env_autoplay = false;
var _url_env_loaded = false;
var amy_yield_synth_commands = null;
var amy_dump_state_to_string_c = null;

var _python_error_buffer = "";
var _python_error_timer = null;

// Intercept console.log to catch Python errors printed by micropython WASM
var _orig_console_log = console.log.bind(console);
console.log = function() {
  _orig_console_log.apply(console, arguments);
  var msg = Array.from(arguments).join(" ");
  if (msg.indexOf("Traceback") !== -1 || msg.indexOf("sketch.py load failed") !== -1 ||
      msg.indexOf("sketch.loop() error") !== -1 || _python_error_buffer.length > 0) {
    _python_error_buffer += msg + "\n";
    if (_python_error_timer) clearTimeout(_python_error_timer);
    _python_error_timer = setTimeout(function() {
      if (_python_error_buffer.trim().length > 0 && typeof show_python_error === "function") {
        show_python_error(_python_error_buffer.trim());
      }
      _python_error_buffer = "";
      _python_error_timer = null;
    }, 500);
  }
};

function amy_add_log_message(message) {
  console.log(message);
  if (typeof amy_add_message === "function") {
    amy_add_message(message);
  }
}

var mp = null;
var midiOutputDevice = null;
var midiInputDevice = null;
var midiPassthruDevice = null;
var midiPassthruOptionIds = [];  // parallel to the pass-thru <option>s; first entry null for [None]
var _midi_passthru_listener = null;
var _midi_passthru_input = null;
var midiInputOptionIds = [];
var midiOutputOptionIds = [];
var editor = null;
var treeView = null;
var editor_shown = false;
var amy_audioin_toggle = false;
var editor_height = 300;
var term = null;
var cv_1_voltage = 0;
var cv_2_voltage = 0;
var amy_module = null;
var amyboard_world_files = [];
var amyboard_world_loading_index = null;
var amyboard_world_selected_tag = "";
var amyboard_world_tag_palette = {};
// Tag pills shown in the AMYboard World filter UI. #generated is a shortcut
// for username=generator (see refresh_amyboard_world_files); the hardware
// tags (#display, #encoder) are applied automatically by the server when a
// shared generation is published.
var AMYBOARD_WORLD_PILL_TAGS = ["featured", "official", "generated", "display", "encoder"];
var selected_environment_file = null;
var pending_environment_editor_load = false;
var environment_editor_dirty = false;
var environment_autosave_inflight = false;
const AMYBOARD_WORLD_API_BASE = (typeof window !== "undefined" && typeof window.AMYBOARD_WORLD_API_BASE === "string")
    ? String(window.AMYBOARD_WORLD_API_BASE).replace(/\/+$/, "")
    : String(window.location.origin || "").replace(/\/+$/, "");
const CURRENT_BASE_DIR = "/amyboard/user/current";
const CURRENT_ENV_DIR = CURRENT_BASE_DIR;
// Default sketch. The canonical source is amyboard.DEFAULT_SKETCH_SOURCE in Python
// (read via mp in simulate); this JS copy is the fallback for control mode, where
// there is no MicroPython. Keep it in sync with amyboard.py's DEFAULT_SKETCH_SOURCE.
var AMYBOARD_DEFAULT_SKETCH =
  "# AMYboard Sketch\n" +
  "# Code put here runs first, then loop(step) is called every 32nd note,\n" +
  "# starting on a bar downbeat. step counts 32nd notes on the sequencer's\n" +
  "# bar-locked grid, so step % 32 == 0 is always a downbeat.\n" +
  "import amyboard, amy\n\n" +
  "def loop(step):\n    pass\n\n" +
  "# Do not edit. Set automatically by the knobs on AMYboard Online.\n" +
  "_auto_generated_knobs = \"\"\"\n\"\"\"\n";
function _get_default_sketch() {
  if (mp) {
    try { return mp.runPython("import amyboard; amyboard.DEFAULT_SKETCH_SOURCE"); } catch (e) {}
  }
  return AMYBOARD_DEFAULT_SKETCH;
}

// ── Persistent editor draft ─────────────────────────────────────────────────
// The sketch in the CodeMirror editor persists locally across reloads, separate
// from "writing to the AMYboard" (which targets the device — zT over sysex in
// control, the MicroPython FS in simulate). On page load we restore this draft
// instead of pulling from the board; with no draft we show the default sketch.
// Stored in localStorage rather than a cookie because a sketch (code + knob
// block) can exceed the ~4KB cookie limit, which would silently truncate it.
var SKETCH_DRAFT_KEY = 'amyboard_sketch_draft';
function save_sketch_draft(text) {
  try { localStorage.setItem(SKETCH_DRAFT_KEY, String(text == null ? '' : text)); } catch (e) {}
}
function load_sketch_draft() {
  try { return localStorage.getItem(SKETCH_DRAFT_KEY); } catch (e) { return null; }
}
function clear_sketch_draft() {
  try { localStorage.removeItem(SKETCH_DRAFT_KEY); } catch (e) {}
}
window.save_sketch_draft = save_sketch_draft;
window.clear_sketch_draft = clear_sketch_draft;

// set_knobs_from_sketch() needs the channel knob grid to be rendered. On page
// load the restore can run before that, so poll briefly until the knobs exist,
// then position. (User-triggered callers — Reset, Read, file-load — call
// set_knobs_from_sketch directly since the grid is already up.)
function position_knobs_from_sketch_when_ready(attempts) {
  attempts = attempts || 0;
  var knobs = (typeof window.get_current_knobs === 'function') ? window.get_current_knobs() : null;
  var ready = Array.isArray(knobs) && knobs.length > 0;
  if (ready || attempts >= 50) {
    if (typeof set_knobs_from_sketch === 'function') set_knobs_from_sketch();
  } else {
    setTimeout(function() { position_knobs_from_sketch_when_ready(attempts + 1); }, 100);
  }
}
window.position_knobs_from_sketch_when_ready = position_knobs_from_sketch_when_ready;

// Restore the editor from the persisted draft (or the default sketch if none) and
// position the knobs. Used on page load for both modes — no Pull dialog.
function restore_sketch_into_editor() {
  var draft = load_sketch_draft();
  var text = (draft != null && draft.length) ? draft : _get_default_sketch();
  if (typeof editor !== 'undefined' && editor) {
    editor.setValue(text);
    window.environment_editor_dirty = false;
    setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
    position_knobs_from_sketch_when_ready();
  } else {
    window._deferred_editor_content = text;
  }
}
window.restore_sketch_into_editor = restore_sketch_into_editor;
const EDITOR_ALLOWED_EXTENSIONS = [".py", ".txt", ".json"];
const AMYBOARD_SYSEX_MFR_ID = [0x00, 0x03, 0x45];
const AMYBOARD_TRANSFER_CHUNK_BYTES = 188;
window.current_synth = 1;
// GM MIDI convention: channel 10 is the drum channel. The device boots a GM
// drum kit on it (i10K384iv6 in amyboard.py start_amy/_apply_knobs_text), so
// the web editor treats channel 10 as active-with-drums by default too.
// This is a BOOT-only default: once the user changes channel 10 (deactivates
// it, loads a preset, clears it), the UI never re-assumes drums — the channel
// behaves like any other (K257 on activate/clear).
window.DRUM_CHANNEL = 10;
window.DRUM_DEFAULT_PATCH = 384;  // drum kit 0 TR-808 — matches the device boot default
window.active_channels = Array.isArray(window.active_channels) ? window.active_channels : new Array(17).fill(false);
window.active_channels[1] = true;
window.active_channels[window.DRUM_CHANNEL] = true;
window.channel_control_mapping_sent = Array.isArray(window.channel_control_mapping_sent) ? window.channel_control_mapping_sent : new Array(17).fill(false);
window.suppress_knob_cc_send = false;

// ============================================================================
// Knob log — the single source of truth for the _auto_generated_knobs block.
//
// SYNC 2 MODEL: the sketch is ground truth. The knob block is NOT a dump of live
// AMY state; it is a log of UI-originated actions only (knob moves, knob/CC
// settings, preset loads, channel activate/deactivate). We NEVER grab AMY state
// to build it — external MIDI/CV moves and sketch-code effects are deliberately
// not captured.
//
// window.knob_log is an insertion-ordered Map(key -> {line, synth}). Each value
// is one wire line (or a Z-joined pair, for the compound pitch/detune knobs).
// Serializing the values in order yields the block; on boot the device replays
// the lines on top of a freshly reset default synth-1 (see amyboard.py
// _apply_knobs_text), so the sketch alone fully determines the sound.
//
//   - Knob moves/settings collapse last-value-wins, keyed by the message's
//     STRUCTURAL key (i<synth>/v<osc> kept literal, other values normalized), so
//     wiggling a knob never grows the block — and a line loaded from a sketch
//     keys the same way, so a later move collapses onto it instead of duplicating.
//   - Setup/preset lines are per-synth; reset_synth_in_sketch() clears a synth's
//     lines before a (re)activate / Clear / preset load rebuilds them, so they
//     never accumulate across loads.
//   - Reading a sketch (Read / download / pageload) rebuilds the log from the
//     block text verbatim, then positions the visible knobs via the existing
//     apply_zd_dump_to_knobs() parser (legacy combined-dump lines included), so a
//     later Write re-emits what was loaded with knob tweaks layered on top.
// ============================================================================
window.knob_log = (window.knob_log instanceof Map) ? window.knob_log : new Map();
var _KNOB_LOG_SEP = '';  // unit separator — never appears in section/name

function _knob_log_parse_synth(line) {
  var m = /^i(\d+)/.exec(String(line || ''));
  return m ? parseInt(m[1], 10) : null;
}

function _knob_log_ensure_z(line) {
  line = String(line == null ? '' : line);
  return line.charAt(line.length - 1) === 'Z' ? line : line + 'Z';
}

// Structural key for a wire message: keep the i<synth> and v<osc> selectors
// literal (they identify the channel/oscillator) and normalize every other numeric
// value to '#', preserving comma positions. So the same parameter slot collapses
// regardless of its value — and a line loaded from a sketch keys the SAME way a
// live knob move does, so they never duplicate (the i1v0F-written-twice bug).
function _knob_log_struct_key(message) {
  var s = String(message == null ? '' : message);
  var out = '';
  var i = 0;
  // A LEADING y<bus> is a bus selector (bus-directed FX line, e.g. y1x,,-3) —
  // keep it literal so each bus gets its own last-wins slot. A y elsewhere is
  // the bus VALUE of a synth assignment (i1y1) and must normalize to '#' so
  // re-assignments collapse onto one line.
  if (s.charAt(0) === 'y' && s.charCodeAt(1) >= 48 && s.charCodeAt(1) <= 57) {
    out += 'y'; i = 1;
    while (i < s.length && s.charCodeAt(i) >= 48 && s.charCodeAt(i) <= 57) { out += s.charAt(i); i++; }
  }
  while (i < s.length) {
    var c = s.charAt(i);
    if ((c === 'i' || c === 'v') && s.charCodeAt(i + 1) >= 48 && s.charCodeAt(i + 1) <= 57) {
      // i<synth> / v<osc> selector — keep the letter and its digits literally.
      out += c; i++;
      while (i < s.length && s.charCodeAt(i) >= 48 && s.charCodeAt(i) <= 57) { out += s.charAt(i); i++; }
    } else if ((c >= '0' && c <= '9') ||
               ((c === '-' || c === '.') && s.charCodeAt(i + 1) >= 48 && s.charCodeAt(i + 1) <= 57)) {
      // value number — collapse the whole run to a single '#'.
      out += '#';
      while (i < s.length && ((s.charAt(i) >= '0' && s.charAt(i) <= '9') || s.charAt(i) === '.' || s.charAt(i) === '-')) i++;
    } else {
      out += c; i++;
    }
  }
  return out;
}

// Serialize the log to the block body: one Z-terminated wire line per entry.
function serialize_knob_log() {
  var out = [];
  window.knob_log.forEach(function(entry) {
    if (entry && entry.line) out.push(_knob_log_ensure_z(entry.line));
  });
  return out.join('\n');
}
window.serialize_knob_log = serialize_knob_log;

// Upsert (or delete) a UI-originated wire message in the log, optionally sending
// it live to AMY. opts.key identifies the slot (so repeats collapse). opts.synth
// scopes it for per-synth clearing (defaults to the i<N> prefix in the message).
// opts.silent skips the live AMY send; opts.remove deletes opts.key.
function send_amy_message_in_sketch(message, opts) {
  opts = opts || {};
  var msg = String(message == null ? '' : message);
  // Default to the message's structural key so the same parameter slot always
  // collapses (live moves AND lines loaded from a sketch land on one key).
  var key = (opts.key !== undefined) ? opts.key : _knob_log_struct_key(msg);
  var synth = (opts.synth !== undefined) ? opts.synth : _knob_log_parse_synth(msg);
  if (opts.remove) {
    if (key !== undefined) window.knob_log.delete(key);
  } else if (key !== undefined && msg) {
    window.knob_log.set(key, { line: msg.replace(/Z+$/, ''), synth: synth });
  }
  if (!opts.silent && msg) {
    amy_add_log_message(_knob_log_ensure_z(msg));
  }
  schedule_knob_block_reflect();
  return msg;
}
window.send_amy_message_in_sketch = send_amy_message_in_sketch;

// Stable identity key for a knob. Global knobs (EQ/echo/reverb/etc. — change_code
// has no i%i prefix) are channel-independent; per-synth knobs are scoped by synth.
function knob_log_key_for_knob(synth, knob) {
  if (!knob) return undefined;
  var isGlobal = typeof knob.change_code !== 'string' || knob.change_code.indexOf('i%i') < 0;
  var who = (knob.section || '') + _KNOB_LOG_SEP + (knob.display_name || '');
  return isGlobal ? ('kg' + _KNOB_LOG_SEP + who)
                  : ('k' + _KNOB_LOG_SEP + synth + _KNOB_LOG_SEP + who);
}

// Record a knob's current value in the log (collapse by knob identity). This is
// RECORD-ONLY (silent): the live AMY send is done by send_change_code's existing
// path, so the funnel sends once and records once. Called from send_change_code,
// so it inherits the same suppress_knob_cc_send gating at every call site.
function record_knob_value(synth, value, knob) {
  if (!knob || typeof window.make_change_code !== 'function') return;
  if (knob.bus_fx) {
    // Bus-FX knob (EQ/Chorus/Reverb/Echo/bus level): the live send above went
    // out synth-prefixed (i<synth>x,,-3 — AMY routes it to the synth's bus),
    // but the LOG records the bus-explicit form (y<bus>x,,-3). A synth-keyed
    // log slot couldn't hold more than one bus's history: moving the channel
    // to another bus and tweaking the knob again would overwrite the old
    // bus's value in place, replaying against the wrong bus on boot.
    var bus = (typeof window.get_channel_bus === 'function') ? window.get_channel_bus(synth) : 0;
    var busCode = window.make_change_code(synth, value, knob, /* no_instrument */ true);
    if (!busCode) return;
    send_amy_message_in_sketch('y' + bus + busCode, { synth: null, silent: true });
    return;
  }
  var code = window.make_change_code(synth, value, knob);
  if (!code) return;
  var isGlobal = typeof knob.change_code === 'string' && knob.change_code.indexOf('i%i') < 0;
  // No explicit key: send_amy_message_in_sketch keys by the message's structural
  // key, which collapses repeat moves AND matches a line loaded from a sketch.
  send_amy_message_in_sketch(code, { synth: isGlobal ? null : synth, silent: true });
}
window.record_knob_value = record_knob_value;

// Log (and optionally remove) a knob's MIDI CC mapping line: i<synth>c<ccVal>.
function log_knob_cc(synth, knob, ccVal) {
  if (!knob || typeof knob.change_code !== 'string') return;
  if (!ccVal) {
    // Remove this knob's CC line: key it by the structural key of a representative
    // CC line for the knob (CC number/params normalized; change_code identifies it).
    send_amy_message_in_sketch('', {
      key: _knob_log_struct_key('i' + synth + 'ic0,0,0,0,0,' + knob.change_code),
      remove: true, silent: true,
    });
    return;
  }
  // The MIDI-CC mapping command is `ic` (amy_api midi_cc wire="ic"), NOT a bare
  // `c` — `ic` consumes the whole change_code (commas and all) as a string until
  // Z; a bare `c` does not, so the sketch fails to boot. Only block bookkeeping
  // here; the live ic send is done by the caller via amy_send({synth, midi_cc}).
  // Auto structural key collapses re-assigns and matches a CC line loaded from a sketch.
  send_amy_message_in_sketch('i' + synth + 'ic' + ccVal, { synth: synth, silent: true });
}
window.log_knob_cc = log_knob_cc;

// Delete all of a synth's lines (setup + knob + preset + cc), keeping globals.
function _knob_log_clear_synth(synth) {
  var doomed = [];
  window.knob_log.forEach(function(entry, key) {
    if (entry && entry.synth === synth) doomed.push(key);
  });
  for (var i = 0; i < doomed.length; i++) window.knob_log.delete(doomed[i]);
}

// Reset a synth in the block to the amyboard default (clear CC maps + default
// Juno patch, 6 voices) and send the same live. Used by channel-activate, the
// per-synth Clear button, and as the first step of a preset load.
function reset_synth_in_sketch(synth) {
  synth = Number(synth);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) return;
  _knob_log_clear_synth(synth);
  send_amy_message_in_sketch('i' + synth + 'ic255', { synth: synth });
  send_amy_message_in_sketch('i' + synth + 'K257iv6', { synth: synth });
  // K257iv6 re-creates the instrument, which puts it back on bus 0 (and the
  // synth's i<ch>y line was just cleared from the log) — mirror that.
  _reset_channel_bus_ui(synth);
}
window.reset_synth_in_sketch = reset_synth_in_sketch;

// True if the knob log already holds a patch (a K<n> setup line) for this
// synth — e.g. a preset was loaded while the channel's Active checkbox was off.
// CC-mapping lines (i<ch>ic...) are skipped: their change_code templates are
// not patch loads. A deactivated channel's log is just i<ch>iv0, so this is
// false after a user toggle-off.
function channel_has_patch_in_log(synth) {
  synth = Number(synth);
  if (!window.knob_log || typeof window.knob_log.forEach !== 'function') return false;
  var found = false;
  window.knob_log.forEach(function(entry) {
    if (found || !entry || Number(entry.synth) !== synth) return;
    var line = String(entry.line || '');
    if (/^i\d+ic/.test(line)) return;
    if (/K\d+/.test(line)) found = true;
  });
  return found;
}
window.channel_has_patch_in_log = channel_has_patch_in_log;

// Reset a channel's bus mirror to 0 and refresh the bus UI if it's on display.
function _reset_channel_bus_ui(synth) {
  if (typeof window.set_channel_bus_local === 'function') {
    window.set_channel_bus_local(synth, 0);
  }
  if (Number(window.current_synth || 1) === Number(synth)) {
    if (typeof window.syncBusSelectForChannel === 'function') window.syncBusSelectForChannel();
    if (typeof window.syncFxLevelForBus === 'function') window.syncFxLevelForBus();
  }
}

// Assign a channel (synth) to a mix bus: update the mirror, send + record
// i<ch>y<bus>, and re-point the Effects column at the new bus's knob state.
window.set_channel_bus = function(channel, bus, opts) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) return;
  bus = (typeof window.clamp_bus === 'function') ? window.clamp_bus(bus) : (Number(bus) || 0);
  opts = opts || {};
  if (typeof window.set_channel_bus_local === 'function') {
    window.set_channel_bus_local(ch, bus);
  }
  // Only send/record for an ACTIVE channel: AMY ignores a bus assignment for
  // a synth that doesn't exist, and a stray i<ch>y line in the block would
  // make an inactive channel read as active on the next load.
  if (!opts.silent && is_channel_active(ch)) {
    send_amy_message_in_sketch('i' + ch + 'y' + bus, { synth: ch });
  }
  if (Number(window.current_synth || 1) === ch) {
    // Re-render the Effects column (and its master Level) from the new bus's
    // mirrored knob state, without sending anything to AMY.
    if (typeof window.refresh_knobs_for_active_channel === 'function') {
      window.refresh_knobs_for_active_channel({ sendControlMappings: false });
    }
    if (typeof window.syncBusSelectForChannel === 'function') window.syncBusSelectForChannel();
    if (typeof window.syncFxLevelForBus === 'function') window.syncFxLevelForBus();
  }
};

// Remove a synth from the block and silence it. We log an explicit i<synth>iv0 so
// that on boot — where the device always starts a default synth on channel 1 —
// a removed channel 1 stays off (the block is applied on top of that default).
function remove_synth_from_sketch(synth) {
  synth = Number(synth);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) return;
  _knob_log_clear_synth(synth);
  send_amy_message_in_sketch('i' + synth + 'iv0', { synth: synth });
  _reset_channel_bus_ui(synth);
}
window.remove_synth_from_sketch = remove_synth_from_sketch;

// Record every set CC mapping for a channel into the log (silent). Knob VALUES
// are recorded automatically when send_all_knob_values() replays them through
// send_change_code; only the CC lines (sent via amy_send, not send_change_code)
// need this. Used when (re)activating a channel so its CC assignments persist.
function record_all_cc_for_channel(ch) {
  ch = Number(ch);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) return;
  if (typeof get_knobs_for_channel !== 'function' || typeof build_knob_cc_value !== 'function') return;
  var knobs = get_knobs_for_channel(ch);
  var disabled = window._disabled_sections || {};
  for (var i = 0; i < knobs.length; i++) {
    var knob = knobs[i];
    if (!knob || disabled[knob.section]) continue;
    var ccVal = build_knob_cc_value(knob);
    if (ccVal) log_knob_cc(ch, knob, ccVal);
  }
}
window.record_all_cc_for_channel = record_all_cc_for_channel;

// Record one preset wire command into the log (silent — the caller already sent
// it live via amy_send/amy_add_log_message). Auto structural key: distinct preset
// commands have distinct structures, and reset_synth_in_sketch() clears the synth
// before another preset loads. Used by the Load Preset handler.
function log_preset_message(synth, message) {
  if (!message) return;
  send_amy_message_in_sketch(message, { synth: synth, silent: true });
}
window.log_preset_message = log_preset_message;

// Reflect the in-memory log into the editor's _auto_generated_knobs block. Done
// on a short debounce so dragging a knob stays smooth; the code above the block
// (and the cursor/scroll) is preserved. The log — not this text — is canonical;
// Write re-splices from the log regardless.
var _knob_block_reflect_timer = null;
function schedule_knob_block_reflect() {
  if (typeof editor === 'undefined' || !editor || typeof editor.getValue !== 'function') return;
  if (_knob_block_reflect_timer) return;
  _knob_block_reflect_timer = setTimeout(function() {
    _knob_block_reflect_timer = null;
    reflect_knob_log_to_editor();
  }, 200);
}

// Run any pending knob-block reflect immediately (log -> editor). Called before
// Write so the editor's block includes the latest knob moves before we send it.
function flush_knob_block_reflect() {
  if (_knob_block_reflect_timer) {
    clearTimeout(_knob_block_reflect_timer);
    _knob_block_reflect_timer = null;
    reflect_knob_log_to_editor();
  }
}
window.flush_knob_block_reflect = flush_knob_block_reflect;

function reflect_knob_log_to_editor() {
  if (typeof editor === 'undefined' || !editor || typeof editor.getValue !== 'function') return;
  // Don't rewrite the buffer while the user is typing in it (would disrupt the
  // caret/undo). A later knob move re-schedules this once the editor loses focus,
  // and Write flushes it (flush_knob_block_reflect) before sending.
  if (typeof editor.hasFocus === 'function' && editor.hasFocus()) return;
  try {
    var cur = editor.getValue();
    var merged = splice_knobs_into_sketch(cur, serialize_knob_log());
    if (merged === cur) return;
    var pos = (typeof editor.getCursor === 'function') ? editor.getCursor() : null;
    var scroll = (typeof editor.getScrollInfo === 'function') ? editor.getScrollInfo() : null;
    editor.setValue(merged);
    if (pos && typeof editor.setCursor === 'function') editor.setCursor(pos);
    if (scroll && typeof editor.scrollTo === 'function') editor.scrollTo(scroll.left, scroll.top);
  } catch (e) {
    console.warn('reflect_knob_log_to_editor failed', e);
  }
}
window.reflect_knob_log_to_editor = reflect_knob_log_to_editor;

// Rebuild the in-memory log from a block of wire lines (verbatim). Used after
// Read / download / pageload so a subsequent Write re-emits exactly what was
// loaded, with later knob tweaks layered on top.
function rebuild_knob_log_from_block(blockText) {
  window.knob_log.clear();
  var lines = String(blockText || '').split(/\r?\n/);
  for (var i = 0; i < lines.length; i++) {
    var line = lines[i].trim();
    if (!line || line.charAt(0) === '#') continue;
    var clean = line.replace(/Z+$/, '');
    // Key by the structural key (same as live knob moves), so a later move
    // collapses onto the loaded line instead of duplicating it — and any
    // duplicate already in the loaded block self-heals to a single entry.
    window.knob_log.set(_knob_log_struct_key(clean), {
      line: clean,
      synth: _knob_log_parse_synth(line),
    });
  }
}
window.rebuild_knob_log_from_block = rebuild_knob_log_from_block;

// Position the visible knobs + active checkboxes from the sketch's knob block,
// WITHOUT sending anything to AMY, and rebuild the in-memory log. Parses both the
// new one-line-per-knob format and legacy combined-dump lines (apply_zd_dump_to_knobs).
// Decide which channels are active from a knob block, matching the device boot
// contract: a synth is active iff its last iv<N> in the block is N>0. Channels 1
// and 10 default to active when they have NO lines (the board always starts the
// default synth on channel 1 and the GM drum kit on channel 10 before applying
// the block); other channels default off when absent. So an empty block ⇒
// channels 1 and 10 active, and an explicit i<ch>iv0 ⇒ that channel off.
function _compute_active_from_block(block) {
  var voices = {};   // synth -> last num_voices seen
  var hasLines = {};
  var lines = String(block || '').split(/\r?\n/);
  for (var i = 0; i < lines.length; i++) {
    var line = lines[i].trim();
    if (!line || line.charAt(0) === '#') continue;
    var m = /^i(\d+)/.exec(line);
    if (!m) continue;  // global effect line (no i prefix) — not a synth
    var synth = parseInt(m[1], 10);
    if (synth < 1 || synth > 16) continue;
    hasLines[synth] = true;
    // num_voices (iv<N>) only appears in a synth's SETUP line (i<ch>K257iv6,
    // i<ch>iv0) — never in a CC-mapping line, where any "iv<N>" is inside the
    // change_code template (e.g. i%iv0F, i%iv2f) and must NOT be read as
    // num_voices, or a channel with CC maps would look deactivated.
    if (/^i\d+ic/.test(line)) continue;  // CC mapping — skip the num_voices scan
    var iv = line.match(/iv(\d+)/g);  // num_voices token (e.g. iv6, iv0)
    if (iv && iv.length) voices[synth] = parseInt(iv[iv.length - 1].slice(2), 10);
  }
  var active = new Array(17).fill(false);
  for (var ch = 1; ch <= 16; ch++) {
    if (voices[ch] !== undefined) active[ch] = voices[ch] > 0;
    else if (hasLines[ch]) active[ch] = true;   // lines but no iv -> assume active
    else active[ch] = (ch === 1 || ch === window.DRUM_CHANNEL);  // ch 1 (K257) + ch 10 (drums) on by default at boot
  }
  return active;
}

function set_knobs_from_sketch() {
  if (typeof editor === 'undefined' || !editor || typeof editor.getValue !== 'function') return;
  var block = extract_knobs_from_sketch(editor.getValue());
  rebuild_knob_log_from_block(block);
  // Compute boot-aware active state FIRST (an empty/default block keeps channel 1
  // on; i<ch>iv0 reads as off). apply_zd_dump_to_knobs reads window.active_channels
  // to decide which synth to position, so a default channel 1 with no explicit
  // lines still gets positioned to the K257 baseline.
  var active = _compute_active_from_block(block);
  if (!Array.isArray(window.active_channels)) window.active_channels = new Array(17).fill(false);
  for (var ch = 1; ch <= 16; ch++) window.active_channels[ch] = active[ch];
  if (typeof apply_zd_dump_to_knobs === 'function') {
    // Suppress live sends AND log recording while we position the UI: set_knobs_from_events
    // → set_amy_knob_value → send_change_code would otherwise re-send every value to AMY
    // and re-record it. Reading a sketch only moves knobs; Write/restart makes sound.
    var prev = window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = true;
    try {
      apply_zd_dump_to_knobs(block);
    } finally {
      window.suppress_knob_cc_send = prev;
    }
  }
  var cb = document.getElementById('channel-active-checkbox');
  if (cb) cb.checked = !!active[Number(window.current_synth || 1)];
  _last_synced_editor_block = block;  // log is now in sync with this block
}
window.set_knobs_from_sketch = set_knobs_from_sketch;

// --- Editor edits -> knob log (the reverse of reflect_knob_log_to_editor) ------
// When the USER pastes or hand-edits the sketch, rebuild the knob log from the
// editor's block so the log stays the source of truth. Without this, Write (and
// the first knob move after a paste) would splice a stale/empty log over the
// user's block and wipe it. Gated on the CodeMirror change origin (so our own
// programmatic setValue/reflect doesn't loop) + a block-changed check (so typing
// CODE above the block doesn't churn the knob grid).
var _last_synced_editor_block = null;
var _editor_edit_sync_timer = null;
function _rebuild_log_from_editor_now() {
  if (typeof editor === 'undefined' || !editor || typeof extract_knobs_from_sketch !== 'function') return;
  var block = extract_knobs_from_sketch(editor.getValue());
  if (block === _last_synced_editor_block) return;  // block unchanged (code-only edit)
  // set_knobs_from_sketch rebuilds the log from the editor's block AND repositions
  // the knobs to match, then updates _last_synced_editor_block.
  set_knobs_from_sketch();
}
function schedule_log_rebuild_from_editor() {
  if (_editor_edit_sync_timer) clearTimeout(_editor_edit_sync_timer);
  _editor_edit_sync_timer = setTimeout(function() {
    _editor_edit_sync_timer = null;
    _rebuild_log_from_editor_now();
  }, 300);
}
function flush_log_rebuild_from_editor() {
  if (_editor_edit_sync_timer) {
    clearTimeout(_editor_edit_sync_timer);
    _editor_edit_sync_timer = null;
    _rebuild_log_from_editor_now();
  }
}
window.schedule_log_rebuild_from_editor = schedule_log_rebuild_from_editor;
window.flush_log_rebuild_from_editor = flush_log_rebuild_from_editor;

// The K257 "amyboard default" patch as an AMY wire string is the implicit
// baseline every synth starts from (the device loads K257 before applying the
// knob block — see amyboard.py). We use only its oscillator + global-FX setup
// (everything before the first `ic` MIDI-CC mapping) to position the knobs; the
// ic mappings carry %v/%i change_code templates that aren't real wire commands,
// and the knobs already hold their default CC numbers from amy_parameters.js.
// Sourced at runtime from patch_code_for_patch_number[257] (generated from
// amy/src/patches.h), so it stays in sync with the amy pin.
var _k257_baseline_cache = null;
function k257_default_wire_baseline() {
  if (_k257_baseline_cache) return _k257_baseline_cache;
  var codes = window.patch_code_for_patch_number;
  var s = (codes && typeof codes[257] === 'string') ? codes[257] : '';
  if (!s) return '';                    // patches not loaded yet — don't cache empty
  var idx = s.indexOf('Zic');           // first ic MIDI-CC mapping
  _k257_baseline_cache = (idx >= 0) ? s.slice(0, idx + 1) : s;
  return _k257_baseline_cache;
}
window.k257_default_wire_baseline = k257_default_wire_baseline;

// A "drum patch" is any of the GM drum kits (K384+, named "drum kit N ...") or
// the pcm_tiny ROM drums (K258, "MIDI drums ..."). Detected from the generated
// patch-name table so a new kit added in amy is picked up automatically. The
// prefixes can't collide with preset names ("Juno A32 Steel Drums",
// "DX7 STEEL DRUM") because those all start with "Juno "/"DX7 ".
function is_drum_patch(patchNumber) {
  var pn = Number(patchNumber);
  if (!Number.isInteger(pn) || pn < 0) return false;
  var name = (window.amy_patches && typeof window.amy_patches[pn] === 'string') ? window.amy_patches[pn] : '';
  return /^(drum kit|MIDI drums)/.test(name);
}
window.is_drum_patch = is_drum_patch;

// Grey out the knob sections that don't apply to a channel's patch:
// - DX7 presets (K128-255): Osc A/B + ADSR (per-voice FM oscs/EGs the analog
//   knobs can't represent; the VCF coda still applies).
// - Drum patches: every per-channel section — a drum kit maps each MIDI note to
//   its own preconfigured sample, so only the FX knobs (per-bus, in the global
//   grid) and the dedicated Level slider still make sense.
// The FX sections and the Level slider (section "Synth"/"Bus") are never touched.
var DX7_DISABLED_SECTIONS = { "Osc A": true, "Osc B": true, "ADSR": true };
var CHANNEL_KNOB_SECTIONS = ["Osc A", "Osc B", "ADSR", "VCF", "VCF ENV", "LFO"];
function update_knob_sections_for_patch(patchNumber) {
  if (typeof window.set_section_disabled !== 'function') return;
  var pn = Number(patchNumber);
  var isDX7 = Number.isInteger(pn) && pn >= 128 && pn <= 255;
  var isDrums = is_drum_patch(pn);
  for (var i = 0; i < CHANNEL_KNOB_SECTIONS.length; i++) {
    var section = CHANNEL_KNOB_SECTIONS[i];
    window.set_section_disabled(section, isDrums || (isDX7 && !!DX7_DISABLED_SECTIONS[section]));
  }
}
window.update_knob_sections_for_patch = update_knob_sections_for_patch;

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

window.set_channel_active = function(channel, active, opts) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) {
    return;
  }
  opts = opts || {};
  var wasActive = !!window.active_channels[ch];
  window.active_channels[ch] = !!active;

  // The user toggled this channel's "Active" checkbox.
  if (opts.userToggle) {
    if (active && !wasActive) {
      // SYNC 2: activating a channel loads the amyboard default patch (K257) on
      // it — that single command configures the synth AND reinstalls its default
      // CC map on the device. We do NOT replay the channel's knob values/CC maps:
      // those are generic UI defaults (SINE, f440, …) that would override K257
      // and silence the Juno (the bug where activating ch2 dumped ~50 commands).
      // Instead we reset to the K257 default and position the UI knobs to match,
      // like startup. This applies to channel 10 too: its GM-drum default is a
      // BOOT-only default (the device and simulator boot with i10K384iv6) —
      // once the user touches the channel we never re-assume drums; activating
      // an empty channel 10 loads K257 like any other channel. And if the
      // channel already has a patch in the log (e.g. a preset loaded while the
      // checkbox was off), don't overwrite it — just mark it active.
      if (!channel_has_patch_in_log(ch)) {
        reset_synth_in_sketch(ch);          // sends + records i<ch>ic255, i<ch>K257iv6
      }
      position_current_channel_from_log();  // UI knobs -> patch state (no AMY send)
    } else if (!active && wasActive) {
      // Disable: tear the synth down so it stops playing and frees its voices,
      // and remove it from the block (with an explicit i<ch>iv0 so a removed
      // channel 1 stays off past the boot-time default synth — see amyboard.py).
      remove_synth_from_sketch(ch);       // records i<ch>iv0 + sends num_voices=0
    }
    return;
  }

  // Other callers (startup, sketch/dump load): initialize a newly activated
  // channel with the default patch; never tear down. (Unchanged behavior.)
  if (active && !wasActive) {
    if (ch === window.DRUM_CHANNEL) {
      // Drum default, exactly as the device boot does. No CC-map flood: the
      // kit has no knob-controllable params (see update_knob_sections_for_patch).
      amy_add_log_message("i" + ch + "K" + window.DRUM_DEFAULT_PATCH + "iv6Z");
    } else {
      amy_add_log_message("i" + ch + "ic255Z");
      amy_add_log_message("i" + ch + "K257iv6Z");
      send_all_knob_cc_mappings(ch);
    }
  }
};

// ── AMY shared file I/O hooks (called from AMY WASM via EM_ASM) ─────────────
// These bridge AMY's C file operations to the MicroPython Emscripten filesystem.
var _amy_shared_files = {};  // handle → { filename, mode, chunks }
var _amy_shared_next_handle = 1;

window.amy_shared_open = function(filename, mode) {
    var handle = _amy_shared_next_handle++;
    _amy_shared_files[handle] = { filename: filename, mode: mode || 'r', chunks: [] };
    return handle;
};

window.amy_shared_write = function(handle, bufPtr, len) {
    var file = _amy_shared_files[handle];
    if (!file) return 0;
    // Read bytes from AMY WASM heap.
    var bytes = new Uint8Array(amy_module.HEAPU8.buffer, bufPtr, len);
    file.chunks.push(new Uint8Array(bytes));  // copy
    return len;
};

window.amy_shared_close = function(handle) {
    var file = _amy_shared_files[handle];
    if (!file) return;
    delete _amy_shared_files[handle];
    // If opened for writing, concatenate chunks and write to mp.FS.
    if (file.mode && file.mode.indexOf('w') !== -1 && mp) {
        var totalLen = 0;
        for (var i = 0; i < file.chunks.length; i++) totalLen += file.chunks[i].length;
        var combined = new Uint8Array(totalLen);
        var offset = 0;
        for (var i = 0; i < file.chunks.length; i++) {
            combined.set(file.chunks[i], offset);
            offset += file.chunks[i].length;
        }
        var content = new TextDecoder().decode(combined);
        // Map AMY paths to mp.FS paths (prepend /amyboard if needed for simulate).
        var fsPath = file.filename;
        if (amyboard_mode === 'simulate' && !fsPath.startsWith('/amyboard')) {
            fsPath = '/amyboard' + fsPath;
        }
        try {
            mp.FS.writeFile(fsPath, content);
            sync_persistent_fs();
            fill_tree();
        } catch (e) {
            console.warn('amy_shared_close: failed to write', fsPath, e);
        }
    }
};

// Once AMY module is loaded, register its functions and start AMY (not yet audio, you need to click for that)
// In control mode the WASM script tag is omitted so amyModule won't exist.
// Store the promise so start_amyboard() can await it before sending init messages.
var _amy_wasm_ready = null;
if (typeof amyModule === 'function') _amy_wasm_ready = amyModule().then(async function(am) {
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
  amy_dump_state_to_string_c = am.cwrap(
    'amy_dump_state_to_string', 'number', ['number']
  );
  amy_start_web_no_synths();
  amy_module = am;
  res_ptr_in = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  res_ptr_out = amy_module._malloc(2 * 256 * 2); // 2 channels, 256 frames, int16s
  // Expose on globalThis so MicroPython JS module calls can access them
  globalThis._amy_module_ref = amy_module;
  globalThis._amy_res_ptr_out = res_ptr_out;
});

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

function get_wire_commands_for_juno_patch(patch) {
  // The new convention for AMYboard voice osc usage.
  const CTL_OSC = 0;  // Osc with filter & env commands.
  const LFO_OSC = 1;
  const OSCA_OSC = 2;
  const OSCB_OSC = 3;

  function translate_juno_events_to_webeditor_wire_commands(events) {
    let filterFreq = null;
    let filterEnv = null;
    let filterLfo = null
    let filterKbd = null;
    let filter_osc = 0;  // The osc with the filter.  It's always 0.
    let resonanceValue = null;
    let adsr = [0, 0, 1, 0];
    let f_adsr = [0, 0, 1, 0];
    let lfoFreq = null;
    let lfoDelay = null;
    let lfoWave = 4;  // always triangle
    let lfoOsc = 0;
    let lfoPwm = 0;
    let vcaGain = 1.0;
    // Track coefficients for all 5 oscs.  Not sure which one is LFO.
    let osc_freq = [null, null, null, null, null, null];
    let osc_wave = [0, 0, 0, 0, 0, 0];
    let osc_duty = [0.5, 0.5, 0.5, 0.5, 0.5, 0.5];
    let osc_gain = [0, 0, 0, 0, 0, 0];
    let mod_source_osc = LFO_OSC;  // Start with Juno LFO osc, but may get updated if it's an amyboardsynth patch.
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
        if (Number.isFinite(event.chorus[0])) { chorus[0] = event.chorus[0]; }  // level
        if (Number.isFinite(event.chorus[2])) { chorus[1] = event.chorus[2]; }  // lfo_freq
        if (Number.isFinite(event.chorus[3])) { chorus[2] = event.chorus[3]; }  // depth
      }

      // Remainder of block assumes osc is set.
      if (!Number.isFinite(event.osc)) continue;

      if (event.osc == CTL_OSC) {
        if (event.filter_freq) {
          if (Number.isFinite(event.filter_freq[0])) {
            filter_osc = event.osc;  // Assume we'll see this at least once
            filterFreq = event.filter_freq[0];
          }
          if (Number.isFinite(event.filter_freq[1])) {
            filterKbd = event.filter_freq[1];  // COEF_NOTE
          }
          if (Number.isFinite(event.filter_freq[3]) && event.filter_freq[3] > 0) {
            filterEnv = event.filter_freq[3];  // COEF_EG0, Juno filter env
          }
          if (Number.isFinite(event.filter_freq[4]) && event.filter_freq[4] > 0) {
            filterEnv = event.filter_freq[4];  // COEF_EG1, Juno filter env, gate mode
            oscGate = 1;
          }
          if (Number.isFinite(event.filter_freq[5])) {
            filterLfo = event.filter_freq[5];  // COEF_MOD
          }
        }
        if (Number.isFinite(event.resonance)) {
          resonanceValue = event.resonance;
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
        if (event.amp && Number.isFinite(event.amp[0])) {
          // CtlOsc only gets vca gain applied to velocity (scales all other oscs).
          vcaGain = event.amp[0];
        }
      } else if (event.osc == mod_source_osc) {
        // LFO
        if (event.freq && Number.isFinite(event.freq[0])) {
          lfoFreq = event.freq[0];
        }
        if (event.wave >= 0 && event.wave < 127) {
          lfoWave = event.wave;
        }
        if (event.eg0_times && bpTimeIsSet(event.eg0_times[0])) {
          lfoDelay = event.eg0_times[0];
        }
      } else if (event.osc >= 0) {
        // Non-CTL, non-LFO osc, don't assume what order they come in.
        const parsedModSource = Number(event.mod_source);
        if (Number.isInteger(parsedModSource) && parsedModSource >= 0 && parsedModSource < 64) {
          mod_source_osc = parsedModSource;  // Should never change the original value.
        }
        // Extract key parameters for each osc
        if (event.amp) {
          if (Number.isFinite(event.amp[0])) {
            osc_gain[event.osc] = event.amp[0];
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
      }
    }

    if (oscGate == 1) {
      adsr = [0, 0, 1, 0];
    } else {
      f_adsr = adsr;
    }
    // Logic to choose juno oscs for osc A and osc B
    // among original oscs 2 (pulse), 3 (saw), 4 (subosc), and 5 (noise).
    if (osc_gain[4] == 0 && osc_gain[5] == 0) {
      // Only 2 oscs, let them be
      oscAB_osc[0] = 2;
      oscAB_osc[1] = 3;
    } else {
      for (let osc = 2; osc < 6; ++osc) {
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
    // Helper: null/undefined → "" for safe wire code concatenation.
    const s = (v) => v != null ? v : "";

    const AMY_OSC_OF_LOGICAL_OSC = [OSCA_OSC, OSCB_OSC];
    wire_commands = [];
    // Osc 0 has the filter and envelope controls, including both EG0 (vca) and EG1 (vcf)
    let command = "v" + CTL_OSC + "w" + AMY.SILENT + "L" + LFO_OSC + "c" + OSCA_OSC;
    command += "G" + AMY.FILTER_LPF24;
    command += "a1,0,1,1,0" + "A" + adsr[0] + ",1," + adsr[1] + "," + adsr[2] + "," + adsr[3] + ",0"
    if (resonanceValue != null) command += "R" + resonanceValue;
    command += "F" + s(filterFreq) + "," + s(filterKbd) + ",,," + s(filterEnv) + "," + s(filterLfo);
    command += "B" + f_adsr[0] + ",1," + f_adsr[1] + "," + f_adsr[2] + "," + f_adsr[3] + ",0";
    wire_commands.push(command + "Z");
    for (const osc of [0, 1]) {
      let src_osc = oscAB_osc[osc];
      let amy_osc = AMY_OSC_OF_LOGICAL_OSC[osc];  // too many osc numbers.
      let command = "v" + amy_osc + "w" + osc_wave[src_osc] + "L" + LFO_OSC
          + "f" + s(osc_freq[src_osc]) + ",1,,,," + lfoOsc + ",1"
          + "d" + osc_duty[src_osc] + ",,,,," + lfoPwm
          + "a" + vcaGain * osc_gain[src_osc] + ",,0,0";
      if (osc == 0) {
        // Osc 0 must chain to osc 1
        command += "c" + AMY_OSC_OF_LOGICAL_OSC[1];
      }
      wire_commands.push(command + "Z");
    }
    // LFO command
    command = "v" + LFO_OSC + "w" + lfoWave + "a1,0,0,1" + "f" + s(lfoFreq) + ",0,,0,0,0,0"
        + "A" + s(lfoDelay) + ",1,100,1,10000,0";
    wire_commands.push(command + "Z");
    // Global FX commands
    command = "x" + s(eq[0]) + "," + s(eq[1]) + "," + s(eq[2])
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

// Canonical JS bridge for AMY's get_synth_commands. The C convenience wrapper
// exists for CPython (amy/src/pyamy.c) and for MicroPython as
// tulip.amy_get_synth_commands (modtulip.c) -- but the MicroPython one is compiled
// out on web (#ifndef __EMSCRIPTEN__) because here micropython does not link AMY;
// AMY runs in a separate WASM worklet. So we bridge it from JS instead, driving
// AMY's exported low-level generator yield_synth_commands. Reads back the wirecode
// commands that reconstruct synth `synth` (1..16) from AMY's current state and
// returns them as an array of strings (empty if the synth has no state).
// include_fx (default true) also emits the global FX commands. Throws if AMY's
// WASM module is not loaded or `synth` is out of range.
function get_synth_commands(synth, include_fx = true) {
  const s = Number(synth);
  if (!Number.isInteger(s) || s < 1 || s > 16) {
    throw new Error("get_synth_commands: synth must be an integer 1..16.");
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

// Editor helper: get_synth_commands for a UI channel (1..16), but treats an empty
// result as an error (a configured channel always yields at least one command).
function get_wire_commands_for_channel(channel) {
  const synth = normalize_synth_channel(channel);
  if (!synth) {
    throw new Error("Invalid channel.");
  }
  const lines = get_synth_commands(synth, true);
  if (!lines.length) {
    throw new Error("No synth commands were generated for this channel.");
  }
  return lines;
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
  // In control mode, AMY WASM isn't loaded so we can't read synth state back.
  if (amyboard_mode === 'control') {
    return false;
  }
  // Audio must be running for AMY to process queued messages; skip if not started.
  if (!audio_started) {
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
    for (let attempt = 0; attempt < 5; attempt += 1) {
      // AMY applies queued wire messages asynchronously; keep retrying briefly during startup.
      await new Promise(function(resolve) { setTimeout(resolve, attempt === 0 ? 50 : 200); });
      try {
        set_knobs_from_synth(synth);
        window.refresh_knobs_for_channel();
        return true;
      } catch (e) {
        lastError = e;
        // If the synth has no voices, don't keep retrying — it won't appear.
        if (e && e.message && e.message.indexOf('No synth commands') !== -1) break;
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

function schedule_dirty_autosave_for_channel(channel) { /* no-op */ }




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


window.clear_current_channel_patch = async function() {
  if (mp) ensure_current_environment_layout(true);
  const synth = Number(window.current_synth || 1);
  if (!Number.isInteger(synth) || synth < 1 || synth > 16) {
    throw new Error("Invalid channel.");
  }
  // SYNC 2: reset this channel to the default patch (K257 — even on channel 10,
  // whose drum default is boot-only) and position the UI knobs to its defaults —
  // no knob-value/CC flood (K257 reinstalls the channel's default CC map on the
  // device), no zA AMY-state pull. position_current_channel_from_log also
  // greys/ungreys the knob sections for the reloaded patch.
  reset_synth_in_sketch(synth);          // sends + records i<synth>ic255, K257iv6
  // Clear on an inactive channel just sent its default patch live — the channel
  // is now audibly playing, so mark it active (the checkbox syncs from
  // active_channels inside position_current_channel_from_log).
  if (Array.isArray(window.active_channels)) window.active_channels[synth] = true;
  position_current_channel_from_log();   // UI knobs -> patch defaults (no AMY send)
  if (amyboard_mode !== 'control') {
    reset_global_effects();
  }
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
  var synthChannel = Number(window.current_synth || 1);
  if (!Number.isInteger(synthChannel) || synthChannel < 1 || synthChannel > 16) {
    synthChannel = 1;
  }

  // Deduplicate CC numbers: if the new CC is already assigned to a different
  // knob on the same channel, unset that older knob (make its UI blank and
  // skip it in ic output). AMY itself only keeps one mapping per CC so the
  // newest ic replaces the old one — we just need to clean up knob state.
  var newCcNum = Number(knob.cc);
  var hasNewCc = knob.cc !== "" && knob.cc !== null && knob.cc !== undefined
    && Number.isInteger(newCcNum) && newCcNum >= 0 && newCcNum <= 127;
  if (hasNewCc) {
    var allKnobs = get_knobs_for_channel(synthChannel);
    for (var i = 0; i < allKnobs.length; i++) {
      var other = allKnobs[i];
      if (!other || other === knob) continue;
      var otherCcNum = Number(other.cc);
      if (other.cc !== "" && other.cc !== null && other.cc !== undefined
        && Number.isInteger(otherCcNum) && otherCcNum === newCcNum) {
        other.cc = "";
      }
    }
  }

  var ccVal = build_knob_cc_value(knob);
  if (window.suppress_knob_cc_send) {
    return;
  }
  // If the CC number changed and the previous CC isn't being re-used by the
  // same knob, tell AMY to unset the old CC → parameter mapping.
  if (previousCc !== undefined && previousCc !== "" && previousCc !== null
    && Number(previousCc) !== newCcNum) {
    amy_send({synth: synthChannel, midi_cc: previousCc + ",0,0,0,0,"}, true);
  }
  if (ccVal) {
    amy_send({synth: synthChannel, midi_cc: ccVal}, true);
  }
  // SYNC 2: record (or remove) this knob's CC mapping in the knob log.
  if (typeof window.log_knob_cc === "function") {
    window.log_knob_cc(synthChannel, knob, ccVal);
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
  // Set the JS globals that amy.wasm reads in its audio loop
  if (index === 0) cv_1_voltage = value;
  else if (index === 1) cv_2_voltage = value;
  // Also update the Python/MicroPython side
  if (mp) mp.runPythonAsync("tulip.cv_local(" + index + ", " + value + ")");
};

// --- Rotary encoder emulation (24 pulses, wraps 0-23) ---
(function() {
  var encoderPos = 0;
  var ENCODER_STEPS = 24;
  var valueEl = null;

  function updateDisplay() {
    if (!valueEl) valueEl = document.getElementById('encoder-value');
    if (valueEl) valueEl.textContent = encoderPos;
  }

  function encoderTurn(delta) {
    encoderPos = ((encoderPos + delta) % ENCODER_STEPS + ENCODER_STEPS) % ENCODER_STEPS;
    updateDisplay();
    if (mp) mp.runPythonAsync("amyboard._web_encoder_turn(" + delta + ")");
  }

  function encoderPress(down) {
    if (mp) mp.runPythonAsync("amyboard._web_encoder_press(" + (down ? "True" : "False") + ")");
  }

  document.addEventListener('DOMContentLoaded', function() {
    var leftBtn = document.getElementById('encoder-left');
    var rightBtn = document.getElementById('encoder-right');
    var pushBtn = document.getElementById('encoder-push');
    if (leftBtn) leftBtn.addEventListener('click', function() { encoderTurn(-1); });
    if (rightBtn) rightBtn.addEventListener('click', function() { encoderTurn(1); });
    if (pushBtn) {
      pushBtn.addEventListener('mousedown', function() {
        pushBtn.classList.add('pressed');
        encoderPress(true);
      });
      pushBtn.addEventListener('mouseup', function() {
        pushBtn.classList.remove('pressed');
        encoderPress(false);
      });
      pushBtn.addEventListener('mouseleave', function() {
        if (pushBtn.classList.contains('pressed')) {
          pushBtn.classList.remove('pressed');
          encoderPress(false);
        }
      });
    }
  });
})();

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

function reset_global_effects(bus) {
  // Reset one mix bus's FX knobs (and master volume) to defaults in AMY and
  // refresh the UI. Defaults to the bus the current channel is on.
  if (bus === undefined && typeof window.get_channel_bus === "function") {
    bus = window.get_channel_bus(window.current_synth || 1);
  }
  bus = (typeof window.clamp_bus === "function") ? window.clamp_bus(bus) : (Number(bus) || 0);
  var busKnobs = typeof window.get_bus_knobs === "function"
    ? window.get_bus_knobs(bus) : [];
  for (var i = 0; i < busKnobs.length; i++) {
    var knob = busKnobs[i];
    if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half") {
      continue;
    }
    knob.default_value = (knob.amy_default !== undefined) ? knob.amy_default : 0;
    // Send bus-addressed (y<bus>...) so the reset hits this bus regardless of
    // which channel is current.
    var payload = (typeof make_bus_change_code === "function")
      ? make_bus_change_code(bus, knob.default_value, knob)
      : null;
    if (payload) {
      amy_add_log_message(payload);
    }
    if (knob.dedicated_slider && typeof window.setBusVolumeFromAmy === "function") {
      window.setBusVolumeFromAmy(bus, knob.default_value);
    }
  }
  // Drop this bus's FX lines from the knob log: after the reset the bus is at
  // its boot defaults, which is exactly what an absent line replays to. (Keys
  // for bus FX lines are structural keys prefixed y<bus> — see record_knob_value.)
  var doomed = [];
  window.knob_log.forEach(function(entry, key) {
    if (typeof key === 'string' && key.indexOf('y' + bus) === 0) doomed.push(key);
  });
  for (var d = 0; d < doomed.length; d++) window.knob_log.delete(doomed[d]);
  if (doomed.length) schedule_knob_block_reflect();
  if (typeof window.refresh_knobs_for_channel === "function") {
    var previousSuppress = !!window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = true;
    window.refresh_knobs_for_channel();
    window.suppress_knob_cc_send = previousSuppress;
  }
}

async function apply_default_patch_state_from_clear_patches() {
  if (typeof window.reset_amy_knobs_to_defaults !== "function") {
    return false;
  }
  window.reset_amy_knobs_to_defaults();
  reset_global_effects();
  if (typeof window.set_channel_active === "function") {
    window.set_channel_active(1, true);
    for (let ch = 2; ch <= 16; ch += 1) {
      window.set_channel_active(ch, ch === window.DRUM_CHANNEL);
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
  if (typeof window.get_channel_knobs === "function" && typeof window.get_bus_knobs === "function"
    && typeof window.get_channel_bus === "function") {
    // FX knobs come from the bus THIS channel is on (not the current channel's).
    return window.get_channel_knobs(ch).concat(window.get_bus_knobs(window.get_channel_bus(ch)));
  }
  if (typeof window.get_channel_knobs === "function" && typeof window.get_global_knobs === "function") {
    return window.get_channel_knobs(ch).concat(window.get_global_knobs());
  }
  return window.get_current_knobs ? window.get_current_knobs() : [];
}

function build_knob_cc_value(knob) {
  if (!knob || knob.knob_type === "spacer" || knob.knob_type === "spacer-half"
    || knob.knob_type === "pushbutton") {
    return "";
  }
  // Skip knobs with no CC assigned (blank/null/undefined/non-integer or out of range).
  var ccNum = Number(knob.cc);
  if (knob.cc === "" || knob.cc === null || knob.cc === undefined
    || !Number.isInteger(ccNum) || ccNum < 0 || ccNum > 127) {
    return "";
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
  return ccNum + "," + log + "," + min_val + "," + max_val + "," + offset + "," + knob.change_code;
}

function send_all_knob_cc_mappings(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) ch = 1;
  var knobs = get_knobs_for_channel(ch);
  var disabledSections = window._disabled_sections || {};
  for (var i = 0; i < knobs.length; i++) {
    if (knobs[i] && disabledSections[knobs[i].section]) continue;
    var ccVal = build_knob_cc_value(knobs[i]);
    if (ccVal) {
      amy_send({synth: ch, midi_cc: ccVal}, true);
    }
  }
  if (Array.isArray(window.channel_control_mapping_sent)) {
    window.channel_control_mapping_sent[ch] = true;
  }
}

// Push every knob's current value for a channel to AMY by replaying its
// change_code — the exact wire command a knob-turn sends. Used when re-enabling
// a channel: after the synth is rebuilt we re-apply the user's existing knob
// values so the channel sounds the way it did, instead of the bare default patch.
// The dedicated "level" slider is skipped here: its value lives outside the knob
// grid (_synthLevels) and is restored separately by the Active-checkbox handler.
function send_all_knob_values(channel) {
  var ch = Number(channel);
  if (!Number.isInteger(ch) || ch < 1 || ch > 16) ch = 1;
  if (typeof send_change_code !== "function") return;
  var knobs = get_knobs_for_channel(ch);
  var disabledSections = window._disabled_sections || {};
  for (var i = 0; i < knobs.length; i++) {
    var knob = knobs[i];
    if (!knob || knob.dedicated_slider) continue;
    if (disabledSections[knob.section]) continue;
    if (knob.knob_type === "spacer" || knob.knob_type === "spacer-half"
      || knob.knob_type === "pushbutton") continue;
    var value = Number(knob.default_value);
    if (!Number.isFinite(value)) continue;
    send_change_code(ch, value, knob);
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


// AMY's main-thread WASM module calls this (via EM_ASM in
// amy_event_midi_message_received) for every NON-sysex MIDI message it parses
// out of amy_process_single_midi_byte(). We deliberately do NOT forward to
// mp.midiInHook here: the "midimessage" listener in setup_midi_devices() below
// already forwards every complete message (sysex included) to MicroPython
// explicitly. Forwarding here as well delivered every non-sysex message to
// Python twice — the "MIDI CC repeated 2x" bug. Kept as a no-op (not removed)
// so AMY's `typeof amy_external_midi_input_js_hook === 'function'` check still
// passes. NOTE: tulip/web (not this file) still relies on this hook as its only
// path, so don't "fix" it the same way there.
function amy_external_midi_input_js_hook(bytes, len, sysex) { /* no-op: see comment above */ }

// Inject raw MIDI bytes from on-page UI (the soft keyboard) into AMY exactly
// as if they had arrived on a real MIDI input. This does NOT use WebMIDI, so
// it works on Safari/Firefox. Simulate mode: feed the bytes into the local
// AMY WASM MIDI parser and forward the complete message to MicroPython's
// midi callbacks — the same two paths the hardware "midimessage" listener in
// setup_midi_devices() takes. Control mode: send the bytes out the selected
// MIDI output so the AMYboard parses them like any other MIDI input.
function inject_midi_bytes(bytes) {
  if (!bytes || bytes.length === 0) return;
  if (amyboard_mode === 'control') {
    var out = get_selected_midi_output_device() || midiOutputDevice;
    if (out && typeof out.send === "function") {
      try { out.send(bytes); } catch (e) {}
    }
    return;
  }
  if (!audio_started || typeof amy_process_single_midi_byte !== "function") return;
  for (var i = 0; i < bytes.length; i++) {
    amy_process_single_midi_byte(bytes[i], 1);
  }
  if (typeof mp !== 'undefined' && mp && mp.midiInHook) {
    var sysex = bytes[0] === 0xF0 ? 1 : 0;
    mp.midiInHook(new Uint8Array(bytes), bytes.length, sysex);
  }
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


var _raw_sysex_listener = null;
var _raw_sysex_input = null;
// Reassembly buffer for sysex messages that arrive across multiple events
// (some browsers/drivers fragment large sysex). `_sysex_reasm` accumulates
// bytes from an 0xF0 up to the matching 0xF7, then dispatches the complete
// message.
var _sysex_reasm = null;

// Application-level reassembly across MULTIPLE sysex frames. AMY's
// _send_as_sysex_b64 chunks large dumps into multiple sysex messages of
// ~1 KB each because Chrome's Web MIDI often drops single sysex messages
// above a few KB. Each chunk payload starts with a 1-byte marker:
//   '0' (0x30) = single chunk (whole message in one frame)
//   'C' (0x43) = continue (more chunks follow)
//   'E' (0x45) = end (last chunk of a multi-chunk message)
// We accumulate the base64 text from 'C' chunks here and flush on 'E' or
// single '0'. The accumulator is reset whenever a new chunk protocol frame
// arrives, and also at the start of each sync_amy_state.
var _sysex_b64_accum = '';

function _process_complete_sysex(d) {
    if (!d || d.length < 5) return;
    if (d[0] !== 0xF0 || d[d.length - 1] !== 0xF7) return;
    // SPSS (AMYboard) sysex manufacturer id: 00 03 45
    if (d[1] !== 0x00 || d[2] !== 0x03 || d[3] !== 0x45) {
        console.log('sysex mfr mismatch: d[1]=' + d[1] + ' d[2]=' + d[2] + ' d[3]=' + d[3]);
        return;
    }
    // zI ping reply: F0 00 03 45 'O' 'K' F7
    if (d.length === 7 && d[4] === 0x4F /* O */ && d[5] === 0x4B /* K */) {
        console.log('zI: board ready');
        if (_ping_resolve) { var r = _ping_resolve; _ping_resolve = null; _ping_reject = null; r(); }
        return;
    }
    // Sysex ACK: F0 00 03 45 'A' 'K' F7
    if (d.length === 7 && d[4] === 0x41 /* A */ && d[5] === 0x4B /* K */) {
        if (_sysex_ack_resolve) { var r = _sysex_ack_resolve; _sysex_ack_resolve = null; r(); }
        return;
    }
    // Sketch error report from the device: F0 00 03 45 'X' <base64 error text> F7.
    // Sent by amyboard.run_sketch() when a sketch fails to load (control mode —
    // simulate catches it off the JS console). Surface it: error modal + Code-tab
    // badge. An empty payload clears the indicator.
    if (d[4] === 0x58 /* 'X' */) {
        var errB64 = '';
        for (var xi = 5; xi < d.length - 1; xi++) errB64 += String.fromCharCode(d[xi]);
        var errText = '';
        if (errB64) { try { errText = decodeURIComponent(escape(atob(errB64))); } catch (e) { errText = errB64; } }
        if (errText && errText.trim().length) {
            show_python_error('Your sketch could not run on the AMYboard:\n\n' + errText.trim()
                + '\n\nThe board reverted to the default sketch.');
        } else if (typeof clear_python_error === 'function') {
            clear_python_error();
        }
        return;
    }
    // Firmware version report: F0 00 03 45 'V' <ascii "YYYYMMDD-<hash>"> F7,
    // sent by amyboard.report_version(). Intercept before the chunk-marker logic
    // (0x56 isn't a chunk marker, so that path would mis-read it as base64).
    if (d[4] === 0x56 /* 'V' */) {
        var verStr = '';
        for (var vi = 5; vi < d.length - 1; vi++) verStr += String.fromCharCode(d[vi]);
        console.log('[fwdetect] <- V reply: ' + JSON.stringify(verStr));
        if (_fw_version_resolve) { var vr = _fw_version_resolve; _fw_version_resolve = null; vr(verStr); }
        return;
    }
    // AMY CPU overload report: F0 00 03 45 'L' <base64 load percent> F7.
    // Sent by amyboard._on_amy_overload() after AMY's failsafe reset the synth
    // and the sketch loop was stopped. Like 'V', intercept before the
    // chunk-marker logic.
    if (d[4] === 0x4C /* 'L' */) {
        var loadB64 = '';
        for (var li = 5; li < d.length - 1; li++) loadB64 += String.fromCharCode(d[li]);
        var loadPct = '';
        if (loadB64) { try { loadPct = atob(loadB64); } catch (e) { loadPct = ''; } }
        console.log('AMY overload report from board, load ' + loadPct + '%');
        show_python_error('This sketch needed more CPU than the AMYboard has, so the sketch was stopped and the synth was reset.\n\n'
            + 'Try fewer voices, less reverb/chorus, or shorter note releases, then run it again.');
        return;
    }
    // Chunk marker at position 4.
    var marker = d[4];
    var payloadStart, isChunked, isEnd;
    if (marker === 0x30 /* '0' */ || marker === 0x43 /* 'C' */ || marker === 0x45 /* 'E' */) {
        payloadStart = 5;
        isChunked = (marker === 0x43 || marker === 0x45);
        isEnd = (marker === 0x30 || marker === 0x45);
    } else {
        // Legacy / backward-compat: no marker, whole payload is base64.
        payloadStart = 4;
        isChunked = false;
        isEnd = true;
    }
    var b64 = '';
    for (var i = payloadStart; i < d.length - 1; i++) b64 += String.fromCharCode(d[i]);
    console.log('sysex chunk:', d.length, 'bytes, marker=' + (marker === 0x43 ? 'C' : marker === 0x45 ? 'E' : marker === 0x30 ? '0' : 'legacy') + ', b64 len:', b64.length, 'stage:', _sync_stage);

    if (isChunked && !isEnd) {
        // 'C' chunk — accumulate and wait for more.
        _sysex_b64_accum += b64;
        return;
    }

    // Single chunk or final chunk of a multi-chunk message — flush.
    var fullB64;
    if (marker === 0x45 /* 'E' */) {
        fullB64 = _sysex_b64_accum + b64;
        _sysex_b64_accum = '';
    } else {
        // Single '0' marker or legacy: any leftover accum is stale from an
        // interrupted prior sequence; drop it.
        if (_sysex_b64_accum.length > 0) {
            console.warn('sysex: dropping stale accumulator ' + _sysex_b64_accum.length + ' chars');
            _sysex_b64_accum = '';
        }
        fullB64 = b64;
    }
    console.log('sysex complete: b64 len:', fullB64.length, 'stage:', _sync_stage);

    var payloadText = '';
    try {
        var binaryStr = atob(fullB64);
        var bytes = new Uint8Array(binaryStr.length);
        for (var j = 0; j < binaryStr.length; j++) bytes[j] = binaryStr.charCodeAt(j);
        payloadText = new TextDecoder('utf-8').decode(bytes);
    } catch (e) {
        console.warn('sync: base64 decode failed, b64 len:', fullB64.length, e);
        return;
    }
    console.log('sysex decoded:', payloadText.length, 'chars');
    _handle_sync_sysex_payload(payloadText);
}
var _on_midi_ready = null;  // one-shot callback for after WebMIDI init
var _amyboard_port_warning_dismissed = false;

function refresh_amyboard_port_warning() {
    var warning = document.getElementById('amyboard-port-warning');
    if (!warning) return;
    if (amyboard_mode !== 'control' || _amyboard_port_warning_dismissed) {
        warning.classList.add('d-none');
        return;
    }
    var midiIn = document.amyboard_settings && document.amyboard_settings.midi_input;
    var midiOut = document.amyboard_settings && document.amyboard_settings.midi_output;
    function _pn(sel) {
        if (!sel || !sel.options[sel.selectedIndex]) return '';
        var t = sel.options[sel.selectedIndex].text || '';
        var idx = t.indexOf(': ');
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    var inName = _pn(midiIn);
    var outName = _pn(midiOut);
    var bothAmy = /amyboard/i.test(inName) && /amyboard/i.test(outName);
    if (bothAmy) {
        warning.classList.add('d-none');
    } else {
        warning.classList.remove('d-none');
    }
}

function dismiss_amyboard_port_warning() {
    _amyboard_port_warning_dismissed = true;
    var warning = document.getElementById('amyboard-port-warning');
    if (warning) warning.classList.add('d-none');
}

// Populate the MIDI Input Pass-Thru dropdown with all inputs except the one
// currently selected as the control-mode MIDI in. Preserves the user's current
// selection if still available; otherwise falls back to [None].
function populate_midi_passthru_dropdown() {
  var select = document.getElementById('midi-passthru-select');
  if (!select || !WebMidi || !WebMidi.supported) return;
  var midi_in = document.amyboard_settings && document.amyboard_settings.midi_input;
  var excludedInputId = null;
  if (midi_in && Array.isArray(midiInputOptionIds) && midiInputOptionIds.length > 0) {
    var idx = Number(midi_in.selectedIndex);
    if (Number.isInteger(idx) && idx >= 0 && idx < midiInputOptionIds.length) {
      excludedInputId = midiInputOptionIds[idx];
    }
  }
  // Remember the currently selected pass-thru port id so we can re-select it
  // after rebuilding (if it's still in the list).
  var previousId = null;
  if (select.selectedIndex >= 0 && select.selectedIndex < midiPassthruOptionIds.length) {
    previousId = midiPassthruOptionIds[select.selectedIndex];
  }
  select.options.length = 0;
  midiPassthruOptionIds = [];
  // [None] option first
  select.options[0] = new Option("MIDI Input Pass-Thru: [None]", "");
  midiPassthruOptionIds.push(null);
  if (WebMidi.inputs && WebMidi.inputs.length > 0) {
    WebMidi.inputs.forEach(function(input) {
      var inputId = safe_midi_port_id(input);
      if (!inputId) return;
      if (inputId === excludedInputId) return;  // exclude the MIDI in port
      midiPassthruOptionIds.push(inputId);
      select.options[select.options.length] = new Option(
        "MIDI Input Pass-Thru: " + safe_midi_port_name(input)
      );
    });
  }
  // Restore previous selection if still present.
  if (previousId) {
    for (var i = 0; i < midiPassthruOptionIds.length; i++) {
      if (midiPassthruOptionIds[i] === previousId) {
        select.selectedIndex = i;
        return;
      }
    }
    // Previous port is gone (disconnected or now selected as MIDI in) — detach.
    _detach_midi_passthru_listener();
  }
  select.selectedIndex = 0;
}

function _detach_midi_passthru_listener() {
  if (_midi_passthru_input && _midi_passthru_listener) {
    try { _midi_passthru_input.removeEventListener('midimessage', _midi_passthru_listener); } catch (e) {}
  }
  _midi_passthru_input = null;
  _midi_passthru_listener = null;
  midiPassthruDevice = null;
}

function setup_midi_passthru() {
  _detach_midi_passthru_listener();
  var select = document.getElementById('midi-passthru-select');
  if (!select) return;
  var idx = Number(select.selectedIndex);
  if (!Number.isInteger(idx) || idx <= 0 || idx >= midiPassthruOptionIds.length) return;
  var inputId = midiPassthruOptionIds[idx];
  if (!inputId) return;
  var input = null;
  try { input = WebMidi.getInputById(inputId); } catch (e) { input = null; }
  if (!input) return;
  midiPassthruDevice = input;
  // Attach a raw listener on the underlying MIDIInput to forward every byte
  // (including sysex) to the current control-mode MIDI out, unchanged.
  var rawInput = input._midiInput || input.input;
  if (!rawInput || typeof rawInput.addEventListener !== "function") return;
  _midi_passthru_input = rawInput;
  _midi_passthru_listener = function(ev) {
    var d = ev && ev.data;
    if (!d || d.length === 0) return;
    // Forward byte-for-byte to the control-mode MIDI out.
    var out = get_selected_midi_output_device() || midiOutputDevice;
    if (out && typeof out.send === "function") {
      try { out.send(d); } catch (err) {}
    }
    // Also feed CC messages into move_knob so CC learn + live knob tracking
    // work in control mode (the main MIDI in is the AMYboard sysex port and
    // doesn't route to move_knob in control mode).
    if (d.length >= 3 && (d[0] & 0xF0) === 0xB0) {
      var channel = (d[0] & 0x0F) + 1;
      var cc = d[1];
      var value = d[2];
      try { move_knob(channel, cc, value); } catch (err) {}
    }
  };
  rawInput.addEventListener('midimessage', _midi_passthru_listener);
  console.log('MIDI pass-thru attached:', safe_midi_port_name(input));
}

async function setup_midi_devices() {
  var selectedInput = get_selected_midi_input_device();
  if (selectedInput) {
    // Only destroy the old wrapper if it's a DIFFERENT Input from the
    // new selectedInput. WebMidi.js keeps Input wrappers keyed by port
    // id and returns the same instance for the same port, so on a
    // "same port came back" connected event selectedInput IS
    // midiInputDevice. Calling destroy() on it removes the wrapper
    // from WebMidi.inputs entirely — then the subsequent assignment
    // leaves midiInputDevice pointing at a dead reference and the
    // next _refresh_main_midi_dropdowns shows 0 inputs, silently
    // breaking reply reception. This bit us on Windows post-reload,
    // where two `connected` events (one per port) fired in quick
    // succession and the second run wiped out the input set up by
    // the first.
    if (midiInputDevice != null && midiInputDevice !== selectedInput &&
        typeof midiInputDevice.destroy === "function") {
      try { midiInputDevice.destroy(); } catch (e) {}
    }
    midiInputDevice = selectedInput;
    // Remove previous raw listener (setup_midi_devices can run multiple times).
    if (_raw_sysex_input && _raw_sysex_listener) {
        try { _raw_sysex_input.removeEventListener('midimessage', _raw_sysex_listener); } catch (e) {}
        _raw_sysex_input = null;
        _raw_sysex_listener = null;
    }
    // Also attach a raw listener on the underlying MIDIInput to guarantee we
    // see sysex even if WebMidi.js filters it from the "midimessage" event.
    try {
        var rawInput = midiInputDevice._midiInput || midiInputDevice.input;
        if (rawInput) {
            _raw_sysex_input = rawInput;
            _raw_sysex_listener = function(ev) {
                var d = ev.data;
                if (!d || d.length === 0) return;
                console.log('raw sysex event:', d.length, 'bytes, first:', d[0].toString(16), 'last:', d[d.length-1].toString(16));
                // Walk the incoming bytes and stitch together complete sysex messages.
                // Max raw-byte size we'll accumulate before giving up on an
                // in-progress message; guards against a stuck reassembler
                // (no 0xF7 ever arrives) eating memory.
                var SYSEX_REASM_MAX = 131072;
                for (var k = 0; k < d.length; k++) {
                    var b = d[k];
                    if (b === 0xF0) {
                        // Start (or restart) a new sysex message.
                        _sysex_reasm = [0xF0];
                    } else if (_sysex_reasm !== null) {
                        _sysex_reasm.push(b);
                        if (b === 0xF7) {
                            var full = _sysex_reasm;
                            _sysex_reasm = null;
                            _process_complete_sysex(full);
                        } else if (_sysex_reasm.length > SYSEX_REASM_MAX) {
                            console.warn('sysex reassembler exceeded ' + SYSEX_REASM_MAX + ' bytes, discarding');
                            _sysex_reasm = null;
                        }
                    }
                    // Bytes outside of a sysex frame are ignored here (status
                    // bytes / running-status data are handled by the WebMidi
                    // "midimessage" listener below).
                }
            };
            rawInput.addEventListener('midimessage', _raw_sysex_listener);
        }
    } catch (e) { console.warn('raw sysex listener setup failed:', e); }
    // setup_midi_devices() runs again on dropdown onchange, statechange, and the
    // sync/pull flows — and WebMidi.js addListener() APPENDS (it does not
    // replace). Without clearing first, each re-run stacked another
    // "midimessage" handler on the same Input, so every incoming message got
    // delivered (and forwarded to MicroPython + move_knob) N times — the "MIDI
    // messages repeated 4x/2x" bug. Remove any handler we attached on a previous
    // run before adding the new one. (No callback arg = remove all "midimessage"
    // listeners on this WebMidi.js Input wrapper; the raw-sysex and passthru
    // listeners live on the underlying native MIDIInput via addEventListener and
    // are unaffected.)
    try { midiInputDevice.removeListener("midimessage"); } catch (e) {}
    midiInputDevice.addListener("midimessage", e => {
      const data = e.message && e.message.data ? e.message.data : [];
      const status = data.length > 0 ? data[0] : null;
      // In control mode, the main MIDI in is the AMYboard sysex port — skip
      // move_knob from this path so learn + live knob moves don't trigger on
      // incidental MIDI coming back from the board. The MIDI Input Pass-Thru
      // port is the authoritative source for move_knob/learn in control mode.
      if (amyboard_mode !== 'control'
        && Number.isFinite(status) && (status & 0xF0) === 0xB0 && data.length >= 3) {
        const channel = (status & 0x0F) + 1;
        const cc = data[1];
        const value = data[2];
        move_knob(channel, cc, value);
      }
      // Fallback sysex reassembly via the WebMidi.js "midimessage" path in
      // case the raw-listener attachment above didn't land (e.g. WebMidi.js
      // internal property name drift). SKIP if the raw listener is active —
      // both listeners share _sysex_b64_accum, so 'C' chunks get doubled
      // when both fire, corrupting multi-chunk zD responses.
      if (!_raw_sysex_listener && data && data.length > 0 && data[0] === 0xF0) {
        console.log('webmidi.js midimessage sysex:', data.length, 'bytes, first:', data[0].toString(16), 'last:', data[data.length-1].toString(16));
        var _SYSEX_REASM_MAX_WM = 131072;
        for (var kk = 0; kk < data.length; kk++) {
          var bb = data[kk];
          if (bb === 0xF0) {
            _sysex_reasm = [0xF0];
          } else if (_sysex_reasm !== null) {
            _sysex_reasm.push(bb);
            if (bb === 0xF7) {
              var full2 = _sysex_reasm;
              _sysex_reasm = null;
              _process_complete_sysex(full2);
            } else if (_sysex_reasm.length > _SYSEX_REASM_MAX_WM) {
              console.warn('webmidi.js sysex reassembler exceeded ' + _SYSEX_REASM_MAX_WM + ' bytes, discarding');
              _sysex_reasm = null;
            }
          }
        }
      }
      // Sysex is also handled by the raw listener above to avoid WebMidi.js filtering.
      // MIDI Thru: always on in control mode, off in simulate mode.
      // Skip thru for SPSS sysex (our own protocol) to avoid feedback loop —
      // the AMYboard's sysex response must not echo back to its own input.
      if (amyboard_mode === 'control' && midiOutputDevice && data.length > 0) {
        var isSpssSysex = (data[0] === 0xF0 && data.length > 3 &&
                           data[1] === 0x00 && data[2] === 0x03 && data[3] === 0x45);
        if (!isSpssSysex) {
          try { midiOutputDevice.send(data); } catch (err) {}
        }
      }
      // Feed bytes to AMY audioworklet for sound processing
      for(byte in e.message.data) {
        if(audio_started) amy_process_single_midi_byte(e.message.data[byte], 1);
      }
      // Also forward complete message to MicroPython for Python midi callbacks.
      // AMY's EM_ASM hook can't cross the audioworklet boundary, so we do it here.
      if (audio_started && typeof mp !== 'undefined' && mp.midiInHook) {
        const sysex = (data.length > 0 && data[0] === 0xF0) ? 1 : 0;
        mp.midiInHook(new Uint8Array(data), data.length, sysex);
      }
    });
  } else {
    midiInputDevice = null;
  }
  midiOutputDevice = get_selected_midi_output_device();
  // Force both the underlying MIDIInput and MIDIOutput to connection='open'.
  // Per WebMIDI spec, addEventListener('midimessage', ...) and the first
  // send() should auto-open, but on Windows Chrome has been observed
  // leaving both at connection='closed' even after listener attach and
  // sendSysex calls — which silently eats all traffic. Explicit .open()
  // returns a Promise that resolves when the underlying OS port is really
  // open. Fire-and-forget (don't await) so setup_midi_devices stays quick.
  try {
      var rawIn = _raw_sysex_input;
      if (rawIn && typeof rawIn.open === 'function') {
          rawIn.open().then(function() {
              console.log('midi input opened: id=' + rawIn.id +
                          ' state=' + rawIn.state +
                          ' connection=' + rawIn.connection);
          }).catch(function(e) {
              console.warn('midi input open failed:', e && e.message ? e.message : e);
          });
      }
  } catch (e) {}
  try {
      var rawOut = midiOutputDevice && (midiOutputDevice._midiOutput || midiOutputDevice.output || null);
      if (rawOut && typeof rawOut.open === 'function') {
          rawOut.open().then(function() {
              console.log('midi output opened: id=' + rawOut.id +
                          ' state=' + rawOut.state +
                          ' connection=' + rawOut.connection);
          }).catch(function(e) {
              console.warn('midi output open failed:', e && e.message ? e.message : e);
          });
      }
  } catch (e) {}
  // Refresh the pass-thru dropdown so it excludes the (possibly new) MIDI in.
  populate_midi_passthru_dropdown();
  refresh_amyboard_port_warning();
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
        midi_out.options[midi_out.options.length] = new Option("MIDI out: " + safe_midi_port_name(output));
      });
    }
    // In control mode: preselect ports. The default is always the AMYboard
    // hardware port (exact name); see _preselect_control_port for the order.
    function _opt_portname(opt) {
        var t = opt.text || "";
        var idx = t.indexOf(": ");
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    if (amyboard_mode === 'control') {
      var savedPorts = _get_midi_cookie();
      _preselect_control_port(midi_in, savedPorts ? savedPorts.input : null, null,
                              _midi_ports_user_choice ? _midi_ports_user_choice.input : null);
      _preselect_control_port(midi_out, savedPorts ? savedPorts.output : null, null,
                              _midi_ports_user_choice ? _midi_ports_user_choice.output : null);
    }
    // Populate the MIDI Input Pass-Thru dropdown (control mode only).
    populate_midi_passthru_dropdown();
    // First run setup
    setup_midi_devices();
    // Show warning if neither port is an AMYboard (control mode only).
    refresh_amyboard_port_warning();
    // Fire the post-MIDI-init callback if set (used for initial sync in control mode).
    if (typeof _on_midi_ready === 'function') {
        var cb = _on_midi_ready;
        _on_midi_ready = null;
        cb();
    }
  }

  if(WebMidi.supported) {
    WebMidi
      .enable({sysex:true})
      .then(function() {
          console.log('WebMidi enabled. sysex state:', WebMidi.sysexEnabled);
          onEnabled();
      })
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
    show_python_error(e.message);
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


var _last_python_error = "";

function _set_code_tab_error_badge(show) {
    var badge = document.getElementById("code-tab-error-badge");
    if (badge) badge.classList.toggle("d-none", !show);
}

// Clear the sketch-error indicator (the Code-tab badge + the error modal).
// Called at the start of every Write so each attempt starts clean; the device
// (control) or the console interceptor (simulate) re-raises it if the new sketch
// fails to load.
function clear_python_error() {
    _last_python_error = "";
    window._last_python_error = "";
    _set_code_tab_error_badge(false);
    var existing = document.getElementById("python_error_modal");
    if (existing) existing.remove();
}
window.clear_python_error = clear_python_error;

function show_python_error(message) {
    // Remember the error and flag the Code tab so it's visible from any tab
    // (e.g. after downloading a broken sketch from AMYboard World).
    _last_python_error = message;
    window._last_python_error = message;
    _set_code_tab_error_badge(true);
    var existing = document.getElementById("python_error_modal");
    if (existing) existing.remove();
    var overlay = document.createElement("div");
    overlay.id = "python_error_modal";
    overlay.style.cssText = "position:fixed;inset:0;z-index:10000;background:rgba(0,0,0,0.6);display:flex;align-items:center;justify-content:center;";
    overlay.addEventListener("click", function(e) { if (e.target === overlay) overlay.remove(); });
    var box = document.createElement("div");
    box.style.cssText = "background:#1a1a1a;border:3px solid #ff4444;color:#fff;padding:1.5rem 2rem;max-width:700px;width:90vw;max-height:70vh;overflow:auto;font-family:monospace;font-size:0.85rem;position:relative;box-shadow:0 8px 32px rgba(0,0,0,0.5);";
    var header = document.createElement("div");
    header.style.cssText = "display:flex;justify-content:space-between;align-items:center;margin-bottom:1rem;";
    header.innerHTML = '<span style="color:#ff4444;font-weight:700;font-size:1.1rem;font-family:sans-serif;">Python Error</span>';
    var closeBtn = document.createElement("button");
    closeBtn.textContent = "✕";
    closeBtn.style.cssText = "background:none;border:none;color:#888;font-size:1.3rem;cursor:pointer;padding:0 0.25rem;";
    closeBtn.onclick = function() { overlay.remove(); };
    header.appendChild(closeBtn);
    box.appendChild(header);
    var pre = document.createElement("pre");
    pre.style.cssText = "margin:0;white-space:pre-wrap;word-break:break-word;color:#ffcccc;line-height:1.5;";
    pre.textContent = message;
    box.appendChild(pre);
    overlay.appendChild(box);
    document.body.appendChild(overlay);
}

async function show_alert(text) {
    const alertbox = document.querySelector("#alert_box");
    alertbox.innerHTML = text;
    alertbox.classList.remove('d-none'); //Remove hidden class
    alertbox.classList.add('alert-danger'); //Chance red color
    setTimeout(function() {alertbox.classList.add('d-none')}, 3000);

}

function ensure_current_environment_layout(seedDefaults) {
    // Directory creation only — sketch.py is handled by Python (amyboard.ensure_user_environment).
    try { mp.FS.mkdirTree(CURRENT_BASE_DIR); } catch (e) {}
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
    files.sort(function(a, b) { return String(a).localeCompare(String(b)); });
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
        container.innerHTML = '<div class="env-file-empty">No files in current sketch.</div>';
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
        show_alert("Select a sketch file first.");
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
        show_alert("Select a sketch file first.");
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
        show_alert("Select a sketch file first.");
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
        show_alert("Select a sketch file first.");
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
    var targetFilename = selected_environment_file || "sketch.py";
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
    // In control mode there is no filesystem to save to.
    if (amyboard_mode === 'control' || !mp) {
        environment_editor_dirty = false;
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
        show_alert("Select a sketch file first.");
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
        // SYNC 2: when the main sketch is loaded, position the knobs and rebuild
        // the knob log from its _auto_generated_knobs block (UI only — no AMY
        // send). Gated to sketch.py so opening another file doesn't wipe the log.
        if (selected_environment_file === "sketch.py" && typeof set_knobs_from_sketch === "function") {
            set_knobs_from_sketch();
        }
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
    if (files.indexOf("sketch.py") !== -1) {
        target = "sketch.py";
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

function is_world_environment_filename(filename) {
    var fn = normalize_world_filename(filename).toLowerCase();
    return fn.endsWith(".py") || fn.endsWith(".tar");
}

function get_world_package_name(filename) {
    var normalized = normalize_world_filename(filename);
    // Strip .py or .tar extension for display.
    return normalized.replace(/\.(py|tar)$/i, "");
}

function get_world_display_name(filename) {
    return get_world_package_name(filename);
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

// --- "How about one of these?" sketch suggestions ---------------------------
// Before a prompt is sent to the (token-billed) generator, the editor asks the
// World API for existing sketches similar to the description and offers them
// in a popup. Any failure here returns [] so generation always proceeds.
async function fetch_similar_sketches(description) {
    try {
        var resp = await fetch(world_api_url("/api/amyboardworld/similar"), {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ description: description, limit: 10 }),
        });
        if (!resp.ok) return [];
        var data = await resp.json();
        var items = (data && Array.isArray(data.items)) ? data.items : [];
        // Only offer plain .py sketches — a .tar package can't be loaded into
        // the editor as text by load_suggested_sketch().
        return items.filter(function (item) {
            return item && normalize_world_filename(item.filename).toLowerCase().endsWith(".py");
        });
    } catch (e) {
        return [];
    }
}

var sketch_suggestion_items = [];
var sketch_suggestions_loading_index = null;
var sketch_suggestions_choice = null;
var sketch_suggestions_resolve = null;

function render_sketch_suggestions_list() {
    var list = document.getElementById("sketch-suggestions-list");
    if (!list) return;
    var html = "";
    for (var i = 0; i < sketch_suggestion_items.length; i++) {
        var item = sketch_suggestion_items[i];
        var filename = normalize_world_filename(item.filename);
        var displayName = get_world_display_name(filename);
        var description = (typeof item.description === "string") ? item.description.trim() : "";
        var when = format_world_file_timestamp(item.time);
        var meta = (item.username || "unknown") + (when ? " • " + when : "");
        var isLoading = (sketch_suggestions_loading_index === i);
        html += '<div class="col">';
        html += '<button type="button" class="w-100 border rounded p-2 bg-white text-start d-flex justify-content-between align-items-start"' +
            ' onclick="choose_sketch_suggestion(' + i + ')"' + (sketch_suggestions_loading_index !== null ? " disabled" : "") + ">";
        html += '<div class="me-2 text-start"><div class="fw-bold">' + escape_html(displayName);
        if (description.length > 0) {
            html += ' <span class="fw-normal text-muted">- ' + escape_html(description) + "</span>";
        }
        html += '</div><div class="small text-muted">' + escape_html(meta) + '</div></div>';
        if (isLoading) {
            html += '<span class="d-inline-flex align-items-center text-secondary small"><span class="spinner-border spinner-border-sm me-2" role="status" aria-hidden="true"></span>Loading...</span>';
        }
        html += "</button></div>";
    }
    list.innerHTML = html;
}

// Show the suggestions popup. Resolves with how it was dismissed:
// "loaded" (a suggestion was loaded into the editor), "generate" (the
// "No thanks" button), or "cancel" (closed without choosing).
function show_sketch_suggestions(items) {
    return new Promise(function (resolve) {
        var modalEl = document.getElementById("sketchSuggestionsModal");
        if (!modalEl || !window.bootstrap) {
            resolve("generate");
            return;
        }
        sketch_suggestion_items = items;
        sketch_suggestions_loading_index = null;
        sketch_suggestions_choice = null;
        sketch_suggestions_resolve = resolve;
        render_sketch_suggestions_list();
        modalEl.addEventListener("hidden.bs.modal", function () {
            var r = sketch_suggestions_resolve;
            sketch_suggestions_resolve = null;
            if (r) r(sketch_suggestions_choice || "cancel");
        }, { once: true });
        window.bootstrap.Modal.getOrCreateInstance(modalEl).show();
    });
}

// Download a suggested World sketch into the editor (same behavior as a World
// tab import, minus the confirm — clicking the suggestion IS the confirmation).
async function load_suggested_sketch(item) {
    var downloadUrl = resolve_world_download_url(item);
    if (!downloadUrl) throw new Error("Missing file URL");
    var response = await fetch(downloadUrl);
    if (!response.ok) throw new Error("HTTP " + response.status.toString());
    var sketchText = await response.text();
    if (editor) {
        editor.setValue(sketchText);
        setTimeout(function () { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
    }
    var packageName = normalize_world_filename(item.filename).replace(/\.(py|tar)$/, "");
    var envNameInput = document.getElementById("editor_filename");
    if (envNameInput) envNameInput.value = packageName;
    if (typeof set_knobs_from_sketch === 'function') set_knobs_from_sketch();
    // Move to the Code tab so the user can review the sketch and then Write.
    var codeTabBtn = document.getElementById('environment-tab');
    if (codeTabBtn && window.bootstrap) {
        try { new window.bootstrap.Tab(codeTabBtn).show(); } catch (e) {}
    }
}

async function choose_sketch_suggestion(index) {
    if (sketch_suggestions_loading_index !== null) return;
    var item = sketch_suggestion_items[index];
    if (!item) return;
    sketch_suggestions_loading_index = index;
    render_sketch_suggestions_list();
    try {
        await load_suggested_sketch(item);
        sketch_suggestions_choice = "loaded";
    } catch (e) {
        show_alert("Failed to load that sketch.");
        return; // keep the popup open so the user can pick another or generate
    } finally {
        sketch_suggestions_loading_index = null;
        render_sketch_suggestions_list();
    }
    var modalEl = document.getElementById("sketchSuggestionsModal");
    if (modalEl && window.bootstrap) window.bootstrap.Modal.getOrCreateInstance(modalEl).hide();
}

function generate_anyway_from_suggestions() {
    sketch_suggestions_choice = "generate";
    var modalEl = document.getElementById("sketchSuggestionsModal");
    if (modalEl && window.bootstrap) window.bootstrap.Modal.getOrCreateInstance(modalEl).hide();
}

// "Prompt to sketch": send the user's description to the World API, which calls
// the Claude API server-side and returns a generated AMYboard sketch. The result
// is loaded into the editor and then auto-started (same as the Restart Sketch
// button) so the user hears it right away.
async function generate_sketch_from_prompt() {
    var input = document.getElementById("prompt-to-sketch-input");
    var btn = document.getElementById("prompt-to-sketch-btn");
    var statusEl = document.getElementById("prompt-to-sketch-status");
    if (!input || !btn) return;

    function setStatus(text, kind) {
        if (!statusEl) return;
        statusEl.textContent = text || "";
        statusEl.className = "small " + (kind || "text-muted");
    }

    var description = String(input.value || "").trim();
    if (description.length < 3) {
        setStatus("Describe what you want the sketch to do.", "text-danger");
        return;
    }

    var prevLabel = btn.textContent;

    // Before generating, offer existing World sketches that match the prompt.
    btn.disabled = true;
    btn.textContent = "Checking…";
    setStatus("Checking AMYboard World for similar sketches…", "text-muted");
    var suggestions = await fetch_similar_sketches(description);
    btn.disabled = false;
    btn.textContent = prevLabel;
    if (suggestions.length > 0) {
        setStatus("", "");
        var choice = await show_sketch_suggestions(suggestions);
        if (choice === "loaded") {
            setStatus("Loaded a sketch from AMYboard World.", "text-success");
            return;
        }
        if (choice !== "generate") {
            return; // popup dismissed — don't generate
        }
    }

    btn.disabled = true;
    btn.textContent = "Creating…";
    setStatus("Creating sketch. May take a couple of minutes…", "text-muted");

    try {
        var currentCode = (typeof editor !== "undefined" && editor) ? editor.getValue() : "";
        var shareEl = document.getElementById("prompt-to-sketch-share");
        var share = shareEl ? !!shareEl.checked : false;
        var resp = await fetch(world_api_url("/api/amyboardworld/generate"), {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ description: description, current_code: currentCode, share: share }),
        });

        var data = {};
        try { data = await resp.json(); } catch (e) { data = {}; }

        if (!resp.ok) {
            setStatus((data && data.detail) ? data.detail : ("Error " + resp.status), "text-danger");
            return;
        }

        if (data && data.code && typeof editor !== "undefined" && editor) {
            editor.setValue(data.code);
            var remaining = (typeof data.remaining_today === "number") ? data.remaining_today : null;
            setStatus(
                "Loaded into the editor." + (remaining !== null ? (" " + remaining + " left today.") : ""),
                "text-success"
            );
            // Don't touch the board here — just load the code into the editor.
            // Auto-restarting reboots the AMYboard (zB1), which the user didn't
            // ask for. They hit "Write to your AMYboard" when they're ready.
        } else {
            setStatus("No sketch was returned.", "text-danger");
        }
    } catch (e) {
        setStatus("Network error: " + e, "text-danger");
    } finally {
        btn.disabled = false;
        btn.textContent = prevLabel;
    }
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
    var tags = AMYBOARD_WORLD_PILL_TAGS;
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
    var tags = AMYBOARD_WORLD_PILL_TAGS;
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
        "Clear current code and create a fresh sketch.py?",
        "Clear"
    );
    if (!confirmed) {
        return;
    }
    try {
        clear_current_environment_dir();
        try { await mp.runPythonAsync("import amyboard; amyboard.ensure_user_environment()"); } catch (e) {}
        selected_environment_file = null;
        await sync_persistent_fs();
        await fill_tree();
        if (list_environment_files().indexOf("sketch.py") !== -1) {
            await select_environment_file("sketch.py", true);
        } else {
            var files = list_environment_files();
            if (files.length) {
                await select_environment_file(files[0], true);
            } else if (editor) {
                editor.setValue("");
            }
        }
    } catch (e) {
        show_alert("Could not clear sketch.");
    }
}

async function run_current_environment() {
    await save_editor_if_dirty();
    await runCodeBlock("import amyboard; amyboard.restart_sketch()");
}

async function stop_current_environment() {
    // Stop the sketch loop, all notes off, and reset the sequencer.
    await runCodeBlock("import amyboard, amy; amyboard.stop_sketch(); amy.send(reset=" + (4096 + 131072) + ")");
}

async function restart_sketch() {
    if (amyboard_mode === 'control') {
        // zB1: normal reboot — runs sketch.py on boot.
        _show_resetting_modal('Restarting\u2026');
        amy_add_log_message('zB1Z');
        console.log('restart: zB1 sent, waiting for board...');
        await wait_for_board_ready();
        _hide_resetting_modal();
        if (document.activeElement) document.activeElement.blur();
    } else {
        await save_editor_if_dirty();
        await runCodeBlock("import amyboard; amyboard.restart_sketch()");
    }
}

function set_tempo_from_ui(inputId) {
    var input = document.getElementById(inputId);
    if (!input) return;
    var val = parseInt(input.value, 10);
    if (isNaN(val) || val < 40 || val > 240) {
        if (typeof show_alert === 'function') show_alert('Tempo must be between 40 and 240 BPM.');
        return;
    }
    if (amyboard_mode === 'control') {
        amy_add_log_message('zPimport sequencer; sequencer.tempo(' + val + ')Z');
    } else {
        runCodeBlock('import sequencer; sequencer.tempo(' + val + ')');
    }
}

function reboot_to_bootloader() {
    // zB/zB0: reboot into bootloader mode (sketch skipped).
    // Handled in pure C — works even when loop() is hogging the scheduler.
    amy_add_log_message('zBZ');
}

// Windows Chrome WebMIDI can't recover from a USB reboot within the same
// document: the MIDIOutput handle stays stale, no statechange fires, and
// sendSysex silently goes into the void. We work around this by sending
// zB, waiting for the board to finish rebooting, then reloading the page
// — which gives us a fresh MIDIAccess from scratch. Each zB+wait flow
// saves its post-bootloader context in sessionStorage and reloads; on the
// new page, pageload_control_sync picks up the flag and runs the post-
// bootloader work against the fresh handles. macOS doesn't need this.
var _IS_WINDOWS_CHROME = typeof navigator !== 'undefined' &&
                         /Windows/i.test(navigator.userAgent || '') &&
                         /Chrome/i.test(navigator.userAgent || '');

// Complete the post-bootloader portion of a sketch-upload flow. Shared
// between the synchronous (macOS) path and the post-reload (Windows) path
// so both end up doing identical work — upload file, apply knobs to UI,
// optionally restart the sketch, set filename input, refresh world list.
// opts fields (all optional unless marked REQUIRED):
//   sketchText       REQUIRED — file content to upload.
//   sketchPath       defaults to '/user/current/sketch.py'.
//   restart          true → send zP restart_sketch after upload;
//                    'transfer_done' → send zP environment_transfer_done
//                    instead (Write semantics: also restarts the sequencer
//                    that the zT transfer stopped).
//   setEditor        default true → editor.setValue(sketchText) + refresh.
//   applyKnobs       default true → extract knobs from text and apply to UI.
//   suppressKnobCc   true → set window.suppress_knob_cc_send around
//                    apply_zd_dump_to_knobs (avoids echoing CC back when
//                    the zT transfer already put the knob values on hw).
//   packageName      string → set #editor_filename input value.
//   refreshWorldFiles true → re-fetch the world file list after upload.
async function _upload_sketch_post_bootloader(opts) {
    if (!opts || typeof opts.sketchText !== 'string') return;
    var sketchPath = opts.sketchPath || '/user/current/sketch.py';
    try {
        await _send_text_file_to_amyboard(sketchPath, opts.sketchText);
        console.log('post-zb upload: sketch sent to AMYboard');
        await sleep_ms(opts.restart ? 500 : 1000);
    } catch (e) {
        console.warn('post-zb upload: sketch upload to AMYboard failed', e);
    }
    try { _hide_saving_modal(); } catch (e) {}
    try { _hide_syncing_modal(); } catch (e) {}
    if (opts.setEditor !== false && typeof editor !== 'undefined' && editor) {
        editor.setValue(opts.sketchText);
        setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
    }
    if (opts.applyKnobs !== false) {
        var knobs = (typeof extract_knobs_from_sketch === 'function')
                    ? extract_knobs_from_sketch(opts.sketchText) : null;
        if (knobs && typeof apply_zd_dump_to_knobs === 'function') {
            if (opts.suppressKnobCc) {
                window.suppress_knob_cc_send = true;
                try { apply_zd_dump_to_knobs(knobs); }
                finally { window.suppress_knob_cc_send = false; }
            } else {
                apply_zd_dump_to_knobs(knobs);
            }
        }
    }
    if (opts.packageName) {
        var envNameInput = document.getElementById('editor_filename');
        if (envNameInput) envNameInput.value = opts.packageName;
    }
    if (opts.restart) {
        var restartFn = (opts.restart === 'transfer_done') ? 'environment_transfer_done' : 'restart_sketch';
        amy_add_log_message('zPimport amyboard; amyboard.' + restartFn + '()Z');
        console.log('post-zb upload: sketch restarted via zP (' + restartFn + ')');
    }
    if (opts.refreshWorldFiles && typeof refresh_amyboard_world_files === 'function') {
        try { await refresh_amyboard_world_files(); } catch (e) {}
    }
}

// Windows Chrome entry point for any zB+wait+upload flow. Stashes the
// post-bootloader options in sessionStorage, sends zB, waits ~4s for the
// board to reboot, then reloads the page. Never returns in practice —
// the reload unloads the current document.
async function _zb_then_reload_with_upload_context(opts) {
    try {
        sessionStorage.setItem('amyboard_post_zb_upload', JSON.stringify(opts));
    } catch (e) {
        console.warn('_zb_then_reload_with_upload_context: stash failed:', e);
    }
    reboot_to_bootloader();
    console.log('Windows Chrome — zB sent, reloading in 4s for fresh MIDIAccess');
    await sleep_ms(4000);
    console.log('reloading now');
    window.location.reload();
    // Block the caller's await in case reload is delayed — we do NOT want
    // the macOS-path fallthrough to run on Windows.
    await new Promise(function() {});
}

// zI ping — wait for the board to be ready after a reboot.
var _ping_resolve = null;
var _ping_reject = null;

// Send a single zI ping directly, bypassing sysex_write_amy_message's
// ackPromise path. zI is handled in pure C on the board and replies with
// "OK" — it never triggers the "AK" ACK that sysex_write_amy_message
// waits for, so routing pings through that path would leak a 5-second
// setTimeout + pending Promise for every retry. Also refreshes the
// output handle on each call: on Windows the MIDIOutput reference goes
// stale after the USB reconnect that follows zB, and we need the fresh
// one from WebMidi.outputs. Returns true if the send was dispatched,
// false otherwise (no output, or sendSysex threw).
function _send_zi_ping() {
    var outputDevice = get_selected_midi_output_device();
    if (!outputDevice) return false;
    try {
        // Best-effort: force the underlying MIDIOutput to connection='open'
        // before each send. On Windows Chrome, connection has been observed
        // stuck at 'closed' through the entire poll window — sendSysex is
        // then a silent no-op. Fire-and-forget (don't await): if open()
        // succeeds in time, the send reaches the port; if not, we'll try
        // again next iteration.
        var rawOut = outputDevice._midiOutput || outputDevice.output || null;
        if (rawOut && typeof rawOut.open === 'function') {
            try { rawOut.open().catch(function() {}); } catch (e) {}
        }
        var payload = Array.from(new TextEncoder().encode('zIZ'));
        if (typeof outputDevice.sendSysex === 'function') {
            outputDevice.sendSysex(AMYBOARD_SYSEX_MFR_ID, payload);
        } else if (typeof outputDevice.send === 'function') {
            outputDevice.send([0xF0].concat(AMYBOARD_SYSEX_MFR_ID, payload, [0xF7]));
        } else {
            return false;
        }
        return true;
    } catch (e) {
        console.warn('_send_zi_ping: send failed (port likely stale):', e);
        return false;
    }
}

// Install a raw MIDIAccess.statechange listener ONCE per page lifetime.
// This bypasses WebMidi.js and logs the raw platform events. On Windows,
// the WebMidi.js 'connected'/'disconnected' wrapper events have been
// observed not to fire for USB MIDI reconnects; this diagnostic tells us
// whether the raw MIDIAccess layer sees them (and when) so we can tune
// our workaround strategy.
var _raw_midi_statechange_installed = false;
async function _install_raw_midi_statechange_diagnostic() {
    if (_raw_midi_statechange_installed) return;
    _raw_midi_statechange_installed = true;
    if (typeof navigator === 'undefined' || !navigator.requestMIDIAccess) return;
    try {
        var access = await navigator.requestMIDIAccess({ sysex: true });
        access.addEventListener('statechange', function(e) {
            var p = e.port;
            console.log('[MIDIAccess] statechange: id=' + (p ? p.id : 'null') +
                        ' name=' + (p ? p.name : 'null') +
                        ' state=' + (p ? p.state : 'null') +
                        ' connection=' + (p ? p.connection : 'null') +
                        ' type=' + (p ? p.type : 'null'));
        });
        var inputs_count = 0, outputs_count = 0;
        access.inputs.forEach(function() { inputs_count++; });
        access.outputs.forEach(function() { outputs_count++; });
        console.log('[MIDIAccess] diagnostic listener installed; inputs=' +
                    inputs_count + ' outputs=' + outputs_count);
        access.inputs.forEach(function(p) {
            console.log('  [MIDIAccess] input: id=' + p.id + ' name=' + p.name +
                        ' state=' + p.state + ' connection=' + p.connection);
        });
        access.outputs.forEach(function(p) {
            console.log('  [MIDIAccess] output: id=' + p.id + ' name=' + p.name +
                        ' state=' + p.state + ' connection=' + p.connection);
        });
    } catch (e) {
        console.warn('[MIDIAccess] diagnostic install failed:', e);
    }
}

async function wait_for_board_ready(timeout_ms) {
    // Default 30s — Windows repros show Chrome's WebMIDI never fires
    // statechange to JS across a USB reboot until we force it via a
    // raw-port close() cycle. 30s gives room for burst pings, probe
    // pings, and the halfway close/open dance. macOS happy path
    // resolves in < 5s so the ceiling is a worst-case bound.
    if (!timeout_ms) timeout_ms = 30000;
    // Install raw-access diagnostic if we haven't already. No-op on repeat calls.
    _install_raw_midi_statechange_diagnostic();
    // Poll with zI until the board replies with OK.
    var start_time = Date.now();
    var deadline = start_time + timeout_ms;

    // Persistent reply catcher. A previous iteration overwrote
    // _ping_resolve inside `await new Promise(...)` on every loop tick,
    // meaning _ping_resolve was NULL during the gaps (e.g. during the
    // halfway port-cycle's 1.5s sleep). On Windows the zI reply arrived
    // during that gap, hit `if (_ping_resolve)` at line 1237 with a
    // null handler, and was silently dropped — wait_for_board_ready
    // ran to full timeout even though the board was demonstrably ready.
    //
    // Fix: install ONE handler at function entry that persists for the
    // whole wait. The polling loop races its per-iteration 500ms sleep
    // against this single reply_promise so any reply at any time breaks
    // us out. Also unifies the sync-modal "Change ports" cancel path
    // (line 4189 calls _ping_resolve() to abort us) — cancel paths
    // resolve the same promise and return true, matching prior behavior.
    var reply_received = false;
    var reply_promise = new Promise(function(resolve) {
        _ping_resolve = function() {
            if (reply_received) return;
            reply_received = true;
            resolve(true);
        };
    });
    _ping_reject = null;

    // Send strategy:
    //   - Burst: the first MAX_BURST_PINGS iterations ping every loop tick
    //     (catches the macOS happy path where the board replies within
    //     a couple of seconds).
    //   - Probe: after the burst, send at most one ping every PROBE_INTERVAL_MS.
    //     This keeps gently nudging Chrome's WebMIDI plumbing (which we've
    //     seen truly frozen on Windows across USB reboots) without queueing
    //     up the 30 stale sends that caused the original 60s renderer hang.
    //
    // Previous iteration used a hard cap of 3 then silence for the whole
    // remaining window; that avoided the hang but never sent again, so if
    // Chrome *eventually* got its state together we never poked it to find
    // out. Probe mode keeps the nudging alive without the pileup.
    var MAX_BURST_PINGS = 3;
    var PROBE_INTERVAL_MS = 5000;
    var consecutive_failures = 0;
    var last_probe_time = 0;

    var connectedHandler = async function() {
        console.log('wait_for_board_ready: WebMidi connected event, refreshing handles');
        try {
            if (typeof _refresh_main_midi_dropdowns === 'function') {
                _refresh_main_midi_dropdowns();
            }
            await setup_midi_devices();
        } catch (err) {}
        // Fresh port → reset so the burst fires again.
        consecutive_failures = 0;
        last_probe_time = 0;
        _send_zi_ping();
    };
    function _attach_connected_listener() {
        if (typeof WebMidi !== 'undefined' && WebMidi && typeof WebMidi.addListener === 'function') {
            try { WebMidi.addListener('connected', connectedHandler); } catch (e) {}
        }
    }
    function _detach_connected_listener() {
        if (typeof WebMidi !== 'undefined' && WebMidi && typeof WebMidi.removeListener === 'function') {
            try { WebMidi.removeListener('connected', connectedHandler); } catch (e) {}
        }
    }
    _attach_connected_listener();

    // Track the bound input id so we only re-run setup_midi_devices (which
    // destroys and re-attaches the midimessage listener) when the port
    // actually changes. On macOS CoreMIDI re-uses the id across reboots so
    // this avoids churning listeners; on Windows the id has been observed
    // to stay frozen through the entire window, so this branch rarely
    // fires there — but if Chrome ever does update, we catch it.
    var initialInput = null;
    try { initialInput = get_selected_midi_input_device(); } catch (e) {}
    var prevInputId = initialInput ? initialInput.id : null;

    // One-shot: halfway through the timeout, cycle the raw MIDIOutput/MIDIInput
    // through close() → pause → open(). Windows repros show Chrome reporting
    // connection='open' and state='connected' for the full 30s while the
    // physical device was rebooted underneath — sends go nowhere. The
    // only remaining lever we have from JS is forcing Chrome to RELEASE
    // the cached USB endpoint handle via close() and then re-acquire it
    // via open(), with a pause in between to give Chrome time to notice
    // the underlying Windows device changed. WebMidi.disable()/enable()
    // (previous iteration) didn't produce this effect — it has to happen
    // on the raw MIDIPort. No-op on macOS because the happy path already
    // succeeded well before halfway.
    var did_port_cycle = false;

    try {
        while (Date.now() < deadline) {
            if (!did_port_cycle && (Date.now() - start_time) > timeout_ms / 2) {
                did_port_cycle = true;
                console.log('wait_for_board_ready: halfway, cycling raw ports (close → wait → open)...');
                try {
                    var output_ = get_selected_midi_output_device();
                    var input_ = get_selected_midi_input_device();
                    var rawOut_ = output_ && (output_._midiOutput || output_.output || null);
                    var rawIn_ = input_ && (input_._midiInput || input_.input || null);
                    if (rawOut_ && typeof rawOut_.close === 'function') {
                        console.log('  output before close: connection=' + rawOut_.connection);
                        try { await rawOut_.close(); } catch (e) { console.warn('  output close failed:', e && e.message); }
                        console.log('  output after close: connection=' + rawOut_.connection);
                    }
                    if (rawIn_ && typeof rawIn_.close === 'function') {
                        console.log('  input before close: connection=' + rawIn_.connection);
                        try { await rawIn_.close(); } catch (e) { console.warn('  input close failed:', e && e.message); }
                        console.log('  input after close: connection=' + rawIn_.connection);
                    }
                    // Give Chrome time to release the underlying Windows USB
                    // handle. Too short and Chrome re-uses the cached handle;
                    // ~1.5s has been observed sufficient in similar Chromium
                    // MIDI workarounds.
                    await new Promise(function(r) { setTimeout(r, 1500); });
                    if (rawOut_ && typeof rawOut_.open === 'function') {
                        try { await rawOut_.open(); } catch (e) { console.warn('  output open failed:', e && e.message); }
                        console.log('  output after open: connection=' + rawOut_.connection);
                    }
                    if (rawIn_ && typeof rawIn_.open === 'function') {
                        try { await rawIn_.open(); } catch (e) { console.warn('  input open failed:', e && e.message); }
                        console.log('  input after open: connection=' + rawIn_.connection);
                    }
                    // Counters reset so burst mode fires again against the
                    // hopefully-refreshed ports.
                    consecutive_failures = 0;
                    last_probe_time = 0;
                } catch (e) {
                    console.warn('wait_for_board_ready: port cycle failed:', e);
                }
            }
            // Refresh midi*OptionIds from current WebMidi.inputs/outputs
            // every iteration — but only on Windows. The `connected`
            // WebMidi event is unreliable on Chrome Windows (0 events
            // observed across 30s in repros), so polling is the only
            // way to pick up fresh port ids after a USB reboot.
            //
            // On macOS polling this loop actively HURTS: WebMidi.inputs
            // transiently drops AMYboard during the ~1-3s zB USB reboot
            // while IAC Driver Bus 1 stays present. _refresh_main_midi_dropdowns
            // rebuilds the dropdown with only IAC Driver, falls back to
            // index 0, and the main dropdown sticks on IAC Driver for
            // the rest of the session (prev-name matching is sticky on
            // the wrong port). Leave macOS on the connected-event path —
            // CoreMIDI does fire those reliably.
            if (_IS_WINDOWS_CHROME && typeof _refresh_main_midi_dropdowns === 'function') {
                _refresh_main_midi_dropdowns();
            }

            // If the bound input id changed, rebind listeners and restart
            // the burst.
            var currentInput = null;
            try { currentInput = get_selected_midi_input_device(); } catch (e) {}
            var currentInputId = currentInput ? currentInput.id : null;
            if (currentInputId !== prevInputId) {
                try { await setup_midi_devices(); } catch (e) {}
                prevInputId = currentInputId;
                consecutive_failures = 0;
                last_probe_time = 0;
            }

            var output = get_selected_midi_output_device();
            var port_up = (output && output.state !== 'disconnected');
            var now = Date.now();
            // Should we send this iteration?
            //   - Burst phase: yes, every tick, until MAX_BURST_PINGS failures.
            //   - Probe phase: yes, once every PROBE_INTERVAL_MS.
            var in_burst = consecutive_failures < MAX_BURST_PINGS;
            var probe_due = (now - last_probe_time) >= PROBE_INTERVAL_MS;
            var should_send = port_up && (in_burst || probe_due);
            if (should_send) {
                _send_zi_ping();
                last_probe_time = now;
            }

            // Wait 500ms or until reply arrives — whichever is first.
            // Uses the persistent reply_promise set up at function entry,
            // so a reply arriving during any earlier await (e.g. the
            // halfway close/open cycle's 1.5s sleep) will break us out
            // here too.
            await Promise.race([
                reply_promise,
                new Promise(function(r) { setTimeout(r, 500); })
            ]);
            if (reply_received) {
                console.log('wait_for_board_ready: board is ready after ' +
                            (Date.now() - start_time) + 'ms');
                return true;
            }
            consecutive_failures++;
            console.log('wait_for_board_ready: no reply (t=' + (Date.now() - start_time) + 'ms' +
                        ', failures=' + consecutive_failures +
                        ', sent=' + should_send +
                        ', phase=' + (in_burst ? 'burst' : 'probe') +
                        ', out_state=' + (output ? output.state : 'null') +
                        ', out_conn=' + (output ? output.connection : 'null') +
                        ', out_id=' + (output ? String(output.id).slice(0, 12) : 'null') + ')');
        }
        console.warn('wait_for_board_ready: timed out after ' + timeout_ms + 'ms');
        return false;
    } finally {
        _detach_connected_listener();
        // Release the persistent reply catcher so stray late replies
        // (e.g. from a ping sent just before timeout) don't fire into
        // a dead resolver.
        _ping_resolve = null;
        _ping_reject = null;
    }
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
    // Encode as UTF-8 bytes so multi-byte characters are preserved correctly.
    var encoded = new TextEncoder().encode(text);
    var n = Math.min(length, encoded.length);
    for (var j = 0; j < n; j++) {
        buffer[offset + j] = encoded[j];
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
        params.set("limit", "200");  // headroom so we still have ~100 after filtering
        params.set("latest_per_user_env", "true");
        params.set("item_type", "environment");
        var q = get_world_search_query();
        var tag = get_world_tag_query();
        if (q.length) {
            params.set("q", q);
        }
        if (tag === "generated") {
            // #generated isn't a real tag — it's a shortcut for every sketch by
            // the `generator` user, so filter by username instead of tag.
            params.set("username", "generator");
        } else if (tag.length) {
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
                return item && is_world_environment_filename(item.filename);
            })
            .sort(function(a, b) {
                return Number(b.time || 0) - Number(a.time || 0);
            })
            .slice(0, 100);
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
    var packageName = filename.replace(/\.(py|tar)$/, "");
    // SYNC 2: downloading replaces the current sketch — confirm first.
    var _dlOk = await confirm_environment_action(
        "Download sketch",
        "This will replace your sketch with “" + packageName + "”. Continue?",
        "Download");
    if (!_dlOk) return;
    amyboard_world_loading_index = index;
    render_amyboard_world_file_list();

    try {
        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) throw new Error("Missing file URL");
        var response = await fetch(downloadUrl);
        if (!response.ok) throw new Error("HTTP " + response.status.toString());

        var sketchText = await response.text();
        // SYNC 2: a download only LOADS the sketch into the editor — it does NOT
        // write to the AMYboard (or the simulator). The user reviews it on the
        // Code tab and clicks "Write to your AMYboard" when ready, so any sketch
        // error surfaces only after an explicit write.
        if (editor) {
            editor.setValue(sketchText);
            setTimeout(function () { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
        }
        var envNameInput = document.getElementById("editor_filename");
        if (envNameInput) envNameInput.value = packageName;
        if (typeof set_knobs_from_sketch === 'function') set_knobs_from_sketch();
        // Move to the Code tab so the user can review the sketch and then Write.
        var codeTabBtn = document.getElementById('environment-tab');
        if (codeTabBtn && window.bootstrap) {
            try { new window.bootstrap.Tab(codeTabBtn).show(); } catch (e) {}
        }
        try { await refresh_amyboard_world_files(); } catch (e) {}
    } catch (e) {
        show_alert("Failed to import remote sketch.");
    } finally {
        amyboard_world_loading_index = null;
        render_amyboard_world_file_list();
    }
}



// Load an AMYboard World environment by username and environment name.
// Called from URL params: /editor/?env=woodpiano&user=bwhitman
// Returns true if the sketch was fetched and loaded into the editor.
async function load_world_environment_by_name(username, envName) {
    try {
        var params = new URLSearchParams();
        params.set("username", username);
        params.set("item_type", "environment");
        var response = await fetch(world_api_url("/api/amyboardworld/files?" + params.toString()));
        if (!response.ok) throw new Error("HTTP " + response.status.toString());
        var data = await response.json();
        if (!data || !Array.isArray(data.items) || !data.items.length) {
            show_alert("Sketch '" + envName + "' by " + username + " not found.");
            return false;
        }
        // Match by filename stem (e.g. "spacey" matches "spacey.py" or legacy "spacey.tar")
        var item = null;
        for (var i = 0; i < data.items.length; i++) {
            var fn = data.items[i].filename || "";
            var stem = fn.replace(/\.(py|tar)$/, "");
            if (fn === envName || stem === envName || fn === envName + ".py" || fn === envName + ".tar") {
                item = data.items[i];
                break;
            }
        }
        if (!item) {
            show_alert("Sketch '" + envName + "' by " + username + " not found.");
            return false;
        }
        var filename = normalize_world_filename(item.filename);
        var packageName = filename.replace(/\.(py|tar)$/, "");

        show_world_toast(item.description || envName, username);

        var downloadUrl = resolve_world_download_url(item);
        if (!downloadUrl) throw new Error("Missing file URL");
        var dlResponse = await fetch(downloadUrl);
        if (!dlResponse.ok) throw new Error("HTTP " + dlResponse.status.toString());

        var sketchText = await dlResponse.text();
        // SYNC 2: a shared-link load only LOADS the sketch into the editor — it
        // does not write to the AMYboard (or simulator). The user reviews it on
        // the Code tab and clicks "Write to your AMYboard" when ready.
        if (editor) {
            editor.setValue(sketchText);
            setTimeout(function () { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
        }
        var envNameInput = document.getElementById("editor_filename");
        if (envNameInput) envNameInput.value = packageName;
        if (typeof set_knobs_from_sketch === 'function') set_knobs_from_sketch();
        var codeTabBtn = document.getElementById('environment-tab');
        if (codeTabBtn && window.bootstrap) {
            try { new window.bootstrap.Tab(codeTabBtn).show(); } catch (e) {}
        }
        return true;
    } catch (e) {
        show_alert("Failed to load sketch '" + envName + "' by " + username + ".");
        return false;
    }
}

function show_world_toast_loading(envName) {
    var existing = document.getElementById("world_toast");
    if (existing) existing.remove();
    var toast = document.createElement("div");
    toast.id = "world_toast";
    toast.style.cssText = "position:fixed;top:80px;left:50%;transform:translateX(-50%);z-index:9999;background:#FFDD00;color:#000;padding:1rem 2rem;font-weight:700;font-size:1.1rem;border:3px solid #000;box-shadow:4px 4px 0 #000;max-width:90vw;text-align:center;transition:opacity 0.5s;";
    toast.innerHTML = "Loading <strong>" + envName + "</strong>&hellip;";
    document.body.appendChild(toast);
}

function show_world_toast(description, username) {
    var existing = document.getElementById("world_toast");
    if (existing) existing.remove();
    var toast = document.createElement("div");
    toast.id = "world_toast";
    toast.style.cssText = "position:fixed;top:80px;left:50%;transform:translateX(-50%);z-index:9999;background:#FFDD00;color:#000;padding:1rem 2rem;font-weight:700;font-size:1.1rem;border:3px solid #000;box-shadow:4px 4px 0 #000;max-width:90vw;text-align:center;transition:opacity 0.5s;";
    toast.innerHTML = "Loaded <strong>" + description + "</strong> by " + username;
    document.body.appendChild(toast);
    setTimeout(function() {
        toast.style.opacity = "0";
        setTimeout(function() { toast.remove(); }, 500);
    }, 4000);
}

// Check URL params on startup for environment loading
function check_url_env_params() {
    var params = new URLSearchParams(window.location.search);
    var env = params.get("env");
    var user = params.get("user");
    var tab = params.get("tab"); // "code" or "patch"
    if (env && user) {
        // ?play=1 (marketing-page links): auto-write the sketch to the
        // simulator once it loads and audio starts. Simulate mode only.
        _url_env_autoplay = params.get("play") === "1" && amyboard_mode !== 'control';
        // Show immediate loading toast
        show_world_toast_loading(env);
        // Delay slightly to let micropython init complete
        setTimeout(async function() {
            _url_env_loaded = !!(await load_world_environment_by_name(user, env));
            maybe_autoplay_url_env();
            // Switch to requested tab after environment loads
            if (tab === "code") {
                var codeTab = document.getElementById("environment-tab");
                if (codeTab) {
                    new bootstrap.Tab(codeTab).show();
                    // Re-select sketch.py and refresh CodeMirror after tab is visible.
                    // loadEditor must be false: true would re-read FS sketch.py into
                    // the editor, clobbering the world sketch we just loaded above.
                    setTimeout(async function() {
                        if (list_environment_files().indexOf("sketch.py") !== -1) {
                            await select_environment_file("sketch.py", false);
                        }
                        if (editor) {
                            editor.refresh();
                        }
                    }, 300);
                }
            } else if (tab === "patch") {
                var patchTab = document.getElementById("patch-tab");
                if (patchTab) new bootstrap.Tab(patchTab).show();
            }
        }, 1500);
        // Clean up URL without reload
        window.history.replaceState({}, document.title, window.location.pathname);
    }
}

// Deep-link autoplay: fires the same write_sketch_to_amyboard() the "Write to
// Simulator" button does, once BOTH the deep-link sketch load and start_audio()
// have completed. Either can finish first, so both call sites invoke this.
async function maybe_autoplay_url_env() {
    if (!_url_env_autoplay || !_url_env_loaded || !audio_started) return;
    if (amyboard_mode === 'control') return;
    _url_env_autoplay = false;  // one-shot
    try {
        await write_sketch_to_amyboard();
        console.log('deep-link autoplay: sketch written to simulator');
    } catch (e) {
        console.warn('deep-link autoplay failed:', e);
    }
}

// Remembered admin token for reserved usernames (e.g. "shorepine"). Stored in
// a cookie only after a successful upload so a bad token never gets cached.
var _ADMIN_TOKEN_COOKIE = "amyboard_world_admin_token";

function get_cached_admin_token() {
    var cookies = document.cookie ? document.cookie.split('; ') : [];
    for (var i = 0; i < cookies.length; i++) {
        var c = cookies[i];
        if (c.indexOf(_ADMIN_TOKEN_COOKIE + '=') === 0) {
            try { return decodeURIComponent(c.slice(_ADMIN_TOKEN_COOKIE.length + 1)); }
            catch (e) { return ''; }
        }
    }
    return '';
}

function set_cached_admin_token(token) {
    if (!token) return;
    var value = encodeURIComponent(String(token));
    document.cookie = _ADMIN_TOKEN_COOKIE + '=' + value
        + '; max-age=31536000; path=/; SameSite=Lax';
}

function clear_cached_admin_token() {
    document.cookie = _ADMIN_TOKEN_COOKIE + '=; max-age=0; path=/; SameSite=Lax';
}

// Prompt for admin token via modal. Resolves with token string or null if cancelled.
function prompt_admin_token() {
    return new Promise(function(resolve) {
        var modalEl = document.getElementById("adminTokenModal");
        var input = document.getElementById("admin-token-input");
        var errorDiv = document.getElementById("admin_token_error");
        var submitBtn = document.getElementById("admin-token-submit-btn");
        if (!modalEl || !input || !submitBtn) { resolve(null); return; }
        input.value = "";
        errorDiv.style.display = "none";
        errorDiv.textContent = "";
        var modal = window.bootstrap.Modal.getOrCreateInstance(modalEl);
        var resolved = false;
        function cleanup() {
            freshBtn.removeEventListener("click", onSubmit);
            modalEl.removeEventListener("hidden.bs.modal", onDismiss);
        }
        function onSubmit() {
            var token = (input.value || "").trim();
            if (!token) {
                errorDiv.textContent = "Token cannot be empty.";
                errorDiv.style.display = "block";
                return;
            }
            resolved = true;
            cleanup();
            modal.hide();
            resolve(token);
        }
        function onDismiss() {
            cleanup();
            if (!resolved) resolve(null);
        }
        // Replace button to clear old listeners
        var freshBtn = submitBtn.cloneNode(true);
        submitBtn.parentNode.replaceChild(freshBtn, submitBtn);
        freshBtn.addEventListener("click", onSubmit);
        modalEl.addEventListener("hidden.bs.modal", onDismiss, { once: true });
        modal.show();
    });
}

// Create a js File object and upload it to the AMYboard World API.
// Returns {ok, detail} — detail is a human-readable error when ok is false.
async function amyboard_world_upload_file(pwd, filename, username, description) {
    var contents = await mp.FS.readFile(pwd+filename, {encoding:'binary'});
    var file = await new File([new Uint8Array(contents)], filename, {type: 'application/binary'})
    var data = new FormData();
    data.append('file',file);
    data.append('username', username);
    data.append('description', description);
    var resp;
    try {
        resp = await fetch(world_api_url("/api/amyboardworld/upload"), {
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
    if (mp) ensure_current_environment_layout(false);
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
        show_alert("Sketch Name must be 1-20 chars: only A-Z, a-z, 0-9, - and _.");
        return;
    }

    if (description.length < 1 || description.length > 400) {
        show_alert("Description must be 1-400 characters.");
        return;
    }

    var adminToken = null;
    var adminTokenFromCache = false;
    if (username.toLowerCase() === "shorepine") {
        adminToken = get_cached_admin_token();
        if (adminToken) {
            adminTokenFromCache = true;
        } else {
            adminToken = await prompt_admin_token();
            if (adminToken === null) return;
        }
    }

    var uploadButton = document.getElementById("upload_current_environment_btn");
    if (uploadButton && uploadButton.disabled) {
        return;
    }
    if (uploadButton) {
        uploadButton.disabled = true;
        uploadButton.innerHTML = '<span class="spinner-border spinner-border-sm me-1" role="status" aria-hidden="true"></span>Uploading...';
    }

    // SYNC 2: the knob log is the single source of truth — splice it into the
    // editor's code so the uploaded sketch.py carries the current knobs. No
    // AMY-state grab (zA / update_sketch_knobs) in either mode.
    if (editor) {
        editor.setValue(splice_knobs_into_sketch(editor.getValue(), serialize_knob_log()));
    }
    if (amyboard_mode === 'simulate' && mp && editor) {
        try {
            mp.FS.writeFile(CURRENT_ENV_DIR + '/sketch.py', editor.getValue());
            sync_persistent_fs();
        } catch (e) {
            console.warn('upload: failed to write sketch.py', e);
        }
    }
    // Upload sketch.py content with the environment name as the file key.
    var sketchContent = '';
    if (amyboard_mode === 'simulate' && mp) {
        try {
            sketchContent = mp.FS.readFile(CURRENT_ENV_DIR + '/sketch.py', { encoding: 'utf8' });
        } catch (e) {
            sketchContent = editor ? editor.getValue() : _get_default_sketch();
        }
    } else if (editor) {
        sketchContent = editor.getValue();
    }
    if (!sketchContent || !sketchContent.trim()) {
        show_alert("No sketch content to upload.");
        if (uploadButton) { uploadButton.disabled = false; uploadButton.textContent = "Upload"; }
        return;
    }
    var pyFilename = environmentName + ".py";
    var uploadUrl = world_api_url("/api/amyboardworld/upload");
    function buildUploadBody() {
        var file = new File([sketchContent], pyFilename, { type: "text/x-python" });
        var data = new FormData();
        data.append("file", file);
        data.append("username", username);
        data.append("description", description);
        return data;
    }
    async function doUpload(token) {
        var fetchOpts = { method: "POST", body: buildUploadBody() };
        if (token) fetchOpts.headers = { "X-Admin-Token": token };
        return fetch(uploadUrl, fetchOpts);
    }
    try {
        var response = await doUpload(adminToken);
        // If a cached token was rejected, clear it and prompt for a fresh one.
        if (!response.ok && response.status === 403 && adminTokenFromCache) {
            clear_cached_admin_token();
            adminTokenFromCache = false;
            adminToken = await prompt_admin_token();
            if (adminToken === null) return;
            response = await doUpload(adminToken);
        }
        if (!response.ok) {
            var detail = "";
            try { detail = (await response.json()).detail || ""; } catch (_) {}
            if (response.status === 403) {
                show_alert(detail || "Wrong token.");
            } else {
                show_alert(detail || "Upload failed.");
            }
            return;
        }
        // Upload succeeded — remember the token for next time.
        if (adminToken) set_cached_admin_token(adminToken);
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

function bytes_to_base64_ascii(bytes) {
    var chunk = "";
    for (var i = 0; i < bytes.length; i++) {
        chunk += String.fromCharCode(bytes[i]);
    }
    return btoa(chunk);
}

var _sysex_ack_resolve = null;

async function sysex_write_amy_message(message) {
    var outputDevice = get_selected_midi_output_device() || midiOutputDevice;
    if (!outputDevice) {
        throw new Error("No MIDI output is selected.");
    }
    midiOutputDevice = outputDevice;
    var payload = Array.from(new TextEncoder().encode(String(message || "")));
    // Set up ACK wait before sending so we don't miss a fast reply.
    var ackPromise = new Promise(function(resolve) {
        _sysex_ack_resolve = resolve;
        // Timeout: if no ACK after 5s, proceed anyway. Longer than the
        // 2s we had before because the ACK now fires from the MP callback
        // (after processing the message) not from parse_sysex (on receipt).
        setTimeout(function() {
            if (_sysex_ack_resolve === resolve) { _sysex_ack_resolve = null; resolve(); }
        }, 5000);
    });
    if (typeof outputDevice.sendSysex === "function") {
        outputDevice.sendSysex(AMYBOARD_SYSEX_MFR_ID, payload);
    } else if (typeof outputDevice.send === "function") {
        outputDevice.send([0xF0].concat(AMYBOARD_SYSEX_MFR_ID, payload, [0xF7]));
    } else {
        throw new Error("Selected MIDI output does not support sysex send.");
    }
    // Wait for hardware ACK before sending the next message.
    await ackPromise;
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

// ── Mode detection ──────────────────────────────────────────────────────────
// Priority: URL param > cookie > null (show modal)
function detect_amyboard_mode() {
    var params = new URLSearchParams(window.location.search);
    var urlMode = params.get('mode');
    if (urlMode === 'control' || urlMode === 'simulate') return urlMode;
    var cookie = document.cookie.split('; ').find(function(c) { return c.startsWith('amyboard_mode='); });
    if (cookie) {
        var val = cookie.split('=')[1];
        if (val === 'control' || val === 'simulate') return val;
    }
    return null; // show modal
}

var _detected_mode = detect_amyboard_mode();
var amyboard_mode = _detected_mode || 'simulate';

// Called by the mode selection modal buttons
function select_mode(mode) {
    var remember = document.getElementById('mode_remember');
    if (remember && remember.checked) {
        document.cookie = 'amyboard_mode=' + mode + '; max-age=31536000; path=/; SameSite=Lax';
    }
    // Reload into the chosen mode
    window.location.href = window.location.pathname + '?mode=' + mode;
}

// Called by the "Switch to X" button on the MIDI bar
function switch_amyboard_mode() {
    var newMode = (amyboard_mode === 'control') ? 'simulate' : 'control';
    // Update cookie if one was previously set
    var hasCookie = document.cookie.split('; ').some(function(c) { return c.startsWith('amyboard_mode='); });
    if (hasCookie) {
        document.cookie = 'amyboard_mode=' + newMode + '; max-age=31536000; path=/; SameSite=Lax';
    }
    window.location.href = window.location.pathname + '?mode=' + newMode;
}

// Show the mode selection modal (called if no URL param and no cookie)
function show_mode_modal() {
    var modalEl = document.getElementById('modeSelectionModal');
    if (modalEl && window.bootstrap) {
        var modal = bootstrap.Modal.getOrCreateInstance(modalEl, { backdrop: 'static', keyboard: false });
        modal.show();
    } else {
        // Fallback if Bootstrap not loaded yet — redirect to simulate mode.
        console.warn('show_mode_modal: bootstrap or modal not available, defaulting to simulate');
        window.location.href = window.location.pathname + '?mode=simulate';
    }
}

// Apply UI visibility based on mode (called after DOM ready)
function apply_mode_ui() {
    var isControl = (amyboard_mode === 'control');
    // "Your AMYboard" controls: control mode only.
    var amyboardControls = document.getElementById('amyboard-controls-li');
    if (amyboardControls) { if (isControl) amyboardControls.classList.remove('d-none'); else amyboardControls.classList.add('d-none'); }
    // Simulate controls: simulate mode only.
    var simulateControls = document.getElementById('simulate-controls-li');
    if (simulateControls) { if (!isControl) simulateControls.classList.remove('d-none'); else simulateControls.classList.add('d-none'); }
    // Run controls: simulate has start/stop/clear, control has start/stop (sequencer via MIDI).
    var runControls = document.getElementById('run-controls');
    if (runControls) runControls.style.display = isControl ? 'none' : '';
    var controlRunControls = document.getElementById('control-run-controls');
    if (controlRunControls) { if (isControl) controlRunControls.classList.remove('d-none'); else controlRunControls.classList.add('d-none'); }
    // Hardware widgets (OLED canvas, rotary encoder, CV knobs) — simulate only.
    var hwWidgets = document.getElementById('amyboard-hardware-widgets');
    if (hwWidgets) hwWidgets.style.display = isControl ? 'none' : '';
    // Terminal REPL — simulate only (MicroPython isn't loaded in control mode).
    var replDisclosure = document.getElementById('repl-disclosure');
    if (replDisclosure) replDisclosure.style.display = isControl ? 'none' : '';
    // Audio input toggle (not needed in control mode) — use d-none to override d-flex !important
    var audioIn = document.getElementById('audioin_grow');
    if (audioIn) { if (isControl) audioIn.classList.add('d-none'); else audioIn.classList.remove('d-none'); }
    // MIDI dropdowns + switch button: in control mode, these live in the sync modal instead.
    // In simulate mode, hide the dropdowns when WebMIDI isn't available (the
    // "This page can use WebMIDI on..." notice in #midi-warning-col takes their place).
    var _webmidi_ok = (typeof WebMidi !== 'undefined') && WebMidi.supported;
    var _show_midi_dropdowns = !isControl && _webmidi_ok;
    var midiInputCol = document.getElementById('midi-input-col');
    if (midiInputCol) midiInputCol.style.display = _show_midi_dropdowns ? '' : 'none';
    var midiOutputCol = document.getElementById('midi-output-col');
    if (midiOutputCol) midiOutputCol.style.display = _show_midi_dropdowns ? '' : 'none';
    // MIDI Input Pass-Thru: control mode only.
    var midiPassthruCol = document.getElementById('midi-passthru-col');
    if (midiPassthruCol) {
        if (isControl) midiPassthruCol.classList.remove('d-none');
        else midiPassthruCol.classList.add('d-none');
    }
    var modeSwitchBtn = document.getElementById('mode-switch-btn');
    if (modeSwitchBtn) modeSwitchBtn.parentElement.style.display = isControl ? 'none' : '';
    var portWarning = document.getElementById('amyboard-port-warning');
    if (portWarning) portWarning.style.display = isControl ? 'none' : '';
    // Upgrade Firmware: control mode only.
    var upgradeTab = document.getElementById('upgrade-tab');
    if (upgradeTab) { if (isControl) upgradeTab.parentElement.classList.remove('d-none'); else upgradeTab.parentElement.classList.add('d-none'); }
    // AMYboard World: hide Patches sub-tab bar, only show Environments
    var worldSubTabs = document.getElementById('world-sub-tabs');
    if (worldSubTabs) worldSubTabs.style.display = 'none';
    // Switch button label
    var switchBtn = document.getElementById('mode-switch-btn');
    if (switchBtn) {
        switchBtn.textContent = isControl ? 'Switch to Simulate' : 'Switch to Control';
    }
    // In control mode, set amy_add_message to sysex wrapper
    if (isControl) {
        window.amy_add_message = amy_add_message = function(msg) {
            sysex_write_amy_message(msg);
        };
    }
    // Refresh CodeMirror layout after visibility changes
    if (editor && typeof editor.refresh === 'function') {
        setTimeout(function() { editor.refresh(); }, 50);
    }
}

var _KNOBS_MARKER = '_auto_generated_knobs = """';
var _KNOBS_END = '"""';

// Extract the LAST _auto_generated_knobs = """...""" block from the sketch.
// Using lastIndexOf defends against files that picked up duplicate marker
// blocks from a prior corrupted save — the fresh one is always the latest.
function extract_knobs_from_sketch(text) {
    var start = text.lastIndexOf(_KNOBS_MARKER);
    if (start < 0) return '';
    var bodyStart = start + _KNOBS_MARKER.length;
    var end = text.indexOf(_KNOBS_END, bodyStart);
    if (end < 0) return '';
    return text.slice(bodyStart, end).trim();
}

var _KNOBS_COMMENT_LINE = '# Do not edit. Set automatically by the knobs on AMYboard Online.';

// Remove ALL _auto_generated_knobs = """...""" blocks from the text (and the
// preceding "Do not edit" comment when it's adjacent). Idempotent. Self-heals
// files that accumulated multiple blocks from a prior corrupted save.
function _strip_all_knobs_blocks_js(text) {
    while (true) {
        var start = text.indexOf(_KNOBS_MARKER);
        if (start < 0) break;
        var bodyStart = start + _KNOBS_MARKER.length;
        var end = text.indexOf(_KNOBS_END, bodyStart);
        if (end < 0) {
            // Unterminated block — drop from the marker onward.
            text = text.slice(0, start);
            break;
        }
        var cutFrom = start;
        // Absorb the preceding "# Do not edit..." comment line when it's
        // adjacent (only whitespace between it and the marker).
        var commentIdx = text.lastIndexOf(_KNOBS_COMMENT_LINE, start);
        if (commentIdx >= 0) {
            var gap = text.slice(commentIdx + _KNOBS_COMMENT_LINE.length, start);
            if (gap.replace(/\s/g, '') === '') {
                var lineStart = text.lastIndexOf('\n', commentIdx - 1);
                cutFrom = (lineStart < 0 ? 0 : lineStart + 1);
            }
        }
        var cutTo = end + _KNOBS_END.length;
        while (cutTo < text.length && (text.charAt(cutTo) === ' ' || text.charAt(cutTo) === '\t')) {
            cutTo++;
        }
        if (cutTo < text.length && text.charAt(cutTo) === '\n') {
            cutTo++;
        }
        text = text.slice(0, cutFrom) + text.slice(cutTo);
    }
    return text;
}

function splice_knobs_into_sketch(text, knobsText) {
    // Strip every existing knobs block (defensive self-heal) and append a
    // single fresh block at the end. This guarantees we never leave orphan
    // blocks behind when the source text already has duplicates.
    var cleaned = _strip_all_knobs_blocks_js(text);
    // Ensure the cleaned text ends with exactly one newline.
    while (cleaned.endsWith('\n\n')) {
        cleaned = cleaned.slice(0, -1);
    }
    if (!cleaned.endsWith('\n')) {
        cleaned += '\n';
    }
    var knobsBody = String(knobsText || '');
    if (!knobsBody.endsWith('\n')) knobsBody += '\n';
    return cleaned + '\n' + _KNOBS_COMMENT_LINE + '\n'
        + _KNOBS_MARKER + '\n'
        + knobsBody
        + _KNOBS_END + '\n';
}

async function load_knobs_from_sketch() {
    // Read sketch.py, extract _auto_generated_knobs, send as wire commands.
    var content = "";
    try {
        content = mp.FS.readFile(CURRENT_ENV_DIR + "/sketch.py", { encoding: "utf8" });
    } catch (e) {
        content = "";
    }
    var knobs = extract_knobs_from_sketch(content);
    if (!knobs) {
        // First run: set up channel 1 with default Juno patch 257, 6 voices,
        // and the GM drum kit on channel 10 — the device boot defaults.
        amy_add_log_message("i1ic255Z");
        amy_add_log_message("i1K257iv6Z");
        send_all_knob_cc_mappings(1);
        amy_add_log_message("i" + window.DRUM_CHANNEL + "K" + window.DRUM_DEFAULT_PATCH + "iv6Z");
        return;
    }
    var lines = knobs.split(/\r?\n/);
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i].trim();
        if (!line || line.startsWith("#")) continue;
        amy_add_log_message(line);
    }
}

function generate_knobs_text_js() {
    // Call the C amy_dump_state_to_string via WASM. Returns the state text.
    if (!amy_module || !amy_dump_state_to_string_c || !amy_module.HEAPU8) return '';
    var lenPtr = amy_module._malloc(4);
    var strPtr = amy_dump_state_to_string_c(lenPtr);
    // Read 4-byte little-endian length from HEAPU8 (HEAPU32 isn't exported on this module).
    var h = amy_module.HEAPU8;
    var len = h[lenPtr] | (h[lenPtr + 1] << 8) | (h[lenPtr + 2] << 16) | (h[lenPtr + 3] << 24);
    amy_module._free(lenPtr);
    if (!strPtr || len <= 0) return '';
    var result = read_c_string_from_heap(strPtr, len + 1);
    amy_module._free(strPtr);
    return result;
}

async function _send_text_file_to_amyboard(path, text) {
    // Upload a text file to the AMYboard via zT (base64-chunked over sysex).
    var encoder = new TextEncoder();
    var bytes = encoder.encode(text);
    var fileSize = bytes.length;
    var header = "zT" + path + "," + fileSize + "Z";
    console.log('_send_text_file_to_amyboard: sending header:', header);
    await sysex_write_amy_message(header);
    // sysex_write_amy_message already waited for the ACK (sent from the
    // callback after amy_add_message processed the zT header), so the
    // hardware has already called start_receiving_file_transfer and opened
    // the file. No extra sleep needed.
    var chunks = Math.ceil(fileSize / AMYBOARD_TRANSFER_CHUNK_BYTES);
    console.log('_send_text_file_to_amyboard: sending ' + chunks + ' chunks (' + fileSize + ' bytes)');
    for (var offset = 0; offset < fileSize; offset += AMYBOARD_TRANSFER_CHUNK_BYTES) {
        var chunk = bytes.slice(offset, offset + AMYBOARD_TRANSFER_CHUNK_BYTES);
        var b64 = bytes_to_base64_ascii(chunk);
        await sysex_write_amy_message(b64);
    }
    console.log('sent ' + fileSize + ' bytes to ' + path);
}

// ============================================================================
// Pre-op board check: reachability, liveness, firmware version
// ----------------------------------------------------------------------------
// Every Write/Read in control mode first verifies the board can actually take
// the operation, in escalating depth:
//   1. reachable — replies OK to a zI ping (answered in pure C on the board).
//      Silent -> "Can't reach your AMYboard", abort. Re-checked every op.
//   2. responsive — ACKs an inert zP (needs the MicroPython scheduler, which a
//      runaway sketch loop() starves; such a board still passes check 1).
//      No ACK -> "stuck" warning offering a zB reboot that skips the sketch,
//      after which the op continues. Re-checked every op.
//   3. firmware version — probe the build date (amyboard.report_version() ->
//      'V' sysex) and show a DISMISSABLE notice if it couldn't be detected or
//      is older than the rolling 'amyboard' GitHub release. "Continue"
//      proceeds; "Update Firmware" opens the flasher; dismissed once, it
//      won't nag again this session (checks 1-2 still run every op).
//
// Console entry points for debugging (everything logs under [fwdetect]):
//   await zp_liveness_ping()         -> true/false             (one zP ACK)
//   await probe_amyboard_version()   -> {date, raw} or null   (one round-trip)
//   await check_amyboard_firmware()  -> true/false            (full decision)
// ============================================================================
var _fw_version_resolve = null;   // resolves with the 'V' payload string
var _amyboard_fw_date = null;     // last parsed YYYYMMDD int, or null
var _fw_warned = false;           // once dismissed, don't warn again this session

function _fmt_fw_date(yyyymmdd) {
    if (!yyyymmdd) return 'unknown';
    var s = String(yyyymmdd);
    return (s.length === 8) ? s.slice(0, 4) + '-' + s.slice(4, 6) + '-' + s.slice(6, 8) : s;
}

// Latest released firmware date, fetched once from the rolling 'amyboard' GitHub
// release. null until fetched / on failure (in which case case (b) is skipped).
var _latest_fw_date = null;
var _latest_fw_fetch_started = false;
async function _fetch_latest_fw_date() {
    if (_latest_fw_fetch_started) return _latest_fw_date;
    _latest_fw_fetch_started = true;
    try {
        var resp = await fetch('https://api.github.com/repos/shorepine/tulipcc/releases/tags/amyboard', { cache: 'no-store' });
        if (resp.ok) {
            var j = await resp.json();
            // Prefer the newest firmware-asset updated_at: the rolling release
            // replaces assets on each push but keeps its original published_at.
            var iso = null;
            if (j.assets && j.assets.length) {
                for (var i = 0; i < j.assets.length; i++) {
                    var u = j.assets[i].updated_at;
                    if (u && (!iso || u > iso)) iso = u;
                }
            }
            if (!iso) iso = j.published_at || j.created_at;
            if (iso) {
                var d = String(iso).slice(0, 10).replace(/-/g, '');
                if (/^\d{8}$/.test(d)) _latest_fw_date = parseInt(d, 10);
            }
        }
    } catch (e) { console.warn('[fwdetect] latest fw date fetch failed:', e); }
    console.log('[fwdetect] latest released fw date =', _latest_fw_date);
    return _latest_fw_date;
}

// Direct probe sender — mirrors _send_zi_ping: refresh the MIDIOutput handle
// (defends against the stale port after a flash/USB re-enumerate), then send the
// report_version zP as a raw sysex, bypassing the ack path.
function _send_version_probe() {
    var outputDevice = get_selected_midi_output_device();
    if (!outputDevice) { console.warn('[fwdetect] no MIDI output selected — cannot probe'); return false; }
    try {
        var rawOut = outputDevice._midiOutput || outputDevice.output || null;
        if (rawOut && typeof rawOut.open === 'function') { try { rawOut.open().catch(function(){}); } catch (e) {} }
        var payload = Array.from(new TextEncoder().encode('zPimport amyboard; amyboard.report_version()Z'));
        if (typeof outputDevice.sendSysex === 'function') {
            outputDevice.sendSysex(AMYBOARD_SYSEX_MFR_ID, payload);
        } else if (typeof outputDevice.send === 'function') {
            outputDevice.send([0xF0].concat(AMYBOARD_SYSEX_MFR_ID, payload, [0xF7]));
        } else { return false; }
        console.log('[fwdetect] -> version probe sent (zP report_version)');
        return true;
    } catch (e) { console.warn('[fwdetect] probe send failed (port likely stale):', e); return false; }
}

// One round-trip. Resolves {date, raw} on a 'V' reply, or null on timeout.
function probe_amyboard_version(timeout_ms) {
    timeout_ms = timeout_ms || 2500;
    return new Promise(function(resolve) {
        var done = false;
        var t = setTimeout(function() {
            if (done) return; done = true; _fw_version_resolve = null;
            console.warn('[fwdetect] version probe TIMEOUT after ' + timeout_ms + 'ms (no V reply)');
            resolve(null);
        }, timeout_ms);
        _fw_version_resolve = function(raw) {
            if (done) return; done = true; clearTimeout(t);
            var m = String(raw || '').match(/(\d{8})/);
            var date = m ? parseInt(m[1], 10) : null;
            console.log('[fwdetect] parsed build date =', date, '(' + _fmt_fw_date(date) + ')');
            resolve({ date: date, raw: raw });
        };
        _send_version_probe();   // on send failure we just let it time out
    });
}
window.probe_amyboard_version = probe_amyboard_version;

// Liveness check one level deeper than zI. zI is answered in pure C on the
// board's MIDI task, so a board wedged by a runaway sketch loop() (which
// starves the MicroPython scheduler) still replies OK to zI — but never ACKs
// a zP, and everything a Write actually needs (the transfer completion, the
// environment_transfer_done restart, the version probe) runs on the
// MicroPython side. Sends an inert `zPpassZ` raw (same stale-port defenses as
// _send_version_probe) and resolves true only if the board's AK arrives within
// timeout_ms. Mirrors ping() in tools/amyworld_recorder/amyboard_link.py.
function zp_liveness_ping(timeout_ms) {
    timeout_ms = timeout_ms || 2500;
    return new Promise(function(resolve) {
        var done = false;
        var t = null;
        var onAck = function() { finish(true); };
        function finish(ok) {
            if (done) return;
            done = true;
            if (t) clearTimeout(t);
            if (_sysex_ack_resolve === onAck) _sysex_ack_resolve = null;
            resolve(ok);
        }
        var outputDevice = get_selected_midi_output_device();
        if (!outputDevice) { console.warn('[fwdetect] no MIDI output selected — cannot liveness-ping'); finish(false); return; }
        t = setTimeout(function() { finish(false); }, timeout_ms);
        // Claim the shared ACK slot. Safe: check_amyboard_firmware runs before
        // any transfer, so no sysex_write_amy_message is in flight here.
        _sysex_ack_resolve = onAck;
        try {
            var rawOut = outputDevice._midiOutput || outputDevice.output || null;
            if (rawOut && typeof rawOut.open === 'function') { try { rawOut.open().catch(function(){}); } catch (e) {} }
            var payload = Array.from(new TextEncoder().encode('zPpassZ'));
            if (typeof outputDevice.sendSysex === 'function') {
                outputDevice.sendSysex(AMYBOARD_SYSEX_MFR_ID, payload);
            } else if (typeof outputDevice.send === 'function') {
                outputDevice.send([0xF0].concat(AMYBOARD_SYSEX_MFR_ID, payload, [0xF7]));
            } else { finish(false); }
        } catch (e) {
            console.warn('[fwdetect] liveness ping send failed (port likely stale):', e);
            finish(false);
        }
    });
}
window.zp_liveness_ping = zp_liveness_ping;

// Pre-op board check (see block comment above). Returns true to proceed with
// the operation. Returns false — aborting the op — when the board is
// unreachable, when it's wedged and the user declines (or the recovery reboot
// fails), or when the user chooses "Update Firmware" on a version notice.
// opts.sketchText (optional): the pending Write's sketch, carried into the
// wedged-recovery path so Windows Chrome can resume the upload after the
// page reload its WebMIDI needs following a zB reboot.
async function check_amyboard_firmware(opts) {
    if (amyboard_mode !== 'control') return true;
    // Confirm the board is actually responding BEFORE the _fw_warned gate — a
    // silent board (unplugged, unpowered, or needs RST after a firmware flash)
    // must surface an error and abort the write every time, even in a session
    // where we've already acknowledged a firmware notice. Otherwise the
    // fire-and-forget transfer below "succeeds" against nothing and the user
    // gets no error (see write_sketch_to_amyboard).
    var alive = await wait_for_board_ready(5000);
    if (!alive) {
        console.warn('[fwdetect] board not ready (no zI OK) — not connected; warning + aborting write');
        return await show_firmware_warning(null, null, 'disconnected');
    }
    // The board answered zI — but zI is pure C, so it succeeds even when a
    // runaway sketch loop() has wedged MicroPython. Require a zP ACK too,
    // every op (before the _fw_warned gate — a board can wedge mid-session).
    // Without this, a wedged board fell through to the version probe below and
    // was misreported as "firmware out of date" — seen in the field on a board
    // running the latest firmware.
    var responsive = await zp_liveness_ping(2500);
    if (!responsive) {
        console.log('[fwdetect] no zP ACK — retrying liveness ping once…');
        responsive = await zp_liveness_ping(2500);
    }
    if (!responsive) {
        console.warn('[fwdetect] board replies to zI but not zP — wedged (stuck sketch loop()?); offering reboot');
        return await show_firmware_warning(null, null, 'wedged', opts);
    }
    if (_fw_warned) return true;              // already acknowledged this session
    var res = await probe_amyboard_version(2500);
    if (!res) { console.log('[fwdetect] retrying probe once…'); res = await probe_amyboard_version(2500); }
    var date = res ? res.date : null;
    _amyboard_fw_date = date;
    var latest = await _fetch_latest_fw_date();   // may be null
    var reason = null;
    if (date === null) reason = 'undetected';
    else if (latest && date < latest) reason = 'outdated';
    if (!reason) { console.log('[fwdetect] firmware ok (' + _fmt_fw_date(date) + ')'); return true; }
    console.warn('[fwdetect] firmware notice: reason=' + reason + ' board=' + _fmt_fw_date(date) + ' latest=' + _fmt_fw_date(latest));
    return await show_firmware_warning(date, latest, reason);
}
window.check_amyboard_firmware = check_amyboard_firmware;

// Dismissable warning modal. "Continue" -> resolve true (proceed, don't nag
// again); "Update Firmware" -> open the Upgrade tab and resolve false.
function show_firmware_warning(date, latest, reason, opts) {
    return new Promise(function(resolve) {
        // 'disconnected' means the board never replied (no zI) — not a firmware
        // *version* issue. Unlike the version notices it can't be dismissed with
        // "Continue" (there's nothing to write to), so its dismiss button aborts
        // the write and it never sets _fw_warned (so the next attempt re-checks).
        var isDisconnect = (reason === 'disconnected');
        // 'wedged' means the board answers zI (pure C) but not zP (MicroPython)
        // — a runaway sketch loop() is starving the scheduler. Like
        // 'disconnected' it never sets _fw_warned; unlike it, we can fix it:
        // zB is also handled in pure C, so a reboot-skipping-the-sketch gets
        // through even while wedged. The action button sends it, waits for the
        // board to re-enumerate, and resolves true so the pending op continues
        // (a Write then replaces the stuck sketch).
        var isWedged = (reason === 'wedged');
        var titleEl = document.getElementById('fw-warn-title');
        var bodyEl  = document.getElementById('fw-warn-body');
        if (isDisconnect) {
            if (titleEl) titleEl.textContent = 'Can’t reach your AMYboard';
            if (bodyEl) bodyEl.textContent = 'Your AMYboard didn’t respond. Make sure it’s connected and you’ve pressed RST after upgrading the firmware, then try again.';
        } else if (isWedged) {
            if (titleEl) titleEl.textContent = 'Your AMYboard is stuck';
            if (bodyEl) bodyEl.textContent = 'Your AMYboard is connected but not accepting commands — usually a sketch stuck in a tight loop. Reboot it (the stuck sketch will be skipped) and this operation will continue.';
        } else if (reason === 'undetected') {
            if (titleEl) titleEl.textContent = 'Couldn’t detect your AMYboard firmware';
            if (bodyEl) bodyEl.textContent = 'Your AMYboard didn’t report a firmware version, which usually means it’s running older firmware. Updating is recommended so this editor works correctly. If you want to upgrade to our new improved drum samples, make sure to press “Fully Erase and Re-flash” when upgrading!';
        } else {
            if (titleEl) titleEl.textContent = 'AMYboard firmware update available';
            if (bodyEl) bodyEl.textContent = 'Your AMYboard firmware (' + _fmt_fw_date(date) + ') is older than the latest release (' + _fmt_fw_date(latest) + '). Updating is recommended. If you want to upgrade to our new improved drum samples, make sure to press “Fully Erase and Re-flash” when upgrading!';
        }
        var el = document.getElementById('fwWarnModal');
        // No modal available: abort on disconnect/wedge (can't write to a board
        // that isn't responding); for version notices fall back to the old
        // proceed-once behavior.
        if (!el || !window.bootstrap) { if (isDisconnect || isWedged) { resolve(false); return; } _fw_warned = true; resolve(true); return; }
        var inst = bootstrap.Modal.getOrCreateInstance(el, { backdrop: 'static', keyboard: false });
        var contBtn = document.getElementById('fw-warn-continue');
        var updBtn  = document.getElementById('fw-warn-update');
        // The modal element is reused across reasons — set both labels every time.
        if (contBtn) contBtn.textContent = isDisconnect ? 'OK' : (isWedged ? 'Cancel' : 'Continue');
        if (updBtn)  updBtn.textContent  = isWedged ? 'Reboot AMYboard' : 'Update Firmware';
        function cleanup() { if (contBtn) contBtn.onclick = null; if (updBtn) updBtn.onclick = null; }
        if (contBtn) contBtn.onclick = function() {
            cleanup(); inst.hide();
            if (isDisconnect || isWedged) { console.log('[fwdetect] user: dismiss (' + reason + ') — aborting op'); resolve(false); return; }
            _fw_warned = true; console.log('[fwdetect] user: continue'); resolve(true);
        };
        if (updBtn)  updBtn.onclick  = isWedged ? async function() {
            cleanup(); inst.hide();
            console.log('[fwdetect] user: reboot wedged board (zB)');
            if (_IS_WINDOWS_CHROME) {
                // Same limitation as every zB flow: Windows Chrome WebMIDI can't
                // survive the USB reboot in-document (see
                // _zb_then_reload_with_upload_context). If a Write is pending,
                // stash its sketch so the fresh page finishes the upload; a Read
                // just reloads and the user retries against the unwedged board.
                if (opts && typeof opts.sketchText === 'string') {
                    // setEditor/applyKnobs default true: the reload wipes the
                    // page, so restore editor + knob UI from the written sketch
                    // (suppressKnobCc — the transfer already put them on hw).
                    await _zb_then_reload_with_upload_context({
                        sketchText: opts.sketchText,
                        restart: 'transfer_done',
                        suppressKnobCc: true
                    });
                } else {
                    reboot_to_bootloader();
                    console.log('[fwdetect] Windows Chrome — zB sent, reloading in 4s for fresh MIDIAccess');
                    await sleep_ms(4000);
                    window.location.reload();
                }
                resolve(false);  // unreachable in practice — the reload unloads us
                return;
            }
            _show_resetting_modal('Rebooting AMYboard…');
            reboot_to_bootloader();   // zB: pure C on the board, gets through while wedged
            var back = await wait_for_board_ready();
            _hide_resetting_modal();
            if (!back) {
                console.warn('[fwdetect] board did not come back after zB reboot');
                resolve(await show_firmware_warning(null, null, 'disconnected'));
                return;
            }
            console.log('[fwdetect] board recovered after zB — continuing the op');
            resolve(true);
        } : function() {
            cleanup(); inst.hide();
            var t = document.getElementById('upgrade-tab');
            if (t && window.bootstrap) new bootstrap.Tab(t).show();
            console.log('[fwdetect] user: update firmware');
            resolve(false);
        };
        inst.show();
    });
}

// SYNC 2 — Write to your AMYboard. The knob log is the single source of truth:
// splice it into the editor's code to form the sketch, then transfer it and
// restart the Python sketch (which resets AMY and replays the knob block on top
// of a default synth 1 — see amyboard.py _apply_knobs_text). No implicit pull, no
// merge, no zA, no bootloader reboot: the board becomes exactly what's in this
// editor. Live MIDI/CV tweaks and sketch-code effects are deliberately NOT saved.
async function write_sketch_to_amyboard() {
    if (!editor) return;
    // Sync the knob log <-> editor BOTH ways, then send the editor's sketch
    // VERBATIM. Write must never modify the sketch (it used to splice the in-memory
    // log over the editor's block — which wiped a freshly pasted/edited sketch
    // whose block hadn't been rebuilt into the log). Flush a pending knob-block
    // reflect (recent knob moves -> editor) and a pending user-edit rebuild
    // (pasted/hand-edited block -> log); in practice at most one is pending.
    // This runs BEFORE the board check so the final sketch text exists when the
    // wedged-recovery path needs to carry it across a Windows Chrome reload.
    if (typeof flush_knob_block_reflect === 'function') flush_knob_block_reflect();
    if (typeof flush_log_rebuild_from_editor === 'function') flush_log_rebuild_from_editor();
    var sketchText = editor.getValue();
    // Fail-safe board check — aborts on an unreachable board, a wedged board the
    // user declines to reboot, or a declined firmware notice.
    if (amyboard_mode === 'control' && !(await check_amyboard_firmware({ sketchText: sketchText }))) return;
    // Clear any prior sketch-error indicator — this Write is a fresh attempt; it's
    // re-raised (banner + Code-tab badge) if the new sketch fails to load.
    if (typeof clear_python_error === 'function') clear_python_error();
    if (amyboard_mode === 'control') {
        // Use the race-free resetting modal (toggles classes synchronously). The
        // bootstrap-API saving modal can stick open when show()/hide() bracket a
        // fast operation like this transfer — see _show_resetting_modal's comment.
        _show_resetting_modal('Writing to AMYboard…');
        try {
            // zT stops the sequencer on the board during the transfer, so the
            // running sketch's loop() can't corrupt it — no reboot required.
            await _send_text_file_to_amyboard('/user/current/sketch.py', sketchText);
            // Restart the sketch: resets AMY, replays the knob block, runs code.
            amy_add_log_message('zPimport amyboard; amyboard.environment_transfer_done()Z');
        } catch (e) {
            console.warn('write_sketch_to_amyboard: control failed', e);
        } finally {
            _hide_resetting_modal();
            if (document.activeElement) document.activeElement.blur();
        }
    } else {
        // Simulate: write the file to the in-browser MicroPython FS and restart
        // the sketch — the SAME restart_sketch primitive as hardware (reset AMY,
        // replay knobs, run sketch). No AMY-state dump (update_sketch_knobs).
        if (!mp) return;
        try {
            mp.FS.writeFile(CURRENT_ENV_DIR + '/sketch.py', sketchText);
            sync_persistent_fs();
            await mp.runPythonAsync("import amyboard; amyboard.restart_sketch()");
        } catch (e) {
            console.warn('write_sketch_to_amyboard: simulate failed', e);
        }
        if (editor) setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
    }
}
window.write_sketch_to_amyboard = write_sketch_to_amyboard;

// SYNC 2 — Read from your AMYboard. Pull-only: replace the editor with the
// board's sketch.py and position the knobs from its _auto_generated_knobs block.
// Never writes to the board and never grabs live AMY state (no zA).
async function read_sketch_from_amyboard() {
    if (amyboard_mode === 'control') {
        // Fail-safe firmware check (only aborts if the user declines a confirmed-old board).
        if (!(await check_amyboard_firmware())) return;
        // Clicking "Read from your AMYboard" IS the consent to replace the
        // sketch — skip the pull-gate modal and go straight to the busy
        // spinner. (The gate still fronts the automatic page-load pull.)
        // On failure the sync error modal offers Try again / Set MIDI Ports.
        _skip_pull_gate = true;
        await sync_amy_state();
    } else {
        if (!mp) return;
        var content = '';
        try {
            content = mp.FS.readFile(CURRENT_ENV_DIR + '/sketch.py', { encoding: 'utf8' });
        } catch (e) { content = ''; }
        if (editor) {
            editor.setValue(content);
            setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
        }
        set_knobs_from_sketch();
    }
}
window.read_sketch_from_amyboard = read_sketch_from_amyboard;

// Back-compat alias for existing onclick handlers that still call save_amy_state.
function save_amy_state() { return write_sketch_to_amyboard(); }

// Sync: single request — zD updates sketch.py with live AMY state on hardware, then sends it.
var _sync_stage = null;  // null | 'pending'
var _sync_timeout = null;
var _last_sketch_text = null;

function _refresh_main_midi_dropdowns() {
    // Re-scan WebMidi.inputs/outputs and rebuild the main MIDI dropdowns,
    // preserving the user's current selection by port name where possible.
    // Uses WebMidi.outputs/inputs presence as the enabled check — once the
    // WebMidi.enable() promise has resolved these are always truthy (arrays).
    if (typeof WebMidi === 'undefined' || !WebMidi || !WebMidi.outputs || !WebMidi.inputs) {
        console.warn('_refresh_main_midi_dropdowns: WebMidi not ready');
        return;
    }
    var midi_in = document.amyboard_settings && document.amyboard_settings.midi_input;
    var midi_out = document.amyboard_settings && document.amyboard_settings.midi_output;
    if (!midi_in || !midi_out) return;

    function _name_from_opt(opt) {
        if (!opt) return '';
        var t = opt.text || '';
        var idx = t.indexOf(': ');
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    var prevInName = _name_from_opt(midi_in.options[midi_in.selectedIndex]);
    var prevOutName = _name_from_opt(midi_out.options[midi_out.selectedIndex]);

    midiInputOptionIds = [];
    midiOutputOptionIds = [];

    midi_in.options.length = 0;
    WebMidi.inputs.forEach(function(input) {
        var inputId = safe_midi_port_id(input);
        if (!inputId) return;
        midiInputOptionIds.push(inputId);
        midi_in.options[midi_in.options.length] = new Option("MIDI in: " + safe_midi_port_name(input));
    });

    midi_out.options.length = 0;
    WebMidi.outputs.forEach(function(output) {
        var outputId = safe_midi_port_id(output);
        if (!outputId) return;
        midiOutputOptionIds.push(outputId);
        midi_out.options[midi_out.options.length] = new Option("MIDI out: " + safe_midi_port_name(output));
    });

    // Preselect via the shared control-port priority (see
    // _preselect_control_port): explicit dialog choice, then the AMYboard
    // hardware port, then cookie / regex / previous name. Preferring the
    // hardware port when it (re)appears also un-sticks the dropdown after
    // a zB USB reboot, where a transient empty port list used to leave it
    // stuck on e.g. "IAC Driver Bus 1" forever.
    var savedPorts = typeof _get_midi_cookie === 'function' ? _get_midi_cookie() : null;
    _preselect_control_port(midi_in, savedPorts ? savedPorts.input : null, prevInName,
                            _midi_ports_user_choice ? _midi_ports_user_choice.input : null);
    _preselect_control_port(midi_out, savedPorts ? savedPorts.output : null, prevOutName,
                            _midi_ports_user_choice ? _midi_ports_user_choice.output : null);

    console.log('_refresh_main_midi_dropdowns: rescanned ' +
                midiInputOptionIds.length + ' inputs, ' +
                midiOutputOptionIds.length + ' outputs; ' +
                'main_in=' + midi_in.selectedIndex +
                ' main_out=' + midi_out.selectedIndex);

    // Keep the MIDI Input Pass-Thru dropdown in sync with the new port list.
    if (typeof populate_midi_passthru_dropdown === 'function') {
        populate_midi_passthru_dropdown();
    }

}

function _show_saving_modal() {
    var el = document.getElementById('savingModal');
    if (el && window.bootstrap) {
        bootstrap.Modal.getOrCreateInstance(el, { backdrop: 'static', keyboard: false }).show();
    }
}

function _hide_saving_modal() {
    var el = document.getElementById('savingModal');
    if (!el) return;
    try {
        var m = window.bootstrap && bootstrap.Modal.getOrCreateInstance(el);
        if (m) m.hide();
    } catch (e) {}
    el.classList.remove('show');
    el.style.display = 'none';
    el.setAttribute('aria-hidden', 'true');
    document.querySelectorAll('.modal-backdrop').forEach(function(b) { b.remove(); });
    document.body.classList.remove('modal-open');
    document.body.style.removeProperty('overflow');
    document.body.style.removeProperty('padding-right');
}

// Bypass Bootstrap's Modal API for the resetting modal. Bootstrap schedules
// its show() animation via requestAnimationFrame; for a very fast operation
// like factory_reset (<120ms), the matching hide() can run before the
// animation has processed the show, leaving the modal stuck visible after
// Bootstrap's delayed animation finally fires. Toggling classes/styles
// directly keeps show/hide strictly synchronous.
function _show_resetting_modal(label) {
    var lbl = document.getElementById('resetting-modal-label');
    if (lbl) lbl.textContent = label || 'Resetting\u2026';
    var el = document.getElementById('resettingModal');
    if (!el) return;
    el.classList.add('show');
    el.style.display = 'block';
    el.removeAttribute('aria-hidden');
    el.setAttribute('aria-modal', 'true');
    el.setAttribute('role', 'dialog');
    // Ensure there's a backdrop (Bootstrap normally adds one).
    if (!document.querySelector('.modal-backdrop')) {
        var b = document.createElement('div');
        b.className = 'modal-backdrop fade show';
        document.body.appendChild(b);
    }
    document.body.classList.add('modal-open');
}

function _hide_resetting_modal() {
    var el = document.getElementById('resettingModal');
    if (!el) return;
    el.classList.remove('show');
    el.style.display = 'none';
    el.setAttribute('aria-hidden', 'true');
    el.removeAttribute('aria-modal');
    el.removeAttribute('role');
    document.querySelectorAll('.modal-backdrop').forEach(function(b) { b.remove(); });
    document.body.classList.remove('modal-open');
    document.body.style.removeProperty('overflow');
    document.body.style.removeProperty('padding-right');
}

// Pending green-button gate. _show_syncing_modal returns a promise tied
// to these handles; start_pull_from_amyboard resolves them when the user
// clicks the green button, and modal-dismiss paths reject them.
var _pull_button_resolve = null;
var _pull_button_reject = null;

// One-shot flag: set by sync_modal_retry before re-invoking the work
// path so the next _show_syncing_modal call goes straight to busy state
// instead of re-prompting with the green button. The user already
// chose ports and hit Try again — bouncing them back to a green button
// would force two clicks per retry.
var _skip_pull_gate = false;

function _clear_pull_button_gate(reason) {
    if (_pull_button_reject) {
        var rj = _pull_button_reject;
        _pull_button_resolve = null;
        _pull_button_reject = null;
        rj(new Error(reason || 'cancelled'));
    } else {
        _pull_button_resolve = null;
        _pull_button_reject = null;
    }
}

// Show the syncing modal in its "ready to act" state — green Pull button
// visible, MIDI dropdowns enabled, no spinner. Returns a promise that
// resolves when the user clicks the green button (after MIDI port
// selections have been applied to the main dropdowns and the modal has
// transitioned to the busy/spinner state). Rejects if the modal is
// dismissed via _hide_syncing_modal or an exit button.
//
// Every caller awaits this before issuing sysex traffic — the user
// always has the chance to confirm/change MIDI ports first. This is the
// ONE syncing modal the editor exposes; no separate auto-spinner path.
function _show_syncing_modal() {
    if (amyboard_mode !== 'control') return Promise.resolve();
    var el = document.getElementById('syncingModal');
    if (!el || !window.bootstrap) return Promise.resolve();
    // sync_modal_retry has already applied MIDI ports and validated; go
    // straight to busy state and resolve so the caller's work continues.
    if (_skip_pull_gate) {
        _skip_pull_gate = false;
        _show_syncing_modal_busy();
        return Promise.resolve();
    }
    // Reset to ready-to-act state with green Pull button visible.
    var spinner = document.getElementById('sync-modal-spinner');
    var error = document.getElementById('sync-modal-error');
    var retryBtn = document.getElementById('sync-modal-retry-btn');
    var pullBtn = document.getElementById('sync-modal-pull-btn');
    if (spinner) spinner.classList.add('d-none');
    if (error) error.classList.add('d-none');
    if (retryBtn) retryBtn.classList.add('d-none');
    if (pullBtn) pullBtn.classList.remove('d-none');
    var portsBtnReady = document.getElementById('sync-modal-ports-btn');
    if (portsBtnReady) portsBtnReady.classList.remove('d-none');
    var pullWarn = document.getElementById('sync-modal-pull-warning');
    if (pullWarn) pullWarn.classList.remove('d-none');  // warn: pull replaces the sketch
    bootstrap.Modal.getOrCreateInstance(el, { backdrop: 'static', keyboard: false }).show();
    // Replace any prior pending gate (only one click handler in flight).
    _clear_pull_button_gate('superseded');
    return new Promise(function(resolve, reject) {
        _pull_button_resolve = resolve;
        _pull_button_reject = reject;
    });
}

// Internal helper: transition the modal to its busy/spinner state.
// Used after the green button click and from continuation paths
// (Windows-Chrome post-zB reload) where the click already happened on
// the previous page and we need to resume in busy state.
function _show_syncing_modal_busy() {
    var el = document.getElementById('syncingModal');
    if (el && window.bootstrap) {
        bootstrap.Modal.getOrCreateInstance(el, { backdrop: 'static', keyboard: false }).show();
    }
    var spinner = document.getElementById('sync-modal-spinner');
    var error = document.getElementById('sync-modal-error');
    var retryBtn = document.getElementById('sync-modal-retry-btn');
    var pullBtn = document.getElementById('sync-modal-pull-btn');
    if (spinner) spinner.classList.remove('d-none');
    if (error) error.classList.add('d-none');
    if (retryBtn) retryBtn.classList.add('d-none');
    if (pullBtn) pullBtn.classList.add('d-none');
    var portsBtnBusy = document.getElementById('sync-modal-ports-btn');
    if (portsBtnBusy) portsBtnBusy.classList.add('d-none');
    var pullWarnBusy = document.getElementById('sync-modal-pull-warning');
    if (pullWarnBusy) pullWarnBusy.classList.add('d-none');
}

// Click handler for the green "Pull from AMYboard" button. Applies the
// modal's MIDI port selections back to the main dropdowns, runs
// setup_midi_devices, transitions to busy state, then resolves the
// pending _show_syncing_modal promise so the awaiting caller proceeds.
async function start_pull_from_amyboard() {
    // Refresh against current WebMidi state; the main dropdowns are the
    // single source of truth (port choice lives in the MIDI Ports dialog).
    if (typeof _refresh_main_midi_dropdowns === 'function') {
        _refresh_main_midi_dropdowns();
    }
    try {
        await setup_midi_devices();
    } catch (e) {
        console.warn('start_pull_from_amyboard: setup_midi_devices threw', e);
    }
    if (!midiOutputDevice || !midiInputDevice) {
        _show_syncing_modal_error();
        return;
    }
    _show_syncing_modal_busy();
    if (_pull_button_resolve) {
        var r = _pull_button_resolve;
        _pull_button_resolve = null;
        _pull_button_reject = null;
        r();
    }
}

function _show_syncing_modal_error() {
    var spinner = document.getElementById('sync-modal-spinner');
    var error = document.getElementById('sync-modal-error');
    var retryBtn = document.getElementById('sync-modal-retry-btn');
    var pullBtn = document.getElementById('sync-modal-pull-btn');
    if (spinner) spinner.classList.add('d-none');
    if (error) error.classList.remove('d-none');
    if (pullBtn) pullBtn.classList.add('d-none');
    var portsBtnErr = document.getElementById('sync-modal-ports-btn');
    if (portsBtnErr) portsBtnErr.classList.remove('d-none');
    var pullWarnErr = document.getElementById('sync-modal-pull-warning');
    if (pullWarnErr) pullWarnErr.classList.add('d-none');
    if (retryBtn) {
        retryBtn.classList.remove('d-none');
        retryBtn.onclick = function() {
            sync_modal_retry().catch(function(e) {
                console.warn('sync_modal_retry error:', e);
                _show_syncing_modal_error();
            });
        };
    }
}

function _hide_syncing_modal() {
    var el = document.getElementById('syncingModal');
    if (!el) return;
    // Reject any pending green-button gate so awaiting callers stop
    // waiting (instead of silently leaving an orphaned promise that
    // could resolve later if the modal is reshown).
    _clear_pull_button_gate('hidden');
    // Bootstrap modal.hide() is unreliable when called from async sysex callbacks.
    // Force-remove the modal and backdrop directly.
    try {
        var m = window.bootstrap && bootstrap.Modal.getOrCreateInstance(el);
        if (m) m.hide();
    } catch (e) {}
    el.classList.remove('show');
    el.style.display = 'none';
    el.setAttribute('aria-hidden', 'true');
    document.querySelectorAll('.modal-backdrop').forEach(function(b) { b.remove(); });
    document.body.classList.remove('modal-open');
    document.body.style.removeProperty('overflow');
    document.body.style.removeProperty('padding-right');
}

function dismiss_sync_modal_and_upgrade() {
    _sync_stage = null;
    if (_sync_timeout) { clearTimeout(_sync_timeout); _sync_timeout = null; }
    _hide_syncing_modal();
    var upgradeTab = document.getElementById('upgrade-tab');
    if (upgradeTab && window.bootstrap) {
        var tab = new bootstrap.Tab(upgradeTab);
        tab.show();
    }
}

async function sync_modal_retry() {
    // Re-validate the current (main-dropdown) port selection and re-sync.
    var mainIn = document.amyboard_settings && document.amyboard_settings.midi_input;
    var mainOut = document.amyboard_settings && document.amyboard_settings.midi_output;

    // Refresh the ID arrays against current WebMidi.inputs/outputs before
    // doing anything else — on Windows the AMYboard gets a fresh port id
    // after its post-zB reboot, so the ids in midiInputOptionIds /
    // midiOutputOptionIds from the initial page load are stale.
    // _refresh_main_midi_dropdowns preserves selection by port name so the
    // AMYboard entry auto-aligns with its new id. Without this,
    // get_selected_midi_input_device() returns null below and Try Again
    // fails with "cannot proceed, MIDI devices not ready" even though the
    // dropdowns look right.
    if (typeof _refresh_main_midi_dropdowns === 'function') {
        _refresh_main_midi_dropdowns();
    }

    // Wait for setup_midi_devices to finish wiring up the new selections.
    // It's async but has no internal awaits today, so this resolves synchronously;
    // awaiting it explicitly is just defensive in case that ever changes.
    try {
        await setup_midi_devices();
    } catch (e) {
        console.warn('sync_modal_retry: setup_midi_devices threw', e);
    }

    console.log('sync_modal_retry: main_in=' + (mainIn ? mainIn.selectedIndex : '?') +
                ' main_out=' + (mainOut ? mainOut.selectedIndex : '?') +
                ' ids_in=' + midiInputOptionIds.length +
                ' ids_out=' + midiOutputOptionIds.length +
                ' midiOutputDevice=', midiOutputDevice,
                ' midiInputDevice=', midiInputDevice);

    if (!midiOutputDevice || !midiInputDevice) {
        // Either the port list is empty or WebMidi can't resolve the selected
        // id. Don't try to send sysex — it will just throw. Leave the modal
        // in the error state so the user can pick different ports.
        console.warn('sync_modal_retry: cannot proceed, MIDI devices not ready');
        _show_syncing_modal_error();
        return;
    }

    // Skip the green-button gate on the next _show_syncing_modal call —
    // the user already chose ports and clicked Try again. Reuse the work
    // path (pageload_control_sync / sync_amy_state) so retries converge
    // with the initial flow.
    _skip_pull_gate = true;
    if (amyboard_mode === 'control' && typeof pageload_control_sync === 'function') {
        pageload_control_sync();
    } else {
        sync_amy_state();
    }
}

function sync_modal_open_ports() {
    // Pause any in-progress sync and hand port selection to the single
    // MIDI Ports dialog; on Apply, resume via the normal retry path.
    _sync_stage = null;
    if (_sync_timeout) { clearTimeout(_sync_timeout); _sync_timeout = null; }
    if (_sync_reject) { var r = _sync_reject; _sync_resolve = null; _sync_reject = null; r(new Error('cancelled')); }
    // Cancel any pending ping wait from wait_for_board_ready.
    if (_ping_resolve) { var pr = _ping_resolve; _ping_resolve = null; pr(); }
    show_midi_ports_modal(function() {
        // Skip the green-button gate — the user just chose ports.
        _skip_pull_gate = true;
        if (amyboard_mode === 'control' && typeof pageload_control_sync === 'function') {
            pageload_control_sync();
        } else {
            sync_amy_state();
        }
    });
}

// Cookie helpers for remembering successful MIDI port selections.
function _set_midi_cookie(inputName, outputName) {
    try {
        var value = encodeURIComponent(inputName || '') + '|' + encodeURIComponent(outputName || '');
        document.cookie = 'amyboard_midi_ports=' + value + '; max-age=31536000; path=/; SameSite=Lax';
    } catch (e) {}
}

function _get_midi_cookie() {
    try {
        var c = document.cookie.split('; ').find(function(x) { return x.startsWith('amyboard_midi_ports='); });
        if (!c) return null;
        var v = c.split('=')[1];
        var parts = v.split('|');
        return {
            input: decodeURIComponent(parts[0] || ''),
            output: decodeURIComponent(parts[1] || ''),
        };
    } catch (e) { return null; }
}

function _save_successful_midi_ports() {
    var mainIn = document.amyboard_settings && document.amyboard_settings.midi_input;
    var mainOut = document.amyboard_settings && document.amyboard_settings.midi_output;
    function _opt_portname(sel) {
        if (!sel || !sel.options[sel.selectedIndex]) return '';
        var t = sel.options[sel.selectedIndex].text || '';
        var idx = t.indexOf(': ');
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    _set_midi_cookie(_opt_portname(mainIn), _opt_portname(mainOut));
}

// ── MIDI Ports dialog ───────────────────────────────────────────────────────
// The single place to choose the control-mode MIDI in/out ports. Opened from
// the "MIDI Ports" button next to Read/Write and from the sync modal. The
// default is always the AMYboard hardware port (exact name "AMYboard"); an
// explicit choice made here holds for the rest of the session (including
// across the port re-scans wait_for_board_ready does during reboots).

var _midi_ports_user_choice = null;   // {input, output} names picked in the dialog
var _midi_ports_modal_resume = null;  // continuation after Apply (sync-modal path)

// Shared preselect priority for the control-mode port dropdowns:
//   1. The user's explicit dialog choice this session.
//   2. The AMYboard hardware port (exact name "AMYboard" — the default;
//      exact so e.g. "AMYboard VCV" never outranks real hardware).
//   3. Saved cookie names (covers boards reached via a MIDI interface).
//   4. Any /amyboard/i port (the VCV plugin's virtual port, etc).
//   5. Previous selection by name, then index 0.
function _preselect_control_port(select, savedName, prevName, userName) {
    if (!select || select.options.length === 0) return;
    function _name(opt) {
        var t = (opt && opt.text) || '';
        var idx = t.indexOf(': ');
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    function _pick(test) {
        for (var i = 0; i < select.options.length; i++) {
            if (test(_name(select.options[i]))) { select.selectedIndex = i; return true; }
        }
        return false;
    }
    select.selectedIndex = 0;  // last-resort fallback
    if (userName && _pick(function(n) { return n === userName; })) return;
    if (_pick(function(n) { return n === 'AMYboard'; })) return;
    if (savedName && _pick(function(n) { return n === savedName; })) return;
    if (_pick(function(n) { return /amyboard/i.test(n); })) return;
    if (prevName) _pick(function(n) { return n === prevName; });
}

function _midi_ports_modal_status(text, cls) {
    var el = document.getElementById('midi-ports-modal-status');
    if (!el) return;
    el.textContent = text || '';
    el.className = 'small mt-2' + (cls ? ' ' + cls : '');
}

function show_midi_ports_modal(resumeFn) {
    if (typeof _refresh_main_midi_dropdowns === 'function') {
        _refresh_main_midi_dropdowns();
    }
    var mainIn = document.amyboard_settings && document.amyboard_settings.midi_input;
    var mainOut = document.amyboard_settings && document.amyboard_settings.midi_output;
    var mIn = document.getElementById('midi-ports-modal-in');
    var mOut = document.getElementById('midi-ports-modal-out');
    if (mIn && mainIn) { mIn.innerHTML = mainIn.innerHTML; mIn.selectedIndex = mainIn.selectedIndex; }
    if (mOut && mainOut) { mOut.innerHTML = mainOut.innerHTML; mOut.selectedIndex = mainOut.selectedIndex; }
    _midi_ports_modal_status('');
    _midi_ports_modal_resume = (typeof resumeFn === 'function') ? resumeFn : null;
    var el = document.getElementById('midiPortsModal');
    if (el && window.bootstrap) {
        bootstrap.Modal.getOrCreateInstance(el).show();
    }
}

function hide_midi_ports_modal() {
    _midi_ports_modal_resume = null;
    var el = document.getElementById('midiPortsModal');
    if (!el) return;
    // Same force-hide pattern as _hide_syncing_modal: bootstrap's hide() is
    // unreliable from async callbacks, so also clear the DOM state directly.
    try {
        var m = window.bootstrap && bootstrap.Modal.getOrCreateInstance(el);
        if (m) m.hide();
    } catch (e) {}
    el.classList.remove('show');
    el.style.display = 'none';
    el.setAttribute('aria-hidden', 'true');
    document.querySelectorAll('.modal-backdrop').forEach(function(b) { b.remove(); });
    document.body.classList.remove('modal-open');
    document.body.style.removeProperty('overflow');
    document.body.style.removeProperty('padding-right');
}

function midi_ports_modal_default() {
    // Reset both dropdowns to the AMYboard hardware port (exact name first,
    // then any /amyboard/i port if no hardware is attached).
    ['midi-ports-modal-in', 'midi-ports-modal-out'].forEach(function(id) {
        var sel = document.getElementById(id);
        if (!sel || sel.options.length === 0) return;
        function _name(opt) {
            var t = (opt && opt.text) || '';
            var idx = t.indexOf(': ');
            return (idx >= 0) ? t.slice(idx + 2) : t;
        }
        for (var i = 0; i < sel.options.length; i++) {
            if (_name(sel.options[i]) === 'AMYboard') { sel.selectedIndex = i; return; }
        }
        for (var j = 0; j < sel.options.length; j++) {
            if (/amyboard/i.test(_name(sel.options[j]))) { sel.selectedIndex = j; return; }
        }
    });
    _midi_ports_modal_status('');
}

async function midi_ports_modal_apply() {
    var mainIn = document.amyboard_settings && document.amyboard_settings.midi_input;
    var mainOut = document.amyboard_settings && document.amyboard_settings.midi_output;
    var mIn = document.getElementById('midi-ports-modal-in');
    var mOut = document.getElementById('midi-ports-modal-out');
    function _sel_name(sel) {
        if (!sel || !sel.options[sel.selectedIndex]) return '';
        var t = sel.options[sel.selectedIndex].text || '';
        var idx = t.indexOf(': ');
        return (idx >= 0) ? t.slice(idx + 2) : t;
    }
    // Record the explicit choice FIRST so the port re-scans that
    // wait_for_board_ready triggers below preserve it.
    _midi_ports_user_choice = { input: _sel_name(mIn), output: _sel_name(mOut) };

    // Copy modal selection to the main dropdowns (clamped to valid indexes).
    if (mainIn && mIn) {
        var wantIn = mIn.selectedIndex;
        mainIn.selectedIndex = (wantIn >= 0 && wantIn < mainIn.options.length) ? wantIn
            : (mainIn.options.length > 0 ? 0 : -1);
    }
    if (mainOut && mOut) {
        var wantOut = mOut.selectedIndex;
        mainOut.selectedIndex = (wantOut >= 0 && wantOut < mainOut.options.length) ? wantOut
            : (mainOut.options.length > 0 ? 0 : -1);
    }

    var applyBtn = document.getElementById('midi-ports-apply-btn');
    if (applyBtn) applyBtn.disabled = true;
    try {
        await setup_midi_devices();
    } catch (e) {
        console.warn('midi_ports_modal_apply: setup_midi_devices threw', e);
    }
    if (typeof refresh_amyboard_port_warning === 'function') refresh_amyboard_port_warning();

    if (!midiInputDevice || !midiOutputDevice) {
        _midi_ports_modal_status('Could not open these MIDI ports.', 'text-danger');
        if (applyBtn) applyBtn.disabled = false;
        return;
    }

    // Quick liveness check so the user gets immediate feedback.
    var alive = false;
    _midi_ports_modal_status('Checking for an AMYboard on these ports\u2026', 'text-muted');
    try {
        if (typeof wait_for_board_ready === 'function') {
            alive = await wait_for_board_ready(4000);
        }
    } catch (e) {
        console.warn('midi_ports_modal_apply: wait_for_board_ready threw', e);
    }
    if (applyBtn) applyBtn.disabled = false;

    if (alive) {
        _save_successful_midi_ports();
        var resume = _midi_ports_modal_resume;
        hide_midi_ports_modal();
        if (resume) resume();
    } else {
        // Keep the ports (the user asked for them) but say the board didn't
        // answer, so they can fix cabling or pick again.
        _midi_ports_modal_status('Ports set, but no AMYboard replied. Check the connection, or pick different ports.', 'text-danger');
    }
}

// Post-bootloader portion of reset_amyboard: send zP factory_reset to the
// (already-rebooted) board and reset JS-side state to factory defaults.
// Extracted so it can be called both from the macOS/synchronous path and
// from pageload_control_sync's post-reset-reload branch on Windows, where
// the reset straddles a page reload forced by the Chrome WebMIDI stale-
// handle workaround.
async function _reset_amyboard_send_and_cleanup() {
    amy_add_log_message('zPimport amyboard; amyboard.factory_reset()Z');
    console.log('reset: zP factory_reset sent');
    await sleep_ms(2000);
    // Set JS state to defaults.
    var defaultSketch = "# AMYboard Sketch\n# Code put here runs first, then loop(step) is called every 32nd note,\n# starting on a bar downbeat. step counts 32nd notes on the sequencer's\n# bar-locked grid, so step % 32 == 0 is always a downbeat.\nimport amyboard, amy\n\ndef loop(step):\n    pass\n\n# Do not edit. Set automatically by the knobs on AMYboard Online.\n_auto_generated_knobs = \"\"\"\n\"\"\"\n";
    // SYNC 2: clear the knob log so a later Write doesn't re-emit the old
    // session's knobs (the default sketch has an empty knobs block).
    if (window.knob_log && typeof window.knob_log.clear === 'function') window.knob_log.clear();
    if (typeof clear_sketch_draft === 'function') clear_sketch_draft();
    if (editor) {
        editor.setValue(defaultSketch);
        setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
    }
    if (typeof window.reset_amy_knobs_to_defaults === "function") {
        window.reset_amy_knobs_to_defaults();
    }
    // Reset channel state: channel 1 (Juno) and channel 10 (GM drums —
    // factory_reset's sketch restart re-applies i10K384iv6 on the board) active.
    if (!Array.isArray(window.active_channels)) window.active_channels = [];
    window.active_channels[1] = true;
    for (var _ch = 2; _ch <= 16; _ch++) window.active_channels[_ch] = (_ch === window.DRUM_CHANNEL);
    window.current_synth = 1;
    var channelSelect = document.getElementById("midi-channel-select");
    if (channelSelect) channelSelect.value = "1";
    var activeCheckbox = document.getElementById("channel-active-toggle");
    if (activeCheckbox) activeCheckbox.checked = true;
    if (Array.isArray(window.channel_control_mapping_sent)) {
        for (var _ch2 = 1; _ch2 <= 16; _ch2++) window.channel_control_mapping_sent[_ch2] = false;
    }
    // SYNC 2: position the knobs from the default sketch via the K257 baseline
    // (correct waveforms etc.) instead of leaving them at generic defaults.
    if (typeof set_knobs_from_sketch === "function") {
        set_knobs_from_sketch();
    } else if (typeof window.refresh_knobs_for_channel === "function") {
        window.suppress_knob_cc_send = true;
        try { window.refresh_knobs_for_channel(); } finally { window.suppress_knob_cc_send = false; }
    }
    _hide_resetting_modal();
    if (document.activeElement) document.activeElement.blur();
}

async function reset_amyboard() {
    if (amyboard_mode === 'control') {
        _show_resetting_modal();
        // Windows Chrome workaround: the zB USB reboot leaves Chrome holding
        // a stale MIDI handle with no statechange event, so a post-zB zP
        // factory_reset silently goes into the void. Same fix as pageload:
        // send zB, wait for the board to reboot, reload the page. Post-
        // reload, pageload_control_sync notices the amyboard_post_reset_reload
        // flag and calls _reset_amyboard_send_and_cleanup() — which sends
        // zP factory_reset and resets the JS state — against a fresh
        // MIDIAccess.
        if (_IS_WINDOWS_CHROME) {
            reboot_to_bootloader();
            console.log('reset: Windows Chrome — zB sent, reloading in 4s for fresh MIDIAccess');
            sessionStorage.setItem('amyboard_post_reset_reload', '1');
            await sleep_ms(4000);
            console.log('reset: reloading now');
            window.location.reload();
            return;  // unreachable after reload
        }
        // macOS / others: zB, wait, then send + cleanup synchronously.
        reboot_to_bootloader();
        console.log('reset: zB sent, waiting for board...');
        await wait_for_board_ready();
        await _reset_amyboard_send_and_cleanup();
    } else {
        // Simulate mode: let Python handle the reset (same path as hardware),
        // then refresh JS-side knob/channel state so the UI matches the
        // freshly-reset AMY. A "Resetting…" modal is shown for the duration
        // because mp.runPythonAsync blocks the main thread — without it, the
        // Reset button visually stays "pressed"/red until Python returns.
        if (!mp) return;
        _show_resetting_modal();
        // Yield one frame so the modal actually paints before Python blocks.
        await sleep_ms(50);
        try {
            try {
                await mp.runPythonAsync("import amyboard; amyboard.factory_reset()");
            } catch (e) {
                console.warn('reset: amyboard.factory_reset() failed', e);
            }
            sync_persistent_fs();
            // SYNC 2: clear the knob log so a later Write doesn't re-emit the old
            // session's knobs. factory_reset wrote a default sketch.py (empty
            // knobs block); the UI is reset to defaults below.
            if (window.knob_log && typeof window.knob_log.clear === 'function') window.knob_log.clear();
            if (typeof clear_sketch_draft === 'function') clear_sketch_draft();
            // Reload the sketch into the editor.
            if (editor) {
                try {
                    var sketchContent = mp.FS.readFile(CURRENT_ENV_DIR + '/sketch.py', { encoding: 'utf8' });
                    editor.setValue(sketchContent);
                } catch (e) {
                    console.warn('reset: failed to reload sketch.py into editor', e);
                }
                setTimeout(function() { if (typeof editor.refresh === 'function') editor.refresh(); }, 0);
            }
            // Rebuild JS-side knob state to clean defaults. factory_reset
            // already sent RESET_SYNTHS + set up the default Juno patch on
            // channel 1 in AMY, so channels 2-16 are empty and channel 1
            // holds the Juno values — we reflect that below.
            if (typeof window.reset_amy_knobs_to_defaults === "function") {
                window.reset_amy_knobs_to_defaults();
            }
            // Reset global FX (reverb/chorus/echo/EQ) — Python's RESET_SYNTHS
            // doesn't touch these, so we clear them from the JS side.
            if (typeof reset_global_effects === "function") {
                reset_global_effects();
            }
            // Reset channel active state: channel 1 (Juno) and channel 10
            // (GM drums — amyboard.py's restart re-applies i10K384iv6) active
            // after reset. Assign directly rather than calling
            // set_channel_active() so we don't re-send redundant inits to AMY.
            if (!Array.isArray(window.active_channels)) window.active_channels = [];
            window.active_channels[1] = true;
            for (var _ch = 2; _ch <= 16; _ch += 1) {
                window.active_channels[_ch] = (_ch === window.DRUM_CHANNEL);
            }
            // Reset current synth to 1 and sync the channel-select dropdown.
            window.current_synth = 1;
            var channelSelect = document.getElementById("midi-channel-select");
            if (channelSelect) channelSelect.value = "1";
            var activeCheckbox = document.getElementById("channel-active-toggle");
            if (activeCheckbox) activeCheckbox.checked = true;
            // Drop any cached "already sent knob CC mappings" flags so the
            // next channel switch re-sends them from the fresh default state.
            if (Array.isArray(window.channel_control_mapping_sent)) {
                for (var _ch2 = 1; _ch2 <= 16; _ch2 += 1) {
                    window.channel_control_mapping_sent[_ch2] = false;
                }
            }
            // SYNC 2: position the knobs from the (default) sketch. The K257
            // baseline inside set_knobs_from_sketch gives the amyboard-default knob
            // positions (correct waveforms, filter, ADSR, …) without reading live
            // AMY state.
            if (typeof set_knobs_from_sketch === "function") {
                set_knobs_from_sketch();
            } else if (typeof window.refresh_knobs_for_channel === "function") {
                var previousSuppress = !!window.suppress_knob_cc_send;
                window.suppress_knob_cc_send = true;
                try { window.refresh_knobs_for_channel(); }
                finally { window.suppress_knob_cc_send = previousSuppress; }
            }
        } finally {
            _hide_resetting_modal();
            // The modal backdrop intercepts mouseup so the Reset button's
            // :focus/:active state gets stuck (solid red instead of outline).
            // Blur whatever has focus so the button returns to its normal style.
            if (document.activeElement) document.activeElement.blur();
        }
    }
}

// Promise-based sync that resolves when both sketch + state are received.
var _sync_resolve = null;
var _sync_reject = null;

function sync_amy_state_async() {
    return new Promise(function(resolve, reject) {
        _sync_resolve = resolve;
        _sync_reject = reject;
        sync_amy_state();
    });
}

// ── Control mode: Pull from AMYboard ────────────────────────────────────────
// Pull = zA (update sketch.py with AMY state on disk) then zD (send it back).
// The zA runs update_sketch_knobs() on hardware, then zD reads the file.

// Max time to wait for the zD response. Large sketches (e.g. embedded MIDI
// files) can easily exceed 16 KB once base64-encoded and wrapped in sysex;
// Chrome's Web MIDI can take several seconds to deliver that to the event
// handler, so give it plenty of headroom.
var _SYNC_TIMEOUT_MS = 20000;

async function sync_amy_state() {
    // SYNC 2: pull-only. Read sketch.py straight off the board with zD — the
    // board's sketch.py is the single source of truth, so we no longer send zA
    // to dump live AMY state into it first. Gates behind the green Pull button
    // before sending any sysex.
    console.log('sync_amy_state: start');
    try {
        await _show_syncing_modal();
    } catch (e) {
        console.log('sync_amy_state: gate cancelled', e && e.message);
        // Surface the cancellation to sync_amy_state_async's promise so
        // its caller (e.g. save()) doesn't hang forever.
        if (_sync_reject) { var sr = _sync_reject; _sync_resolve = null; _sync_reject = null; sr(e); }
        return;
    }
    _sync_stage = 'pending';
    if (_sync_timeout) clearTimeout(_sync_timeout);
    // Drop any stale reassembly state from a previous sync attempt that may
    // have timed out mid-message. Starting clean guarantees the reassembler
    // won't accidentally prepend leftover bytes to the fresh zD response.
    _sysex_reasm = null;
    _sysex_b64_accum = '';
    _sync_timeout = setTimeout(function() {
        _sync_timeout = null;
        if (_sync_stage !== null) {
            console.warn('sync_amy_state: TIMEOUT after ' + _SYNC_TIMEOUT_MS + 'ms, stage=' + _sync_stage);
            _sync_stage = null;
            _show_syncing_modal_error();
            if (_sync_reject) { var r = _sync_reject; _sync_resolve = null; _sync_reject = null; r(new Error('sync timeout')); }
        }
    }, _SYNC_TIMEOUT_MS);
    // Pull the file directly. No zA AMY-state grab, so no wait for a device-side
    // update_sketch_knobs() hook — just request sketch.py off the board.
    console.log('sync_amy_state: sending zD');
    amy_add_log_message('zD/user/current/sketch.pyZ');
}

// Strip any leading garbage (U+FFFD replacement chars, control bytes, or
// non-ASCII) from a sketch.py text until we find a plausible Python-file
// starting character. Protects against corruption that would otherwise trip
// a SyntaxError when the hardware imports sketch.py after receiving it back.
function _sanitize_sketch_text(text) {
    if (typeof text !== 'string' || !text.length) return text;
    var i = 0;
    var n = text.length;
    while (i < n) {
        var c = text.charCodeAt(i);
        // Accept tab/newline/CR/space, printable ASCII, or any char > 0x7E
        // that isn't U+FFFD (replacement char emitted by TextDecoder on bad UTF-8).
        if (c === 0x09 || c === 0x0A || c === 0x0D
            || (c >= 0x20 && c <= 0x7E)
            || (c > 0x7E && c !== 0xFFFD)) {
            break;
        }
        i++;
    }
    if (i > 0) {
        console.warn('sync: stripped ' + i + ' leading garbage bytes from sketch.py');
    }
    return text.slice(i);
}

function _handle_sync_sysex_payload(payload) {
    // payload is the sketch.py text, base64-decoded.
    payload = _sanitize_sketch_text(payload);
    // Reject empty/garbage payloads — a stale sysex fragment shouldn't
    // consume the pending sync state and cause the real zD response to
    // be ignored.
    if (!payload || !payload.length) {
        console.warn('sync: ignoring empty payload');
        return;
    }
    if (_sync_timeout) { clearTimeout(_sync_timeout); _sync_timeout = null; }

    if (_sync_stage === 'pending') {
        _sync_stage = null;
        // Self-heal: if the incoming sketch has multiple _auto_generated_knobs
        // blocks OR any "AMYboard Online." fragment outside the usual comment
        // line (from an earlier bad save on hardware that we can't fix until
        // firmware is reflashed), truncate at the first occurrence of the
        // canonical comment line and append a single fresh block built from
        // the most recent wire commands. This normalizes the text before it
        // lands in the editor.
        var markerCount = 0;
        var mcIdx = 0;
        while (true) {
            var mcFound = payload.indexOf(_KNOBS_MARKER, mcIdx);
            if (mcFound < 0) break;
            markerCount++;
            mcIdx = mcFound + _KNOBS_MARKER.length;
        }
        if (markerCount > 1) {
            console.warn('pull: sketch has ' + markerCount + ' knobs blocks, collapsing to the newest');
            // Pull the newest (last) block's content as the canonical knobs.
            var freshKnobs = extract_knobs_from_sketch(payload);  // lastIndexOf -> newest
            // Find the first "# Do not edit. ..." comment line — everything
            // before it is the clean code; everything from it onward is
            // replaced with a single fresh block.
            var cleanBefore;
            var commentStart = payload.indexOf(_KNOBS_COMMENT_LINE);
            if (commentStart >= 0) {
                var lineStart = payload.lastIndexOf('\n', commentStart - 1);
                cleanBefore = payload.slice(0, lineStart < 0 ? 0 : lineStart + 1);
            } else {
                var firstMarker = payload.indexOf(_KNOBS_MARKER);
                cleanBefore = payload.slice(0, firstMarker < 0 ? payload.length : firstMarker);
            }
            while (cleanBefore.endsWith('\n\n')) cleanBefore = cleanBefore.slice(0, -1);
            if (!cleanBefore.endsWith('\n')) cleanBefore += '\n';
            var fk = String(freshKnobs || '');
            if (fk && !fk.endsWith('\n')) fk += '\n';
            payload = cleanBefore
                + '\n' + _KNOBS_COMMENT_LINE + '\n'
                + _KNOBS_MARKER + '\n'
                + fk
                + _KNOBS_END + '\n';
            console.warn('pull: normalized sketch length =', payload.length);
        }
        _last_sketch_text = payload;
        console.log('pull: received ' + payload.length + ' bytes');
        // Set editor content.
        if (editor) {
            editor.setValue(payload);
            setTimeout(function() {
                if (typeof editor.refresh === 'function') editor.refresh();
            }, 0);
        }
        // SYNC 2: rebuild the in-memory knob log from the pulled sketch and
        // position the knobs from it. set_knobs_from_sketch() reads the editor we
        // just set, so a later Write re-emits exactly what we pulled (plus tweaks).
        set_knobs_from_sketch();
        _hide_syncing_modal();
        // Remember the MIDI ports that worked so next page load can preselect.
        try { _save_successful_midi_ports(); } catch (e) {}
        if (_sync_resolve) { var r = _sync_resolve; _sync_resolve = null; _sync_reject = null; r(); }
        return;
    }
}

// Given the zD dump text, parse out per-synth wire lines plus the global
// effects line, mark those synths active, and populate the knob UI for the
// current synth.
function apply_zd_dump_to_knobs(dumpText) {
    // An empty block is valid in SYNC 2: a default channel has no explicit knob
    // lines, but the K257 baseline (prepended below) still positions every knob.
    dumpText = dumpText || '';
    var lines = dumpText.split('\n');
    // Group by synth. Lines starting with 'i{N}' belong to that synth.
    // FX lines (no 'i' prefix) are bus-directed: y<bus>-prefixed lines target
    // that bus; legacy unprefixed lines mean bus 0.
    var perSynth = {};       // synth → array of wire lines (prefix stripped)
    var effectsLines = [];   // bus-directed FX lines
    var activeSynths = {};
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i].trim();
        if (!line) continue;
        var m = line.match(/^i(\d+)(.*)$/);
        if (m) {
            var synth = parseInt(m[1], 10);
            var rest = m[2];
            if (!perSynth[synth]) perSynth[synth] = [];
            perSynth[synth].push(rest);
            activeSynths[synth] = true;
        } else {
            // amy_send() puts the synth token mid-line (e.g. a recorded preset
            // load "K385i2iv6", or a DX7 coda "...G4i1"). Match i<digits> not
            // preceded by a letter or % — two-char codes (iv/in/ic/...) have a
            // non-digit second char and %i templates carry no digits, so this
            // only hits the synth token. Without this, preset K lines fell into
            // the FX bucket and DX7/drum greying was lost on channel switch.
            var sm = /(^|[^A-Za-z%])i(\d+)/.exec(line);
            if (sm) {
                var synthMid = parseInt(sm[2], 10);
                var stripped = line.slice(0, sm.index + sm[1].length) + line.slice(sm.index + sm[0].length);
                if (!perSynth[synthMid]) perSynth[synthMid] = [];
                perSynth[synthMid].push(stripped);
                activeSynths[synthMid] = true;
            } else {
                effectsLines.push(line);
            }
        }
    }
    // Compute each channel's mix bus from its own lines (last y<bus> wins;
    // absent = bus 0, matching a fresh K257 instrument). Done BEFORE the CC
    // mapping pass below, which files FX mappings under the channel's bus.
    if (typeof window.set_channel_bus_local === 'function'
        && typeof events_from_wire_code_messages === 'function') {
        for (var chB = 1; chB <= 16; chB++) {
            var chBus = 0;
            if (perSynth[chB]) {
                var busEvents = events_from_wire_code_messages(perSynth[chB]);
                for (var be = 0; be < busEvents.length; be++) {
                    if (Number.isFinite(busEvents[be].bus)) chBus = busEvents[be].bus;
                }
            }
            window.set_channel_bus_local(chB, chBus);
        }
        if (typeof window.syncBusSelectForChannel === 'function') window.syncBusSelectForChannel();
        if (typeof window.syncFxLevelForBus === 'function') window.syncFxLevelForBus();
    }
    // Apply CC mappings (ic lines) to per-channel knob config so CC numbers,
    // ranges, log flag, and offset reflect the saved sketch state.
    if (typeof window.apply_knob_cc_mappings_from_patch_source === "function") {
        for (var chCC = 1; chCC <= 16; chCC++) {
            window.apply_knob_cc_mappings_from_patch_source(chCC, dumpText);
        }
    }
    // Active state is OWNED by the caller: set_knobs_from_sketch sets the
    // boot-aware window.active_channels before calling us (channel 1 on by
    // default; i<ch>iv0 off). We must NOT overwrite it from the dump's "has any
    // i<N> line" signal — that would clear channel 1 for an empty/default block
    // (the very case that broke), and wrongly mark a deactivated i<ch>iv0 channel
    // as active. Fall back to the dump only for channels the caller never set.
    var currentSynth = Number(window.current_synth || 1);
    var bootActive = (Array.isArray(window.active_channels) && window.active_channels[currentSynth] !== undefined)
        ? !!window.active_channels[currentSynth]
        : !!activeSynths[currentSynth];
    var checkbox = document.getElementById('channel-active-checkbox');
    if (checkbox) checkbox.checked = bootActive;
    // Grey/ungrey knob sections from the channel's effective patch: the last
    // K<n> line wins (e.g. a K257 preset re-enables); with no K line the channel
    // holds its boot default — the GM drum kit on channel 10, K257 elsewhere.
    // In SYNC 2 the block carries only the patch number (K128-255 = DX7 presets,
    // K258/K384+ = drums), not a full per-voice osc dump, so detect from K.
    // Done before the !bootActive return so switching to an inactive channel
    // doesn't keep the previous channel's greyed sections.
    if (typeof window.update_knob_sections_for_patch === "function") {
        var effPatch = (currentSynth === window.DRUM_CHANNEL) ? window.DRUM_DEFAULT_PATCH : 257;
        var kLines = perSynth[currentSynth] || [];
        for (var ki = 0; ki < kLines.length; ki++) {
            var km = /K(\d+)/.exec(kLines[ki]);
            if (km) effPatch = parseInt(km[1], 10);
        }
        window.update_knob_sections_for_patch(effPatch);
    }
    // Populate knobs for the current synth.
    if (!bootActive) {
        console.log('apply_zd_dump_to_knobs: synth ' + currentSynth + ' is not active');
        return;
    }
    if (typeof window.set_knobs_from_events !== 'function' ||
        typeof events_from_wire_code_messages !== 'function') {
        console.warn('apply_zd_dump_to_knobs: knob helpers unavailable');
        return;
    }
    // Prepend the K257 amyboard-default baseline so a synth with no (or only
    // setup) lines still positions every knob; the synth's own lines then
    // override specific knobs on top (last-wins in set_knobs_from_events).
    var baseline = (typeof k257_default_wire_baseline === 'function') ? k257_default_wire_baseline() : '';
    var wireLines = (baseline ? [baseline] : []).concat(perSynth[currentSynth] || []).concat(effectsLines);
    try {
        var events = events_from_wire_code_messages(wireLines);
        window.set_knobs_from_events(events, currentSynth);
        if (typeof window.refresh_knobs_for_channel === 'function') {
            window.suppress_knob_cc_send = true;
            try { window.refresh_knobs_for_channel(); } finally { window.suppress_knob_cc_send = false; }
        }
        console.log('apply_zd_dump_to_knobs: synth ' + currentSynth + ' → ' + wireLines.length + ' wire lines → ' + events.length + ' events');
    } catch (e) {
        console.warn('apply_zd_dump_to_knobs: parse/apply failed', e);
    }
}

// Position the CURRENT channel's knobs from the in-memory knob log (+ the K257
// baseline), suppressed so nothing is sent to AMY. Reads the live log rather than
// the editor text, so it's correct even before the debounced editor reflect has
// run (e.g. right after (re)activating a channel). Used on channel switch and
// activate to show a channel's knobs without dumping any commands to AMY.
function position_current_channel_from_log() {
    if (typeof apply_zd_dump_to_knobs !== 'function') return;
    var prev = window.suppress_knob_cc_send;
    window.suppress_knob_cc_send = true;
    try {
        apply_zd_dump_to_knobs(serialize_knob_log());
    } finally {
        window.suppress_knob_cc_send = prev;
    }
}
window.position_current_channel_from_log = position_current_channel_from_log;

// SYNC 2 legacy alias: there is no cached zD dump any more (that path was removed).
// Position the current channel's knobs from the knob log instead. Kept so any
// remaining caller of the old name works (and never throws on _last_zd_dump_text).
window.reapply_last_zd_dump = function() {
    position_current_channel_from_log();
};

// Start/stop the sequencer transport. Mirrors set_tempo_from_ui(): in control
// mode we send a zP python-exec sysex to the board; in simulate mode we run the
// same python directly in the wasm MicroPython. Both call sequencer.start()/stop()
// (-> tulip.sequencer_start/stop -> AMY sequencer_midi_start/stop), which works
// regardless of MIDI clock sync state (tulip.external_midi_sync). We deliberately
// no longer send raw MIDI Start/Stop (0xFA/0xFC): AMY now ignores those unless the
// user has enabled external MIDI sync, which is what broke these buttons.
function control_sequencer_start() {
    if (amyboard_mode === 'control') {
        amy_add_log_message('zPimport sequencer; sequencer.start()Z');
    } else {
        runCodeBlock('import sequencer; sequencer.start()');
    }
}

function control_sequencer_stop() {
    if (amyboard_mode === 'control') {
        amy_add_log_message('zPimport sequencer; sequencer.stop()Z');
    } else {
        runCodeBlock('import sequencer; sequencer.stop()');
    }
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

 function get_output_audio_samples() {
     var am = globalThis._amy_module_ref;
     var ptr = globalThis._amy_res_ptr_out;
     if (!am || !am._amy_get_output_buffer || !ptr) {
         return new Uint8Array(1024);
     }
     var n = am._amy_get_output_buffer(ptr);
     var result = new Uint8Array(1024);
     if (n > 0) {
         result.set(am.HEAPU8.subarray(ptr, ptr + 1024));
     }
     return result;
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
  await mp.registerJsModule('amy_get_output_buffer', get_output_audio_samples);
  // AMY WASM lives in a separate module; bridge amy_dump_state for Python via JS.
  await mp.registerJsModule('amy_dump_state_js', generate_knobs_text_js);
//  await mp.registerJsModule('amy_get_input_buffer', get_audio_samples);
//  await mp.registerJsModule('amy_set_external_input_buffer', set_audio_samples);

  // time.sleep on this would block the browser from executing anything, so we override it to a JS thing
  mp.registerJsModule("jssleep", sleep_ms);

  // Set up the micropython context for AMY.
  await mp.runPythonAsync(`
    import tulip, amy, amy_js_message, amy_sysclock, amy_get_output_buffer as _amy_get_output_buf_js, amy_dump_state_js as _amy_dump_state_js
    amy.override_send = amy_js_message
    amy.ticks_ms = amy_sysclock
    tulip.amy_ticks_ms = amy_sysclock
    tulip.amy_get_output_buffer = _amy_get_output_buf_js
    tulip.amy_dump_state = _amy_dump_state_js
  `);

  // If you don't have these sleeps we get a MemoryError with a locked heap. Not sure why yet.
  await sleep_ms(400);
  await mp.runFrozenAsync('_boot.py');
  ensure_current_environment_layout(true);

  // Wait for AMY WASM to be ready (so we can read sketch.py and eventually send messages).
  if (_amy_wasm_ready) await _amy_wasm_ready;

  // Load sketch.py into the editor so the user can see/edit it before clicking to start audio.
  // The actual run_sketch() (applying knobs + starting loop) is deferred to start_audio().
  // Use a persistent module-level flag (set before check_url_env_params mutates
  // window.location.search) so start_audio() later still knows the deep-link
  // sketch is pending and doesn't race a default run_sketch() against it.
  _url_env_pending = !!(new URLSearchParams(window.location.search).get("env"));
  if (!_url_env_pending) {
    // SYNC 2: restore the editor from the local draft (or default) — not the FS
    // sketch.py. The draft is the persistent editor content across reloads.
    restore_sketch_into_editor();
  }
  await fill_tree();
  amyboard_started = true;
}

var _audio_starting = false;
async function start_audio() {
  document.body.removeEventListener('click', start_audio, true);
  document.body.removeEventListener('keydown', start_audio, true);

  // Don't start local audio in control mode
  if (amyboard_mode === 'control') return;
  // Don't run this twice (guard both completed and in-progress starts).
  if (audio_started || _audio_starting) return;
  _audio_starting = true;
  // Start the audio worklet (miniaudio)
  if(amy_audioin_toggle) {
      await amy_live_start_web_audioin();
  } else {
      await amy_live_start_web();
  }
  // Allow a few audio callbacks to fire so amy_execute_deltas processes
  // any wire commands that were queued before audio started.  Without this
  // delay, yield_synth_commands would read stale default values for global
  // effects.
  await new Promise(function(resolve) { setTimeout(resolve, 150); });
  // Set up AnalyserNode to capture output waveform for Python
  try {
    if (typeof emAudio !== 'undefined') {
      var actx = null, workletNode = null;
      for (var k in emAudio) {
        var obj = emAudio[k];
        if (obj && obj.constructor && obj.constructor.name === 'AudioContext') actx = obj;
        if (obj && obj.constructor && obj.constructor.name === 'AudioWorkletNode') workletNode = obj;
      }
      console.log("emAudio scan: AudioContext=", !!actx, "WorkletNode=", !!workletNode);
      if (actx && workletNode) {
        var analyser = actx.createAnalyser();
        analyser.fftSize = 512;
        workletNode.connect(analyser);
        // Don't disconnect workletNode from destination - just add a parallel tap
        globalThis._amy_analyser = analyser;
        console.log("AnalyserNode connected for waveform capture");
      }
    }
  } catch(e) { console.warn("AnalyserNode setup failed:", e); }

  audio_started = true;

  // Audio is now running and AMY can process messages — run sketch.py the same
  // way hardware does: apply _auto_generated_knobs, import sketch.py, start loop().
  // Read the persistent flag (set in start_amyboard before check_url_env_params
  // cleared the URL query string) — do NOT re-parse window.location.search here,
  // it's been mutated by that point and would falsely return false, causing us
  // to race a default run_sketch() against check_url_env_params's deferred load.
  if (mp && !_url_env_pending) {
    // Init synth 1 with default Juno patch (and the GM drum kit on channel 10)
    // before run_sketch applies knobs. Without it, synth 1 doesn't exist and
    // _apply_knobs_text fails with "synth not defined".
    amy_add_message("i1K257iv6Z");
    amy_add_message("i" + window.DRUM_CHANNEL + "K" + window.DRUM_DEFAULT_PATCH + "iv6Z");
    // Let AMY process the synth init before running Python.
    await new Promise(function(resolve) { setTimeout(resolve, 100); });
    try {
      await mp.runPythonAsync(`
import sys, amyboard
try:
    amyboard.run_sketch()
except Exception as _e:
    print("run_sketch exception:")
    sys.print_exception(_e)
`);
    } catch (e) {
      console.warn("run_sketch outer failure:", e);
    }
    // Pull the _auto_generated_knobs text from sketch.py and update the JS
    // knob config (CC numbers, ranges, log flag) so the UI reflects the
    // saved ic mappings. Each knob line is channel-prefixed (i{N}...), so
    // apply per-channel.
    try {
      var sketchText = mp.FS.readFile(CURRENT_ENV_DIR + '/sketch.py', { encoding: 'utf8' });
      var knobsText = extract_knobs_from_sketch(sketchText);
      if (knobsText && typeof window.apply_knob_cc_mappings_from_patch_source === "function") {
        for (var ch = 1; ch <= 16; ch++) {
          window.apply_knob_cc_mappings_from_patch_source(ch, knobsText);
        }
      }
    } catch (e) {
      console.warn("apply knobs-text on boot failed:", e);
    }
    // Wait for AMY audio worklet to process the queued events from
    // _apply_knobs_text (synth init + wire code). Without this, the synth
    // has no voices yet and yield_synth_commands fails.
    await new Promise(function(resolve) { setTimeout(resolve, 200); });
    // Sync UI knobs from the AMY state that run_sketch just applied.
    try { await sync_channel_knobs_from_synth_to_ui(window.current_synth || 1); } catch (e) {}
  }
  // Deep-link ?play=1: if the sketch already loaded, write it to the simulator
  // now that audio is running (the load-side call fires when audio wins the race).
  await maybe_autoplay_url_env();
}

// ---- wire_code_parser.js ----
function parse_wire_code(message) {

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

  // Build reverse lookup (wire code → [fieldname, type]) from AMY_KW_MAP (source of truth from __init__.py).
  let codes = new Map();
  for (const [fieldname, mapping] of Object.entries(AMY_KW_MAP)) {
    codes.set(mapping.wire, [fieldname, mapping.type]);
  }
  codes.set("Z", ["end", "Z"]);
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
    if (pushback != null) {
      [lexcode, token, rest] = pushback;
      pushback = null;
    } else {
      [lexcode, token, rest] = next_token(rest);0
    }
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
  // Special-case handling of eg/bp0 and 1
  // Accept eg0/1 as aliases of bp0/1, but normalize here.
  if ("eg0" in result) {result["bp0"] = result["eg0"]; delete result["eg0"]; }
  if ("eg1" in result) {result["bp1"] = result["eg1"]; delete result["eg1"]; }
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
  // Safety-capped: if parse_wire_code ever returns without advancing (or we
  // blow past a sane event count), bail out so we can never wedge the JS event
  // loop on a malformed message.
  events = [];
  let rest = message;
  const MAX_EVENTS = 5000;
  let iterations = 0;
  while (rest != "") {
    const prev_rest = rest;
    [event, rest] = parse_wire_code(rest);
    events.push(event);
    if (rest === prev_rest) {
      console.warn("events_from_wire_code_message: parse_wire_code did not advance at:", String(rest).substring(0, 80));
      break;
    }
    if (++iterations >= MAX_EVENTS) {
      console.warn("events_from_wire_code_message: bailed after " + MAX_EVENTS + " events");
      break;
    }
  }
  return events;
}

function events_from_wire_code_messages(messages) {
  // Messages is an array of wire code message strings; all the events contained are concatenated.
  // Per-message try/catch: if one wire line is malformed (corruption from a
  // prior partial save, a stray comment that leaked through, etc.), we log
  // and skip it instead of failing the whole batch.
  var all_events = [];
  for (const message of messages) {
    try {
      let message_events = events_from_wire_code_message(message);
      for (const event of message_events) {
        all_events.push(event);
      }
    } catch (e) {
      console.warn('events_from_wire_code_messages: skipping malformed line:', message, e && e.message);
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

