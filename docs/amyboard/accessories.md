# AMYboard Accessories

AMYboard has a front panel I2C port (STEMMA QT / Qwiic connector) for plugging in accessories. These connect with a simple cable -- no soldering required.

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
