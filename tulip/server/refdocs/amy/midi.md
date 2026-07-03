# AMY MIDI Mode

AMY assigns `synth`s between 1 and 16 to MIDI channels and listens to MIDI messages on those synths. For example, to set up a MIDI channel with a Juno-6 patch, you simply do

```python
amy.send(synth=1, patch_number=3, num_voices=5)
```

Where `num_voices` indicates the polyphony supported for that channel. AMY will handle note stealing and voice allocation. 

## Note ons, offs, pitch bend, sustain

AMY responds to [MIDI note ons](http://midi.teragonaudio.com/tech/midispec/noteon.htm) and [note offs.](http://midi.teragonaudio.com/tech/midispec/noteoff.htm)

AMY responds to [MIDI pitch bend messages.](http://midi.teragonaudio.com/tech/midispec/wheel.htm) This does apply to every note played on the entire AMY instance.

AMY responds to [MIDI sustain pedal messages.](http://midi.teragonaudio.com/tech/midispec/hold.htm)

## Control changes

AMY will turn off all notes when receiving an [All Notes Off control change.](http://midi.teragonaudio.com/tech/midispec/ntnoff.htm)

If you enable a [MIDI input hook](api.md) you can add more controller mappings. In the AMY source we provide [midi_mappings.c](../src/midi_mappings.c) with examples. The `juno_filter_midi_handler` assigns MIDI CC 70 to `filter_freq` and 71 to `resonance`. You probably want to make your own [MIDI input hook](api.md) to expand on this for your use cases.

AMY responds to a so far small set of [MIDI CC changes](http://midi.teragonaudio.com/tech/midispec/ctl.htm) to apply to [CtrlCoefs](synth.md) on that channel.

**TODO: fill this in**

## Program changes

AMY responds to [MIDI program changes](http://midi.teragonaudio.com/tech/midispec/pgm.htm). Since program changes are limited to 0-127, right now, we only support switching within an AMY "bank" -- if a synth is setup for Juno-6 (0-127), you can only change between other Juno-6 patches. If a synth is setup for DX-7 (128-255), you can set any DX-7 patch. For example, to change a DX7 patch from 5 to 3, send a program change to that synth with `128+3 = 131`. 

**TODO: BANKS**

## SYSEX

AMY can receive its [wire messages](api.md) over [SYSEX](http://midi.teragonaudio.com/tech/midispec/sysex.htm). Since wire messages are always in lower ASCII, you do not need to encode your wire message before sending it to AMY. 

Preface your wire message with AMY's SYSEX manufacturer code: `0x00 0x03 0x45`. 

For example, to send the wire message `v0f440l1` (sine wave, 440Hz, note on) to AMY over SYSEX, you'd send `0xF0 0x00 0x03 0x45 v 0 f 4 4 0 l 1 0xF7`. 


## Others

AMY supports MIDI realtime transport for the internal sequencer:

- `0xF8` (Timing Clock): advances the sequencer from external clock input.
- `0xFA` (Start): starts sequencer playback from tick 0 in external clock mode.
- `0xFC` (Stop): stops sequencer playback.

External realtime sync is **off by default** — AMY ignores incoming clock/start/stop and keeps its own tempo. Enable it with the C API `amy_external_midi_sync(1)`, or over the wire with `amy.send(external_midi_sync=1)` (wire command `zC1`). Set it back to 0 to return to the internal clock.

MIDI Timing Clock is 24 PPQ. AMY's sequencer runs at 48 PPQ, so AMY advances two sequencer ticks per one MIDI clock pulse.

You can also start and stop the sequencer transport directly, independent of MIDI clock sync, with `amy.send(sequencer_run=1)` / `amy.send(sequencer_run=0)` (wire command `zY1` / `zY0`). 

## Sending MIDI out

As well as responding to incoming MIDI, AMY can _send_ MIDI out. Set any oscillator's `wave` to `amy.AMY_MIDI` and it stops producing audio — instead, every note you play on that osc is emitted as a MIDI message out whatever MIDI interface you've configured (`amy_config.midi`, e.g. UART or USB gadget).

```python
amy.send(osc=0, wave=amy.AMY_MIDI)   # osc 0 is now a MIDI sender, not an audio oscillator
amy.send(osc=0, note=60, vel=1)      # -> MIDI note on:  channel 1, note 60, velocity 127
amy.send(osc=0, note=60, vel=0)      # -> MIDI note off (sent as a note on with velocity 0)
```

- Messages always go out on **MIDI channel 1** (status byte `0x90`).
- The event's `note` (`midi_note`) and `vel` become the MIDI note number and velocity. Velocity is scaled from AMY's `0.0`–`1.0` range to `0`–`127`. A note-off is sent as a note-on with velocity 0.
- Notes that AMY received over MIDI *in* are **not** echoed back out, so you won't create a feedback loop. Only notes you generate inside AMY — directly, or from the sequencer — are forwarded.

### Sequencing MIDI out

Because an `AMY_MIDI` osc emits MIDI in response to ordinary note events, you can drive it from [AMY's sequencer](synth.md) to get tightly-timed MIDI output — a step sequencer or drum pattern clocked by AMY that plays an external synth. Schedule note-ons and note-offs on the osc just like you would schedule audio notes:

```python
amy.send(osc=0, wave=amy.AMY_MIDI)                    # set up the MIDI sender once

# Send a MIDI note on channel 1 every quarter note (48 ticks), held for an eighth note.
amy.send(osc=0, note=60, vel=1, sequence="0,48,1")    # note on  at tick 0  of each 48-tick period
amy.send(osc=0, note=60, vel=0, sequence="24,48,2")   # note off at tick 24 of each 48-tick period
```

AMY keeps sending those MIDI messages out the port at the configured tempo until you remove them (by their `tag`) or reset the sequencer. See [the sequencer docs](synth.md) for `tick` / `period` / `tag` details.

