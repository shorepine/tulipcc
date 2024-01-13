# Debugging a live ESP32-S3 HOWTO

We recently figured out how to use GDB on a live running ESP32S3, or how to to profile a running ESP32S3. 

I couldn't find a single HOWTO anywhere that tied everything together, and based 
on my challenge getting it going and my google searches I may be one of the few people outside of Espressif that has done this, so I thought I'd write a short guide. 

We use this to debug and profile code running in [Tulip](https://github.com/bwhitman/tulipcc). It's very helpful!

## You need

 - An ESP32S3 with the USB HOST pins broken out (pins 19 and 20.) Most dev boards do this. On Tulip, this is the USB HOST USB-C connector we use for keyboards. 
 - You also hopefully want a separate way to see the UART from the ESP. This is to see the serial monitor and optionally control your code. On Tulip this is the charging/serial USB port. 
 - The ESP-IDF toolchain installed and the environment set up (`export.sh`) in each terminal you use
 - Two USB cables. For some reason, the JTAG cable on our dev board had to be a USB-C -> A cable. C->C didn't work for JTAG, but it does for serial. 
 This may be our problem but keep it in mind.

## Set up

First just flash your program to the board as usual using UART / the first USB cable. 
It would help to use `-Og` in menuconfig Compiler Options for debugging, but it's not required.

On the first terminal window, open your serial monitor `idf.py monitor` and confirm everything is working and controllable. For Tulip, this means you can enter Python commands from the monitor. 
(Since you won't be able to attach a USB keyboard during debugging!)

While the program is running, connect the second USB cable to your computer. In a second terminal window, make sure you export.sh and then run

```c
% openocd -f board/esp32s3-builtin.cfg

Open On-Chip Debugger v0.12.0-esp32-20230921 (2023-09-21-13:27)
Licensed under GNU GPL v2
For bug reports, read
    http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselecting 'jtag'
Info : esp_usb_jtag: VID set to 0x303a and PID to 0x1001
Info : esp_usb_jtag: capabilities descriptor set to 0x2000
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : esp_usb_jtag: serial (84:FC:E6:6D:B0:D4)
Info : esp_usb_jtag: Device found. Base speed 40000KHz, div range 1 to 255
Info : clock speed 40000 kHz
Info : JTAG tap: esp32s3.cpu0 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : JTAG tap: esp32s3.cpu1 tap/device found: 0x120034e5 (mfg: 0x272 (Tensilica), part: 0x2003, ver: 0x1)
Info : starting gdb server for esp32s3.cpu0 on 3333
Info : Listening on port 3333 for gdb connections
....
```

Keep that terminal window open.

## To use GDB

Now you can run GDB. This is helpful when you get a crash and want to inspect the trace with more information than the default ESP stacktrace gives you.

Create a file called `gdbinit` that has this in it:

```
target remote :3333
set remote hardware-watchpoint-limit 2
mon reset halt
maintenance flush register-cache
thb app_main
c
```

Then start gdb and debug a crash. Here, gdb will pause your program when you start, so type `continue` and then manually cause the crash.
Here, we were debugging an AMY crash when changing synth patches, so I started the Python code to do that and waited. 
When the crash happens you can inspect the frame:

```c
% xtensa-esp32s3-elf-gdb -x gdbinit build/micropython.elf
...
Reading symbols from build/micropython.elf...
...
[esp32s3.cpu1] Target halted, PC=0x40382BFA, debug_reason=00000000
Thread 2 "main" hit Temporary breakpoint 1, app_main () at /Users/bwhitman/outside/tulipcc/tulip/tulipcc_r10/main.c:321
321 void app_main(void) {
(gdb) continue
Continuing.

...

Thread 9 "alles_fb_task" received signal SIGTRAP, Trace/breakpoint trap.
[Switching to Thread 1070397072]
0x42124e93 in render_lut (buf=0x3fcca67c, phase=0, step=6370381, incoming_amp=0, ending_amp=2338, lut=0x0) at /Users/bwhitman/outside/tulipcc/amy/src/oscillators.c:178
178     RENDER_LUT_PREAMBLE
(gdb) bt
#0  0x42124e93 in render_lut (buf=0x3fcca67c, phase=0, step=6370381, incoming_amp=0, ending_amp=2338, lut=0x0)
    at /Users/bwhitman/outside/tulipcc/amy/src/oscillators.c:178
#1  0x42039020 in render_sine (buf=0x3fcca67c, osc=<optimized out>) at /Users/bwhitman/outside/tulipcc/amy/src/oscillators.c:415
#2  0x4203568a in render_osc_wave (osc=3, core=0 '\000', buf=0x3fcca67c) at /Users/bwhitman/outside/tulipcc/amy/src/amy.c:979
#3  0x42035761 in amy_render (start=<optimized out>, end=<optimized out>, core=<optimized out>)
    at /Users/bwhitman/outside/tulipcc/amy/src/amy.c:998
#4  0x42032539 in esp_fill_audio_buffer_task () at /Users/bwhitman/outside/tulipcc/tulip/shared/alles.c:60
(gdb) frame 2
#2  0x4203568a in render_osc_wave (osc=3, core=0 '\000', buf=0x3fcca67c) at /Users/bwhitman/outside/tulipcc/amy/src/amy.c:979
979     if(synth[osc].wave == SINE) render_sine(buf, osc);
(gdb) print osc
$3 = 3
```

## To profile running code live

You can get a `gprof` profile of running code by percentage of time spent. This is very helpful for optimization / tuning. 
To do this, make sure `openocd` is still running and then, in a second terminal window, while you're doing the thing you want to profile on the chip:

```c
% telnet localhost 4444
...
Connected to localhost.
Escape character is '^]'.
Open On-Chip Debugger
> profile 10 gmon.out
Starting profiling. Halting and resuming the target as often as we can...
[esp32s3.cpu1] Target halted, PC=0x40387E09, debug_reason=00000000
Set GDB target to 'esp32s3.cpu1'
...
Profiling completed. 127 samples.
Wrote gmon.out
```

(I had to install telnet on my mac to get this to work - `brew install telnet` did it.)

`gmon.out` will get written to your local host computer wherever `openocd` is running. 

Now we need to process this `gmon.out` file using `gprof`. 

To do so, you need access to the compiled binary as well as `gmon.out`. For ESP-IDF, a compiled binary is an `elf` file, probably `build/X.elf`. This `elf` file needs some light modification before you give it to `gprof`. You need to copy it and then rename the section name `.flash.text` to `.text` so that gprof understands it. You can do that with a call to `xtensa-esp32s3-elf-objcopy`. 

Then you can run the included `gprof` that comes with ESP-IDF. `gprof` has tons of options, can draw pretty pictures to a PDF, etc, but i just like the default flat profile. 

So put together, here's how we process the `elf` file and then profile in Tulip:

```bash
% cp build/micropython.elf micropython_gprof.elf # copy the .elf as we have to modify it
% xtensa-esp32s3-elf-objcopy -I elf32-xtensa-le --rename-section .flash.text=.text micropython_gprof.elf
% xtensa-esp32s3-elf-gprof micropython_gprof.elf gmon.out

Flat profile:

Each sample counts as 0.0833333 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
 54.26      5.74     5.74                             spi_bus_lock_bg_check_dev_acq
 22.38      8.11     2.37                             spi_bus_lock_bg_clear_req
 13.39      9.53     1.42                             editor_down
  1.58      9.70     0.17                             prvAddNewTaskToReadyList
  1.58      9.86     0.17                             xt_ints_off
  1.56     10.03     0.17                             esp_restart_noos
  1.36     10.17     0.14                             spi_bus_lock_bg_check_dev_req
...
```

## USB unplug / reflash

I find that running JTAG sometimes messes up the UART flasher. 
If you get a message about `A fatal error occurred: Serial data stream stopped: Possible serial noise or corruption.` during flashing, 
just unplug the JTAG cable and reset the board and try again.
Presumably, you can use a single USB cable to the USB HOST pins to both flash using CDC and use JTAG after it boots. 
I had trouble getting this to work so recommend having a separate UART connected USB cable for flashing / monitoring.




