# Tulip Desktop

![Tulip Desktop](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pics/desktop.png)

Tulip Desktop is the desktop computer version of [Tulip CC](../README.md) itself. It supports everything Tulip CC does, and simulates the display and hardware to the best of its ability. It's a great way to learn how to use Tulip or develop for Tulip itself without having to have access to hardware or wait for long flash cycles. 

You can download Tulip Desktop here:

 * [macOS universal build (Apple Silicon & Intel), 10.15 or later](https://github.com/shorepine/tulipcc/releases/)
 * For Linux, see the build instructions below
 * Windows-native build is forthcoming, but it works under WSL using the Linux instructions!

You don't need to compile your own Tulip Desktop unless you're a developer and want to work on Tulip (or on a platform we haven't uploaded a build for.) We regularly upload latest builds of Tulip Desktop for macOS.

## macOS build of Tulip Desktop 

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

## Linux (including Windows WSL2) build of Tulip Desktop

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


## Windows build of Tulip Desktop

We've heard this works fine under WSL2 using the Linux instructions. We'd love a native build!

## Troubleshooting

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
