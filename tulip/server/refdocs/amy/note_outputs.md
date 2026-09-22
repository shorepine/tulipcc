# Note outputs: a synth whose notes are CV/gate or MIDI

AMY plays notes on oscillators. On a board with a CV DAC, a gate pin or
a MIDI port you often want a note to go *somewhere else* instead: out to
a modular rack as 1V/oct plus a gate, or out of the MIDI port as a
note-on with velocity.

**A synth can have a note output**, and that is the whole feature:

```python
amy.send(synth=1, note_output='%d,0,2' % amy.NOTE_OUTPUT_CV_GATE)
amy.send(synth=1, note=60, vel=1)     # 3.0 V on control out 0, gate out 2 high
amy.send(synth=1, note=60, vel=0)     # gate low, pitch held at 3.0 V

amy.send(synth=2, note_output='%d,6' % amy.NOTE_OUTPUT_MIDI_OUT)
amy.send(synth=2, note=60, vel=0.8)   # 0x95 0x3C 0x66 out the MIDI port

amy.send(synth=1, note_output='%d' % amy.NOTE_OUTPUT_OFF)
```

## The synth still plays

**A note output is in ADDITION to the synth's own voices**, which sound
exactly as they always did. Layering an internal sound with an external
one is the common case.

**A synth that should be silent inside AMY is simply one you never gave
voices to**, which costs no oscillators and needs no flag.

```python
# plays a Juno AND drives the rack
amy.send(synth=1, num_voices=1, patch=0)
amy.send(synth=1, note_output='%d,0,2' % amy.NOTE_OUTPUT_CV_GATE)

# a pure CV/gate interface: no voices, no oscillators, no sound in AMY
amy.send(synth=2, note_output='%d,1,3' % amy.NOTE_OUTPUT_CV_GATE)
```

Everything else about those synths is unchanged. They are sequenced with
`ticks=`, routed with `to_synth`, delayed with `synth_delay`, and they
come back from `amy_dump_state`. A pattern that plays an internal
instrument plays an external one by changing one line.

## One parameter

The mode and its channels arrive **together**, in one value. Split over
two commands there is a window in which a synth is `MIDI_OUT` holding CV
channel numbers, and there should be no way to express that.

| mode | value | |
|---|---|---|
| `NOTE_OUTPUT_OFF` | `'0'` | the default; the synth is an ordinary synth |
| `NOTE_OUTPUT_CV_GATE` | `'1,<pitch>,<gate>[,<vel>[,<scale>[,<offset>[,<gate_volts>]]]]'` | |
| `NOTE_OUTPUT_MIDI_OUT` | `'2,<channel 1..16>[,<forward_midi_in>]'` | |

The mode is the constant, interpolated, the way every other explicit
constant is given to `amy.send`:

```python
CV = amy.NOTE_OUTPUT_CV_GATE
amy.send(synth=1, note_output='%d,0,2' % CV)          # pitch out 0, gate out 2
amy.send(synth=1, note_output='%d,0,2,3' % CV)        # ...velocity as volts on out 3
amy.send(synth=1, note_output='%d,0,2,,12,24,5' % CV) # ...spelling out the defaults
```

Defaults: 12 semitones per volt, MIDI note 24 (C1) at 0 V, gate high at
5 V, no velocity output.

## A control output is whatever the host says it is

`CV_GATE`'s channels index a flat list of **control outputs** the host
provides. AMY hands over an index and a voltage and stops caring:

```c
void (*amy_external_cv_output_hook)(uint8_t channel, float volts);
```

Whether output 2 is a second DAC channel, a GPIO driven to 0 or 5 V, or
an opto-isolated trigger is entirely the host's business. That is what
makes **using a CV output as a gate free** rather than a feature — a
gate *is* a control output driven to a voltage. A host with two DAC
channels and one gate pin publishes three control outputs; one with four
DAC channels publishes four, and the gate can go on any of them.

`AMY_MAX_CV_OUT` (default 4) range-checks the channel. Raise it for a
host with more.

The hook is called from the render thread as the note event is applied,
so it lands at block resolution rather than at whatever rate the host
polls — keep it to a register write or a mailbox post.

**`MIDI_OUT` needs no hook.** `midi_out()` already reaches every port a
host has wired up.

## What a note does

**Note-on (`CV_GATE`)**: the pitch voltage is written **first**, then the
gate rises. In that order and not the other way round, so nothing
downstream is ever told to look at a voltage that is still moving.

**Note-off (`CV_GATE`)**: the gate falls and **the pitch is held**.
Holding it is what a hardware sequencer does and is what makes a slide
into the next note possible; zeroing it would put a click on every
release.

**`CV_GATE` is monophonic, last-note priority, legato.** One pitch
output carries one note. A note-on while another is held moves the pitch
and **leaves the gate high**, which is how a mono synth gives you a slide
for free; the gate falls only when the last held note is released. A
note-off for a note that is no longer sounding changes nothing audible.

**`MIDI_OUT` is polyphonic** and needs none of that — MIDI carries
several notes on one channel, so notes pass through one for one.

**Velocity**: `MIDI_OUT` sends it as the velocity byte. `CV_GATE` sends
it as a voltage on the optional third channel, scaled 0..1 to 0..gate
volts, and sends nothing if no third channel was given.

**Notes that arrived over MIDI are not forwarded by default**, or a
thru-patched port is a feedback loop. `MIDI_OUT`'s second argument turns
forwarding on, for the MIDI-thru-with-a-transpose case.

## 1V/oct, and why it is the inverse of `ig`

`cv_trigger` (the `ig` command) turns a CV **input** into a note with

```
note = volts * pitch_scale + pitch_offset
```

and this is exactly its inverse, with the same two numbers spelled the
same way:

```
volts = (note - pitch_offset) / pitch_scale
```

So **a host that patches a control output into a CV input and gives both
ends the same scale and offset gets the note back unchanged.** That is
deliberate: it makes the whole feature testable on one board with one
patch cable, and `tests/test_note_output.c` asserts the identity over
seven octaves.

## On the wire

In the synth-layer namespace, deliberately paired with `ig`: lower case
is CV in, upper case is note out. (AMY already uses case this way in the
same namespace — `iv`/`iV`, `im`/`iM`.)

```
i<synth>iG<mode>[,<arg>...]

i1iG1,0,2            CV_GATE, pitch on control out 0, gate on control out 2
i2iG2,6              MIDI_OUT on channel 6
i1iG0                off
```

**The payload is numeric in every binding.** AMY's parser delimits a
command's argument with the next alphabetic character, so a payload
containing letters runs into whatever command follows it — a mode name
could never have gone on the wire. A name that reaches AMY anyway is
**refused out loud** rather than read by `atoff()` as 0, which is
`OFF`, which would be silence with nothing said anywhere.

## Upgrading from `wave=AMY_MIDI`

Wave type 16 is gone. An oscillator with that wave sent a note-on out
MIDI channel 1 — the status byte was hardcoded — and cost an oscillator
to make no sound. `note_output` replaces it with a channel you choose
and no oscillator spent.

**Wave number 16 stays reserved and unused**, so a stored patch that
still names it is silent rather than something else.

**Wave number 16 stays reserved and unused.** A recycled wave number
would be a silent wrong sound in every stored patch and wire string that
still names it.

## Not done yet

- **Polyphonic CV.** `CV_GATE` is mono. The natural extension is voice
  *i* taking pitch channel `pitch + i` and gate channel `gate + i`,
  which is how a polyphonic CV interface is normally wired.
- **Raw MIDI out.** A way to put arbitrary bytes on a `MIDI_OUT` synth's
  port — CCs, program changes — verbatim, with no rewriting of the
  status byte. Arguably a top-level command rather than a synth
  property, since "put these bytes on the MIDI port" needs no synth.
- **`iG` applies on parse**, like `ig`, so it is not captured into a
  patch and does not respect `ticks=`. Configuration rather than
  performance, so it has not mattered yet.
- **Continuous CV out** — driving a control output from an oscillator's
  *signal*, an LFO to a jack — is a different feature and still
  unimplemented. `W`/`external_channel` is the wire letter reserved for
  it. It should use this same hook and the same channel space.
