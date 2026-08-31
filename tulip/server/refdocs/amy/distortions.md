# Distortion in AMY

AMY has a per-oscillator distortion stage with three waveshapers: a soft
clipper, a wavefolder, and a bitcrusher. The stage runs inside each osc's
signal chain, after the amplitude envelope and before the filter, so note
dynamics drive the shaper: a hard hit pushes further into the nonlinearity
than a soft one, and a decaying tail cleans up on its own. On a `SILENT`
chained-osc head the same stage shapes the voice's summed chain instead, which
is a musically different effect - see [Per-osc versus the voice's
sum](#per-osc-versus-the-voices-sum) below. The same chain also runs per bus,
as a mixbus stage ahead of the bus FX - see [Per-bus
distortion](#per-bus-distortion).

All the clips on this page are rendered offline through the Python module and
peak-normalized to -6 dBFS, so louder never reads as "better" in an A/B.

## Commands

Distortion rides `G` sub-commands on the wire (a digit after `G` is
`filter_type` as ever; a letter is a distortion sub-command):

| Wire | Python kwarg | Values | Meaning |
| ---- | ------------ | ------ | ------- |
| `GC1` / `GC0` | `dist_clip=1` / `0` | 0 or 1 | Enable / disable the soft clipper. |
| `GF1` / `GF0` | `dist_fold=1` / `0` | 0 or 1 | Enable / disable the wavefolder. |
| `GH<bits>,<rate>` | `dist_crush=[bits, rate]` | ints; bits 1-24, rate 1-1024 | Enable the bitcrusher: quantize to `bits` magnitude bits (24 leaves bit depth unchanged) and hold each sample for `rate` samples (1 disables the sample-and-hold). `GH0` disables. |
| `GD<coefs>` | `dist_drive=` | coef list | Pre-gain into the shaper (fold depth for the wavefolder), shared by all stages. A single value sets just the constant term in linear drive (1 = unity, range 2^-4 to 2^4); further coefs modulate it in octaves - see [Modulating drive and mix](#modulating-drive-and-mix) below. Default 1. |
| `GM<coefs>` | `dist_mix=` | coef list | Wet/dry mix, shared by all stages; combines linearly (like duty), clamped 0-1. Default 1 (full wet). |

Stages are independent: each command toggles only its own stage, and enabled
stages stack in a fixed clip -> fold -> crush order (shaping before lo-fi;
the reverse order per voice is reachable by putting the crusher on a chain
member and the clipper on its `SILENT` head). Drive and mix are shared
across the chain, and each stage applies drive as its own pre-gain, so
stacking re-amplifies per pass. One riff with the stage set changing on each
downbeat - dry, clip, clip+fold, all three, crusher alone:


https://github.com/user-attachments/assets/b66d66b6-22b1-4074-9295-0551db2e01a2


```python
amy.send(osc=0, wave=amy.TRIANGLE, dist_drive=4, dist_mix=1)
amy.send(osc=0, dist_clip=1)           # bar 2: clip
amy.send(osc=0, dist_fold=1)           # bar 3: clip + fold
amy.send(osc=0, dist_crush=[6, 5])     # bar 4: all three
amy.send(osc=0, dist_clip=0, dist_fold=0)  # bar 5: crusher alone
```

```python
amy.send(osc=0, wave=amy.SINE, dist_clip=1, dist_drive=3, dist_mix=1)
amy.send(osc=0, note=45, vel=1)
```

is the wire message `v0w0GC1GD3GM1Z` followed by `v0n45l1Z`.

## Soft clip (`GC`)

Pre-gain into a cubic soft knee: unity gain for small signals, saturating
toward the rails. Because the stage is post-envelope, the harmonic content is
dynamic per note - here the same note played at velocity 1.0 then 0.2: the
hard hit barks and audibly cleans up as the envelope walks the signal back
down the transfer curve; the soft hit never reaches the knee.

https://github.com/user-attachments/assets/59a768f8-1ac5-4168-a971-9e3a27cd6591

```python
amy.send(osc=0, wave=amy.SINE, bp0='0,1,2500,0.02,300,0',
         dist_clip=1, dist_drive=3, dist_mix=1)
amy.send(osc=0, note=45, vel=1.0)   # barks, then cleans up
amy.send(osc=0, note=45, vel=0.2)   # stays clean
```

Drive into the clipper also makes a convincing kick treatment - the ROM 808
kick with drive stepping 1 to 16, saturation compressing the pitch sweep into
a harder, longer-sustaining hit:

https://github.com/user-attachments/assets/8d52fca9-05fc-4d05-a8fb-512508a6584f

## Wavefolder (`GF`)

A triangle folder: identity on [-1, 1], reflecting back on itself beyond.
Drive is the fold depth. Sweeping drive 0 to 8 on a pure sine blooms a fan of
partials with no filter involved:

https://github.com/user-attachments/assets/0b015871-ce23-4b9c-81c5-4fb83447f902

The same envelope interaction as the clipper, opposite direction of travel: a
fast-decay pluck attacks folded and relaxes toward a pure tone inside its own
decay - every note ends purer than it started.

https://github.com/user-attachments/assets/464f0861-39e7-4ccf-933d-d889d697e667

```python
amy.send(osc=0, wave=amy.TRIANGLE, bp0='0,1,900,0,0,0',
         dist_fold=1, dist_drive=5, dist_mix=1)
```

## Bitcrusher (`GH`)

Bit-depth and sample-rate reduction in one stage; drive is a pre-gain into a
saturator ahead of the quantizer. The two parameters make distinct sounds.

**Rate** holds each sample for `rate` samples. The alias partials fall near
multiples of `sample_rate / rate`, so they move against the melody - lo-fi
that tracks the notes rather than sitting at a fixed fizz (a DC blocker on
the wet path keeps near-coincident partials from folding down to a
subsonic swing):

https://github.com/user-attachments/assets/f074844b-b636-4291-8fcb-1bbaf3103313

**Bits** quantizes the amplitude. Its audible signature is the tail: because
the envelope scales the signal before the stage, a release decays through
ever fewer quantization levels and finally gates out. One note per bit depth,
8 down to 1 - by 2 bits the tail is gone mid-release:

https://github.com/user-attachments/assets/5f0e8403-b7c4-420a-a105-4800c3f96c31

Drive interacts with bits: holding 2 bits and stepping drive 1 to 16 lifts
the decaying tail back over the quantization steps and squares the sustain:

https://github.com/user-attachments/assets/671b1ec0-5fe6-4579-9517-06ed513a9668

```python
amy.send(osc=0, wave=amy.SAW_DOWN, dist_crush=[6, 5], dist_mix=1)
```

## Modulating drive and mix

`GD` and `GM` are control-coefficient lists, the same rail as `freq` and
`filter_freq`: the constant term stays in linear drive and the modulation
coefs are octaves of it, so a coef of 1 doubles the drive (an octave is
also the natural unit for the wavefolder, where it buys one more fold).
Velocity into drive is what makes the shaper respond like part of the
voice - soft hits play a near-pure tone, hard hits bark:



https://github.com/user-attachments/assets/39d34301-3422-4e5d-b4a7-6065adf0d0c2



```python
# drive = 1 * 2^(3 * velocity): about 1.5 soft, 8 at full velocity
amy.send(osc=0, wave=amy.SINE, dist_clip=1, dist_mix=1,
         dist_drive={'const': 1, 'vel': 3})
```

The control: identical notes at static `dist_drive=8` bark at full velocity
exactly the same (that is the drive the vel-coef arm reaches there), but the
soft hits buzz just as hard relative to their level - static drive can't
tell touch apart:




https://github.com/user-attachments/assets/23fa611d-74c5-49d6-b8e7-68ac94d6d03a


Any modulation source works. Drive on EG1, swelling a single held note from
pure tone into growl with no parameter events after the note-on:




https://github.com/user-attachments/assets/d45b9dd6-5c0d-47ad-83c6-a5e7bb10024a


```python
amy.send(osc=0, wave=amy.SINE, bp1='0,0,4000,1,600,0',
         dist_clip=1, dist_mix=1, dist_drive={'const': 0.5, 'eg1': 3})
```

## Per-bus distortion

The same stage chain runs per bus, first in the bus FX chain - before EQ,
chorus, echo and reverb - so the delays and reverb take tails of the shaped
signal. There are no separate commands for it: the `G` sub-commands above
configure a bus whenever the message that carries them names no oscillator.

| Message | What it shapes |
| ------- | -------------- |
| `amy.send(synth=0, osc=1, dist_clip=1)` | Osc 1 of synth 0's voices. |
| `amy.send(osc=1, bus=2, dist_clip=1)` | Osc 1, routed to bus 2 - `bus=` alongside an explicit osc is routing, as it is for every other osc command. |
| `amy.send(bus=1, dist_clip=1)` | Bus 1. |
| `amy.send(synth=1, dist_clip=1)` | The bus synth 1 is on, or bus 0 if it is on none. |

```python
amy.send(bus=0, dist_clip=1, dist_drive=4, dist_mix=1)
```

A bus sum has no per-note modulation sources to combine, so at bus scope
`dist_drive` and `dist_mix` take only their constant term; the rest of a coef
list is ignored.

A bus stage responds to the whole bus level - which is what a mixbus
saturator is for. A quiet sustained tone and a loud pulsing bass through the
same clip settings: per-osc leaves the quiet tone static, on the bus the
bass drags the sum into the knee and the quiet tone ducks and buzzes
whenever the bass sounds:

https://github.com/user-attachments/assets/a88bcf1c-e19e-4d66-8dc8-59685b6ad54e

<details><summary>dry twin</summary>


https://github.com/user-attachments/assets/8778d836-46af-41a9-b6f1-1f74a71d24f2


</details>

<details><summary>per osc</summary>


https://github.com/user-attachments/assets/3de002c5-57d0-40a9-b246-b7843094872b


</details>

One drive knob over a whole held chord blooms the intermodulation fan out of
three pure tones in a single gesture:



https://github.com/user-attachments/assets/86182961-81e9-4c7f-83e4-f4020776d349


## Per-osc versus the voice's sum

Distortion scope matters because the nonlinearity is what generates
intermodulation: it matters whether oscs sit inside the shaper together. An
equal-tempered A-major triad, same drive, two ways - clip on each osc
independently keeps sparse, separate harmonic stacks per note:

https://github.com/user-attachments/assets/ebcb7caf-71f1-43fc-9904-179aa017250a

Chaining the three oscs into a `SILENT` head that carries the distortion
shapes the sum once: the gaps fill with sum/difference intermodulation and a
low difference-tone band - the power-chord growl.

https://github.com/user-attachments/assets/0e2ea972-e957-4928-8228-4224cd815b0f

```python
amy.send(osc=0, wave=amy.SILENT, chained_osc=1, bp0='0,1,300,1,150,0',
         dist_clip=1, dist_drive=4, dist_mix=1)
amy.send(osc=1, wave=amy.SINE, freq=220.00, chained_osc=2)
amy.send(osc=2, wave=amy.SINE, freq=277.18, chained_osc=3)
amy.send(osc=3, wave=amy.SINE, freq=329.63)
amy.send(osc=0, note=60, vel=0.6)   # note/vel at the head drive the chain
```

Both scopes compose - for example crush per osc for grit, clip on the head as
glue. Per-osc scope is also what makes the stage usable as a drum-kit effect:
each drum is shaped against its own level, not a bus mix. A two-bar 808
pattern with 6-bit crush on every drum osc:

https://github.com/user-attachments/assets/cce8013c-0a24-4cb2-9763-407a63362010

## Distortion and the filter

Per-osc distortion runs before that osc's filter, so a resonant sweep stays a
clean whistle over the distorted wave, with the filter taming everything
above the cutoff:

https://github.com/user-attachments/assets/00118699-6d68-4f71-bcce-1034e7417730

The reverse order is expressible with the same pieces: put the filter on a
chain member and the clip on its `SILENT` head, and the shaper regenerates
harmonics from the resonant peak itself - the spectrum fills to the top, the
"303 into a distortion pedal" sound:

https://github.com/user-attachments/assets/94364b18-ee81-4f89-afea-87f1b1e11f32
