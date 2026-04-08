# Modular Synth Setup

AMYboard is designed to integrate directly with Eurorack and other modular synth systems. It has a 10-pin modular power connector, analog CV inputs and outputs, MIDI I/O, and an I2C bus for expansion. 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/amyboard_modular_2.jpg" width=600>

## Accessories

You can add an optional [OLED screen or knob](accessories.md) yourself by just connecting them to the I2C port on the front of the AMYboard. 

Your AMYboard ships with a plexiglass "front panel" with cutouts for the jacks and an optional screen and knob. The panel can be attached to the AMYboard through the jack screws. The front panel then screws into your Eurorack case. 

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/amyboard_bambu.png" width=600>

You can also 3D print your own front panel for whatever use case you have in mind! [You can grab a BambuLab 3MF file here](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pcbs/amyboard/amyboard_front_panel.3mf). Or build from the [DXF file for the provided front panel](https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/pcbs/amyboard/amyboard_front_panel.dxf).


## 10vpp Operation

Most modular synthesis units operate at 10Vpp, where an audio signal swings between -5V and +5V. Out of the box, AMYboard is "line level" at 1Vpp. To change the audio input and output of AMYboard to 10Vpp, you need to change the DIP switches on the back of the board to "OFF" / closed.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/www/img/dip_switches.png" width=600>

 - **Switches 1 & 2** (input): When closed, attenuate the inputs -- making line in friendly to 10Vpp Eurorack signals.
 - **Switches 3 & 4** (output): When closed, increase the gain of the output buffer -- making line out more friendly to 10Vpp Eurorack.

## CV outputs

AMYboard has **two CV outputs** on 3.5mm jacks, powered by a GP8413 12-bit DAC (I2C address `0x58`). The outputs range from approximately **-10V to +10V**, suitable for controlling Eurorack pitch, filter cutoff, or any CV-controlled parameter.

```python
import amyboard

# Output 3.3V on CV out 1
amyboard.cv_out(3.3, channel=0)

# Output -5V on CV out 2
amyboard.cv_out(-5.0, channel=1)
```

You can route an entire AMY synth's audio to a CV output using `set_cv_out`. The synth's audio is silenced from the speakers and sent to the DAC instead, so you can use any AMY waveform as a CV source:

```python
import amy, amyboard

# Create a synth and route it to CV1
amy.send(synth=5, wave=amy.SAW_DOWN, vel=1, freq=0.5)
amyboard.set_cv_out(channel=0, synth=5)
```

This sends a 0.5Hz saw wave out CV1 at full range (-10V to +10V). You can change the waveform, frequency, or amplitude at any time with `amy.send(synth=5, ...)`.

To stop it, release the note or clear the mapping:

```python
amyboard.set_cv_out(channel=0, synth=0)  # clear the CV mapping
```


### Use cases

 - **Pitch CV**: Output 1V/oct pitch voltages to control an external oscillator
 - **Modulation**: Generate LFOs, envelopes, or sequenced voltages in Python
 - **Sample & hold**: Read a value, process it, and output the result


## CV inputs

AMYboard has **two CV inputs** on 3.5mm jacks, powered by an ADS1115 16-bit ADC (I2C address `0x48`). They accept **-10V to +10V**.

```python
import amyboard

# Read voltage on CV in 1
volts = amyboard.cv_in(channel=0)
print(f"CV in 1: {volts:.2f}V")

# Read raw ADC value
raw = amyboard.adc1115_raw(channel=0)
```

### Use cases

 - **External CV to AMY**: Map incoming CV to synth parameters (filter, pitch, amplitude)
 - **Gate detection**: Read a gate signal and trigger AMY notes
 - **Sensor input**: Connect any voltage source (-10V to +10V range) and read it from Python

### Example: CV input controls AMY filter with CtrlCoefs

AMY's CtrlCoef system lets you map CV inputs directly to synth parameters without any Python polling loop. For frequency parameters like `filter_freq`, CtrlCoefs work in **log2 (1V/oct) space** -- the `const` value sets a center frequency in Hz, and the `ext0` coefficient scales how many octaves the CV shifts per volt.

The formula for the final frequency is:

```
freq = const * 2^(ext0 * cv_voltage)
```

So to sweep a low-pass filter between roughly 100 Hz and 1000 Hz using CV input 1 (-10V to +10V):

1. Pick the **geometric center** of your range: `sqrt(100 * 1000) ≈ 316 Hz`
2. Solve for `ext0`: you need `316 * 2^(ext0 * 10) = 1000`, so `ext0 = log2(1000/316) / 10 ≈ 0.166`

In practice, round numbers like `const=300, ext0=0.15` get you close (roughly 106 Hz to 849 Hz):

```python
import amy

# Set up a saw wave with a CV-controlled low-pass filter
amy.send(synth=1, wave=amy.SAW_DOWN)
amy.send(synth=1, filter_freq={'const': 300, 'ext0': 0.15}, filter_type=amy.FILTER_LPF24)

# Play a note -- the filter cutoff now tracks CV in 1
amy.send(synth=1, vel=1, note=48)
```

With nothing patched to CV in 1 (0V), the filter sits at 300 Hz. Patch in a modular LFO or envelope and the cutoff sweeps exponentially -- just like a hardware filter with a 1V/oct CV input.

For exact 100–1000 Hz range, use `'const': 316, 'ext0': 0.166`. For a wider sweep (e.g. 50–5000 Hz), increase `ext0`:

```python
# Wider sweep: geometric center ~500 Hz, ±10V covers 50 to 5000 Hz
amy.send(synth=1, filter_freq={'const': 500, 'ext0': 0.33}, filter_type=amy.FILTER_LPF24)
```


[Back to Getting Started](README.md)

