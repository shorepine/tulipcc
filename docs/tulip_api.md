# The Tulip Creative Computer API 

Here you can see the API [Tulip](../README.md) currently ships with.

# Current API

**NOTE**: Our own builds of [Tulip Desktop](tulip_desktop.md) may lag behind some of the APIs listed here. This page represents the APIs in the latest commit of our _main_ branch. If something isn't working properly, try building from scratch.

![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4.png)

## General

[Tulip](../README.md) boots right into a Python prompt and all interaction with the system happens there. You have your own space to store code and files in `/user` and we keep system examples and programs in `/sys`. 

You can make your own Python programs with Tulip's built in editor and execute them, or just experiment on the Tulip REPL prompt in real time.

```python
# Interact with the filesystem.
# Supported: ls, head, cat, newfile, cp, mv, rm, pwd, cd, mkdir, rmdir
ls
mkdir('directory')
cd('directory')

# Clears the REPL screen and formatting
clear

# If you want something to run when Tulip boots, add it to boot.py
edit("boot.py")

# You can upgrade the firmware over-the-air over wifi
tulip.upgrade()

# Takes a screenshot and saves to disk. The screen will blank for a moment
# If no filename given will upload to imgur and return a URL (needs wifi)
tulip.screenshot("screenshot.png")
imgur_url = tulip.screenshot()

# Return the current CPU usage (% of time spent on CPU tasks like Python code, sound, some display)
# Note that Python code is bound to one core, so Python-only usage tops out at 50%.
usage = tulip.cpu() # or use tulip.cpu(1) to show more detail in a connected UART

ms = tulip.ticks_ms() # returns the milliseconds since boot, aka Arduino millis() 

board = tulip.board() # Returns the board type, e.g. "TDECK", "N16R8" etc
```

## Using and making scripts, packages and screens

Tulip can run different types of programs that you make or you can download from [Tulip World](#tulip-world). They range from simple Python scripts or modules, to full-screen "apps" with multitasking and UIs. You can edit and create these apps on Tulip itself using our editor, and upload them to Tulip World for others to use. 

You can run any Python script in your current directory with `execfile`:

```python
>>> execfile("hello.py")
Hello world
```

You can also create Python libraries and `import` them from your current directory:

```python
>>> import my_library
>>> my_library.do_something()
Doing it
```

## Tulip Packages

If you have a program that relies on mulitple files (graphics, sounds or multiple Python files) you'll want to create a Tulip package. A package is just a folder with your files in it, like:

```
rabbit_game/
... rabbit_game.py # the main script should have the same name as the package
... extra.py # can put any other python files in here
... rabbit_pic.png
... rabbit_pic1.png
... rabbit_sample.wav
```

The main Python script must be the name of the package. This script needs to explicitly `import tulip` or `amy` or others if you are using those. Then, you and your users can start the package by `run('rabbit_game')` from the directory that has the folder in it. The package will be cleaned up after when they exit. 

By default, a package is imported (for example, `import rabbit_game`.) If your `rabbit_game.py` has code that runs on import, it will run. If you are writing a game or other thing that needs full access to Tulip, put an infinite loop in your code. Users can use `Control-C` to quit and they will be back at the REPL with the program's imports removed from memory.

We ship a couple of game-like examples, check them out:
 * [`bunny_bounce`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/bunny_bounce/bunny_bounce.py)
 * [`planet_boing`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/planet_boing/planet_boing.py)


The Tulip World BBS supports uploading and downloading packages as tar files: just `world.upload('package', username)` or `world.download('package')`. 

We put a few system packages in `/sys/app` and some examples in `/sys/ex`, and if you `run('app')`, it will look in your current folder, the `/sys/ex` folder, and the `/sys/app` folder for the app. 


### Multitasking apps 

If you want your package to run alongside other apps, and show a task bar with a quit and app-switch button, you need to use a package that implements `UIScreen`. `UIScreen`'s API is [detailed below](#uiscreen), but a simplest example is:

```python
# my switchable program, program.py
def run(app):
    # Setup my app
    app.present() # I'm ready, show my app
```

Put that in a package called `program`, and when `run('program')` is called, your app will start and show a task bar. Multitasking apps have to return immediately after setup (the `run` function) and rely on callbacks to process data and user input. We have callbacks for almost everything you'd need: keyboard input, MIDI input, music sequencer ticks and touch input. `UIScreen` also sets up callbacks for "activating" (switching to the app or first run), "deactivating" (switching away from the app) or quitting. 

`UIScreen` apps should use LVGL/`tulip.UIX` classes for their UI, so that the UI appears and disappears automatically during switching. This is especially important on Tulip CC hardware, where we ensure the UI switching drawing does not interrupt music or other time sensitive callbacks. You can also use other Tulip drawing commands for the UI, but be mindful that the BG (and often TFB) will be cleared on switching away from your app, so you'll have to redraw those on your activate callback. 

The REPL itself is treated as a (special) multitasking app, always first in the list and cannot be quit. 

You can switch apps with the keyboard: `control-tab`, and quit apps with `control-Q`. 

We ship a few examples of multitasking apps, please check them out here:
 * [`juno6`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/juno6.py)
 * [`wordpad`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/wordpad/wordpad.py)
 * [`buttons`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/buttons/buttons.py)
 * [`drums`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/drums.py)
 * [`voices`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/voices.py)

## Tulip World

Still very much early days, but Tulip supports a native chat and file sharing BBS called **TULIP ~ WORLD** where you can hang out with other Tulip owners. You're able to pull down the latest messages and files and send messages and files yourself. 

You can call the underlying Tulip World APIs:

```python
import world
messages = world.messages(n=500, mtype='files') # returns a list of latest files (not unique ones)
messages = world.messages(n=100, mtype='text') # returns a list of latest chat messages

# When posting messages or files you set a username, minimum 1 character, maximum 10 
world.post_message("hello!!", username) # Sends a message to Tulip World. username required.

world.upload(filename, username, description) # Uploads a file to Tulip World. username required. description optional (25 characters)
world.upload(folder, username, description) # Packages a folder and uploads it to Tulip World as a package
world.download(filename) # Downloads the latest file named filename from Tulip World if it exists
world.download(filename, username) # Downloads the latest file named filename from username from Tulip World if it exists
world.download(package_name) # Downloads a package and extracts it

world.ls() # lists most recent unique filenames/usernames
world.ls(100) # optional count (most recent)
```

Big note: Tulip World is hosted by a bot running on the [Tulip/AMY/Alles Discord](https://discord.gg/TzBFkUb8pG). If there's any abuse of the system, I'll revoke the key. I'd love more help making Tulip World a more stable and fun experience for everyone. 


## The Tulip Editor

Tulip ships with a text editor, based on pico/nano. It supports syntax highlighting, search, save/save-as. 

```python
# Opens the Tulip editor to the given filename. 
# Control-X saves the file, if no filename give will prompt for one. 
# Control-O is save as -- will write to new filename given
# Control-W searches
edit("game.py")
edit() # no filename
```


## User interface

We include [LVGL 9](https://lvgl.io) for use in making your own user interface. LVGL is optimized for constrained hardware like Tulip. You can build nice UIs with simple Python commands. You can use LVGL directly by simply `import lvgl` and setting up your own widgets. Please check out [LVGL's examples page](https://docs.lvgl.io/8.3/examples.html) for inspiration. (As of this writing, their Python examples have not been ported to our version of LVGL (9.0.0) but most things should still work.) 

It's best to build user interfaces inside a `UIScreen` multitasking Tulip package. Our `UIScreen` will handle placing elements on your app and dealing with multitasking. 

For more simple uses of LVGL, like buttons, sliders, checkboxes and single line text entry, we provide wrapper classes like `UICheckbox`, `UIButton`, `UISlider`, `UIText`, and `UILabel`. See our fully Python implementation of these in [`ui.py`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/ui.py) for hints on building your own UIs. Also see our [`buttons.py`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/buttons/buttons.py) example, or more complete examples like [`drums`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/drums.py), [`juno6`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/juno6.py), [`wordpad`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/wordpad/wordpad.py) etc in `/sys/app`. 

### UIScreen

Tulip apps that support multitasking are called `UIScreen`s and they wrap functionality for adding UI elements and switching between apps. A Tulip package tries to `run(screen)` in your main Python file, and if it exists, will expect the `run(screen)` function to exit quickly and hand over control to various callbacks. This allows multiple apps to work at the same time. It's especially useful for music apps that share the sequencer and MIDI callbacks.

By default a `UIScreen` is created for you when you `run(app)`, presuming `app.py` in the package has a `def run(screen):` function. The `UIScreen` object is passed into `screen`. You can treat that object as your app's global state, and also set and get various parameters of the app:

```python
def run(screen):
    # These are all the defaults:
    screen.bg_color = 0 # tulip color of the screen BG
    screen.keep_tfb = False # whether to hide the TFB while running the app
    screen.offset_y = 100 # by default, screens "start" at 0,100 to leave room for the task bar
    screen.activate_callback = None # called when the app starts and when it is switched to
    screen.quit_callback = None # called when the quit button is pressed
    screen.handle_keyboard = False # if you set up UI components that accept keyboard input
    # Set up your UI with screen.add(), adding UIElement objects
    screen.add(tulip.UILabel("hello there"), x=500,y=100)
    # You can use LVGL alignment to add objects in relation to the last object added
    # See https://docs.lvgl.io/master/widgets/obj.html for a listing of aligns
    screen.add(tulip.UILabel("under that one"), direction=lv.ALIGN.BOTTOM_MID)
    # When you're ready, do 
    screen.present()

def quit(screen):
    # your quit callback gets a screen object, use it to shut down
def activate(screen):
    # use this to re-draw anything explicitly. LVGL components added with add() will automatically appear
```

Tulip `UIScreen` apps should never wait in a loop or call `sleep`. They should rely on callbacks to do all their work. For example, our drum machine waits for the sequencer callback to play the next note. Our editor app relies on the keyboard callback for the next keypress. This allows Tulip to run multiple programs at once.

See some examples of more complex UIs using `UIScreen`:
 * [`juno6`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/juno6.py)
 * [`drums`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/drums.py)
 * [`voices`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/voices.py)


You can see running multitasking apps with `tulip.running_apps`, which is a dict by app name. This lets you set or inspect parameters of running apps. `tulip.repl_screen` always returns the REPL UIScreen. You can programtically switch apps with e.g. `tulip.app('drums')`. The current running UIScreen is `tulip.current_uiscreen()`. 

```python
>>> tulip.running_apps['voices'].piano_y
320

>>> tulip.repl_screen.bg_color
9
```



### Other interface elements

You can summon a touch keyboard with `tulip.keyboard()`. Tapping the keyboard icon dismisses it, or you can use `tulip.keyboard()` again to remove it. 

We boot a launcher for common operations. It's available via the small grey icon on the bottom right.

```python
tulip.keyboard() # open or close the soft keyboard
tulip.launcher() # open or close our launcher

# You're free to use any direct LVGL calls. It's a powerful library with a lot of functionality and customization, all accessible through Python.
import lvgl as lv

# our tulip.lv_scr is the base screen on bootup, to use as a base screen in LVGL.
calendar = lv.calendar(lv.current_screen())
calendar.set_pos(500,100)

# use our tulip.UIX classes to add simple UI elements to your app.

# UISlider: draw a slider
# bar_color - the color of the whole bar, or just the set part if using two colors
# unset_bar_color - the color of the unset side of the bar, if None will just be all one color
# handle_v_pad, h_pad -- how many px above/below / left/right of the bar it extends
# handle_radius - 0 for square 
screen.add(tulip.UISlider(val=0, w=None, h=None, bar_color=None, unset_bar_color=None, 
    handle_color=None, handle_radius=None, handle_v_pad=None, handle_h_pad=None, callback=None))

# UIButton: push button with text
screen.add(tulip.UIbutton(text=None, w=None, h=None, bg_color=None, fg_color=None, 
    font=None, radius=None, callback=None))

# UILabel: text
screen.add(tulip.UILabel(text="", fg_color=None, w=None, font=None))

# UIText: text entry
screen.add(tulip.UIText(text=None, placeholder=None, w=None, h=None, 
    bg_color=None, fg_color=None, font=None, one_line=True, callback=None))

# UICheckbox
# Optionally draw a label next to the checkbox
screen.add(tulip.UICheckbox(text=None, val=False, bg_color=None, fg_color=None, callback=None))
```

See our [`buttons.py`](https://github.com/bwhitman/tulipcc/blob/main/tulip/fs/app/buttons/buttons.py) example for `UIX` class use.

## Input

Tulip supports USB keyboard input and touch input. It also supports a software on-screen keyboard, and any I2C connected keyboard or joystick on Tulip CC. On Tulip Desktop, mouse clicks act as touch points, and your computers' keyboard works.

```python
# Returns a mask of joystick-like presses from the keyboard, from arrow keys, Z, X, A, S, Q, W, enter and '
tulip.joyk()

# test for joy presses. Try UP, DOWN, LEFT, RIGHT, X, Y, A, B, SELECT, START, R1, L1
if(tulip.joyk() & tulip.Joy.UP):
    print("up")

# Returns the current held keyboard scan codes, up to 6 and the modifier mask (ctrl, shift etc)
(modifiers, scan0, scan1... scan5) = tulip.keys()

# Gets a key ascii code
(char, scan, modifier) = tulip.key_wait() # waits for a key press, returns scan code and modifier too  
ch = tulip.key() # returns immediately, returns -1 if nothing held

# If scanning key codes in a program, you may want to turn on "key scan" mode so that
# keys are not sent to the underlying python process 
tulip.key_scan(1)
tulip.key_scan(0) # remember to turn it back off or you won't be able to type into the REPL

# If you need to remap keys on your keyboard (we default to US)
tulip.remap()  # interactive, can write to your boot.py for you
tulip.key_remap(scan_code, modifier, target_cp437_code)

# You can also register a keyboard callback. Useful for full screen apps that share with others
# there can only be one keyboard callback running. 
tulip.keyboard_callback(key)
def key(k):
    print("got key: %d" % (key))

tulip.keyboard_callback() # removes callbacks. 

# Return the last touch panel coordinates, up to 3 fingers at once
(x0, y0, x1, y1, x2, y2) = tulip.touch()

# Modify the touch screen calibration if needed (on Tulip CC only)
# Run ex/calibrate.py to determine this for your panel 
tulip.touch_delta(-20, 0) # -20 x, 0 y
tulip.touch_delta() # returns current delta

# Set up a callback to receive raw touch events. up == 1 when finger / mouse lifted up
def touch_callback(up):
    t = tulip.touch()
    print("up %d points x1 %d y1 %d" % (up, t[0], t[1]))

tulip.touch_callback(cb)

# Make your own elements on the BG with your own styling and set up your own callbacks to process them
tulip.bg_rect(200,200,50,50,23,1)
tulip.bg_touch_register(12, 200, 200, 50, 50) # 12 is an ID -- from 0-254
def bgt(id):
    print("Got a click on id %d. up was %d" % (id, tulip.bg_touch_up(id)) # bg_touch_up returns 1 for touch up

tulip.bg_touch_callback(bgt)
tulip.bg_touch_callback() # turns off
tulip.bg_touch_del(12) # removes
```


## Network

Tulip CC has the capability to connect to a Wi-Fi network, and Python's native requests library will work to access TCP and UDP. We ship a few convenience functions to grab data from URLs as well. 

```python
# Join a wifi network (not needed on Tulip Desktop)
tulip.wifi("ssid", "password")

# Get IP address or check if connected
ip_address = tulip.ip() # returns None if not connected

# Save the contents of a URL to disk (needs wifi)
# Note: the screen will blank during this operation 
bytes_read = tulip.url_save("https://url", "filename.ext")

# Get the contents of a URL to memory (needs wifi, and be careful of RAM use)
content = tulip.url_get("https://url")

# Set the time from an NTP server (needs wifi)
tulip.set_time() 
```

## Async

We ship `asyncio` and also provide a simpler `tulip.defer()` callback to schedule code in the future.

```python
import asyncio
async def sleep(sec):
    await asnycio.sleep(sec)
    print("done")
asyncio.run(sleep(5))


def hello(t):
    print("hello called with arg %d" % (t))

tulip.defer(hello, 123, 1500) # will be called 1500ms later
```


## Music / sound

Tulip comes with the AMY synthesizer, a very full featured 120-oscillator synth that supports FM, PCM, subtractive and additive synthesis, partial synthesis, filters, and much more. See the [AMY documentation](https://github.com/bwhitman/amy/blob/main/README.md) for more information, Tulip's version of AMY comes with stereo sound, chorus and reverb. It includes a "small" version of the PCM patch set (29 patches) alongside all the Juno-6 and DX7 patches. It also has support for loading WAVE files in Tulip as samples. 

Once connected to Wi-Fi, Tulip can also control or respond to an [Alles mesh.](https://github.com/bwhitman/alles/blob/main/README.md) Alles is a wrapper around AMY that lets you control the synthesizer over Wi-Fi to remote speakers, or other computers or Tulips. Connect any number of Alles speakers to the wifi to have instant surround sound! See the Alles [getting started tutorial](https://github.com/bwhitman/alles/blob/main/getting-started.md) for more information and for more music examples.

![With Alles](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/nicoboard-alles.jpg)

```python

amy.drums() # plays a test song
amy.volume(4) # change volume
amy.reset() # stops all music / sounds playing
amy.send(voices='0', load_patch=129, note=45, vel=1) # plays a tone
amy.send(voices='0', pan=0) # set to the right channel
amy.send(voices='0', pan=1) # set to the left channel

# start mesh mode (control multiple speakers over wifi)
alles.mesh() # after turning on wifi
alles.mesh(local_node=False) # call it this way if you don't want Tulip to also be an Alles node
alles.mesh(local_ip='192.168.50.4') # useful for setting a network on Tulip Desktop

alles.map() # returns booted Alles synths on the mesh

amy.send(voices='0', load_patch=101, note=50, vel=1) # all Alles speakers in a mesh will respond
amy.send(voices='0', load_patch=101, note=50, vel=1, client=2) # just a certain client
alles.local() # turns off mesh mode and goes back to local mode
```

To load your own WAVE files as samples, use `tulip.load_sample`:

```python
# To save space / RAM, you may want to downsample your WAVE files to 22050Hz. We detect SR automatically.
patch = tulip.load_sample("flutea4.wav") # samples are converted to mono if they are stereo

# You can optionally tell us the loop start and end point (in samples), and base MIDI note of the sample.
patch = tulip.load_sample("flutea4.wav", midinote=81, loopstart=1020, loopend=1500)

# The patch number can now be used in the custom Tulip memory PCM sample player. 
# It has all the features of the AMY's PCM wave type.
amy.send(osc=20, wave=amy.CUSTOM, patch=patch, vel=1, note=50)

# You can load up to 32 custom PCM patches. Be careful of memory use. load_sample will return -1 if there's no more room.
# You can unload already allocated patches:
tulip.unload_patch(patch) # frees the RAM and the patch slot
tulip.unload_patch() # frees all allocated PCM patches
```

Tulip also ships with our own [`music.py`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/music.py), which lets you create chords, progressions and scales through code:

```python
import music
chord = music.Chord("F:min7")
for i,note in enumerate(chord.midinotes()):
    amy.send(wave=amy.SINE,osc=i*9,note=note,vel=0.25)
```

## Music sequencer

Tulip is always running a live sequencer, meant for music programs you write to share a common clock. This allows you to have multiple music programs running that respond to a callback to play notes. 

To use the clock in your code, you should first register on the music callback with `slot = tulip.seq_add_callback(my_callback)`. You can remove your callback with `tulip.seq_remove_callback(slot)`.  You can remove all callbacks with `tulip.seq_remove_callbacks()`. We support up to 8 callbacks running at once. 

When adding a callback, there's an optional second parameter to denote a divider on the system level parts-per-quarter timer (currently at 48). If you run `slot = tulip.seq_add_callback(my_callback, 6)`, it would call your function `my_callback` every 6th "tick", so 8 times a quarter note at a PPQ of 48. The default divider is 48, so if you don't set a divider, your callback will activate once a quarter note. 

By default, your callback will receive a message 50 milliseconds ahead of the time of the intended tick, with the parameters `my_callback(intended_time_ms)`. This is so that you can take extra CPU time to prepare to send messages at the precise time, using AMY scheduling commands, to keep in perfect sync. You can set this "lookahead" globally for all callbacks if you want more or less latency with `tulip.seq_latency(X)` or get it with `tulip.seq_latency()`. 

You can set the system-wide BPM (beats, or quarters per minute) with `tulip.seq_bpm(120)` or retrieve it with `tulip.seq_bpm()`. 

You can see what tick you are on with `tulip.seq_ticks()`. 

See the example `seq.py` on Tulip World for an example of using the music clock, or the [`drums`](https://github.com/bwhitman/tulipcc/blob/main/tulip/shared/py/drums.py) included app.

## MIDI

Tulip supports MIDI in and out to connect to external music hardware. You can set up a python callback to respond immediately to any incoming MIDI message. You can also send messages out to MIDI out. 

By default, Tulip boots into a live MIDI synthesizer mode. Any note-ons, note-offs, program changes or pitch bend messages will be processed automatically with polyphony and voice stealing, and Tulip will play the tones with no other user intervention needed.

By default, MIDI notes on channel 1 will map to Juno-6 patch 0. And MIDI notes on channel 10 will play the PCM samples (like a drum machine).

You can adjust which voices are sent with `tulip.music_map(channel, patch_number, voice_count)`. For example, you can have Tulip play DX7 patch 129 on channel 2 with `tulip.music_map(2, 129)`. The channel is a MIDI channel (we use 1-16 indexing), the patch_number is an AMY patch number, and voice_count is the optional number of voices (polyphony) you want to support for that channel and patch. 

(A good rule of thumb is Tulip CC can support about 8 simultaneous total voices for Juno-6 and DX7, and 20-30 total voices for PCM and more for other simpler oscillator patches.)

These mappings will get reset to default on boot. If you want to save them, put tulip.music_map() commands in your boot.py.

You can set up your own MIDI callbacks in your own programs. You can call `midi.add_callback(function)`, which will call your `function` with a list of a (2 or 3-byte) MIDI message. These callbacks will get called alongside the default MIDI callback (that plays synth notes on MIDI in). You can stop the default MIDI callback with `midi.stop_default_callback()` and start it again with `midi.start_default_callback()`. 

On Tulip Desktop, MIDI works on our iOS and macOS 11.0 (Big Sur, released 2020) and later ports using the "IAC" MIDI bus. (It does not yet work at all on Linux or Windows.) This lets you send and receive MIDI with Tulip to any program running on the same computer. If you don't see "IAC" in your MIDI programs' list of MIDI ports, enable it by opening Audio MIDI Setup, then showing MIDI Studio, double click on the "IAC Driver" icon, and ensure it is set to "Device is online." 

You can also send MIDI messages "locally", e.g. to a running program that is expecting hardware MIDI input, via `tulip.midi_local()`

```python
tulip.music_map(1,129) # change MIDI channel 1 to patch 129.

def callback():
    m = tulip.midi_in()
    if(m[0]==144):
        print("Note on, note # %d velocity # %d" % (m[1], m[2]))

midi.add_callback(callback)
midi.remove_callback(callback) # turns off callback

def callback(message):
    print(message[0]) # first byte of MIDI in message

midi.stop_default_callback() # turn on the default MIDI receiver
midi.start_default_callback() # turn on the default MIDI receiver

tulip.midi_out((144,60,127)) # sends a note on message
tulip.midi_out(bytes) # Can send bytes or list

tulip.midi_local((144, 60, 127)) # send note on to local bus
```


## Graphics system

The Tulip GPU consists of 3 subsystems, in drawing order:
 * A bitmap graphics plane (BG) (default size: 2048x750), with scrolling x- and y- speed registers. Drawing shape primitives and UI elements draw to the BG.
 * A text frame buffer (TFB) that draws 8x12 fixed width text on top of the BG, with 256 colors
 * A sprite layer on top of the TFB (which is on top of the BG). The sprite layer is fast, doesn't need to have a clear screen, is drawn per scanline, can draw bitmap color sprites as well as line buffers, stored in the same RAM. The line buffers are useful for fast wireframe drawing. 

The Tulip GPU runs at a fixed FPS depending on the resolution and display clock. You can change the display clock but will limit the amount of room for sprites and text tiles per line. The default for Tulip CC is 28Mhz, which is 30FPS. This is a great balance of speed and stability for text -- the editor and REPL. 

You can set a python callback for the frame done interrupt, for use in games or animations. 


```python
# returns current GPU usage computed on the last 100 frames, as a percentage of max
usage = tulip.gpu()

# returns current FPS, based on the display clock
fps = tulip.fps() 

# resets all 3 GPU systems back to their starting state, clears all BG and sprite ram and clears the TFB.
tulip.gpu_reset()

# Get or set the display clock in MHz. Current default is 18.
# Higher clocks mean smoother animations, but less time for the CPU to prepare things to draw
clock = tulip.display_clock() 
tulip.display_clock(mhz)

# Convenience function for getting the screen width and height,
# which are just the first two values returned by tulip.timing()
(WIDTH, HEIGHT) = tulip.screen_size()

# if the display clock gets in a strange state, you can restart it by just
tulip.display_restart() # does not clear any data like gpu_reset()

# You can also manually stop and start the display. This is useful if you want to do something intensive
# that requires the resources of the GPU as well as the CPU, or want to do faster disk access
tulip.display_stop() # Tulip will still run 
tulip.display_start()

# Sets a frame callback python function to run every frame 
# See the Game class below for an easier way to make games
game_data = {"frame_count": 0, "score": 0}
def game_loop(data):
    update_inputs(data)
    check_collisions(data)
    do_animation(data)
    update_score(data) # etc
    data["frame_count"] += 1

tulip.frame_callback(game_loop, game_data) # starts calling the callback every frame
tulip.frame_callback() # disables the callback

# Sets the screen brightness, from 1-9 (9 max brightness.) 5 is default.
tulip.brightness(5)

# Show the GPU usage (frames per second, time spent in GPU) at the next GPU epoch (100 frames) in stderr 
tulip.gpu_log()
```

## Graphics background plane

The default background plane (BG) is 2048 x 750, with the visible portion 1024x600. (You can change this with `tulip.timing()`.) Use the extra for double buffering, hardware scrolling or for storing bitmap data "offscreen" for later blitting (you can treat it as fixed bitmap RAM.) The BG is drawn first, with the TFB and sprite layers drawn on top.

The UI operations (LVGL or anything in `tulip.UI`) also draw to the BG. Be careful if you're using both BG drawing operations and LVGL as they may draw on top of one another.

Tulip uses RGB332, with 256 colors. Here's the palette: 

![tulip_pal](https://github.com/bwhitman/tulipcc/blob/main/docs/pics/rgb332.png?raw=true)


```python
# Set or get a pixel on the BG
pal_idx = tulip.bg_pixel(x,y)
tulip.bg_pixel(x,y,pal_idx)  # pal_idx is 0-255 for 8-bit RGB332 mode 

# Convert between packed palette color and r,g,b
pal_idx = tulip.color(r,g,b)
(r,g,b) = tulip.rgb(pal_idx)

# Set the contents of a PNG file on the background.
# To save RAM and disk space, I recommend converting your PNG to 255 colors before moving to Tulip
# Imagemagick does this with: convert input.png -colors 255 output.png 
# Or with dithering: convert input.png -dither FloydSteinberg -colors 255 output.png
png_file_contents = open("file.png", "rb").read()
tulip.bg_png(png_file_contents, x, y)
# Or use the png filename directly 
tulip.bg_png(png_filename, x, y)

# Copy bitmap area from x,y of width,height to x1, y1
tulip.bg_blit(x,y,w,h,x1, y1)

# If you give blit an extra parameter it will not copy over alpha color (0x55), good for blending BG images
tulip.bg_blit(x,y,w,h,x1, y1, 1)

# Sets or gets a rect of the BG with bitmap data (RGB332 pal_idxes) 
tulip.bg_bitmap(x, y, w, h, bitmap) 
bitmap = tulip.bg_bitmap(x, y, w, h)

# Clear the BG with a color or default
tulip.bg_clear(pal_idx)
tulip.bg_clear() # uses default

# Drawing primitives. These all write to the BG. 
# If you want to use them for sprites, you can use bg_bitmap after drawing offscreen.
# set filled to 1 if you want the shape filled, 0 or omit otherwise
tulip.bg_line(x0,y0, x1,y1, pal_idx)
tulip.bg_bezier(x0,y0, x1,y1, x2,y2, pal_idx)
tulip.bg_circle(x,y,r, pal_idx, filled) # x and y are the center
tulip.bg_roundrect(x,y, w,h, r, pal_idx, filled)
tulip.bg_rect(x,y, w,h, pal_idx, filled)
tulip.bg_triangle(x0,y0, x1,y1, x2,y2, pal_idx, filled)
tulip.bg_fill(x,y,pal_idx) # Flood fill starting at x,y
tulip.bg_str(string, x, y, pal_idx, font) # same as char, but with a string. x and y are the bottom left
tulip.bg_str(string, x, y, pal_idx, font, w, h) # Will center the text inside w,h

"""
  Set scrolling registers for the BG. 
  line is visible line number (0-599). 
  x_offset sets x position pixels to offset for that line (default is 0)
  y_offset sets y position pixels to offset for that line (default is the line number)
  x_speed is how many pixels a frame to add to x_offset (default is 0)
  y_speed is how many pixels a frame to add to y_offset (default is 0)

  For example, to scroll the BG up two pixels a frame
  for i in range(600):
    tulip.bg_scroll(i, 0, i, 0, -2) 

"""
tulip.bg_scroll(line, x_offset, y_offset, x_speed, y_speed)
tulip.bg_scroll() # resets to default

# Change individual registers
tulip.bg_scroll_x_speed(line, x_speed)
tulip.bg_scroll_y_speed(line, y_speed)
tulip.bg_scroll_x_offset(line, x_offset)
tulip.bg_scroll_y_offset(line, y_offset)

# "Swap" the visible BG with the one to its right, using the scrolling registers
# This would make 1024,0 the top left BG pixel after the first call to swap, and 0,0 after the second call to swap
tulip.bg_swap()
```

We currently ship some fonts with Tulip to use for the BG. These are aside from the fonts that come with LVGL for the UI. You can see them all by running `fonts.py`, which also shows how to address them:

![IMG_3339](https://user-images.githubusercontent.com/76612/229381546-46ec4c50-4c4a-4f3a-9aec-c77d439081b2.jpeg)



## Text frame buffer (TFB)

The TFB is a 128x50 character plane for fast drawing of 8x12 text glyphs. It supports 256 ANSI 
colors for foreground and background, and supports formatting. TFB is used by the text 
editor and Python REPL.

```python
# Sets a string / gets a character and/or format to the text frame buffer (TFB), 128x50 
# Format has ANSI codes for reverse (0x80), underline (0x40), flash (0x20), bold (0x10)
# fg color is palette index, 0-255, same for bg color
# Note that the REPL and editor use the TFB
tulip.tfb_str(x,y, "string", [format], [fg], [bg])
(char, format, fg, bg) = tulip.tfb_str(x,y)

# ANSI color and formatting codes have convenience functions
print(tulip.Colors.LIGHT_RED + "this is red " + tulip.Colors.GREEN + tulip.Colors.INVERSE + " and then green inverse")
# To reset ANSI formatting
print(tulip.Colors.DEFAULT)

# Tulip REPL supports ANSI 256 color modes as well
print(tulip.ansi_fg(56))

# You can also stop or start the TFB. It will maintain what is on screen in memory, and you can still read/write it
tulip.tfb_stop()
tulip.tfb_start()

# If you want to keep the existing TFB around, you can save it to a temporary buffer and recall it
# This is used in the Game() class to keep the REPL around after running a game, and the editor
tulip.tfb_save()
tulip.tfb_restore()

# You can copy what the TFB prints to your local terminal or ESP monitor for debugging
# On Tulip CC you can also type into the ESP monitor, but not yet on Tulip Desktop 
tulip.tfb_log_start() # starts copying TFB to local stderr
tulip.tfb_log_stop() # stops
```


## Sprites
You can have up to 32 bitmap sprites on screen at once, and have 32KB of bitmap data to store them in. Sprites have collision detection built in.
Sprites are drawn in order of sprite index, so sprite index 5 will draw on top of sprite index 3 if they share pixel space.

```python
# Load the data from a PNG file into sprite RAM at the memory position (0-32767). 
# Returns w, h, and number of bytes used
# Alpha is used if given
(w, h, bytes) = tulip.sprite_png(png_data, mem_pos)
(w, h, bytes) = tulip.sprite_png("filename.png", mem_pos)

# Or load sprites in from a bitmap in memory (packed pallete indexes for RGB332)
# The bitmap can be made from code you wrote, or from bg_bitmap to sample the background
# Use pal idx 0x55 to denote alpha when generating your own sprites 
bytes = tulip.sprite_bitmap(bitmap, mem_pos)

# Read bitmap data from sprite ram if you need to modify sprites or copy them to BG
bitmap = tulip.sprite_bitmap(mem_pos, length)

# "Register" a piece of sprite RAM into a sprite handle index for later use.
# Creates sprite handle #12 referencing sprite data starting at mem_pos and w,h pixels
tulip.sprite_register(12, mem_pos, w, h)  

# Turn on a sprite to draw on screen
tulip.sprite_on(12)

# And off
tulip.sprite_off(12)

# Set a sprite x and y position
tulip.sprite_move(12, x, y)

# Every frame, we update a collision list of things that collided that frame
# Collisions are evaluated every scanline (left to right and top to bottom), 
# and only on pixels that are written to the screen (not ALPHA, and must be visible)
# See world.download("collide") for an example
# Calling collisions() clears the memory of collisions we've kept up to that point. 
for c in tulip.collisions():
    (a,b) = c # a and b are sprite #s that collided. a will always < b. 
    # Check if a touch or mouse click hit a sprite by looking for sprite #31
    if(b==31): 
        print("Touch/click on sprite %d" % (a))

# Clear all sprite RAM, reset all sprite handles
tulip.sprite_clear()
```

## Wireframes and fast line drawing

![Line drawing](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/lines.png)


You can also use the sprite RAM to also draw lists of lines. You can store lists of `x0,y0,x1,y1` in sprite RAM and register the sprite, Tulip will draw those lines every frame as the scanlines get drawn, on top of the BG and TFB like sprites. This lets you do fast wireframe-like animations without having to draw to the BG and clear it every frame. 

You can also load 3D models as wireframes in from standard `obj` files, and set their rotation and scale, which will render a list of line positions for you to sprite line list RAM. 


```python
# Load an obj file into a list of raw faces and vertices - unscaled and unrotated.
model = tulip.wire_load("teapot.obj")
# A model encodes vertices and faces of a 3d model. You can also generate this model in code yourself.  

# Draw model wireframe to a line buffer
lines = tulip.wire_to_lines(model, x, y, scale, theta, max_faces)
# scale = integer multiplier on 0..1 coordinates. in general, sets width/height of model as pixels
# theta = # of 100.0/PI rotations
# max_faces = optional, cuts the model off at a number of faces (3 lines). if not given uses all faces in model

# You can also generate line lists yourself in code.
lines = tulip.lines([
    [x0_0,y0_0,x1_0,y1_0],
    [x0_1,y0_1,x1_1,y1_1]
    ]) # will return packed buffer of lines, sorted, including the last sentinel line

# However you got your lines buffer, you can now load it into sprite RAM at whatever position you want. 
# See len(lines) to see how many bytes your line list takes up so you don't overwrite other data. 
tulip.sprite_bitmap(lines, mem_pos)

# Register this "sprite" as a line buffer. If you don't pass w and h like a normal sprite, we assume it's a wireframe and we turn it on right away
tulip.sprite_register(12, mem_pos)

# Whenever you want to rotate, scale or translate the wireframe, regenerate the lines and write them to sprite RAM
# The screen will immediately start drawing the new data once you call sprite_bitmap if the sprite is registered.
lines = tulip.wire_to_lines(model, x, y, scale, theta)
tulip.sprite_bitmap(lines, mem_pos)

# Or, update your line coordinates directly:
tulip.sprite_bitmap(tulip.lines(new_lines_list), mem_pos)

tulip.sprite_off(12) # turn off drawing
```


## Convenience classes for sprites and games

We provide a few classes to make it easier to make games and work with sprites, `Sprite`, `Player`, and `Game`.

```python
def game_loop(game):
    # game.X available

# make your game a subclass of Game. This will set up the display, reset the sprites, etc, and provide a quit()
class MyGame(Game):
    def __init__(self):
        # debug=True will keep the TFB on the screen and not remove sprites/BG after finishing
        super().init(debug=False)
        # .. game setup stuff
        tulip.frame_callback(game_loop, self)

game = MyGame()
try:
    while game.run:
        time.sleep_ms(100)
except KeyboardInterrupt:
    game.quit() # will clean up 

```

You can access sprites using the `tulip_sprite_X` commands, or use our convenience `Sprite` class to manage memory and IDs for you:

```python
class Bullet(tulip.Sprite):
    def __init__(self, copy_from=None):
        super().__init__(copy_from=copy_from)
        self.load("bullet.png", 32, 32)
        b.on()
        b.move_to(20,20)

b = Bullet()
b.off() # turn off
b.on() # turns on
b.x = 1025
b.clamp() # ensure it is within screen range
b.move() # move to its most recent position
b.move_to(x,y) # sets x and y and moves

b2 = Bullet(copy_from=b) # will use the image data from b but make a new sprite handle
b2.move_to(25,25) # Can have many sprites of the same image data on screen this way
```

A `Player` class comes with a quick way to move a sprite from the keyboard:

```python
p = tulip.Player(speed=5) # 5px per movement
p.load("me.png", 32, 32)
p.joy_move() # will update the position based on the joystick
```

See `world.download('planet_boing')` for a fleshed out example of using the `Game` and `Sprite` classes.

# Can you help? 

Things we've thought of we'd love your help on:

 * Sprite editor in Tulip
 * Tile / Map editor in Tulip

Any questions? [Chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)











