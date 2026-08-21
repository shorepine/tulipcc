# Distortion in AMY

AMY has a per-oscillator distortion stage with three waveshapers: a soft
clipper, a wavefolder, and a bitcrusher. The stage runs inside each osc's
signal chain, after the amplitude envelope and before the filter, so note
dynamics drive the shaper: a hard hit pushes further into the nonlinearity
than a soft one, and a decaying tail cleans up on its own. On a `SILENT`
chained-osc head the same stage shapes the voice's summed chain instead, which
is a musically different effect - see [Per-osc versus the voice's
sum](#per-osc-versus-the-voices-sum) below.

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
| `GD<drive>` | `dist_drive=` | float 0-16 | Pre-gain into the shaper (fold depth for the wavefolder), shared by all types. Default 1. |
| `GM<mix>` | `dist_mix=` | float 0-1 | Wet/dry mix, shared by all types. Default 1 (full wet). |

One type is active per osc; enabling one replaces another. Drive and mix keep
their values across type changes.

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
