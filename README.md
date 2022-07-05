# Tulip Creative Computer

Welcome to the Tulip Creative Computer (aka Tulip, aka Tulip CC.) 

Tulip is a self contained portable creative computer, with a display and keyboard and sound. It boots directly into a Python prompt. You can dive right into making something without distractions or complications. It is not a shell on top of another operating system. Every byte of RAM and every cycle of the CPU is dedicated to your code, the display and music. 

You can use Tulip to make music, write code, make art, games, or just write. It does not have a web browser, although it can connect to a network in a slow fashion.

Tulip is available both as a hardware DIY project (Tulip CC) and a macOS app (Tulip Desktop.) They both have all the same features. I use Tulip Desktop when developing Tulip, but use the hardware Tulip CC when making things! If you're nervous about building the hardware, give the computer version a go to learn about it.

You can build your own Tulip CC for about $25 plus the cost of a display and USB keyboard. 

The hardware for revision 4 of Tulip CC is based on the ESP32-S3 dual core microcontroller running at 240MHz. This single inexpensive chip can support all of Tulip's functionality at low power use. It can last on any USB battery pack for many hours.

The display we use is a 10.1" 1024 x 600 RGB dot clock color LCD. 

Tulip's sound system is a full featured 32-voice synthesizer, we use a powered mono amplifier breakout to a speaker. You can use headphones or other connectors instead.

I've been working on Tulip on and off for years over many hardware iterations and hope that someone out there finds it as fun as I have, either making things with Tulip or working on Tulip itself. I'd love feedback, your own Tulip experiments or pull requests for the base system.

Tulip rev 4 supports:
- 8.5MB of RAM
- 8MB flash storage, as a filesystem accesible in Python
- The [Alles](https://github.com/bwhitman/alles) 32-voice synthesizer engine running locally or as a control for an Alles mesh. Tulip's synth supports additive oscillators, an excellent FM synthesis engine, samplers, karplus-strong, filters, and much more. 
- Text frame buffer layer, 128 x 50, with ANSI support for 256 colors, inverse, bold, underline, background color
- Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
- A 1280x750 background frame buffer to draw arbitrary bitmaps to, which can scroll horizontally / vertically
- WiFi, access http via Python requests or TCP / UDP sockets 
- Adjustable display clock, defaults to 30 FPS.
- 256 colors
- Can load PNGs from disk to set sprites or background, or generate bitmap data from code
- Built in code and text editor
- USB keyboard support
- 500mA power usage including display, at medium display brightness, can last for hours on USB battery pack 

Tulip's "OS" is heavily based on the great work of [Micropython](https://micropython.org), which a lot of additions to support the Python REPL on screen and integrated into the display and sound system. 


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

### Keyboard / USB

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
 * A bitmap graphics plane (1280x750), with scrolling x- and y- speed registers
 * A text frame buffer (TFB) that draws 8x12 fixed width text on top, with 256 colors
 * A sprite layer on top

The Tulip GPU runs at a fixed FPS depending on the display clock. You can change the display clock but will limit the amount of room for sprites and text tiles per line. You can set a python callback for the frame done interrupt, for use in games or animations. 

```python
# returns current GPU usage computed on the last 100 frames, as a percentage of max
usage = tulip.gpu()

# returns current FPS, based on the display clock
fps = tulip.fps() 

# resets all 3 GPU systems back to their starting state, clears all BG and sprite ram and clears the TFB.
tulip.gpu_reset()

# Get or set the display clock in MHz. Current default is 18.
# Try: 10 (11.5 FPS), 12 (15.5 FPS), 14 (18.6 FPS), 18 (23.2 FPS), 22 (30.9 FPS), 28 (46.4 FPS)
# Higher clocks mean smoother animations, but less time for the CPU to prepare things to draw
# For heavy text things, use a lower clock like 12, for games with sprites, use 18 or 22. 
clock = tulip.display_clock() 
tulip.display_clock(mhz)

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

The background plane (BG) is 1280 x 750, with the visible portion 1024x600. Use the extra for hardware scrolling or for storing bitmap data "offscreen" for later blitting (you can treat it as fixed bitmap RAM.) The BG is drawn first, with the TFB and sprite layers drawn on top.

```python
# Set or get a pixel on the BG
(r,g,b) = tulip.bg_pixel(x,y)
tulip.bg_pixel(x,y,r,g,b)
tulip.bg_pixel(x,y,pal_idx) # pal_idx is 0-255 for 8-bit RGB332 mode 

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
tulip.bg_clear(r,g,b) 
tulip.bg_clear(pal_idx)
tulip.bg_clear() # uses default

# Drawing primitives. These all write to the BG. 
# If you want to use them for sprites, you can use bg_bitmap after drawing offscreen.
tulip.line(x0,y0, x1,y1)
tulip.bezier(x0,y0, x1,y1, x2,y2, pal_idx)
tulip.circle(x,y,r, pal_idx, [filled=1])
tulip.roundrect(x0,y0, w,h, r, pal_idx, [filled=1])
tulip.rect(x0,y0, w,h, pal_idx, [filled=1])
tulip.triange(x0,y0, x1,y1, x2,y2, pal_idx, [filled=1])

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

Tulip comes with a local version of the Alles synthesizer, a very full featured 32-oscillator synth that supports FM, PCM, additive synthesis, partial synthesis, filters, and much more. See the [Alles documentation](https://github.com/bwhitman/alles/blob/main/README.md) or the [getting started tutorial](https://github.com/bwhitman/alles/blob/main/getting-started.md) for more information. Tulip can also (TBD) control an Alles mesh over Wi-Fi. 

```python
alles.drums() # plays a test song
alles.volume(4) # change volume
alles.reset() # stops all music / sounds playing
alles.send(osc=2, wave=alles.ALGO, patch=4, note=45, vel=1) # plays a tone
```



## DIY HOWTO

### Hardware 

To build your own Tulip CC:
- Get an [ESP32-S3 NXR8 dev board.](https://www.adafruit.com/product/5336). There's lots of variants, but you need an S3 for sure, and the "NXR8" means 8MB of SPI RAM and XMB of non-volatile flash storage (they have 8 and 32). If you get this exact one (the ESP32-S3-WROOM-1 N8R8) you can follow my pin numberings / get my breakout board. 
- Get a RGB dot clock display. [I am currently using this one](https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html). You want to make sure it takes "RGB" input, usually with 8 pins each for R, G and B (and then HSYNC, VSYNC, DE). If it is not 1024x600 you'll want to make changes in `display.h`. 
- For sound, get an I2S decoder. [You can get ones that have line outs like this](https://www.adafruit.com/product/3678) or ones that [have speaker amps built in like this](https://www.adafruit.com/product/3006). I use the speaker amp model and hook it into a little 3W speaker.
- We are working on a breakout board that plugs into the back of a ESP32S3 dev board and has a USB female A jack for the keyboard, pins for [a display breakout]( https://www.adafruit.com/product/4905) and pins for the i2s amp breakout. That makes things a lot simpler. 

A note, if you're using the Adafruit display adapter, these pin numbers for the display (D#) are to match the **side of the board with the FPC connector is on**, the side that reads FPC-40P 0.5MM. I have no idea why the pin numbers are inverted on the other side, but don't be like me and waste a board run because you wired it wrong! 

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
| USB 5V        | 5V           | L21                          | USB 5V         |
| USB D+        | 20           | R19                          | USB D+         |       
| USB D-        | 19           | R20                          | USB D-         |       
| USB GND       | GND          | L22                          | USB GND        |
| Audio LRC     | 4            | L4                           | Audio LRC      |
| Audio BCLK    | 1            | R4                           | Audio BCLK     |
| Audio DIN     | 2            | R5                           | Audio DIN      |
| Audio GND     | GND          | L22                          | Audio GND      |
| Audio VIN     | 5V           | L21                          | Audio VIN      |



## How to compile and help develop Tulip

Tulip's firmware / desktop application is based on Micropython. We use their folder structure and "ports" for `esp32` and `unix` (for Tulip Desktop.) To compile your own firmware / desktop app, start by cloning this repository. 

Local version:

```
cd ports/unix
make
./tulip.{arm64,x86_64}

./package.sh # makes .app bundle, not needed if you're just using it locally
```

ESP32S3:

```
cd ports/esp32
export ../../esp-idf/export.sh
pip3 install littlefs-python
# Connect your esp32s3 board over USB (from the UART connector)
ls /dev/cu* # see what port it is on
idf.py -D MICROPY_BOARD=TULIP4 -p PORT_YOU_FOUND flash
python tulip_fs_create.py # first run only, will erase flash
idf.py -D MICROPY_BOARD=TULIP4 -p PORT_YOU_FOUND monitor
```

On ESP32S3 The "GPU" hardware is mostly implemented in the `esp_lcd_bw` fork of `esp_lcd`. That's in our local components folder. 

Some development guidelines if you'd like to help contribute!

 * Be nice and helpful!
 * We're currently a hard fork of ESP-IDF and Micropython; we aim to have no external libraries. Keep everything needed to build for both Tulip Desktop and CC in this repository when possible. We can merge new features of libraries manually. No git submodules. Yes, there are many downsides to this approach but keeping the hardware & software in sync is much easier this way, especially with fast moving projects like ESP-IDF.
 * Any change or feature must be equivalent across Tulip Desktop and Tulip CC. There are of course limited exceptions to this rule, but please test on hardware before proposing a new feature / change.

 








