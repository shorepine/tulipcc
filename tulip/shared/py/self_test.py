# Self-test routines for amyboard.

import amy, amyboard, tulip, midi
import struct, time
from ulab import numpy as np

def input_rms(blocks=10):
  """Read some of the current input and return its RMS."""
  rmss = []
  for _ in range(blocks):
    waveform = np.array(
      struct.unpack('<' + (512 * 'h'), tulip.amy_get_input_buffer())
    )
    rmss.append(np.sqrt(np.mean(waveform * waveform)))
  return float(np.mean(rmss))

def self_test():
  """Run a series of hardware self-tests."""
  # This assumes that loop-back cables have been inserted between in and out
  # for line(audio), MIDI, CV1, CV2
  all_pass = []

  def passfail(condition):
    """Common code to test conditions."""
    if condition:
      all_pass.append(True)
      return "PASS"
    else:
      all_pass.append(False)
      return "**FAIL"
  
  amy.reset()
  # 1. Check that audio-in is small but not identically zero with no input.
  time.sleep(0.5)  # Need a little longer to let inputs not see startup chime.
  rms = input_rms()
  print("Quiescent input rms: %6.2f" % rms, " " * 8, 
        passfail(rms > 1.0 and rms < 4.0))
  # 2. Check that audio-in is large when output is large
  amy.send(osc=0, freq=440, vel=0.1)
  time.sleep(0.1)
  rms = input_rms()
  print("Active    input rms: %6.2f" % rms, " " * 8,
        passfail(rms > 45.0 and rms < 60.0))
  # 3. Check MIDI loopback
  midi.config.reset()
  global rx_midi
  rx_midi = []

  def trap_midi(m):
    global rx_midi
    rx_midi = [int(i) for i in m]

  midi.add_callback(trap_midi)
  time.sleep(0.1)
  tulip.midi_out([0x90, 0x40, 0x7F])
  time.sleep(0.1)
  print("MIDI received: %-21s" % str([i for i in rx_midi]), 
        passfail(rx_midi == [0x90, 0x40, 0x7F]))
  midi.remove_callback(trap_midi)
  # 4. Test CV loopback
  for channel in range(2):
    amyboard.cv_out(volts=0.0, channel=channel)
    time.sleep(0.1)
    volts_bias = amyboard.cv_in(channel)
    print("CV%d volts_bias %5.2f" % (channel, volts_bias),
          " " * 15,
          passfail(volts_bias > -1.5 and volts_bias < 1.5))
    for volts in [-5, 5]:
      amyboard.cv_out(volts=volts, channel=channel)
      time.sleep(0.1)
      volts_in = amyboard.cv_in(channel)
      print("CV%d volts_out %4.1f volts_in %5.2f" % (channel, volts, volts_in),
            " " * 2,
            passfail(abs(volts_in - volts_bias - volts) < 0.2))
  # Audible report
  if all(all_pass):
    # All OK chime
    amy.send(osc=0, wave=amy.SINE, freq=1000,
             bp0="0,1,500,0,0,0", vel=1)
    time.sleep(0.5)
  else:
    # Fail buzz
    for p in all_pass:
      amy.send(osc=0, wave=amy.PULSE, freq=150 + 150 * p,
               bp0="0,1,100,1,20,0,0,0", vel=1)
      time.sleep(0.15)    
  # Tear down
  amy.reset()


# If BOOT button is down, invoke self-test
from machine import Pin

def self_test_if_button():
  # The BOOT button is connected to GPIO0
  button = Pin(0, Pin.IN, Pin.PULL_UP)
  if button.value() == 0:  # Button pulls down
    time.sleep(0.5)
    self_test()

#self_test_if_button()
