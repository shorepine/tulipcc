# Tulip Desktop building and running

![Tulip Desktop](https://github.com/bwhitman/tulipcc/raw/main/pics/desktop.png)

Tulip Desktop should build and run on macOS 10.15 (Catalina, released 2019) and later, Apple Silicon & Intel supported. It also should be fine on Linux but I haven't yet tried. Windows is possible but probably needs some help. 

You don't need to compile your own Tulip Desktop unless you're a developer and want to work on Tulip. We regularly upload latest builds of Tulip Desktop for Mac. You can download it [here](latest)

To compile your Tulip Desktop, start by cloning this repository. 

On macOS, start by installing homebrew
```
# install homebrew first, skip this if you already have it...
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# Then restart your terminal
```

To build Tulip Desktop (macOS 10.15 (Catalina) and higher, Apple Silicon or x86_64):

```
cd ports/macos
brew install pkg-config

# For local development (just your native arch, and shows stderr in terminal)
./build.sh
./dev/Tulip\ CC.app/Contents/MacOS/tulip

# Or package for distribution (creates a universal binary)
./package.sh # makes .app bundle in dist, not necessary if you're just using it locally
```
