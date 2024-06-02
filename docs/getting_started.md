# Getting started with the Tulip CC v4r11

Welcome to the [Tulip Creative Computer!](https://github.com/bwhitman/tulipcc) Here you'll find out all about your new Tulip board that you bought from Makerfabs. 

Before we get started, we want to explain the brief history of Tulip and how we work on it. You'll get a lot more out of Tulip if you understand what sort of thing we're building and what sort of community we are.

Tulip has been a side project of [mine](https://notes.variogram.com/about) for a few years. I've always wanted a small low-power computer I could write code or games or music on, that was portable and didn't have distractions like a web browser or work email. Over the years, Tulip has gone through a few iterations from [a bunch of wires](tulip_breadboard.md) to the much more polished and cheap board you just bought from our friends at Makerfabs. Along the way, I encouraged [a friend](https://scholar.google.com/citations?user=1H4HuCkAAAAJ&hl=en) to help me with the [music synthesizer underlying Tulip](https://github.com/bwhitman/amy). We quickly saw the power and fun of having an excellent analog and FM-style synth that could be controlled in Python on constrained real-time hardware. The Tulip you have in your hands is brand new, we constantly chase bugs, add new features, make new music and demos for it. There's a small but growing community of like minded people on our [Discord](https://discord.gg/TzBFkUb8pG) who try fun experiments as we build. 

**No one working on Tulip makes any money from it**. This is not our full time job. We're all hobbyists and engineers and musicians who like working on a small project like this and we hope others can join us. Tulip was designed to be incredibly cheap and completely open source. You can even [build one yourself!](tulip_build.md). I've fronted the initial money to be able to get this version of Tulip in your hands, and you did pay a tiny amount (single-digit US $) extra over the raw cost to build Tulip, to support future design work from our partner Makerfabs. 

**Tulip is fun to use today but still needs help**. You will run into lots of weirdness and challenges. We're hoping to build a community of people that can work together to fix these and constantly make the firmware in Tulip better every day. We're often amazed that it even works this well today. A low power all-in-one computer with high-res screen that supports more Juno-6 polyphony than Roland's own modern products, that can run Python code every 16th note? This is very cool to us, and we hope you agree and can help us make it even better. 

If you have problems with Tulip, you can find us on GitHub [issues](https://github.com/bwhitman/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG). We'll do our best to help out!

-- Brian


## Get used to the ports

Here's a legend to the ports, facing the front. 

![Front legend](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulipv4r11_front.png)

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


![Back legend](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tulipv4r11back.png)

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

 - Type `run('calibrate')` to calibrate the touch screen if you think it is needed. 
 - The black icon on the bottom right is a launcher, for quick access to some included programs. Tap on `Drums` and you should see (and hear!) a drum machine. 
 - Tap the top right blue "switch" icon to get back to the REPL. Tap it again to get back to the drum machine.
 - Tap the red "quit" icon to quit the drum machine.
 - You can `edit('boot.py')` to add anything you want to run on Tulip startup, like wifi, or calibration, or setting up a synth.

Now, [check out the API for making your own music, games, graphics and writing with Tulip.](tulip_api.md)

[If you want to work on Tulip itself, check out our guide to building and flashing the Tulip firmware.](tulip_flashing.md)

[![shore pine sound systems discord](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/shorepine100.png) **Chat about Tulip on our Discord!**](https://discord.gg/TzBFkUb8pG)

Have fun! See you on Tulip World!



## Known problems

### Flashing on boot, or a message "Tulip flash not setup properly"

We "overclock" Tulip's RAM to 120MHz so it can support more polyphony. This works great but currently (we're working on it) has a downside where first cold boots often stall or hang in certain temperature conditions. If you see your Tulip flash or appear to reboot on first turn on (with the swtich), it should eventually settle down and be fine. Future reboots (with the `RESET` button or by `control-D`) will be fine as well. In rare instances, the cold boot may not load our file system properly. You'll see a message like "Tulip flash not setup properly." If you see that message, just hit `control-D` or the `RESET` button on top. It will go away and you'll be fine. We're working with the chip manufacturer on a more solid solution.

### Touchscreen not responding

If your touchscreen appears to not respond, the most likely explanation is the touchscreen panel cable got loose during shipping or if you installed a battery. We're working with Makerfabs to tape down this connector for higher reliability. But it's a quick fix if it happens to you. Just take off the back case, unseat the touch connector (labeled above) by pulling up the black clamp, and push back in the cable tight, applying a little pressure. Clamp it back, and take a piece of tape to hold down the cable right at the connector, like this: 

![Touch connector tape](https://raw.githubusercontent.com/bwhitman/tulipcc/main/docs/pics/tuilpv4r11ctp.jpg)


### USB computer keyboard not working

If your USB computer keyboard is not working, the most likely explanation is that it appears as a "hub" instead of a bare keyboard to Tulip. At this time (we're working on it!) Tulip only supports "root" devices. You'll have to try another keyboard. If you think your keyboard should work, please find us on [issues](https://github.com/bwhitman/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG) and we'll help out!






