// editor_drums.js — per-drum parameter knobs for GM drum kit channels.
//
// A drum kit patch (K384+ Gamma9001 kits, K258 legacy ROM drums) is, since
// amy#913, a SINGLE-VOICE synth whose one voice is a container holding one
// dedicated osc per drum sound. The kit patch sets up each osc with its PCM
// preset (`v<k>w7p<preset>`) and maps each GM note to its osc with a per-note
// MIDI note mapping `io<note>,<L>,<N>,<X>,<O>,v<k>n<base>l%vi%i`, whose
// template carries only what a hit must set: the osc, the sample's base note
// and the velocity (X = the sample's kit-balancing gain). Kit synths are
// created with synth_flags=3 (SYNTH_FLAGS_NOTES_VIA_MIDI), so every note
// path — MIDI in, the sequencer, amy.send(note=...) — goes through these
// mappings. Because each drum owns its osc, osc params set on it persist
// across hits (and across other drums' hits).
//
// The drum knob view exposes six parameters per drum SOUND (distinct PCM
// preset; a GM kit maps several notes onto some sounds, e.g. the toms —
// each such note has its own osc, so a sound's edit is sent to every one):
//
//   pitch   log 220..880 Hz     → the osc's freq const (AMY-native value;
//                                 440 Hz = no shift; logfreq coefficients
//                                 combine additively, so the const transposes
//                                 the sample — an octave down to an octave up)
//   vol     0..1.2              → the osc's amp const, a<vol>
//   offset  0..0.95             → sample start point, the osc's persistent
//                                 trigger_phase P (phase = start_frame / 2^23;
//                                 frames from drum_presets.generated.js)
//   cutoff  log 50..16000 Hz    → G4F<hz> (LPF24); at max the filter is
//                                 bypassed (G0) like the gamma9001 demo
//   res     0.1..8              → R<q>
//   pan     0..1                → Q<pan>
//
// Ranges/defaults match the gamma9001 web drum machine.
//
// Every knob is a persistent param on the drum's dedicated osc — the kit's
// `io` note mappings are NEVER rewritten. A sound's edits are one line per
// note, addressed BY NOTE: `i<ch>n<note>f<hz>a<amp>Q<pan>G4F<hz>R<q>P<ph>`
// (all params in a fixed shape so the line's structural key is stable). AMY
// routes a note-addressed event with no velocity through the note mapping as
// a "fake note on" (amy#913), so it lands on the right osc without the UI
// needing osc numbers. Hits never touch these params, so they stick until
// changed.
//
// Level (channel strip / CC 7): the per-instrument level `i<ch>iV<val>` —
// AMY scales every osc of the synth at render, orthogonal to the per-drum
// amps above, so the same command serves melodic and drum channels and a
// level change never touches (or is touched by) the vol knobs.
//
// Sketches saved before this scheme recorded io lines (pre-amy#913 templates
// baking p<preset>/params, or an interim velocity-max form), an interim
// amp-broadcast level (i<ch>a<val>, with per-note amps as vol*level
// products), and/or a pre-amy#913 v0a level line. refresh_drum_knob_values()
// migrates them: params are recovered (products divided back out), the kit
// patch is re-sent to restore pristine mappings, every logged sound is
// re-sent in the current per-note form, the level moves to iV, and the stale
// slots are dropped.

(function() {
  var PCM_PHASE_DENOM = 1 << 23;   // AMY PCM phase is frame_index / 2^PCM_INDEX_BITS
  var CUTOFF_MAX_HZ = 16000;       // knob at max = filter bypassed (G0)
  var CUTOFF_MIN_HZ = 50;
  var PITCH_MIN_HZ = 220;          // pitch = AMY-native freq const; 440 = no
  var PITCH_MAX_HZ = 880;          // shift, so this range is ±1 octave

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
  // Only used to GROUP the kit into drum sounds for the UI (note → osc →
  // preset for the row labels and audition) and to recover values from
  // legacy io lines during migration — knob edits never depend on it.

  // Parse one CURRENT-form mapping template: v<osc>n<base>l%vi%i.
  // Returns null for anything else (i.e. a pre-amy#913 template).
  function parse_template(t) {
    var m = /^v(\d+)n(-?[\d.]+)l%vi%i$/.exec(t);
    if (!m) return null;
    return {
      osc: parseInt(m[1], 10),
      baseNote: parseFloat(m[2]),
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
  // `io35,0,0,4.02,0,v0n60l%vi%i` (velMax = the drum's kit-balancing gain,
  // kept here only for legacy-line vol recovery).
  // Returns [{note, velMax, baseNote, preset}].
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
      pitch: 440,   // AMY-native freq const; 440 = no shift
      vol: 1,       // the drum osc's amp const (channel level rides iV, not amp)
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

  // One pass over the knob log for this channel: note -> recorded osc-params
  // line tail (i<ch>n<note>...), the channel level (i<ch>iV<val>), plus stale
  // lines from earlier formats for migration: io note-map rewrites
  // (pre-amy#913 baked templates or the interim velocity-max form), the
  // interim amp-broadcast level i<ch>a<val> (whose era recorded per-note amps
  // as vol*level PRODUCTS — ampDivisor undoes that on recovery), and the
  // pre-amy#913 control-osc level i<ch>v0a<val>.
  function log_lines_for_channel(ch) {
    var out = { paramsByNote: {}, level: null,
                ioByNote: {}, legacyKeys: [], legacyLevel: null, ampDivisor: 1 };
    if (!(window.knob_log instanceof Map)) return out;
    var paramRe = new RegExp("^i" + ch + "n(\\d+)([fQGFR].*)$");
    var levelRe = new RegExp("^i" + ch + "iV(-?[\\d.]+)$");
    var ioRe = new RegExp("^i" + ch + "io(\\d+),");
    var broadcastLevelRe = new RegExp("^i" + ch + "a(-?[\\d.]+)$");
    var oldLevelRe = new RegExp("^i" + ch + "v0a(-?[\\d.]+)$");
    window.knob_log.forEach(function(entry, key) {
      if (!entry || Number(entry.synth) !== ch) return;
      var line = String(entry.line || "");
      var m = paramRe.exec(line);
      if (m) {
        out.paramsByNote[parseInt(m[1], 10)] = m[2];
        return;
      }
      m = levelRe.exec(line);
      if (m) {
        out.level = parseFloat(m[1]);
        return;
      }
      m = ioRe.exec(line);
      if (m) {
        out.ioByNote[parseInt(m[1], 10)] = line;
        out.legacyKeys.push(key);
        return;
      }
      m = broadcastLevelRe.exec(line);
      if (m) {
        out.legacyLevel = { key: key, val: parseFloat(m[1]) };
        if (parseFloat(m[1]) > 0) out.ampDivisor = parseFloat(m[1]);
        return;
      }
      m = oldLevelRe.exec(line);
      if (m) out.legacyLevel = { key: key, val: parseFloat(m[1]) };
    });
    return out;
  }

  // Recover a sound's params from its first note's recorded lines (if any).
  // paramsTail (the f/a/Q/G/F/R/P tail of an i<ch>n<note> line) is the
  // current form; ioLine is a stale rewrite kept only for migration.
  // ampDivisor is 1 except for interim-era logs whose per-note amps were
  // vol*level products (see log_lines_for_channel).
  function params_from_log(sound, entry, ioLine, paramsTail, ampDivisor) {
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
          // This PR's earlier form: vol rode the mapping's velocity max.
          if (Number.isFinite(velMax)) params.vol = velMax / velBase;
        } else {
          // Pre-amy#913 line: every edited param was baked into the template.
          var lt = parse_legacy_template(m[5]);
          if (Number.isFinite(velMax)) params.vol = velMax / velBase;
          if (Number.isFinite(lt.baseNote)) {
            // Old pitch was a semitone shift of the template's base note —
            // convert to the native freq const.
            params.pitch = Math.min(PITCH_MAX_HZ, Math.max(PITCH_MIN_HZ,
              440 * Math.pow(2, (lt.baseNote - entry.baseNote) / 12)));
          }
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
      pm = /^f([\d.]+)/.exec(paramsTail);
      if (pm && parseFloat(pm[1]) > 0) {
        params.pitch = Math.min(PITCH_MAX_HZ, Math.max(PITCH_MIN_HZ, parseFloat(pm[1])));
      }
      pm = /a(-?[\d.]+)/.exec(paramsTail);
      if (pm) {
        var div = (Number.isFinite(ampDivisor) && ampDivisor > 0) ? ampDivisor : 1;
        params.vol = parseFloat(pm[1]) / div;
      }
      pm = /Q(-?[\d.]+)/.exec(paramsTail); if (pm) params.pan = parseFloat(pm[1]);
      pm = /G(\d+)F(-?[\d.]+)/.exec(paramsTail);
      if (pm) {
        params.cutoff = (parseInt(pm[1], 10) === 4)
          ? Math.min(Math.max(parseFloat(pm[2]), CUTOFF_MIN_HZ), CUTOFF_MAX_HZ)
          : CUTOFF_MAX_HZ;
      }
      pm = /R(-?[\d.]+)/.exec(paramsTail); if (pm) params.res = parseFloat(pm[1]);
      pm = /P([\d.]+)/.exec(paramsTail);
      if (pm && parseFloat(pm[1]) > 0 && sound.frames > 0) {
        params.offset = Math.min(0.95, Math.max(0, parseFloat(pm[1]) * PCM_PHASE_DENOM / sound.frames));
      }
    }
    return found ? params : null;
  }

  // Build one note's persistent osc-params line: pitch (freq const, 440 Hz =
  // no shift), amp (vol * channel level), pan, cutoff, res and offset
  // (trigger_phase), set directly on the drum's dedicated osc, addressed by
  // note (routed through the note mapping as a velocity-less "fake note on",
  // amy#913). Full fixed shape (f/a/Q/G/F/R/P) so the structural key stays
  // stable.
  function build_param_line(ch, entry, sound) {
    var p = sound.params;
    var open = p.cutoff >= CUTOFF_MAX_HZ - 1;
    var phase = 0;
    if (p.offset > 0 && sound.frames > 0) {
      phase = Math.floor(p.offset * sound.frames) / PCM_PHASE_DENOM;
    }
    return "i" + ch + "n" + entry.note
      + "f" + fmt(p.pitch)
      + "a" + fmt(p.vol)
      + "Q" + fmt(p.pan)
      + "G" + (open ? 0 : 4)
      + "F" + fmt(Math.round(p.cutoff))
      + "R" + fmt(p.res)
      + "P" + fmtPhase(phase);
  }

  // Send (live) + record (knob log) one sound's per-note lines.
  function send_sound(ch, sound) {
    if (typeof window.send_amy_message_in_sketch !== "function") return;
    for (var i = 0; i < sound.entries.length; i++) {
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

  // wire: the single-param template suffix for this knob's device MIDI-CC
  // (ic) mapping — one note-addressed command per mapped note, Z-joined in
  // build_knob_configs. The ic mapping carries ONLY this knob's parameter.
  var DRUM_KNOB_DEFS = [
    { key: "pitch",  display_name: "pitch",  min_value: PITCH_MIN_HZ, max_value: PITCH_MAX_HZ, knob_type: "log", wire: "f%v" },
    { key: "offset", display_name: "offset", min_value: 0,    max_value: 0.95, wire: "P%v" },
    { key: "pan",    display_name: "pan",    min_value: 0,    max_value: 1,    wire: "Q%v" },
    { key: "vol",    display_name: "vol",    min_value: 0,    max_value: 1.2,  wire: "a%v" },
    // Cutoff's template pins the filter type (G4 = LPF24) so a CC sweep is
    // audible from any state; unlike the knob it has no bypass-at-max.
    { key: "cutoff", display_name: "cutoff", min_value: CUTOFF_MIN_HZ, max_value: CUTOFF_MAX_HZ, knob_type: "log", wire: "G4F%v" },
    { key: "res",    display_name: "res",    min_value: 0.1,  max_value: 8,    wire: "R%v" },
  ];

  function build_knob_configs(ch, state) {
    var knobs = [];
    state.sounds.forEach(function(sound) {
      var section = sound_header(sound);
      // Device MIDI-CC (ic) template per knob: the knob's single param on
      // each of the sound's notes, e.g. bass-drum pitch on CC 77 installs
      // `i10ic77,1,220,880,0,i%in35f%vZ`. Multi-note sounds (the toms) get a
      // Z-joined multi-command template, same as K257's dual-osc knobs.
      DRUM_KNOB_DEFS.forEach(function(def, di) {
        var config = {
          drum: true,
          section: section,
          display_name: def.display_name,
          cc: "",
          change_code: sound.entries.map(function(e) {
            return "i%in" + e.note + def.wire;
          }).join("Z"),
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
        };
        if (def.key === "offset" && sound.frames > 0) {
          // The offset knob's UI range is a 0..0.95 fraction of the sample,
          // but its wire value is an absolute trigger_phase — give the ic
          // mapping the phase range for this sound's sample length.
          config.cc_min_value = 0;
          config.cc_max_value = Math.floor(0.95 * sound.frames) / PCM_PHASE_DENOM;
        }
        knobs.push(config);
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
    // The channel level (iV) is orthogonal to the per-drum params; a stale
    // pre-iV line stands in for slider positioning until migrated below.
    var level = Number.isFinite(log.level) ? log.level
      : (log.legacyLevel ? log.legacyLevel.val : 1);
    if (!(Number.isFinite(level) && level >= 0)) level = 1;
    state.sounds.forEach(function(sound) {
      var params = null;
      for (var i = 0; i < sound.entries.length && !params; i++) {
        var note = sound.entries[i].note;
        if (log.ioByNote[note] || log.paramsByNote[note]) {
          params = params_from_log(sound, sound.entries[i],
                                   log.ioByNote[note], log.paramsByNote[note], log.ampDivisor);
        }
      }
      sound.params = params || default_params(sound.entries[0]);
    });
    // Position the Level slider from the recorded level.
    if ((Number.isFinite(log.level) || log.legacyLevel)
      && typeof window.setSynthLevelFromAmy === "function") {
      window.setSynthLevelFromAmy(ch, level);
    }
    // Migrate stale-format lines to the current form in one shot:
    // - pre-amy#913 io rewrites were replayed into AMY on load (their
    //   templates don't reference the per-drum oscs, so a hit through one
    //   would broadcast to every osc in the voice) — re-send the kit patch
    //   to restore pristine mappings/oscs, then every logged sound.
    // - a pre-iV level line (i<ch>v0a or the interim i<ch>a broadcast) moves
    //   to i<ch>iV; interim-era per-note amps were vol*level products, undone
    //   by ampDivisor in the recovery above and re-recorded as bare vols by
    //   the same sound re-send.
    if ((log.legacyKeys.length || log.legacyLevel)
      && typeof window.send_amy_message_in_sketch === "function") {
      if (log.legacyKeys.length && typeof window.amy_add_log_message === "function") {
        // Live-only: the kit's K line is already in the log (normalized to
        // iv1 on load) — this just restores AMY's runtime state.
        window.amy_add_log_message("i" + ch + "K" + state.kit + "iv1Z");
      }
      if (log.legacyLevel) {
        window.send_amy_message_in_sketch("i" + ch + "iV" + fmt(level), { synth: ch });
        window.send_amy_message_in_sketch("", { key: log.legacyLevel.key, remove: true, silent: true });
        if (typeof window.setSynthLevelFromAmy === "function") {
          window.setSynthLevelFromAmy(ch, level);
        }
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
