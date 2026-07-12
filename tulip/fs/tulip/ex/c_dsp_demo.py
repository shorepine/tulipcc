# c_dsp_demo.py
# User C DSP demo: write audio effects and oscillators in C, from Python.
# (Tulip Desktop only for now -- AMYboard/web support is coming.)
#
# You give tulip just the *body* of a C function. Samples are plain 16-bit
# PCM, -32767..32767, like a WAV file. No fixed point math needed.
#
# Effect body variables:  int16_t *buf, int frames, int chans
#   buf is chans sequential channel blocks of frames samples; edit in place.
# Osc body variables:     int16_t *buf, int frames, int osc,
#                         int phase_inc_q16, int amp_q15
#   Fill buf (mono) with frames samples. phase_inc_q16 is the per-sample
#   phase step for the note's current pitch (65536 = one full cycle) and
#   amp_q15 is the envelope level right now (0..32767) -- multiply by it so
#   your osc follows AMY's ADSR.
#
# KEEPING STATE BETWEEN CALLS: declare variables `static`. They live as long
# as the install. For oscillators, many AMY oscs can play your code at once
# (polyphony!), so keep per-osc state in a static array indexed by `osc`,
# like the phase[] array below.

import tulip, amy, time

# --- An effect: 10-bit bitcrusher ------------------------------------------

CRUSH = """
    int i = 0;
    while (i < frames * chans) {
        buf[i] = (buf[i] >> 10) << 10;
        i = i + 1;
    }
"""

# --- An oscillator: CZ-101-style phase distortion "saw" --------------------
# Casio's CZ series makes bright waves without a filter: read a plain cosine
# with a *bent* phase. The first `dcw` fraction of the cycle sweeps the
# cosine's whole first half, the rest sweeps the second half. Small dcw =
# buzzy and bright (like an open filter); dcw = 32768 = pure cosine.

CZ = """
    static int phase[256];               // per-osc phase, Q16: state between calls
    int dcw = 6554;                      // bend point (0.1 of a cycle). edit me live!
    int i = 0;
    while (i < frames) {
        int p = phase[osc];
        int wp;                          // warped phase, Q16
        // All int32: p*32768 < 2^31 since p < 65536, and (p-dcw)*32768
        // stays under 2^31 for any dcw >= 1.
        if (p < dcw) wp = p * 32768 / dcw;
        else wp = 32768 + (p - dcw) * 32768 / (65536 - dcw);
        // Integer cosine: cos = sin(quarter cycle later), and sin by the
        // parabola trick -- t*(half - t) peaks at exactly 1.0 in Q15.
        int t = wp + 16384;
        if (t >= 65536) t = t - 65536;
        int s;
        if (t < 32768) s = (t * (32768 - t)) >> 13;
        else { t = t - 32768; s = 0 - ((t * (32768 - t)) >> 13); }
        buf[i] = (int16_t)((s * amp_q15) >> 15);
        p = p + phase_inc_q16;
        if (p >= 65536) p = p - 65536;
        phase[osc] = p;
        i = i + 1;
    }
"""

def run():
    tulip.install_c_process('crush', CRUSH)
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
    tulip.c_osc('cz', 200, False)
    print("done. try editing CZ's dcw and re-running -- reinstalls hot-swap.")

run()
