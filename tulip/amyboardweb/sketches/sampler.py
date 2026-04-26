# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
# DESCRIPTION: Live sampler. Hold a MIDI key to record audio in; play it on synth 1.

# How this works:
#  1. synth 1 is configured as a 4-voice PCM player reading from preset 50.
#  2. The first MIDI note-on calls amy.start_sample, which begins recording up to
#     5 seconds (44100 * 5 frames) from the audio input bus into preset 50.
#     The midi note is passed as the sample's natural pitch -- so when you
#     play the same note back you hear it at the original speed; higher notes
#     play faster, lower notes slower.
#  3. The matching MIDI note-off calls amy.stop_sample to end the recording
#     and removes the MIDI callback -- recording is one-shot.
#  4. Once a sample has been captured, hold any key on MIDI ch 1 to hear the
#     audio you recorded, pitch-shifted to that note. Subsequent note-ons go
#     straight to synth 1 and never re-trigger start_sample.
#
# In simulate mode, turn on "allow audio input" in the web editor to record from your mic.
# In simulate mode, you have to use MIDI input, not the onscreen keyboard.

import amy, midi

PRESET = 50
SR = 44100

amy.send(synth=1, wave=amy.PCM, preset=PRESET, num_voices=4)

# Track which note is driving the one-shot recording. None once we've stopped.
recording_note = None


def midi_cb(m):
    global recording_note
    if not m or len(m) < 3:
        return
    status = m[0] & 0xF0
    note = m[1]
    vel = m[2]
    if recording_note is None and status == 0x90 and vel > 0:
        # First note-on: start the one-shot recording into preset PRESET.
        recording_note = note
        amy.start_sample(preset=PRESET, max_frames=SR * 5, midinote=note)
    elif recording_note is not None and note == recording_note and (
        status == 0x80 or (status == 0x90 and vel == 0)
    ):
        # Matching note-off ends the recording. Detach the callback so all
        # subsequent MIDI notes go straight to synth 1 and play the captured
        # sample without re-triggering start_sample.
        amy.stop_sample()
        midi.remove_callback(midi_cb)


midi.add_callback(midi_cb)


def loop():
    pass

# Do not edit. Set automatically by the knobs on AMYboard Online.
_auto_generated_knobs = """
"""
