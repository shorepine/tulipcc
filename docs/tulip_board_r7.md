# The Tulip Creative Computer Board

##  🌈🌈 Revision 7 🌈🌈
This page is for **revision 7**. We are currently assembling and testing r7. This page will be in flux as we verify the design! 

![Tulip Board](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip_board_r7.jpg)

We've designed our own integrated [Tulip CC](../README.md) board. It's the nicest way to experience Tulip: a small 66x56mm PCB with a display FPC connector, USB-C connector for charging, power and flashing, joystick connector, USB keyboard connector, 3 3.5mm jacks for audio out, MIDI in, and MIDI out, a power switch, and connector for external power. 

If you're comfortable with surface mount soldering, you can put together your own board relatively easily. You can order the PCBs from [OSH Park here.](UPDATE_THIS) (OSH Park charges only to manufacture the boards, I do not receive anything.)

[The BOM for the latest revision is here.](https://github.com/bwhitman/tulipcc/blob/main/docs/pcbs/tulip4_board_v4r7/tulip_board_bom.csv) Please note I haven't yet optimized this BOM for price. But everything is at least in stock, mostly at Digikey. [Make sure to use my public Digikey list to order the parts from Digikey quickly.](https://www.digikey.com/en/mylists/list/ZB5IAQSM2O) [EAGLE board and schematic files are here.](https://github.com/bwhitman/tulipcc/tree/main/docs/pcbs/tulip4_board_v4r7) 

If this looks too hard, you can [instead make a breakout board with just through hole solder](tulip_breakout.md), or [with no soldering and a breadboard.](tulip_breadboard.md)

![Tulip Board](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/board_r7.png)


## Assembly tips

I've found assembling a Tulip with a hot plate and stencil the easiest way to go. You can get stencils from OSH Park when you're ordering the boards. 

The FPC connector goes on the bottom of the board. I wait to attach the FPC connector until after I've done a round on the hot plate. I do it manually with a fine tip iron and flux. You need to apply pressure to the FPC pins as you solder them down. 

There are a few through hole connectors. Especially don't forget to solder the legs of the USB-C connector.

The RESET transistor chip should be oriented so that you can read M A, and there will be two small dots on the bottom left corner. 

Attach the FPC cable to the RGB port on the display. You can use the cable that comes with the display.


## Power

The board can be powered from USB-C or via an external battery charge board. The external batteries connect via the 3-pin JST connector. If powering from USB-C only, set the EXT-USB jumper to USB. If powering from the external JST connector, set it to EXT.

For example, the [J5019 PCB](https://www.amazon.com/dp/B08GWTBD3T?th=1) works great with Tulip and 2 18650 batteries, wired in parallel. Wire the JST cable to the VOUT-, VIN+, and VOUT+ of the J5019. (That's in order, looking from top to bottom of the Tulip PCB.) Make sure you've used the trim pot on the J5019 to get as close to 5V as possible. This will let the USBC input charge the battery, and the battery will power the Tulip.

For both jumper settings the USBC connection will allow you to see the serial monitor on a computer.

Please be careful using unprotected batteries like this. Don't leave them unattended while you're setting them up. Tulip's board has no protection for inverted polarity or out of spec voltages.

![Battery setup](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip_board_r7_battery.jpg)

## Expansion headers

The 8 pin EXP header is, from left to right on the board face: `5V`, `3V3`, `GND`, `SDA`, `SCL`, `IO38`, `IO0`, `IO9`. If you want more GPIO and don't want a hardware joystick, the 7 pins on the SNES header are (left to right, from the board face:) `5V`, `CLOCK` = `IO13`, LATCH = `IO48`, DATA = `IO45`, `NC`, `NC`, `GND`.

## MIDI

The 3.5mm MIDI jacks are for Type A TRS MIDI connectors. If you want to wire them to full size MIDI connectors, you should get [Type A converters.](https://www.amazon.com/ZAWDIO-Breakout-LittleBits-Female-Electribe/dp/B08WHSP7ZL/) You can also get Type-A to Type-B converter cables if your equipment is mostly Type B. 

## Joystick

You can optionally install a [SNES gamepad connector](https://www.zedlabz.com/products/zedlabz-7-pin-90-degree-female-controller-connector-port-for-nintendo-snes-console-2-pack-grey?_pos=2&_sid=c482be32b&_ss=r) on the bottom of the board. You can also use the CLOCK, DATA and LATCH pins to connect your own controller of any similar type (NES will work as well.) 


## Flash and startup

After you're done assembling, [read about how to compile and flash Tulip.](tulip_flashing.md)


## Help wanted

I would love hands on help to make the board better -- routing, placement, part selection, BOM optimization, charging/boost on board, porting to a different EDA program. Please file a GitHub pull request or [chat with us on our discussions page](https://github.com/bwhitman/tulipcc/discussions) to start contributing!


![Schematic](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pcbs/tulip4_board_v4r7/schematic.png)






