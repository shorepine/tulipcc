# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# Load a custom DX7 synth (WOOD PIANO) on synth channel 1
import amyboard, amy
import umidiparser

# Render using Juno patch 9
amy.send(synth=1, num_voices=8, patch=9)

print('methodical')

# Methodical.MID is by Nurykabe
# https://ccmixter.org/files/Nurykabe/10176
song = umidiparser.MidiFile('Methodical.MID')

utic = 0

us_per_loop = 50000

events = []
for event in song:
    utic = utic + (event.delta_us)
    if(event.status == umidiparser.NOTE_ON or event.status == umidiparser.NOTE_OFF):
        events.append([utic // us_per_loop, event.note + 12, event.velocity, event.status])
        
event_i = 0
next_event = events[event_i]
tick = 0

def loop():
    global tick, events, event_i, next_event
    while event_i <= len(events) and tick >= next_event[0]:
        if next_event[3] == umidiparser.NOTE_ON:
            amy.send(synth=1, note=next_event[1], vel=next_event[2] / 255.0)
        elif next_event[3] == umidiparser.NOTE_OFF:
            amy.send(synth=1, note=next_event[1], vel=0)
        event_i += 1
        if event_i < len(events):
            next_event = events[event_i]
    tick += 1
