# AMY synthesizer API

This page collects the current API for [AMY](https://github.com/shorepine/amy).

[**Please see our interactive AMY tutorial for more tips on using AMY**](https://shorepine.github.io/amy/tutorial.html)

## C / Arduino API

Parsing, creating and adding events to AMY:

```c
// returns default empty amy_event
amy_event amy_default_event();

// clears an existing AMY event
amy_clear_event(amy_event *e);

// given an event play / schedule the event directly
amy_add_event(amy_event *e);

// given a wire message string play / schedule the event directly
amy_add_message(char *message);
```

Two sample types appear in this API: `output_sample_type` is a final
interleaved audio sample, an `int16_t`; `SAMPLE` is AMY's internal sample
format, S8.23 fixed point in an `int32_t` (so 1.0 is `1<<23` — see
`src/amy_fixedpoint.h`).

Get and set external audio buffers:
```c

// get AUDIO_IN0 and AUDIO_IN1
amy_get_input_buffer(output_sample_type * samples);

// set AUDIO_EXT0 and AUDIO_EXT1
amy_set_external_input_buffer(output_sample_type * samples);
```

If not running live, render a new block of AMY audio into a int16_t buffer:

```c
output_sample_type * amy_simple_fill_buffer();
```

Get the AMY time:
```c
// on all platforms, sysclock is based on total samples played, using audio out (i2s or etc) as system clock
uint32_t amy_sysclock();
```


Start and stop AMY:
```c
// Emscripten web start
amy_start_web();
amy_start_web_no_synths();

// Start AMY with a config.  If c.audio is set, will attempt to start live audio
amy_start(amy_config_t c);

// Stop AMY including any live audio output
amy_stop();

```

Default MIDI handlers:
```c
void amy_enable_juno_filter_midi_handler(); // assigns the Juno-6 MIDI CC handler
```

## Cross-platform C API (table-driven)

These C functions are exposed to every host language from a single function
table in `scripts/gen_amy_c_api.py`. The generator emits the CPython module
(`c_amy`), the MicroPython bindings (the `tulip` module on Tulip / AMYboard /
Tulip Desktop / VCV), the web WASM bridge (`amy_c_api` in JS, wired back into
MicroPython on the web builds), and the Godot `AmySynth` methods — so
`amy.<name>(...)` in Python behaves identically on every platform. The
`tulip.amy_*` names are legacy redirects to the same bindings.

To add a function: edit the table in `scripts/gen_amy_c_api.py`, run
`make c-api`, and commit the regenerated files (CI runs `make check-c-api`).
The table below is generated from the same source.

Not everything belongs here. `amy.reset_sysclock()` (and its GDScript and JS
equivalents) is written natively in each binding as a `send()` of
`reset=RESET_TIMEBASE`: the reset is an ordinary event, so no C entry point is
needed.

<!-- BEGIN GENERATED C API DOCS - scripts/gen_amy_c_api.py -->
| Python (all platforms) | C function | MicroPython alias | Godot (`AmySynth`) | What it does |
|---|---|---|---|---|
| `amy.send_wire(message)` | `void amy_add_message(char * message)` | `tulip.amy_send` | — | Send a wire-protocol message to AMY |
| `amy.send_wire_from_sysex(message)` | `void amy_send_wire_from_sysex(char * message)` | `tulip.amy_send_wire_from_sysex` | — | Send a wire message as if from sysex (file-transfer routing applies) |
| `amy.ticks_ms()` | `uint32_t amy_sysclock()` | `tulip.amy_ticks_ms` | — | Read the AMY millisecond clock |
| `amy.render_load()` | `float amy_get_render_load()` | `tulip.amy_render_load` | `render_load` | Smoothed fraction of real time AMY spends rendering (0..1) |
| `amy.set_render_load_threshold(threshold)` | `void amy_set_render_load_threshold(float threshold)` | `tulip.amy_set_render_load_threshold` | `set_render_load_threshold` | Set the render-load fraction that trips the overload failsafe (0 disables) |
| `amy.bleep(start=0)` | `void amy_bleep(uint32_t start)` | `tulip.amy_bleep` | `bleep` | Play the startup bleep |
| `amy.sequencer_ticks()` | `uint32_t sequencer_ticks()` | `tulip.amy_sequencer_ticks` | `sequencer_ticks` | Read the sequencer tick count |
| `amy.process_single_midi_byte(byte, from_web_or_usb=1)` | `void amy_process_single_midi_byte(uint8_t byte, uint8_t from_web_or_usb)` | `tulip.amy_process_single_midi_byte` | — | Feed one MIDI byte to AMY's stream parser |
| `amy.set_cv_from_osc(cv_channel, osc)` | `void set_cv_from_osc(int cv_channel, int osc)` | `tulip.amy_set_cv_from_osc` | — | Feed external CV input from a mod osc (testing support) |
| `amy.get_synth_commands` (low-level; see below) | `void *yield_synth_commands(uint8_t synth, char *s, size_t len, bool include_fx, void *state)` | `tulip.amy_get_synth_commands` | — | Read the wire commands that reconstruct synth state (list of str) |
| `amy.dump_state()` | `char *amy_dump_state_to_string(int *out_len)` | `tulip.amy_dump_state` | `dump_state` | Read the complete replayable AMY state as a wire-command string |
| `amy.get_output_buffer()` | `int amy_get_output_buffer(int16_t *samples)` | `tulip.amy_get_output_buffer` | — | Read the most recent rendered audio block as bytes (None if none ready) |
| `amy.get_input_buffer()` | `int amy_get_input_buffer(int16_t *samples)` | `tulip.amy_get_input_buffer` | — | Read the most recent captured input audio block as bytes (None if none ready) |

Notes:

- `amy.get_synth_commands(synth, ...)` is a hand-written wrapper in
  `amy/__init__.py` adding `patch_num`/`dest_synth` handling on top of the
  generated backend (which drives the C generator `yield_synth_commands`);
  it returns the commands newline-joined into one string.
- `amy.get_output_buffer()` / `amy.get_input_buffer()` return up to 1024
  bytes of interleaved int16 samples (`None` when no block is ready).
- `amy.dump_state()` returns the complete replayable engine state as a
  newline-separated wire-command string.

<!-- END GENERATED C API DOCS -->

## JavaScript API

AMY provides a high-level JavaScript API (`amy_send`) that mirrors the Python `amy.send()` interface. It is auto-generated from the same source of truth (`amy/__init__.py` and `amy/constants.py`) so parameter names are always identical to Python. The connector and API are bundled into `amy.js`, so you only need two includes:

```html
<script src="enable-threads.js"></script>
<script src="amy.js"></script>
```

### `amy_send(params)`

Build and send an AMY wire message from a JS object. Parameter names and types are the same as Python `amy.send()`.

```js
// Play a sine wave at 440 Hz
amy_send({osc: 0, wave: AMY.SINE, freq: 440, vel: 1})

// Load a Juno-6 patch
amy_send({patch: 0, synth: 1, num_voices: 6})
amy_send({synth: 1, vel: 1, note: 60})

// FM synthesis with CtrlCoef dict
amy_send({osc: 0, amp: {const: 1, vel: 0, eg0: 2}, bp0: '0,0,5000,1,0,0'})
```

### `amy_message(params)`

Like `amy_send` but returns the wire string without sending it. Useful for debugging or building messages to send later.

```js
amy_message({osc: 0, wave: AMY.SINE, freq: 440})  // => "v0w0f440Z"
```

### `AMY` constants

All constants from `amy/constants.py` are available on the global `AMY` object:

```js
AMY.SINE         // 0
AMY.SAW_DOWN     // 2
AMY.PULSE        // 1
AMY.FILTER_LPF   // 1
AMY.ALGO         // 8
// ... all constants from amy/constants.py
```

### Regenerating

The JS API is generated during `make web` and bundled into `docs/amy.js` by `make deploy-web`. To regenerate after changing `amy/__init__.py` or `amy/constants.py`:

```bash
make deploy-web
```

### Try it live

[**AMY JavaScript REPL**](https://shorepine.github.io/amy/repl.html) — write and run `amy_send()` commands in your browser.

## `amy_config_t`

Use like:

```c
amy_config_t amy_config = amy_default_config()
amy_config.max_sequencer_tags = 128;
amy_start(amy_config);
```

| Field   | Values | Default   |  Notes                                 |
| ------  | ------ | --------  |  --------                              |
| `features.chorus` | `0=off, 1=on` | On | If chorus is enabled (uses RAM) |
| `features.reverb` | `0=off, 1=on` | On | If reverb is enabled (uses RAM) |
| `features.echo` | `0=off, 1=on` | On | If echo is enabled (uses RAM) |
| `features.partials` | `0=off, 1=on` | On | If partials are enabled |
| `features.custom` | `0=off, 1=on` | On | If custom C oscillators are enabled |
| `features.audio_in` | `0=off, 1=on` | Off | If audio_in gets processed via the audio interface. Must be 1 for AUDIO_IS_MINIAUDIO |
| `features.default_synths` | `0=off, 1=on` | Off| If AMY boots with Juno-6 on `synth` 1 and GM drums on `synth` 10 |
| `features.startup_bleep` | `0=off, 1=on` | Off | If AMY plays a startup sound on boot |
| `platform.multicore` | `0=off, 1=on` | On | Attempts to use 2nd core if available |
| `platform.multithread` | `0=off, 1=on` | On | Attempts to multithreading if available (ESP/RTOS) |
| `midi` | `AMY_MIDI_IS_NONE`, `AMY_MIDI_IS_UART`, `AMY_MIDI_IS_USB_GADGET`, `AMY_MIDI_IS_WEBMIDI` | `AMY_MIDI_IS_NONE` | Which MIDI interface(s) are active |
| `audio` | `AMY_AUDIO_IS_NONE`, `AMY_AUDIO_IS_I2S`, `AMY_AUDIO_IS_USB_GADGET`, `AMY_AUDIO_IS_MINIAUDIO`| I2S or miniaudio | Which audio interface(s) are active |
| `write_samples_fn` | fn ptr | `NULL` | If provided, `amy_update` will call this with each new block of samples | 
| `max_oscs` | Int | 180 | How many oscillators to support |
| `max_buses` | Int | 4 | How many FX buses to support. No compile-time ceiling — every bus-indexed table is allocated from this at `amy_start`. Each bus costs a few KB of mix buffers even when idle, plus whatever its effects allocate once switched on |
| `max_sequencer_tags` | Int | 256 | How many sequencer items to handle |
| `max_voices` | Int | 64 | How many voices |
| `max_synths` | Int | 64 | How many synths |
| `max_memory_patches` | Int | 32 | How many in memory patches to supprot |
| `i2s_lrc`, `i2s_dout`, `i2s_din`, `i2s_bclk`, `i2s_mclk` | Int | -1 | Pin numbers for the I2S interface |
| `midi_out`, `midi_in` | Int | -1 | Pin number for the MIDI UART pins |
| `midi_uart` | 0,1,[2] | -1 | UART device index for MCU. Default 1 (`UART1`) on Pi Pico and ESP. Teensy is always `8` |
| `capture_device_id`, `playback_device_id` | Int | -1 | Which miniaudio device to use, -1 is auto |


## Hooks

Hooks are configured on `amy_config_t` before calling `amy_start`:

```c
amy_config_t amy_config = amy_default_config();
amy_config.amy_external_midi_input_hook = my_midi_hook;
amy_config.amy_external_render_hook = my_render_hook;
amy_start(amy_config);
```

Hook fields in `amy_config_t`:

| Hook | Signature | Used by | Description |
| ---- | --------- | ------- | ----------- |
| `amy_external_render_hook` | `uint8_t (uint16_t osc, SAMPLE *buf, uint16_t len)` | — | Custom oscillator renderer for redirecting the output waveforms on an osc-by-osc level. Return 1 if handled, in which case that osc does not contribute to the normal output. |
| `amy_external_bus_postprocess_hook` | `void (uint16_t bus, SAMPLE *buf, uint16_t len)` | — | Custom effect processing, called at the end of each bus' effects chain (after EQ/chorus/echo/reverb), before buses are mixed to the output. `buf` is `AMY_NCHANS` sequential (non-interleaved) channel blocks of `len` `SAMPLE`s (S8.23 in `int32_t`, see above) each; modify it in place. Called every block for each bus from 0 up to the highest bus activated so far — so bus 0 is always processed, but a hook installed for bus 2 only starts firing once something (an osc assignment, a bus FX setting) has touched bus 2. |
| `amy_external_coef_hook` | `float (uint16_t channel)` | — | Provide external coefficient values (e.g. CV input). |
| `amy_external_block_done_hook` | `void (void)` | — | Called after each audio block is rendered. |
| `amy_external_midi_input_hook` | `void (uint8_t *bytes, uint16_t len, uint8_t is_sysex)` | — | Called when MIDI bytes are received. |
| `amy_external_midi_output_hook` | `void (uint8_t *bytes, uint16_t len)` | — | Called with every run of bytes AMY sends over MIDI out (notes, clock, sysex responses), before — and regardless of — any device interface configured in `midi`. Use it to forward AMY's MIDI output to a transport AMY doesn't drive itself, e.g. BLE MIDI. May be called from the render/sequencer task; keep it fast. |
| `amy_external_sequencer_hook` | `void (uint32_t tick_count)` | — | Called on each sequencer tick. |
| `amy_external_fopen_hook` | `uint32_t (char *filename, const char *mode)` | `zT`, `zD`, `zF` | Open a file on host disk. Returns opaque handle. |
| `amy_external_fwrite_hook` | `uint32_t (uint32_t fptr, uint8_t *bytes, uint32_t len)` | `zT` | Write bytes to a file opened via fopen hook. |
| `amy_external_fread_hook` | `uint32_t (uint32_t fptr, uint8_t *bytes, uint32_t len)` | `zD` | Read bytes from a file opened via fopen hook. |
| `amy_external_fseek_hook` | `void (uint32_t fptr, uint32_t pos)` | `zD` | Seek to position in a file opened via fopen hook. |
| `amy_external_fclose_hook` | `void (uint32_t fptr)` | `zT`, `zD`, `zF` | Close a file opened via fopen hook. |
| `amy_external_file_transfer_done_hook` | `void (const char *filename)` | `zT` | Called after a `zT` file transfer completes. On AMYboard, restarts sketch.py. |
| `amy_external_exec_hook` | `void (const char *code)` | `zP` | Called by `zP` to execute a string on the host. On AMYboard, runs the string as Python via `exec()`. |
| `amy_external_reboot_hook` | `void (uint8_t mode)` | `zB` | Called by `zB` to reboot the host. `mode` selects which post-reboot state: `0` = bootloader (skip sketch on next boot), `1` = normal reboot (run sketch), `2` = ROM download / flash mode. Handled in pure C before `mp_sched_schedule`. On AMYboard, sets an RTC flag with the requested mode and calls `esp_restart()`. |

All hook fields default to `NULL` in `amy_default_config()`.

### JS hook variants (web build)

On web, AMY renders inside an AudioWorklet, so the render and bus-postprocess
hooks have JS variants: define a global function in the AudioWorklet scope
(i.e. in JS loaded as part of `amy.js`, which the worklet also evaluates) and
AMY calls it per block. Pointers arrive as byte offsets into the module's
linear memory; `module` is the worklet scope's Emscripten `Module` instance,
whose `wasmMemory` and exports (`_cos_lut`, `_malloc`, ...) let hook JS read
the buffers or instantiate further wasm sharing AMY's memory.

| JS global | Signature | Description |
| --------- | --------- | ----------- |
| `amy_render_js_hook` | `(osc, buf, len, phase_inc, amp, module) -> handled` | Web version of `amy_external_render_hook`. `phase_inc` is the osc's current pitch as a phase increment in cycles/sample (tracks bend/portamento), `amp` its envelope level 0..1. Return 1 if the osc was handled (skips the normal mix), 0 to let AMY mix `buf` (with pan) as usual. |
| `amy_bus_postprocess_js_hook` | `(bus, buf, len, nchans, module)` | Web version of `amy_external_bus_postprocess_hook`: process the bus buffer (`nchans` sequential channel blocks of `len` S8.23 samples) in place. |

Because the page's JS and the worklet's hook JS run in separate scopes sharing
only linear memory, `amy_set_external_hook_context(void *)` /
`amy_get_external_hook_context()` (both exported on web) hold one opaque
pointer for embedders: the page `_malloc`s a control block and stores it, and
worklet hook JS finds it via `module._amy_get_external_hook_context()`.


## `amy_event`, `amy.send`, and `amy_send` API:

AMY parameters can be set via three interfaces:
- **C**: Set fields on `amy_event` structs
- **Python**: `amy.send(param=value, ...)`
- **JavaScript**: `amy_send({param: value, ...})`

Python and JavaScript use identical parameter names (shown in the **Python / JS** column below). A few parameters are not yet available via C `amy_event` (marked **TODO**).

Please see [AMY synthesizer details](synth.md) for more explanation on the synthesizer parameters.

[**Please see our interactive AMY tutorial for more tips on using AMY**](https://shorepine.github.io/amy/tutorial.html)

A note on list parameters:  When an argument is a list of parameters, you can in general set any subset of those parameters by omitting the values you don't want to change - either by leaving them in their initial `AMY_UNSET` value in C, or by having missing values in Python lists.  For instance, you can set up an envelope that moves immediately to 1, then decase to a sutain level of 0.5 over 200ms, then has a 300ms decay to zero on note-off, with `bp0='0,1,200,0.5,300,0'.  Subsequently, you could change just the sustain level (the 4th value in the list) to 0.2 with `bp0=,,,0.2`.  However, there's at present no way to say ".. and the list should now only be 4 items long.  This only affects breakpoint sets which are variable length, but the net result is that once you have a certain number of breakpoints in a list, you cannot shorten it except by resetting the whole osc and building it all up again.


### `synth`s:

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `i`    | `synth` | `synth`  | 0-31  | Define a set of voices for voice management. |
| `ic`   | **TODO** | `midi_cc`  | C,L,N,X,O,CMD | MIDI Control Code command for this synth (1-16).  `C`=MIDI CC (0-127), `L`=log mapping (0/1), `N`=min val, `X`=max val, `O`=offset, `CMD`=wire command to execute, where `%i` is replaced by the channel number and `%v` is replaced by the value after min/max/offset/log mapping.  Providing `C` with no further args deletes that CC.  `C=255` deletes all CC mappings for the specified synth. See [#524](https://github.com/shorepine/amy/issues/524) |
| `id`   | `synth_delay_ms` | `synth_delay` | uint | Delay (in ms) applied to synth note-ons.  Gives time for decay of 'stolen' notes. |
| `if`   | `synth_flags` | `synth_flags` | uint | Flags for synth creation: 1 = Use MIDI drum note->preset translation; 2 = Drop note-off events; 4 = Invert MIDI pedal sense. |
| `ig`   | `cv_trigger` | `cv_trigger` | uint,uint | Configure external CV event triggering: Gate CV, trigger threshold, reset threshold, pitch CV, pitch scale, pitch offset, wire command template, %v gets pitch value. |
| `im`   | `grab_midi_notes` | `grab_midi_notes` | 0/1 | Use `amy.send(synth=CHANNEL, grab_midi_notes=0)` to prevent the default direct forwarding of MIDI note-on/offs to synth CHANNEL. |
| `iM`   | `note_source_channel` | `note_source_channel` | 1-16 | Used internally to mark events that result from MIDI inputs, meaning they won't get forwarded to MIDI out. |
| `in`   | `oscs_per_voice`  | `oscs_per_voice`  | >0  | Reserve this many oscs for each voice.  Needed when initializing a synth (or voice) withouth an initial patch. Setting `oscs_per_voice` on an existing synth resets all oscs to their default state. |
| `io`   | **TODO** | `midi_note_cmd`  | M,L,N,X,O,CMD | MIDI Note on/off command for this synth.  M=MIDI note number, or -1 for all notes.  Other args map the velocity, as for `ic`. `%n` is substituted with the note number. |
| `ip`   | `pedal` | `pedal` | int | Non-zero means pedal is down (i.e., sustain).  Must be used with `synth`. |
| `it`   | `to_synth` |  `to_synth` | 0-31 | New synth number, when changing the number (MIDI channel for n=1..16) of an entire synth. |
| `iv`   | `num_voices` | `num_voices` | int | The number of voices to allocate when defining a synth.  Only valid with `synth=X`. |
| `iV`   | `synth_level` | `synth_level` | float >= 0 | Per-instrument level, default 1.0.  Scales the audio of every osc in the synth at render time (applied at the output stage, just before the pan/bus mix), independent of any osc `amp` settings.  The natural target for a channel volume control (e.g. MIDI CC 7) — works for every synth type, including drum kits whose per-drum oscs carry their own amps. |
| `iy`   | `bus`   | `bus`   | int | Bus onto which the synth outputs are added (synonym for `y`). |
| `K`    | `patch_number` | `patch` | uint 0-X | Apply a saved or user patch to a specified synth or voice. Built-ins: 0-127 Juno, 128-255 DX7, 256 piano, 258 legacy GM drums, 384-390 Gamma9001 GM drum kits (see Drum kits below), 1024+ user patches. |
| `u`    | **TODO**| `patch_string` | string | Provide AMY message to define up to 32 patches in RAM with ID numbers (1024-1055) provided via `patch_number`, or directly configure a `synth`. |


### Oscillator control

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `v`    | `osc` | `osc` | uint 0 to OSCS-1 | Which oscillator to control. In a command addressed to a synth this is relative to the voice, as are `chained_osc`, `mod_source` and `algo_source`; a number outside the voice is refused (see [synth.md](synth.md)) |
| `w`    | `wave` | `wave` | uint 0-21 | Waveform: [0=SINE, PULSE, SAW_DOWN, SAW_UP, TRIANGLE, NOISE, KS, PCM, ALGO, PARTIAL, BYO_PARTIALS, INTERP_PARTIALS, AUDIO_IN0, AUDIO_IN1, AUDIO_EXT0, AUDIO_EXT1, AMY_MIDI, PCM_LEFT, PCM_RIGHT, WAVETABLE, CUSTOM, OFF]. default: 0/SINE |
| `ww`   | `mode` | `mode` | uint | Wave-specific modes.  For PCM, we have `PCM_PLAY` (plays through to end), `PCM_PLAY_STOP` (stops immediately on note-off, default), `PCM_LOOP` (observes loop marks in wave, plays through release on note-off), `PCM_LOOP_STOP` (plays loop, stops immediately on note-off), `PCM_LOOP_FOREVER` (loops, and ignores note-offs, expected to be used with an EG). |
| `S`    | `reset_osc`| `reset`  | uint | Resets given oscillator. set to RESET_ALL_OSCS to reset all oscillators, gain and EQ. RESET_TIMEBASE resets the clock and sequencer tick count to zero. It is an ordinary event, so it works from `amy_add_event()` as well as the wire, and can be scheduled with `ticks=` (or `amy_event.time` from the C API) like any other event; the counters restart at the next audio block boundary, so the reset cannot race the render thread, and events already queued for a future time keep their relative timing across it. (RESET_AMY and RESET_EVENTS are the exceptions: they act the moment the message is parsed, since neither restarting AMY nor emptying the event queue can be carried in a queued event.) RESET_SYNTHS is a deprecated alias for RESET_ALL_OSCS; prefer RESET_ALL_OSCS. RESET_AMY restarts AMY. RESET_SEQUENCER clears the sequencer.|
| `A`    | `eg0_times[]`, `eg0_values[]` | `bp0`    | string (wire) / arrays (`amy_event`)      | Envelope Generator 0 breakpoints as time(ms),value pairs. Wire/Python format remains comma-separated, e.g. `100,0.5,50,0.25,200,0`. In C `amy_event`, use typed arrays (`eg0_times[i]`, `eg0_values[i]`). The last pair is release (triggers on note off). |
| `B`    | `eg1_times[]`, `eg1_values[]` | `bp1`    | string (wire) / arrays (`amy_event`)      | Envelope Generator 1 breakpoints. Wire/Python format remains comma-separated; in C `amy_event`, use typed arrays (`eg1_times[i]`, `eg1_values[i]`). |
| `b`    | `feedback` | `feedback` | float 0-1 | Use for the ALGO synthesis type in FM or for karplus-strong, or to indicate PCM looping (0 off, >0, on) |
| `c`    | `chained_osc` | `chained_osc` |  uint 0 to OSCS-1 | Chained oscillator.  Note/velocity events to this oscillator will propagate to chained oscillators.  VCF is run only for first osc in chain, but applies to all oscs in chain. |
| `G`    | `filter_type` | `filter_type` | 0-6 | Filter type: 0 = none (default.) 1 = lowpass, 2 = bandpass, 3 = highpass, 4 = double-order lowpass, 5 = notch, 6 = phaser. A letter instead of a digit selects a distortion sub-command; see the `G` distortion rows below. |
| `GC` / `GF` | `dist_type` | `dist_clip` / `dist_fold` | 0 or 1 | Per-oscillator distortion, applied before the filter. `GC1` enables soft clip, `GF1` enables wavefold; 0 turns the stage off. |
| `GH`   | `dist_type`, `dist_bits`, `dist_rate` | `dist_crush` | list of 2 ints | Enables the bitcrusher as [bits, rate]. bits: bit depth 1-24, >= 24 leaves bit depth unchanged. rate: sample-hold length in samples, 1 disables. `GH0` turns the stage off. |
| `GD`   | `dist_drive` | `dist_drive` | float 0-16 | Distortion pre-gain (fold depth for wavefold), shared by all types; default 1. |
| `GM`   | `dist_mix` | `dist_mix` | float 0-1 | Distortion wet/dry, shared by all types; default 1 (full wet). |
| `I`    | `ratio` | `ratio`  | float | For ALGO types, ratio of modulator frequency to  base note frequency  |
| `L`    | `mod_source` | `mod_source` | 0 to OSCS-1, up to two, comma-separated | Which oscillator(s) are used as modulation/LFO sources for this oscillator. Source oscillators will be silent. The first feeds the `mod0` control coefficient and the second `mod1`, so `mod_source=[3, 4]` makes osc 3 the `mod0` input and osc 4 the `mod1` input. |
| `m`    | `portamento`| `portamento` | uint | Time constant (in ms) for pitch changes when note is changed without intervening note-off.  default 0 (immediate), 100 is good. |
| `n`    | `midi_note` | `note` | float, but typ. uint 0-127 | Midi note, sets frequency.  Fractional Midi notes are allowed. |
| `o`    | `algorithm` | `algorithm` | uint 1-32 | DX7 FM algorithm to use for ALGO type |
| `O`    | `algo_source[]`| `algo_source` | string | Which oscillators to use for the FM algorithm. list of six (starting with op 6), use empty for not used, e.g 0,1,2 or 0,1,2,,, |
| `p`    | `preset` | `preset` | int | Which predefined PCM or wavetable preset patch to use, or number of partials if < 0. For `wave=WAVETABLE`, use the wavetable presets appended to PCM. (Juno/DX7 patches are different - see `patch_number`). |
| `p`    | `preset` | `num_partials` | int | Alias for `preset`. Must be used with `wave=BYO_PARTIALS`. Cannot be combined with `preset` in the same message. |
| `P`    | `phase` | `phase` | float 0-1 | Where in the oscillator's cycle to begin the waveform (also works on the PCM buffer). default 0. For PCM oscs, a phase sent with (or before) a note-on sets the sample start point for that note-on (`start_frame / 2^23`) and is consumed by it; later note-ons without a phase start from 0 again. |
| `R`    | `resonance` | `resonance` | float | Q factor of variable filter, 0.5-16.0. default 0.7 |
| `T`    | `eg_type[0]` | `eg0_type` | uint 0-3 | Type for Envelope Generator 0 - 0: Normal (RC-like) / 1: Linear / 2: DX7-style / 3: True exponential. |
| `X`    | `eg_type[1]` | `eg1_type` | uint 0-3 | Type for Envelope Generator 1 - 0: Normal (RC-like) / 1: Linear / 2: DX7-style / 3: True exponential. |
| `y`    | `bus`      | `bus` | int   | Bus that this osc gets added onto (default 0) |
| `l`    | `velocity` | `vel` | float | Note on velocity. Use to start an envelope or set amplitude |

### CtrlCoefs

These per-oscillator parameters use [CtrlCoefs](synth.md) notation

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `Q`    | `pan_coefs[]` | `pan`   | float[,float...] | Panning index ControlCoefficients (for stereo output), 0.0=left, 1.0=right. default 0.5. |
| `a`    | `amp_coefs[]` | `amp`    | float[,float...]  | Control the amplitude of a note; a set of ControlCoefficients. Default is 1,0,1,1  (i.e. an overall gain of 1, scaled by the note velocity and Envelope Generator 0.) The first (`const`) coefficient is an overall gain: setting it to 0 mutes the oscillator entirely. Coefficients are combined in the log (dB) domain — see [synth.md](synth.md#control-coefficients). |
| `d`    | `duty_coefs[]` | `duty`   |  float[,float...] | Duty cycle for pulse wave, ControlCoefficients, defaults to 0.5 |
| `f`    | `freq_coefs[]` | `freq`   |  float[,float...]      | Frequency of oscillator, set of ControlCoefficients.  Default is 0,1,0,0,0,0,1 (from `note` pitch plus `pitch_bend`) |
| `F`    | `filter_freq_coefs[]` | `filter_freq` | float[,float...]  | Center/break frequency for variable filter, set of ControlCoefficients |

### PCM sampling

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `z`    | **TODO**| `load_sample` | uint x 6 | Signal to start loading sample. preset number, length(frames), samplerate, channels, midinote, loopstart, loopend. All subsequent messages are base64 encoded WAVE-style frames of audio until `length` is reached. Set `preset` and `length=0` to unload a sample from RAM. |
| `zF`   | **TODO**| `disk_sample` | uint,string,uint | Set a PCM preset to play live from a WAV filename on AMY host disk. Params: preset number, filename, midinote. See `hooks` for reading files on host disk. **Only one file sample can be played at once per preset number. Use multiple presets if you want polyphony from a single sample.** |
| `zS`   | **TODO**| `start_sample` | uint x 6 | Start sampling to a stereo PCM preset from source. Params: preset number, source, max length in frames, midinote, loopstart, loopend. source = 1 is AMY mixed output. source = 2 is AUDIO_IN0 + 1.  Will sample until max length is reached, `stop_sample` is issued, or a new `start_sample` is issued. | 
| `zO`   | **TODO**| `stop_sample` | uint | Stop sampling. Does nothing if no sampling active. param ignored. | 

### Sampler timing and time-stretch (experimental)

These hang off the `p` (preset) prefix rather than taking top-level codes of
their own: 44 of the 52 single letters are already allocated, and the sampler
corner is still growing. `p<n>` still selects PCM preset `n` as before; a
sub-letter after `p` addresses a PCM parameter instead.

| code | AMY parameter | Python/JS keyword | type | description |
| ---- | ------------- | ----------------- | ---- | ----------- |
| `po`   | `sample_offset` | `sample_offset` | uint 0 to BLOCK_SIZE-1 | PCM only. Start this note-on at a sample offset *within* the render block it fires in, leaving the head of the block silent. Events fire on block (256-sample) boundaries; `sample_offset` supplies the sub-block remainder, so slices of arbitrary length can be scheduled to butt-join sample-accurately (e.g. reconstructing a chopped break with no gaps). Sticky per osc like other params; set 0 to clear. |
| `pF`   | `fit_ticks` | `fit` | float | PCM only, in-memory presets. Engage the granular time/pitch engine at the next note-on. `fit=N` (N>0): play the sample in exactly N sequencer ticks with a pitch-invariant time stretch; `note` still transposes without changing duration. Because the target is in ticks, it tracks `tempo` *while the note is sounding*, not just at note-on: change the tempo mid-note and the stretch rate follows, so the note still ends N ticks after it started. `fit=0`: time-invariant pitch shift — `note` transposes but the sample keeps its original duration. `fit=-1`: turn the engine off. Non-destructive and real-time (~2.5x the render cost of plain PCM). |
| `pS`   | `fit_search` | `fit_search` | uint frames, 0 to 512 | PCM only, and only meaningful while the `fit` engine is running. Half-width, in input frames, of the WSOLA correlation search that aligns each new grain against the one still playing (see `fit` below). Default 64; `0` turns the search off entirely, leaving a fixed-grid overlap-add. Read every block, so it can be changed while a note sounds. Clamped to 512. |

Two parameters turn AMY's PCM oscillators into a "real" sampler (see
`experiments/sampler/` for worked examples):

- **`sample_offset` (`po`)** gives note-ons sub-block placement. AMY events
  execute on block boundaries (256 samples, ~5.8 ms); a PCM note-on with
  `sample_offset=k` starts at sample `k` of its block. Schedule a slice of
  length `L` starting at absolute sample `S`, then its successor at
  `S + L` (block `(S+L)//256`, offset `(S+L)%256`), and the two butt-join
  with no gap — a chopped break plays back bit-exact against the original.
  Untransposed playback (no `note`, no freq mods) is drift-free: AMY uses
  the preset's native rate exactly in that case.

- **`fit` (`pF`)** decouples duration from pitch, non-destructively, at
  note-on time. `fit=N` plays the sample over exactly N sequencer ticks
  (so it tracks `tempo`, including tempo changes that land part-way
  through a note -- the timeline rate is rescaled in the render loop, so
  a sounding note re-aims at the same tick rather than finishing at the
  tempo it started under) without changing pitch; `fit=0` changes pitch
  (via `note`) without changing duration. The engine is a fixed-point
  granular overlap-add (two 1024-sample Hann grains, 50% overlap) with a
  WSOLA-style correlation search aligning each new grain's phase against
  the one still playing — no FFT, no float in the render path, so it
  targets every AMY platform. Looping modes (`ww`) work: the input
  timeline wraps at the loop marks. Streamed `disk_sample` presets can't
  `fit` (no random access).

- **`fit_search` (`pS`)** sets how far that correlation search may roam. It
  has to be able to reach half a period of the lowest pitch in the sample or
  it can't find the aligned splice at all, and the default 64 frames only
  covers periods down to about `AMY_SAMPLE_RATE/128` — ~345 Hz at 44.1 kHz.
  Below that the aligner locks onto the nearest line of the comb spaced at the
  hop rate (~86 Hz at 44.1 kHz) instead of the note: a 110 Hz tone stretched 2x
  comes out at 141 Hz. `fit_search=256` puts it back at 110 Hz, and takes the
  ratio of energy at the tone to everything else from −15 dB to +26 dB (a
  440 Hz tone, already in reach of the default, goes from 20 dB to 38 dB —
  `experiments/sampler/fit_search_test.py` measures both). The cost
  is `(2 * fit_search + 1) * 64` multiplies per 512-sample hop — linear in the
  setting, so 256 is 4x the default's search cost — which is why it is opt-in
  rather than the default. Broadband material (drums, breaks) has no
  periodicity to align to and does not care; leave it alone there.

  The comparison window widens with the setting (it also has to span roughly a
  period), but the number of taps stays at 64 and the stride opens up instead,
  so the cost stays linear rather than quadratic. That decimation aliases —
  acceptable because wide settings exist for bass, where the taps still land
  many per period.

  `fit_search=0` disables alignment and gives you a plain fixed-grid
  overlap-add: every grain lands on the nominal hop no matter what the
  waveform is doing. That is roughly what the 90s Akai samplers' CYCLIC time
  stretch did, and it fails the same characteristic way — the splice error
  repeats at exactly the hop rate, so the artifact is a stable pitched comb
  rather than WSOLA's diffuse warble. On tonal material it is wrong by tens of
  dB; as an effect it is the "sampler timestretch" sound.

`fit` composes with `phase` to start a sample part-way through, which is what
a "drop the playhead into the middle of a loop" transport needs. `phase` sets
the note-on's start frame (`start_frame / 2^23`) and the stretcher picks the
input up from there — but it stretches *whatever input is left* over
*whatever `fit` it is given*, so `fit` has to be the REMAINING ticks, not the
clip's full length; pass the full value and the tail plays at half speed.
Entering a 16-beat loop at its midpoint is `phase = (frames/2) / 2^23` with
`fit` set to 8 beats of ticks, and it ends on the same grid line the
uninterrupted loop would have.

### WAVETABLE wave type

`wave=WAVETABLE` is available when AMY is built with `-DAMY_WAVETABLE`.

- Wavetable samples are baked into `pcm_tiny` and exposed as contiguous PCM presets.
- The preset range is dynamic at runtime:
  - start: `pcm_wavetable_base`
  - count: `pcm_wavetable_samples`
  - valid presets: `pcm_wavetable_base ... pcm_wavetable_base + pcm_wavetable_samples - 1`
- Each wavetable preset is expected to be one 64-cycle table (normally `16384` samples total, `256` samples per cycle).
- `duty` crossfades across the 64 cycles inside the selected wavetable preset.

### Drum kits (GAMMA9001 builds)

Devices built with `-DGAMMA9001` (Tulip, AMYboard, AMY on the web, the CPython `amy` module) carry the Gamma9001 drum sample banks: the full TR-808 bank is baked in as PCM presets 0-18, and 136 more samples (TR-909, Linn 9000, Univox MR-12, Tokyo Synthetics, 80s Power Kit, Percussion) live at presets 256-391, served from a platform-provided blob (linked into the binary on web and CPython; an mmapped `drums` flash partition on ESP32-S3). All of them play directly with `wave=PCM, preset=P`.

Patches **384-390** are ready-made General MIDI drum kits over these banks -- load one on a synth to make GM note numbers trigger the mapped samples:

| patch | MIDI PC (bank MSB 3) | kit |
|-------|----------------------|-----|
| 384 | 0 | TR-808 (baked; works without the sample banks mounted) |
| 385 | 1 | TR-909 |
| 386 | 2 | Linn 9000 |
| 387 | 3 | Univox Micro Rythmer 12 |
| 388 | 4 | Tokyo Synthetics |
| 389 | 5 | 80s Power Kit |
| 390 | 6 | Percussion |

Switch kits with `amy.send(synth=10, patch=38x)`, or over MIDI with bank select MSB 3 (CC0=3) + program change 0-6 on the drum channel; a synth already on a kit patch stays in the kit bank, so a bare program change also switches kits. Channel 10 defaults to the TR-808 kit when default synths are enabled. Non-GAMMA9001 builds keep the legacy 11-sample kit at patch 258. Patches 259-383 are reserved.

### Per-bus Effects

Each of the `y` buses has separate effects units.  You set their parameters with commands such as `amy.send(bus=0, reverb=1)` (or `y0h1`).

The final mixdown of the buses onto the AMY output is controlled per bus by the `volume` (`V`) command: `bus=N, volume=X` (`yNVX` on the wire) sets bus N's volume, and with no `bus` it sets bus 0.

Default AMY has 4 buses, 0..3.  Set `max_buses` in `amy_config_t` before `amy_start` to run as many as you have RAM for — there is no compile-time limit.  A bus number outside the configured range is rejected with a warning and treated as bus 0.  If the bus (`y`) is not specified for one of these commands, it defaults to 0.

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `h`    | `reverb_level, reverb_liveness, reverb_damping, reverb_xover_hz` | `reverb` | float[,float,float,float] | Reverb parameters -- level, liveness, damping, xover: Level is for output mix;
| `k`    | `chorus_level, chorus_max_delay, chorus_lfo_freq, chorus_depth` | `chorus` | float[,float,float,float] | Chorus parameters -- level, delay, freq, depth: Level is for output mix (0 to turn off); delay is max in samples (320); freq is LFO rate in Hz (0.5); depth is proportion of max delay (0.5). |
| `M`    | `echo_level, echo_delay_ms, echo_max_delay_ms, echo_feedback, echo_filter_coef` | `echo` | float[,int,int,float,float] | Echo parameters --  level, delay_ms, max_delay_ms, feedback, filter_coef (-1 is HPF, 0 is flat, +1 is LPF). |
| `x`    | `eq_l, eq_m, eq_h` |`eq` | float,float,float | Equalization in dB low (~800Hz) / med (~2500Hz) / high (~7500Hz) -15 to 15. 0 is off. default 0. |

### Other

| Wire code   | C `amy_event` | Python / JS   | Type-range  | Notes                                 |
| ------ | -------- | ---------- | ----------  | ------------------------------------- |
| `H`    | `ticks[3]` | `ticks` | int[,int[,tag]] | Tick, period, tag for sequencing (see "AMY's sequencer" in synth.md). `tag` omitted: stored but not individually cancelable. `period` also omitted: a one-off event at that tick. **If used in a wire string message**, the `H` **must** be the first character of the message. |
| `j`    | `tempo` | `tempo`  | float | The tempo (BPM, quarter notes) of the sequencer. Defaults to 108.0. |
| `zY`   | **TODO** | `sequencer_run` | 0/1 | Sequencer transport: `zY1` starts the sequencer, `zY0` stops it.  Lets a host drive playback without MIDI clock sync (see `external_midi_sync`). |
| `zC`   | **TODO** | `external_midi_sync` | 0/1/2 | MIDI clock sync: 1 = the sequencer follows incoming MIDI realtime clock/start/stop (0xF8/0xFA/0xFC); 2 = AMY is the clock master, sending those messages (0xF8 at 24 PPQ from the internal tempo, 0xFA/0xFC on transport start/stop); 0 (default) = internal clock, neither follows nor sends. |
| `N`    | `latency_ms`| `latency_ms` | uint | Sets latency in ms. default 0 (see LATENCY) |
| `s`    | `pitch_bend` | `pitch_bend` | float | Sets the global pitch bend, by default modifying all note frequencies by (fractional) octaves up or down |
| `V`    | `volume`| `volume` | float  | Volume knob for the addressed bus (`bus`/`y`, default 0) in the final mixdown, default 1.0 |
| `g`    | `client` | `client` | uint | Client number for Alles distributed synthesis. |
| `W`    | `external_channel` | `external_channel` | uint | External channel routing (used by Tulip for CV output). |
| `D`    | **TODO** | `debug`  |  uint, 2-4  | 2 shows queue sample, 3 shows oscillator data, 4 shows modified oscillator. Will interrupt audio! |
| `zT`   | **TODO**| `transfer_file` | string,uint | Transfer a file to the host. Params: destination filename, file size. See `hooks` for writing files on host disk. |
| `zD`   | **TODO**| `dump_sysex` | string (optional) | Dump data over MIDI sysex (base64-encoded, wrapped with SPSS manufacturer ID `00 03 45`). With no params (`zDZ`): dumps all active instrument state. With a filename (`zD/user/current/sketch.pyZ`): reads file and sends it. The filename is "rest of message": a trailing `Z` end-of-message marker is stripped, so interior capital-`Z` characters (e.g. `/user/ZFILE.py`) are preserved. Filenames whose last character is `Z` are not addressable. |
| `zP`   | **TODO**| `exec` | string | Execute code on the host via `amy_external_exec_hook`. On AMYboard, runs the string as Python (e.g. `zPimport amyboard; amyboard.restart_sketch()`). Max 255 chars. The code string is "rest of message": a trailing `Z` end-of-message marker is stripped, so interior capital-`Z` characters in the code are preserved. Code whose last character is `Z` is not expressible. |
| `zI`   | **TODO**| `ping` | (none) | Ping the host. Replies with a short sysex frame `F0 00 03 45 'O' 'K' F7` so the caller can confirm the board is alive and sysex is flowing. Handled entirely in pure C (no scheduler needed). |
| `zB`   | **TODO**| `reboot` | uint (optional) | Reboot the host via `amy_external_reboot_hook`. Optional `mode` argument selects the post-reboot state: `zBZ` / `zB0Z` = bootloader mode (skip sketch on next boot), `zB1Z` = normal reboot (run sketch), `zB2Z` = ROM download / flash mode. Handled in pure C (no scheduler needed). On AMYboard, sets an RTC flag with the mode and calls `esp_restart()`. |
