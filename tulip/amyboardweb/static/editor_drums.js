// editor_drums.js — per-drum parameter knobs for GM drum kit channels.
//
// A drum kit patch (K384+ Gamma9001 kits, K258 legacy ROM drums) is a bank of
// per-note MIDI note mappings: each entry `io<note>,<L>,<N>,<X>,<O>,<template>`
// maps one MIDI note to a wire-command template that bakes in the sample's PCM
// preset (p), base note (n) and level (a). Kit synths are created with
// synth_flags=3 (SYNTH_FLAGS_NOTES_VIA_MIDI), so every note path — MIDI in,
// the sequencer, amy.send(note=...) — goes through these mappings.
//
// The drum knob view exposes six parameters per drum SOUND (distinct PCM
// preset; a GM kit maps several notes onto some sounds, e.g. the toms):
//
//   pitch   -24..+24 semitones  → shifts the template's n relative to the kit
//   vol     0..1.2              → the mapping's velocity max (X field)
//   offset  0..0.95             → sample start point, P<phase> in the template
//                                 (phase = start_frame / 2^23; frames from
//                                 drum_presets.generated.js)
//   cutoff  log 50..16000 Hz    → G4F<hz> (LPF24); at max the filter is
//                                 bypassed (G0) like the gamma9001 demo
//   res     0.1..8              → R<q>
//   pan     0..1                → Q<pan>
//
// Ranges/defaults match the gamma9001 web drum machine.
//
// A knob change REWRITES the note mapping(s) for that sound — sent live and
// recorded in the knob log (one last-wins slot per note, keyed by the line's
// structural key, so sketch-loaded lines and live moves collapse together).
//
// Voice-param leak and normalization: kit voices are reused across drums and
// osc params (pan, filter, phase) persist on the osc between notes. A template
// that sets Q/G/F/R/P would therefore leak into drums whose templates don't
// mention those params. So the FIRST knob change on a channel rewrites the
// WHOLE kit's note map with a fixed template shape (explicit defaults
// everywhere); after that, single-sound rewrites are safe.
//
// The Offset knob needs AMY to respect trigger_phase (P) at PCM note-on
// (amy#903); on older AMY builds the knob is sent but has no audible effect.

(function() {
  var PCM_PHASE_DENOM = 1 << 23;   // AMY PCM phase is frame_index / 2^PCM_INDEX_BITS
  var CUTOFF_MAX_HZ = 16000;       // knob at max = filter bypassed (G0)
  var CUTOFF_MIN_HZ = 50;

  // ── per-channel drum view state ─────────────────────────────────────────────
  // _channel_drum_kit[ch] = kit patch number while the channel's patch is a
  // drum kit, else null. Owned by update_knob_sections_for_patch (spss.js).
  var _channel_drum_kit = {};
  // _drum_state[ch] = { kit, sounds: [...], knobs: [knobConfig, ...] } built
  // lazily per (channel, kit); knob configs persist so CC assignments and
  // range edits survive re-renders. Values are re-read from the knob log by
  // refresh_drum_knob_values() before each render.
  var _drum_state = {};

  window.set_channel_drum_kit = function(channel, kitPatch) {
    var ch = Number(channel);
    if (!Number.isInteger(ch) || ch < 1 || ch > 16) return;
    _channel_drum_kit[ch] = Number.isInteger(kitPatch) ? kitPatch : null;
  };

  window.get_channel_drum_kit = function(channel) {
    var kit = _channel_drum_kit[Number(channel)];
    return Number.isInteger(kit) ? kit : null;
  };

  function preset_info(preset) {
    var table = window.AMY_DRUM_PRESETS || {};
    return table[String(preset)] || null;
  }

  function fmt(value) {
    var n = Number(value);
    if (!Number.isFinite(n)) return "0";
    return String(Number(n.toFixed(3)));
  }

  function fmtPhase(value) {
    var n = Number(value);
    if (!Number.isFinite(n) || n <= 0) return "0";
    return String(Number(n.toFixed(7)));
  }

  // ── kit patch parsing ───────────────────────────────────────────────────────

  // Parse one mapping template: n<base>l%v[a<amp>]i%ip<preset>[Q..][G..][F..][R..][P..]
  function parse_template(t) {
    var out = { baseNote: 60, amp: null, preset: null, pan: null,
                ftype: null, ffreq: null, res: null, phase: null };
    var m;
    m = /n(-?[\d.]+)/.exec(t); if (m) out.baseNote = parseFloat(m[1]);
    m = /a(-?[\d.]+)/.exec(t); if (m) out.amp = parseFloat(m[1]);
    m = /p(\d+)/.exec(t);      if (m) out.preset = parseInt(m[1], 10);
    m = /Q(-?[\d.]+)/.exec(t); if (m) out.pan = parseFloat(m[1]);
    m = /G(\d+)/.exec(t);      if (m) out.ftype = parseInt(m[1], 10);
    m = /F(-?[\d.]+)/.exec(t); if (m) out.ffreq = parseFloat(m[1]);
    m = /R(-?[\d.]+)/.exec(t); if (m) out.res = parseFloat(m[1]);
    m = /P([\d.]+)/.exec(t);   if (m) out.phase = parseFloat(m[1]);
    return out;
  }

  // The kit's patch wire string is a Z-separated list; note-map entries look
  // like `io35,0,0,1,0,n60l%va4.02i%ip0`. Returns [{note, velMax, tmpl}].
  function parse_kit_entries(kitPatch) {
    var codes = window.patch_code_for_patch_number;
    var s = (codes && typeof codes[kitPatch] === "string") ? codes[kitPatch] : "";
    if (!s) return [];
    var entries = [];
    var segments = s.split("Z");
    for (var i = 0; i < segments.length; i++) {
      var m = /^io(\d+),([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/.exec(segments[i]);
      if (!m) continue;
      var tmpl = parse_template(m[6]);
      if (!Number.isInteger(tmpl.preset)) continue;
      entries.push({
        note: parseInt(m[1], 10),
        velMax: Number.isFinite(parseFloat(m[4])) ? parseFloat(m[4]) : 1,
        tmpl: tmpl,
      });
    }
    return entries;
  }

  // Group a kit's note entries by PCM preset — one knob row per drum SOUND.
  function build_sounds(kitPatch) {
    var entries = parse_kit_entries(kitPatch);
    var byPreset = {};
    var sounds = [];
    for (var i = 0; i < entries.length; i++) {
      var e = entries[i];
      var sound = byPreset[e.tmpl.preset];
      if (!sound) {
        var info = preset_info(e.tmpl.preset);
        sound = {
          preset: e.tmpl.preset,
          name: info ? info.name : ("preset " + e.tmpl.preset),
          frames: info ? Number(info.frames) || 0 : 0,
          entries: [],
          params: default_params(e),
        };
        byPreset[e.tmpl.preset] = sound;
        sounds.push(sound);
      }
      sound.entries.push(e);
    }
    return sounds;
  }

  function default_params(entry) {
    return {
      pitch: 0,
      vol: entry ? entry.velMax : 1,
      offset: 0,
      cutoff: CUTOFF_MAX_HZ,
      res: 0.7,
      pan: 0.5,
    };
  }

  function sound_header(sound) {
    var notes = sound.entries.map(function(e) { return e.note; }).sort(function(a, b) { return a - b; });
    var label = notes.length <= 3
      ? notes.join("/")
      : notes.slice(0, 3).join("/") + "+" + (notes.length - 3);
    return label + " · " + sound.name;
  }

  // ── knob log <-> per-sound params ───────────────────────────────────────────

  // One pass over the knob log: note -> recorded io line for this channel.
  function log_io_lines_for_channel(ch) {
    var lines = {};
    if (!(window.knob_log instanceof Map)) return lines;
    var re = new RegExp("^i" + ch + "io(\\d+),");
    window.knob_log.forEach(function(entry) {
      if (!entry || Number(entry.synth) !== ch) return;
      var m = re.exec(String(entry.line || ""));
      if (m) lines[parseInt(m[1], 10)] = String(entry.line);
    });
    return lines;
  }

  // Recover a sound's params from its first note's recorded io line (if any).
  function params_from_log_line(sound, entry, line) {
    var m = /^i\d+io\d+,([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/.exec(line);
    if (!m) return null;
    var tmpl = parse_template(m[5]);
    var params = default_params(entry);
    var velMax = parseFloat(m[3]);
    if (Number.isFinite(velMax)) params.vol = velMax;
    if (Number.isFinite(tmpl.baseNote)) params.pitch = tmpl.baseNote - entry.tmpl.baseNote;
    if (Number.isFinite(tmpl.pan)) params.pan = tmpl.pan;
    if (tmpl.ftype === 4 && Number.isFinite(tmpl.ffreq)) {
      params.cutoff = Math.min(Math.max(tmpl.ffreq, CUTOFF_MIN_HZ), CUTOFF_MAX_HZ);
    } else {
      params.cutoff = CUTOFF_MAX_HZ;
    }
    if (Number.isFinite(tmpl.res)) params.res = tmpl.res;
    if (Number.isFinite(tmpl.phase) && tmpl.phase > 0 && sound.frames > 0) {
      params.offset = Math.min(0.95, Math.max(0, tmpl.phase * PCM_PHASE_DENOM / sound.frames));
    }
    return params;
  }

  // Rebuild one note mapping line from a sound's current params. The template
  // always carries the FULL param set (Q/G/F/R/P) in a fixed shape, so (a) the
  // line's structural key stays stable as values change (one log slot per
  // note) and (b) no osc param can leak between drums via voice reuse.
  function build_io_line(ch, entry, sound) {
    var p = sound.params;
    var t = "n" + fmt(entry.tmpl.baseNote + p.pitch) + "l%v";
    if (entry.tmpl.amp !== null) t += "a" + fmt(entry.tmpl.amp);
    t += "i%ip" + entry.tmpl.preset;
    t += "Q" + fmt(p.pan);
    var open = p.cutoff >= CUTOFF_MAX_HZ - 1;
    t += "G" + (open ? 0 : 4);
    t += "F" + fmt(Math.round(p.cutoff));
    t += "R" + fmt(p.res);
    var phase = 0;
    if (p.offset > 0 && sound.frames > 0) {
      phase = Math.floor(p.offset * sound.frames) / PCM_PHASE_DENOM;
    }
    t += "P" + fmtPhase(phase);
    // Same command form as CC-mapping lines (i1ic70,...): i<ch> selects the
    // synth, then the synth-level `io` note-map command. A bare `o` after the
    // channel digits would parse as the top-level `algorithm` command.
    return "i" + ch + "io" + entry.note + ",0,0," + fmt(p.vol) + ",0," + t;
  }

  // Send (live) + record (knob log) the mappings for one sound's notes.
  function send_sound(ch, sound) {
    if (typeof window.send_amy_message_in_sketch !== "function") return;
    for (var i = 0; i < sound.entries.length; i++) {
      window.send_amy_message_in_sketch(build_io_line(ch, sound.entries[i], sound), { synth: ch });
    }
  }

  // First knob change on a channel rewrites the whole kit's note map so every
  // template carries the full param shape (see leak note at top of file).
  function ensure_kit_normalized(ch, state) {
    if (state.normalized) return;
    state.normalized = true;
    for (var i = 0; i < state.sounds.length; i++) {
      send_sound(ch, state.sounds[i]);
    }
  }

  // ── knob configs ────────────────────────────────────────────────────────────

  var DRUM_KNOB_DEFS = [
    { key: "pitch",  display_name: "pitch",  min_value: -24,  max_value: 24 },
    { key: "offset", display_name: "offset", min_value: 0,    max_value: 0.95 },
    { key: "pan",    display_name: "pan",    min_value: 0,    max_value: 1 },
    { key: "vol",    display_name: "vol",    min_value: 0,    max_value: 1.2 },
    { key: "cutoff", display_name: "cutoff", min_value: CUTOFF_MIN_HZ, max_value: CUTOFF_MAX_HZ, knob_type: "log" },
    { key: "res",    display_name: "res",    min_value: 0.1,  max_value: 8 },
  ];

  function build_knob_configs(ch, state) {
    var knobs = [];
    state.sounds.forEach(function(sound) {
      var section = sound_header(sound);
      DRUM_KNOB_DEFS.forEach(function(def) {
        knobs.push({
          drum: true,
          no_device_cc: true,   // CC works via move_knob (web) only — a device
                                // ic mapping can't nest the io note template.
          section: section,
          display_name: def.display_name,
          cc: "",
          knob_type: def.knob_type,
          min_value: def.min_value,
          max_value: def.max_value,
          default_value: sound.params[def.key],
          drum_send: function(synth, value) {
            var channel = Number(synth) || ch;
            sound.params[def.key] = value;
            ensure_kit_normalized(channel, state);
            send_sound(channel, sound);
          },
        });
      });
    });
    return knobs;
  }

  // Cached per-(channel, kit) knob configs; built on first use so CC numbers
  // and range edits persist across re-renders while the kit stays loaded.
  window.get_drum_knobs_for_channel = function(channel) {
    var ch = Number(channel);
    var kit = window.get_channel_drum_kit(ch);
    if (!kit) return [];
    var state = _drum_state[ch];
    if (!state || state.kit !== kit) {
      state = { kit: kit, sounds: build_sounds(kit), normalized: false, knobs: null };
      state.knobs = build_knob_configs(ch, state);
      _drum_state[ch] = state;
    }
    return state.knobs;
  };

  // Re-read every sound's params from the knob log (recorded io lines override
  // kit defaults) into the knob configs. Called before each drum-grid render
  // so sketch loads / channel switches / kit reloads position the knobs.
  window.refresh_drum_knob_values = function(channel) {
    var ch = Number(channel);
    var knobs = window.get_drum_knobs_for_channel(ch);
    var state = _drum_state[ch];
    if (!state || !knobs.length) return;
    var logLines = log_io_lines_for_channel(ch);
    state.normalized = Object.keys(logLines).length > 0;
    state.sounds.forEach(function(sound) {
      var params = null;
      for (var i = 0; i < sound.entries.length && !params; i++) {
        var line = logLines[sound.entries[i].note];
        if (line) params = params_from_log_line(sound, sound.entries[i], line);
      }
      sound.params = params || default_params(sound.entries[0]);
    });
    // Push the refreshed values into the knob configs (init_knobs reads
    // default_value at render time).
    var k = 0;
    state.sounds.forEach(function(sound) {
      DRUM_KNOB_DEFS.forEach(function(def) {
        if (knobs[k]) knobs[k].default_value = sound.params[def.key];
        k += 1;
      });
    });
  };
})();
