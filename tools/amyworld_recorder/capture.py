"""Per-clip A/V capture.

Video (iPhone / Continuity Camera) is captured with ffmpeg's avfoundation input.
Audio is captured with sounddevice (PortAudio/CoreAudio), NOT ffmpeg — ffmpeg's
avfoundation audio input slips samples against the device's USB clock, which is
inaudible in silence but clicks on every signal. PortAudio follows the device
clock exactly (verified: 0 discontinuities vs ~70 for ffmpeg on the same signal).
The two are muxed into one clip when the take is marked.

Video and audio devices are matched by name at run time (their indices can move),
but note ffmpeg-avfoundation and sounddevice each have their OWN device numbering.
"""

import os
import queue
import re
import subprocess
import threading
import time
import wave

import numpy as np
import sounddevice as sd


def list_avfoundation_devices():
    """Return (video, audio) dicts mapping index -> name."""
    p = subprocess.run(
        ["ffmpeg", "-hide_banner", "-f", "avfoundation", "-list_devices", "true", "-i", ""],
        capture_output=True, text=True)
    out = p.stderr
    video, audio, section = {}, {}, None
    for line in out.splitlines():
        if "AVFoundation video devices" in line:
            section = video
            continue
        if "AVFoundation audio devices" in line:
            section = audio
            continue
        m = re.search(r"\]\s*\[(\d+)\]\s+(.*)$", line)
        if m and section is not None:
            section[int(m.group(1))] = m.group(2).strip()
    return video, audio


def resolve_device(devices, want):
    """Resolve `want` (an int index, a numeric string, or a name substring) to an
    avfoundation index using the {index:name} map `devices`."""
    if want is None:
        return None
    s = str(want)
    if s.isdigit() and int(s) in devices:
        return int(s)
    low = s.lower()
    for idx, name in devices.items():
        if low in name.lower():
            return idx
    return None


def find_iphone(video_devices):
    """Best-effort auto-detect of a Continuity Camera video device."""
    for pat in ("iphone", "continuity", "desk view"):
        for idx, name in video_devices.items():
            if pat in name.lower():
                return idx
    return None


def _rm(p):
    try:
        if p and os.path.exists(p):
            os.remove(p)
    except OSError:
        pass


# --------------------------------------------------------------------------
# audio devices + capture (sounddevice / PortAudio / CoreAudio)
# --------------------------------------------------------------------------
def list_sd_audio_devices():
    """Return {index: name} of sounddevice INPUT devices."""
    out = {}
    for i, d in enumerate(sd.query_devices()):
        if d.get("max_input_channels", 0) > 0:
            out[i] = d["name"]
    return out


def resolve_sd_audio(want):
    """Resolve `want` (int index, numeric string, or name substring) to a
    sounddevice input-device index."""
    if want is None:
        return None
    devs = list_sd_audio_devices()
    s = str(want)
    if s.isdigit() and int(s) in devs:
        return int(s)
    low = s.lower()
    for idx, name in devs.items():
        if low in name.lower():
            return idx
    return None


class SdAudioRecorder:
    """Records `channels` (1-indexed device channels, e.g. [13, 14]) from a
    sounddevice input device to a PCM WAV file, until stop(). The audio callback
    only enqueues frames; a writer thread does the file I/O so the realtime
    callback never blocks."""

    def __init__(self, device_idx, channels=None, samplerate=48000):
        self.device_idx = device_idx
        self.want = [c - 1 for c in channels] if channels else [0, 1]  # 0-based
        self.open_ch = max(self.want) + 1   # PortAudio opens channels 1..open_ch
        self.samplerate = samplerate
        self.stream = None
        self.wav = None
        self._q = queue.Queue()
        self._stop = threading.Event()
        self._writer = None
        self.xruns = 0

    def start(self, path):
        self.wav = wave.open(path, "wb")
        self.wav.setnchannels(len(self.want))
        self.wav.setsampwidth(2)      # int16
        self.wav.setframerate(self.samplerate)

        def cb(indata, frames, time_info, status):
            if status:
                self.xruns += 1
            self._q.put(indata[:, self.want].copy())

        self.stream = sd.InputStream(
            device=self.device_idx, channels=self.open_ch,
            samplerate=self.samplerate, dtype="int16", latency="high", callback=cb)
        self._writer = threading.Thread(target=self._drain, daemon=True)
        self._writer.start()
        self.stream.start()

    def _drain(self):
        while not self._stop.is_set() or not self._q.empty():
            try:
                block = self._q.get(timeout=0.2)
            except queue.Empty:
                continue
            self.wav.writeframesraw(block.tobytes())

    def stop(self):
        if self.stream:
            try:
                self.stream.stop()
                self.stream.close()
            except Exception:
                pass
            self.stream = None
        self._stop.set()
        if self._writer:
            self._writer.join(timeout=5)
            self._writer = None
        if self.wav:
            self.wav.close()
            self.wav = None


class ClipRecorder:
    def __init__(self, video_idx, audio_idx, framerate=30, video_size=None,
                 log_dir=None, vbitrate="8M", audio_channels=None):
        self.video_idx = video_idx
        self.audio_idx = audio_idx
        self.audio_channels = audio_channels  # 1-indexed list, e.g. [13, 14]
        self.framerate = framerate
        self.video_size = video_size  # e.g. "1280x720"; None = device default
        self.log_dir = log_dir
        self.vbitrate = vbitrate
        self.path = None
        self.started_at = None
        # Video is captured by ffmpeg (avfoundation); audio by sounddevice
        # (PortAudio). They are muxed at stop(). ffmpeg's avfoundation AUDIO
        # input slips samples vs the device clock (clicks on signal), so audio
        # must NOT go through ffmpeg — see module docstring. video_idx is an
        # ffmpeg-avfoundation index; audio_idx is a sounddevice index.
        self.vproc = None
        self.arec = None
        self._vlog = None
        self._vtmp = self._atmp = None

    def _video_cmd(self, out):
        cmd = ["ffmpeg", "-hide_banner", "-y", "-thread_queue_size", "1024",
               "-f", "avfoundation", "-framerate", str(self.framerate)]
        if self.video_size:
            cmd += ["-video_size", self.video_size]
        cmd += ["-i", "%d:none" % self.video_idx,
                "-c:v", "h264_videotoolbox", "-b:v", self.vbitrate,
                "-pix_fmt", "yuv420p", out]
        return cmd

    def _open_log(self, name):
        if not self.log_dir:
            return None, subprocess.DEVNULL
        os.makedirs(self.log_dir, exist_ok=True)
        p = os.path.join(self.log_dir, name)
        f = open(p, "wb")
        return f, f

    def start(self, path):
        os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
        self.path = path
        base = os.path.basename(path)
        # Start audio first (it's the take that matters), then video right after.
        if self.audio_idx is not None:
            self._atmp = path + ".audio.wav"
            self.arec = SdAudioRecorder(self.audio_idx, channels=self.audio_channels)
            try:
                self.arec.start(self._atmp)
            except Exception as e:
                self.arec = None
                raise RuntimeError("audio capture failed to start on device %r: %s"
                                   % (self.audio_idx, e))
        self.started_at = time.time()
        # video_idx None == audio-only mode (video comes from an external camera
        # recorded separately; we just capture the clean audio here).
        if self.video_idx is not None:
            self._vtmp = path + ".video.mp4"
            self._vlog, vsink = self._open_log(base + ".video.log")
            self.vproc = subprocess.Popen(self._video_cmd(self._vtmp),
                                          stdin=subprocess.PIPE, stdout=vsink, stderr=vsink)
            time.sleep(0.6)  # liveness: if video ffmpeg dies, device/permission is wrong
            if self.vproc.poll() is not None:
                if self.arec:
                    self.arec.stop()
                raise RuntimeError(
                    "video ffmpeg exited immediately (rc=%s). Check device index/permission; see %s"
                    % (self.vproc.returncode, self.log_dir or "stderr"))
        return self

    def elapsed(self):
        return time.time() - self.started_at if self.started_at else 0.0

    @staticmethod
    def _quit(proc):
        if not proc:
            return
        if proc.poll() is None:
            try:
                proc.stdin.write(b"q")
                proc.stdin.flush()
            except Exception:
                pass
            try:
                proc.wait(timeout=8)
            except subprocess.TimeoutExpired:
                proc.terminate()
                try:
                    proc.wait(timeout=4)
                except subprocess.TimeoutExpired:
                    proc.kill()

    def stop(self, discard=False):
        """Stop both recordings, mux video+audio into one clip, return its path
        (or None if discarded)."""
        if self.arec:
            self.arec.stop()
        self._quit(self.vproc)
        try:
            if self._vlog:
                self._vlog.close()
        except Exception:
            pass
        self._vlog = None
        path = self.path
        vtmp, atmp = self._vtmp, self._atmp
        self.arec = None
        self.vproc = None

        if discard:
            for t in (vtmp, atmp, path):
                _rm(t)
            return None

        # audio-only mode (no video device): the clip IS the clean audio.
        if self.video_idx is None:
            if atmp and os.path.exists(atmp):
                enc = ["ffmpeg", "-hide_banner", "-y", "-i", atmp,
                       "-c:a", "aac", "-b:a", "256k", "-ar", "48000", "-ac", "2",
                       "-movflags", "+faststart", path]
                rc = subprocess.run(enc, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode
                if rc == 0:
                    _rm(atmp)
                    return path
                os.replace(atmp, path)  # fall back to raw wav under the clip name
                return path
            return None

        if atmp and os.path.exists(atmp) and vtmp and os.path.exists(vtmp):
            # Mux: copy video, encode the clean PCM audio to AAC. -shortest trims
            # any small length difference from the two independent clocks.
            mux = ["ffmpeg", "-hide_banner", "-y", "-i", vtmp, "-i", atmp,
                   "-map", "0:v:0", "-map", "1:a:0", "-c:v", "copy",
                   "-c:a", "aac", "-b:a", "256k", "-ar", "48000", "-ac", "2",
                   "-shortest", "-movflags", "+faststart", path]
            rc = subprocess.run(mux, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode
            if rc == 0:
                _rm(vtmp)
                _rm(atmp)
                return path
            # mux failed — fall back to the video-only file so the take isn't lost
            if os.path.exists(vtmp):
                os.replace(vtmp, path)
            _rm(atmp)
            return path
        # video-only (no audio device configured)
        if vtmp and os.path.exists(vtmp):
            os.replace(vtmp, path)
        return path if path and os.path.exists(path) else None
