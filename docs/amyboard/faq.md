# AMYboard FAQ

Real questions from the [#amyboard Discord](https://discord.gg/TzBFkUb8pG), with answers from the team. If your question isn't here, hop on Discord -- the `#amyboard` channel is active and friendly.

See also: [Getting Started](README.md) · [Troubleshooting](troubleshooting.md) · [Firmware](firmware.md) · [Accessories](accessories.md) · [Modular](modular.md) · [Python](python.md) · [Online editor](online.md) · [Arduino](arduino.md)

---

## Buying & the board itself

### Who makes the AMYboard and how do I buy one?

AMYboard is an open source project staffed by volunteers. We worked with [Makerfabs](https://www.makerfabs.com/amyboard.html) to design and produce the AMYboard. Makerfabs handles all fulfilment and purchasing. Contact them with any order related questions! 

### What comes with the $29 AMYboard? Does it need assembly / soldering?

AMYboard comes with 3 things in a box:
 * A fully assembled AMYboard -- all parts are pre-soldered, including 3.5mm jacks
 * A plexiglass / acrylic front panel for mounting in 10HP modular setups. The panel has cutouts for an optional display and optional knob. 
 * Nuts for the 10 3.5mm jacks that screw on over the panel, if you want to attach the panel to the board.
 
 The board ships from [Makerfabs](https://amyboard.com/#get). The only "assembly" is snapping on the faceplate and fastening the jack nuts if you want to rack it, along with any optional modules you want to install.

### Are AMYboards in stock? 

Boards are sold through Makerfabs and they ship in "batches" based on order sizes. For example, before AMYboard launched, they made a small batch, which sold out quickly at launch, so they made a bigger batch. Batches take a couple of weeks for them to manufacture boards. You should look at the [AMYboard page on Makerfabs](https://www.makerfabs.com/amyboard.html) to see the latest shipping estimate. 

### How do I check my order or shipping status? (And the Makerfabs login won't work!)

All orders, payments, and shipping are handled by [Makerfabs](https://www.makerfabs.com/amyboard.html) -- we (the AMYboard team) can't look up or change orders. For anything about your order or delivery:

- **Order & tracking questions:** use the [Makerfabs contact page](https://www.makerfabs.com/contact) -- they're responsive.
- **Tracking number:** it arrives in a **separate email** from your order confirmation, often a little later. Check your inbox (and spam) before assuming nothing shipped.
- **Can't log in to the Makerfabs site?** Their login has been intermittent lately -- let the page fully load and try a couple of times.

### How do I power it?

Three ways, and AMYboard uses the highest voltage present: **USB-C** (5V), **Eurorack 10-pin** (+12V), or the **I2C host / Grove** port (3.3V, e.g. from a Tulip). You don't need a computer -- a plain USB power supply is fine. See [Power Supplies](README.md#power-supplies).

---

## Connecting over USB

### My AMYboard only shows up as a "USB JTAG/serial debug unit" -- no MIDI or serial port. (Most common question!)

If AMYboard *only ever* appears as a USB/JTAG device, **it isn't running** -- it's sitting in DFU (firmware-update) mode, waiting. To confirm and fix:

1. Connect a serial terminal (`mpremote`, `minicom`, PuTTY, or `screen`) to the port. If you see something like `waiting for download`, it's in DFU mode.
2. Press **BOOT** to start the firmware. You should get a MicroPython `>>>` prompt, and the board will then enumerate its USB serial (CDC) and USB MIDI ports.
3. After flashing firmware, the sequence to leave DFU and run the new firmware is **RST, then BOOT**.

A working board enumerates as a USB MIDI device (VID `0xCAF0`, PID `0x4009`) plus a USB serial port. See [USB MIDI not recognized](troubleshooting.md#usb-midi-not-recognized).

### The online editor / "Control" can't find my AMYboard

The editor talks to the board over **WebSerial/WebMIDI**, which only work in **Chrome or Edge** (desktop). Safari and Firefox don't support them, so the board won't show up at all. Also plug the board in and finish any DFU/BOOT step **before** clicking Connect/Control, and pick the right port when the browser prompts.

If the **Update firmware** button shows something like "amyboard.com refused to connect," clear your browser cache and reload the page -- a stale cached page has caused exactly this.

### It works on Mac/Linux but not Windows (or the reverse)

Most setups work across macOS, Linux, and Windows, especially after a firmware update -- we've fixed a lot of USB issues recently, so [upgrade to the latest firmware](firmware.md) first. If one machine sees the board and another doesn't:

- First make sure it isn't just stuck in **DFU mode** -- press **BOOT** to run the firmware (see the first question above).
- Try a known **data** USB-C cable -- many cables are charge-only and carry no data.
- Make sure **nothing else is holding the port** (close `mpremote`, serial terminals, the Python REPL, any DAW).
- On **Windows**, check Device Manager under **Sound, video and game controllers** for "AMYboard".
- On **Linux**, add your user to the `dialout` group, then e.g. `screen /dev/ttyACM0 115200`.

Still stuck on a particular machine? **Ask on the [Discord](https://discord.gg/TzBFkUb8pG) `#amyboard` channel** -- it's the fastest way to get help, and your report helps us catch any remaining edge cases.

### Can I plug a USB-MIDI keyboard straight into the AMYboard? (USB host)

No -- AMYboard is **USB device/gadget mode only**: it appears to a computer as a MIDI device, but it **can't host** a USB controller plugged into it. (Tulip does USB host; AMYboard's hardware doesn't.) To play it from a keyboard without a computer, use the **TRS/DIN MIDI in** jack, or put a small **USB-host-to-TRS-MIDI bridge** in between -- you can build one from an RP2040/RP2350 board (a community member did exactly that), or buy a ready-made box like the [CME H2MIDI Pro](https://www.amazon.com/dp/B0DQYD3L7D) (~$49).

---

## Displays (OLED)

### Which displays are supported?

Out of the box our firmware supports **SH1107** and **SSD1327** OLEDs. The Adafruit 128×128 monochrome OLED ([#4741](https://www.adafruit.com/product/4741)) is an SH1107 and works great -- by default the AMY version number shows on it at boot, so you know right away it's wired correctly.

**If the Adafruit display is out of stock**, any **SH1107 128×128** OLED works -- community members have sourced the same panel from **Pimoroni** and **Mouser**. Non-Adafruit ones usually come with a bare 4-pin header instead of a JST/STEMMA socket, so you'll need a **Dupont-to-Grove/STEMMA** cable to connect it (only the Adafruit part is JST-to-JST).

### My SSD1306 / off-brand OLED doesn't light up

- **SSD1306** isn't supported in firmware (different driver), but you can drop a MicroPython driver onto the board and use it yourself -- e.g. [micropython-ssd1306](https://github.com/stlehmann/micropython-ssd1306/blob/master/ssd1306.py). Copy it to `/user/current/` and `import` it from your sketch.
- **Off-brand OLEDs** can use different driver chips and I2C addresses. `amyboard.py` tries to guess, but you may need to edit the default address in `ssd1327_oled` / `sh1107_oled`, or supply a driver for your chip.

### My display cracked or went blank after I mounted it

Be very careful bolting a display to the faceplate -- hand-tightening can press the glass against the panel and crack it. **Use spacers/standoffs** (extra nuts work great) so there's no force on the glass. This has bitten more than one of us. The mounting holes take **M2** screws; a cheap assortment of M2 screws plus nylon spacers and washers works well, and using just **two diagonal holes** instead of all four gives a bit more tolerance.

### My OLED image is rotated 90°

Some panels (especially off-brand ones) come up sideways. Fix it in your sketch:

```python
amyboard.set_display_rotation(90)   # or 180 / 270
```

### My OLED text shows nothing, or my custom screen gets overwritten

- Use color value **1** (not 255) for text on these OLEDs.
- Don't refresh the display from a tight `while` loop or update it too often -- the I2C bandwidth will eventually **starve the audio and can crash the board**. Update one thing at a time inside your sketch's `loop()`. See the `preset_selector`, `house_generator`, and `acid_generator` examples for the right pattern.
- If the default screen keeps coming back, something in your own display setup isn't running; try a known-good OLED example first.

---

## Knobs, encoders & I2C accessories

### What knobs/encoders work, and how do I connect them?

The front-panel **I2C (Grove)** port drives [accessories](accessories.md). Popular choices: Adafruit's single Seesaw rotary encoder, the **quad rotary encoder ([#5752](https://www.adafruit.com/product/5752))**, and various M5Stack units. To connect Adafruit Stemma QT parts you need:

- a **Grove-to-Stemma QT** cable from the AMYboard front panel to the first accessory ([Adafruit #4528](https://www.adafruit.com/product/4528)), and
- a **Stemma-to-Stemma QT** cable to daisy-chain the next accessory (e.g. knob → display).

Use the **front-panel** I2C port for accessories -- not the back "tulip"/host port. The back I2C connector is for running the AMYboard itself *as* an accessory (daisy-chained, or driven from a Tulip or similar host), and the back HOST header pins are for serial debugging during firmware development -- neither will talk to your display or encoder.

### What exact I2C connector/cable does the front-panel port use?

It's a **Grove / MABEE-style** 4-pin connector at **2.0 mm** pitch (also called HY2.0 / DL20005) -- **not** STEMMA QT, which is the smaller 1.0 mm JST-SH. A plain STEMMA QT cable won't plug in directly, so use a **Grove-to-STEMMA QT** cable ([Adafruit #4528](https://www.adafruit.com/product/4528)) to adapt. Most MicroPython I2C drivers for these parts "just work" -- copy the driver `.py` into `/user/current/` and `import` it.

### How do I split or daisy-chain several I2C accessories?

The quad encoder has two Stemma QT jacks so you can chain through it. For a star/hub, M5Stack sells a [mini I2C hub](https://shop.m5stack.com/products/mini-hub-module). You can run many accessories off the single SDA/SCL pair as long as their addresses don't collide.

### Is there one API that works with any encoder? (Adafruit single/quad, M5Stack 8-Encoder)

Yes -- as of the 2026-06-30 firmware, `amyboard.encoder()` autodetects whichever encoder accessory is connected (or the web simulator's emulated encoder) and gives you a single API, so the same sketch works with any of them:

```python
enc = amyboard.encoder()   # autodetects via I2C scan (or the web simulator)
enc.type          # "adafruit_single" | "adafruit_quad" | "m5stack" | "web" | None
enc.encoders      # 1, 4, or 8
enc.read(i)       # cumulative position, starts at 0
enc.button(i)     # True while held (normalized across devices)
enc.led(i, r,g,b) # set encoder i's LED, 0..255 each
enc.reset(i)      # zero one encoder (or all if i omitted)
enc.switch()      # M5Stack toggle (False on other devices)
```

The sketch generator on AMYboard World uses this API too, so generated sketches no longer assume a specific encoder. Note that many older World sketches were written for a *specific* encoder -- if a sketch "runs but nothing happens," check whether it expects a single encoder for preset selection while you only have the 8-pack (or vice versa).

### How do I read the M5Stack 8-Angle (8 pots) or 8-Encoder?

```python
import m58angle
m58angle.get(0)   # first pot/encoder
m58angle.get(7)   # eighth
```

See the [Accessories page](accessories.md) for details. For encoders, prefer the unified `amyboard.encoder()` API above.

### How do I drive the NeoPixel LEDs on the Adafruit encoder board?

Easiest is `enc.led(i, r, g, b)` on the unified encoder object above. You can also talk to the Seesaw NeoPixel registers directly from your sketch with `amyboard.neopixel_init(pin=18, n=4)` and `amyboard.neopixel_set(i, r,g,b)` and `amyboard.neopixel_show()`. 

### Can I wire buttons or LEDs to the ESP32's spare GPIO pins?

Not easily -- the unused ESP32-S3 GPIOs aren't broken out to any header, so you'd have to solder directly to the chip. For extra controls and lights, use the I2C accessories above instead.

### Should I read encoders / update the display in `loop()` or with `tulip.defer()`?

Either works -- `tulip.defer()` uses the same sequencer as `loop()`, it just lets you set the next-call delay. For most people `loop()` is simplest: it runs ~32 times per beat (so timing scales with tempo). Read or update **one thing per call** so the system can keep up, and avoid heavy loops of your own.

---

## Sketches & saving your work

### I edit `sketch.py` but it reverts to the default on reboot

AMYboard tries to **self-heal**: if your `sketch.py` crashes hard at boot, it restores the default sketch. So a revert usually means your sketch errored. Recent firmware + the [web editor](online.md) now **show you the sketch's error** (with a traceback and line number) instead of silently reverting, so upgrade if you're flying blind. Hold **BOOT** while powering on to skip the sketch (this also runs a hardware self-test), then fix or delete it. The live sketch is at `/user/current/sketch.py`. See [Board won't boot / crashes](troubleshooting.md#board-wont-boot--crashes-on-startup).

### The editor used to auto-sync my sketch -- now it asks me to "read" and "write." What changed?

We replaced the old two-way "sync" with an explicit workflow, based on user feedback: you now tell the editor to **"Read from AMYboard"** or **"Write to AMYboard"**. The editor no longer auto-reads your board on page load, it warns before replacing the sketch in the editor, your sketch code stays in the browser until you say otherwise, and sketches downloaded from AMYboard World load into the editor only -- nothing touches your board until you write. You can also swap between Simulate and Control mode with the same sketch.

### My sketch loads without errors but doesn't behave -- how do I debug it?

The editor reports errors when **importing/writing** a sketch, but (deliberately) not errors that happen later **at runtime**. To see those, run the sketch in the **Simulator** and open the **Terminal REPL** there -- runtime tracebacks print right in the terminal. (One caveat: a few hardware-only calls don't exist in the simulator, so you may need to comment those out while debugging.)

### Where do I put my own Python modules or drivers?

Copy them to `/user/current/` on the board and `import` them from your sketch -- they're found at runtime. You only need to **recompile/flash firmware for C changes**; pure-Python modules (display drivers, helpers) don't require a rebuild.

### Can I patch one of the built-in modules (like `m5_8encoder`)?

The built-in modules are **frozen into the firmware**, so you can't edit them on the board, and a same-named file in `/user/current/` won't shadow the frozen copy. Instead, copy the module under a **new name** (e.g. `my_m5_8encoder.py`) into `/user/current/` and `import my_m5_8encoder`. If you fix a real bug, please PR it against [our copy in the tulipcc repo](https://github.com/shorepine/tulipcc/tree/main/tulip/shared/py) -- firmware releases go out about weekly, so fixes land fast.

### Do my MIDI channel and CC assignments survive a power cycle?

Yes -- once you click **"Write to AMYboard"** in the [web editor](online.md), your knob/CC assignments and patch are saved to flash and restored on boot.

### Does it remember the last patch? What's the difference between a preset and a patch?

- It restores your **last *saved* state** (the one you wrote to AMYboard), not necessarily the last one you played.
- A **Program Change (PC)** message switches among the built-in presets (128 Juno + 128 DX7) live, but **does not** write to flash -- after a reboot you're back to your saved setting.
- In the web editor, a **"preset"** is one of the 256 built-in synth presets; loading one overwrites the current knob settings. A **"patch"** is the full set of knob settings for a channel. To make any of it permanent, "Write to AMYboard."
- Because a PC reloads the whole preset, it also **wipes any live CC tweaks** you've made since. If your sequencer sends a PC at the top of every pattern loop, your CC changes will reset each time around -- send the PC **once** when the song/pattern starts instead.

---

## Controlling the synth

### How do I assign a hardware knob to a parameter (MIDI CC)?

Easiest: in the [web editor](online.md), click the parameter's **label** (the text, e.g. "Freq" -- not the knob itself!) and set its CC number -- it then "just works," including when the board runs standalone. In code you can set CC mappings with AMY's `ic` command (see the [AMY API docs](https://github.com/shorepine/amy/blob/main/docs/api.md#synths-and-voices)); those mappings persist in your sketch. We're also shipping a default Juno-style CC map (e.g. CC 74 → filter cutoff).

Two things worth knowing:

- CC mappings are **per MIDI channel** -- a mapping you set on channel 4 doesn't apply to channel 5.
- Everything you configure in the web GUI lands in the **config block at the bottom of your sketch** as `ic` lines (e.g. `i10ic86,...` = channel 10, CC 86). So instead of re-clicking mappings for every channel, set one channel up, look at that block, and copy/edit the lines for the others.

### How do I use a hardware controller without keeping the board tethered to the browser?

Set your CC assignments once, then go standalone:

1. Connect your MIDI controller to your computer over USB and select it as the MIDI input in the web editor.
2. Tweak the on-screen knobs to confirm the mappings.
3. **Write to AMYboard.**
4. Move the controller's MIDI cable to the AMYboard's MIDI in and disconnect the computer -- the knobs keep working on the hardware.

### My TRS MIDI gear doesn't respond -- is it Type A or Type B?

For **MIDI in**, you don't have to care: the AMYboard hardware intrinsically accepts both TRS Type A and Type B. For **MIDI out**, the default is **Type A**; if your downstream gear is Type B, switch at runtime (needs a recent firmware -- late June 2026 or newer):

```python
import amyboard
amyboard.set_midi_type('B')   # switch MIDI OUT to Type B
print(amyboard.midi_type())   # -> 'A' or 'B', whichever is live
```

This isn't persisted -- it resets to Type A on reboot -- so put the call in your sketch.

### Can I connect several MIDI inputs (and power sources) at the same time?

Yes -- it's fine to have TRS MIDI, USB MIDI, and multiple power sources (USB + Eurorack) all connected at once; notes from all MIDI inputs are handled. Just remember the USB port is **device mode only** -- a USB MIDI connection works to a computer (or USB host), not from a controller plugged into the AMYboard (see [USB host](#can-i-plug-a-usb-midi-keyboard-straight-into-the-amyboard-usb-host)).

### How do I play drum sounds?

Set up a synth with the **GM drum-kit patch (258)** -- the note→sample mapping lives in the patch, so plain note numbers won't make drum sounds without it:

```python
amy.send(synth=10, patch=258, num_voices=6, synth_flags=3)
```

`synth_flags=3` routes notes through the GM note map and ignores note-offs. Then play GM drum note numbers (36 = kick, 38 = snare, 42 = closed hat, 46 = open hat, 49 = crash...), either from your sketch with `amy.send(synth=10, note=36, vel=1)` or over **MIDI channel 10**.

### Can I control AMY directly over SysEx?

Yes -- it's more expressive than 0–127 CC values. Send `0xF0 0x00 0x03 0x45` (SysEx start + our vendor ID), then an **ASCII** AMY message, then `0xF7`. It's **one byte per character**, so the AMY command `i1v0F20` (synth 1, osc 0, filter freq → 20) becomes:

```
0xF0 0x00 0x03 0x45  0x69 0x31 0x76 0x30 0x46 0x32 0x30  0xF7
```

That lets you send precise values like `424.12456`. If you only need 0–127 knob values, use the MIDI CC support instead.

### Is AMYboard multitimbral? Can I run different patches on different MIDI channels at once?

Yes. You can put a different synth/patch on each of the **16 MIDI channels**, and they're **all active at the same time** -- the web editor only *shows* one channel at a time, but the rest keep responding on their channels. You're limited by RAM and by the total oscillator count (AMY caps at ~250 oscs), and by CPU as patches get richer, so very dense multitimbral setups run out of voices before all 16 are heavy. (Pitch bend is currently global across channels -- see AMY [#684](https://github.com/shorepine/amy/issues/684).)

---

## Sound design with AMY

### How do I add a pitch or filter LFO to a patch?

LFOs are just oscillators within a voice. The standard analog patches (e.g. `patch=6`) already set up osc 1 as an LFO. To modulate pitch and filter on synth 1:

```python
amy.send(synth=1, osc=2, mod_source=1, freq={'mod': 0.1})         # +/-0.1 octave pitch LFO
amy.send(synth=1, osc=0, mod_source=1, filter_freq={'mod': 0.5})  # filter cutoff LFO (the VCF is on osc 0)
```

If you want several LFOs you'll need to build the voice from scratch with enough `oscs_per_voice`.

### How do I make an FM patch?

AMY does **Yamaha-style phase modulation**, which only works with **SINE** operators (Prophet-style "analog FM" is on the roadmap -- AMY issue [#474](https://github.com/shorepine/amy/issues/474)). A 2-operator voice needs **3 oscs** -- the `ALGO` osc must be separate from the operator oscs. For a working example, see `fm_note_patch` in [xanadu.py](https://github.com/shorepine/tulipcc/blob/main/tulip/fs/tulip/ex/xanadu.py).

### Can I pass external audio through AMYboard / use it as an effect?

Yes. Load the **`audiopassthru`** sketch in the [web editor](https://amyboard.com/editor), enable audio input, and you can run the built-in echo/effects on the incoming signal. Note the echo/delay applies to the **sum of all oscillators** -- there's no arbitrary signal routing today.

### Can I build a ping-pong / multi-tap delay?

Not yet with the built-in echo -- it's a single delay on the master sum, not a routable audio graph. Multi-bus support (AMY issue [#686](https://github.com/shorepine/amy/issues/686)) is in progress and should eventually allow several delay lines. For now, custom routing means writing your own DSP (below).

### How do I write my own DSP effect over the AMY output?

In an Arduino sketch, call `amy_simple_fill_buffer()` in `loop()` to get an `int16*` buffer of finished samples, process it, and send it to the I2S out yourself. (`amy_external_render_hook` is for *generating* oscillator buffers, not post-processing the final mix.) See [Arduino Setup](arduino.md).

### My audio clips or distorts on some presets

A few things to check, roughly in order:

- **Headroom.** High note velocities from your controller/sequencer, or a high per-channel output volume, will clip. Setting the synth output volume around **2.5–3.5** leaves plenty of headroom.
- **Too many voices.** When AMY runs out of CPU/RAM with lots of simultaneous voices, the output degrades. Reduce `num_voices` or simplify patches.

### I set up a second synth channel, and now my first synth sounds different!

The analog synth patches (modeled on Juno) use the global chorus and EQ FX to shape their sounds, but these FX are shared between all synths by default.  So if you set up Juno patch A11 (Brass set 1) on channel 1, which uses the famous Juno chorus effect, then switch to channel 2 and set up Juno patch A13 (Trumpet), which does not use chorus, it will turn off the chorus for channel 1 as well, which will make it sound very thin.

AMY has a mechanism for separating synths onto separate "buses", each of which can run independent FX, but it is not yet exposed in the AMYboard online web editor.  Essentially you'd need to do something like:
```python
amy.send(synth=1, num_voices=6, patch=0, bus=0)  # Set up Brass set 1 on synth 1, bus 0
amy.send(synth=2, num_voices=6, patch=2, bus=1)  # Set up Trumpet on synth 2, bus 1
amy.send(volume='1,1')  # Set gains of buses 0 and 1 to 1.0 in output mix.
```
---

## Chaining multiple AMYboards

### Can I daisy-chain two or more AMYboards?

Yes. Share audio over **S/PDIF** in→out, and control over **I2C** or **MIDI**.

**Over I2C** -- connect amyboard 2's "tulip" (host) port to amyboard 1's front-panel I2C, then on amyboard 1:

```python
from machine import I2C
i2c = I2C(0, freq=400000)
amy.override_send = lambda x: i2c.writeto(0x3f, x)
```

**Over MIDI** -- amyboard 1 MIDI out → amyboard 2 MIDI in, then on amyboard 1:

```python
import amy, midi
amy.override_send = midi.sysex_amy
```

There's nothing built-in for polychaining / voice-splitting, but you can write your own `override_send` to divide notes across boards.

### How many can I power over the I2C chain?

Roughly **4** is a safe bet, maybe one or two beyond that. Each board draws ~350 mA from the 3V3_IN line, and the voltage sags across each board's supply FET down the chain (measured ~2.94 V at the 4th board, which still ran fine). If you're chaining more than a few, give them their own power.

---

## CV & modular integration

### Can I use CV without a Eurorack setup (e.g. on USB power)?

Yes -- CV works on **any** power supply, including a plain USB brick. Just patch your voltage into **CV1 or CV2 IN**: on the TS patch cable the **tip carries the voltage** and the **sleeve is ground**. The inputs measure **−10 V to +10 V** no matter how the board is powered.

### How do I use CV as gate and pitch inputs?

See the **"CV triggering"** examples in [Modular Synth Setup](modular.md#cv-triggering) -- they set up the first CV input as a **gate** (note-on/note-off) and the second as **pitch**. You can drop those `amy.send(...)` lines into an otherwise unmodified sketch, write it to the AMYboard, and be off and running.

### How do I add more CV / analog inputs?

AMYboard uses the **ADS1115** ADC for CV in. Easy add-on options over I2C: the [Adafruit ADS1115 breakout](https://www.adafruit.com/product/1085) (needs soldering), an [NCD screw-terminal ADS1115](https://store.ncd.io/product/ads1115-16-bit-4-channel-precision-analog-to-digital-converter-i2c-mini-module/), or the single-channel [M5Stack ADS1110 unit](https://docs.m5stack.com/en/unit/Unit-ADC_V1.1). For CV/DAC out, the GP8413 unit is documented in [Accessories](accessories.md).

### How do you scale ±10V Eurorack signals to the ADC?

Just resistors -- a divider that uses the analog 3V3 supply as bias to map −10..+10 V into 0..5 V. See the [schematic](https://github.com/shorepine/tulipcc/blob/main/docs/pcbs/amyboard/amyboard-1.4-sch.png). More in [Modular Synth Setup](modular.md).

---

## Cases & mounting

### Is there a case if I'm not using Eurorack?

There's no commercial desktop case yet, but the community has shared several 3D-printable desktop cases and stands -- check the Discord for current links. For inexpensive Eurorack power, a Tiptop µZeus is a popular pick.

### What's the panel with the labels/text I've seen in photos?

The board ships with a plain laser-cut acrylic faceplate. The **bicolor labeled panel** in some Discord photos is a custom 3D print with the [3mf file here](https://github.com/shorepine/tulipcc/blob/main/docs/pcbs/amyboard/amyboard_front_panel.3mf). Everything works the same with or without a faceplate.

### Two AMYboards in a small case -- how much room do I need?

The PCB itself is only ~47 mm wide (under 10HP); the **front panel** is 10HP. In a rack nothing is plugged into USB-C, and an I2C "tulip" cable still fits between two adjacent boards. A 20HP case (e.g. a 4ms Pod) fits two. If you want USB-C accessible in the rack, leave a little extra HP on that side -- many people add a right-angle USB-C panel-mount cable (and an external reset button) to their faceplate.

### Can I program the board once it's mounted in a rack?

Yes -- you don't need USB. You can send sketches and patches to AMYboard over **TRS MIDI** to the front-panel jacks from the [web editor](online.md), which is handy when the USB-C port is hidden inside a case. Firmware can also be updated in place: `amyboard.wifi(ssid, password)` then `amyboard.upgrade()` on each board -- much easier than pulling four boards out of a rack for USB flashing.

---

## Storage

### What SD cards work?

It's a vanilla MicroPython SD interface: **FAT32** (up to 2 TB volumes / 4 GB files). If a card won't mount, reformat it as FAT32 or try another card. See [SD card not mounting](troubleshooting.md#sd-card-not-mounting).

---

## Building from source

### Do I need to do anything special to build for AMYboard?

Once the ESP-IDF tools are set up, build and flash from the `tulip/amyboard` directory with `idf.py flash`. You'll usually need to put the board in DFU mode first (hold **BOOT**, press and release **RST**). To rewrite the whole filesystem: `idf.py erase-flash`, then `python amyboard_fs_create.py full`. Full instructions are on the [Firmware page](firmware.md). Remember: for pure-Python changes you don't need to build at all -- just copy files to `/user/current/`.

### Can I build my own control surface / editor that talks to AMYboard?

Yes! The SysEx-based control API the web editor uses for sketch transfer, reading board state, running Python, and rebooting is documented in [control_api.md](control_api.md) -- we use it ourselves for CI/test harnesses. There's a Python reference implementation and CLI at [`tools/amyboardctl`](https://github.com/shorepine/tulipcc/tree/main/tools/amyboardctl). If you're building an alternative to AMYboard Online, start there.

### Arduino or MicroPython?

We recommend our **MicroPython** firmware: the I2C accessories "just work," and you can log into the board and code in real time while it runs. **Arduino** is more bare-metal and gives you full control, but you'll need to track down libraries for accessories yourself. See [Arduino Setup](arduino.md) and [Using Python](python.md).

---

Still stuck? Ask on the [shore pine sound systems Discord](https://discord.gg/TzBFkUb8pG) -- the `#amyboard` channel is the fastest way to get help.
