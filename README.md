# Tulip Creative Computer

Welcome to the Tulip Creative Computer (aka Tulip, aka Tulip CC, aka Tulip4). 

Tulip is a self contained portable creative computer, with a display and keyboard and sound. It boots directly into a Python prompt. You can dive right into making something without distractions or complications. It is not a shell based on another operating system. Every byte of RAM, every cycle of the CPU is dedicated to your code, the display and music. 

You can use Tulip to make music, write code, make art, games, or just write. It does not have a web browser, although it can connect to a network in a slow fashion.

You can build your own Tulip for about $25 plus the cost of a display and USB keyboard. 

The hardware for revision 4 of Tulip is based on the ESP32-S3 dual core microcontroller running at 240MHz. This single inexpensive chip can support all of Tulip's functionality at low power use. 

The display is a 10.1" 1024 x 600 RGB dot clock color LCD. 

The synth makes sound with a full featured 64-voice synthesizer over I2S, we use a powered mono amplifier breakout to a speaker. You can use headphones or other connectors instead.

I've been working on Tulip on and off for years over many hardware iterations and hope that someone out there finds it as fun as I have, either making things with Tulip or working on Tulip itself. I'd love feedback, your own Tulip experiments or pull requests for the base system.

Tulip rev 4 supports:
- 8.5MB of RAM
- 8MB flash storage, as a filesystem accesible in Python
- The [Alles](https://github.com/bwhitman/alles) 64-voice synthesizer engine running locally or as a control for an Alles mesh.
- Text frame buffer layer, 128 x 50, with ANSI support for 16 colors, inverse, bold, underline
- Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
- A 1280x750 background frame buffer to draw arbitrary bitmaps to, which can scroll horizontally / vertically
- WiFi, access http via Python requests or TCP / UDP sockets 
- Adjustable display clock, defaults to 24 MHz / 30 FPS.
- 256 colors, can be set to 65535 with a hit to FPS
- Can load PNGs from disk to set sprites or background, or generate bitmap data from code
- Built in code and text editor
- USB keyboard support
- 500mA power usage including display, at medium display brightness, can last for hours on USB battery pack 

Tulip's "OS" is heavily based on the great work of [Micropython](https://micropython.org), which a lot of additions to support the Python REPL on screen and integrated into the display and sound system. Most of our code is in the ports/esp32/ directory. 


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
# Filename will be created on save if it doesn't exist.
tulip.edit("game.py")

# Run a saved Python file. Control-C stops it
execfile("game.py")

# If you want something to run when Tulip boots, add it to boot.py
tulip.edit("boot.py")

# Returns the current held keyboard scan codes, up to 6 and the modifier mask (ctrl, shift etc)
(modifiers, scan0, scan1... scan5) = tulip.keys()

# Join a wifi network
tulip.wifi("ssid", "password")

# Get IP address or check if connected
ip_address = tulip.ip() # returns None if not connected

# Save the contents of a URL to disk (needs wifi)
NYI bytes_read = tulip.url_save("https://url", "filename.ext")

# Get the contents of a URL to memory (needs wifi, and be careful of RAM use)
NYI content = tulip.url_get("https://url")

# Set the time from an NTP server (needs wifi)
tulip.set_time() 

# Takes a screenshot and saves to disk. The screen will blank for a moment
# If no filename given will upload to imgur and return a URL (needs wifi)
tulip.screenshot("screenshot.png")
NYI imgur_url = tulip.screenshot()
```

### Graphics system

The Tulip GPU runs at a fixed FPS depending on the display clock. You can change the display clock but will limit the amount of room for sprites and text tiles per line. You can set a python callback for the frame done interrupt, for use in games or animations. 

```python
# returns current GPU usage computed on the last 100 frames, as a percentage of max
usage = tulip.gpu()

# returns current FPS, based on the display clock
fps = tulip.fps() 

# returns CPU usage of the display, code and audio systems -- TBD why is display different from GPU 
NYI (display, code, audio) = tulip.cpu()

# Get or set the display clock in MHz. Current default is 18.
# Try: 10 (11.5 FPS), 12 (15.5 FPS), 14 (18.6 FPS), 18 (23.2 FPS), 22 (30.9 FPS), 28 (46.4 FPS)
# Higher clocks mean smoother animations, but less time for the CPU to prepare things to draw
# For heavy text things, use a lower clock like 12, for games with sprites, use 18 or 22. 
clock = tulip.display_clock() 
tulip.display_clock(mhz)

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

The background plane (BG) is 1280 x 750, with the visible portion 1024x600. Use the extra for hardware scrolling. The BG is drawn first, with the TFB and sprite layers drawn on top.

```python
# Set or get a pixel on the BG
(r,g,b) = tulip.bg_pixel(x,y)
tulip.bg_pixel(x,y,r,g,b)
tulip.bg_pixel(x,y,pal_idx) # pal_idx is 0-255 for 8-bit RGB332 mode and 0-65535 for RGB565 mode

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

# Sets or gets a rect of the BG with bitmap data (packed RGB332 or RGB565 pal_idxes) 
tulip.bg_bitmap(x, y, w, h, bitmap) 
bitmap = tulip.bg_bitmap(x, y, w, h)

# Sets a rect of the BG with a solid color
tulip.bg_rect(x,y,w,h, r,g,b)
tulip.bg_rect(x,y,w,h, pal_idx)

# Clear the BG with a color or default
tulip.bg_clear(r,g,b) 
tulip.bg_clear(pal_idx)
tulip.bg_clear() # uses default

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

# Tulip REPL supports ANSI 256 color modes as well,
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

# Or load sprites in from a bitmap in memory (packed pallete indexes for RGB332 or RGB565)
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
```

### Music / sound

Tulip comes with a local version of the Alles synthesizer, a very full featured 32-oscillator synth that supports FM, PCM, additive synthesis, partial synthesis, filters, and much more. See the [Alles documentation](https://github.com/bwhitman/alles/blob/main/README.md) or the [getting started tutorial](https://github.com/bwhitman/alles/blob/main/getting-started.md) for more information. Tulip can also (TBD) control an Alles mesh over Wi-Fi. 

```python
alles.drums() # plays a test song
alles.reset() # stops all music / sounds playing
alles.send(osc=2, wave=alles.ALGO, patch=4, note=45, vel=1) # plays a tone
```



## DIY HOWTO

### Hardware 

To build your own Tulip:
- Get an [ESP32-S3 NXR8 dev board.](https://www.adafruit.com/product/5336). There's lots of variants, but you need an S3 for sure, and the "NXR8" means 8MB of SPI RAM and XMB of non-volatile flash storage (they have 8 and 32). If you get this exact one (the ESP32-S3-WROOM-1 N8R8) you can follow my pin numberings / get my breakout board. 
- Get a RGB dot clock display. [I am currently using this one](https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html). You want to make sure it takes "RGB" input, usually with 8 pins each for R, G and B (and then HSYNC, VSYNC, DE). If it is not 1024x600 you'll want to make changes in `display.h`. 
- For sound, get an I2S decoder. [You can get ones that have line outs like this](https://www.adafruit.com/product/3678) or ones that [have speaker amps built in like this](https://www.adafruit.com/product/3006). I use the speaker amp model and hook it into a little 3W speaker.
- We are working on a breakout board that plugs into the back of a ESP32S3 dev board and has a USB female A jack for the keyboard, pins for [a display breakout]( https://www.adafruit.com/product/4905) and pins for the i2s amp breakout. That makes things a lot simpler. 

In the meantime, here's the pin connections you'll want to make:

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
| G7            | 46           | L14                          | D21            |
| G6            | 3            | L13                          | D22            |
| G5            | 8            | L12                          | D23            |
| R7            | 15           | L8                           | D29            |
| R6            | 7            | L7                           | D30            |
| R5            | 6            | L7                           | D31            |
| 3v3           | 3v3          | L1                           | D37            |
| GND           | GND          | L22                          | D38            |
| 5V            | 5V           | L21                          | D39            |
| USB 5V        | 5V           | L21                          | USB 5V         |
| USB D+        | 20           | R19                          | USB D+         |       
| USB D-        | 19           | R20                          | USB D-         |       
| USB GND       | GND          | L22                          | USB GND        |
| Audio LRC     | 4            | L4                           | Audio LRC      |
| Audio BCLK    | 1            | R4                           | Audio BCLK     |
| Audio DIN     | 2            | R5                           | Audio DIN      |
| Audio GND     | GND          | L22                          | Audio GND      |
| Audio VIN     | 5V           | L21                          | Audio VIN      |

(Note: if you want to try RGB565 mode, also wire B5-B0, G4-G0, R4-R0. There's enough pins on the ESP32-S3 to support this, but we default to RGB332 to save memory and CPU time. Make sure to `#define RGB565` in `display.h`. 


### Software

TBD: Tulip / micropython build / flash intructions

`idf.py -D MICROPY_BOARD=TULIP4 -p /dev/cu.usbserial-XXX flash`







