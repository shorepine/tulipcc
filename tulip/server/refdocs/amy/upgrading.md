# Upgrading between versions of AMY

Here we will post breaking APIs between releases of AMY and tips on porting.


## 1.0.X -> 1.1.X

This is a big change that moves a lot of stuff you used to have to do yourself into AMY itself -- voice and synth handling, note stealing, MIDI, I2S, sequencer.

V1.1.0 is not backwards compatible with V1.0.3 Arduino scripts.  We updated because we believe we have substantially improved the functionality under Arduino.  In particular, we incorporated MIDI and I2S within AMY for common MCU boards.  See the new [`AMY_MIDI_Synth.ino`](https://github.com/shorepine/amy/blob/main/examples/AMY_MIDI_Synth/AMY_MIDI_Synth.ino).

To migrate your old AMY scripts:

* There's no ``amy`` object.   Instead, the API consists of a bunch of functions with ``amy_`` (underscore) prefix (defined in [`api.c`](https://github.com/shorepine/amy/blob/main/src/api.c)).
* There's a new `amy_config` structure that you use to set startup options.  You initialize it as, e.g.:
```c
  amy_config_t amy_config = amy_default_config();
  amy_config.features.chorus = 0;  // etc, as desired.
  amy_config.features.default_synths = 0;
  // If you want MIDI over UART (5-pin or 3-pin serial MIDI)
  amy_config.midi = AMY_MIDI_IS_UART;
  // Pins for i2s board
  amy_config.i2s_bclk = 8;
  amy_config.i2s_lrc = 9;
  amy_config.i2s_dout = 10;
  ...
  amy_start(amy_config);  // instead of amy.begin()
  amy_live_start();  // performs I2S initialization
```
.. and (subject to several caveats and using supported hardware) MIDI input and I2S output will work.
* `amy_add_event()` now takes a pointer to an event instead of the structure itself:
```c
  amy_event e = amy_default_event();
  e.osc = 0;
  e.wave = SINE;
  e.freq_coefs[COEF_CONST] = 440;
  e.velocity = 1;
  amy_add_event(&e);  // instead of amy.add_event(e)
```
* The main loop is intended to be very simple:
```c
void loop() {
  amy_update(); 
}
```
But if you want to continue handling I2S and MIDI yourself, you can instead do something like:
```c
I2S i2s(OUTPUT);
void setup() {
  amy_config_t amy_config = amy_default_config();
  amy_config.features.default_synths = 0;
  amy_start(amy_config); 
  ...
  // Do your own i2s config
  i2s.begin(AMY_SAMPLE_RATE);
}
void loop() {
  int16_t * samples = amy_simple_fill_buffer();
  for(int i = 0; i < AMY_BLOCK_SIZE; i++) {
    // AMY always renders in stereo.
    i2s.write16(samples[2 * i], samples[2 * i + 1]);
  }
}
```

`patches_store_patch()` doesn't fit so well into our current API, and we're trying to make the use of wire string constants more of an exceptional case - you can do a similar thing by issuing a number of events with the `patch_number` set.  However, you can still use `patches_store_patch()`, it's just that the syntax has changed - rather than having the patch number as a prefix to the string, it's provided in an event structure.  So,
```c
    amy_event e = amy_default_event();
    e.patch_number = 1024;
    patches_store_patch(&e, "v0w7f0");  // Or whatever the wire string defining your patch is.
```


