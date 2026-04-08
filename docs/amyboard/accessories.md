# AMYboard Accessories

AMYboard has a front panel I2C port for plugging in accessories. These connect with a simple cable (GROVE connector) -- no soldering required.

The I2C bus (SCL=18, SDA=17, 400kHz) is available for connecting additional hardware:

```python
import amyboard
i2c = amyboard.get_i2c()

# Scan for connected devices
print(i2c.scan())

# Read/write registers on any I2C device
val = amyboard.read_register(addr, reg)
amyboard.write_register(addr, reg, val)
```

You can add more DACs, ADCs, displays, or sensors to expand AMYboard's capabilities.

**Note:** For the Adafruit units below, you'll need a [GROVE to Stemma QT Adapter Cable](). They can be hard to find in-stock, but you can make one (or two) yourself if you have a soldering iron: Take an existing GROVE cable and an existing Stemma QT cable, cut them both in the middle, then connect the bare ends together. (Note that, for Grove cables, the SDA and SCL colors depend on manufacturer.  The ordering below should be correct, however, i.e. SCL is the "edge" cable).

|Signal|Stemma QT color|Grove color|
|--|--|--|
|GND|Black|Black|
|Vcc|Red|Red|
|SDA|Blue|White (Seeed) or Yellow (M5) or White (Adafruit)|
|SCL|Yellow|Yellow (Seeed) or White (M5) or Green (Adafruit)|


## Known compatible accessories

### Displays

 - [**Adafruit Grayscale 1.5" 128x128 OLED Display (STEMMA QT)**](https://www.adafruit.com/product/4741) -- A high-contrast 128x128 grayscale OLED with 16 shades of gray. AMYboard has built-in support for this display, including waveform visualization. Connects directly via the STEMMA QT cable.

```python
import amyboard

# Initialize the display (auto-detects SSD1327 or SH1107)
amyboard.init_display()

# Draw text (x, y, color 0-255)
amyboard.display.text("Hello!", 0, 0, 255)
amyboard.display.text("AMYboard", 0, 16, 128)
amyboard.display_refresh()

# Draw shapes
amyboard.display.fill_rect(10, 40, 50, 20, 200)
amyboard.display_refresh()

# Show a live waveform visualization
amyboard.draw_waveform()
```

### Rotary encoders

 - [**Adafruit I2C STEMMA QT Rotary Encoder Breakout**](https://www.adafruit.com/product/5880) -- A single rotary encoder with push button and NeoPixel LED, running seesaw firmware over I2C. Supports up to 8 on one I2C bus via address jumpers. AMYboard has built-in Python support for reading encoder position and button state.

```python
import amyboard

# Read encoder position (0-3)
pos = amyboard.read_encoder(encoder=0)

# Initialize and read encoder push buttons
amyboard.init_buttons()
buttons = amyboard.read_buttons()
# Returns tuple of 4 booleans (True = pressed)

# Monitor all encoders on the OLED display
amyboard.monitor_encoders()
```

 - [**Adafruit I2C QT Quad Rotary Encoder Breakout**](https://www.adafruit.com/product/5752) -- Four rotary encoders with built-in push buttons on a single I2C breakout, running seesaw firmware. AMYboard has built-in support via `read_encoder()`, `init_buttons()`, and `read_buttons()`. With a display connected, you can use `patch_selector()` to scroll through your `.patch` files with one encoder and load them with a click.

```python
import amyboard

# Read any of the 4 encoders (0-3)
pos = amyboard.read_encoder(encoder=0)

# Initialize and read all 4 push buttons
amyboard.init_buttons()
buttons = amyboard.read_buttons()
# Returns list of 4 booleans (True = pressed)

# Patch selector: scroll through patches and load on click
# (requires a display to be connected)
amyboard.init_display()
amyboard.patch_selector()
```

 - [**M5Stack 8-Encoder Unit (STM32F030)**](https://shop.m5stack.com/products/8-encoder-unit-stm32f030) -- Eight rotary encoders with RGB LEDs and a toggle switch, all on one I2C unit. Great for controlling multiple synth parameters at once.

## Connecting accessories

All accessories plug into the front panel I2C port. You can daisy-chain multiple I2C devices together.

```python
import amyboard

# Scan the I2C bus to see connected accessories
i2c = amyboard.get_i2c()
print(i2c.scan())
```




See [Using Python](python.md) and [Modular Synth Setup](modular.md) for more on using I2C devices with AMYboard.

[Back to Getting Started](README.md)
