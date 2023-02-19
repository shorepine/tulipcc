# The Tulip Creative Computer Board

![Tulip Board](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/board.jpg)

We've designed our own integrated [Tulip CC](../README.md) board. It's the nicest way to experience Tulip: a small 66x56mm PCB with a display FPC connector, USB-C connector for charging, power and flashing, USB keyboard connector, 3 3.5mm jacks for audio out, MIDI in, and MIDI out, a power switch, and headers for external power and I2C access. 

If you're comfortable with surface mount soldering, you can put together your own board relatively easily. You can order the PCBs from [OSH Park here.](https://oshpark.com/shared_projects/Xv9QDKgz) (OSH Park charges only to manufacture the boards, I do not receive anything.)

[The BOM for the latest revision is here.](https://github.com/bwhitman/tulipcc/blob/main/docs/pcbs/tulip4_board_v4r6/tulip_board_bom.csv) Please note I haven't yet optimized this BOM for price. But everything is at least in stock, mostly at Digikey. [Make sure to use my public Digikey list to order the parts from Digikey quickly.](https://www.digikey.com/en/mylists/list/90SZQTXF6W) [EAGLE board and schematic files are here.](https://github.com/bwhitman/tulipcc/tree/main/docs/pcbs/tulip4_board_v4r6) 

If this looks too hard, you can [instead make a breakout board with just through hole solder](tulip_breakout.md), or [with no soldering and a breadboard.](tulip_breadboard.md)

![Tulip Board](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/board_unpop.png)


## Assembly tips

I've found assembling a Tulip with a hot plate and stencil the easiest way to go. You can get stencils from OSH Park when you're ordering the boards. 

I wait to attach the FPC connector until after I've done a round on the hot plate. I do it manually with a fine tip iron and flux. You need to apply pressure to the FPC pins as you solder them down. 

The only through hole component, the legs of the USB-C connector, need to be hand soldered from the bottom after the hot plate. 

The silkscreen currently omits a polarity for the only diode (D2). The white line on the diode is meant to be pointing down. 

The RESET transistor chip is not labeled on the silkscreen. It's the small 6 pin chip on the top to the right of the ESP. It should be oriented so that you can read M A, and there will be two small dots on the bottom left corner. 

![Schematic](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pcbs/tulip4_board_v4r6/schematic.png)

## Power and header

The board has a header row for power and I2C. Its pins are (from top to bottom, the left column is lower pin and right column is higher pin):

| Lower pin   | Higher pin | 
| ------------| ---------- | 
| 5V_IN | 5V_USB | 
| 3V3 | GND | 
| I2C SDA | I2C SCL |
| 5V | GND |
| Jumper: | USB powered |
| Jumper: | 5V_IN powered |

The board can be powered over USB-C or via an external battery power source. There's a jumper along the bottom two rows of the header. If you have it across the bottom pair of pins, Tulip will expect 5V to come from the 5V_IN pin. If the second from the bottom, the USB-C connector. 

This lets you use an external charger / boost circuit like the ubiquitous [J5019 PCB.](https://www.amazon.com/dp/B08GWTBD3T?th=1) Connect a [battery pack](https://www.amazon.com/dp/B07FRYPYTK?th=1) (these are two cells wired in parallel) to the J5019, and then connect 5V out of the J5019 to 5V_IN of Tulip. Connect 5V_USB of Tulip to power in of the J5019. And connect GND to GND. Make sure the jumper is on the bottom pair. This will let the USB-C connector of Tuliip charge the battery while it is plugged in (and still let you communicate with Tulip over USB.)

Please be careful using unprotected batteries like this. Don't leave them unattended while you're setting them up. Tulip's board has no protection for inverted polarity or out of spec voltages.

![Battery setup](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/battery.jpg)

## MIDI

The 3.5mm MIDI jacks are for Type A TRS MIDI connectors. If you want to wire them to full size MIDI connectors, you should get [Type A converters.](https://www.amazon.com/ZAWDIO-Breakout-LittleBits-Female-Electribe/dp/B08WHSP7ZL/) You can also get Type-A to Type-B converter cables if your equipment is mostly Type B. 


## Flash and startup

After you're done assembling, [read about how to compile and flash Tulip.](tulip_flashing.md)


## Help wanted

I would love hands on help to make the board better -- routing, placement, part selection, BOM optimization, charging/boost on board, porting to a different EDA program. Please file a GitHub pull request or issue to start contributing!








