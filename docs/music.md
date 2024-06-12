# Using Tulip to make music tutorial

import midi
synth = midi.Synth(2) # two voice polyhony
synth.program_change(10) # change to patch #10 (juno-6 patch)
synth.note_on(50, 127)


amy.start_store_patch()
amy.send(osc=0,wave=amy.SINE,chained_osc=1)
amy.send(osc=1,wave=amy.SAW_DOWN, amp="0.1,0,1,1")
amy.stop_store_patch(1024)
amy.send(voices='6', note=52, vel=1, load_patch=1024)



