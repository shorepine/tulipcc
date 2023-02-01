# Tulip Breakout Board Edition

![Tulip Breakout](https://github.com/bwhitman/tulipcc/raw/main/pics/breakout.jpg)


If you'd like a permament version of Tulip and can do through hole soldering, you can buy buy [the breakout PCB I designed fron OSH Park](https://oshpark.com/shared_projects/L1xtM8pM). It's $30 for 3 (this money goes all to OSH Park to manufacture the boards, I don't receive anything), and takes about a week to ship. It's simpler, will be more reliable and will fit inside a case better than a breadboard. The PCB is simply a way to connect the breakout boards without any jumper wires. The [Eagle files for this breakout PCB are here.](https://github.com/bwhitman/tulipcc/tree/main/pcbs/tulip4_breakout_v3)

You'll need:

- [ESP32-S3 WROOM-1 N8R8 dev board.](https://www.adafruit.com/product/5336). If you can find the ESP32-S3 WROOM-2 N32R8 (32MB of flash), it will _probably_ also work fine and you'll have more storage space.
- [This $58 RGB dot-clock 10.1" display with capacitive touch.](https://www.hotmcu.com/101-inch-1024x600-tft-lcd-display-with-capacitive-touch-panel-p-215.html). Note other RGB dot clock displays of different sizes and resolutions can also work, but the pin numberings will be different and you'll have to update the resolution in our code. 
- [A 40-pin FPC header for the display](https://www.adafruit.com/product/4905). 
- This stereo line-out / headphone jack [UDA1334 DAC](https://www.aliexpress.com/item/3256803337983466.html?gatewayAdapt=4itemAdapt). 
- _Almost_ any USB keyboard should work. If yours doesn't, please file an issue here and I can investigate with you. I can only test the ones I have here! I do recommend the [Keychron series of mechanical keyboards](https://www.keychron.com/products/keychron-k7-ultra-slim-wireless-mechanical-keyboard?variant=39396239048793), they're inspiringly clicky. 
- Connectors and random parts: 
   - [Female headers are recommended, so you don't solder the ESP and audio jack directly to the PCB.](https://www.adafruit.com/product/598) 
   - I'd also get this [2x20 shrouded header](https://www.adafruit.com/product/1993) for the display FPC breakout. 
   - [1 USB female A connector](https://www.amazon.com/Uxcell-a13081900ux0112-Female-Socket-Connector/dp/B00H51E7B0)
   - [2 female 5-pin DIN MIDI jacks](https://www.adafruit.com/product/1134)
   - [1 6N138 optoisolator](https://www.amazon.com/Optocoupler-Single-Channel-Darlington-Output/dp/B07DLTSXC1) and an [8-pin socket](https://www.adafruit.com/product/2202)
   - [7 resistors](https://www.amazon.com/BOJACK-Values-Resistor-Resistors-Assortment/dp/B08FD1XVL6): `R1`: 4.7K, `R2`: 4.7K, `R3`: 4.7K, `R4`: 220, `R5`: 470, `R6`: 33, `R7`: 10. `R4` through `R7` don't have to be precisely those numbers, find the closest number that you have. 
   - 1 diode: [1N4001](https://www.adafruit.com/product/755)

The assembly for our breakout PCB is simple. Solder the headers to the DISPLAY, ESP32S3L, ESP32S3R, and AUDIO rows. Solder the 8-pin socket where the 6N138 goes. Solder the resistors in their correct spots, and the diode (note the polarity.) Solder the USB connector. Solder the MIDI connectors. Then place the ESP32-S3 breakout in, the FPC connector in (facing down, with the FPC cable going away from the board), the I2S board in, the 6N138 in, and connect a USB keyboard and the display to the FPC connector (to the displays' "RGB" input, the blue side facing up on both sides of the connector.) Then skip ahead to how to flash Tulip for the first time. That's it!

After you're done assembling, [read about how to compile and flash Tulip](tulip_flashing.md).
