# Tulip Creative Computer

Welcome to the Tulip Creative Computer (aka Tulip, aka Tulip CC.) 

Tulip is a self contained portable creative computer, with a display and keyboard and sound. It boots instantaneously into a Python prompt. Dive right into making something without distractions or complications. 

Tulip is not a shell on top of another operating system. Every byte of RAM and every cycle of the CPU is dedicated to your code, the display and music. 

You can use Tulip to make music, code, make art, games, or just write. It cannot run any existing applications and does not have a web browser, although it can connect to a network in a slow fashion.

Tulip is available both as a hardware DIY project (Tulip CC) and a macOS app (Tulip Desktop.) They both have all the same features. I use Tulip Desktop when developing Tulip, but use the hardware Tulip CC when making things! If you're nervous about building the hardware, give the desktop version a go to learn about it.

You can build your own Tulip CC for about $25 plus the cost of a display ($50) and USB keyboard.

The hardware for revision 4 of Tulip CC is based on the ESP32-S3 dual core microcontroller running at 240MHz. This single inexpensive chip can support all of Tulip's functionality at low power use. It can last on any USB battery pack for many hours.

The display we use is a 10.1" 1024 x 600 RGB dot clock color LCD with capacative touch support. 

Tulip's sound system is a full featured 64-voice synthesizer. We use a powered mono amplifier breakout to a speaker. You can use headphones or other connectors instead.

I've been working on Tulip on and off for years over many hardware iterations and hope that someone out there finds it as fun as I have, either making things with Tulip or working on Tulip itself. I'd love feedback, your own Tulip experiments or pull requests to improve the system.

Tulip CC rev 4 supports:
- 8.5MB of RAM
- 8MB flash storage, as a filesystem accesible in Python
- The [Alles](https://github.com/bwhitman/alles) 64-voice synthesizer engine running locally or as a control for an Alles mesh. Tulip's synth supports additive oscillators, an excellent FM synthesis engine, samplers, karplus-strong, filters, and much more. 
- Text frame buffer layer, 128 x 50, with ANSI support for 256 colors, inverse, bold, underline, background color
- Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
- A 1280x750 background frame buffer to draw arbitrary bitmaps to, which can scroll horizontally / vertically
- WiFi, access http via Python requests or TCP / UDP sockets 
- Adjustable display clock and resolution, defaults to 30 FPS at 1024x600.
- 256 colors
- Can load PNGs from disk to set sprites or background, or generate bitmap data from code
- Built in code and text editor
- USB keyboard support
- Capactive touch support (mouse on Tulip Desktop)
- MIDI input and output 
- 500mA power usage including display, at medium display brightness, can last for hours on USB battery pack 


## Usage 

Tulip boots right into a Python prompt and all interaction with the system happens there. You can make your own Python programs with its built in editor and execute them, or just experiment on the Tulip REPL prompt in real time.

### General

```python
# Interact with the filesystem.
# Supported: ls, head, cat, newfile, cp, mv, rm, pwd, cd, mkdir, rmdir
ls
mkdir('directory')
cd('directory')

# Clears the REPL screen and formatting
clear

# Opens the Tulip editor to the given filename. 
# Control-X exits and prompts to save if any changes. 
# Control-O is save as
# Control-W searches
# Filename will be created on save if it doesn't exist.
edit("game.py")

# Run a saved Python file. Control-C stops it
execfile("game.py")

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

### Input

Tulip supports USB keyboard input and touch input. (On Tulip Desktop, mouse clicks act as touch points.) 

```python
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

### Network

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

### Graphics system

The Tulip GPU consists of 3 subsystems:
 * A bitmap graphics plane (default size: 1280x750), with scrolling x- and y- speed registers
 * A text frame buffer (TFB) that draws 8x12 fixed width text on top, with 256 colors
 * A sprite layer on top

The Tulip GPU runs at a fixed FPS depending on the resolution and display clock. You can change the display clock but will limit the amount of room for sprites and text tiles per line. 
Some example display clocks and resolutions:

| H_RES   | V_RES   | Clock  |  FPS   | H_RES   | V_RES   | Clock  |  FPS   |
| ------- | ------- | ------ | ------ | ------- | ------- | ------ | ------ |
| 1024    | 600     | 10     | 14.98  | 512     | 600     | 10     | 19.91  |
| 1024    | 600     | 14     | 18.55  | 512     | 600     | 14     | 30.26  |
| 1024    | 600     | 18     | 23.19  | 512     | 600     | 18     | 37.82  |
| 1024    | 600     | 22     | 30.91  | 512     | 600     | 22     | 50.43  |
| 1024    | 600     | 28     | 46.37  | 512     | 600     | 28     | 75.65  |
| 1024    | 300     | 10     | 21.47  | 512     | 300     | 10     | 35.03  |
| 1024    | 300     | 14     | 34.36  | 512     | 300     | 14     | 56.05  |
| 1024    | 300     | 18     | 42.95  | 512     | 300     | 18     | 70.07  |
| 1024    | 300     | 22     | 57.26  | 512     | 300     | 22     | 93.45  |
| 1024    | 300     | 28     | 85.90  | 512     | 300     | 28     | 140.13 |

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
	game_data["frame_count"] += 1

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

### Music / sound

Tulip comes with a local version of the Alles synthesizer, a very full featured 32-oscillator synth that supports FM, PCM, additive synthesis, partial synthesis, filters, and much more. See the [Alles documentation](https://github.com/bwhitman/alles/blob/main/README.md) or the [getting started tutorial](https://github.com/bwhitman/alles/blob/main/getting-started.md) for more information. 

Tulip can also control or respond to an Alles mesh over Wi-Fi. Connect any number of Alles speakers to the wifi to have instant surround sound!

```python
alles.drums() # plays a test song
alles.volume(4) # change volume
alles.reset() # stops all music / sounds playing
alles.send(osc=2, wave=alles.ALGO, patch=4, note=45, vel=1) # plays a tone

# start mesh mode (control multiple speakers over wifi)
alles.mesh() # after turning on wifi
alles.send(osc=1, wave=alles.ALGO, patch=101, note=50, ratio=0.1, vel=1) # all Alles speakers in a mesh will respond
alles.send(osc=1, wave=alles.ALGO, patch=101, note=50, ratio=0.1, vel=1, client=2) # just a certain client
alles.local() # turns off mesh mode and goes back to local mode
```

### MIDI

Tulip supports MIDI in and out. You can set up a python callback to respond immediately to any incoming MIDI message. You can also send messages out to MIDI out. 

On Tulip Desktop, MIDI works on macOS 11.0 (Big Sur, released 2020) and later using the "IAC" MIDI bus. This lets you send and receive MIDI with Tulip to any program running on the same computer. If you don't see "IAC" in your MIDI programs' list of MIDI ports, enable it by opening Audio MIDI Setup, then showing MIDI Studio, double click on the "IAC Driver" icon, and ensure it is set to "Device is online." At this time, MIDI will not function (but the rest of Tulip will run) on macOS 10.15 (Catalina, released 2019.)

```python
def callback(x): # for now you have to define a callback with a dummy parameter
    # tulip.midi_in() returns the last midi buffer received
    m = tulip.midi_in()
    if(m[0]==144):
        print("Note on, note # %d velocity # %d" % (m[1], m[2]))

tulip.midi_callback(callback)
tulip.midi_callback() # turns off callbacks

m = tulip.midi_in() # returns bytes
tulip.midi_out((144,60,127)) # sends a note on
tulip.midi_out(bytes) # Can send bytes or list
```



## DIY HOWTO

Tulip CC can be easily built with minimal / or no soldering by connecting breakout boards of the main chips. With a breadboard and a lot of jumper wires, you can put together Tulip in less than an hour.

The easier path is to buy [the breakout PCB I designed fron Aisler.net](https://aisler.net/p/YZIXPKRR). It's $12 for 3 (this money goes all to Aisler to manufacture the boards, I don't receive anything), and takes about a week to ship. It's simpler, will be more reliable and will fit inside a case better than a breadboard. The PCB is simply a way to connect the breakout boards without any jumper wires. The [Eagle files for this breakout PCB are here.](https://github.com/bwhitman/tulipcc/tree/main/pcbs/tulip4_breakout_v2)

Either way, you'll need:

- [ESP32-S3 WROOM-1 N8R8 dev board.](https://www.adafruit.com/product/5336). If you can find the ESP32-S3 WROOM-2 N32R8 (32MB of flash), it will _probably_ also work fine and you'll have more storage space.
- [This $58 RGB dot-clock 10.1" display with capacitive touch.](https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html). Note other RGB dot clock displays of different sizes and resolutions can also work, but the pin numberings will be different and you'll have to update the resolution in our code. 
- [A 40-pin FPC header for the display](https://www.adafruit.com/product/4905). 
- One of two choices for sound: either [this mono I2S speaker amp board](https://www.adafruit.com/product/3006) or [this WM8960 stereo board with speaker & line-out/headphone output.](https://www.amazon.com/Audio-Supports-encoding-Recording-Interface/dp/B07H6FNFDD). (There's also a bigger Raspberry Pi "HAT" version that works, but with wasted space.) It depends on what you want to build. The PCB currently has a header for the mono speaker version; for the WM8960 you'll have to do your own wiring. It's a TODO for me to add a header for the WM8960 pins as well, probably on the bottom. 
- _Almost_ any USB keyboard should work. If yours doesn't, please file an issue here and I can investigate with you. I can only test the ones I have here! I do recommend the [Keychron series of mechanical keyboards](https://www.keychron.com/products/keychron-k7-ultra-slim-wireless-mechanical-keyboard?variant=39396239048793), they're inspiringly clicky. 
- Connectors and random parts: 
   - [Female headers are recommended, so you don't solder the ESP, display and audio jack directly to the PCB.](https://www.adafruit.com/product/598) 
   - I'd also get this [2x20 shrouded header](https://www.adafruit.com/product/1993) for the display FPC breakout. 
   - [1 USB female A connector](https://www.amazon.com/Uxcell-a13081900ux0112-Female-Socket-Connector/dp/B00H51E7B0)
   - [2 female 5-pin DIN MIDI jacks](https://www.adafruit.com/product/1134)
   - [1 6N138 optoisolator](https://www.amazon.com/Optocoupler-Single-Channel-Darlington-Output/dp/B07DLTSXC1) and an [8-pin socket](https://www.adafruit.com/product/2202)
   - [7 resistors](https://www.amazon.com/BOJACK-Values-Resistor-Resistors-Assortment/dp/B08FD1XVL6): `R1`: 4.7K, `R2`: 4.7K, `R3`: 4.7K, `R4`: 220, `R5`: 470, `R6`: 33, `R7`: 10. `R4` through `R7` don't have to be precisely those numbers, find the closest number that you have. 
   - 1 diode: [1N4001](https://www.adafruit.com/product/755)
   - [1 3W speaker](https://www.adafruit.com/product/4445)
- If you are using a breadboard instead of our PCB, [you'll want to see how to wire the MIDI in and out.](https://diyelectromusic.wordpress.com/2021/02/15/midi-in-for-3-3v-microcontrollers/). 

The assembly for our PCB is simple. Solder the headers to the DISPLAY, ESP32S3L, ESP32S3R, and AUDIO rows. Solder the 8-pin socket where the 6N138 goes. Solder the resistors in their correct spots, and the diode (note the polarity.) Solder the USB connector. Solder the MIDI connectors. Then place the ESP32-S3 breakout in, the FPC connector in (facing down, with the FPC cable going away from the board), the I2S board in, the 6N138 in, and connect a USB keyboard and the display to the FPC connector (to the displays' "RGB" input, the blue side facing up on both sides of the connector.) Then skip ahead to how to flash Tulip for the first time. That's it!

If you're breadboarding, here's the pin connections you'll need to make. A note, if you're using the Adafruit FPC display adapter, these pin numbers for the display (D#) are to match the numbers on the **side of the board the FPC connector is on**, the side that reads FPC-40P 0.5MM. 

| Label         | ESP32 S3 Pin | Position on ESP32-S3-WROOM-1 | Connect to     |
| ------------- | ------------ | ---------------------------- | -------------- |
| Backlight PWM | 16           | Left row, 9th pin down (L9)  | Display 6 (D6) |
| Data Enable   | 42           | Right row, 6th pin down (R6) | D7             |
| VSYNC         | 41           | R7                           | D8             |
| HSYNC         | 40           | R8                           | D9             |
| LCD BL EN     | 39           | R9                           | D10            |
| PCLK          | 14           | L20                          | D11            |
| B7            | 21           | R18                          | D13            |
| B6            | 12           | L18                          | D14            |
| B5            | GND          | L22                          | D15            | 
| G7            | 46           | L14                          | D21            |
| G6            | 3            | L13                          | D22            |
| G5            | 8            | L12                          | D23            |
| R7            | 15           | L8                           | D29            |
| R6            | 7            | L7                           | D30            |
| R5            | 6            | L6                           | D31            |
| 3v3           | 3v3          | L1                           | D37            |
| GND           | GND          | L22                          | D38            |
| 5V            | 5V           | L21                          | D39            |
| 5V            | 5V           | L21                          | D40            |
| Touch SDA     | 18           | L11                          | D4             |
| Touch SCL     | 17           | L10                          | D3             |
| Touch CTP INT | 5            | L5                           | D1             |
| USB 5V        | 5V           | L21                          | USB 5V         |
| USB D+        | 20           | R19                          | USB D+         |
| USB D-        | 19           | R20                          | USB D-         |       
| USB GND       | GND          | L22                          | USB GND        |
| Audio LRC     | 4            | L4                           | Audio LRC      |
| Audio BCLK    | 1            | R4                           | Audio BCLK     |
| Audio DIN     | 2            | R5                           | Audio DIN      |
| Audio GND     | GND          | L22                          | Audio GND      |
| Audio VIN     | 5V           | L21                          | Audio VIN      |
| MIDI in       | 47           | R17                          | MIDI TX        |
| MIDI out      | 11           | L17                          | MIDI RX        |
| MIDI 5V       | 5V           | L21                          | MIDI 5v        |
| MIDI GND      | GND          | L22                          | MIDI GND       |

Also, you may want to ground all remaining display pins if you're seeing flickering. 


## How to compile and help develop Tulip

Tulip's firmware is based on Micropython. You'll find our platform independent code in `extmod/tulip` and our platform specific code in `ports/esp32s3` and `ports/macos`. 

Tulip Desktop should build and run on macOS 10.15 (Catalina, released 2019) and later, Apple Silicon & Intel supported. It also should be fine on Linux but I haven't yet tried. Windows is possible but probably needs some help. 

Tulip CC on ESP32S3 should build on all platforms, although I've only tested macOS so far. Please let me know if you have any trouble!

To compile your own firmware / desktop app, start by cloning this repository. 

On macOS, start by installing homebrew
```
# install homebrew first, skip this if you already have it...
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Then restart your terminal
```

To build Tulip Desktop (macOS 10.15 (Catalina) and higher, Apple Silicon or x86_64):

```
cd ports/macos
brew install pkg-config

# For local development (just your native arch, and shows stderr in terminal)
./build.sh
./dev/Tulip\ CC.app/Contents/MacOS/tulip

# Or package for distribution (creates a universal binary)
./package.sh # makes .app bundle in dist, not necessary if you're just using it locally
```

To build and flash the ESP32S3 Tulip CC:

```
# install esp-idf's requirements
brew install cmake ninja dfu-util

# and install our version of the ESP-IDF that comes with our repository
cd esp-idf
./install.sh esp32s3
source ./export.sh

pip3 install Cython littlefs-python # needed to flash the filesystem

cd ../ports/esp32s3
# Connect your esp32s3 board over USB (to the UART connector on the dev board, not the USB connector)
idf.py -D MICROPY_BOARD=TULIP4 flash

python tulip_fs_create.py # first run only, or after each change to tulip_home, will erase the Tulip filesystem
```

Then to build / debug going forward:

```
cd ports/esp32s3
export ../../esp-idf/export.sh # do this once per terminal window
idf.py -D MICROPY_BOARD=TULIP4 flash
idf.py -D MICROPY_BOARD=TULIP4 monitor # shows stderr, use control-] to quit

# If you make changes to the underlying python libraries on micropython, you want to fully clean the build 
idf.py -D MICROPY_BOARD=TULIP4 fullclean
idf.py -D MICROPY_BOARD=TULIP4 flash
```


Some development guidelines if you'd like to help contribute!

 * Be nice and helpful and don't be afraid to ask questions! We're all doing this for fun and to learn. 
 * We're currently a hard fork of ESP-IDF, Alles and Micropython; we aim to have no external libraries. Keep everything needed to build for both Tulip Desktop and CC in this repository when possible. While we work on early versions of Tulip, we can merge new features of libraries manually. No git submodules. 
 * Any change or feature must be equivalent across Tulip Desktop and Tulip CC. There are of course limited exceptions to this rule, but please test on hardware before proposing a new feature / change. 








