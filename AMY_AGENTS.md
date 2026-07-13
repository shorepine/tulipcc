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

## Timing & rhythm: `loop(step)` runs every 32nd note on the sequencer's bar-locked grid — use `step`, don't clock-math beats

AMY's sequencer is always running and calls `loop(step)` once per **32nd note**, starting
on a bar downbeat. `step` is the global 32nd-note index on the sequencer's bar-locked
grid: **`step % 32 == 0` is always a downbeat**, and it is the same grid `AMYSequence`
events fire on, so patterns built from `step` stay in phase with AMY-sequenced patterns.
For anything tempo- or beat-based, schedule events from `step` — **don't** keep your own
call counter (it can drift if a callback is dropped), and **don't** convert BPM to
milliseconds and measure the gap between beats with a wall clock (`tulip.amy_ticks_ms()`
/ `time`). The grid is tempo-locked for you, so hand-rolled beat math is just extra code
that drifts. (A legacy zero-argument `def loop():` still works and starts on a downbeat,
but new sketches should take `step`.)

Set the tempo with **`sequencer.tempo(bpm)`** (`import sequencer`). The grid is always in
32nd notes: 1 beat (quarter) = **8** steps, 8th = 4, 16th = 2, a 4/4 bar = 32.

### Example — "a four-on-the-floor kick drum, 4/4 at 120 BPM"

```python
import amy, sequencer

sequencer.tempo(120)
# GM drum synth: patch=384 is the TR-808 GM drum kit (note 36 = kick, 38 = snare, 42 = hat...);
# synth_flags=3 routes notes through the GM note map and ignores note-offs.
amy.send(synth=10, num_voices=1, synth_flags=3, patch=384, amp=5)

KICK = 36
def loop(step):
    if step % 8 == 0:        # every 8 thirty-second notes = one beat -> four-on-the-floor
        amy.send(synth=10, note=KICK, vel=1)
```

> Source of truth: `_start_sketch_loop` in `tulip/shared/amyboard-py/amyboard.py`
> (downbeat-aligned dispatch and the `loop(step)` signature) plus
> `tulip/amyboardweb/sketches/house_generator.py`, `acid_generator.py`
> (`sequencer.tempo(...)` + 32-steps-per-bar patterns).

> **GM drums need a drum-kit patch (384-390).** To play drums by GM note number (36=kick,
> 38=snare, 42=closed hat, 46=open hat, 49=crash...) load a drum-kit patch: `amy.send(synth=10,
> num_voices=N, synth_flags=3, patch=384)`. The note→sample mapping lives in the patch.
> Seven kits are available: 384 TR-808, 385 TR-909, 386 Linn 9000, 387 Univox MR-12,
> 388 Tokyo Synthetics, 389 80s Power Kit, 390 Percussion. Switch kits with
> `amy.send(synth=10, patch=38x)` or over MIDI with bank select MSB 3 (CC0=3) + program
> change 0-6 on the drum channel. All the kits' raw samples are also playable directly:
> `wave=amy.PCM, preset=P` with presets 0-18 (the baked 808 bank) and 256-391 (the other
> banks).
> The older recipe — `wave=amy.PCM` + `oscs_per_voice=1` with **no** patch — no longer
> makes sound: bare `wave=amy.PCM` needs an explicit `preset=`, and note numbers alone no
> longer select a drum sample. (`synth_flags` bit 1 used to be a built-in GM-drum note map;
> it is now `SYNTH_FLAGS_NOTES_VIA_MIDI`, which only maps notes when a patch like 384 sets
> up the mapping.) If you just want one raw PCM sample pitched by note, use
> `wave=amy.PCM, preset=P` instead of a patch.

---

## Repeating patterns that must stay tight: put them in `sequencer.AMYSequence`, not `loop()`

Notes sent from `loop()` fire only when Python gets there — anything slow in `loop()`
(display writes, file/World access, heavy math) pushes them late and the groove flams.
An `AMYSequence` event lives **inside the audio engine** and repeats sample-accurately no
matter what Python is doing. Prefer it for any fixed, repeating pattern (drum grooves,
arps, ostinatos); keep `loop()` for UI, knobs, and evolving the pattern.

`sequencer.AMYSequence(length, divider)` is a looping grid of `length` slots, each
`1/divider` of a whole note (so `AMYSequence(32, 32)` = one 4/4 bar of 32nd-note slots).
`seq.add(position, amy.send, synth=…, note=…, vel=…)` schedules a repeating note; it
returns an event with `.update(position, amy.send, …)` to change it and `.remove()` to
delete it. `AMYSequence` slots and `loop(step)` share the same bar-locked grid: slot
`p` of a one-bar `AMYSequence(32, 32)` fires exactly when `step % 32 == p`, so a
`loop(step)` part (bass line, chord stabs) stays in phase with an `AMYSequence` groove.

### Example — four-on-the-floor kick + 8th hats that keep time no matter what loop() does

```python
import amy, sequencer

sequencer.tempo(120)
amy.send(synth=10, patch=384, num_voices=6, synth_flags=3)   # TR-808 GM kit

seq = sequencer.AMYSequence(32, 32)          # one 4/4 bar, 32nd-note grid
for pos in (0, 8, 16, 24):
    seq.add(pos, amy.send, synth=10, note=36, vel=1)         # kick on every beat
for pos in range(0, 32, 4):
    seq.add(pos, amy.send, synth=10, note=42, vel=0.3)       # 8th-note closed hats

def loop(step):
    pass     # free for UI/display/knobs -- the pattern plays itself
```

> Source of truth: `tulip/shared/py/sequencer.py` (`AMYSequence`) and
> `tulip/shared/py/drums.py` (`app.drum_seq.add(position=…, func=amy.send, synth=…,
> note=…, vel=…)` — the drum machine app schedules every switch this way).

---

## The OLED display: draw via `amyboard.display`, refresh with `.show()` — redraws are diffed, but update on musical boundaries, not every `loop()`

`amyboard.display` is a 128×128 grayscale framebuffer: `.text(s, x, y, col)`,
`.fill(col)`, `.fill_rect(x, y, w, h, col)`, `.rect`, `.line`, `.pixel`, plus
`.message(s, row=n)` (one text line with background, includes the refresh). `col` is
0–255. Text rows are 8px tall; `message()` rows are 12px. All methods are safe no-ops
when no display is attached, and draw to the on-screen panel in the web simulator.

`display.show()` is cheap: the driver diffs against what the panel already shows and
transfers **only the rows that changed**, and the transfer itself runs on a background
firmware task — `show()` queues it and returns in ~1ms instead of blocking Python for
the 100–200ms a full-screen I2C push takes on the wire. The clear-everything-and-redraw
idiom is fine, and a `show()` where nothing changed costs ~nothing. Still: refresh when
the *content* changes (once per bar/beat, on a knob turn), not unconditionally every
`loop()` call — redrawing in Python isn't free — and put must-stay-tight patterns in
`AMYSequence` (section above).

### Example — kit name + bar counter, updated once per bar

```python
import amy, amyboard, sequencer

sequencer.tempo(120)
amy.send(synth=10, patch=384, num_voices=6, synth_flags=3)
d = amyboard.display

def loop(step):
    if step % 8 == 0:
        amy.send(synth=10, note=36, vel=1)       # (better: AMYSequence, see above)
    if step % 32 == 0:                           # once per bar, content changed
        d.fill(0)
        d.text("TR-808", 0, 0, 255)
        d.text("bar %d" % (step // 32), 0, 16, 255)
        d.show()
```

> Source of truth: the `Display` class in `tulip/shared/amyboard-py/amyboard.py`;
> shadow-diff partial updates in `tulip/shared/py/ssd1327.py` / `sh1107.py`;
> `tulip/amyboardweb/sketches/house_generator.py` (bar-boundary display updates).

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

def loop(step):
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

def loop(step):
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

def loop(step):
    pass
```

No `note`/`vel`/`freq`, no `midi.add_callback` — the synth sounds from incoming MIDI and
`midi_cc` (see the section above) retunes the LFO at control rate.

> Source of truth: `tulip/amyboardweb/sketches/universal_hair.py` (synth + `amy.PULSE` + `mod_source`)
> and `sineclock.py` (`mod_source` + `freq={'const':…, 'mod':…}`); coefficient sources in
> `amy/docs/synth.md` (ctrl/coefficients); wire codes `osc`→`v`, `freq`→`f`, `synth`→`i` in `_KW_MAP_LIST`.

---

## Multiple *audible* oscs per voice: note-ons are sent to all available oscs

When a voice has more than one **audible** osc (e.g. two detuned saws for a fat or
phasing pad), a single synth note-on — MIDI, or `amy.send(synth=N, note=…, vel=…)` —
is sent to **each separate osc in the voice** (excluding oscs that are controlled by 
other means, e.g. by being the `mod_osc` for another osc, or being part of a `chained_osc` chain). 

(We changed this behavior in May 2026, so you may see examples that make extra effort to send note-ons
to each individual osc in a voice, but this is no longer necessary).

### Example — "a phasing saw voice" (two detuned saws per voice + a pitch-drift LFO)

```python
import amy
# 5-voice synth, 3 oscs/voice: osc0 + osc1 = detuned saws, osc2 = silent pitch-drift LFO
amy.send(synth=1, num_voices=5, oscs_per_voice=3)
# osc 0: saw A, left. 
amy.send(synth=1, osc=0, wave=amy.SAW_DOWN, pan=0.2)
# osc 1: saw B, right, ~8 cents sharp; osc2 LFO drifts its pitch -> phasing
amy.send(synth=1, osc=1, wave=amy.SAW_DOWN, pan=0.8,
         freq='442,1,0,0,0,0.01,1', mod_source=2)
# osc 2: slow sine LFO.  a mod_source osc is silent, so it needs NO note-on.
amy.send(synth=1, osc=2, wave=amy.SINE, freq=0.15, amp=1)

def loop(step):
    pass
```

Both saws sound on every note.

> Source of truth: `chained_osc` note/velocity propagation in `amy/docs/juno_patches.md`
> and `amy/docs/api.md` (`c`); "Oscillators will not become audible until a `velocity`
> greater than 0 is set".
---

## Audio pass-through: a synth >16 with two `AUDIO_IN0`/`AUDIO_IN1` oscs at `amp=10`

To pass audio input through to the output (e.g. so a global effect like reverb is
applied to live/external audio), make **one synth numbered >16** with **two oscs** —
one per stereo channel — using the `AUDIO_IN0`/`AUDIO_IN1` wave types. **Do not** use a
bare `amy.AUDIO_IN` (no such constant), a single mono osc, the default synth, or `amp<1`.

Why each piece:
- **`synth=18` (any 17–31):** synths 1–16 are bound to MIDI channels; 17+ are free for sketch-owned synths.
- **`AUDIO_IN0` (left) + `AUDIO_IN1` (right):** audio-in is stereo — one osc per channel; there is no mono `AUDIO_IN`.
- **two separate oscs:** send every per-osc setting (`wave`, `amp`, `pan`, `vel`, `note`) to *each* osc.
- **`amp=10` per osc:** AMY's default volume attenuates ~−20 dB for synth headroom; `amp=10` restores unity (so `amp=0.9` is nearly silent).
- **`vel=1, note=<any>`:** turns the osc on; the note number is required but ignored for `AUDIO_IN`.

### Example — "pass audio input through with reverb"

```python
import amy
amy.send(synth=18, num_voices=1, oscs_per_voice=2)
amy.send(synth=18, osc=0, wave=amy.AUDIO_IN0, pan=0, amp=10)   # left
amy.send(synth=18, osc=1, wave=amy.AUDIO_IN1, pan=1, amp=10)   # right
amy.send(synth=18, vel=1, note=60)          # note-on sent to both active oscs. note number required but ignored
amy.send(reverb="0.8,0.85,0.5")             # global: level, liveness, damping[, xover_hz]

def loop(step):
    pass
```

Global effects (`reverb`, `chorus`, `echo`) then process the passed-through audio.

> Source of truth: `tulip/amyboardweb/sketches/audiopassthru.py`; wave types and the
> reverb (`h`) fields are in `amy/docs/api.md`.

## `chained_osc` and `wave=amy.SILENT`

To allow a single filter and envelope to be applied to the sum of multiple oscillators (as with a classic
analog synth), the `chained_osc` mechanism accumulates the waveforms for mulitple oscs in a chain. Then 
if the first osc in the chain has `wave=amy.SILENT`, it is a special osc that does not contribute any 
waveform of its own, but instead applies its VCF and envelope to the accumulated waveform from the subsequent
chained oscs.  The entire chain inherits the `pan` of the osc at the head; `pan` values in subsequent `chained_osc`s 
are ignored.  In addition, `vel` and `note` parameters sent to the osc at the head of the chain propagate 
down the chain, so all oscs receive the same note-ons.

### Example - classic analog 2-osc synth voice

Below is equivalebt to the  AMYboard Web Editor UI default voice (built-in patch 257).  Where appropriate,
following this exact allocation of oscs will allow user-defined patches to be controlled by the Editor UI.
Specifically, osc 0 is the `SILENT` osc, receiving VCF and envelope controls; osc 1 is the LFO `mod_osc`, 
oscs 2 and 3 are the "sounding" oscs that are chained to osc 0 (and thus experience its VCF and envelope).

```python
import amy
amy.send(synth=1, num_voices=6, oscs_per_voice=4)

# Osc 1 is the LFO.  The attack envelope causes fade-in.
amy.send(synth=1, osc=1, wave=amy.TRIANGLE, freq=0.609)
amy.send(synth=1, osc=1, bp1='148,1.0,10000,0')

# Osc 0 is the "control" osc applying filter and envelope to the sounding oscs (2 and 3).
amy.send(synth=1, osc=0, wave=amy.SILENT, chained_osc=2,
         mod_source=1, filter_type=amy.FILTER_LPF24)
# Overall note envelope.
amy.send(synth=1, osc=0,
         amp={'const': 0.591, 'vel': 1, 'eg0': 1},
         bp0='518,1,83561,0.299,310,0')
# Filter setup, including envelope shaping with bp1.
amy.send(synth=1, osc=0,
         filter_freq={'const': 300.23, 'eg1': 1.8},
         resonance=1.015,
         bp1='2252,1,1000,0.5,500,0')
# OSCA A (osc 2) is a SAW_UP (sawtooth) wave -- the main harmonic source for this patch.
# It also extends the `chained_osc` chain to include osc 3.
amy.send(synth=1, osc=2, wave=amy.SAW_UP,
         amp={'const': 1, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0},
         freq={'const': 440, 'note': 1, 'mod': 0, 'bend': 1},
         mod_source=1, chained_osc=3)
# OSC B (osc 3) is a PULSE (square/PWM) wave tuned one octave below OSC B
amy.send(synth=1, osc=3, wave=amy.PULSE,
         amp={'const': 0.5, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0},
         freq={'const': 220, 'note': 1, 'mod': 0, 'bend': 1},
         duty={'const': 0.72, 'mod': 0.1},
         mod_source=1)
```

## Rotary encoders: one `amyboard.encoder()` for all hardware — never the per-device helpers

There are three encoder accessories (Adafruit single = 1 knob, Adafruit quad = 4,
M5Stack 8Encoder = 8) plus the simulator's single emulated encoder. **Prefer the
unified `amyboard.encoder()` over the legacy `read_encoder()`/`read_buttons()` or the
`m5_8encoder` module**, because those tie a sketch to one device and a hardcoded
encoder count/I2C address — a sketch that indexes encoder 3 silently breaks on the
single-knob hardware and in the simulator.

Build it once at top level, then read it in `loop()`. Drive parameters from the
**delta** of `read(i)` (not its absolute value) and clamp to `range(enc.encoders)` so
the same sketch scales from 1 to 8 knobs and degrades to a no-op when nothing is
plugged in (`enc.encoders == 0`). Guard LED writes with `enc.leds`.

```python
import amyboard, amy

amy.send(synth=1, patch=0, num_voices=4)
enc = amyboard.encoder()          # autodetect; works on any device or the simulator
_last = [enc.read(i) for i in range(enc.encoders)]

def loop(step):
    for i in range(enc.encoders):
        pos = enc.read(i)
        delta = pos - _last[i]
        _last[i] = pos
        if delta:
            # encoder 0 -> cutoff; spread the rest across other params as you like
            if i == 0:
                amy.send(synth=1, filter_freq=max(50, min(8000, 1000 + pos * 50)))
        if i < enc.leds:
            enc.led(i, 0, 40 if enc.button(i) else 0, 40)
```

`enc.read(i)` is 0-based and starts at 0; `enc.button(i)` is True while held on every
device; `enc.led(i, r, g, b)` takes 0..255 and applies immediately. Never reference an
encoder index `>= enc.encoders` or an LED index `>= enc.leds`.

## Custom C DSP (`tulip.install_c_process` / `tulip.install_c_osc`): AMY first, C only when AMY can't

Sketches can compile and run C audio code inside AMY's engine at runtime — on hardware,
desktop, and the web simulator alike. **Always prefer AMY's built-in features when they
can make the requested sound**: patches, filters (`filter_type`/`filter_freq`/`resonance`),
global `chorus`/`reverb`/`echo`, mod routing, PCM presets. Reach for C DSP **only** when
AMY has no equivalent — distortion / overdrive / bitcrush / waveshaping, ring-mod-style
buffer mangling, or a custom oscillator algorithm (phase distortion, bytebeat, chiptune
tricks). **Never** re-implement in C what AMY already has (no C filters, chorus, echo,
reverb, or standard waveforms).

API (`import tulip`):

- `tulip.install_c_process(name, src)` — compile `src` as an **effect** on the final mix.
  Raises `ValueError` with the compiler message if it doesn't compile.
- `tulip.c_process(name, on, bus=0)` — enable/disable the effect (bus 0 is the main output).
- `tulip.install_c_osc(name, src)` — compile `src` as an **oscillator**.
- `tulip.c_osc(name, osc, on=True)` — point AMY osc number `osc` at it. **Use a high osc
  number (200+)** so the default MIDI synths don't fight you for the osc.
- `tulip.uninstall_c_process(name)` removes either kind; `tulip.c_process_calls(name)`
  counts processed blocks (a debugging aid).

You write only the **body** of a C function; the signature is wrapped for you:

- Effect body sees `int *buf, int frames, int chans` — the live output buffer after AMY's
  FX chain, `chans` sequential channel blocks of `frames` samples (NOT interleaved).
  Modify in place.
- Oscillator body sees `int *buf, int frames, int osc, int phase_inc_q16, int amp` — fill
  mono `buf`. `phase_inc_q16` is the note's per-sample phase step in Q16 (65536 = one
  cycle, tracks bends live); `amp` is the live envelope level — `fxmul` your output by it
  so the osc follows AMY's ADSR. Give the osc an envelope (`bp0=...`) and play it like any
  other osc.

The C environment is strict (the on-device compiler is a deliberately small C):

- Samples are **S8.23 fixed-point `int`s** (1.0 == `1 << 23` == 8388608). Helpers always
  in scope: `fxmul(a, b)`, `cos_lut(phase)` (S8.23 phase, 0..`1<<23` = one cycle),
  `to_int16(s)`, `from_int16(v)`.
- Allowed: `int`/`int16_t`, pointers and arrays, `static`, `while`, `if`/`else`, `return`,
  helper functions, the full integer operator set. **NO floats, structs, `for` loops, or
  preprocessor (`#define`)** — write `while` loops with explicit counters.
- `static` variables persist across calls (zeroed on install) — that's how filters keep
  memory, oscs keep phase (`static int phase[256];` indexed by `osc` gives per-osc state),
  delays keep buffers.
- Keep the body tight: it runs on the audio thread every block; heavy loops cause dropouts.

### Example — "a distortion effect" (AMY makes the sound, C shapes the final mix)

```python
import amy, tulip

amy.send(synth=1, patch=0, num_voices=4)     # a Juno synth, playable over MIDI

tulip.install_c_process('dist', """
    int drive = 10;                      // 1 = clean boost, 10 = heavy
    int i = 0;
    while (i < frames * chans) {
        int s = buf[i] * drive;
        if (s > 8388608) s = 8388608;    // clamp to +/-1.0 (S8.23)
        if (s < -8388608) s = -8388608;
        int y = s - fxmul(fxmul(s, s), s) / 3;   // cubic soft clip: growl, not buzz
        buf[i] = y + y / 2;
        i = i + 1;
    }
""")
tulip.c_process('dist', True)

def loop(step):
    pass
```

A bitcrusher is even shorter: `buf[i] = (buf[i] >> 18) << 18;` in the same while loop.

> Source of truth: `docs/user_c_dsp.md` — the full user guide, with working effect
> (bitcrusher, distortion) and oscillator (CZ phase distortion, bytebeat) examples plus
> debugging tips. `read_reference('docs/user_c_dsp.md')` before writing any C DSP sketch.
> Runnable demo: `tulip/fs/tulip/ex/c_dsp_demo.py`.

---

## MicroPython lacks some Python functions

The script environment is MicroPython which attempts to match regular Python but does not 
include all the standard library functions.  One place this comes up is in the use of 
`random` functions: MicroPython lacks `random.shuffle`; instead, you could do this:
```python
import random

def rand_perm(n):
    """Random permutation of numbers 0..n-1"""
    values = list(range(n))
    result = []
    for _ in range(n):
        index = random.randrange(len(values))
        result.append(values[index])
        del values[index]
    return result

def shuffle(input):
    return [input[i] for i in rand_perm(len(input))]

l = shuffle(['A', 'B', 'C', 'D'])  # l gets the list items in shuffled order.
```
