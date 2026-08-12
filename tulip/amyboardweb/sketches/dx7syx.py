# AMYboard Sketch
# Top-level code runs once at boot. loop() is called every 32nd note.
# DESCRIPTION: Load a DX7 patch from a sysex (.SYX) file into MIDI synth 1

import amy
from amy import fm

# Bring your own DX7 patch! Any DX7 .SYX data works here: a whole 32-voice
# bank sysex or a single voice, base64-encoded. Make your own with
#   base64 -i YOURBANK.SYX
# paste the output between the quotes, and pick a voice with voice= (0-31).
# fm.syx_names(SYX) lists all the names in a bank.
# This one is a single packed voice: the classic 'TUB BELLS'.
SYX = """
YlsAHGMAAAAAAAAAAgBVBABMTkdGYwAAAAAAAAA6FGMFM2IMRxxjACAAAAAAACoASwRLXyFHGWMA
IAAAAAAAEgBjAgBiDEccYwAgAAAAAABSAE4ES18hRxljACAAAAAAAEoAXwIAQ19fPDIyMjIEBw4A
AAAQGFRVQiBCRUxMUyA=
"""

name = fm.load_syx(SYX, voice=0, patch=1024)  # store as user patch 1024
print("Loaded DX7 patch:", name)

# Put it on MIDI synth 1: notes on MIDI channel 1 play it now.
amy.send(synth=1, num_voices=6, patch=1024)

# A slow bell arpeggio so you can hear it without a keyboard attached.
# Delete loop()'s body to play it over MIDI only.
BELLS = [52, 59, 64, 68, 71, 68, 64, 59]
count = 0

def loop(tick):
    global count
    if count % 16 == 0:
        amy.send(synth=1, note=BELLS[(count // 16) % len(BELLS)], vel=0.8)
    count += 1
