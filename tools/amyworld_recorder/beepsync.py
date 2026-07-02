"""Detect the AMYboard sync-beep markers in an audio track.

The recorder plays a two-pulse sine marker (see AMYboardLink.beep) at each
sketch. This finds those markers in an externally-recorded video's audio so the
stitcher can locate each sketch on the phone timeline. Detection keys on both the
marker frequency (a Goertzel magnitude at BEEP_FREQ) and the two-pulse *timing*,
so ordinary bright synth content doesn't false-trigger.
"""

import numpy as np

BEEP_FREQ = 3520.0


def _goertzel_env(x, sr, freq, hop):
    """Magnitude of `freq` over non-overlapping `hop`-sample windows."""
    n = len(x) // hop
    if n == 0:
        return np.zeros(0)
    x = x[:n * hop].reshape(n, hop).astype(np.float64)
    t = np.arange(hop) / sr
    ref = np.exp(-2j * np.pi * freq * t)
    return np.abs(x @ ref) / hop


def detect_beeps(audio, sr, freq=BEEP_FREQ, pulse_ms=100, gap_ms=80,
                 hop_ms=10, rel_thresh=0.35):
    """Return marker start times (seconds). `audio` is a mono float/int array.

    A marker = two ~pulse_ms bursts of `freq` energy separated by ~gap_ms of
    quiet. We find bursts (contiguous above-threshold runs) then keep pairs whose
    durations and spacing match the marker shape."""
    a = np.asarray(audio, dtype=np.float64)
    if a.size == 0:
        return []
    hop = max(1, int(sr * hop_ms / 1000.0))
    env = _goertzel_env(a, sr, freq, hop)
    if env.size == 0:
        return []
    # robust threshold: fraction of the loud-marker level (95th pct is ~marker)
    hi = np.percentile(env, 99)
    if hi <= 0:
        return []
    thr = hi * rel_thresh
    above = env > thr

    # contiguous above-threshold runs -> bursts (start_frame, end_frame)
    bursts = []
    i = 0
    while i < len(above):
        if above[i]:
            j = i
            while j < len(above) and above[j]:
                j += 1
            bursts.append((i, j))
            i = j
        else:
            i += 1

    def sec(frame):
        return frame * hop / sr

    pulse_s = pulse_ms / 1000.0
    gap_s = gap_ms / 1000.0
    markers = []
    used = -1
    for k in range(len(bursts) - 1):
        if k <= used:
            continue
        b0, e0 = bursts[k]
        b1, e1 = bursts[k + 1]
        d0, d1 = sec(e0) - sec(b0), sec(e1) - sec(b1)
        gap = sec(b1) - sec(e0)
        # each burst roughly a pulse, and the gap roughly the marker gap
        if (0.4 * pulse_s < d0 < 2.5 * pulse_s and
                0.4 * pulse_s < d1 < 2.5 * pulse_s and
                0.2 * gap_s < gap < 3.0 * gap_s):
            markers.append(sec(b0))
            used = k + 1
    return markers


def load_audio_mono(path, sr=8000):
    """Decode any media file's audio to a mono numpy array at `sr` via ffmpeg."""
    import subprocess
    p = subprocess.run(
        ["ffmpeg", "-v", "error", "-i", path, "-map", "0:a:0",
         "-ac", "1", "-ar", str(sr), "-f", "s16le", "-"],
        capture_output=True)
    return np.frombuffer(p.stdout, dtype=np.int16).astype(np.float64), sr
