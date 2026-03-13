# Using AMYboard Online

The AMYboard web editor at [amyboard.com](https://amyboard.com/editor) lets you design patches, write Python code, and manage your AMYboard -- all from your browser. You can also use it as a standalone browser-based synthesizer without any hardware at all.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/amyboard_online.png" width=600>


## Getting started

Just open [amyboard.com](https://amyboard.com/editor) in Chrome, Edge, or Firefox (Safari works but MIDI will not work there). Click anywhere on the page to unlock audio, and you're ready to go.

## What you can do online

### Play and edit patches

The web editor gives you a knob-per-function interface for Juno-6 like patches

 - Browse and load any of the 128 patches into the interface
 - Tweak every parameter with on-screen knobs
 - Assign patches to any of the 16 MIDI channels 
 - Set min & max & CC assignments per knob (click on the knob title)

### Use Web MIDI

If you have a MIDI controller and a browser that supports WebMIDI, AMYboard Online can use it directly:

 - Click the MIDI device selector to choose your input and output devices
 - Play notes on your controller and hear them through AMYboard's synth engine
 - MIDI CC messages can control patch parameters in real time

### Write Python code

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/amyboard_online_code.png" width=600>


AMYboard Online includes a full MicroPython REPL running in your browser. You can:

 - Type Python code to control the synth
 - Write and test scripts before deploying them to hardware

### Transfer environments to hardware

If you have an AMYboard connected via USB:

 1. Click **Send to AMYboard** in the web editor
 2. Your current patch assignments, modified patches, and any audio files or python files like `env.py` script are packed into an archive and transferred over MIDI SysEx
 3. The hardware AMYboard unpacks and applies everything automatically, then reboots

This is the easiest way to set up your hardware AMYboard from a comfortable editing environment.

Make sure your MIDI out is set your AMYboard! Either directly over USB or via a MIDI cable, choose the correct MIDI output first.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/send_to_amyboard.png" width=600>

### AMYboard World

AMYboard World is a community sharing hub built into the web editor. You can browse, download, and upload both full environments and individual patches.

 - **Environments**: Complete setups including patch assignments, modified patches, audio files, and `env.py` scripts. Import one and your entire editor state is replaced with the shared environment.
 - **Patches**: Individual `.patch` files you can preview and import into any channel. Click a patch to hear it before committing.

To share your own work:

 1. Give your environment a name and fill in your username and a short description
 2. Click **Upload** to publish it to AMYboard World
 3. Others can find it by browsing or searching

[Back to Getting Started](index.md)
