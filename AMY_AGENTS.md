# AMY_AGENTS.md

Guidance for the AMYboard **"Prompt to sketch"** code generator
(`POST /api/amyboardworld/generate`). The server reads this file **at request
time** and appends it to the generator's system prompt, so anything here directly
steers the sketches Claude writes.

**How to maintain this file:** add short, high-signal notes that correct a
wrong-but-tempting pattern. The best entries say **"prefer X over Y, and why,"**
with one small *verified* code example. Keep it tight — every word ships in every
generation's prompt and counts toward token cost, so favor a crisp rule + example
over prose.

---

## Before writing code: lean on the provided examples + guidance, and emit only **real** kwargs

You are single-shot — you can't open files, run code, or browse. The EXAMPLES and API
notes in the prompt above, plus this guidance, are your only references: **prefer a
pattern you can see in them over one you're guessing at**, and copy a working example's
shape rather than inventing one.

**Every `amy.send(...)` keyword must be one of these** (AMY's `_KW_MAP_LIST`) — never
invent names like `lfo_osc` / `lfo0_osc`:

```
osc wave note vel amp freq duty feedback time reset phase pan client volume
pitch_bend filter_freq resonance bp0 bp1 eg0 eg1 eg0_type eg1_type debug
chained_osc mod_source eq filter_type ratio latency_ms algo_source load_sample
transfer_file disk_sample algorithm chorus reverb echo patch voices
external_channel portamento sequence tempo synth pedal synth_flags num_voices
oscs_per_voice to_synth grab_midi_notes note_source synth_delay preset
num_partials start_sample stop_sample bus midi_cc midi_note_cmd cv_trigger
patch_string
```

The keys *inside* a coefficient dict (e.g. `duty={'const':…, 'mod':…}`) are a **separate**
set — `const, note, vel, eg0, eg1, mod, bend, ext0, ext1` — not interchangeable with the
kwargs above (see the LFO section). If a name isn't in the right list, it doesn't exist.

> Maintainers: resync this list from `_KW_MAP_LIST` in `amy/amy/__init__.py` when AMY adds kwargs.

---

## Timing & rhythm: `loop()` runs every 32nd note from the always-on sequencer — count calls, don't clock-math beats

AMY's sequencer is always running and calls `loop()` once per **32nd note**. For anything
tempo- or beat-based, schedule events by counting `loop()` calls on that grid — **don't**
convert BPM to milliseconds and measure the gap between beats with a wall clock
(`tulip.amy_ticks_ms()` / `time`). The grid is tempo-locked for you, so hand-rolled beat
math is just extra code that drifts.

Set the tempo with **`sequencer.tempo(bpm)`** (`import sequencer`). The grid is always in
32nd notes: 1 beat (quarter) = **8** calls, 8th = 4, 16th = 2, a 4/4 bar = 32.

### Example — "a four-on-the-floor kick drum, 4/4 at 120 BPM"

```python
import amy, sequencer

sequencer.tempo(120)
# GM drum synth: synth_flags=3 = GM note map, note 36 = kick
amy.send(synth=10, num_voices=1, oscs_per_voice=1, synth_flags=3, wave=amy.PCM, amp=5)

KICK = 36
step = 0
def loop():
    global step
    if step % 8 == 0:        # every 8 thirty-second notes = one beat -> four-on-the-floor
        amy.send(synth=10, note=KICK, vel=1)
    step += 1
```

> Source of truth: `tulip/amyboardweb/sketches/house_generator.py`, `acid_generator.py`
> (`sequencer.tempo(...)` + 32-steps-per-bar patterns driven by a `loop()` step counter).

---

## MIDI-controlled parameters: map the CC inside AMY (`midi_cc` / `ic`), don't poll in `loop()`

When the user asks for a synth parameter (resonance, filter cutoff, amp, pan,
pitch, …) to be controlled by a MIDI CC, **map the CC to the parameter inside
AMY** with `amy.send(synth=N, midi_cc=...)` — the exact mechanism the on-screen
knobs use. **Do not** write a `midi.add_callback` handler that reads the CC and
calls `amy.send()` from `loop()`.

Why: the AMY mapping lives in the audio engine, so the parameter tracks the CC at
control rate with zero per-message Python overhead, and it keeps working even when
`loop()` is busy. A Python polling callback is slower, jittery, and easy to get
wrong.

### Syntax

`amy.send(synth=N, midi_cc="C,L,N,X,O,CMD")` — comma-separated fields:

| field | meaning |
|---|---|
| `C` | MIDI CC number (0–127) |
| `L` | log mapping? `0` = linear, `1` = logarithmic |
| `N` | min value of the mapped output range |
| `X` | max value of the mapped output range |
| `O` | offset added after mapping |
| `CMD` | the AMY **wire command** to run on each CC message, where `%i` → the MIDI channel and `%v` → the mapped value |

`CMD` is a raw AMY wire command. Common parameter wire codes (from
`amy/amy/__init__.py` `_KW_MAP_LIST`): resonance `R`, filter_freq `F`, amp `a`,
pan `Q`, pitch_bend `s`. A synth-level set is `i%i<code>%v` — the wire form of
`amy.send(synth=%i, <param>=%v)`.

### Example — "make resonance controlled by MIDI CC 42"

```python
import amy
amy.send(synth=1, patch=0, num_voices=4)          # Juno patch (has a filter/resonance)
# Map CC 42 -> resonance on synth 1, linear, range 0..8:
amy.send(synth=1, midi_cc="42,0,0,8,0,i%iR%v")

def loop():
    pass
```

`CMD = "i%iR%v"` runs `amy.send(synth=<channel>, resonance=<value>)` on every CC-42
message — no `loop()` code, no callback.

### Managing mappings

- Remove one mapping: `amy.send(synth=N, midi_cc="42")` (CC number only, no other fields).
- Remove all mappings for a synth: `amy.send(synth=N, midi_cc="255")`.

> Source of truth: the editor builds these strings in `build_knob_cc_value()`
> (`tulip/amyboardweb/static/spss.js`); per-knob `change_code` wire commands live in
> `tulip/amyboardweb/static/amy_parameters.js`; the full field spec is the `ic` row
> of `amy/docs/api.md`.

---

## Polyphony & note stealing are automatic: `num_voices` sets them; `num_voices=1` is monophonic

The synth allocates and steals voices for you as MIDI notes arrive — you never track
held notes or implement note priority yourself. A **monophonic** synth is simply
`num_voices=1`: the single voice is reused for each new note, automatically. Add
`portamento` for a gliding mono lead.

**Don't** hand-roll a held-note stack in `midi.add_callback` to fake monophony, and
**don't** set `grab_midi_notes=0` — a synth plays incoming MIDI by default. (Reach for
`grab_midi_notes=0` + a callback only for genuinely custom per-note behavior the engine
can't express, e.g. one key → a whole chord.)

### Example — "a monophonic synth"

```python
import amy

amy.send(synth=1, patch=0, num_voices=1)   # num_voices=1 -> mono; voice stealing is automatic
amy.send(synth=1, portamento=80)           # optional: glide between notes for a mono lead

def loop():
    pass
```

That whole sketch is a playable mono synth — MIDI on channel 1 drives it, no callback.

---

## LFOs / modulation: a silent osc + `mod_source` + a `'mod'` coefficient (there is no `lfo_osc` / `'lfo0'`)

An LFO is just another oscillator routed into a *carrier* osc. To modulate a carrier
parameter with it:

1. Make a low-freq osc and **don't** give it `vel`/`note` — an osc used as a mod source is silent.
2. On the carrier, route it in with **`mod_source=<lfo osc number>`** — the kwarg is `mod_source`, **not** `lfo_osc`/`lfoN_osc`.
3. Add a **`'mod'`** entry to the *target parameter's* coefficient dict, e.g. `duty={'const': 0.5, 'mod': 0.4}` — the key is `'mod'`, **not** `'lfo0'`. Depth ≈ LFO `amp` × the `'mod'` value.

Only `amp`, `freq`, `filter_freq`, `duty`, `pan` accept coefficient dicts. The keys, in
wire order, are: `const, note, vel, eg0, eg1, mod, bend, ext0, ext1`. (Full list:
`amy/docs/synth.md#ctrlcoefficients`.) **Before emitting any `amy.send(...)`, check each
kwarg is a real AMY keyword** (`_KW_MAP_LIST` in `amy/amy/__init__.py`) — don't invent ones like `lfo0_osc`.

### A "synth" is MIDI-driven: set the voice shape first, never send `note`/`vel`/`freq`

When the user says **synth**, build a MIDI-playable instrument, not a free osc:
- **First** command sets the voice shape: `amy.send(synth=N, num_voices=M, oscs_per_voice=K)`.
- Put `synth=N` on **every** following command.
- **Don't** send `note`/`vel`/`freq` — MIDI note-ons (channel = synth number) supply pitch and velocity. Sending them yourself fights the synth.
- The LFO is an extra osc *inside the voice*: `oscs_per_voice=2` → osc 0 = carrier, osc 1 = LFO.

### Example — "pulse-wave synth, pulse width modulated by a 0.5 Hz sine; CC 1 sets the sine's rate 0.1–5 Hz"

```python
import amy

amy.send(synth=1, num_voices=6, oscs_per_voice=2)        # osc0 = carrier, osc1 = LFO
amy.send(synth=1, osc=1, wave=amy.SINE, freq=0.5, amp=1) # the LFO: no vel -> silent
amy.send(synth=1, osc=0, wave=amy.PULSE,
         duty={'const': 0.5, 'mod': 0.4}, mod_source=1)  # PWM driven by osc1
# CC 1 -> LFO rate, 0.1..5 Hz.  CMD i%iv1f%v == amy.send(synth=%i, osc=1, freq=%v)
amy.send(synth=1, midi_cc="1,0,0.1,5,0,i%iv1f%v")

def loop():
    pass
```

No `note`/`vel`/`freq`, no `midi.add_callback` — the synth sounds from incoming MIDI and
`midi_cc` (see the section above) retunes the LFO at control rate.

> Source of truth: `tulip/amyboardweb/sketches/universal_hair.py` (synth + `amy.PULSE` + `mod_source`)
> and `sineclock.py` (`mod_source` + `freq={'const':…, 'mod':…}`); coefficient sources in
> `amy/docs/synth.md` (ctrl/coefficients); wire codes `osc`→`v`, `freq`→`f`, `synth`→`i` in `_KW_MAP_LIST`.

---

## Multiple *audible* oscs per voice: a note-on reaches only osc 0 — chain the rest (or trigger each), and mind the velocity-multiplied `amp`

When a voice has more than one **audible** osc (e.g. two detuned saws for a fat or
phasing pad), a single synth note-on — MIDI, or `amy.send(synth=N, note=…, vel=…)` —
lands on **only the base osc (osc 0)**. The other audible oscs stay **silent** until
they get their own note-on. (A `mod_source`/LFO osc is the exception: it's silent by
design and needs no note-on — see the LFO section.) Drive every audible osc from one
note in one of two ways:

- **Prefer `chained_osc`:** set `chained_osc=1` on osc 0 so its note/velocity
  propagate to osc 1 (chain on, `chained_osc=2` on osc 1, etc.). Each osc keeps its own
  `wave`/`freq`/`pan`; the chain shares one filter. Once chained, the **default `amp`
  just works** — don't override it.
- **`midi_note_cmd`** when you want each osc triggered *independently* (e.g. hard-panned
  L/R layers): give the synth a note handler that fires a note-on at every osc.
  Substitutions: `%i`=channel, `%v`=velocity, `%n`=note. Wire codes: `i`=synth, `v`=osc,
  `l`=vel, `n`=note; `Z` separates phrases. So
  `'i%iv0l%vn%nZi%iv1l%vn%n'` = "on each note, note-on osc 0 **and** osc 1."

Why silence happens: the default `amp='0,0,1,1,0,0,0'` is amplitude = `vel × eg0` — amp
coefficients **multiply**, so an osc that never receives a note-on velocity computes
`amp = 0`. If an osc genuinely won't get a note-on, give `amp` a non-zero **const**
(the 1st coefficient) instead, `amp='1,0,1,1,0,0,0'`, so it sounds from its envelope.

### Example — "a phasing saw voice" (two detuned saws per voice + a pitch-drift LFO)

```python
import amy
# 5-voice synth, 3 oscs/voice: osc0 + osc1 = detuned saws, osc2 = silent pitch-drift LFO
amy.send(synth=1, num_voices=5, oscs_per_voice=3)
# osc 0: saw A, left.  chain to osc 1 so ONE note-on triggers both audible oscs.
amy.send(synth=1, osc=0, wave=amy.SAW_DOWN, pan=0.2, chained_osc=1)
# osc 1: saw B, right, ~8 cents sharp; osc2 LFO drifts its pitch -> phasing
amy.send(synth=1, osc=1, wave=amy.SAW_DOWN, pan=0.8,
         freq='442,1,0,0,0,0.01,1', mod_source=2)
# osc 2: slow sine LFO.  a mod_source osc is silent, so it needs NO note-on.
amy.send(synth=1, osc=2, wave=amy.SINE, freq=0.15, amp=1)

def loop():
    pass
```

Both saws sound on every note (they're chained, so the default `amp` works). To trigger
them independently instead, drop `chained_osc=1` and add
`amy.send(synth=1, midi_note_cmd='i%iv0l%vn%nZi%iv1l%vn%n')`.

> Source of truth: `chained_osc` note/velocity propagation in `amy/docs/juno_patches.md`
> and `amy/docs/api.md` (`c`); "Oscillators will not become audible until a `velocity`
> over 0 is set" plus synth→base-osc routing in `amy/docs/synth.md`; `amp` default/multiply
> and `midi_note_cmd` (`io`) with wire codes `i`/`v`/`l`/`n` in `amy/docs/api.md`.

---

## Audio pass-through: a synth >16 with two `AUDIO_IN0`/`AUDIO_IN1` oscs at `amp=10`

To pass audio input through to the output (e.g. so a global effect like reverb is
applied to live/external audio), make **one synth numbered >16** with **two oscs** —
one per stereo channel — using the `AUDIO_IN0`/`AUDIO_IN1` wave types. **Do not** use a
bare `amy.AUDIO_IN` (no such constant), a single mono osc, the default synth, or `amp<1`.

Why each piece:
- **`synth=18` (any 17–31):** synths 1–16 are bound to MIDI channels; 17+ are free for sketch-owned synths.
- **`AUDIO_IN0` (left) + `AUDIO_IN1` (right):** audio-in is stereo — one osc per channel; there is no mono `AUDIO_IN`.
- **two separate oscs, not chained:** send every per-osc setting (`wave`, `amp`, `pan`, `vel`, `note`) to *each* osc.
- **`amp=10` per osc:** AMY's default volume attenuates ~−20 dB for synth headroom; `amp=10` restores unity (so `amp=0.9` is nearly silent).
- **`vel=1, note=<any>`:** turns the osc on; the note number is required but ignored for `AUDIO_IN`.

### Example — "pass audio input through with reverb"

```python
import amy
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0, amp=10)   # left
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1, amp=10)   # right
amy.send(synth=18, osc=0, vel=1, note=60)   # note number required but ignored
amy.send(synth=18, osc=1, vel=1, note=60)
amy.send(reverb="0.8,0.85,0.5")             # global: level, liveness, damping[, xover_hz]

def loop():
    pass
```

Global effects (`reverb`, `chorus`, `echo`) then process the passed-through audio.

> Source of truth: `tulip/amyboardweb/sketches/audiopassthru.py`; wave types and the
> reverb (`h`) fields are in `amy/docs/api.md`.
