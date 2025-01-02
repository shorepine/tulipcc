# ulab

[![Documentation Status](https://readthedocs.org/projects/micropython-ulab/badge/?version=latest)](https://micropython-ulab.readthedocs.io/en/latest/index.html)

`ulab` is a `numpy`-like array manipulation library for [micropython](http://micropython.org/) and [CircuitPython](https://circuitpython.org/).
The module is written in C, defines compact containers (`ndarray`s) for numerical data of one to four
dimensions, and is fast. The library is a software-only standard `micropython` user module,
i.e., it has no hardware dependencies, and can be compiled for any platform. 8-, and 16-bit signed
and unsigned integer `dtypes`, as well as `float`, and, optionally, ` complex` are supported.
The `float` implementation of `micropython` (32-bit `float`, or 64-bit `double`) is automatically
detected and handled.

1. [Supported functions and methods](#supported-functions-and-methods)
    1. [ndarray methods](#ndarray-methods)
    2. [numpy and scipy functions](#numpy-and-scipy-functions)
    3. [ulab utilities](#ulab-utilities)
    4. [user module](#user-module)
4. [Usage](#usage)
5. [Finding help](#finding-help)
6. [Benchmarks](#benchmarks)
7. [Firmware](#firmware)
    1. [Customising the firmware](#customising-the-firmware)
    1. [Platforms including ulab](#platforms-including-ulab)
    1. [Compiling](#compiling)
        1. [UNIX](#unix-port)
        1. [STM-based boards](#stm-based-boards)
        1. [ESP32-based boards](#esp32-based-boards)
        1. [RP2-based boards](#rp2-based-boards)
        1. [Compiling for CircuitPython](#compiling-for-circuitpython)
8. [Issues, contributing, and testing](#issues-contributing-and-testing)
    1. [Testing](#testing)

# Supported functions and methods


## ndarray methods

`ulab` implements `numpy`'s `ndarray` with the `==`, `!=`, `<`, `<=`, `>`, `>=`, `+`, `-`, `/`, `*`, `**`,
`+=`, `-=`, `*=`, `/=`, `**=` binary operators, and the `len`, `~`, `-`, `+`, `abs` unary operators that
operate element-wise. Type-aware `ndarray`s can be initialised from any `micropython` iterable, lists of
iterables via the `array` constructor, or by means of the `arange`, `concatenate`, `diag`, `eye`,
`frombuffer`, `full`, `linspace`, `logspace`, `ones`, or `zeros`  functions.

`ndarray`s can be sliced, and iterated on, and have a number of their own methods, and properties, such as `flatten()`, `itemsize`, `reshape()`,
`shape`, `size`, `strides`, `tobytes()`, `tolist()`, and `transpose()` and `T`. If the firmware is compiled with `complex` support,
the `imag`, and `real` properties are automatically included.

## `numpy` and `scipy` functions

In addition, `ulab` includes [universal functions](https://micropython-ulab.readthedocs.io/en/latest/numpy-universal.html), [many `numpy` functions](https://micropython-ulab.readthedocs.io/en/latest/numpy-functions.html), and functions from the [`numpy.fft`](https://micropython-ulab.readthedocs.io/en/latest/numpy-fft.html), [`numpy.linalg`](https://micropython-ulab.readthedocs.io/en/latest/numpy-linalg.html), [`numpy.random`](https://micropython-ulab.readthedocs.io/en/latest/numpy-random.html), [`scipy.linalg`](https://micropython-ulab.readthedocs.io/en/latest/scipy-linalg.html), [`scipy.optimize`](https://micropython-ulab.readthedocs.io/en/latest/scipy-optimize.html), [`scipy.signal`](https://micropython-ulab.readthedocs.io/en/latest/scipy-signal.html), and [`scipy.special`](https://micropython-ulab.readthedocs.io/en/latest/scipy-special.html) modules. A complete list of available routines can be found under [micropython-ulab](https://micropython-ulab.readthedocs.io/en/latest).

## `ulab` utilities

The [`utils`](https://micropython-ulab.readthedocs.io/en/latest/ulab-utils.html) module contains functions for
interfacing with peripheral devices supporting the buffer protocol. These functions do not have an obvious
`numpy` equivalent, but share a similar programming interface, and allow direct data input-output between
numerical arrays and hardware components.

## `user` module

User-defined functions operating on numerical data can easily be added via the `user` module. This allows for transparent extensions, without having to change anything in the core. Hints as to how to work with `ndarray`s at the C level can be found in the [programming manual](https://micropython-ulab.readthedocs.io/en/latest/ulab-programming.html).

# Usage

`ulab` sports a `numpy/scipy`-compatible interface, which makes porting of `CPython` code straightforward. The following
snippet should run equally well in `micropython`, or on a PC.

```python
try:
    from ulab import numpy
    from ulab import scipy
except ImportError:
    import numpy
    import scipy.special

x = numpy.array([1, 2, 3])
scipy.special.erf(x)
```

# Finding help

Documentation can be found on [readthedocs](https://readthedocs.org/) under
[micropython-ulab](https://micropython-ulab.readthedocs.io/en/latest),
as well as at [circuitpython-ulab](https://circuitpython.readthedocs.io/en/latest/shared-bindings/ulab/__init__.html).
A number of practical examples are listed in Jeff Epler's excellent
[circuitpython-ulab](https://learn.adafruit.com/ulab-crunch-numbers-fast-with-circuitpython/overview) overview.
The [tricks](https://micropython-ulab.readthedocs.io/en/latest/ulab-tricks.html) chapter of the user manual discusses
methods by which RAM and speed can be leveraged in particular numerical problems.

# Benchmarks

Representative numbers on performance can be found under [ulab samples](https://github.com/thiagofe/ulab_samples).

# Firmware

Pre-built, and up-to-date firmware files for select platforms can be downloaded
from [micropython-builder](https://github.com/v923z/micropython-builder).
## Customising the firmware

If flash space is a concern, unnecessary functions can be excluded from the compiled firmware with
pre-processor switches. In addition, `ulab` also has options for trading execution speed for firmware size.
A thorough discussion on how the firmware can be customised can be found in the
[corresponding section](https://micropython-ulab.readthedocs.io/en/latest/ulab-intro.html#customising-the-firmware)
of the user manual.

## Platforms including ulab

`ulab` is also included in the following compiled `micropython` variants and derivatives:

1. `CircuitPython` for SAMD51 and nRF microcontrollers https://github.com/adafruit/circuitpython
1. `MicroPython for K210` https://github.com/loboris/MicroPython_K210_LoBo
1. `MaixPy` https://github.com/sipeed/MaixPy
1. `OpenMV` https://github.com/openmv/openmv
1. `pimoroni-pico` https://github.com/pimoroni/pimoroni-pico

## Compiling

If you want to try the latest version of `ulab` on `micropython` or one of its forks, the firmware can be compiled
from the source by following these steps:

### UNIX port

Simply clone the `ulab` repository with

```bash
git clone https://github.com/v923z/micropython-ulab.git ulab
```
and then run

```bash
./build.sh [matrix.dims] # Dimensions is 2 by default
```
This command will clone `micropython`, and build the `unix` port automatically, as well as run the test scripts. If you want an interactive `unix` session, you can launch it in

```bash
ulab/micropython/ports/unix
```

### STM-based boards

First, you have to clone the `micropython` repository by running

```bash
git clone https://github.com/micropython/micropython.git
```
on the command line. This will create a new repository with the name `micropython`. Staying there, clone the `ulab` repository with

```bash
git clone https://github.com/v923z/micropython-ulab.git ulab
```
If you don't have the cross-compiler installed, your might want to do that now, for instance on Linux by executing

```bash
sudo apt-get install gcc-arm-none-eabi
```

If this step was successful, you can try to run the `make` command in the port's directory as

```bash
make BOARD=PYBV11 USER_C_MODULES=../../../ulab all
```
which will prepare the firmware for pyboard.v.11. Similarly,

```bash
make BOARD=PYBD_SF6 USER_C_MODULES=../../../ulab all
```
will compile for the SF6 member of the PYBD series. If your target is `unix`, you don't need to specify the `BOARD` parameter.

Provided that you managed to compile the firmware, you would upload that by running either

```bash
dfu-util --alt 0 -D firmware.dfu
```
or

```bash
python pydfu.py -u firmware.dfu
```

In case you got stuck somewhere in the process, a bit more detailed instructions can be found under https://github.com/micropython/micropython/wiki/Getting-Started, and https://github.com/micropython/micropython/wiki/Pyboard-Firmware-Update.


### ESP32-based boards

`ulab` can be tested on the ESP32 in [wokwi's micropython emulator](https://wokwi.com/arduino/projects/322114140704342610) without having to compile the C code. This utility also offers the possibility to save and share your `micropython` code.

Firmware for `Espressif` hardware can be built in two different ways, which are discussed in the next two paragraphs. A solution for issues with the firmware size is outlined in the [last paragraph](#what-to-do-if-the-firmware-is-too-large) of this section.

#### Compiling with cmake

Beginning with version 1.15, `micropython` switched to `cmake` on the ESP32 port. If your operating system supports `CMake > 3.12`, you can either simply download, and run the single [build script](https://github.com/v923z/micropython-ulab/blob/master/build/esp32-cmake.sh), or follow the step in this section. Otherwise, you should skip to the [next one](#compiling-with-make), where the old, `make`-based approach is discussed.

In case you encounter difficulties during the build process, you can consult the (general instructions for the ESP32)[https://github.com/micropython/micropython/tree/master/ports/esp32#micropython-port-to-the-esp32].

First, clone the `ulab`, the `micropython`, as well as the `espressif` repositories:

```bash
export BUILD_DIR=$(pwd)

git clone https://github.com/v923z/micropython-ulab.git ulab
git clone https://github.com/micropython/micropython.git

cd $BUILD_DIR/micropython/

git clone -b v4.0.2 --recursive https://github.com/espressif/esp-idf.git

```
Also later releases of `esp-idf` are possible (e.g. `v4.2.1`).

Then install the `ESP-IDF` tools:

```bash
cd esp-idf
./install.sh
. ./export.sh
```

Next, build the `micropython` cross-compiler, and the `ESP` sub-modules:

```bash
cd $BUILD_DIR/micropython/mpy-cross
make
cd $BUILD_DIR/micropython/ports/esp32
make submodules
```
At this point, all requirements are installed and built. We can now compile the firmware with `ulab`. In `$BUILD_DIR/micropython/ports/esp32` create a `makefile` with the following content:

```bash
BOARD = GENERIC
USER_C_MODULES = $(BUILD_DIR)/ulab/code/micropython.cmake

include Makefile
```
You specify with the `BOARD` variable, what you want to compile for, a generic board, or `TINYPICO` (for `micropython` version >1.1.5, use `UM_TINYPICO`), etc. Still in `$BUILD_DIR/micropython/ports/esp32`, you can now run `make`.

#### Compiling with make

If your operating system does not support a recent enough version of `CMake`, you have to stay with `micropython` version 1.14. The firmware can be compiled either by downloading and running the [build script](https://github.com/v923z/micropython-ulab/blob/master/build/esp32.sh), or following the steps below:

First, clone `ulab` with

```bash
git clone https://github.com/v923z/micropython-ulab.git ulab
```

and then, in the same directory, `micropython`

```bash
git clone https://github.com/micropython/micropython.git
```

At this point, you should have `ulab`, and `micropython` side by side.

With version 1.14, `micropython` switched to `cmake` on the `ESP32` port, thus breaking compatibility with user modules. `ulab` can, however, still be compiled with version 1.14. You can check out a particular version by pinning the release tag as

```bash

cd ./micropython/
git checkout tags/v1.14

```
Next, update the submodules,

```bash
git submodule update --init
cd ./mpy-cross && make # build cross-compiler (required)
```
and find the ESP commit hash

```bash
cd ./micropython/ports/esp32
make ESPIDF= # will display supported ESP-IDF commit hashes
# output should look like: """
# ...
# Supported git hash (v3.3): 9e70825d1e1cbf7988cf36981774300066580ea7
# Supported git hash (v4.0) (experimental): 4c81978a3e2220674a432a588292a4c860eef27b
```

Choose an ESPIDF version from one of the options printed by the previous command:

```bash
ESPIDF_VER=9e70825d1e1cbf7988cf36981774300066580ea7
```

In the `micropython` directory, create a new directory with
```bash
mkdir esp32
```
Your `micropython` directory should now look like

```bash
ls
ACKNOWLEDGEMENTS    CONTRIBUTING.md  esp32     lib      mpy-cross  README.md
CODECONVENTIONS.md  docs             examples  LICENSE  ports      tests
CODEOFCONDUCT.md    drivers          extmod    logo     py         tools
```

In `./micropython/esp32`, download the software development kit with

```bash
git clone https://github.com/espressif/esp-idf.git esp-idf
cd ./esp-idf
git checkout $ESPIDF_VER
git submodule update --init --recursive # get idf submodules
pip install -r ./requirements.txt # install python reqs
```

Next, still staying in `./micropython/eps32/esd-idf/`, install the ESP32 compiler. If using an ESP-IDF version >= 4.x (chosen by `$ESPIDF_VER` above), this can be done by running `. $BUILD_DIR/esp-idf/install.sh`. Otherwise, for version 3.x, run the following commands in in `.micropython/esp32/esp-idf`:

```bash
# for 64 bit linux
curl https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xvz

# for 32 bit
# curl https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xvz

# don't worry about adding to path; we'll specify that later

# also, see https://docs.espressif.com/projects/esp-idf/en/v3.3.2/get-started for more info
```

Finally, build the firmware:

```bash
cd ./micropython/ports/esp32
# temporarily add esp32 compiler to path
export PATH=../../esp32/esp-idf/xtensa-esp32-elf/bin:$PATH
export ESPIDF=../../esp32/esp-idf # req'd by Makefile
export BOARD=GENERIC # options are dirs in ./boards
export USER_C_MODULES=../../../ulab # include ulab in firmware

make submodules & make all
```

If it compiles without error, you can plug in your ESP32 via USB and then flash it with:

```bash
make erase && make deploy
```

#### What to do, if the firmware is too large?

When selecting `BOARD=TINYPICO`, the firmware is built but fails to deploy, because it is too large for the standard partitions. We can rectify the problem by creating a new partition table. In order to do so, in `$BUILD_DIR/micropython/ports/esp32/`, copy the following 8 lines to a file named `partitions_ulab.cvs`:

```
# Notes: the offset of the partition table itself is set in
# $ESPIDF/components/partition_table/Kconfig.projbuild and the
# offset of the factory/ota_0 partition is set in makeimg.py
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x6000,
phy_init, data, phy,     0xf000,  0x1000,
factory,  app,  factory, 0x10000, 0x200000,
vfs,      data, fat,     0x220000, 0x180000,
```
This expands the `factory` partition by 128 kB, and reduces the size of `vfs` by the same amount. Having defined the new partition table, we should extend `sdkconfig.board` by adding the following two lines:

```
CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions_ulab.csv"
```
This file can be found in `$BUILD_DIR/micropython/ports/esp32/boards/TINYPICO/`. Finally, run `make clean`, and `make`. The new firmware contains the modified partition table, and should fit on the microcontroller.

### RP2-based boards

RP2 firmware can be compiled either by downloading and running the single [build script](https://github.com/v923z/micropython-ulab/blob/master/build/rp2.sh)/[build script for Pico W](https://github.com/v923z/micropython-ulab/blob/master/build/rp2w.sh), or executing the commands below.

First, clone `micropython`:

```bash
git clone https://github.com/micropython/micropython.git
```

Then, setup the required submodules:

```bash
cd micropython
git submodule update --init lib/tinyusb
git submodule update --init lib/pico-sdk
cd lib/pico-sdk
git submodule update --init lib/tinyusb
```

You'll also need to compile `mpy-cross`:

```bash
cd ../../mpy-cross
make
```

That's all you need to do for the `micropython` repository. Now, let us clone `ulab` (in a directory outside the micropython repository):

```bash
cd ../../
git clone https://github.com/v923z/micropython-ulab ulab
```

With this setup, we can now build the firmware. Back in the `micropython` repository, use these commands:

```bash
cd ports/rp2
make USER_C_MODULES=/path/to/ulab/code/micropython.cmake
```

If `micropython` and `ulab` were in the same folder on the computer, you can set `USER_C_MODULES=../../../ulab/code/micropython.cmake`. The compiled firmware will be placed in `micropython/ports/rp2/build`.

# Compiling for CircuitPython

[Adafruit Industries](www.adafruit.com) always include a relatively recent version of `ulab` in their nightly builds. However, if you really need the bleeding edge, you can easily compile the firmware from the source. Simply clone `circuitpython`, and move the commit pointer to the latest version of `ulab` (`ulab` will automatically be cloned with `circuitpython`):

```bash
git clone https://github.com/adafruit/circuitpython.git

cd circuitpyton/extmod/ulab

# update ulab here
git checkout master
git pull
```
You might have to check, whether the `CIRCUITPY_ULAB` variable is set to `1` for the port that you want to compile for. You find this piece of information in the `make` fragment:

```bash
circuitpython/ports/port_of_your_choice/mpconfigport.mk
```
After this, you would run `make` with the single `BOARD` argument, e.g.:

```bash
make BOARD=mini_sam_m4
```

# Issues, contributing, and testing

If you find a problem with the code, please, raise an [issue](https://github.com/v923z/micropython-ulab/issues)! An issue should address a single problem, and should contain a minimal code snippet that demonstrates the difference from the expected behaviour. Reducing a problem to the bare minimum significantly increases the chances of a quick fix.

Feature requests (porting a particular function from `numpy` or `scipy`) should also be posted at [ulab issue](https://github.com/v923z/micropython-ulab/issues).

Contributions of any kind are always welcome. If you feel like adding to the code, you can simply issue a pull request. If you do so, please, try to adhere to `micropython`'s [coding conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md#c-code-conventions).

However, you can also contribute to the documentation (preferably via the [jupyter notebooks](https://github.com/v923z/micropython-ulab/tree/master/docs), or improve the [tests](https://github.com/v923z/micropython-ulab/tree/master/tests).

## Testing

If you decide to lend a hand with testing, here are the steps:

1. Write a test script that checks a particular function, or a set of related functions!
1. Drop this script in one of the folders in [ulab tests](https://github.com/v923z/micropython-ulab/tree/master/tests)!
1. Run the [./build.sh](https://github.com/v923z/micropython-ulab/blob/master/build.sh) script in the root directory of `ulab`! This will clone the latest `micropython`, compile the firmware for `unix`, execute all scripts in the `ulab/tests`, and compare the results to those in the expected results files, which are also in `ulab/tests`, and have an extension `.exp`. In case you have a new snippet, i.e., you have no expected results file, or if the results differ from those in the expected file, a new expected file will be generated in the root directory. You should inspect the contents of this file, and if they are satisfactory, then the file can be moved to the `ulab/tests` folder, alongside your snippet.
