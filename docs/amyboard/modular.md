# Modular Synth Setup

AMYboard is designed to integrate directly with Eurorack and other modular synth systems. It has analog CV inputs and outputs, MIDI I/O, and an I2C bus for expansion -- all controllable from Python.

## CV outputs

AMYboard has **two CV outputs** on 3.5mm jacks, powered by a GP8413 12-bit DAC (I2C address `0x58`). The outputs range from approximately **-10V to +10V**, suitable for controlling Eurorack pitch, filter cutoff, or any CV-controlled parameter.

```python
import amyboard

# Output 3.3V on CV out 1
amyboard.cv_out(3.3, channel=0)

# Output -5V on CV out 2
amyboard.cv_out(-5.0, channel=1)
```

### Use cases

 - **Pitch CV**: Output 1V/oct pitch voltages to control an external oscillator
 - **Modulation**: Generate LFOs, envelopes, or sequenced voltages in Python
 - **Sample & hold**: Read a value, process it, and output the result

### Example: Python LFO to CV

```python
import amyboard, time, math

# Output a slow sine LFO on CV out 1
for i in range(1000):
    volts = 5.0 * math.sin(i * 0.05)
    amyboard.cv_out(volts, channel=0)
    time.sleep(0.01)
```


(BRIAN TODO: Show how to generate wave output in AMY to CV ports)


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

### Example: CV input controls AMY filter

(BRIAN TODO: write this better)

```python
import amyboard, amy, time

# Set up a Juno patch on synth 1
amy.send(synth=1, patch=0, num_voices=6)

# Map CV in 1 to filter frequency
while True:
    v = amyboard.cv_in(channel=0)
    # Map -10..+10V to 100..8000 Hz
    freq = 100 + ((v + 10) / 20.0) * 7900
    amy.send(osc=0, filter_freq=freq)
    time.sleep(0.02)
```

## CV calibration

The CV outputs and inputs can be calibrated for accuracy. AMYboard ships with default calibration values, but you can refine them:

```python
import amyboard

# Run calibration for CV out channel 0
# This outputs test voltages -- measure with a multimeter and follow prompts
amyboard.cv_cal(channel=0)
```

This generates a `cal.csv` file with measured vs. expected values.

## DIP switches

AMYboard has a 4-position DIP switch on the back of the board that switches the gain of the line in and the line out between 1Vpp and 5Vpp. This is useful for modular synthesizer setups that expect "hotter" outputs and inputs.

<img src="https://raw.githubusercontent.com/shorepine/tulipcc/main/docs/amyboard/pics/dip_switches.png" width=600>

 - **Switches 1 & 2** (input): When closed, attenuate the inputs -- making line in friendly to 5Vpp Eurorack signals.
 - **Switches 3 & 4** (output): When closed, increase the gain of the output buffer -- making line out more friendly to 5Vpp Eurorack.
