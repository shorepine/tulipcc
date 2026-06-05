# Understanding Juno Patches

[AMY](https://github.com/shorepine/amy) can be used to simulate many analog synthesizers, and it comes pre-installed with emulations
of the 128 factory patches that came with the Juno-60 (the first MIDI-enabled Juno).  This page explains how the Juno patches work, both
for users who want to modify or manipulate Juno patches, but also as a case-study in how complex voices can be built from basic AMY
oscillators.

You may also want to look at the page on [AMY Synthesizer Details](synth.md) for a more detailed introduction of oscs, voices, synths, and parameters in AMY.

## Juno patches

You can quickly configure a polyphonic Juno synthesizer by loading one of the Juno patches:

```python
# Load the Juno A12 Brass Swell patch onto synth 1 with 6-note polyphony
amy.send(synth=1, num_voices=6, patch=1)
# Play a C major chord
amy.send(synth=1, note=60, vel=1)
amy.send(synth=1, note=64, vel=1)
amy.send(synth=1, note=67, vel=1)
# Stop the chord
amy.send(synth=1, vel=0)  # velocity of zero with no note means 'all notes off'
```

The `patch` argument refers to a set of "wire command strings" in [patches.h](https://github.com/shorepine/amy/blob/main/src/patches.h).
(That file is itself written by [headers.py](https://github.com/shorepine/amy/blob/main/amy/headers.py),
which uses [juno.py](https://github.com/shorepine/amy/blob/main/amy/juno.py) to translate the original Juno SYSEX strings into AMY commands).
The wire string for patch 1 is:
```
v1w4a1,,0,1Z
v0w20c2L1G4Z
v2w1c3L1Z
v3w3c4L1Z
v4w1c5L1Z
v5w5L1Z
v1f0.609A148,1.0,10000,0Z
v2a0.001,0,0,0,0,0f440,1,,,,0,1d0.72,,,,,0m0Z
v3a1,0,0,0,0,0f440,1,,,,0,1m0Z
v4a0.551,0,0,0,0,0f220,1,,,,0,1m0Z
v5a0.001,0,0,0,0,0Z
v0F300.23,0.661,,2.252,0,0R1.015Z
v0a0.591,,1,1,0A518,1,83561,0.299,310,0Z
x7,-3,-3k1,,0.5,0.5Z"
```
Wire strings consist of one or two-letter commands followed by an argument (documented on the [API page](api.md)),
often a single numerical value, or a comma-separated list.
`Z` is a separator, so I've broken the string into separate `Z`-terminated phrases; the stored string does not include any line breaks.
All the settings in a single, `Z`-separated phrase apply to a single osc as specified (or zero by default; some commands like EQ and chorus
apply to all oscs at once, so no osc needs to be indicated).
But even broken up this way, the command strings are not very readable.  However, each command phrase is equivalent to a single `amy.send()`
command in the Python interface.  Thus, the wire strings above correspond to:
```Python
amy.send(osc=1, wave=amy.TRIANGLE, amp={'const':1, 'vel': 0, 'eg0': 1})
amy.send(osc=0, wave=amy.SILENT, chained_osc=2, mod_source=1, filter_type=amy.FILTER_LPF24)
amy.send(osc=2, wave=amy.PULSE, chained_osc=3, mod_source=1)
amy.send(osc=3, wave=amy.SAW_UP, chained_osc=4, mod_source=1)
amy.send(osc=4, wave=amy.PULSE, chained_osc=5, mod_source=1)
amy.send(osc=5, wave=amy.NOISE, mod_source=1)
amy.send(osc=1, freq=0.609, bp1='148,1.0,10000,0')
amy.send(osc=2, amp={'const': 0.001, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0'},
         freq={'const': 440, 'note': 1, 'mod': 0, 'bend': 1}, duty={'const': 0.72, 'mod': 0}, portamento=0)
amy.send(osc=3, amp={'const': 1, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0'},
         freq={'const': 440, 'note': 1, 'mod': 0, 'bend': 1}, portamento=0)
amy.send(osc=4, amp={'const': 0.551, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0'},
         freq={'const': 220, 'note': 1, 'mod': 0, 'bend': 1}, portamento=0)
amy.send(osc=5, amp={'const': 0.001, 'note': 0, 'vel': 0, 'eg0': 0, 'eg1': 0, 'mod': 0'})
amy.send(osc=0, amp={'const': 0.591, 'vel': 1, 'eg0': 1, 'eg1': 0}, bp0='518,1,83561,0.299,310,0')
amy.send(eq='7,-3,-3', chorus='1,,0.5,0.5')
```
Now we can begin to understand how this patch works.  The patch consists of six oscs:

* Osc 0 is the `SILENT` osc, a special osc with gathers the summed outputs of all the oscs connected to it via a `chained_osc` chain.
  The first osc in its chain is 2.
* Osc 1 is the LFO: It has a `TRIANGLE` waveform and a frequency of 0.609 Hz
* Osc 2 (which is part of the chain starting with osc 0) has a `PULSE` waveform and continues the chain with osc 3.
However it also a `const` term in its amplitude (`amp`) of 0.001, or -60 dB relative to 1.0, which makes it inaudible.
* Osc 3 is a `SAW` (sawtooth), and chains forward to osc 4.
* Osc 4 is another `PULSE` oscillator, but with a base frequency of 220 Hz, in contrast to the 440 Hz of oscs 2 and 3.  It chains forward to osc 5.
* Osc 5 has a `NOISE` waveform.  Its amp is also 0.001, i.e. silenced.

Oscs 2-5 correspond to the four waveform sources on the Juno: pulse/PWM, sawtooth, subosc (pulse one octave below), and noise.

We see a chain: Osc 0 > Osc 2 > Osc 3 > Osc 4 > Osc 5.  "Chaining" is a convenience to allow a single note-on/off event to be passed to multiple
oscs in a single event.  So `note` and `vel` parameters sent to osc 0 will propagate down the chain until all of oscs 2-5 see the same event.

`SILENT` oscs, such as osc 0, are special-cases that gather the summed waveforms of all their chained oscs, then apply a single amplitude envelope
(`bp0`, which contributes to `amp` via the `eg0` coefficient) and a filter (`filter_type`).  We *could* put filters and envelopes
on each osc indidually before they get summed through the chain, but its more efficient to do it once after summing the waveforms, since,
for a Juno, all oscillator waveforms are subject to the same envelope and filtering.

Each of the "sounding" oscs, 2-5, has its overall level set by its `amp['const']` coefficient, but no amplitude envelope or velocity scaling
(since the envelope and velocity scaling are applied by osc 0).

The LFO, osc 1, is connected to each of the sounding oscs by their `mod_source` argument.  However, in this patch neither pitch, PWM duty,
or filter frequency, are influenced by the LFO, because in each case their `mod` coefficients are zero.  The LFO does have an amplitude
envelope provided by `bp0`: it fades in over 148 ms (called 'LFO Delay Time' on the original Junos), then decays over 10 seconds, which stands in
for endless sustain.  Modulation oscs (i.e., those named as `mod_source` by some other osc) receive note-on events when the oscs they are modulating
receive note-ons.

The pitched sounding oscs, 2-4, have `portamento` time (in ms) set to 0, i.e., there's no pitch-slide portamento.

Finally, the patch sets stationary EQ, in this case with a 7 dB boost for the low band, and -3 dB for the mid and high bands.  The chorus is
mixed at full amplitude (1), with 0.5 Hz modulating and a delay modulation depth of 0.5 of its maximum length.  This corresponds to mode I of
the Juno chorus.

All Juno patches have the same structure, and vary only in the values of their parameters, corresponding more or less to the positioning of the
sliders on the original Juno front-panel.

Note that the commands above address explicit osc numbers.  But when used to set up a synth (e.g., by including `synth=1` in each `amy.send()`),
the commands refer to an osc relative to the base osc for each voice of the synth, and are thus applied to `num_voices` distinct oscs, one
within each voice of the synth.
