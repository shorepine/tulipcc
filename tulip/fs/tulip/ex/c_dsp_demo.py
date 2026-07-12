# c_dsp_demo.py
# User C DSP demo: write audio effects and oscillators in C, from Python.
#
# You give tulip just the *body* of a C function. Samples are AMY's native
# fixed point: int32, S8.23, so 1.0 == (1 << 23) and the usable range is
# about +/- 8388608. These helpers are always available in your code:
#
#   cos_lut(phase)     AMY's cosine table. phase is S8.23 normalized to one
#                      cycle (0 .. 1<<23); returns S8.23 in -1.0..1.0.
#   fxmul(a, b)        multiply two S8.23 numbers (a*b with 1.0 == 1<<23)
#   to_int16(s)        S8.23 -> plain 16-bit PCM (-32767..32767), clamped
#   from_int16(v)      16-bit PCM -> S8.23
#
# Effect body variables:  int *buf, int frames, int chans
#   buf is the live bus buffer, chans sequential channel blocks of frames
#   samples; edit in place.
# Osc body variables:     int *buf, int frames, int osc,
#                         int phase_inc_q16, int amp
#   Fill buf (mono) with frames samples. phase_inc_q16 is the per-sample
#   phase step for the note's current pitch (65536 = one full cycle) and
#   amp is the envelope level right now in S8.23 -- fxmul by it so your
#   osc follows AMY's ADSR.
#
# KEEPING STATE BETWEEN CALLS: declare variables `static`. They live as long
# as the install. For oscillators, many AMY oscs can play your code at once
# (polyphony!), so keep per-osc state in a static array indexed by `osc`,
# like the phase[] array below.

import tulip, amy, time

# --- An effect: bitcrusher (keep the top 5 of 23 fraction bits) -------------

CRUSH = """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 18) << 18;
        i = i + 1;
    }
"""

# --- An effect: heavy distortion --------------------------------------------
# Overdrive: slam the signal into a ceiling. Multiply by a big drive gain,
# clamp to +/-1.0, then round the corners off with a cubic soft clip
# (y = x - x^3/3, the classic saturator) so it growls instead of buzzing.
# The knee tops out at 2/3, so scale by 3/2 to bring peaks back to 1.0.
# Stateless -- no statics needed. Crank `drive` for more filth.

DIST = """
    int drive = 10;                      // 1 = clean boost, 10 = heavy. edit me!
    int i = 0;
    while (i < frames * chans) {
        int s = buf[i] * drive;
        if (s > 8388608) s = 8388608;    // clamp to +/-1.0 (S8.23)
        if (s < -8388608) s = -8388608;
        int y = s - fxmul(fxmul(s, s), s) / 3;
        buf[i] = y + y / 2;
        i = i + 1;
    }
"""

# --- An oscillator: CZ-101-style phase distortion "saw" --------------------
# Casio's CZ series makes bright waves without a filter: read a plain cosine
# with a *bent* phase. The first `dcw` fraction of the cycle sweeps the
# cosine's whole first half, the rest sweeps the second half. Small dcw =
# buzzy and bright (like an open filter); dcw = 32768 = pure cosine.
# Phase bookkeeping is in Q16 (65536 = one cycle) to keep the warp math in
# int32 range; << 7 converts to cos_lut's S8.23 phase at the lookup.

CZ = """
    static int phase[256];               // per-osc phase, Q16: state between calls
    int dcw = 6554;                      // bend point (0.1 of a cycle). edit me live!
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
"""

def run():
    tulip.install_c_process('crush', CRUSH)
    tulip.install_c_process('dist', DIST)
    tulip.install_c_osc('cz', CZ)

    # Point AMY osc 200 at our oscillator and set up its envelope. Use a HIGH
    # osc number: low ones belong to midi.py's default synths, which fight
    # you for control of them (raw events to an owned osc go silent).
    tulip.c_osc('cz', 200)
    amy.send(osc=200, wave=amy.SINE, bp0='10,1,500,0.2,250,0', vel=0)

    print("CZ phase distortion osc...")
    for note in (48, 52, 55, 60):
        amy.send(osc=200, note=note, vel=1)
        time.sleep(0.4)
    time.sleep(1)

    print("same notes through the bitcrusher...")
    tulip.c_process('crush', True)
    for note in (48, 52, 55, 60):
        amy.send(osc=200, note=note, vel=1)
        time.sleep(0.4)
    time.sleep(1)
    tulip.c_process('crush', False)

    print("same notes, heavy distortion...")
    tulip.c_process('dist', True)
    for note in (48, 52, 55, 60):
        amy.send(osc=200, note=note, vel=1)
        time.sleep(0.4)
    time.sleep(1)
    tulip.c_process('dist', False)
    tulip.c_osc('cz', 200, False)
    print("done. try editing CZ's dcw or DIST's drive and re-running -- reinstalls hot-swap.")

run()
