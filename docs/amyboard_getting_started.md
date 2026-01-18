# Getting started with the AMyboard

Welcome to the AMYboard. Here you'll find out all about your new [AMYboard that you bought from Makerfabs.](https://amyboard.com)

If you have problems with AMYboard, you can find us on GitHub [issues](https://github.com/shorepine/tulipcc/issues) or the [Discord](https://discord.gg/TzBFkUb8pG). We'll do our best to help out!

-- DAn and Brian 


## Get used to the board

Here's a legend to the board, facing the front:

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulipv4r11_front.png" width=600>

And the back:

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulipv4r11_front.png" width=600>


## Acesssories you may want

### Type-A 3.5mm MIDI adapters to full size

We support both USB MIDI and "standard" TRS MIDI. Most modern MIDI equipment now uses 3.5mm jacks for MIDI connectors. If you have the older 5-pin DIN connectors and want to use them on Tulip, you'll want to pick up a couple of [Type-A 3.5mm adapters](https://www.amazon.com/Kurrent-Electric-Type-3-5mm-Adapter/dp/B0C2RLB3SL/). 



## Get started with AMYboard


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
 - If you've got MIDI plugged in, try playing a note! Tulip boots with Juno-6 patch #0 on MIDI Channel 1. You can use the `voices` app to change that assignment. If you want your MIDI knobs / sliders to control things like the Juno-6 parameters in `run('juno6')`, run a script to learn the sliders and update the midi mapping: `import learn_midi_codes`. Note you'll need to "learn" at least 13 buttons, 8 knobs and 9 sliders for full control of Juno. 
 - Try `world.download('mc_dance')`, `world.download('tracks')`, or `world.download('periodic2')` for some neat things by the Tulip community. 


Your next stop: [check out our making music on Tulip tutorial](music.md)

Then, [check out the API for making your own music, games, graphics with Tulip.](tulip_api.md)

If you're having any issues with your Tulip, please check out our [troubleshooting guide.](troubleshooting.md)

[If you want to work on Tulip itself, check out our guide to compiling and flashing the Tulip firmware.](tulip_flashing.md)

If you find yourself wanting to write Tulip code on a "real" computer, check out [Tulip Web](https://tulip.computer/run) and [Tulip Desktop](tulip_desktop.md), which simulates the Tulip hardware, or, see below how to transfer and edit files remotely.

## How to transfer files between Tulip and your computer

There are a few ways to files between Tulip and your computer. You can connect Tulip to your computer over a USB cable (the top USB port, the charging/UART/power port) and use a program called `mpremote`, or you can do it wirelessly by turning on Wi-Fi on the Tulip and using our Tulip World BBS or setting up a little server on your computer. 

### Using `mpremote`

[`mpremote`](https://docs.micropython.org/en/latest/reference/mpremote.html) is provided by the MicroPython project to be able to control and interact with MicroPython on many kinds of devices, including Tulip! It connects to Tulip over the UART USB connection and you are able to use the Tulip REPL, edit, upload and download files. (It does a lot more, but we'll just discuss file transfer here.)

**You may need to install the USB->Serial driver first for your computer's OS.** Some OSes already have it installed and some don't. If `mpremote` can't find a port, [try to install the CH340K drivers.](https://www.wch-ic.com/downloads/CH341SER_ZIP.html). 

First, install `mpremote`on your computer. Usually, this is (in your Terminal app) `pip install mpremote`. (If you don't have `pip`, please search for how to install it on your OS.) Once it's installed, make sure you've connected a USB cable from Tulip's top USB port - "USB pwr/charge/program" - to your computer. Then, with Tulip on, just run `mpremote` on your computer's terminal. You should see the Tulip's REPL on your screen! You can type into it and see it there. It's a fine way to control almost all of Tulip. 

To transfer a file from your computer to Tulip from mpremote, use `mpremote resume fs cp local_file.py :tulip_file.py`. This takes a file called `local_file.py` on your computer in the folder you're in, and transfers it to the current Tulip directory, calling it `tulip_file.py`. It's pretty fast! To transfer a file from Tulip to your computer, do it in reverse: `mpremote resume fs cp :tulip_file.py local_file.py`. 

To edit a file on Tulip directly on your computer, just do `mpremote resume edit file.py`. This will open whatever `EDITOR` you have set in your shell's environment. Once you save it in your editor, it'll be saved on Tulip! 

[See a lot more about `mpremote`](https://docs.micropython.org/en/latest/reference/mpremote.html). Note that we've only confirmed Tulip to work with the `fs`, `mount` and `repl` commands so far.  And on Tulip, you always need to add `resume` to your `mpremote` command. If you have any issues let us know! 

## Using Wi-Fi

With Wi-Fi, after you do `tulip.wifi('ssid', 'password')` first, there are two ways of getting files between Tulip hardware and your computer:

 - **Use Tulip World and Tulip Desktop:** On [Tulip Desktop](tulip_desktop.md) your files are in your  `~/.local/share/tulipcc/user` folder. You can edit and add files there directly and use `world.upload(folder)` or `world.upload(filename)` to place files on Tulip World from either Deskop or hardware Tulip. You can retrieve these files with `world.download(folder)` or `world.download(filename)`. The only downside to this method is that your files are publicly available to anyone else on Tulip World, so please don't share private information using Tulip World. 

 - **Use our file transfer script**: You can privately transfer files to and from a Tulip with our Python script [file_server.py](../tulip/shared/util/file_server.py). Download that script to your computer somewhere. Then, on your computer's terminal, run it, like `$ python file_server.py`. Wherever you run that script, all files in the folder you ran it from will be available to your Tulip. Use your computers' network manager to find out your computers' IP address (the `192.168.1.23` bit below is an example of it.)\
 \
 You can transfer any files **from** a computer to Tulip with `tulip.url_save(url, filename)` like: `tulip.url_save('http://192.168.1.23:8000/file.py', 'file.py')`. The `file.py` should be a file in the current directory that you've run `file_server.py` from.\
 \
 To transfer from a Tulip **to** a computer, use `tulip.url_put(url, file)` like `tulip.url_put('http://192.168.1.23:8000', 'file.py')` and it will appear in that folder on your computer.

 
[![shore pine sound systems discord](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/shorepine100.png) **Chat about Tulip on our Discord!**](https://discord.gg/TzBFkUb8pG)

Have fun! See you on Tulip World!


