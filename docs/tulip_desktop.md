# Tulip Web and Desktop

We have two ways of running Tulip without building or buying [Tulip CC hardware](../README.md). This is useful for local development, or sharing your creations with people that don't have a Tulip of their own yet. 

[![Tulip Web](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/tulipweb.png)](https://tulip.computer/run/)

* [Tulip Web](https://tulip.computer/run) is the easiest way to get started on Tulip on any browser, or even mobile devices. Tulip Web keeps a local filesystem that's just for your code, and you can upload and download to Tulip World to get others creations. It supports everything a "real" Tulip does, including MIDI, audio, and graphics. 

* Tulip Desktop is a version of Tulip for desktop computers. We regularly make builds for macOS and it also supports many types of Linux machines, including Raspberry Pis. It also has been reported to work on Windows in WSL. 

It's hard for our small team to support all possible combinations of OSes and computers, so we'll be focusing our work on Tulip hardware and [Tulip Web.](https://tulip.computer/run) But read below for more information on compiling and running Tulip Desktop on your computer. 

## Tulip Desktop for macOS

![Tulip Desktop](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/desktop.png)

Tulip Desktop is the desktop computer version of [Tulip CC](../README.md) itself. It supports everything Tulip CC does, and simulates the display and hardware to the best of its ability. It's a great way to learn how to use Tulip or develop for Tulip itself without having to have access to hardware or wait for long flash cycles. 

You can download Tulip Desktop for [macOS universal build (Apple Silicon & Intel), 10.15 or later](https://github.com/shorepine/tulipcc/releases/).

### Compiling Tulip Desktop for macOS

If you want to develop or compile Tulip yourself, start by cloning this repository. 


```bash
git clone https://github.com/shorepine/tulipcc
cd tulipcc
```

To build Tulip Desktop (macOS 10.15 (Catalina) and higher, Apple Silicon or x86_64):

```bash
cd tulip/macos

# For local development (just your native arch, and shows stderr in terminal)
./build.sh
./dev/Tulip\ Desktop.app/Contents/MacOS/tulip

# Or package for distribution (creates a universal binary)
./package.sh # makes .app bundle in dist, not necessary if you're just using it locally
```

### Compiling Tulip Desktop for Linux (including Windows WSL2 and Raspberry Pi)

First, clone this repository and cd into it

```bash
git clone https://github.com/shorepine/tulipcc
cd tulipcc
```

Install the dependencies:

```bash
# Ubuntu, Debian, etc., including Windows 11 in WSL2
sudo apt install build-essential libsdl2-dev alsa-utils

# Fedora 40+, etc.
sudo dnf install gcc make SDL2-devel alsa-utils

# Arch
sudo pacman -S sdl2
```

Build and run:

```bash
cd tulip/linux
./build.sh
./dev/tulip
```

### Troubleshooting

Tulip on Mac OS X selects which audio device it will use for output when it starts. If you get no audio, you can check the available audio devices with

```shell
$ <path_to_app_bundle>/Tulip\ Desktop.app/Contents/MacOS/tulip -l
0 - DELL U2715H
1 - DELL U2715H
2 - External Headphones
3 - MacBook Pro Speakers
4 - Microsoft Teams Audio
5 - ZoomAudioDevice
```

And then select the device that you want to use with the `-d <device_number>` option, for example

```shell
$ <path_to_app_bundle>/Tulip\ Desktop.app/Contents/MacOS/tulip -d 2
```

For more details see <https://github.com/shorepine/tulipcc/issues/103>.

## Questions

Any questions? [Chat with us on our discussions page.](https://github.com/shorepine/tulipcc/discussions)

