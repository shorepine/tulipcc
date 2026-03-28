# Using AMYboard Online

The AMYboard web editor at [amyboard.com](https://amyboard.com/editor) lets you design patches, write Python code, and manage your AMYboard -- all from your browser. You can also use it as a standalone browser-based synthesizer without any hardware at all.

<img src="img/load_patches.png" width=400>

*The patch browser lets you load from your own patches, Juno-6 and DX-7 presets, and community patches from AMYboard World.*

## Getting started

Just open [amyboard.com](https://amyboard.com/editor) in Chrome, Edge, or Firefox (Safari works but MIDI will not work there). Click anywhere on the page to unlock audio, and you're ready to go.

## What you can do online

### Play and edit patches

The web editor gives you a knob-per-function interface for Juno-6 like patches

 - Browse and load any of the 128 patches into the interface
 - Tweak every parameter with on-screen knobs
 - Assign patches to any of the 16 MIDI channels
 - Set min & max & CC assignments per knob (click on the knob title)

### Use Web MIDI

If you have a MIDI controller and a browser that supports WebMIDI, AMYboard Online can use it directly:

 - Click the MIDI device selector to choose your input and output devices
 - Play notes on your controller and hear them through AMYboard's synth engine
 - MIDI CC messages can control patch parameters in real time

### Write Python code

<img src="img/code_editor.png" width=600>

*The Code editor with syntax highlighting, file management, and live execution. Write and edit Python scripts that control AMY directly.*

AMYboard Online includes a full MicroPython REPL running in your browser. You can:

 - Type Python code to control the synth
 - Write and test scripts before deploying them to hardware

<img src="img/terminal_repl.png" width=600>

*The Terminal REPL gives you an interactive MicroPython prompt where you can type AMY commands and see results immediately.*

### Transfer environments to hardware

If you have an AMYboard connected via USB:

 1. Click **Send to AMYboard** in the web editor
 2. Your current patch assignments, modified patches, and any audio files or python files like `env.py` script are packed into an archive and transferred over MIDI SysEx
 3. The hardware AMYboard unpacks and applies everything automatically, then reboots

This is the easiest way to set up your hardware AMYboard from a comfortable editing environment.

Make sure your MIDI out is set your AMYboard! Either directly over USB or via a MIDI cable, choose the correct MIDI output first.

<img src="img/oled_rotary_cv.png" width=600>

*The hardware interface: OLED display showing firmware info, rotary encoder for patch selection, and CV input knobs for modular integration.*

### AMYboard World

AMYboard World is a community sharing hub built into the web editor. You can browse, download, and upload both full environments and individual patches.

 - **Environments**: Complete setups including patch assignments, modified patches, audio files, and `env.py` scripts. Import one and your entire editor state is replaced with the shared environment.
 - **Patches**: Individual `.patch` files you can preview and import into any channel. Click a patch to hear it before committing.

To share your own work:

 1. Give your environment a name and fill in your username and a short description
 2. Click **Upload** to publish it to AMYboard World
 3. Others can find it by browsing or searching

---

## Patch editor in depth

### Channel strip

<img src="img/channel_strip.png" width=700>

The channel strip runs across the top of the Patch tab and controls which synth channel you're editing.

| Control | Description |
|---------|-------------|
| **Channel 1** (dropdown) | Select which of the 16 independent synth channels to edit. Each channel has its own patch, MIDI assignment, and parameter set. |
| **Active** (checkbox) | Enable or disable this channel. Unchecking it silences the channel without losing its settings. |
| **Patch: None** (button) | Click to open the patch browser and load one of the 128 built-in preset patches into the current channel. The button shows the name of the currently loaded patch. |
| **Save** (red button) | Save the current knob values as a named patch. You can recall it later from the patch browser. |
| **Revert** | Reset the current channel back to the last saved or loaded patch, discarding any unsaved edits. |
| **Clear** | Reset all knobs on this channel to their default values and remove the patch name. |
| **Effects** (right, dark) | Scrolls to the global effects panel (EQ, Chorus, Reverb, Echo). Effects are shared across all channels. |

---

### Per-patch knobs

<img src="img/patch_knobs.png" width=700>

The patch knob panel contains six sections organized into two columns. The left column (pink) controls the oscillators and LFO; the right column (blue) controls the filter and envelopes.

#### OSC A — Oscillator A

| Knob | Description |
|------|-------------|
| **freq** | Base frequency of oscillator A in Hz. Default 440 (concert A). |
| **wave** | Waveform selector. Click to cycle through: SINE, SAW, SAW\_DOWN, SQUARE, PULSE, TRIANGLE, NOISE, and more. |
| **duty** | Pulse width for PULSE and SQUARE waveforms (0–1). Has no effect on other waveforms. |
| **level** | Output level (amplitude) of oscillator A (0–1). |

<img src="img/wavetable_presets.png" width=250>

*When you select a wavetable waveform, the Wavetable Presets dropdown lets you choose from built-in wavetable files.*

#### OSC B — Oscillator B

| Knob | Description |
|------|-------------|
| **freq** | Base frequency of oscillator B in Hz. Default 220 (one octave below OSC A). Detune this slightly from OSC A for a thicker sound. |
| **wave** | Waveform selector for oscillator B. Independent of OSC A. |
| **duty** | Pulse width for oscillator B. |
| **level** | Output level of oscillator B (0–1). |

#### LFO — Low Frequency Oscillator

| Knob | Description |
|------|-------------|
| **freq** | LFO rate in Hz. Typical values are below 10 Hz for vibrato/tremolo effects. |
| **wave** | LFO waveform (TRIANGLE, SINE, SAW, SQUARE, etc.). |
| **osc** | How much the LFO modulates oscillator pitch (0–1). Creates vibrato. |
| **pwm** | How much the LFO modulates pulse width of OSC A and B (0–1). Creates pulse-width modulation. |
| **filt** | How much the LFO modulates the VCF cutoff frequency (0–1). Creates filter wobble. |

#### VCF — Voltage-Controlled Filter

| Knob | Description |
|------|-------------|
| **freq** | Filter cutoff frequency in Hz. Lower values make the sound darker/bassier; higher values open the filter. |
| **resonance** | Filter resonance (Q factor, 0–1). Higher values create a sharper peak at the cutoff and can produce self-oscillation. |
| **kbd** | Keyboard tracking amount (0–1). At 1.0, the filter cutoff tracks your MIDI note so higher notes are brighter. |
| **env** | How much the VCF ENV (see below) modulates the filter cutoff. Higher values give more filter sweep per note. |

#### VCF ENV — Filter Envelope

| Knob | Description |
|------|-------------|
| **attack** | Time (ms) for the filter envelope to rise from zero to its peak after a note-on. |
| **decay** | Time (ms) for the filter envelope to fall from peak to sustain level. |
| **sustain** | The filter envelope level held while a key is held down (0–1). |
| **release** | Time (ms) for the filter envelope to fall from sustain to zero after note-off. |

#### ADSR — Amplitude Envelope

| Knob | Description |
|------|-------------|
| **attack** | Time (ms) for the note to reach full volume after note-on. |
| **decay** | Time (ms) for the volume to fall from peak to the sustain level. |
| **sustain** | Volume level held while a key is held (0–1). |
| **release** | Time (ms) for the note to fade to silence after note-off. |

---

### Knob editor (MIDI CC and range)

<img src="img/knob_modal.png" width=250>

*The knob settings popup showing min/max range, log scaling toggle, and MIDI CC assignment with Learn mode.*

Click any knob **label** (the text above or below the knob, e.g. "freq") to open its parameter editor popup.

| Field | Description |
|-------|-------------|
| **min / max** | The real-world range this knob covers. For example, OSC A freq defaults to min 50 Hz / max 2000 Hz. Tighten this range to make the knob more precise over a smaller span of values. |
| **log** | When checked, the knob follows a logarithmic scale — useful for frequency and time parameters where large values are less perceptually important. |
| **MIDI CC (0-127)** | Assign a MIDI Continuous Controller number to this knob. When your controller sends that CC, the knob moves in real time. |
| **Learn** | Click Learn, then move a knob or fader on your MIDI controller. The CC number is detected automatically and filled in. |
| **Save / Cancel** | Save commits the new range and CC assignment. Cancel closes without changing anything. |

---

### Global effects knobs

<img src="img/effects_knobs.png" width=220>

The Effects panel (right side of the Patch tab) contains four global processors shared by all channels.

#### EQ — 3-Band Equalizer

| Knob | Description |
|------|-------------|
| **low** | Low-frequency shelf gain (dB). Positive boosts bass; negative cuts it. |
| **mid** | Mid-frequency gain. |
| **high** | High-frequency shelf gain. Positive adds brightness; negative rolls off treble. |

#### Chorus

| Knob | Description |
|------|-------------|
| **level** | Chorus wet/dry mix (0–1). 0 = dry, 1 = full chorus. |
| **freq** | Chorus LFO rate. Higher values give a faster, more intense modulation. |
| **depth** | Chorus depth — how wide the pitch modulation swings. More depth = thicker, swimmy sound. |

#### Reverb

| Knob | Description |
|------|-------------|
| **level** | Reverb wet/dry mix (0–1). |
| **live** | Room size / decay time (0–1). Higher values give a longer, larger-sounding reverb tail. |
| **damp** | High-frequency damping of the reverb tail (0–1). Higher values make the reverb sound warmer and less harsh. |

#### Echo

| Knob | Description |
|------|-------------|
| **level** | Echo wet/dry mix (0–1). |
| **delay** | Echo delay time in milliseconds. |
| **feedback** | How much of the echo signal feeds back into itself (0–1). Higher values create longer, repeating echoes. Keep below 1 to avoid runaway feedback. |

[Back to Getting Started](index.md)
