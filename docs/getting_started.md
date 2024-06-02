# Getting started with the Tulip CC v4r11

Welcome to the Tulip Creative Computer! Here you'll find out all about your new Tulip that you bought from Makerfabs. 

## Get used to the ports

Here's a legend to the ports, both facing the front and the back (with the cover removed.) You can use a standard phillips head screwdriver to remove the back plate. 

![Front legend](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4r11_front.png)

![Back legend](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4r11_back.png)


## Acesssories you may want

The Tulip board you got can work on its own. Most people will want to add a couple of accessories to make it work even better. 

### USB keyboard

Tulip is a very command-line centered interface with some touch controls. You'll have a much easier time using Tulip if you connect a USB computer keyboard to it for typing. It turns the Tulip into a portable "deck" for whatever you can imagine. You can use _almost_ any USB computer keyboard you already have. However, make sure it's just a keyboard -- no extra trackpads or trackballs or nib-pointers. Tulip can only work with a single USB device at a time, and those combination keyboards appear as two. 

### I2C DAC for modular synths

If you use modular synths, Tulip is a powerful and fun control surface and way to "program" your modulars in Python. You can use the existing MIDI ports to do a lot, but if you have CV gear, we recommend getting a I2C DAC to control it. Our friends at Makerfabs are going to list one in their store. In the meantime, we recommend [the M5Stack DAC2](https://shop.m5stack.com/products/m5stickc-dac-2-hat-gp8413). It's only $7 and provides two DAC channels. You can use up to 8 at once for a total of 16 CV outputs!! 

The Makerfabs DACs will have 3.5mm outputs, ready for analog synths. The M5Stack ones use screw terminals, so you should pick up a few [mono 3.5mm screw terminal adapters](https://www.amazon.com/Terminal-Solderless-Speaker-Headphone-Pack-32CM/dp/B08QFKFG13). 

### Type-A 3.5mm MIDI adapters to full size

Most modern MIDI equipment now uses 3.5mm jacks for MIDI connectors. If you have the older 5-pin DIN connectors and want to use them on Tulip, you'll want to pick up a couple of [Type-A 3.5mm adapters](https://www.amazon.com/Kurrent-Electric-Type-3-5mm-Adapter/dp/B0C2RLB3SL/). 

### A small li-po battery

Tulip is a low power device and can run on battery for portable computing. There's a standard JST connector on back for flat li-po or 18650 battery packs. I personally use [this 1200mAh battery](https://www.adafruit.com/product/258). If you want to keep it "inside" Tulip (between the back case and the PCB), you'll want to find a battery that is 5mm or so thin. You can use a piece of double sided tape it keep it inside the battery area of the PCB. 

Larger batteries will work great and last longer, but you'll have to remove the back case to make them fit (or mount the battery some other way.) [This 5200mAh battery pack](https://www.amazon.com/XINLANTECH-Rechargeable-Bluetooth-Electronic-Batteries/dp/B0C2VFTDPY) will last many many hours on Tulip and fit right on the PCB with the back case removed. 

## Get started with Tulip

Now that you've got your Tulip, here's how to get started:

 - Plug in a battery if you have one. If you remove the back case, make sure not to bump the touchscreen connector labeled above as it's been known to be loose (we're working on gluing it down at the factory.) 
 - Connect a USB-C cable from a power adapter or your computer to the **top** USB connector, facing Tulip.
 - Plug in a USB keyboard to the **bottom** USB connector. 
 - Optionally wire up your audio jack, MIDI and I2C connectors if you have them. Headphones will work fine on the audio jack if you don't have a mixer or other sound system. 
 - Turn on the Tulip! The switch on the top of the board.
 - You should see the following. If you have audio connected, you should also hear a "bleep" sound on startup.

![First boot](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulip4r11firstboot.jpg)

 - Your USB keyboard should be able to type into this screen, called the "REPL". It can accept Python code and some simple system commands. Try `ls` and you should see a directory listing. 

You should **upgrade your Tulip** whenever you receive it. We are constantly adding new features, fixing bugs, updating the API. To upgrade your Tulip, connect it to your Wi-Fi first:

 - `tulip.wifi('ssid', 'password')` will return an IP address if successful.
 - `tulip.upgrade()` will walk you through the upgrade. Accept both upgrades -- the system folder and the firmware. This process will take a few minutes in total. Your Tulip will reboot itself when it is done.

 More quick start tips:

 - The black icon on the bottom right is a launcher, for quick access to some included programs. Tap on `Drums` and you should see (and hear!) a drum machine. 
 - Tap the top right blue "switch" icon to get back to the REPL. Tap it again to get back to the drum machine.
 - Tap the red "quit" icon to quit the drum machine.

Now, [check out the API for making your own music, games, graphics and writing with Tulip.](tulip_api.md)

[If you want to work on Tulip itself, check out our guide to building and flashing the Tulip firmware.](tulip_flashing.md)

[![shore pine sound systems discord](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/shorepine100.png) **Chat about Tulip on our Discord!**](https://discord.gg/TzBFkUb8pG)

Have fun! See you on Tulip World!









## Known problems

### Slow boot: flashing on boot, or a message "Tulip flash not setup properly"

### Touchscreen not responding

### USB computer keyboard not working





