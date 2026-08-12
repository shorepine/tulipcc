# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
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

# Parse the whole file once into (amy_tick, note, velocity) triples.
#
# The song is kept in AMY sequencer TICKS, not milliseconds. A MIDI file times
# events in "miditicks", its own subdivision of a quarter note, so converting
# to AMY's 48-ticks-per-quarter grid is a plain integer divide -- Methodical.MID
# is 96 PPQ, so it is a divide by 2, with no tempo term and no rounding.
#
# Nothing is lost to that divide: the file is quantized to 32nd notes (3107 of
# its 3122 note events sit exactly on a 12-miditick boundary) and exactly one
# event lands on an odd miditick, where the truncation moves it 5 ms earlier.
#
# Working in ticks is what makes tempo a real control: ticks carry musical
# position and AMY's tempo decides the speed, so `amy.send(tempo=...)` stretches
# the song correctly. Converting to ms here instead would bake one tempo into
# every timestamp and desynchronise the song from anything else AMY sequences.
AMY_PPQ = amy.AMY_SEQUENCER_PPQ    # 48
mtpq = song.miditicks_per_quarter  # 96 for Methodical.MID
MIDITICKS_PER_AMY_TICK = mtpq // AMY_PPQ   # 2

events = []
miditicks = 0
# Microseconds per quarter note. 500000 (120 BPM) is the MIDI default for a
# file that never sends SET_TEMPO -- which Methodical.MID doesn't, so this is
# the value it actually plays at. Don't drop this default: leaving the tempo
# alone would play the song at AMY's 108 BPM, 10% slow.
song_tempo_us = 500000
_got_tempo = False
for event in song:
    miditicks += event.delta_miditicks
    if not _got_tempo and event.status == umidiparser.SET_TEMPO:
        song_tempo_us = event.tempo
        _got_tempo = True
    if event.status == umidiparser.NOTE_ON or event.status == umidiparser.NOTE_OFF:
        # Methodical.MID encodes all note-offs as vel=0 note-ons; keep that.
        vel = 0 if event.status == umidiparser.NOTE_OFF else event.velocity / 256.0
        events.append((miditicks // MIDITICKS_PER_AMY_TICK,
                       event.note + 12, vel))

# Setting the tempo is what preserves the file's original timing: pick the BPM
# that makes one AMY tick last exactly MIDITICKS_PER_AMY_TICK miditicks, and the
# wall-clock schedule comes out identical to the file by construction. Here that
# is 120 BPM -- an AMY quarter (48 ticks) spans 96 miditicks, one file quarter.
#
# It is also the only knob now: change it here, or send `amy.send(tempo=...)`
# while the song plays, and everything retimes together.
amy.send(tempo=60000000.0 * mtpq
               / (song_tempo_us * AMY_PPQ * MIDITICKS_PER_AMY_TICK))

# Schedule notes ahead using AMY's `ticks=` parameter instead of dispatching
# them live from loop(). AMY's delta queue plays events with sample-accurate
# timing on the audio worklet, so MicroPython scheduler hiccups (heavy loop()
# workloads, GC pauses, sysex traffic in control mode, etc.) can't stretch the
# music out. loop() becomes a queue-refiller: it just has to run often enough
# to keep BUFFER_TICKS of future events queued.
#
# Resetting the timebase makes tick 0 the start of the song, so an event's
# stored tick IS the tick to schedule it at -- no anchor pair, no arithmetic
# against a clock read. Note this is a global reset: it re-phases any other
# sequence already running and defers any pending one-off, which is fine for a
# sketch that owns the machine at boot but is worth knowing if you paste this
# into something larger.
amy.send(reset=amy.RESET_TIMEBASE)

# Both in ticks, i.e. musical time: at 48 PPQ that is one 4/4 bar of lead,
# refilling when less than a quarter note remains queued (2.0 s and 0.5 s at
# this song's 120 BPM). Being musical rather than ms, the lead scales with
# tempo -- slow the song down and you get proportionally more slack.
BUFFER_TICKS = 192
LOW_WATER_TICKS = 48
# Small head start so the opening notes aren't already due when they're queued.
START_TICK = 8

_next_idx = 0                    # index into `events` of the next unscheduled note
_last_scheduled_tick = START_TICK

def _schedule_up_to(target_tick):
    """Queue every event due at or before target_tick."""
    global _next_idx, _last_scheduled_tick
    while _next_idx < len(events):
        tick, note, vel = events[_next_idx]
        play_tick = START_TICK + tick
        if play_tick > target_tick:
            return
        amy.send(synth=1, note=note, vel=vel, ticks=play_tick)
        _last_scheduled_tick = play_tick
        _next_idx += 1

# Prime the queue with the first BUFFER_TICKS of events so playback starts
# with plenty of lead time.
_schedule_up_to(START_TICK + BUFFER_TICKS)

def loop(tick):
    # Once every event has been queued, AMY is handling the rest on its own.
    if _next_idx >= len(events):
        return
    now = amy.sequencer_ticks()
    # Only refill when the queue is getting shallow — each schedule call
    # sends multiple amy.send() messages, so we don't want to do it every
    # loop() tick.
    if _last_scheduled_tick - now < LOW_WATER_TICKS:
        _schedule_up_to(now + BUFFER_TICKS)

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
