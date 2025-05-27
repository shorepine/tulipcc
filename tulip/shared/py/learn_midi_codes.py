"""learn_midi_codes.py - Capture the midi codes.

Tulip's MIDI ControlCode mappings are read at startup from ${tulip_root}/user/midi_cc_file.json
and then accessed by other code as midi.SLIDER_IDS[0], midi.KNOB_IDS[0], midi.BUTTON_IDS[0] etc.

This program allows you to create your own midi_cc_file.json by touching the controls on your
MIDI device in order.
"""

import json
import time
import midi
import tulip

LAST_MIDI_MSG = None


def midi_callback(m):
  global LAST_MIDI_MSG
  LAST_MIDI_MSG = m
  #print(['%x' % x for x in m])


def get_midi_control_codes(num_codes, seen_codes_set, name="code"):
  global LAST_MIDI_MSG
  print("Activate %d %ss in order (space to skip)" % (num_codes, name))
  codes = []
  for code_num in range(num_codes):
    print("Capturing", name, code_num)
    while True:
      if LAST_MIDI_MSG or tulip.key() != -1:
        if LAST_MIDI_MSG:
          if (LAST_MIDI_MSG[0] & 0xf0) == 0xb0:  # Accept codes on any channel.
            control_code = LAST_MIDI_MSG[1]
            if control_code not in seen_codes_set:
              print(".. saw 0x%x"  % control_code)
              codes.append(control_code)
              seen_codes_set.add(control_code)
              break
          LAST_MIDI_MSG = None
        else:
          # Must have been tulip.key(), skip this one
          print("..skipping")
          codes.append(None)
          break
      time.sleep(0.01)
  return codes


def capture_all_codes():
  """Main routine to interactively capture codes."""
  seen_codes_set = set()

  num_sliders = int(input("How many sliders to capture? "))
  sliders = get_midi_control_codes(num_sliders, seen_codes_set, "slider")

  num_knobs = int(input("How many knobs to capture? "))
  knobs = get_midi_control_codes(num_knobs, seen_codes_set, "knob")

  num_buttons = int(input("How many buttons to capture? "))
  buttons = get_midi_control_codes(num_buttons, seen_codes_set, "button")

  # Ignore any final button release.
  time.sleep(0.5)

  data = {'sliders': sliders, 'knobs': knobs, 'buttons': buttons}
  return data


def save_json_file(data):
  """Interactively write the json file."""
  # Write the json file.
  default_file_name = "midi_cc_file.json"
  file_name = input("File name (enter for " + default_file_name + "): ") or default_file_name
  mapping_name = input("Mapping name: ")

  data['name'] = mapping_name

  with open(file_name, 'wt') as f:
    json.dump(data, f)

  print("Mapping written to", file_name)


midi.add_callback(midi_callback)

try:
  data = capture_all_codes()
  save_json_file(data)

except KeyboardInterrupt:
  # Allow ctl-C out, but still clean up the MIDI callbacks.
  pass
  
midi.remove_callback(midi_callback)





