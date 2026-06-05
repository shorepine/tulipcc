# Making "Billie Jean" with AMY_Arduino
dan.ellis@gmail.com 2025-11-03

AMY is a music synthesizer library that allows you to generate studio-quality music with your microcontroller.  It consists of a flexible synthesis engine capable of analog synthesizer emulation, FM-style sounds, additive synthesis, etc., as well as higher-level functions like MIDI input and polyphonic voice management.  AMY can be controlled via a Micropython interface (it’s an important part of the [Tulip Creative Computer](https://tulip.computer)), but you can also drive it directly from C/C++.  This tutorial shows you how to use AMY from Arduino to generate music audio. It’s inspired by the “Billie Jean” example that Floyd Steinberg [demo’d on the original Alles](https://youtu.be/8CmcsQXHVEo?si=VAA65qPmHA-Fq4k9).

## AMY Concepts

At the highest level, AMY provides a “synth” interface where you send note events and configuration commands to individual polyphonic synthesizers, which also automatically handle MIDI events if MIDI input is configured.

Each synth manages a set of a specific number of “voices”; A voice can generate one note event at a time.  If the synth is configured with one voice, it is monophonic, so each successive note event changes its pitch.  But you can configure each synth to have more voices so it can play chords polyphonically.  The synth layer handles the reallocation of the voices (“voice stealing”) if there are too many active notes for the number of voices.

A single voice consists of one or more “oscs” (oscillators).  Oscillators are the basic building block in AMY, and include a single waveform generator (oscillator with a specific waveform, PCM sample, or external audio input) along with modulation, filtering, etc.  Depending on your hardware and the complexity of processing, AMY can support more than 100 simultaneously-sounding oscillators, which might be configured as 12 analog synth voices, or six, 24-partial additive piano voices.

Each osc within a voice may have a lot of individual parameters, but all the voices managed by a single synth will be identically configured, and you send the osc configuration commands to the synth, which then updates the oscs in all its voices.

It’s possible to directly address individual oscs and voices using the AMY interface, but in most cases it’s easiest to control AMY through the synth layer.

## Playing a repeating pattern

[Arduino Setup for AMY](https://github.com/shorepine/amy/blob/main/docs/arduino.md) covers configuring a microcontroller board to run AMY under the Arduino IDE.

Once you have that ready, you should see three sketches with names beginning `BillieJean` in the Arduino `Examples > AMY Synthesizer` submenu.  Open [BillieJeanDrums](https://github.com/shorepine/amy//blob/main/examples/BillieJeanDrums/BillieJeanDrums.ino), our first example sketch.  This sketch uses AMY to play a simple repeating drum pattern using AMY’s built-in PCM drum samples.

The `setup()` function configures and starts the AMY engine.  We select the `default_synths`, which includes emulation of a MIDI drum machine on synth 10:

```C
// BillieJeanDrums - Making a simple drum pattern with AMY.
#include <AMY-Arduino.h>

void setup() {
  amy_config_t amy_config = amy_default_config();
  amy_config.features.startup_bleep = 1;
  // Install the default_synths on synths (MIDI chans) 1, 2, and 10.
  amy_config.features.default_synths = 1;

  amy_config.audio = AMY_AUDIO_IS_I2S;
  // Pins for i2s board
  amy_config.i2s_bclk = 8;
  amy_config.i2s_lrc = 9;
  amy_config.i2s_dout = 10;

  amy_start(amy_config);
}
```
Now AMY is ready to play notes in response to note-on events:
```C
    // Time to play the note.
    amy_event e = amy_default_event();
    e.synth = 10;  // drums channel
    e.midi_note = midi_note;
    e.velocity = velocity;
    amy_add_event(&e);
```
The `midi_note` is usually a number that sets the pitch of a note, using the MIDI convention where 60 is middle C, 61 is C#, etc.  For the drums channel, though, each note number corresponds to a different drum sound, so 35 is the bass drum, 42 is the closed hi-hat, etc.  The `velocity` is how “hard” the note is played, i.e. it determines how loud it is.  `amy_add_event()` is the main way to send commands (both note events and configuration) to AMY.

To play a drum pattern, we need to send a series of events to AMY to command each note event at the appropriate time.  We’ll need a table of data defining our desired pattern:

```C
struct timed_note {
  float start_time;  // In ticks
  int note;
  float velocity;
};

// 35 is kick, 37 is snare, 42 is closed hat, 46 is open hat
// Notes must be sorted in start_time order.
timed_note notes[] = {
  { 0.0, 42, 1.0},  //  0 HH + BD
  { 0.0, 35, 1.0},
  { 1.0, 42, 1.0},  //  1 HH
  { 2.0, 42, 1.0},  //  2 HH + SN
  { 2.0, 37, 1.0},
  { 3.0, 42, 1.0},  //  3 HH
  { 4.0, 42, 1.0},  //  4 HH + BD
  { 4.0, 35, 1.0},
  { 5.0, 42, 1.0},  //  5 HH
  { 6.0, 42, 1.0},  //  6 HH + SN
  { 6.0, 37, 1.0},
  { 7.0, 46, 1.0},  //  7 OH
};
// Time (in ticks) at which we reset to the start of the table.
float cycle_len = 8.0;
```

We define a `timed_note` data structure to hold the time, the note number, and the velocity for each event, then we set up a table with a simple drum pattern using bass drum (kick), snare drum, and closed and open hi-hat.  The times of each event are specified in “ticks”, corresponding to a quarter-note in this 2 bar pattern.  We also specify that the pattern repeats after 8 ticks in `cycle_len`.  Note that there are multiple events at some times (0, 2, 4, 6 ticks).  Also notice that the table is sorted by `start_time` - this means we can step through it note by note without having to check for out-of-sequence notes.

Now we can set up the loop to keep “tick” time and step through the note table, sending the note events to AMY at the appropriate times:

```C
float millis_per_tick = 250;
float base_tick = 0;  // Time of beginning of current cycle.

int note_tab_index = 0;
int note_tab_len = sizeof(notes) / sizeof(timed_note);

void loop() {
  // Your loop() must contain this call to amy:
  amy_update();

  // Calculate "tick time" and choose note.
  float tick_in_cycle = millis() / millis_per_tick - base_tick;
  if (tick_in_cycle >= cycle_len) {
    // Start the next cycle - reset the cycle base_tick, reset the note_tab index.
    tick_in_cycle -= cycle_len;
    base_tick += cycle_len;
    note_tab_index = 0;
  }
```

`millis_per_tick` sets the tempo of our pattern, setting the duration of each tick.  Here, we’ve defined a tick to be 250 milliseconds (a quarter of a second), for a 120 bpm feel.  In the `loop()` function, we calculate `tick_in_cycle` - the current time relative to the start of the current cycle - by scaling Arduino’s `millis()` counter by `millis_per_tick`, then subtracting `base_tick`, the absolute time, in ticks, of the start of the currently-playing cycle.  If `tick_in_cycle` is equal or larger than the `cycle_len`, we reset our index into the notes table, `note_tab_index`, to the beginning of the table, and move on the `base_tick` by one complete `cycle_len`.

Finally, we send any note events from the table whose time has come:

```C
  // Play any notes for this moment from the note table.
  while(note_tab_index < note_tab_len 
        && tick_in_cycle >= notes[note_tab_index].start_time) {
    // Grab the note parameters
    int midi_note = notes[note_tab_index].note;
    float velocity = notes[note_tab_index].velocity;
    // Time to play the note.
    amy_event e = amy_default_event();
    e.synth = 10;  // drums channel
    e.midi_note = midi_note;
    e.velocity = velocity;
    amy_add_event(&e);
    note_tab_index++;
  }
```

As long as our `note_tab_index` hasn’t run off the end of the table (as held in `note_tab_len`), and if the current `tick_in_cycle` is equal to or just past the `start_time` of that event from the table, we send an event with the corresponding `midi_note` and `velocity` to `amy_add_event()`, and move on to look at the next entry in the table by incrementing `note_tab_index`.

That’s it!  Running this sketch should immediately start a short drum pattern which will repeat indefinitely.

## Adding another instrument

Drums are great, but we need more.  Open the sketch [BillieJeanDrumsBass](https://github.com/shorepine/amy/blob/main/examples/BillieJeanDrumsBass/BillieJeanDrumsBass.ino) to see how we add the bass line.

Firstly, we extend our setup to define `synth` 2 as a monophonic synth configured with one of the built-in Juno bass patches:

```C
  // Set up synth 2 as monophonic bass
  amy_event e = amy_default_event();
  e.synth = 2;
  e.patch_number = 30;  // Juno A47 Funky I
  e.num_voices = 1;
  amy_add_event(&e);
```

We’re now going to have multiple instruments in our table, so we add a `channel` field to our `timed_note` structure, and add the bass notes to the table:

```C
struct timed_note {
  float start_time;  // In ticks
  int channel;       // 10 = drums, 2 = bass
  int note;
  float velocity;
};

// Notes must be sorted in start_time order.
timed_note notes[] = {
  { 0.0, 2, 43, 0.2},   // bass G2
  { 0.0, 10, 42, 1.0},  //  0 HH + BD
  { 0.0, 10, 35, 1.0},
  { 1.0, 2, 38, 0.2},   // bass D2
  { 1.0, 10, 42, 1.0},  //  1 HH
  { 2.0, 2, 41, 0.2},   // bass F2
  { 2.0, 10, 42, 1.0},  //  2 HH + SN
  { 2.0, 10, 37, 1.0},
  { 3.0, 2, 43, 0.2},   // bass G2
  { 3.0, 10, 42, 1.0},  //  3 HH
  { 4.0, 2, 41, 0.2},   // bass F2
  { 4.0, 10, 42, 1.0},  //  4 HH + BD
  { 4.0, 10, 35, 1.0},
  { 5.0, 2, 38, 0.2},   // bass D2
  { 5.0, 10, 42, 1.0},  //  5 HH
  { 6.0, 2, 36, 0.2},   // bass C2
  { 6.0, 10, 42, 1.0},  //  6 HH + SN
  { 6.0, 10, 37, 1.0},
  { 7.0, 2, 38, 0.2},   // bass D2
  { 7.0, 10, 46, 1.0},  //  7 OH
};
// Time (in ticks) at which we reset to the start of the table.
float cycle_len = 8.0;
```

Now we simply modify the code that sends the AMY events to also read the channel (i.e., the synth to which the note is directed) from the table:

```C
  // Play any notes for this moment from the note table.
  while(note_tab_index < note_tab_len 
        && tick_in_cycle >= notes[note_tab_index].start_time) {
    // Time to play the note.
    amy_event e = amy_default_event();
    e.synth = notes[note_tab_index].channel;
    e.midi_note = notes[note_tab_index].note;
    e.velocity = notes[note_tab_index].velocity;
    amy_add_event(&e);
    note_tab_index++;
  }
```

That’s it.  Now we have both bass and drums.

## Using the AMY scheduler

So far, we’ve been using AMY in “real time” - we time the note events by sending the `amy_add_event()` command just when we want the note to occur.  This is not always convenient or precise, so AMY includes a scheduler so you can specify exactly *when* your new event is supposed to occur.  Using the scheduler, we can send a bunch of note events all at once, including their timing, then sit back and wait as AMY works through the timeline - we don’t have to worry about implementing the timing in our `loop()` function.

The third version, [BillieJeanScheduled](https://github.com/shorepine/amy/blob/main/examples/BillieJeanScheduled/BillieJeanScheduled.ino), uses AMY scheduling.  Because we’re adding chords, we need to configure an additional synth.  And just to make it sound a little more like the original, we also turn on AMY’s built-in reverb:

```C
  // Reconfigure synth 1 as a 6-note polyphonic synth (for chords)
  amy_event e = amy_default_event();
  e.synth = 1;
  e.patch_number = 5;  // Juno A16 Brass & Strings
  e.num_voices = 6;
  amy_add_event(&e);

  // Reconfigure synth 2 as monophonic bass
  e = amy_default_event();
  e.synth = 2;
  e.patch_number = 30;  // Juno A47 Funky I
  e.num_voices = 1;
  amy_add_event(&e);

  // Turn on reverb
  config_reverb(0.5f, 0.85f, 0.5f, 3000.0f);
```

This version tells AMY exactly what time (in milliseconds) it wants each note to occur.  Because of this, we can send all the notes for an entire cycle at once (we could probably send an entire track at once, but for convenience we do it one cycle at a time), and we don’t have to worry about issuing them in order, so we can have different tables for the different instruments.  This means we don’t store the channel (synth) number in the table, but it also means we can specify the duration for the non-drum notes because we can also schedule note-offs for each note we send:

```C
struct timed_note {
  float start_time;  // In ticks
  float duration;    // In ticks
  int note;
  float velocity;
};

// Cycle length (in ticks) for drums + bass
float cycle_len = 8.0;

// Drum notes have durations of 0 for no note-off
timed_note drum_notes[] = {
  { 0.0, 0.0, 42, 1.0},  //  0 HH + BD
  { 0.0, 0.0, 35, 1.0},
  { 1.0, 0.0, 42, 1.0},  //  1 HH
  { 2.0, 0.0, 42, 1.0},  //  2 HH + SN
  { 2.0, 0.0, 37, 1.0},
  { 3.0, 0.0, 42, 1.0},  //  3 HH
  { 4.0, 0.0, 42, 1.0},  //  4 HH + BD
  { 4.0, 0.0, 35, 1.0},
  { 5.0, 0.0, 42, 1.0},  //  5 HH
  { 6.0, 0.0, 42, 1.0},  //  6 HH + SN
  { 6.0, 0.0, 37, 1.0},
  { 7.0, 0.0, 46, 1.0},  //  7 OH
};

timed_note bass_notes[] = {
  { 0.0, 0.6, 43, 0.2},   // bass G2
  { 1.0, 0.6, 38, 0.2},   // bass D2
  { 2.0, 0.6, 41, 0.2},   // bass F2
  { 3.0, 0.6, 43, 0.2},   // bass G2
  { 4.0, 0.6, 41, 0.2},   // bass F2
  { 5.0, 0.6, 38, 0.2},   // bass D2
  { 6.0, 0.6, 36, 0.2},   // bass C2
  { 7.0, 0.6, 38, 0.2},   // bass D2
};

timed_note chord_notes[] = {
  { 0.0, 0.2, 70, 1.0},  // Fmin:1
  { 0.0, 0.2, 74, 1.0},
  { 0.0, 0.2, 79, 1.0},
  { 3.0, 0.2, 72, 1.0},  // Amin:1
  { 3.0, 0.2, 76, 1.0},
  { 3.0, 0.2, 81, 1.0},
  { 8.0, 0.2, 74, 1.0},  // Bb:1
  { 8.0, 0.2, 77, 1.0},
  { 8.0, 0.2, 82, 1.0},
  { 11.0, 0.2, 72, 1.0},  // Amin:1
  { 11.0, 0.2, 76, 1.0},
  { 11.0, 0.2, 81, 1.0},
};
```

We have a new function that takes an entire table of `timed_notes` along with a start time offset and a channel (synth), and schedules them all, including note-offs if the table includes nonzero note durations.  Note we reuse some fields in the `amy_event` structure.

```C
void schedule_notes(int time, int channel, struct timed_note *notes, int num_notes) {
  amy_event e = amy_default_event();
  e.synth = channel;
  for (int i = 0; i < num_notes; ++i) {
    e.midi_note = notes[i].note;
    e.velocity = notes[i].velocity;
    e.time = time + millis_per_tick * notes[i].start_time;
    amy_add_event(&e);
    // Add note-off too if duration > 0
    if (notes[i].duration > 0) {
      e.time += millis_per_tick * notes[i].duration;
      e.velocity = 0;
      amy_add_event(&e);
    }
  }
}
```

Now the main `loop()` simply spins (while calling `amy_update()`) until the timing indicates that we’re beginning a new cycle of the music, then issues all the notes associated with that cycle as it begins.  We take advantage of this to gradually build the music, starting with the drums, then bringing in the bass, then finally the chords:

```C
int start_millis = 3000;
int last_cycle = -1;

void loop() {
  // Let amy do its processing for this moment.
  amy_update();

  int now = millis();
  int current_cycle = floor((now - start_millis) / (millis_per_tick * cycle_len));
  if (current_cycle > last_cycle) {
    // A new cycle began, issue notes.
    // Drums
    schedule_notes(now, 10, drum_notes, sizeof(drum_notes) / sizeof(timed_note));
    // Bass comes in after two cycles of drums
    if (current_cycle >= 2)
      schedule_notes(now, 2, bass_notes, sizeof(bass_notes) / sizeof(timed_note));
    // Chord sequence is 2 cycles long, so only schedule every other cycle
    if ((current_cycle >= 4) && ((current_cycle % 2) == 0))
      schedule_notes(now, 1, chord_notes, sizeof(chord_notes) / sizeof(timed_note));
    last_cycle = current_cycle;
  }
}
```
And that’s really it!  We now have a program that generates a serviceable reproduction of the opening of Billie Jean using just AMY for all the instruments.
