# Tulip Creative Computer - a portable Python device for music, graphics and writing


![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/flat_tiles.jpg)

Welcome to the Tulip Creative Computer (aka Tulip, aka Tulip CC.) 

Tulip is a self contained portable creative computer, with a display and keyboard and sound. It boots instantaneously into a Python prompt. Dive right into making something without distractions or complications. 

Tulip is not a shell on top of another operating system. The entire system is dedicated to your code, the display and sound, running in real time, on specialized hardware. 

You can use Tulip to make music, code, art, games, or just write. It's a great environment for live coding performances. It doesn't run any existing applications and does not have a web browser, although it can connect to a network in a slow fashion.

 * [**See the full Tulip API**](docs/tulip_api.md)
 * [**Build your own Tulip**](docs/tulip_build.md)
 * **[You can read more about the "why" or "how" of Tulip on my website!](https://notes.variogram.com/2023/02/14/tulip/)** 
 * Any questions? [Chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)

Tulip is available both as a [hardware DIY project (Tulip CC)](docs/tulip_build.md) and a [desktop simulator app (Tulip Desktop.)](docs/tulip_desktop.md) They both have all the same features. I use Tulip Desktop when developing Tulip, but use the hardware Tulip CC when making things! If you're nervous about building the hardware, [download the desktop version!](docs/tulip_desktop.md)

[![Tulip Desktop](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/desktop.png)](docs/tulip_desktop.md)

You can [build your own Tulip CC](docs/tulip_build.md) for about $25 plus the cost of a display ($50) and USB keyboard. The hardware for revision 4 of Tulip CC is based on the ESP32-S3 dual core microcontroller running at 240MHz. This single inexpensive chip can support all of Tulip's functionality at low power use. It can last on any USB battery pack or LiPo battery for many hours. The display we use is a 10.1" 1024 x 600 RGB dot clock color LCD with capacative touch support. Tulip's sound system is a full featured 64-voice synthesizer with a stereo line-out / headphone jack. You can use speakers or other connectors instead.

Tulip CC rev 4 supports:
- 8.5MB of RAM
- 8MB or 32MB flash storage, as a filesystem accesible in Python
- An [AMY](https://github.com/bwhitman/amy) stereo 64-voice synthesizer engine running locally or as a wireless controller for an [Alles](https://github.com/bwhitman/alles) mesh. Tulip's synth supports additive oscillators, an excellent FM synthesis engine, samplers, karplus-strong, filters, and much more. 
- Text frame buffer layer, 128 x 50, with ANSI support for 256 colors, inverse, bold, underline, background color
- Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
- A 1280x750 background frame buffer to draw arbitrary bitmaps to, or use as RAM, and which can scroll horizontally / vertically
- WiFi, access http via Python requests or TCP / UDP sockets 
- Adjustable display clock and resolution, defaults to 30 FPS at 1024x600.
- 256 colors
- Can load PNGs from disk to set sprites or background, or generate bitmap data from code
- Built in code and text editor
- Built in BBS chat room called **T U L I P ~ W O R L D**
- USB keyboard support
- Capactive multi-touch support (mouse on Tulip Desktop)
- MIDI input and output 
- Joystick support for NES/SNES style gamepads (or any connected gamepad on Tulip Desktop)
- 575mA power usage @ 5V including display, at medium display brightness, can last for hours on LiPo, 18650s, or USB battery pack 

I've been working on Tulip on and off for years over many hardware iterations and hope that someone out there finds it as fun as I have, either making things with Tulip or working on Tulip itself. I'd love feedback, your own Tulip experiments or pull requests to improve the system.

![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4.png)


## Getting started

Once you've [built a Tulip](docs/tulip_build.md) or [installed Tulip Desktop](docs/tulip_desktop.md), you'll see that Tulip boots right into a Python prompt and all interaction with the system happens there. You can make your own Python programs with Tulip's built in editor and execute them, or just experiment on the Tulip REPL prompt in real time.

[**See the full Tulip API**](docs/tulip_api.md) for more details on all the graphics, sound and input functions.

Below are a few getting started tips and small examples. The [full API](docs/tulip_api.md) page has more detail on everything you can do on a Tulip.

Any big challenges getting this going? Please [chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)


```python
# Run a saved Python file. Control-C stops it
cd('ex') # The ex folder has a few examples and graphics in it
execfile("parallax.py")
```

### The Tulip Editor

Tulip ships with a text editor, based on pico/nano. It supports syntax highlighting, search, save/save-as. 

```python
# Opens the Tulip editor to the given filename. 
edit("game.py")
```

![Editor](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/editor.png)


### Input and user interface

Tulip supports USB keyboard input and touch input. (On Tulip Desktop, mouse clicks act as touch points.) It also comes with UI elements like buttons and sliders to use in your applications. More in the [full API](docs/tulip_api.md).

```python
tulip.ui_callback(ui_callback)
tulip.button(ui_element_id, "Button text", x, y, w, h, radius, bg_pal_idx, fg_pal_idx, filled)
tulip.slider(ui_element_id, x, y, w, h, orientation, default_value, bg_pal_idx, handle_pal_idx)

(x0, y0, x1, y1, x2, y2) = tulip.touch()
```

![UI demo](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/sliders.png)


### Network

Tulip CC has the capability to connect to a Wi-Fi network, and Python's native requests library will work to access TCP and UDP. We ship a few convenience functions to grab data from URLs as well. More in the [full API](docs/tulip_api.md).

```python
# Join a wifi network (not needed on Tulip Desktop)
tulip.wifi("ssid", "password")

# Get IP address or check if connected
ip_address = tulip.ip() # returns None if not connected

# Save the contents of a URL to disk (needs wifi)
bytes_read = tulip.url_save("https://url", "filename.ext")
```


### Music / sound

Tulip comes with the Alles synthesizer, a very full featured 64-oscillator synth that supports FM, PCM, additive synthesis, partial synthesis, filters, and much more. We also provide a useful "music computer" for scales, chords and progressions. More in the [full API](docs/tulip_api.md).

```python
alles.drums() # plays a test song
alles.volume(4) # change volume
alles.reset() # stops all music / sounds playing
```

https://user-images.githubusercontent.com/76612/215893940-658144b7-0c6f-42e2-9836-bd271597aab3.mov


### MIDI

Tulip supports MIDI in and out to connect to external music hardware. You can set up a python callback to respond immediately to any incoming MIDI message. You can also send messages out to MIDI out. More in the [full API](docs/tulip_api.md).

```python
m = tulip.midi_in() # returns bytes of the last MIDI message received
tulip.midi_out((144,60,127)) # sends a note on message
tulip.midi_out(bytes) # Can send bytes or list
```

### Graphics system

The Tulip GPU supports a scrolling background layer, hardware sprites and a text layer. Much more in the [full API](docs/tulip_api.md).

```python
# Set or get a pixel on the BG
pal_idx = tulip.bg_pixel(x,y)

# Set the contents of a PNG file on the background.
tulip.bg_png(png_filename, x, y)

tulip.bg_scroll(line, x_offset, y_offset, x_speed, y_speed)
```

https://user-images.githubusercontent.com/76612/215895305-7b02ad27-b02a-429a-92ef-f13136e9f9d2.mov


Hardware sprites are supported. They draw over the background and text layer per scanline per frame:

```python
(w, h, bytes) = tulip.sprite_png("filename.png", mem_pos)

...

# Set a sprite x and y position
tulip.sprite_move(12, x, y)
```

https://user-images.githubusercontent.com/76612/215896311-fc0823aa-44bc-4305-85db-a6773db11a98.mov


### Tulip World

Still very much early days, but Tulip supports a native chat room called **T U L I P ~ W O R L D** where you can hang out with other Tulip owners. We're hoping to build this out to eventually support file transfer and other BBS-style features. But for now, you're able to pull down the latest messages and send messages yourself. 

```python
tulip.world() # opens the Tulip World UI
```

## How to build, compile and help develop Tulip

 * [See our new page that describes how to build your own Tulip Creative Computer](docs/tulip_build.md) with FOUR different options.
 * [How to compile and flash Tulip hardware](docs/tulip_flashing.md)
 * [How to run or compile Tulip Desktop](docs/tulip_desktop.md)
 * [The full Tulip API](docs/tulip_api.md)
 * Please [chat with us on our discussions page.](https://github.com/bwhitman/tulipcc/discussions)
 * [File any code issues or pull requests!](https://github.com/bwhitman/tulipcc/issues)

Some development guidelines if you'd like to help contribute!

 * Be nice and helpful and don't be afraid to ask questions! We're all doing this for fun and to learn. 
 * We're currently a hard fork of ESP-IDF, Alles/AMY and [Micropython](https://github.com/bwhitman/tulipcc/discussions/9); we aim to have no external libraries. Keep everything needed to build for both Tulip Desktop and CC in this repository when possible. While we work on early versions of Tulip, we can merge new features of libraries manually. 
 * Any change or feature must be equivalent across Tulip Desktop and Tulip CC. There are of course limited exceptions to this rule, but please test on hardware before proposing a new feature / change. 

Have fun!

![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4.png)








