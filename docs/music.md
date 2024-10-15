# Using Tulip to make music tutorial

Welcome to Tulip! 

This tutorial shows you all the ways to make music and make code to make music on Tulip. We'll update it as more examples, techniques and demos appear. 

First, make sure you've gone through the initial [getting started tutorial](getting_started.md). Especially, please make sure you've run `tulip.upgrade()`. 

## What Tulip is

Tulip is a **music computer** where everything about the underlying synthesis and sequencer engine can be programmed, live, on the Tulip itself. We provide a code editor and you can run programs right on Tulip, including multi-tasking apps that work together. We provide a few programs -- a patch editor, a place to assign voices, a drum machine -- but the real value of Tulip is the power to make whatever you imagine, or the programs contributed by others. 

## What Tulip can do with music 

 - Play very accurate Juno-6 patches and similar analog synths, up to 6 note polyphony / multimbrality. You have full control over all the parameters.
 - Play very accurate DX7 patches, and allow you to create your own FM synthesis setup -- in code!
 - Play built in 808-style drum patches, with pitch control 
 - Load in your own .WAV samples to use in a sampler
 - Control CV outputs for modular synths and analog synths, with built in waveforms and sample & hold 
 - Sample CV inputs from an ADC to control other events on Tulip 
 - Send MIDI in and out, from code. You can write code to respond to MIDI messages to do whatever you want
 - Use USB MIDI devices like keyboards or synthesizers or adapters connected to Tulip's USB-KB port (with a hub to still support typing keyboards)
 - Share a common sequencer clock across multiple apps, for example, a drum machine and an arpeggiator 
 - Add global EQ, chorus, echo or reverb to the audio output
 - A scale and chord library to define musical notes in code, e.g. `music.Chord("F:min")`
 - Have total low level control of all oscillators, specifying their filters, waveform, modulation sources, ADSRs

## A small note about Tulip Desktop

If you're using [Tulip Desktop](tulip_desktop.md) instead of a real Tulip, things will mostly be the same. CV sending does not work on Tulip Desktop. And anytime you see `/sys/` (for example, to open an example), replace it with `../sys/`. 


## The built-in Tulip synthesizer

When you start up your Tulip, it is configured to receive MIDI messages from the MIDI in port (or the USB MIDI port if connected.) You can plug in any MIDI device that sends MIDI out, like a MIDI keyboard or your computer running a sequencer. 

Try to just play notes once you've turned on Tulip, By default, MIDI channel 1 plays a Juno-6 patch. Notes on channel 10 will play PCM patches, roughly aligned with General MIDI drums. 

You can adjust patch assignments per channel, or change patches, using our built in `voices` app. You can type `run('voices')` or tap the bottom right launcher menu and tap `Voices`. 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/voices.png" width=600>


The red button on the top right will quit the app, and the blue button will switch to other apps, including the REPL (where you can type commands.)

Try changing the channel 1 patch to a different Juno-6 patch, or a DX-7 patch. You can also tap the keyboard on screen, even with multiple fingers. Try also playing with the arpeggiator. 

## The built-in Juno-6 patch editor

If you want to modify parameters of the Juno-6 synthesizer you can open `juno6`, either from the REPL or the launcher menu. While you have a Juno-6 patch assigned to a MIDI channel, you can modify any parameters of it, just like a real Juno-6. You can also assign MIDI control change values to a parameter, so that your MIDI keyboards sliders/knobs can control parameters.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/juno6.png" width=600>


## The Tulip sequencer and built-in drum machine

There's an 808-style drum machine that comes with Tulip, accessed via `run('drums')`. You can play back any PCM sample that ships on Tulip, or even load your own (see below.) The Tulip drum machine shares a common clock alongside any other Tulip program that uses a sequencer. This means that for example the arpeggiator in the `voices` app will stay in time with the drum machine. We'll show you how to control and use the sequencer in your own programs below.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/drums.png" width=600>


Set up a nice drum sequence for a bit. Play with the pitch parameter and pan. Now, go back to the REPL with the "switch" icon, or by using `control-Tab`. The drum machine should still be playing!

Let's make some of our own Tulip code to play along! You can make programs with our editor or just write small functions in the REPL itself. Let's start there. What we want to do is play the notes of a chord randomly, but in time with the drum machine already playing.

First, let's grab the synth we'll be playing. Let's just use the default playing synth - the Juno-6 patch 0 on MIDI channel 1. If you changed it to something else, that's fine! While we're here, we'll also choose the chord we want to play. So just type

```python
import music, random
chord = music.Chord("F:min7").midinotes()
synth = midi.config.get_synth(channel=1)
```

The first `import music, random` tells Tulip that we'll be using those libraries. Some (like `tulip, amy, midi`) are already included on bootup, but it's a good habit to get into when writing programs. 

If you just type `chord` and enter, you'll see the contents - just a list of MIDI notes corresponding to F minor 7. And `synth` is an object you can send messages to do make tones. Try `synth.note_on(50, 0.5)` -- will play a MIDI note 50 at half volume. 

To set this up to play with the sequencer, we will add a **sequencer callback**.  This is just a simple Python function that will get called every sequencer "tick". You can customize how often the tick is called. We'll call ours every quarter note. But first, let's define the function that will get called. Type this into the REPL. Hit the `Tab` key for the line that have four spaces before the code to make an indent. Hit enter a few times at the end until you see a `>>>` prompt again (not a `...`.) 

```python
def note(t):
    synth.note_on(random.choice(chord), 0.6, time=t)
```

This function is called `note` and will tell our `synth` to play a random note of the `chord` at velocity 0.6 at time `t`. `random.choice(x)` just means to pull a random thing out of a list, like our list of midi notes. Don't worry about `time` right now, the sequencer takes care of it! This callback can do anything -- it can draw graphics, send or read MIDI events, send or read CV values from an external synth, do something on a Wi-Fi network change synth parameters, whatever you want. But we'll start with simply playing a note every quarter note.

For now, tell the callback to start:

```python
slot = tulip.seq_add_callback(note, 48)
```

You should now be hearing a synth pattern play every quarter note in time with the drums. It will keep going! The second parameter of `seq_add_callback` is the `divider` of the callback. This tells Tulip how often to call the callback. We send 48 ticks a quarter note. So giving `48` to the divider just means to call the function once a quarter note. We can instead call our `note` every 1/8th note by changing the divider to `24`:

```python
tulip.seq_remove_callback(slot) # the synth should stop 
slot = tulip.seq_add_callback(note, 24) # it should start in double time again
```

(If you want to do more complex things that aren't in even time, you can set your divider to `1` and it will be called every single tick (with roughly 10ms between ticks) and you can decide when to emit notes.)

The `slot` that gets returned by `seq_add_callback` is how you keep track of your callbacks. Every program on Tulip that uses the sequencer (like the drum machine, arpeggiator, etc) has its own callback slot. Your programs should end in a `tulip.seq_remove_callback(slot)`.

While you're playing with this, go back to the `voices` app and change the patch on channel 1 to something else. You'll hear your synth change immediately. You can also do this programatically: 

```python
synth.program_change(143) # change to patch #143 (dx-7 patch BASS 2)
```

You can also easily change the BPM of the sequencer -- this will impact everything using it, like the drum machine as well:

```python
tulip.seq_bpm(120)
```

## Making new Synths

We're using `midi.config.get_synth(channel=1)` to "borrow" the synth booted with Tulip. But if you're going to want to share your ideas with others, you should make your own `Synth` that doesn't conflict with anything already running on Tulip. That's easy, you can just run:

```python
synth = midi.Synth(num_voices=2, patch_number=143) # two note polyphony, patch 143 is DX7 BASS 2
```

And if you want to play multimbral tones, like a Juno-6 bass alongside a DX7 pad:

```python
synth1 = midi.Synth(num_voices=1, patch_number=0)  # Juno
synth2 = midi.Synth(num_voices=1, patch_number=128)  # DX7
synth1.note_on(50, 1)
synth2.note_on(50, 0.5)
```

You can also "schedule" notes in the near future (up to 20 seconds ahead). This is useful for sequencing fast parameter changes or keeping in time with the sequencer. `Synth`s accept a `time` parameter, and it's in milliseconds. For example:

```python
# play a chord all at once
import music, midi, tulip
synth4 = midi.Synth(num_voices=4, patch_number=1)
chord = music.Chord("F:min7").midinotes()
for i,note in enumerate(chord):
    synth4.note_on(note, 0.5, time=tulip.ticks_ms() + (i * 1000))   # time is i seconds from now
    # each note on will play precisely one second after the last
```

You can send `all_notes_off()` to your synth to stop playing notes:

```python
synth4.all_notes_off()
```

If you are booting a new Synth in your program, remember to `release` your synths when you're done with them
```python
synth1.release() # Does all note-off and then clears the voice alloc 
synth2.release()
synth4.release()
```

As you learn more about AMY (the underlying synth engine) you may be interested in making your own `Synth`s in Python. See `midi.py`'s `OscSynth` for an example. 

## Modifying the default synth or other MIDI channel assignments in code

You may want to programatically change the MIDI to synth mapping. One example would be to lower the polyphony of the booted by default 6-note synth on channel 1, so that notes coming in through MIDI don't impact the performance or polyphony of your app. Or if you want to set up your music app to receive different patches on different MIDI channels. 

You can change the parameters of channel synths like this:

```python
midi.config.add_synth(channel=c, patch_number=p, num_voices=n)
```

Note that `add_synth` will stop any running Synth on that channel and boot a new one in its place. 

## The editor

You can get a lot done in the Tulip REPL just playing around. But you'll eventually want to save your creations and run them alongside other things. Let's start up the Tulip Editor and save your first program. You can go ahead and quit the drum machine if you want, and remember to run

```
tulip.seq_remove_callback(slot) # the synth should stop 
```

to stop your REPL sequencer callback.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/jampy.png" width=600>

Type `edit('jam.py')` (or whatever you want to call it.) You'll see a black screen open. This is the editor! You can use it like a computer's editor to save code. Just start typing, let's put our program in posterity:

```python
import tulip, midi, music, random

chord = music.Chord("F:min7").midinotes()
synth = midi.Synth(num_voices=1, patch_number=143)  # DX7 BASS 2
slot = None

def note(t):
    synth.note_on(random.choice(chord), 0.6, time=t)

def start():
    global slot
    slot = tulip.seq_add_callback(note, 24)

def stop():
    global slot
    tulip.seq_remove_callback(slot)
    synth.release()

```


Hit `control-X` to save this (you'll see a little asterisk `*` go away in the status bar) and then either `control-Q` to quit the editor or `control-Tab` to switch back to the REPL. Now, just:


```
import jam
jam.start() # should start playing
jam.stop() # will stop playing
```

This will be easier for you to edit your creations and try things out. Try different chords, patches, different sequences, etc. If you stopped the drum machine, you can restart it anytime and it will stay in time with your `jam`.

The drum machine itself is actually a simple Tulip program, written in Python. You can look at the code on your Tulip, or edit it and run your own version. You can go back to the REPL and run `edit('/sys/ex/my_drums.py')` -- this will open a copy of the drum machine you can edit. Try changing the color of the LED on line 12 to say, 9. Then save ('control-X'), then quit the running drum machine and open your version -- `run('my_drums')`. You should see the change! Check out the rest of the drum machine code. Most of it is UI setup, and then you can see it setting up a sequencer callback and sending off the drum messages every beat. 

## UIScreens

You may have noticed that the editor, the `voices` and `drums` app all have those buttons on the top right and their own screen, while your `jam.py` just runs in the REPL. That works for a lot of things, but maybe you want to build a UI or show status of your app on its own screen. To do that, we can easily adapt our simple program to be a Tulip `UIScreen` app like its big siblings. Let's make a `jam2.py` into a UIScreen with a slider for tempo. Open `edit('jam2.py')` (quit the editor and restart it if you're editing something else) and make it

```python
import tulip, midi, music, random

def note(t):
    global app
    app.synth.note_on(random.choice(app.chord), 0.6, time=t)

def start(app):
    app.slot = tulip.seq_add_callback(note, 24)

def stop(app):
    tulip.seq_remove_callback(app.slot)
    app.synth.release()

def run(screen):
    global app
    app = screen
    app.slot = None
    app.chord = music.Chord("F:min7").midinotes()
    app.synth = midi.Synth(num_voices=1, patch_number=143)  # DX7 BASS 2
    app.present()
    app.quit_callback = stop
    start(app)
```

Save this and then just `run('jam2')` from the REPL (you can omit the `.py` if you want.) You'll see a blank screen with the top right buttons and you should be hearing your sequence, started when the app starts. If you hit "switch" or `control-Tab` it will still be going. If you quit or `control-Q` the app, it will stop. That's the `quit_callback` being setup there. 

Let's add a UI component. Add a few lines to `run` so it looks like:

```python
def run(screen):
    global app
    app = screen
    app.slot = None
    app.chord = music.Chord("F:min7").midinotes()
    app.synth = midi.Synth(num_voices=1, patch_number=143)  # DX7 BASS 2
    bpm_slider = tulip.UISlider(tulip.seq_bpm()/2.4, w=300, h=25,
        callback=bpm_change, bar_color=123, handle_color=23)
    app.add(bpm_slider, x=300,y=200)
    app.present()
    app.quit_callback = stop
    start(app)
```

And also add a new function, above the `run(screen)`. This is the callback from when someone changes the bpm slider. In this function, we want to change the system BPM:

```python
def bpm_change(event):
    tulip.seq_bpm(event.get_target_obj().get_value()*2.4)
```

Now quit the `jam2` app if it was already running and re-`run` it. You should see a slider, and when you move it, the system BPM changes. The `2.4` you see in the code is because sliders return values between 0-100, and we convert that into BPMs of 0-240. You can add all sorts of UI elements. We provide a few classic ones like `UICheckbox` and `UIButton`. You should look at the source of `drums` and `voices` to see how they're built and [our API documentation for the full details.](tulip_api.md) How about a `UIText` entry box for people to type in the chord they want to play? 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/jam2.png" width=600>


## Sampler, OscSynth

Tulip defines a few different `Synth` classes, including `OscSynth` which directly uses one oscillator per voice of polyphony, as in this simple sine wave synth:

```python
s = midi.OscSynth(wave=amy.SINE)
s.note_on(60,1)
s.note_off(60)
```

Let's try it as a sampler. There are 29 samples of drum-like and instrument sounds in Tulip, and it can adjust the pitch and pan and loop of each one. You can try it out with:

```python
# You can pass any AMY arguments to the setup of OscSynth
s = midi.OscSynth(wave=amy.PCM, patch=10) # PCM wave type, patch=10 (808 Cowbell)

s.note_on(50, 1.0)
s.note_on(40, 1.0) # different pitch

s.update_oscs(pan=0) # different pan
s.note_on(40, 1.0)

s.update_oscs(feedback=1, patch=23) # patch 23 is Koto, feedback=1 loops the sound
s.note_on(40, 1.0)
s.note_off(40) # looped instruments require a note_off to stop
```

You can load your own samples into Tulip. Take any .wav file and [load it onto Tulip.](getting_started.md#transfer-files-between-tulip-and-your-computer) Now, load it in as a PCM patch:

```python
amy.load_sample('sample.wav', patch=50)
s = midi.OscSynth(wave=amy.PCM, patch=50)
s.note_on(60, 1.0)
```

You can also load PCM patches with looped segments if you have their loopstart and loopend parameters (these are often stored in the WAVE metadata. If the .wav file has this metadata, we'll parse it. The example file `/sys/ex/vlng3.wav` has it. You can also provide the metadata directly.) To indicate looping, use `feedback=1`. 

```python
amy.load_sample("/sys/ex/vlng3.wav", patch=50)  # loads wave looping metadata
s = midi.OscSynth(wave=amy.CUSTOM, patch=50, feedback=1, num_voices=1)
s.note_on(60, 1.0) # loops
s.note_on(55, 1.0) # loops
s.note_off(55) # stops
```

You can unload samples from RAM with `amy.unload_sample(patch_number)`. 

## Modify Juno-6 patches programatically

We showed above how to `run('juno6')` to see a Juno-6 editor. But if you want your code to change the patches, you can do it yourself with:

```python
run("juno6")
# Go back to REPL
juno6.vcf_res.set(64) # 0-127
```

If you switch back to the Juno-6 editor, you'll see the slider for resonance physically moved as well.

You can type `juno6.` and then hit the `TAB` key to see functions you can call. 


## AMY low-level control in Tulip

Tulip's synth is powered by [AMY](https://github.com/shorepine/amy), a very full featured multi-oscillator additive and subtractive style synth. It's got tons of features. Our examples so far have all been in the "higher-level" Tulip python mode, but you can send direct AMY commands by simply calling `amy.send()`:

```python
import amy
amy.send(osc=30, wave=amy.SINE, freq=440, vel=1) # 440Hz sine wave
amy.send(osc=30, vel = 0) # note off
amy.reverb(1) # turn on global reverb
amy.echo(level=1, delay_ms=400, feedback=0.8) # global echo
amy.drums() # play a test pattern
amy.reset() # reset every AMY oscillator
```

Here's how to make an 808-style bass drum tone in pure AMY oscillators:

```python
amy.send(osc=31, wave=amy.SINE, amp=0.5, freq=0.25, phase=0.5)
amy.send(osc=32, wave=amy.SINE, bp0="0,1,500,0,0,0", freq="261.63,1,0,0,0,1", mod_source=31, vel=1)
```

If you're interested in going deeper on all that AMY can do, [check out AMY's README](https://github.com/shorepine/amy/blob/main/README.md). 


## Sending and receiving MIDI in code

You can write functions that respond to MIDI inputs easily on Tulip. Let's say you wanted to make a very simple sine wave oscillator that responded to MIDI messages on channel 1. All you need to do is write a MIDI callback function, like the sequencer one you did earlier. 

```python
import midi, amy
def sine(m):
    if m[0] == 144: # MIDI message byte 0 note on
        # send a sine wave to osc 30, with midi note and velocity
        amy.send(osc=30, wave=amy.SINE, note=m[1], vel=m[2] / 127.0)

# Stop the default MIDI callback that plays e.g. Juno notes, so we can hear ours 
midi.stop_default_callback()

# Add our callback
midi.add_callback(sine)

# Now play a MIDI note into Tulip. If you don't have a KB attached, use midi_local to send the message:
tulip.midi_local((144, 40, 100))
# You should hear a sine wave

midi.start_default_callback()
```

To send MIDI out, just use `tulip.midi_send(message)`. You can, for example, send a MIDI message out every sequencer tick on Tulip.

## Outputting CV signals to modular and analog synths

Tulip can output CV signals instead of audio signals out a compatible DAC chip that you attach on the side "i2c" port. [You can get a DAC](https://github.com/shorepine/tulipcc/blob/main/docs/getting_started.md#dacs-or-adcs-for-modular-synths) and send any waveform out its port, even synced to a tempo or using sample & hold. I'd first recommend trying out the user-contributed `waves.py` app, which brings all this together using a GUI: 

To get the `waves.py` app, you'll first need to join Wi-Fi and get it from Tulip World. See the [getting started](getting_started.md) tutorial for more info, but just do:

```python
tulip.wifi(ssid, password)
world.download('waves.py')
run('waves')
```

And you should see:

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/waves.png" width=600>

If you look at the source of `waves.py` in the editor, you can see it's pretty simple after the UI setup. 

```python
amy.send(osc=30, external_channel=1, wave=amy.SAW_DOWN, vel=1, freq=0.5, amp=1)
```
This sends a saw wave out the first channel of DACs at 0.5Hz with amplitude of 1, so will be 0-5V peak to peak. 

Send

```python
amy.send(osc=30, amp=0)
```

to stop it. You can also send individual voltages with

```python
import mabeedac
mabeedac.send(2.5, channel=0) # sends 2.5V to the first CV channel.
```

## Custom FM tones and AMY patches (WOOD PIANO)

Let's see how to make your own FM synthesis in AMY commands. The 4-operator "WOOD PIANO" FM tone is a classic. It's pretty simple, too. 

Let's make a custom patch, which is a set of AMY commands you can assign to a patch number (from 1024 to 1055.)

To do so, ask AMY to "remember" what we're doing, but not send it to the internal synthesizer. This is done by

```python
amy.start_store_patch()
```

Anything you send AMY from now will be logged to a custom patch and not to the internal synth. 

Now, send the AMY setup commands for your patch. Make sure your patch is consecutive oscillators, starting at oscillator 0 for the "root oscillator."

The WOOD PIANO patch is four operators, each with an envelope and different modulation amplitude.

```python
amy.send(osc=1, bp0="0,1,5300,0,0,0", phase=0.25, ratio=1, amp="0.3,0,0,1,0,0")
amy.send(osc=2, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
amy.send(osc=3, bp0="0,1,6700,0,0,0", phase=0.25, ratio=1, amp="0.23,0,0,1,0,0")
amy.send(osc=4, bp0="0,1,3400,0,0,0", phase=0.25, ratio=0.5, amp="1.68,0,0,1,0,0")
```

Then, send the "root" oscillator instructions. This is the one you'd send note-ons to. The root oscillator gets an "algorithm", which indicates how to modulate the operators. See the [AMY documentation](https://github.com/shorepine/amy) for more detail. It also gets its own amplitude and pitch envelopes (`bp0` and `bp1`). 


```python
amy.send(osc=0, wave=amy.ALGO, algorithm=5, algo_source="1,2,3,4", bp0="0,1,147,0", bp1="0,1,179,1", freq="0,1,0,0,1,1")
```

Now, tell AMY to stop logging the patch and store it to a custom patch number.

```python
amy.stop_store_patch(1024)
```

Now, you're free to use this patch number like all the Juno and DX7 ones. For a polyphonic wood piano, do:

```python
s = midi.Synth(5)
s.program_change(1024)
s.note_on(50, 1)
s.note_on(50, 1)
s.note_on(55, 1)
```

Try saving these setup commands (including the `store_patch`, which gets cleared on power / boot) to a python file, like `woodpiano.py`, and `execfile("woodpiano.py")` on reboot to set it up for you!







