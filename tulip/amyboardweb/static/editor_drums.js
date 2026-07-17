// editor_drums.js — per-drum parameter knobs for GM drum kit channels.
//
// A drum kit patch (K384+ Gamma9001 kits, K258 legacy ROM drums) is, since
// amy#913, a SINGLE-VOICE synth whose one voice is a container holding one
// dedicated osc per drum sound. The kit patch sets up each osc with its PCM
// preset (`v<k>w7p<preset>`) and maps each GM note to its osc with a per-note
// MIDI note mapping `io<note>,<L>,<N>,<X>,<O>,<template>`, where the template
// `v<k>n<base>l%vi%i` carries only what a hit must set: the osc, the sample's
// base note and the velocity. The sample's kit-balancing gain rides the
// mapping's linear velocity scale (the X/max field: %v = X * vel/127). Kit
// synths are created with synth_flags=3 (SYNTH_FLAGS_NOTES_VIA_MIDI), so every
// note path — MIDI in, the sequencer, amy.send(note=...) — goes through these
// mappings. Because each drum owns its osc, osc params set on it persist
// across hits (and across other drums' hits).
//
// The drum knob view exposes six parameters per drum SOUND (distinct PCM
// preset; a GM kit maps several notes onto some sounds, e.g. the toms —
// each such note has its own osc, so a sound's edit is sent to every one):
//
//   pitch   -24..+24 semitones  → shifts the template's n relative to the kit
//   vol     0..1.2              → multiplier on the kit gain in the mapping's
//                                 velocity max (X field)
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
// The knobs split into two groups by where the value has to live:
//
//   pan / cutoff / res — persistent params on the drum's dedicated osc, set by
//   addressing the drum BY NOTE: `i<ch>n<note>Q<pan>G4F<hz>R<q>` (one line per
//   note, all three params in a fixed shape so the line's structural key is
//   stable). AMY routes a note-addressed event with no velocity through the
//   note mapping as a "fake note on" (amy#913), so it lands on the right osc
//   without the UI needing to know osc numbers. Hits never touch these params,
//   so they stick until changed.
//
//   pitch / vol / offset — live in the note mapping itself, so these REWRITE
//   the drum's io line(s) in the kit's canonical template shape
//   `v<k>n<base+pitch>l%vi%iP<phase>`: pitch is the template's n, vol scales
//   the velocity max (X field), and offset must re-arm on every hit because
//   AMY consumes trigger_phase (P) at PCM note-on (amy#904) — a one-shot by
//   design, so it can't be a persistent osc param.
//
// Both line forms are sent live and recorded in the knob log (one last-wins
// slot per note per form, keyed by the line's structural key, so
// sketch-loaded lines and live moves collapse together). Only edited sounds
// get lines in _auto_generated_knobs.
//
// Sketches saved before amy#913 recorded io lines in the old one-osc-per-VOICE
// form (template baking p<preset> and the edited Q/G/F/R/P, no v<osc>).
// refresh_drum_knob_values() migrates them: their params are recovered, the
// kit patch is re-sent to restore pristine mappings/oscs, every logged sound
// is re-sent in the new two-line form, and the stale slots are dropped.

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

  // Parse one CURRENT-form mapping template: v<osc>n<base>l%vi%i[P<phase>]
  // (the trailing P only appears in editor-rewritten lines, never in the kit).
  // Returns null for anything else (i.e. a pre-amy#913 template).
  function parse_template(t) {
    var m = /^v(\d+)n(-?[\d.]+)l%vi%i(?:P([\d.]+))?$/.exec(t);
    if (!m) return null;
    return {
      osc: parseInt(m[1], 10),
      baseNote: parseFloat(m[2]),
      phase: m[3] !== undefined ? parseFloat(m[3]) : null,
    };
  }

  // Parse a LEGACY (pre-amy#913) template: n<base>l%v[a<amp>]i%ip<preset>
  // [Q..][G..][F..][R..][P..] — the old one-osc-per-voice form where every
  // edited param was baked into the mapping.
  function parse_legacy_template(t) {
    var out = { baseNote: 60, preset: null, pan: null,
                ftype: null, ffreq: null, res: null, phase: null };
    var m;
    m = /n(-?[\d.]+)/.exec(t); if (m) out.baseNote = parseFloat(m[1]);
    m = /p(\d+)/.exec(t);      if (m) out.preset = parseInt(m[1], 10);
    m = /Q(-?[\d.]+)/.exec(t); if (m) out.pan = parseFloat(m[1]);
    m = /G(\d+)/.exec(t);      if (m) out.ftype = parseInt(m[1], 10);
    m = /F(-?[\d.]+)/.exec(t); if (m) out.ffreq = parseFloat(m[1]);
    m = /R(-?[\d.]+)/.exec(t); if (m) out.res = parseFloat(m[1]);
    m = /P([\d.]+)/.exec(t);   if (m) out.phase = parseFloat(m[1]);
    return out;
  }

  // The kit's patch wire string is a Z-separated list: per-drum osc setup
  // segments `v<k>w7p<preset>` followed by note-map entries like
  // `io35,0,0,4.02,0,v0n60l%vi%i` (velMax = the drum's kit-balancing gain).
  // Returns [{note, velMax, osc, baseNote, preset}].
  function parse_kit_entries(kitPatch) {
    var codes = window.patch_code_for_patch_number;
    var s = (codes && typeof codes[kitPatch] === "string") ? codes[kitPatch] : "";
    if (!s) return [];
    var oscPreset = {};
    var entries = [];
    var segments = s.split("Z");
    for (var i = 0; i < segments.length; i++) {
      var m = /^v(\d+)w7p(\d+)$/.exec(segments[i]);
      if (m) {
        oscPreset[parseInt(m[1], 10)] = parseInt(m[2], 10);
        continue;
      }
      m = /^io(\d+),([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/.exec(segments[i]);
      if (!m) continue;
      var tmpl = parse_template(m[6]);
      if (!tmpl || !Number.isInteger(oscPreset[tmpl.osc])) continue;
      entries.push({
        note: parseInt(m[1], 10),
        velMax: Number.isFinite(parseFloat(m[4])) ? parseFloat(m[4]) : 1,
        osc: tmpl.osc,
        baseNote: tmpl.baseNote,
        preset: oscPreset[tmpl.osc],
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
      var sound = byPreset[e.preset];
      if (!sound) {
        var info = preset_info(e.preset);
        sound = {
          preset: e.preset,
          name: info ? info.name : ("preset " + e.preset),
          frames: info ? Number(info.frames) || 0 : 0,
          entries: [],
          params: default_params(e),
        };
        byPreset[e.preset] = sound;
        sounds.push(sound);
      }
      sound.entries.push(e);
    }
    return sounds;
  }

  function default_params(entry) {
    return {
      pitch: 0,
      vol: 1,   // multiplier on the kit's baked velocity gain (entry.velMax)
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

  // A pre-amy#913 io line bakes the preset (and any edited params) into the
  // template instead of referencing the drum's dedicated osc (v<k>).
  function io_line_is_legacy(line) {
    var m = /^i\d+io\d+,(?:[^,]*,){4}(.*)$/.exec(line);
    return !!(m && !parse_template(m[1]));
  }

  // One pass over the knob log for this channel: note -> recorded io line,
  // note -> recorded osc-params line (i<ch>n<note>Q...), plus the log keys of
  // any legacy-format io lines (for migration).
  function log_lines_for_channel(ch) {
    var out = { ioByNote: {}, paramsByNote: {}, legacyKeys: [], legacyLevel: null };
    if (!(window.knob_log instanceof Map)) return out;
    var ioRe = new RegExp("^i" + ch + "io(\\d+),");
    var paramRe = new RegExp("^i" + ch + "n(\\d+)([QGFR].*)$");
    // Pre-amy#913 drum Level line: amp on the (then) control osc — under the
    // per-drum-osc model v0 is the first drum, so this must migrate to the
    // osc-less broadcast form (i<ch>a<val>).
    var levelRe = new RegExp("^i" + ch + "v0a(-?[\\d.]+)$");
    window.knob_log.forEach(function(entry, key) {
      if (!entry || Number(entry.synth) !== ch) return;
      var line = String(entry.line || "");
      var m = ioRe.exec(line);
      if (m) {
        out.ioByNote[parseInt(m[1], 10)] = line;
        if (io_line_is_legacy(line)) out.legacyKeys.push(key);
        return;
      }
      m = paramRe.exec(line);
      if (m) {
        out.paramsByNote[parseInt(m[1], 10)] = m[2];
        return;
      }
      m = levelRe.exec(line);
      if (m) out.legacyLevel = { key: key, val: parseFloat(m[1]) };
    });
    return out;
  }

  // Recover a sound's params from its first note's recorded lines (if any).
  // ioLine carries pitch/vol/offset (or, for a legacy line, everything);
  // paramsTail (the Q/G/F/R tail of an i<ch>n<note> line) carries
  // pan/cutoff/res.
  function params_from_log(sound, entry, ioLine, paramsTail) {
    var params = default_params(entry);
    var found = false;
    if (ioLine) {
      var m = /^i\d+io\d+,([^,]*),([^,]*),([^,]*),([^,]*),(.*)$/.exec(ioLine);
      if (m) {
        found = true;
        var velMax = parseFloat(m[3]);
        var velBase = Number(entry.velMax) || 1;
        var tmpl = parse_template(m[5]);
        if (tmpl) {
          if (Number.isFinite(velMax)) params.vol = velMax / velBase;
          if (Number.isFinite(tmpl.baseNote)) params.pitch = tmpl.baseNote - entry.baseNote;
          if (Number.isFinite(tmpl.phase) && tmpl.phase > 0 && sound.frames > 0) {
            params.offset = Math.min(0.95, Math.max(0, tmpl.phase * PCM_PHASE_DENOM / sound.frames));
          }
        } else {
          // Legacy line: every edited param was baked into the template.
          var lt = parse_legacy_template(m[5]);
          if (Number.isFinite(velMax)) params.vol = velMax / velBase;
          if (Number.isFinite(lt.baseNote)) params.pitch = lt.baseNote - entry.baseNote;
          if (Number.isFinite(lt.pan)) params.pan = lt.pan;
          if (lt.ftype === 4 && Number.isFinite(lt.ffreq)) {
            params.cutoff = Math.min(Math.max(lt.ffreq, CUTOFF_MIN_HZ), CUTOFF_MAX_HZ);
          }
          if (Number.isFinite(lt.res)) params.res = lt.res;
          if (Number.isFinite(lt.phase) && lt.phase > 0 && sound.frames > 0) {
            params.offset = Math.min(0.95, Math.max(0, lt.phase * PCM_PHASE_DENOM / sound.frames));
          }
        }
      }
    }
    if (paramsTail) {
      found = true;
      var pm;
      pm = /Q(-?[\d.]+)/.exec(paramsTail); if (pm) params.pan = parseFloat(pm[1]);
      pm = /G(\d+)F(-?[\d.]+)/.exec(paramsTail);
      if (pm) {
        params.cutoff = (parseInt(pm[1], 10) === 4)
          ? Math.min(Math.max(parseFloat(pm[2]), CUTOFF_MIN_HZ), CUTOFF_MAX_HZ)
          : CUTOFF_MAX_HZ;
      }
      pm = /R(-?[\d.]+)/.exec(paramsTail); if (pm) params.res = parseFloat(pm[1]);
    }
    return found ? params : null;
  }

  // Rebuild one note's mapping line from a sound's current params, in the
  // kit's canonical template shape `v<k>n<base+pitch>l%vi%iP<phase>`: pitch
  // rides the template's n, vol the mapping's velocity max, and offset the
  // trigger phase (P), which AMY consumes at PCM note-on so it must re-arm on
  // every hit. The fixed shape keeps the line's structural key stable (one
  // log slot per note).
  function build_io_line(ch, entry, sound) {
    var p = sound.params;
    var phase = 0;
    if (p.offset > 0 && sound.frames > 0) {
      phase = Math.floor(p.offset * sound.frames) / PCM_PHASE_DENOM;
    }
    var t = "v" + entry.osc + "n" + fmt(entry.baseNote + p.pitch) + "l%vi%i"
      + "P" + fmtPhase(phase);
    // Same command form as CC-mapping lines (i1ic70,...): i<ch> selects the
    // synth, then the synth-level `io` note-map command. A bare `o` after the
    // channel digits would parse as the top-level `algorithm` command.
    return "i" + ch + "io" + entry.note + ",0,0," + fmt(p.vol * (Number(entry.velMax) || 1)) + ",0," + t;
  }

  // Build one note's persistent osc-params line: pan/cutoff/res set directly
  // on the drum's dedicated osc, addressed by note (routed through the note
  // mapping as a velocity-less "fake note on", amy#913). Full fixed shape
  // (Q/G/F/R) so the structural key stays stable.
  function build_param_line(ch, entry, sound) {
    var p = sound.params;
    var open = p.cutoff >= CUTOFF_MAX_HZ - 1;
    return "i" + ch + "n" + entry.note
      + "Q" + fmt(p.pan)
      + "G" + (open ? 0 : 4)
      + "F" + fmt(Math.round(p.cutoff))
      + "R" + fmt(p.res);
  }

  // Send (live) + record (knob log) both line forms for one sound's notes.
  function send_sound(ch, sound) {
    if (typeof window.send_amy_message_in_sketch !== "function") return;
    for (var i = 0; i < sound.entries.length; i++) {
      window.send_amy_message_in_sketch(build_io_line(ch, sound.entries[i], sound), { synth: ch });
      window.send_amy_message_in_sketch(build_param_line(ch, sound.entries[i], sound), { synth: ch });
    }
  }

  // Audition one drum sound: send a MIDI note-on (and a note-off, which kit
  // synths ignore — synth_flags bit 2) exactly as a keyboard would, so it
  // works in both simulate (local AMY) and control (real board) modes.
  function play_sound(ch, sound) {
    if (typeof window.inject_midi_bytes !== "function" || !sound.entries.length) return;
    var note = sound.entries[0].note;
    for (var i = 1; i < sound.entries.length; i++) {
      if (sound.entries[i].note < note) note = sound.entries[i].note;
    }
    var chBits = (Number(ch) - 1) & 0x0F;
    window.inject_midi_bytes([0x90 | chBits, note, 100]);
    setTimeout(function() {
      window.inject_midi_bytes([0x80 | chBits, note, 0]);
    }, 200);
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
      DRUM_KNOB_DEFS.forEach(function(def, di) {
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
          // One audition button per sound, rendered in the section header.
          header_button: di === 0 ? {
            label: "▶",
            title: "Play " + sound.name,
            onClick: function() { play_sound(ch, sound); },
          } : undefined,
          drum_send: function(synth, value) {
            var channel = Number(synth) || ch;
            sound.params[def.key] = value;
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
      state = { kit: kit, sounds: build_sounds(kit), knobs: null };
      state.knobs = build_knob_configs(ch, state);
      _drum_state[ch] = state;
    }
    return state.knobs;
  };

  // Re-read every sound's params from the knob log (recorded lines override
  // kit defaults) into the knob configs. Called before each drum-grid render
  // so sketch loads / channel switches / kit reloads position the knobs.
  window.refresh_drum_knob_values = function(channel) {
    var ch = Number(channel);
    var knobs = window.get_drum_knobs_for_channel(ch);
    var state = _drum_state[ch];
    if (!state || !knobs.length) return;
    var log = log_lines_for_channel(ch);
    state.sounds.forEach(function(sound) {
      var params = null;
      for (var i = 0; i < sound.entries.length && !params; i++) {
        var note = sound.entries[i].note;
        if (log.ioByNote[note] || log.paramsByNote[note]) {
          params = params_from_log(sound, sound.entries[i],
                                   log.ioByNote[note], log.paramsByNote[note]);
        }
      }
      sound.params = params || default_params(sound.entries[0]);
    });
    // Migrate pre-amy#913 io lines to the two-line form in one shot. The
    // stale lines were replayed into AMY on load and their templates don't
    // reference the per-drum oscs (a hit through one would broadcast to every
    // osc in the voice), so first re-send the kit patch to restore pristine
    // mappings/oscs, then re-send every logged sound in the current form
    // (live + logged under the current structural keys) and drop the stale
    // slots.
    if (log.legacyKeys.length && typeof window.send_amy_message_in_sketch === "function") {
      if (typeof window.amy_add_log_message === "function") {
        // Live-only: the kit's K line is already in the log (normalized to
        // iv1 on load) — this just restores AMY's runtime state.
        window.amy_add_log_message("i" + ch + "K" + state.kit + "iv1Z");
      }
      state.sounds.forEach(function(sound) {
        var logged = sound.entries.some(function(e) {
          return !!log.ioByNote[e.note] || !!log.paramsByNote[e.note];
        });
        if (logged) send_sound(ch, sound);
      });
      log.legacyKeys.forEach(function(key) {
        window.send_amy_message_in_sketch("", { key: key, remove: true, silent: true });
      });
    }
    // Migrate a pre-amy#913 Level line (amp on osc 0 — now just the first
    // drum) to the osc-less broadcast form, and position the slider.
    if (log.legacyLevel && Number.isFinite(log.legacyLevel.val)
      && typeof window.send_amy_message_in_sketch === "function") {
      window.send_amy_message_in_sketch("i" + ch + "a" + log.legacyLevel.val.toFixed(3), { synth: ch });
      window.send_amy_message_in_sketch("", { key: log.legacyLevel.key, remove: true, silent: true });
      if (typeof window.setSynthLevelFromAmy === "function") {
        window.setSynthLevelFromAmy(ch, log.legacyLevel.val);
      }
    }
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
