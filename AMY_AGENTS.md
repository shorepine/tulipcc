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
