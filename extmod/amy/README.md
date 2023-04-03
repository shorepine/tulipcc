# AMY - the Additive Music synthesizer librarY

AMY is a fast, small and accurate audio synthesizer C library with Python bindings that deals with combinations of many oscillators very well. It can easily be embedded into almost any program, architecture or microcontroller with an FPU and around 100KB of RAM. We've built AMY on Mac, Linux, the microcontrollers ESP32 and ESP32S3, and more to come. 

AMY powers the multi-speaker mesh synthesizer [Alles](https://github.com/bwhitman/alles), as well as the forthcoming Tulip Creative Computer. Let us know if you use AMY for your own projects and we'll add it here!

AMY was built by [DAn Ellis](https://research.google/people/DanEllis/) and [Brian Whitman](https://notes.variogram.com), and would love your contributions.

It supports

 * An arbitrary number (compile-time option) of band-limited oscillators, each with adjustable frequency and amplitude:
   * pulse (+ adjustable duty cycle)
   * sine
   * saw (up and down)
   * triangle
   * noise
   * PCM, reading from a baked-in buffer of percussive and misc samples
   * karplus-strong string with adjustable feedback 
   * An operator / algorithm-based frequency modulation (FM) synth
 * Biquad low-pass, bandpass or hi-pass filters with cutoff and resonance, can be assigned to any oscillator
 * An additive partial synthesizer with an analysis front end to play back long strings of breakpoint-based sine waves, including amplitude modulated noise
 * Oscillators can be specified by frequency in floating point or midi note 
 * Each oscillator has 3 breakpoint generators, which can modify any combination of amplitude, frequency, duty, filter cutoff, feedback or resonance over time
 * Each oscillator can also act as an modulator to modify any combination of parameters of another oscillator, for example, a bass drum can be indicated via a half phase sine wave at 0.25Hz modulating the frequency of another sine wave. 
 * Control of overall gain and 3-band parametric EQ
 * Built in patches for PCM, FM and partials
 * Built-in clock for short term sequencing of events
 * Can use multi-core (including on ESP32 microcontrollers) for rendering if available

The FM synthesizer in AMY is especially well-loved and as close to a real DX7 as you can get in floating point. We provide a Python library, `fm.py` that can convert any DX7 patch into AMY setup commands, and also a pure-Python implementation of the AMY FM synthesizer in `dx7_simulator.py`.

The partial tone synthesizer also provides `partials.py`, where you can model the partials of any arbitrary audio into AMY setup commands for live partial playback of hundreds of oscillators.

## Using AMY in your software

To use AMY in your own software, simply copy the .c and .h files in `src` to your program and compile them, or run `setup.py install` to be able to `import amy` in Python to generate audio signals directly in Python. No other libraries should be required to synthesize audio in AMY. 

To run a simple C example, make sure `libsoundio` (`brew install libsoundio` on macOS or `apt-get install libsoundio-dev` on Linux) is installed (to access audio output devices) and run `make`. Then run `./amy-example`. 

## Controlling AMY

AMY can be controlled using its wire protocol or by fillng its data structures directly. It's up to what's easier for you and your application. 

In Python, rendering to a buffer of samples, using the high level API:

```python
>>> import amy
>>> m = amy.message(osc=0,wave=amy.ALGO,patch=30,note=50,vel=1)
>>> print(m) # Show the wire protocol message
't76555951v0w8n50p30l1Z'
>>> amy.send_raw(m)
>>> audio = amy.render(5.0)
```

You can also start a thread playing live audio (requires libsoundio):

```python
>>> import amy
>>> amy.live()
>>> amy.send(osc=0,wave=amy.ALGO,patch=30,note=50,vel=1)
>>> amy.stop()
```


In C, using the high level structures directly:

```c
#include "amy.h"
void bleep() {
    struct event e = amy_default_event();
    int64_t sysclock = amy_sysclock();
    e.time = sysclock;
    e.wave = SINE;
    e.freq = 220;
    e.velocity = 1;
    amy_add_event(e);
    e.time = sysclock + 150;
    e.freq = 440;
    amy_add_event(e);
    e.time = sysclock + 300;
    e.velocity = 0;
    e.amp = 0;
    e.freq = 0;
    amy_add_event(e);
}

void main() {
    amy_start(); // initializes amy 
    amy_live_start(); // uses libsoundio to render live audio
    bleep();
}
```

Or in C, sending the wire protocol directly:

```c
#include "amy.h"

void main() {
    amy_start();
    amy_live_start();
    amy_play_message("t76555951v0w8n50p30l1Z");
}
```

AMY's wire protocol is a series of numbers delimited by ascii characters that define all possible parameters of an oscillator. This is a design decision intended to make using AMY from any sort of environment as easy as possible, with no data structure or parsing overhead on the client. It's also readable and compact, far more expressive than MIDI and can be sent over network links, UARTs, or as arguments to functions or commands. We've used AMY over multicast UDP, over javascript, in MAX/MSP, in Python, C, Micropython and many more! 

AMY accepts commands in ASCII, like so:

```
v0w4f440.0l0.9
```

Here's the full list:

| Code | Python |  Type-range      | Notes                                 |
| ---- | ------ |  ---------- | ------------------------------------- |
| a    | amp    |  float 0-1+ | use after a note on is triggered with velocity to adjust amplitude without re-triggering the note |
| A    | bp0    | string     | in commas, like 100,0.5,150,0.25,200,0 -- envelope generator with alternating time(ms) and ratio. last pair triggers on note off |
| B    | bp1    |  string     | set the second breakpoint generator. see breakpoint 0 |
| b    | feedback | float 0-1  | use for the ALGO synthesis type in FM, or partial synthesis (for bandwidth) or for karplus-strong, or to indicate PCM looping (0 off, >0, on) |
| C    | bp2    |  string     | 3rd breakpoint generator |
| d    | duty   |  float 0.001-0.999 | duty cycle for pulse wave, default 0.5 |
| D    | debug  |  uint, 2-4  | 2 shows queue sample, 3 shows oscillator data, 4 shows modified oscillator. will interrupt audio! |
| f    | freq   |  float      | frequency of oscillator |
| F    | filter_freq | float  | center frequency for biquad filter |
| g    | mod_target | uint mask | Which parameter modulation/LFO controls. 1=amp, 2=duty, 4=freq, 8=filter freq, 16=resonance, 32=feedback. Can handle any combo, add them together |
| G    | filter_type | 0-3 |  0 = none (default.) 1 = low pass, 2 = band pass, 3 = hi pass. |
| H    | reverb_liveness | float 0-1 | Reverb decay time, 1 = longest, default = 0.85. |
| h    | reverb_level | float | Level at which reverb is mixed in to final output.  Default 0, typically 1. |
| I    | ratio  | float | for ALGO types, where the base note frequency controls the modulators, or for the ALGO base note and PARTIALS base note, where the ratio controls the speed of the playback |
| j    | reverb_damping  | float 0-1 | Reverb extra decay of high frequencies, default = 0.5. |
| J    | reverb_xover_hz | float  | Crossover frequency (in Hz) for damping decay, default = 3000. |
| k    | chorus_level | float 0-1 | Gain applied to chorus when mixing into output.  Set to 0 to turn off chorus. |
| L    | mod_source | 0 to OSCS-1 | Which oscillator is used as an modulation/LFO source for this oscillator. Source oscillator will be silent. |
| l    | vel | float 0-1+ | velocity - >0 to trigger note on, 0 to trigger note off. sets amplitude |
| m    | chorus_delay | uint 1-512 | Maximum delay in chorus delay lines, in samples. Default 320. |
| N    | latency_ms | uint | sets latency in ms. default 0 | 
| n    | note | uint 0-127 | midi note, sets frequency | 
| o    | algorithm | uint 1-32 | DX7 algorith to use for ALGO type | 
| O    | algo_source | string | which oscillators to use for the algorithm. list of six, use -1 for not used, e.g 0,1,2,-1,-1-1 |
| p    | patch | uint | choose a preloaded PCM sample, partial patch or FM patch number for ALGO waveforms. |
| P    | phase | float 0-1 | where in the oscillator's cycle to start sampling from (also works on the PCM buffer). default 0 |
| Q    | pan   | float 0-1 | panning index (for stereo output), 0=right, 1=left. default both. |
| R    | resonance | float | q factor of biquad filter. in practice, 0-10.0. default 0.7 | 
| S    | reset  | uint | resets given oscillator. set to > OSCS to reset all oscillators, gain and EQ |  
| T    | bp0_target | uint mask | Which parameter bp0 controls. 1=amp, 2=duty, 4=freq, 8=filter freq, 16=resonance, 32=feedback (can be added together). Can add 64 for linear ramp, otherwise exponential | 
| t    | timestamp | uint | ms of expected playback since some fixed start point on your host. you should always give this if you can. |
| v    | osc | uint 0 to OSCS-1 | which oscillator to control | 
| V    | volume | float 0-10 | volume knob for entire synth, default 1.0 | 
| w    | wave | uint 0-11 | waveform: [0=SINE, PULSE, SAW_DOWN, SAW_UP, TRIANGLE, NOISE, KS, PCM, ALGO, PARTIAL, PARTIALS, OFF]. default: 0/SINE |
| W    | bp1_target | uint mask | see bp0_target |
| x    | eq_l | float | in dB, fc=800Hz amount, -15 to 15. 0 is off. default 0. |
| X    | bp2_target | uint mask | see bp0_target |
| y    | eq_m | float |  in dB, fc=2500Hz amount, -15 to 15. 0 is off. default 0. |
| z    | eq_h | float | in dB, fc=7500Hz amount, -15 to 15. 0 is off. default 0. | 



# Synthesizer details

We'll use Python for showing examples of AMY, make sure you've installed `libamy` and are running a live AMY first by 

```bash
$ cd src
$ python setup.py install
$ cd ..
$ python
>>> import amy
>>> amy.live()
```

## AMY and timestamps

AMY is meant to receive messages in real time. It, on its own, is not a sequencer where you can schedule notes to play in the future. However, it does maintain a window of (configurable) 20 seconds in advance of its clock where events can be scheduled. This is very helpful in cases where you can't rely on an accurate clock from the client, or don't have one. The clock used internally by AMY is based on the audio samples being generated out the speakers, which should run at an accurate 44,100 times a second.  This lets you do things like schedule fast moving parameter changes over short windows of time. 

For example, to play two notes, one a second after the first, you could do:

```python
amy.send(osc=0,note=50,vel=1)
time.sleep(1)
amy.send(osc=0,note=52,vel=1)
```

But you'd be at the mercy of Python's internal timing, or your OS. A more precise way is to send the messages at the same time, but to indicate the intended time of the playback:

```python
start = amy.millis() # arbitrary start timestamp
amy.send(osc=0,note=50,vel=1,timestamp=start)
amy.send(osc=0,note=52,vel=1,timestamp=start+1000)
```

Both `amy.send()`s will return immediately, but you'll hear the second note play precisely a second after the first. AMY uses this internal clock to schedule step changes in breakpoints as well. 


## Examples

`amy.drums()` should play a test pattern.

Try to set the volume of the synth with `amy.volume(2)` -- that can be up to 10 or so.  The default is 1. 

`amy.reset()` resets all oscillators to default. You can also do `amy.reset(osc=5)` to do just one oscillator.

Let's set a simple sine wave first

```python
amy.send(osc=0, wave=amy.SINE, freq=220, amp=1)
```

What we're doing here should be pretty straightforward. I'm telling oscillator 0 to be a sine wave at 220Hz and amplitude 1. You can also try `amy.PULSE`, or `amy.SAW_DOWN`, etc. 

**Why can't you hear anything yet?** It's because you haven't triggered the note on for this oscillator. We accept a parameter called `vel` (velocity) that can turn a note on or off (`vel=0`.) So now that we've set up the oscillator, we just turn it on by `amy.send(osc=0, vel=1)`. Note the oscillator remembers all its state and setup. To turn off the note, just do `amy.send(osc=0, vel=0)`. 

You can also make oscillators louder with `amp` or `vel` over 1. 

You can also always use `note`, (MIDI note value) instead of `freq`.

```python
amy.send(osc=0, wave=amy.SINE, note=57, vel=1)
```

Now let's make a lot of sine waves! 

```python
import time
amy.reset()
for i in range(16):
    amy.send(osc=i, wave=amy.SINE, freq=110+(i*80), vel=((16-i)/32.0))
    time.sleep(0.5) # Sleep for 0.5 seconds
```

Neat! You can see how simple / powerful it is to have control over lots of oscillators. You have up to 64. Let's make it more interesting. A classic analog tone is the filtered saw wave. Let's make one.

```python
amy.send(osc=0,wave=amy.SAW_DOWN,filter_freq=2500, resonance=5, filter_type=amy.FILTER_LPF)
amy.send(osc=0, vel=1, note=40)
```

Sounds nice. But we want that filter freq to go down over time, to make that classic filter sweep tone. Let's use a breakpoint! A breakpoint is a simple list of (time, value) - you can have up to 8 of those pairs, and up to 3 different sets to control different things. They're just like ADSRs, but more powerful. You can control amplitude, frequency, duty cycle, feedback, filter frequence, or resonance with a breakpoint. It gets triggered when the note does. So let's make a breakpoint that turns the filter frequency down from its start at 2500 to 1250 after 100 milliseconds. And when the note goes off, taper the frequency to 0 after 25 millseconds. 

```python
amy.send(osc=0,wave=amy.SAW_DOWN,filter_freq=2500, resonance=5, filter_type=amy.FILTER_LPF)
amy.send(osc=0, bp0="100,0.5,25,0", bp0_target=amy.TARGET_FILTER_FREQ)
amy.send(osc=0, vel=1, note=40)
```

Great. You can add multiple targets together, for example, if you want a breakpoint to control both filter frequency and resonance, use `bp0_target=amy.TARGET_FILTER_FREQ+amy.TARGET_RESONANCE`. Give it a go!

We also have LFOs, which are implemented as one oscillator modulating another. You set the lower-frequency oscillator up, then have it control a parameter of another audible oscillator. Let's make the classic 8-bit duty cycle pulse wave modulation, a favorite: 

```python
amy.send(osc=1, wave=amy.SAW_DOWN, freq=0.5, amp=0.75)
amy.send(osc=0, wave=amy.PULSE, duty=0.5, freq=220, mod_source=1, mod_target=amy.TARGET_DUTY)
amy.send(osc=0, vel=0.5)
```

You see we first set up the modulation oscillator (a saw wave at 0.5Hz, with amplitude 0.75-- this indicates the "depth" of the LFO). Then we set up the oscillator to be modulated, a pulse wave with mod source of oscillator 1 and mod target of duty cycle. The initial duty cycle will start at 0.5 and be multiplied by the state of oscillator 1 every tick, to make that classic thick saw line from the C64 et al. The modulation will re-trigger every note on. Just like breakpoints, you can modulate duty cycle, amplitude, frequency, filter frequency, resonance or feedback! And if you want to modulate more than one thing, like frequency and duty, just add them together:

```python
amy.send(osc=1, wave=amy.TRIANGLE, freq=5, amp=0.25)
amy.send(osc=0, wave=amy.PULSE, duty=0.5, freq=110, mod_source=1, mod_target=amy.TARGET_DUTY+amy.TARGET_FREQ)
amy.send(osc=0, vel=0.5)
```

`amy.py` has some helpful presets, if you want to use them, or add to them. To make that filter bass, just do `amy.preset(1, osc=0)` and then `amy.send(osc=0, vel=1, note=40)` to hear it. Here's another one:

```python
amy.preset(0, osc=2) # will set a simple sine wave tone on oscillator 2
amy.send(osc=2, note=50, vel=1.5) # will play the note at velocity 1.5
amy.send(osc=2, vel=0) # will send a "note off" -- you'll hear the note release
amy.send(osc=2, freq=220.5, vel=1.5) # same but specifying the frequency
amy.reset()
```

## Core oscillators

We support bandlimited saw, pulse/square and triangle waves, alongside sine and noise. Use the wave parameter: 0=SINE, PULSE, SAW_DOWN, SAW_UP, TRIANGLE, NOISE. Each oscillator can have a frequency (or set by midi note), amplitude and phase (set in 0-1.). You can also set `duty` for the pulse type. We also have a karplus-strong type (KS=6). 

Oscillators will not become audible until a `velocity` over 0 is set for the oscillator. This is a "note on" and will trigger any modulators or breakpoints / ADSRs set for that oscillator. Setting `velocity` to 0 sets a note off, which will stop modulators and also finish the breakpoint at its release pair. `velocity` also internally sets `amplitude`, but you can manually set `amplitude` after `velocity` starts a note on.

## LFOs & modulators

Any oscillator can modulate any other oscillator. For example, a LFO can be specified by setting oscillator 0 to 0.25Hz sine, with oscillator 1 being a 440Hz sine. Using `mod_target`, you can have oscillator 0 modulate frequency, amplitude, filter frequency, resonance, duty or feedback of oscillator 1. You can also add targets together, for example amplitude+frequency. Set the `mod_target` and `mod_source` on the audible oscillator (in this case, oscillator 1.) The source mod oscillator will not be audible once it is referred to as a `mod_source` by another oscillator. The amplitude of the modulating oscillator indicates how strong the modulation is (aka "LFO depth.")

## Filters

We support lowpass, bandpass and hipass filters in AMY. You can set `resonance` and `filter_freq` per oscillator. 

## EQ & Volume

You can set a synth-wide volume (in practice, 0-10), or set the EQ of the entire synths's output. 

## Breakpoints

AMY allows you to set 3 "breakpoint generators" per oscillator. You can see these as ADSR / envelopes (and they can perform the same task), but they are slightly more capable. Breakpoints are defined as pairs (up to 8 per breakpoint) of time (specified in milliseconds) and ratio. You can specify any amount of pairs, but the last pair you specify will always be seen as the "release" pair, which doesn't trigger until note off. All other pairs previously have time in the aggregate from note on, e.g. 10ms, then 100ms is 90ms later, then 250ms is 150ms after the last one. The last "release" pair counts from ms from the note-off. 

A breakpoint can target amplitude, duty, frequency, filter frequency, resonance or feedback of an oscillator.

For example, to define a common ADSR curve where a sound sweeps up in volume from note on over 50ms, then has a 100ms decay stage to 50% of the volume, then is held until note off at which point it takes 250ms to trail off to 0, you'd set time to be 50ms at ratio to be 1.0, then 150ms with ratio .5, then a 250ms release with ratio 0. You then set the target of this breakpoint to be amplitude. At every synthesizer tick, the given amplitude (default of 1.0) will be multiplied by the breakpoint modifier. In AMY wire parlance, this would look like "`v0f220w0A50,1.0,150,0.5,250,0T1`" to specify a sine wave at 220Hz with this envelope. 

When using `amy.py`, use the string form of the breakpoint: `bp0="50,1.0,150,0.5,250,0"`. 

Every note on (specified by setting velocity / `l` to anything > 0) will trigger this envelope, and setting `l` to 0 will trigger the note off / release section. 

Adding 64 to the target mask `T` will set the breakpoints to compute in linear, while the default is an exponential curve. (There are 2 more breakpoint curve types defined, for use in the DX7 simulation.)

You can set a completely separate breakpoints using the second and third breakpoint operator and target mask, for example, to change pitch and amplitude at different rates.


## FM & ALGO type

Try default DX7 patches:

```python
amy.send(wave=amy.ALGO,osc=0,patch=0,note=50,vel=1)
amy.send(wave=amy.ALGO,osc=0,patch=1,note=50,vel=1)
```

The `patch` lets you set which preset. It can be from 0 to 1024. Another fun parameter is `ratio`, which for ALGO patch types indicates how slow / fast to play the patch's envelopes. Really cool to slow them down!

```python
amy.send(wave=amy.ALGO,osc=0,note=40,vel=1,ratio=0.5,patch=8) # half speed
amy.send(wave=amy.ALGO,osc=0,note=40,vel=1,ratio=0.05,patch=8)  # reaaall sloooow
amy.send(wave=amy.ALGO,osc=0,note=30,vel=1,ratio=0.1,patch=590) 
```

Let's make the classic FM bell tone ourselves, without a preset. We'll just be using two operators (two sine waves), one modulating the other. 

![DX7 Algorithms](https://raw.githubusercontent.com/bwhitman/alles/main/pics/dx7_algorithms.jpg)

When building your own algorithm sets, assign a separate oscillator as wave=`ALGO`, but the source oscillators as `SINE`. The algorithm #s are borrowed from the DX7. You don't have to use all 6 operators, any operators specified as `-1` will be ignored. Note that the `algo_source` parameter counts backwards from operator 6. When building operators, they can have their frequencies specified directly with `freq` or as a ratio of the root `ALGO` oscillator via `ratio`.


```python
amy.reset()
amy.send(wave=amy.SINE,ratio=0.2,amp=0.1,osc=0,bp0_target=amy.TARGET_AMP,bp0="1000,0,0,0")
amy.send(wave=amy.SINE,ratio=1,amp=1,osc=1)
amy.send(wave=amy.ALGO,algorithm=1,algo_source="-1,-1,-1,-1,1,0",osc=2)
```

Let's unpack that last line: we're setting up a ALGO "oscillator" that controls up to 6 other oscillators. We only need two, so we set the `algo_source` to mostly -1s (not used) and have oscillator 1 modulate oscillator 0. You can have the operators work with each other in all sorts of crazy ways. For this simple example, we just use the DX7 algorithm #1. And we'll use only operators 2 and 1. Therefore our `algo_source` lists the oscillators involved, counting backwards from 6. We're saying only have operators 2 and 1, and have oscillator 1 modulate oscillator 0. 

What's going on with `ratio`? And `amp`? Ratio, for FM synthesis operators, means the ratio of the frequency for that operator and the base note. So oscillator 0 will be played a 20% of the base note, and oscillator 1 will be the frequency of the base note. And for `amp`, that's something called "beta" in FM synthesis, which describes the strength of the modulation. Note we are having beta go down over 1,000 milliseconds using a breakpoint. That's key to the "bell ringing out" effect. 

Ok, we've set up the oscillators. Now, let's hear it!

```python
amy.send(osc=2, note=60, vel=3)
```

You should hear a bell-like tone. Nice. Another classic two operator tone is to instead modulate the higher tone with the lower one, to make a filter sweep. Let's do it over 5 seconds.

```python
amy.reset()
amy.send(osc=0,ratio=0.2,amp=0.5,bp0_target=amy.TARGET_AMP,bp0="0,0,5000,1,0,0")
amy.send(osc=1,ratio=1)
amy.send(osc=2,algorithm=1,wave=amy.ALGO,algo_source="-1,-1,-1,-1,0,1")
```

Just a refresher on breakpoints; here we are saying to set the beta parameter (amplitude of the modulating tone) to 0.5 but have it start at 0 at time 0, then be at 1.0x of 0.5 (so, 0.5) at time 5000ms. At the release of the note, set beta immediately to 0. We can play it with

```python
amy.send(osc=2,vel=2,note=50)
```


## Partials

Additive synthesis is simply adding together oscillators to make more complex tones. You can modulate the breakpoints of these oscillators over time, for example, changing their pitch or time without artifacts, as the synthesis is simply playing sine waves back at certain amplitudes and frequencies (and phases.) It's well suited to certain types of instruments. 

![Partials](https://raw.githubusercontent.com/bwhitman/alles/main/pics/partials.png)

We have analyzed the partials of a group of instruments and stored them as presets baked into the synth. Each of these patches are comprised of multiple sine wave oscillators, changing over time. The `PARTIALS` type has the presets:

```python
amy.send(osc=0,vel=1,note=50,wave=amy.PARTIALS,patch=5) # a nice organ tone
amy.send(osc=0,vel=1,note=55,wave=amy.PARTIALS,patch=5) # change the frequency
amy.send(osc=0,vel=1,note=50,wave=amy.PARTIALS,patch=6,ratio=0.2) # ratio slows down the partial playback
```

Our partial breakpoint analyzer also emits "noise-excited bandwidth enhancement", which means it tries to emulate tones that are hard to generate with sine waves alone by modulating the amplitude of a sine wave with a filtered noise signal. You can try that out on the patches by adding `feedback`, like so:

```python
amy.send(osc=0,vel=1,note=50,wave=amy.PARTIALS,patch=6,feedback=0) # no bandwidth
amy.send(osc=0,vel=1,note=50,wave=amy.PARTIALS,patch=6,feedback=0.5) # more bandwidth
```

The presets are just the start of what you can do with partials in AMY. You can analyze any piece of audio and decompose it into sine waves and play it back on the synthesizer in real time. It requires a little setup on the client end, here on macOS:

```bash
brew install python3 swig libsoundio ffmpeg
python3.9 -m pip install pydub numpy --user
tar xvf loris-1.8.tar
cd loris-1.8
CPPFLAGS=`python3-config --includes` PYTHON=`which python3.9` ./configure --with-python
make
sudo make install
cd ..
```

And then in python:

```python
import partials
(m,s) = partials.sequence("sleepwalk.mp3") # Any audio file
109 partials and 1029 breakpoints, max oscs used at once was 8

partials.play(s, amp_ratio=2, bw_ratio=0)
```

https://user-images.githubusercontent.com/76612/131150119-6fa69e3c-3244-476b-a209-1bd5760bc979.mp4


You can see, given any audio file, you can hear a sine wave decomposition version of in AMY. This particular sound emitted 109 partials, with a total of 1029 breakpoints among them to play back to the mesh. Of those 109 partials, only 8 are active at once. `partials.sequence()` performs voice stealing to ensure we use as few oscillators as necessary to play back a set. 

There's a lot of parameters you can (and should!) play with in Loris. `partials.sequence`  and `partials.play`takes the following with their defaults:

```python
def sequence(filename, # any audio filename
                max_len_s = 10, # analyze first N seconds
                amp_floor=-30, # only accept partials at this amplitude in dB, lower #s == more partials
                hop_time=0.04, # time between analysis windows, impacts distance between breakpoints
                max_oscs=amy.OSCS, # max AMY oscs to take up
                freq_res = 10, # freq resolution of analyzer, higher # -- less partials & breakpoints 
                freq_drift=20, # max difference in Hz within a single partial
                analysis_window = 100 # analysis window size 
                ) # returns (metadata, sequence)

def play(sequence, # from partials.sequence
                osc_offset=0, # start at this oscillator #
                sustain_ms = -1, # if the instrument should sustain, here's where (in ms)
                sustain_len_ms = 0, # how long to sustain for
                time_ratio = 1, # playback speed -- 0.5 , half speed
                pitch_ratio = 1, # frequency scale, 0.5 , half freq
                amp_ratio = 1, # amplitude scale,
                bw_ratio = 1, # bandwidth / noise scale
                )
```



## PCM

AMY comes with a set of 67 drum-like and instrument PCM samples to use as well, as they are normally hard to render with additive or FM synthesis. You can use the type `PCM` and patch numbers 0-66 to explore them. Their native pitch is used if you don't give a frequency or note parameter. You can update the PCM sample bank using `amy_headers.py`. 


```python
amy.send(osc=0, wave=amy.PCM, vel=1, patch=10) # cowbell
amy.send(osc=0, wave=amy.PCM, vel=1, patch=10, note=70) # higher cowbell! 
```

You can turn on sample looping, helpful for instruments, using `feedback`:

```python
amy.send(wave=amy.PCM,vel=1,patch=21,feedback=0) # clean guitar string, no looping
amy.send(wave=amy.PCM,vel=1,patch=21,feedback=1) # loops forever until note off
amy.send(vel=0) # note off
amy.send(wave=amy.PCM,vel=1,patch=35,feedback=1) # nice violin
```


## Developer zone

### Generate header files for patches and LUTs

Run `python amy_headers.py` to generate all the LUTs and patch .h files compiled into AMY.

### ESP32

Use `amy-example-esp32.c` for a starting point to booting AMY on an ESP32 chip.






