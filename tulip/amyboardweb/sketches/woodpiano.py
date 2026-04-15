# AMYboard Sketch
# Top-level code runs once at boot. loop() runs repeatedly (~60ms).
# DESCRIPTION: Load a custom DX7 synth (WOOD PIANO) on synth channel 1
import amyboard, amy

print("WOOD PIANO!")

# Show it on the display
amyboard.display.fill(0)
amyboard.display.text("WOOD PIANO!",0,20,255)
amyboard.display_refresh()

# The four-op WOOD PIANO patch
amy.send(synth=1, num_voices=6, oscs_per_voice=5)
amy.send(synth=1, osc=4, wave=amy.SINE, ratio=1, bp0='10,1,1000,0.8,100,0', amp='0.4,0,0,1', eg0_type=2, phase=0)
amy.send(synth=1, osc=3, wave=amy.SINE, ratio=0.5, bp0='0,1,1000,0,100,0', amp='1,0,0,1', eg0_type=2, phase=0)
amy.send(synth=1, osc=2, wave=amy.SINE, ratio=1, bp0='0,1,300,0.5,500,0.3,1000,0', amp='0.8,0,0,1,0,0', eg0_type=2, phase=0)
amy.send(synth=1, osc=1, wave=amy.SINE, ratio=0.495, bp0='0,1,2000,0,300,0', amp='1,0,0,1,0,0', eg0_type=2, phase=0)
# Osc 0 amp envelope is just to avoid truncating the FM output.
amy.send(synth=1, osc=0, wave=amy.ALGO, algorithm=5, algo_source=',,4,3,2,1', bp0='0,1,1000,1,300,0', amp='4,0,1,1', freq='220,1,0,0,0,0')


count = 0

def loop():
	pass
