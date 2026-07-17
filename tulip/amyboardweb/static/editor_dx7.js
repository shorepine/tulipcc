// editor_dx7.js — the DX7 knob surface for FM preset channels (K128-255).
//
// A DX7 preset patch is 8 oscillators on the synth (see amy fm.py, which
// generated the patches.h strings):
//
//   osc 0  the ALGO osc: wave ALGO, algorithm (o, 1-32), feedback (b),
//          algo_source O2,3,4,5,6,7, bp0 = the PITCH envelope (5 time,ratio
//          pairs), freq coefs "0,1,0,1,0,<pitch lfo amp>" (NOTE + EG0 + MOD),
//          mod_source 1. bp1/EG1 is unused by fm.py — the preset-load VCF coda
//          (editor/index.html) claims it for the filter envelope, so the VCF /
//          VCF ENV sections use the SAME change codes as the Juno surface
//          (F/R/B on osc 0).
//   osc 1  the LFO: wave, freq const.
//   osc 2-7  operators — in fm.py order op6 first, so osc = 8 - opN
//          (algorithms.c's tables are op6..op1 too). Each has amp coefs
//          "<op level>,0,0,1,0,<amp lfo>" (CONST + EG0 + MOD), bp0 = the amp
//          envelope (5 time,level pairs), and either I<ratio> (ratio-tuned)
//          or f<hz> (fixed-frequency), per the original DX7 op.
//
// The surface is one scrollable grid (drum-view height), two sections per row:
//
//   FM        | PITCH ENV        (algorithm/feedback/LFO + pitch env knobs)
//   VCF       | VCF ENV
//   ENV PLOT  | ENV PLOT         (all 7 envelope plots: pitch + op 1-6)
//   OP 1      | OP 1 AMP ENV
//   ...       | ...
//   OP 6      | OP 6 AMP ENV
//
// The plots redraw live as their envelope knobs move.
//
// Envelope knobs edit the AMY representation directly: 4 segment times (ms)
// and 4 target levels, i.e. bp0 pairs 1-4. Pair 0 is (0, initial level) and
// fm.py always sets its level to the release level, so the L4 knob writes
// both fields (its change code carries two %v slots).
//
// Every knob has a real change_code, so moves send + record through the
// standard send_change_code path (one last-wins knob-log slot per parameter,
// same structural key as a line loaded from a sketch) and device-side MIDI CC
// mappings work (ic lines, unlike drum knobs' io templates). Values position
// from the preset's patches.h wire string overlaid with the channel's
// knob-log lines — the same source the device boots from.

(function() {
  var MAIN_OSC = 0;
  var LFO_OSC = 1;
  var NUM_OPS = 6;
  var BP_UNSET = 32767;

  function op_osc(opN) { return 8 - opN; }  // Op 1 -> osc 7 ... Op 6 -> osc 2

  // ── per-channel DX7 view state ──────────────────────────────────────────────
  // _channel_dx7_patch[ch] = preset number while the channel's patch is a DX7
  // preset (128-255), else null. Owned by update_knob_sections_for_patch.
  var _channel_dx7_patch = {};
  // _dx7_state[ch] = { patch, knobs, plots } built lazily per (channel, patch);
  // knob configs persist so CC assignments and range edits survive re-renders.
  // Values are re-read by refresh_dx7_knob_values().
  var _dx7_state = {};

  window.set_channel_dx7_patch = function(channel, patchNumber) {
    var ch = Number(channel);
    if (!Number.isInteger(ch) || ch < 1 || ch > 16) return;
    _channel_dx7_patch[ch] = Number.isInteger(patchNumber) ? patchNumber : null;
  };

  window.get_channel_dx7_patch = function(channel) {
    var pn = _channel_dx7_patch[Number(channel)];
    return Number.isInteger(pn) ? pn : null;
  };

  // ── wire events -> per-osc parameter state ──────────────────────────────────

  function empty_osc() {
    return { amp: {}, freq: {}, ff: {}, eg0t: {}, eg0v: {}, eg1t: {}, eg1v: {},
             ratio: null, wave: null, algorithm: null, feedback: null, resonance: null };
  }

  function merge_list(dst, src) {
    if (!Array.isArray(src)) return;
    for (var i = 0; i < src.length; i++) {
      if (Number.isFinite(src[i]) && src[i] !== BP_UNSET) dst[i] = src[i];
    }
  }

  function merge_events(state, events) {
    for (var i = 0; i < events.length; i++) {
      var ev = events[i];
      if (!ev || !Number.isFinite(ev.osc)) continue;
      var o = state[ev.osc] || (state[ev.osc] = empty_osc());
      if (Number.isFinite(ev.ratio)) o.ratio = ev.ratio;
      if (Number.isFinite(ev.wave)) o.wave = ev.wave;
      if (Number.isFinite(ev.algorithm)) o.algorithm = ev.algorithm;
      if (Number.isFinite(ev.feedback)) o.feedback = ev.feedback;
      if (Number.isFinite(ev.resonance)) o.resonance = ev.resonance;
      merge_list(o.amp, ev.amp);
      merge_list(o.freq, ev.freq);
      merge_list(o.ff, ev.filter_freq);
      merge_list(o.eg0t, ev.eg0_times);
      merge_list(o.eg0v, ev.eg0_values);
      merge_list(o.eg1t, ev.eg1_times);
      merge_list(o.eg1v, ev.eg1_values);
    }
  }

  function patch_events(patchNumber) {
    var codes = window.patch_code_for_patch_number;
    var s = (codes && typeof codes[patchNumber] === "string") ? codes[patchNumber] : "";
    if (!s || typeof events_from_wire_code_messages !== "function") return [];
    return events_from_wire_code_messages([s]);
  }

  // The channel's knob-log lines as osc events, in log (= replay) order. The
  // synth token is stripped the same way apply_zd_dump_to_knobs does; ic/io
  // lines contribute nothing osc-shaped and drop out in merge_events.
  function channel_log_events(ch) {
    var out = [];
    if (!(window.knob_log instanceof Map) || typeof events_from_wire_code_messages !== "function") return out;
    window.knob_log.forEach(function(entry) {
      if (!entry || Number(entry.synth) !== ch) return;
      var line = String(entry.line || "");
      if (!line) return;
      var sm = /(^|[^A-Za-z%])i(\d+)/.exec(line);
      if (sm && parseInt(sm[2], 10) === ch) {
        line = line.slice(0, sm.index + sm[1].length) + line.slice(sm.index + sm[0].length);
      }
      if (!line || /^io\d/.test(line) || /^ic/.test(line)) return;
      var evs = events_from_wire_code_messages([line]);
      for (var i = 0; i < evs.length; i++) out.push(evs[i]);
    });
    return out;
  }

  // Baseline (patches.h wire string) overlaid with the channel's logged lines.
  function compute_osc_state(ch, patchNumber) {
    var state = {};
    merge_events(state, patch_events(patchNumber));
    merge_events(state, channel_log_events(ch));
    return state;
  }

  function osc_of(state, osc) { return state[osc] || empty_osc(); }

  function val(v, fallback) { return Number.isFinite(v) ? v : fallback; }

  // ── knob configs ────────────────────────────────────────────────────────────

  var LFO_WAVE_OPTIONS = ["SINE", "PULSE", "SAW_DOWN", "SAW_UP", "TRIANGLE", "NOISE"];
  var LFO_WAVE_VALUES = [0, 1, 2, 3, 4, 5];

  // bp0 field-position change code: "A" with the value at pair-list index
  // `idx` (times at even, levels at odd indices). e.g. idx 4 -> "A,,,,%v".
  function bp0_code(osc, idx) {
    return "i%iv" + osc + "A" + new Array(idx + 1).join(",") + "%v";
  }
  // The L4 (release level) code also writes pair 0's level, which fm.py keys
  // to the release level (the envelope's resting point).
  function bp0_release_level_code(osc) {
    return "i%iv" + osc + "A,%v,,,,,,,,%v";
  }

  // One envelope section's 8 knobs (T1 L1 T2 L2 ... so the pairs read left to
  // right). `lv` describes the level knobs.
  function env_knob_defs(osc, section, lv) {
    var defs = [];
    for (var seg = 1; seg <= 4; seg++) {
      defs.push({
        dx7: true, section: section,
        display_name: "T" + seg, cc: "",
        change_code: bp0_code(osc, 2 * seg),
        knob_type: "log", min_value: 0, max_value: 30000, offset: 2,
        default_value: 100,
        dx7_env: { osc: osc, seg: seg, field: "time" },
      });
      defs.push({
        dx7: true, section: section,
        display_name: "L" + seg, cc: "",
        change_code: seg === 4 ? bp0_release_level_code(osc) : bp0_code(osc, 2 * seg + 1),
        knob_type: "log", min_value: lv.min, max_value: lv.max, offset: lv.offset,
        default_value: lv.def,
        dx7_env: { osc: osc, seg: seg, field: "level" },
      });
    }
    return defs;
  }

  var AMP_LEVELS = { min: 0, max: 1, offset: 0.001, def: 0 };
  var PITCH_LEVELS = { min: 0.0625, max: 16, offset: 0, def: 1 };

  function build_knob_configs(ch, patchNumber) {
    var baseline = {};
    merge_events(baseline, patch_events(patchNumber));
    var knobs = [];

    // ---- Row 1: FM | PITCH ENV ----
    knobs.push({
      dx7: true, section: "FM", display_name: "algorithm", cc: "",
      knob_type: "selection",
      options: Array.from({ length: 32 }, function(_, i) { return String(i + 1); }),
      option_values: Array.from({ length: 32 }, function(_, i) { return i + 1; }),
      change_code: "i%iv" + MAIN_OSC + "o%v",
      default_value: 1,
    });
    knobs.push({
      dx7: true, section: "FM", display_name: "feedback", cc: "",
      change_code: "i%iv" + MAIN_OSC + "b%v",
      knob_type: "log", min_value: 0, max_value: 0.32, offset: 0.00125,
      default_value: 0,
    });
    knobs.push({
      dx7: true, section: "FM", display_name: "LFO wave", cc: "",
      knob_type: "selection", options: LFO_WAVE_OPTIONS, option_values: LFO_WAVE_VALUES,
      change_code: "i%iv" + LFO_OSC + "w%v",
      default_value: 0,
    });
    knobs.push({
      dx7: true, section: "FM", display_name: "LFO freq", cc: "",
      change_code: "i%iv" + LFO_OSC + "f%v",
      knob_type: "log", min_value: 0.05, max_value: 50, offset: 0,
      default_value: 6,
    });
    knobs.push({
      // Pitch LFO depth: freq MOD coef on the ALGO osc (octaves at LFO peak).
      dx7: true, section: "FM", display_name: "LFO pitch", cc: "",
      change_code: "i%iv" + MAIN_OSC + "f,,,,,%v",
      knob_type: "log", min_value: 0, max_value: 1.5, offset: 0.005,
      default_value: 0,
    });
    knobs = knobs.concat(env_knob_defs(MAIN_OSC, "Pitch Env", PITCH_LEVELS));

    // ---- Row 2: VCF | VCF ENV — identical commands to the Juno surface
    // (osc 0 F/R + EG1 via B). The preset-load VCF coda sets
    // filter_type/eg1_type so these are live on any UI-loaded DX7 preset;
    // defaults below mirror the coda.
    knobs.push({
      dx7: true, section: "VCF", display_name: "freq", cc: 74,
      change_code: "i%iv" + MAIN_OSC + "F%v",
      knob_type: "log", min_value: 20, max_value: 8000,
      default_value: 8000,
    });
    knobs.push({
      dx7: true, section: "VCF", display_name: "resonance", cc: 71,
      change_code: "i%iv" + MAIN_OSC + "R%v",
      knob_type: "log", min_value: 0.5, max_value: 16,
      default_value: 0.7,
    });
    knobs.push({
      dx7: true, section: "VCF", display_name: "kbd", cc: "",
      change_code: "i%iv" + MAIN_OSC + "F,%v",
      min_value: 0, max_value: 1, default_value: 1,
    });
    knobs.push({
      dx7: true, section: "VCF", display_name: "env", cc: "",
      change_code: "i%iv" + MAIN_OSC + "F,,,,%v",
      min_value: -10, max_value: 10, default_value: 4,
    });
    knobs.push({
      dx7: true, section: "VCF ENV", display_name: "attack", cc: "",
      change_code: "i%iv" + MAIN_OSC + "B%v,1,,,,0",
      min_value: 0, max_value: 1000, default_value: 0,
    });
    knobs.push({
      dx7: true, section: "VCF ENV", display_name: "decay", cc: "",
      change_code: "i%iv" + MAIN_OSC + "B,1,%v,,,0",
      knob_type: "log", min_value: 0, max_value: 2000, offset: 50,
      default_value: 0,
    });
    knobs.push({
      dx7: true, section: "VCF ENV", display_name: "sustain", cc: "",
      change_code: "i%iv" + MAIN_OSC + "B,1,,%v,,0",
      min_value: 0, max_value: 1, default_value: 1,
    });
    knobs.push({
      dx7: true, section: "VCF ENV", display_name: "release", cc: "",
      change_code: "i%iv" + MAIN_OSC + "B,1,,,%v,0",
      knob_type: "log", min_value: 0, max_value: 10000, offset: 50,
      default_value: 10000,
    });

    // ---- (the ENV PLOT row is inserted by render_dx7_chrome, not knobs) ----

    // ---- Rows: OP N | OP N AMP ENV ----
    for (var opN = 1; opN <= NUM_OPS; opN++) {
      var osc = op_osc(opN);
      var b = osc_of(baseline, osc);
      // Ratio vs fixed-frequency tuning comes from the preset (fm.py emits
      // I<ratio> or f<hz>); the knob edits whichever mode the op uses.
      var isRatio = Number.isFinite(b.ratio) || !Number.isFinite(b.freq[0]);
      knobs.push({
        dx7: true, section: "Op " + opN, display_name: "level", cc: "",
        change_code: "i%iv" + osc + "a%v",
        knob_type: "log", min_value: 0, max_value: 2, offset: 0.002,
        default_value: 1,
      });
      knobs.push(isRatio ? {
        dx7: true, section: "Op " + opN, display_name: "ratio", cc: "",
        change_code: "i%iv" + osc + "I%v",
        knob_type: "log", min_value: 0.25, max_value: 32,
        default_value: 1,
      } : {
        dx7: true, section: "Op " + opN, display_name: "freq", cc: "",
        change_code: "i%iv" + osc + "f%v",
        knob_type: "log", min_value: 1, max_value: 10000,
        default_value: 440,
      });
      knobs.push({
        // Amp LFO depth for this op: amp MOD coef (fm.py's ampmodsens gate).
        dx7: true, section: "Op " + opN, display_name: "amp lfo", cc: "",
        change_code: "i%iv" + osc + "a,,,,,%v",
        min_value: 0, max_value: 1, default_value: 0,
      });
      knobs = knobs.concat(env_knob_defs(osc, "Op " + opN + " Amp Env", AMP_LEVELS));
    }
    return knobs;
  }

  // ── state access ────────────────────────────────────────────────────────────

  function get_state(ch) {
    var pn = window.get_channel_dx7_patch(ch);
    if (!pn) return null;
    var state = _dx7_state[ch];
    if (!state || state.patch !== pn) {
      state = { patch: pn, knobs: build_knob_configs(ch, pn), plots: [] };
      _dx7_state[ch] = state;
    }
    return state;
  }

  // The surface's knobs in render order — the channel grid shows all of them.
  window.get_dx7_knobs_for_channel = function(channel) {
    var state = get_state(Number(channel));
    return state ? state.knobs : [];
  };

  // Re-read every knob's value from the preset baseline + the channel's
  // knob-log lines. Called before each render so preset loads, sketch loads
  // and channel switches position the knobs.
  window.refresh_dx7_knob_values = function(channel) {
    var ch = Number(channel);
    var state = get_state(ch);
    if (!state) return;
    var oscs = compute_osc_state(ch, state.patch);
    var main = osc_of(oscs, MAIN_OSC);
    var lfo = osc_of(oscs, LFO_OSC);
    state.knobs.forEach(function(k) {
      if (k.dx7_env) {
        var o = osc_of(oscs, k.dx7_env.osc);
        var isPitch = k.dx7_env.osc === MAIN_OSC;
        k.default_value = (k.dx7_env.field === "time")
          ? val(o.eg0t[k.dx7_env.seg], 0)
          : val(o.eg0v[k.dx7_env.seg], isPitch ? 1 : 0);
        return;
      }
      var sec = k.section, name = k.display_name;
      if (sec === "FM" && name === "algorithm") k.default_value = val(main.algorithm, 1);
      else if (sec === "FM" && name === "feedback") k.default_value = val(main.feedback, 0);
      else if (sec === "FM" && name === "LFO wave") k.default_value = val(lfo.wave, 0);
      else if (sec === "FM" && name === "LFO freq") k.default_value = val(lfo.freq[0], 6);
      else if (sec === "FM" && name === "LFO pitch") k.default_value = val(main.freq[5], 0);
      else if (sec === "VCF" && name === "freq") k.default_value = val(main.ff[0], 8000);
      else if (sec === "VCF" && name === "resonance") k.default_value = val(main.resonance, 0.7);
      else if (sec === "VCF" && name === "kbd") k.default_value = val(main.ff[1], 1);
      else if (sec === "VCF" && name === "env") k.default_value = val(main.ff[4], 4);
      // VCF ENV bp1 shape is A,1,D,S,R,0 (see the Juno knobs): times 0/1/2,
      // sustain = values[1].
      else if (sec === "VCF ENV" && name === "attack") k.default_value = val(main.eg1t[0], 0);
      else if (sec === "VCF ENV" && name === "decay") k.default_value = val(main.eg1t[1], 0);
      else if (sec === "VCF ENV" && name === "sustain") k.default_value = val(main.eg1v[1], 1);
      else if (sec === "VCF ENV" && name === "release") k.default_value = val(main.eg1t[2], 10000);
      else if (/^Op \d$/.test(sec)) {
        var o = osc_of(oscs, op_osc(Number(sec.slice(3))));
        if (name === "level") k.default_value = val(o.amp[0], 1);
        else if (name === "ratio") k.default_value = val(o.ratio, 1);
        else if (name === "freq") k.default_value = val(o.freq[0], 440);
        else if (name === "amp lfo") k.default_value = val(o.amp[5], 0);
      }
    });
  };

  // ── envelope plots ──────────────────────────────────────────────────────────

  // Read one envelope's 4 (time, level) pairs back out of the knob configs
  // (they are the live values — a mid-drag knob has already updated its
  // config.default_value by the time the redraw runs).
  function env_pairs_from_knobs(state, osc) {
    var times = [0, 0, 0, 0, 0];
    var levels = [0, 0, 0, 0, 0];
    state.knobs.forEach(function(k) {
      if (!k.dx7_env || k.dx7_env.osc !== osc) return;
      var v = Number(k.default_value);
      if (!Number.isFinite(v)) v = 0;
      if (k.dx7_env.field === "time") times[k.dx7_env.seg] = v;
      else levels[k.dx7_env.seg] = v;
    });
    levels[0] = levels[4];  // pair 0 rests at the release level (fm.py)
    return { times: times, levels: levels };
  }

  function amp_y(level) {
    // DX7 levels are log-spaced (2^((L-99)/8), floor ~2e-4): map log2(level)
    // over ~13 octaves so the useful range fills the plot.
    var l = Math.max(Number(level) || 0, 0.0001);
    return Math.min(Math.max(1 + Math.log2(l) / 13.3, 0), 1.04);
  }

  function pitch_y(ratio) {
    // Pitch env levels are f0 ratios; center 1.0, ±2 octaves full scale.
    var r = Math.max(Number(ratio) || 1, 0.01);
    return Math.min(Math.max(0.5 + Math.log2(r) / 4, 0), 1);
  }

  function draw_env_plot(canvas, env, kind) {
    var ctx = canvas.getContext("2d");
    if (!ctx) return;
    var dpr = window.devicePixelRatio || 1;
    var W = canvas.clientWidth, H = canvas.clientHeight;
    if (!W || !H) return;
    canvas.width = Math.round(W * dpr);
    canvas.height = Math.round(H * dpr);
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    ctx.clearRect(0, 0, W, H);

    var padX = 6, padY = 5;
    var plotW = W - 2 * padX, plotH = H - 2 * padY;
    var yOf = kind === "pitch" ? pitch_y : amp_y;
    function Y(level) { return padY + (1 - yOf(level)) * plotH; }

    // Segment widths: sqrt-of-time so short attacks stay visible next to long
    // decays; a fixed-width sustain hold sits between key-on and release.
    var segW = [];
    var onW = 0;
    for (var s = 1; s <= 4; s++) {
      segW[s] = Math.sqrt(Math.max(env.times[s], 0) + 1);
      if (s < 4) onW += segW[s];
    }
    var holdW = 0.30 * (onW + segW[4]);
    var scale = plotW / (onW + holdW + segW[4]);

    var xs = [padX];
    for (var s2 = 1; s2 <= 3; s2++) xs[s2] = xs[s2 - 1] + segW[s2] * scale;
    var keyOffX = xs[3] + holdW * scale;
    xs[4] = keyOffX + segW[4] * scale;

    var line = kind === "pitch" ? "#f2c94c" : "#1fd17a";
    var fill = kind === "pitch" ? "rgba(242, 201, 76, 0.14)" : "rgba(31, 209, 122, 0.16)";

    // Pitch reference line at ratio 1.0.
    if (kind === "pitch") {
      ctx.strokeStyle = "rgba(255, 255, 255, 0.18)";
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.moveTo(padX, Y(1));
      ctx.lineTo(W - padX, Y(1));
      ctx.stroke();
    }

    function trace() {
      ctx.moveTo(xs[0], Y(env.levels[0]));
      for (var p = 1; p <= 3; p++) ctx.lineTo(xs[p], Y(env.levels[p]));
      ctx.lineTo(keyOffX, Y(env.levels[3]));   // sustain hold at L3
      ctx.lineTo(xs[4], Y(env.levels[4]));     // release to L4
    }

    if (kind !== "pitch") {
      ctx.fillStyle = fill;
      ctx.beginPath();
      trace();
      ctx.lineTo(xs[4], H - padY);
      ctx.lineTo(padX, H - padY);
      ctx.closePath();
      ctx.fill();
    }

    // Key-off marker at the end of the sustain hold.
    ctx.strokeStyle = "rgba(255, 255, 255, 0.25)";
    ctx.setLineDash([3, 3]);
    ctx.beginPath();
    ctx.moveTo(keyOffX, padY);
    ctx.lineTo(keyOffX, H - padY);
    ctx.stroke();
    ctx.setLineDash([]);

    ctx.strokeStyle = line;
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    trace();
    ctx.stroke();

    ctx.fillStyle = line;
    var dots = [[xs[1], env.levels[1]], [xs[2], env.levels[2]], [xs[3], env.levels[3]], [xs[4], env.levels[4]]];
    for (var d = 0; d < dots.length; d++) {
      ctx.beginPath();
      ctx.arc(dots[d][0], Y(dots[d][1]), 2, 0, 2 * Math.PI);
      ctx.fill();
    }
  }

  function redraw_plots(ch) {
    var state = _dx7_state[ch];
    if (!state) return;
    state.plots.forEach(function(plot) {
      if (!plot.canvas.isConnected) return;
      draw_env_plot(plot.canvas, env_pairs_from_knobs(state, plot.osc), plot.kind);
    });
  }

  var _redraw_scheduled = false;
  // init_knobs onChange hook for the DX7 channel grid: keep the normal
  // change handling (autosave dirty), then redraw the envelope plots with
  // the new value (rAF-deferred: notifyKnobChange updates config.default_value
  // just after calling us).
  window.dx7_on_knob_change = function(index, value) {
    if (typeof window.onKnobChange === "function") window.onKnobChange(index, value);
    if (_redraw_scheduled) return;
    _redraw_scheduled = true;
    window.requestAnimationFrame(function() {
      _redraw_scheduled = false;
      redraw_plots(Number(window.current_synth || 1));
    });
  };

  // ── the ENV PLOT row ────────────────────────────────────────────────────────

  // One plot cell: a knob-style (non-clickable) title above the canvas.
  // `span` is the cell's flex weight within its section.
  function make_plot_cell(state, title, osc, kind, span) {
    var cell = document.createElement("div");
    cell.className = "dx7-env-plot";
    cell.style.flex = String(span);
    var label = document.createElement("div");
    label.className = "knob-label small";
    label.textContent = title;
    var canvas = document.createElement("canvas");
    cell.appendChild(label);
    cell.appendChild(canvas);
    state.plots.push({ canvas: canvas, osc: osc, kind: kind });
    return cell;
  }

  function make_plot_section(cells) {
    var wrap = document.createElement("div");
    wrap.className = "col-12 knob-section knob-section-main dx7-plot-section";
    var header = document.createElement("div");
    header.className = "knob-section-header";
    header.textContent = "Env Plot";
    var row = document.createElement("div");
    row.className = "dx7-env-plots-row";
    for (var i = 0; i < cells.length; i++) row.appendChild(cells[i]);
    wrap.appendChild(header);
    wrap.appendChild(row);
    return wrap;
  }

  // Called by refresh_knobs_for_channel right after init_knobs rendered the
  // knob sections into #knob-grid-channel: insert the ENV PLOT row (all 7
  // envelope plots, pitch + op 1-6) between the VCF ENV and OP 1 rows.
  window.render_dx7_chrome = function(channel) {
    var ch = Number(channel);
    var state = get_state(ch);
    var grid = document.getElementById("knob-grid-channel");
    if (!state || !grid) return;

    state.plots = [];
    var left = make_plot_section([
      make_plot_cell(state, "pitch", MAIN_OSC, "pitch", 2),
      make_plot_cell(state, "op 1", op_osc(1), "amp", 1),
      make_plot_cell(state, "op 2", op_osc(2), "amp", 1),
    ]);
    var right = make_plot_section([
      make_plot_cell(state, "op 3", op_osc(3), "amp", 1),
      make_plot_cell(state, "op 4", op_osc(4), "amp", 1),
      make_plot_cell(state, "op 5", op_osc(5), "amp", 1),
      make_plot_cell(state, "op 6", op_osc(6), "amp", 1),
    ]);

    // Insert before the "Op 1" section so the plot row sits between the
    // VCF/VCF ENV row and the per-op rows.
    var anchor = null;
    var sections = grid.querySelectorAll(".knob-section");
    for (var i = 0; i < sections.length; i++) {
      var header = sections[i].querySelector(".knob-section-header");
      if (header && header.textContent.trim() === "Op 1") { anchor = sections[i]; break; }
    }
    grid.insertBefore(left, anchor);
    grid.insertBefore(right, anchor);
    redraw_plots(ch);
  };
})();
