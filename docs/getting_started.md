# Getting started with the Tulip CC v4r11

Welcome to the [Tulip Creative Computer!](https://github.com/shorepine/tulipcc) Here you'll find out all about your new [Tulip board that you bought from Makerfabs.](https://www.makerfabs.com/the-tulip-creative-computer.html)

Before we get started, we want to explain the brief history of Tulip and how we work on it. You'll get a lot more out of Tulip if you understand what sort of thing we're building and what sort of community we are.

![Tulip](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulip4.png)

Tulip has been a side project of [mine](https://notes.variogram.com/about) for a few years. I've always wanted a small low-power computer I could write code or games or music on, that was portable and didn't have distractions like a web browser or work email. Over the years, Tulip has gone through a few iterations from [a bunch of wires](tulip_breadboard.md) to the much more polished and cheap board you just bought from our friends at Makerfabs. Along the way, I encouraged [a friend](https://scholar.google.com/citations?user=1H4HuCkAAAAJ&hl=en) to help me with the [music synthesizer underlying Tulip](https://github.com/shorepine/amy). We quickly saw the power and fun of having an excellent analog and FM-style synth that could be controlled in Python on constrained real-time hardware. The Tulip you have in your hands is brand new, we constantly chase bugs, add new features, make new music and demos for it. There's a small but growing community of like minded people on our [Discord](https://discord.gg/TzBFkUb8pG) who try fun experiments as we build. 

**No one working on Tulip makes any money from it**. We're all hobbyists and engineers and scientists and musicians who enjoy working on a small project like this and we hope others can join us. Tulip was designed to be incredibly cheap and completely open source. You can even [build one yourself!](tulip_build.md) I've spent the initial money on PCB design to be able to get this version of Tulip in your hands, and you did pay a tiny amount extra over the raw cost of the parts and assembly needed to build Tulip, to support future design work from our partner Makerfabs. 

**Tulip is fun to use today but still needs help**. You should have fun hacking on and using Tulip, and never expect it to be perfect. You will run into lots of weirdness and challenges. We hope you find those constraints entertaining and inspiring. We're hoping to build a community of people that can work together to make Tulip better every day. Remember, if the Tulip firmware doesn't do it for you, you can always use the hardware you bought for your own creations. You can easily replace our firmware and flash it yourself using the ESP-IDF or Arduino.

If you have problems with Tulip, you can find us on GitHub [issues](https://github.com/shorepine/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG). We'll do our best to help out!

-- Brian


## Get used to the ports

Here's a legend to the ports, facing the front. 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulipv4r11_front.png" width=600>

## Acesssories you may want

The Tulip board you got can work on its own. Most people will want to add a couple of accessories to make it work even better. You should at least get a computer keyboard, the rest are optional. 

### USB keyboard

Tulip is a very command-line centered interface with some touch controls. You'll have a much easier time using Tulip if you connect a USB computer keyboard to it for typing. It turns the Tulip into a portable "deck" for whatever you can imagine. 

You can use _almost_ any USB computer keyboard you already have. However, a keyboard including an internal USB hub will not work. You can tell if a keyboard includes a hub if it has one or more subsidiary USB sockets (e.g. for connecting a mouse), or if it includes an integrated pointing device (trackpad, trackball etc). These keyboards are not compatible with Tulip. 

[If you want a more compact hardware keyboard, the tiny "cardKB" will work great with Tulip, plugged into the I2C port.](https://shop.m5stack.com/products/cardkb-mini-keyboard-programmable-unit-v1-1-mega8a)

### DACs or ADCs for modular synths

If you use modular synths, Tulip is a powerful and fun control surface and way to "program" your modular rig or older analog synth in Python. You can use the existing MIDI ports to do a lot, but if you have CV gear, we recommend getting a I2C DAC to control it. [You can get a 2 channel DAC with modular ready 3.5mm jacks from Makerfabs](https://www.makerfabs.com/mabee-dac-gp8413.html). You can use up to 8 at once for a total of 16 CV outputs!

[You can see a list other supported I2C devices, including ADCs to read CV output, that you can use with your Tulip.](tulip_api.md#i2c--grove--mabee)

### Type-A 3.5mm MIDI adapters to full size

Most modern MIDI equipment now uses 3.5mm jacks for MIDI connectors. If you have the older 5-pin DIN connectors and want to use them on Tulip, you'll want to pick up a couple of [Type-A 3.5mm adapters](https://www.amazon.com/Kurrent-Electric-Type-3-5mm-Adapter/dp/B0C2RLB3SL/). 

### A small li-po battery

Tulip is a low power device and can run on battery for portable computing. There's a standard JST connector on back for flat li-po or 18650 battery packs. I personally use [this 1200mAh battery](https://www.adafruit.com/product/258). If you want to keep it "inside" Tulip (between the back case and the PCB), you'll want to find a battery that is 5mm or so thin. You can use a piece of double sided tape it keep it inside the battery area of the PCB. 

Larger batteries will work great and last longer, but you'll have to remove the back case to make them fit (or mount the battery some other way.) [This 5200mAh battery pack](https://www.amazon.com/XINLANTECH-Rechargeable-Bluetooth-Electronic-Batteries/dp/B0C2VFTDPY) will last many many hours on Tulip and fit right on the PCB with the back case removed. 

### An extra Alles or five

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/nicoboard-alles.jpg" width=400>

Tulip supports using [Alles](http://github.com/shorepine/alles) as extra speakers. You control up to dozens of other speakers wirelessly using the AMY synthesizer for amazing multi-channel audio setups, using only a Tulip to control them! [You can get Alles PCBs from our friends at Blinkinlabs.](https://shop.blinkinlabs.com/products/alles-pcb)


## Get started with Tulip


<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulipv4r11back.png" width=600>


Now that you've got your Tulip, here's how to get started:

 - If you want to use a battery, open the back black case with a screwdriver and plug it in. If you do remove the back case, make sure not to bump the touchscreen connector labeled above as it's been known to be loose (we're working on gluing it down at the factory.) 
 - Connect a USB-C cable from a power adapter or your computer to the **top** USB connector, facing Tulip.
 - Plug in a USB keyboard to the **bottom** USB connector. 
 - Optionally wire up your stereo audio jack, MIDI and I2C connectors if you have them. Headphones will work fine on the audio jack if you don't have a mixer or other sound system. 
 - Turn on the Tulip! The switch is on the top of the board.
 - You should see the following. If you have audio connected, you should also hear a "bleep" sound on startup.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulip4r11firstboot.jpg" width=400>

 - Your USB keyboard should be able to type into this screen, called the "REPL". It can accept Python code and some simple system commands. Try `ls` and you should see a directory listing. 

### First, upgrade your Tulip! 

You should **upgrade the firmware on Tulip** whenever you receive it. You should also upgrade maybe once every week or so. We are constantly adding new features, fixing bugs, updating the API. To upgrade your Tulip, connect it to your Wi-Fi first:

 - `tulip.wifi('ssid', 'password')` will return an IP address if successful.
 - `tulip.upgrade()` will walk you through the upgrade. Accept both upgrades -- the system folder and the firmware. This process will take a few minutes in total. Your Tulip will reboot itself when it is done.

[If you don't have easy access to Wi-Fi, or want to flash Tulip from code we haven't released yet, you can also directly flash Tulip from your computer.](tulip_flashing.md)

### More quick start tips

 - Type `run('calibrate')` to calibrate the touch screen if you think it is needed. 
 - The black icon on the bottom right is a launcher, for quick access to some included programs. Tap on `Drums` and you should see (and hear!) a drum machine. 
 - Tap the top right blue "switch" icon to get back to the REPL. Tap it again to get back to the drum machine. You can also use `control-Tab`
 - Tap the red "quit" icon to quit the drum machine.
 - You can `edit('boot.py')` to add anything you want to run on Tulip startup, like wifi, or calibration, or setting up a synth. You can save in the editor with `control-X` and quit with `control-Q`. 
 - After joining Wi-Fi, try out Tulip World with `run('worldui')` or `world.ls()` to see files and messages posted by others! 
 - Other things to try:
   - Some games/animations: `run('bunny_bounce')` or `run('planet_boing')` or `run('parallax')` (use control-C to quit), 
   - Music apps: `run('voices')`, `run('juno6')`, `run('drums')`
   - Music demo: `run('xanadu')`
   - Other utlities: `run('wordpad')`, `run('buttons')`. 
 - The code for all of these are available, for you to modify or learn from. We ship a copy of `drums`, `voices` and `juno6` as `my_drums`, `my_voices`, `my_juno6` in `/sys/ex` if you want to edit those. The official versions are read-only and you can't mess them up! 
 - If you've got MIDI plugged in, try playing a note! Tulip boots with Juno-6 patch #0 on MIDI Channel 1. You can use the `voices` app to change that assignment. If you want your MIDI knobs / sliders to control things like the Juno-6 parameters in `run('juno6')`, run DAn's script to learn the sliders and update the midi mapping: `world.download('learn_midi_codes.py','dpwe')`
 - Try `world.download('mc_dance')` or `world.download('periodic2')` for some neat things by the Tulip community. 


Your next stop: [check out our making music on Tulip tutorial](music.md)

Then, [check out the API for making your own music, games, graphics with Tulip.](tulip_api.md)

[If you want to work on Tulip itself, check out our guide to compiling and flashing the Tulip firmware.](tulip_flashing.md)

If you find yourself wanting to write Tulip code on a "real" computer, check out [Tulip Desktop](tulip_desktop.md), which simulates the Tulip hardware. 

[![shore pine sound systems discord](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/shorepine100.png) **Chat about Tulip on our Discord!**](https://discord.gg/TzBFkUb8pG)

Have fun! See you on Tulip World!


## How-tos

### Transfer files between Tulip and your computer

Using WiFi on Tulip (start `tulip.wifi('ssid', 'password')` first), there are two ways of getting files between Tulip hardware and your computer:

 - **Use Tulip World and Tulip Desktop:** On [Tulip Desktop](tulip_desktop.md) your files are in your  `~/Documents/tulipcc/user` folder. You can edit and add files there directly and use `world.upload(folder)` or `world.upload(filename)` to place files on Tulip World from either Deskop or hardware Tulip. You can retrieve these files with `world.download(folder)` or `world.download(filename)`. The only downside to this method is that your files are publicly available to anyone else on Tulip World, so please don't share private information using Tulip World. 

 - **Use our file transfer script**: You can privately transfer files to and from a Tulip with our Python script [file_server.py](../tulip/shared/util/file_server.py). Download that script to your computer somewhere. Then, on your computer's terminal, run it, like `$ python file_server.py`. Wherever you run that script, all files in the folder you ran it from will be available to your Tulip. Use your computers' network manager to find out your computers' IP address (the `192.168.1.23` bit below is an example of it.)\
 \
 You can transfer any files **from** a computer to Tulip with `tulip.url_save(url, filename)` like: `tulip.url_save('http://192.168.1.23:8000/file.py', 'file.py')`. The `file.py` should be a file in the current directory that you've run `file_server.py` from.\
 \
 To transfer from a Tulip **to** a computer, use `tulip.url_put(url, file)` like `tulip.url_put('http://192.168.1.23:8000', 'file.py')` and it will appear in that folder on your computer. 

### Remap your non-US keyboard

Tulip uses low-level USB keyboard scan codes to translate from keypresses to ASCII to show on screen. We don't have the luxury of an OS to translate these for us across all the different locales of keyboards in the world! If your keyboard is not a US keyboard, you'll need to remap some keys. You can do it one at a time with `tulip.remap()`. If you have a German keyboard, Tulip community friend `olav` has contributed a keymap on Tulip World: get it from `world.download('keys_de.txt')`. You can see how to use it with their `boot.py`: `world.download('boot.py', 'olav')`

## Known problems

### Flashing on boot, or a message "Tulip flash not setup properly"

We "overclock" Tulip's RAM to 120MHz so it can support more polyphony. This works great but currently (we're working on it) has a downside where first cold boots often stall or hang in certain temperature conditions. If you see your Tulip flash or appear to reboot on first turn on (with the swtich), it should eventually settle down and be fine. Future reboots (with the `RESET` button or by `control-D`) will be fine as well. In rare instances, the cold boot may not load our file system properly. You'll see a message like "Tulip flash not setup properly." If you see that message, just hit `control-D` or the `RESET` button on top. It will go away and you'll be fine. We're working with the chip manufacturer on a more solid solution.

### Touchscreen not responding

If your touchscreen appears to not respond, the most likely explanation is the touchscreen panel cable got loose during shipping or if you installed a battery. We're working with Makerfabs to tape down this connector for higher reliability. But it's a quick fix if it happens to you. Just take off the back case, unseat the touch connector (labeled above) by pulling up the black clamp, and push back in the cable tight, applying a little pressure. Clamp it back, and take a piece of tape to hold down the cable right at the connector, like this: 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tuilpv4r11ctp.jpg" width=600>


### USB computer keyboard not working

If your USB computer keyboard is not working, the most likely explanation is that it appears as a "hub" instead of a bare keyboard to Tulip. At this time (we're working on it!) Tulip only supports "root" devices. You'll have to try another keyboard. If you think your keyboard should work, please find us on [issues](https://github.com/shorepine/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG) and we'll help out!






