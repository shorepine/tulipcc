# Writing audio effects and oscillators in C

Tulip and AMYboard let you write **audio DSP in C, from Python, at runtime**.
You hand `tulip.install_c_process()` a string of C code and a moment later it
is compiled and running inside AMY's audio engine — on the actual hardware,
with no toolchain, no reboot, no re-flash. Edit the string and install it
again to hot-swap the running code.

This works everywhere Tulip runs:

| Platform | Compiler underneath |
| -------- | ------------------- |
| Tulip CC / AMYboard (ESP32-S3) | `xcc700`, a tiny on-device C compiler |
| Tulip Desktop (macOS) | `libtcc` in-memory JIT |
| Tulip Web / AMYboard Web | `xcc700` again, emitting WebAssembly into AMY's AudioWorklet |

The Python API and the C you write are the same on all of them. A runnable
demo with the examples from this guide ships at `/sys/ex/c_dsp_demo.py`.
(For how this is implemented, see the [design doc](user_c_dsp_design.md).)

## Quick start: a bitcrusher in six lines

```python
import tulip

tulip.install_c_process('crush', """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 18) << 18;   // keep the top 5 fraction bits
        i = i + 1;
    }
""")
tulip.c_process('crush', True)   # enable it on bus 0 -- everything you play is now crushed
```

Play some notes. When you've had enough:

```python
tulip.c_process('crush', False)
```

You gave Tulip just the **body** of a C function; the signature and helper
declarations are wrapped around it for you. Edit the string (try `>> 20`) and
call `install_c_process` again — the new code replaces the old one in place,
keeping its bus assignment, without interrupting audio.

## The API

| Call | What it does |
| ---- | ------------ |
| `tulip.install_c_process(name, src)` | Compile `src` as an **effect** and install it under `name`. Returns the slot number, raises `ValueError` with the compiler message if it doesn't compile. |
| `tulip.install_c_osc(name, src)` | Same, but as an **oscillator**. |
| `tulip.c_process(name, on, bus=0)` | Enable/disable an effect on a bus (0-3). An effect can run on several buses at once. |
| `tulip.c_osc(name, osc, on=True)` | Point AMY oscillator number `osc` at your oscillator (or un-point it). Many oscs can play one installed program. |
| `tulip.c_process_calls(name)` | How many audio blocks your code has processed since install. The quickest "is it actually running?" check. |
| `tulip.uninstall_c_process(name)` | Remove it (either kind) and free the compiled code. |

There are 8 slots, shared between effects and oscillators. Reinstalling an
existing name reuses its slot and keeps its bus enables / osc bindings, so
your edit-listen loop is just: edit string, install, listen.

## The sample format, and the helpers you always have

Samples are AMY's native fixed point: **`int` in S8.23**, where 1.0 is
`1 << 23` (8388608). No floats — these run on machines without fast float,
and the fixed-point discipline is part of the fun. Four helpers are always
in scope:

| Helper | Meaning |
| ------ | ------- |
| `fxmul(a, b)` | Multiply two S8.23 numbers (`a*b` with 1.0 == `1<<23`). |
| `cos_lut(phase)` | AMY's cosine table. `phase` is S8.23 normalized to one cycle (0 .. `1<<23`); returns S8.23 in -1.0..1.0. |
| `to_int16(s)` | S8.23 → familiar 16-bit PCM (-32767..32767), clamped. |
| `from_int16(v)` | 16-bit PCM → S8.23. |

If you'd rather think in 16-bit samples like a tracker, convert on the way
in and out: `buf[i] = from_int16(my_math(to_int16(buf[i])));`.

## Effects

An effect body sees:

```c
void process(int *buf, int frames, int chans)
```

`buf` is the live bus buffer **after** AMY's own FX chain (EQ, chorus, echo,
reverb) — you are the last thing before the speaker. It holds `chans`
sequential channel blocks of `frames` samples each (not interleaved): left
is `buf[0] .. buf[frames-1]`, right starts at `buf[frames]`. Modify it in
place. Enable on buses with `tulip.c_process(name, True, bus)`.

## Oscillators

An oscillator body sees:

```c
void render(int *buf, int frames, int osc,
            int phase_inc_q16, int amp)
```

Fill `buf` (mono) with `frames` samples; AMY then applies the osc's pan and
mixes it like any other oscillator. You get two musical inputs per block:

- `phase_inc_q16` — the per-sample phase step for the note's **current
  pitch**, with 65536 meaning one full cycle. It tracks pitch bends and
  portamento live.
- `amp` — the envelope level right now, in S8.23. `fxmul` your output by it
  and your oscillator follows AMY's ADSR.

Bind it to an AMY osc and play it like any oscillator:

```python
tulip.install_c_osc('mine', SRC)
tulip.c_osc('mine', 200)
amy.send(osc=200, wave=amy.SINE, bp0='10,1,500,0.2,250,0', vel=0)
amy.send(osc=200, note=60, vel=1)
```

**Use a high osc number (200 is a good habit).** The low oscillators belong
to the default MIDI synths, which will fight you for control of them —
events sent to an osc a synth owns go silent.

Polyphony: bind several oscs to the same name and keep per-osc state in a
static array indexed by the `osc` argument (see the CZ example below).

## Keeping state between calls

Declare variables `static` in your body and they live as long as the
install. This is how filters keep their memory, oscillators keep their
phase, and delays keep their buffers:

```c
static int last;                       // one persistent int
static int phase[256];                 // per-osc state, indexed by `osc`
static int16_t delayline[22050];       // half a second of mono at 16-bit
```

Everything starts at zero on (re)install.

## The C you can write

On Tulip Desktop the compiler is a full C JIT (`libtcc`) — includes, floats,
structs, the lot. On Tulip CC, AMYboard and the web builds it's `xcc700`, a
deliberately small C:

- `int` and `int16_t`, pointers and arrays of both; `enum`; `static`
- `while`, `if`/`else`, `return`; functions (you can define helpers above
  your `process`/`render` and call them)
- the full integer expression zoo: `* / % + - << >> < <= > >= == != & ^ |`
  `&& || ! ~`, assignment
- **no** floats, structs, `for`, or preprocessor (`#define` etc.)

Write to the subset and the same string runs on every platform. (If a
formula seems to misbehave, add parentheses — then check your precedence
assumptions.)

Two warnings that keep their promises:

- **You can crash your synth.** This is real machine code in the audio
  path. Write past a buffer or divide by zero and the whole synth can stop
  (on hardware, it reboots). That's the fun part; save your work.
- **Keep it fast.** Your code runs once per block per bus (or per playing
  osc) on the audio thread. Integer math is cheap; huge loops are not.

## Example: bitcrusher (effect)

The quick start above, spelled out: shifting right then left throws away
low bits, quantizing the waveform into crunchy steps. Try different depths
live — each install hot-swaps the last.

```python
tulip.install_c_process('crush', """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 18) << 18;
        i = i + 1;
    }
""")
tulip.c_process('crush', True)
```

## Example: heavy distortion (effect)

Overdrive: slam the signal into a ceiling. Multiply by a drive gain, clamp
to ±1.0, then round the corners off with the classic cubic saturator
(`y = x - x³/3`) so it growls instead of buzzing. The knee tops out at 2/3,
so scale by 3/2 to bring peaks back to full scale.

```python
tulip.install_c_process('dist', """
    int drive = 10;                      // 1 = clean boost, 10 = heavy
    int i = 0;
    while (i < frames * chans) {
        int s = buf[i] * drive;
        if (s > 8388608) s = 8388608;    // clamp to +/-1.0 (S8.23)
        if (s < -8388608) s = -8388608;
        int y = s - fxmul(fxmul(s, s), s) / 3;
        buf[i] = y + y / 2;
        i = i + 1;
    }
""")
tulip.c_process('dist', True)
```

Stateless, so no statics needed. Crank `drive` for more filth.

## Example: CZ-101 phase distortion (oscillator)

Casio's CZ series made bright, filter-like tones with no filter at all:
read a plain cosine with a *bent* phase. The first `dcw` fraction of the
cycle sweeps the cosine's whole first half, the rest sweeps the second
half. Small `dcw` = buzzy and bright, like an open filter; `dcw = 32768` =
pure cosine. Phase bookkeeping is in Q16 (65536 = one cycle); `<< 7`
converts to `cos_lut`'s S8.23 phase at the lookup.

```python
tulip.install_c_osc('cz', """
    static int phase[256];               // per-osc phase: polyphony for free
    int dcw = 6554;                      // bend point (0.1 of a cycle)
    int i = 0;
    while (i < frames) {
        int p = phase[osc];
        int wp = 0;                      // warped phase, Q16
        if (p < dcw) wp = p * 32768 / dcw;
        else wp = 32768 + (p - dcw) * 32768 / (65536 - dcw);
        buf[i] = fxmul(cos_lut(wp << 7), amp);
        p = p + phase_inc_q16;
        if (p >= 65536) p = p - 65536;
        phase[osc] = p;
        i = i + 1;
    }
""")
tulip.c_osc('cz', 200)
amy.send(osc=200, wave=amy.SINE, bp0='10,1,500,0.2,250,0', vel=0)
amy.send(osc=200, note=60, vel=1)
```

Sweep `dcw` between installs (or extend the body to modulate it with
another counter) and you've built a CZ envelope's worth of timbre.

## Example: bytebeat (oscillator)

[Bytebeat](https://canonical.org/~kragen/bytebeat/) is the genre where one
line of integer C *is* the whole composition: run a counter `t` up once per
sample, mash it through shifts and masks, and play the low bits. Here's a
classic formula as a Tulip oscillator — the note's pitch is ignored (the
formula is the melody), but velocity and the envelope still gate it
through `amp`:

```python
tulip.install_c_osc('beat', """
    static int t[256];
    int i = 0;
    while (i < frames) {
        int n = t[osc];
        int v = (n * (n >> 12 | n >> 8) & 63 & n >> 4) - 32;
        buf[i] = fxmul(from_int16(v * 1024), amp);
        t[osc] = n + 1;
        i = i + 1;
    }
""")
tulip.c_osc('beat', 200)
amy.send(osc=200, wave=amy.SINE, bp0='10,1,10000,1,500,0', vel=0)
amy.send(osc=200, note=60, vel=1)   # hold a note open and listen to it unfold
```

The formula (`t*(t>>12|t>>8)&63&t>>4`) produces 6-bit output, recentered
and scaled to full range through `from_int16`. Swap in any bytebeat you
like — the whole integer operator set is available. Ideas: make the
keyboard transpose it by stepping `t` with something derived from
`phase_inc_q16` instead of 1, or run two formulas and mix them.

## Is it working? Debugging

- `tulip.c_process_calls(name)` counts processed blocks. If it's
  increasing, your code is being called. Effects tick on every block while
  enabled; oscillators only while a bound osc is audible.
- Compile errors raise `ValueError` at install time, with the compiler's
  message and line number pointing into your string. Nothing is replaced
  until the new code compiles.
- Hearing nothing from an oscillator? Check the osc has an envelope set up
  (the `bp0=...` line), that you bound the same osc number you're playing,
  and that you `fxmul` by `amp` — a zero envelope gives you a perfect
  silence generator.
- Silence, but `c_process_calls` is climbing? Your math probably collapsed
  to zero — `int` division truncates; multiply before you divide.
