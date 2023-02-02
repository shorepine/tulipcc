# Tulip Creative Computer


![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/flat_tiles.jpg)

Welcome to the Tulip Creative Computer (aka Tulip, aka Tulip CC.) 

Tulip is a self contained portable creative computer, with a display and keyboard and sound. It boots instantaneously into a Python prompt. Dive right into making something without distractions or complications. 

Tulip is not a shell on top of another operating system. The entire system is dedicated to your code, the display and sound, running in real time, on specialized hardware. 

You can use Tulip to make music, code, art, games, or just write. It's a great environment for live coding performances. It doesn't run any existing applications and does not have a web browser, although it can connect to a network in a slow fashion.

Tulip is available both as a [hardware DIY project (Tulip CC)](https://github.com/bwhitman/tulipcc#tulip-hardware-howtos) and a [desktop simulator app (Tulip Desktop.)](docs/tulip_desktop.md) They both have all the same features. I use Tulip Desktop when developing Tulip, but use the hardware Tulip CC when making things! If you're nervous about building the hardware, [download the desktop version!](docs/tulip_desktop.md)

[![Tulip Desktop](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/desktop.png)](docs/tulip_desktop.md)

You can [build your own Tulip CC](https://github.com/bwhitman/tulipcc#tulip-hardware-howtos) for about $25 plus the cost of a display ($50) and USB keyboard. The hardware for revision 4 of Tulip CC is based on the ESP32-S3 dual core microcontroller running at 240MHz. This single inexpensive chip can support all of Tulip's functionality at low power use. It can last on any USB battery pack or LiPo battery for many hours. The display we use is a 10.1" 1024 x 600 RGB dot clock color LCD with capacative touch support. Tulip's sound system is a full featured 32-voice synthesizer with a stereo line-out / headphone jack. You can use speakers or other connectors instead.

Tulip CC rev 4 supports:
- 8.5MB of RAM
- 8MB or 32MB flash storage, as a filesystem accesible in Python
- An [AMY](https://github.com/bwhitman/amy) 32-voice synthesizer engine running locally or as a wireless controller for an [Alles](https://github.com/bwhitman/alles) mesh. Tulip's synth supports additive oscillators, an excellent FM synthesis engine, samplers, karplus-strong, filters, and much more. 
- Text frame buffer layer, 128 x 50, with ANSI support for 256 colors, inverse, bold, underline, background color
- Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
- A 1280x750 background frame buffer to draw arbitrary bitmaps to, or use as RAM, and which can scroll horizontally / vertically
- WiFi, access http via Python requests or TCP / UDP sockets 
- Adjustable display clock and resolution, defaults to 30 FPS at 1024x600.
- 256 colors
- Can load PNGs from disk to set sprites or background, or generate bitmap data from code
- Built in code and text editor
- USB keyboard support
- Capactive multi-touch support (mouse on Tulip Desktop)
- MIDI input and output 
- 575mA power usage @ 5V including display, at medium display brightness, can last for hours on LiPo, 18650s, or USB battery pack 

I've been working on Tulip on and off for years over many hardware iterations and hope that someone out there finds it as fun as I have, either making things with Tulip or working on Tulip itself. I'd love feedback, your own Tulip experiments or pull requests to improve the system.

![Tulip](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4.png)


## Usage and the Tulip API 

Tulip boots right into a Python prompt and all interaction with the system happens there. You can make your own Python programs with Tulip's built in editor and execute them, or just experiment on the Tulip REPL prompt in real time.

### General

```python
# Interact with the filesystem.
# Supported: ls, head, cat, newfile, cp, mv, rm, pwd, cd, mkdir, rmdir
ls
mkdir('directory')
cd('directory')

# Clears the REPL screen and formatting
clear

# Run a saved Python file. Control-C stops it
cd('ex') # The ex folder has a few examples and graphics in it
execfile("parallax.py")

# If you want something to run when Tulip boots, add it to boot.py
edit("boot.py")

# Takes a screenshot and saves to disk. The screen will blank for a moment
# If no filename given will upload to imgur and return a URL (needs wifi)
tulip.screenshot("screenshot.png")
imgur_url = tulip.screenshot()

# Return the current CPU usage (% of time spent on CPU tasks like Python code, sound, some display)
# Note that Python code is bound to one core, so Python-only usage tops out at 50%.
usage = tulip.cpu() # or use tulip.cpu(1) to show more detail in a connected UART
```

### The Tulip Editor

Tulip ships with a text editor, based on pico/nano. It supports syntax highlighting, search, save/save-as. 

```python
# Opens the Tulip editor to the given filename. 
# Control-X exits and prompts to save if any changes. 
# Control-O is save as
# Control-W searches
# Filename will be created on save if it doesn't exist.
edit("game.py")
```

![Editor](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/editor.png)


### Input and user interface

Tulip supports USB keyboard input and touch input. (On Tulip Desktop, mouse clicks act as touch points.) It also comes with UI elements like buttons and sliders to use in your applications.

```python
# Create a callback to activate when UI elements are triggered
def ui_callback(x):
    # x is the element ID that was triggered
    print("Item %d was fired" % (x))
    if(x==1): # slider
        print("Slider value is now %f" % (tulip.slider(1)))

tulip.ui_callback(ui_callback)
tulip.button(ui_element_id, "Button text", x, y, w, h, radius, bg_pal_idx, fg_pal_idx, filled)
tulip.slider(ui_element_id, x, y, w, h, orientation, default_value, bg_pal_idx, handle_pal_idx)

# Returns the current held keyboard scan codes, up to 6 and the modifier mask (ctrl, shift etc)
(modifiers, scan0, scan1... scan5) = tulip.keys()

# Gets a key ascii code
ch = tulip.key_wait() # waits for a key press 
ch = tulip.key() # returns immediately, returns -1 if nothing held

# If scanning key codes in a program, you may want to turn on "key scan" mode so that
# keys are not sent to the underlying python process 
tulip.key_scan(1)
tulip.key_scan(0) # remember to turn it back off or you won't be able to type into the REPL

# Return the last touch panel coordinates, up to 3 fingers at once
(x0, y0, x1, y1, x2, y2) = tulip.touch()
```

![UI demo](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/sliders.png)


### Network

Tulip CC has the capability to connect to a Wi-Fi network, and Python's native requests library will work to access TCP and UDP. We ship a few convenience functions to grab data from URLs as well. 

```python
# Join a wifi network (not needed on Tulip Desktop)
tulip.wifi("ssid", "password")

# Get IP address or check if connected
ip_address = tulip.ip() # returns None if not connected

# Save the contents of a URL to disk (needs wifi)
bytes_read = tulip.url_save("https://url", "filename.ext")

# Get the contents of a URL to memory (needs wifi, and be careful of RAM use)
content = tulip.url_get("https://url")

# Set the time from an NTP server (needs wifi)
tulip.set_time() 
```


### Music / sound

Tulip comes with the Alles synthesizer, a very full featured 32-oscillator synth that supports FM, PCM, additive synthesis, partial synthesis, filters, and much more. See the [Alles documentation](https://github.com/bwhitman/alles/blob/main/README.md) for more information.

Once connected to Wi-Fi, Tulip can also control or respond to an [Alles mesh](https://github.com/bwhitman/alles/blob/main/README.md). Connect any number of Alles speakers to the wifi to have instant surround sound! 

See the Alles [getting started tutorial](https://github.com/bwhitman/alles/blob/main/getting-started.md) for more information and for more music examples.

```python
alles.drums() # plays a test song
alles.volume(4) # change volume
alles.reset() # stops all music / sounds playing
alles.send(osc=0, wave=alles.ALGO, patch=4, note=45, vel=1) # plays a tone

# start mesh mode (control multiple speakers over wifi)
alles.mesh() # after turning on wifi
alles.send(osc=1, wave=alles.ALGO, patch=101, note=50, ratio=0.1, vel=1) # all Alles speakers in a mesh will respond
alles.send(osc=1, wave=alles.ALGO, patch=101, note=50, ratio=0.1, vel=1, client=2) # just a certain client
alles.local() # turns off mesh mode and goes back to local mode
```

Tulip also ships with our own [`music.py`](https://github.com/bwhitman/tulipcc/blob/main/extmod/tulip/py/music.py), which lets you create chords, progressions and scales through code:

```python
import music
chord = music.Chord("F:min7")
for i,note in enumerate(chord.midinotes()):
  alles.send(wave=alles.ALGO,osc=i*9,note=note,vel=0.25,patch=101,ratio=0.1)
```

https://user-images.githubusercontent.com/76612/215893940-658144b7-0c6f-42e2-9836-bd271597aab3.mov


### MIDI

Tulip supports MIDI in and out to connect to external music hardware. You can set up a python callback to respond immediately to any incoming MIDI message. You can also send messages out to MIDI out. 

On Tulip Desktop, MIDI works on macOS 11.0 (Big Sur, released 2020) and later using the "IAC" MIDI bus. This lets you send and receive MIDI with Tulip to any program running on the same computer. If you don't see "IAC" in your MIDI programs' list of MIDI ports, enable it by opening Audio MIDI Setup, then showing MIDI Studio, double click on the "IAC Driver" icon, and ensure it is set to "Device is online." At this time, MIDI will not function (but the rest of Tulip will run fine) on macOS versions before 11.0.

```python
def callback(x): # for now you have to define a callback with a dummy parameter
    m = tulip.midi_in()
    if(m[0]==144):
        print("Note on, note # %d velocity # %d" % (m[1], m[2]))

tulip.midi_callback(callback)
tulip.midi_callback() # turns off callbacks

m = tulip.midi_in() # returns bytes of the last MIDI message received
tulip.midi_out((144,60,127)) # sends a note on message
tulip.midi_out(bytes) # Can send bytes or list
```


### Graphics system

The Tulip GPU consists of 3 subsystems, in drawing order:
 * A bitmap graphics plane (BG) (default size: 1280x750), with scrolling x- and y- speed registers. Drawing shape primitives draw to the BG.
 * A text frame buffer (TFB) that draws 8x12 fixed width text on top of the BG, with 256 colors
 * A sprite layer on top of the TFB (which is on top of the BG)

The Tulip GPU runs at a fixed FPS depending on the resolution and display clock. You can change the display clock but will limit the amount of room for sprites and text tiles per line. 

Some example display clocks and resolutions:

| H_RES   | V_RES   | Clock  |  FPS   | - | H_RES   | V_RES   | Clock  |  FPS   |
| ------- | ------- | ------ | ------ | --| ------- | ------- | ------ | ------ |
| 1024    | 600     | 10     | 14.98  | |512     | 600     | 10     | 19.91  |
| 1024    | 600     | 14     | 18.55  | |512     | 600     | 14     | 30.26  |
| 1024    | 600     | 18     | 23.19  | |512     | 600     | 18     | 37.82  |
| **1024** | **600**| **22** | 30.91  | |512     | 600     | 22     | 50.43  |
| 1024    | 600     | 28     | 46.37  | |512     | 600     | 28     | 75.65  |
| 1024    | 300     | 10     | 21.47  | |512     | 300     | 10     | 35.03  |
| 1024    | 300     | 14     | 34.36  | |512     | 300     | 14     | 56.05  |
| 1024    | 300     | 18     | 42.95  | |512     | 300     | 18     | 70.07  |
| 1024    | 300     | 22     | 57.26  | |512     | 300     | 22     | 93.45  |
| 1024    | 300     | 28     | 85.90  | |512     | 300     | 28     | 140.13 |

You can set a python callback for the frame done interrupt, for use in games or animations. 

```python
# returns current GPU usage computed on the last 100 frames, as a percentage of max
usage = tulip.gpu()

# returns current FPS, based on the display clock
fps = tulip.fps() 

# resets all 3 GPU systems back to their starting state, clears all BG and sprite ram and clears the TFB.
tulip.gpu_reset()

# Get or set the display clock in MHz. Current default is 22.
# Higher clocks mean smoother animations, but less time for the CPU to prepare things to draw
clock = tulip.display_clock() 
tulip.display_clock(mhz)

# You can also change the timing and resolution on the fly. 
# This is helpful for getting higher FPS with lower resolution (less pixels)
(h_res, v_res, h_offscreen_px, v_offscreen_px, 
  hsync_back_porch, hsync_front_porch, hsync_pulse_width, 
  vsync_back_porch, vsync_front_porch, vsync_pulse_width) = tulip.timing() # returns current
tulip.timing(1024, 600, 256, 150, 139, 140, 20, 20, 12, 20) # sets, will erase all display RAM

# if the display clock gets in a strange state, you can restart it by just
tulip.display_restart() # does not clear any data like gpu_reset()

# Sets a frame callback python function to run every frame 
game_data = {"frame_count": 0, "score": 0}
def game_loop(data):
	update_inputs(data)
	check_collisions(data)
	do_animation(data)
	update_score(data) # etc
	data["frame_count"] += 1

tulip.frame_callback(game_loop, game_data) # starts calling the callback every frame
tulip.frame_callback() # disables the callback

# Sets the screen brightness, from 1-9 (9 max brightness). 5 is default.
tulip.brightness(5)

```

### Graphics background plane

The default background plane (BG) is 1280 x 750, with the visible portion 1024x600. (You can change this with `tulip.timing()`.) Use the extra for hardware scrolling or for storing bitmap data "offscreen" for later blitting (you can treat it as fixed bitmap RAM.) The BG is drawn first, with the TFB and sprite layers drawn on top.

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
tulip.blit(x,y,w,h,x1, y1)

# Sets or gets a rect of the BG with bitmap data (RGB332 pal_idxes) 
tulip.bg_bitmap(x, y, w, h, bitmap) 
bitmap = tulip.bg_bitmap(x, y, w, h)

# Clear the BG with a color or default
tulip.bg_clear(pal_idx)
tulip.bg_clear() # uses default

# Drawing primitives. These all write to the BG. 
# If you want to use them for sprites, you can use bg_bitmap after drawing offscreen.
# set filled to 1 if you want the shape filled, 0 or omit otherwise
tulip.line(x0,y0, x1,y1, pal_idx)
tulip.bezier(x0,y0, x1,y1, x2,y2, pal_idx)
tulip.circle(x,y,r, pal_idx, filled)
tulip.roundrect(x0,y0, w,h, r, pal_idx, filled)
tulip.rect(x0,y0, w,h, pal_idx, filled)
tulip.triangle(x0,y0, x1,y1, x2,y2, pal_idx, filled)
tulip.fill(x0,y0,pal_idx) # Flood fill starting at x,y
tulip.char(c, x, y, pal_idx) # proportional font, returns # of x pixels to advance for the next char
tulip.str(string, x, y, pal_idx) # same as char, but with a string

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
```



https://user-images.githubusercontent.com/76612/215895305-7b02ad27-b02a-429a-92ef-f13136e9f9d2.mov



### Text frame buffer (TFB)

The TFB is a 128x50 character plane for fast drawing of text glyphs. It supports 256 ANSI 
colors for foreground and background, and supports formatting. TFB is used by the text 
editor and Python REPL.

```python
# Sets a string / gets a character and/or format to the text frame buffer (TFB), 128x50 
# Format has ANSI codes for reverse (0x80), underline (0x40), flash (0x20), bold (0x10)
# fg color is palette index, 0-255, same for bg color
# Note that the REPL and editor use the TFB
tulip.tfb_char(x,y, "string", [format], [fg], [bg])
(char, format, fg, bg) = tulip.tfb_str(x,y)

# ANSI color and formatting codes have convenience functions
print(tulip.Colors.LIGHT_RED + "this is red " + tulip.Colors.GREEN + tulip.Colors.INVERSE + " and then green inverse")
# To reset ANSI formatting
print(tulip.Colors.DEFAULT)

# Tulip REPL supports ANSI 256 color modes as well
print(tulip.ansi_fg(56))
```

![TFB](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tfb.png)


### Sprites
You can have up to 32 sprites on screen at once, and have 32KB of bitmap data to store them in. 

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

# Clear all sprite RAM, reset all sprite handles
tulip.sprite_clear()
```


https://user-images.githubusercontent.com/76612/215896311-fc0823aa-44bc-4305-85db-a6773db11a98.mov



## Tulip hardware HOWTOs

![Custom Tulip laptop](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/hinge.jpg)

There are four ways to start building and using your own Tulip Creative Computer. 

 * [No hardware for me, I want to just run Tulip Desktop on my desktop computer for now](docs/tulip_desktop.md). Use this option if you're just curious about Tulip and want to try it out. If you get into it, you can then upgrade to real hardware!
 * [I just want to build a temporary Tulip with hookup wires and a breadboard](docs/tulip_breadboard.md). Do this if you aren't great at soldering or just want to quickly throw a Tulip together. It won't be very stable and will be hard to move or use portably, as there's many wires. But it'll work!
 * [I can solder through-hole just fine and want to build a more reliable Tulip with your breakout board PCB.](docs/tulip_breakout.md) Do this if you are competent at thru-hole soldering and want to make a reliable Tulip by soldering together other breakout boards. You can buy a cheap PCB that bridges together all the things you need. It's more bulky than our integrated Tulip board, but works just the same. 
 * [I want to build a single board Tulip and know how to solder SMT or use a service that assembles one for me.](docs/tulip_board.md) This is the deluxe option: a fully integrated Tulip board with connectors for a display, audio, MIDI and keyboard. Like a Raspberry Pi but it's Tulip. But you'll need either good soldering skills (many small SMT parts and the FPC connector) or use a service like JLC or Aisler to assemble the board for you. 


## How to compile and help develop Tulip

 * [How to compile and flash Tulip hardware](docs/tulip_flashing.md)
 * [How to run or compile Tulip Desktop](docs/tulip_desktop.md)

Some development guidelines if you'd like to help contribute!

 * Be nice and helpful and don't be afraid to ask questions! We're all doing this for fun and to learn. 
 * We're currently a hard fork of ESP-IDF, Alles/AMY and Micropython; we aim to have no external libraries. Keep everything needed to build for both Tulip Desktop and CC in this repository when possible. While we work on early versions of Tulip, we can merge new features of libraries manually. 
 * Any change or feature must be equivalent across Tulip Desktop and Tulip CC. There are of course limited exceptions to this rule, but please test on hardware before proposing a new feature / change. 








