# Tulip Creative Computer

This a repository for the Tulip Creative Computer (aka Tulip, aka Tulip CC, aka Tulip4), a personal "weird thing" I've spent a lot of time working on over the years. I'm making it available to the world, just in case there's someone out there with the same strange interests. 

Tulip is a self contained computer, with a display and keyboard and sound. It boots directly into a Python prompt. It is not a shell based on another operating system. Every byte of RAM, every cycle of the CPU is dedicated to your code, the display and music. 

The idea behind Tulip is a portable creative terminal. Without distractions or complications, you are able to dive right into making something. 

You can use Tulip to make music, write code, make art, games, or just write. It does not have a web browser or social media, although it can connect to a network in a slow fashion.

You can build your own Tulip for about $25 plus the cost of a display and USB keyboard. 

The hardware for revision 4 of Tulip is based on the ESP32-S3 dual core microcontroller running at 240MHz. 
The display is a 1024 x 600 "dot clock RGB" display.
The synth makes sound over I2S, we use a powered mono amplifier breakout to a speaker. You can use headphones or other connectors instead.

Tulip rev 4 supports:
    - 8.5MB of RAM
    - 8MB flash storage, as a filesystem accesible in Python
    - The Alles 64-voice synthesizer engine, locally. You can also control an Alles mesh.
    - Text frame buffer layer, 128 x 50, with 16 colors, inverse, bold, underline
    - Up to 32 sprites on screen, drawn per scanline, from a total of 32KB of bitmap memory (1 byte per pixel)
    - A 1280x750 background frame buffer to draw arbitrary bitmaps to, which can scroll horizontally / vertically
    - WiFi via Python sockets / curl / etc
    - Adjustable display clock, defaults to 24 MHz / 30 FPS.
    - 256 colors, can be set to 65535 with a hit to FPS
    - Can load PNGs from disk to set sprites or background from code
    - Built in code and text editor
    - USB keyboard support

Tulip's "OS" is heavily based on the great work of Micropython, which a lot of changes and additions to support the Python REPL on screen and integrated into the display system. Most of our code is in the ports/esp32/ directory. 

Changes and features very welcome via pull request. 


## DIY HOWTO

To build your own Tulip:
    - Get a ESP32-S3 NXR8 dev board. https://www.adafruit.com/product/5336 . There's lots of variants, but you need an S3 for sure, and the "NXR8" means 8MB of SPI RAM and XMB of non-volatile flash storage (they have 8 and 32). If you get this exact one (the ESP32-S3-WROOM-1 N8R8) you can follow my pin numberings / get my breakout board. 
    - Get a RGB dot clock display. I am currently using this one https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html . You want to make sure it takes "RGB" input, usually with 8 pins each for R, G and B (and then HSYNC, VSYNC, DE). 
    - For sound, get an I2S decoder. You can get ones that have line outs like https://www.adafruit.com/product/3678 or ones that have speaker amps built in like https://www.adafruit.com/product/3006 . I use the speaker amp model and hook it into a little 3W speaker.
    - We are working on a breakout board that plugs into the back of a ESP32S3 dev board and has a USB female A jack, pins for the display breakout https://www.adafruit.com/product/4905 and pins for the i2s amp breakout. That makes things a lot simpler. In the meantime, here's the pin connections you'll want to make:

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

(Note: if you want to try RGB565 mode, also wire B5-B0, G4-G0, R4-R0. There's enough pins on the ESP32-S3 to support this, but we default to RGB332 to save memory and CPU time.)








