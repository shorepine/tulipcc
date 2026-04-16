# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: Play a MIDI file

print('methodical')

import amyboard, amy, tulip
import umidiparser

# Render using Juno patch 29, adapted
amy.reset()
setup = """i1iv6in4Z
i1v0w20F622.673,0.559,,,2.339R2.651c2L1G4A6,,1632,0.118,1166,0.000B6,1.000,640,0.175,446,0.000Z
i1v1w4a,,0.000f1.317,0.000,,,,,0.000A5,,100,1.000,10000,0.000Z
i1v2w1a0.709,,0.000,0.000d0.843c3L1Z
i1v3w1a0.542,,0.000,0.000f220.000L1Z
i1V1.000x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k0.000,320.000,0.830,0.500h0.000,0.850,0.500,3000.000Z
"""
for cmd in setup.split('\n'):
    amy.send_raw(cmd)

# Methodical.MID is by Nurykabe
# https://ccmixter.org/files/Nurykabe/10176
song_fn = tulip.root_dir() + "sys/ex/Methodical.MID"
song = umidiparser.MidiFile(song_fn)

# Parse the whole file once into (time_offset_ms, note, velocity) triples.
# We keep OFFSETS from song-start rather than absolute times so we can
# anchor playback to our actual boot time below.
events = []
utic = 0
for event in song:
    utic += event.delta_us
    if event.status == umidiparser.NOTE_ON or event.status == umidiparser.NOTE_OFF:
        # Methodical.MID encodes all note-offs as vel=0 note-ons; keep that.
        vel = 0 if event.status == umidiparser.NOTE_OFF else event.velocity / 256.0
        events.append((utic // 1000, event.note + 12, vel))

# Schedule notes ahead using AMY's `time=` parameter instead of dispatching
# them live from loop(). AMY's delta queue plays events with sample-accurate
# timing on the audio worklet, so MicroPython scheduler hiccups (heavy loop()
# workloads, GC pauses, sysex traffic in control mode, etc.) can't stretch
# the music out. loop() becomes a queue-refiller: it just has to run often
# enough to keep ~BUFFER_MS of future events queued.
BUFFER_MS = 2000    # how far ahead of "now" we keep events queued
LOW_WATER_MS = 500  # refill when the queue gets this shallow

SONG_START_MS = tulip.amy_ticks_ms() + 200  # small head-start so the first notes don't miss
_next_idx = 0            # index into `events` of the next unscheduled note
_last_scheduled_ms = SONG_START_MS

def _schedule_up_to(target_ms):
    """Queue every event whose play time is <= target_ms."""
    global _next_idx, _last_scheduled_ms
    while _next_idx < len(events):
        offset_ms, note, vel = events[_next_idx]
        play_time = SONG_START_MS + offset_ms
        if play_time > target_ms:
            return
        amy.send(synth=1, note=note, vel=vel, time=play_time)
        _last_scheduled_ms = play_time
        _next_idx += 1

# Prime the queue with the first BUFFER_MS of events so playback starts
# with plenty of lead time.
_schedule_up_to(SONG_START_MS + BUFFER_MS)

def loop():
    # Once every event has been queued, AMY is handling the rest on its own.
    if _next_idx >= len(events):
        return
    now = tulip.amy_ticks_ms()
    # Only refill when the queue is getting shallow — each schedule call
    # sends multiple amy.send() messages, so we don't want to do it every
    # loop() tick.
    if _last_scheduled_ms - now < LOW_WATER_MS:
        _schedule_up_to(now + BUFFER_MS)

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
i1ic255Z
i1iv6in4Z
i1v0w20F870.724,0.559,,,2.339R2.651c2L1G4A6,,1632,0.118,1166,0.000B6,1.000,640,0.175,446,0.000Z
i1v1w4a,,0.000f1.317,0.000,,,,,0.000A5,,100,1.000,10000,0.000Z
i1v2w1a0.709,,0.000,0.000d0.843c3L1Z
i1v3w1a0.542,,0.000,0.000f220.000L1Z
i1V1.000x0.000,0.000,0.000M0.000,500.000,,0.000,0.000k0.000,320.000,0.830,0.500h0.000,0.850,0.500,3000.000Z
"""
