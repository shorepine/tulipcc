"""Implement a polyphonic synthesizer by managing a fixed pool of voices."""

# Optional monkeypatch of send() method to diagnose exactly what is being sent.
def amy_send_patch(**kwargs):
    print("amy_send:", kwargs)
    orig_amy_send(**kwargs)


midi_in_fn = None
control_change_fn = None
set_patch_fn = None

def midi_event_cb(x):
    """Callback that takes MIDI note on/off to create Note objects."""
    m = midi_in_fn()    # tulip.midi_in()
    while m is not None and len(m) > 0:
        if m[0] == 0xb0:    # Other control slider.
            control_change_fn(m[1], m[2])
        elif m[0] == 0xbf:
            # Special case for Oxygen49 transport buttons which send val 0x00 on release.
            if m[2] == 0x7f:
                control_change_fn(m[1], m[2])
                
        # Are there more events waiting?
        m = m[3:]
        if len(m) == 0:
            m = midi_in_fn()


def init(synth=None, my_midi_in_fn=None, my_control_change_fn=None, my_set_patch_fn=None):
    # Install the callback.
    #tulip.midi_callback(midi_event_cb)
    global midi_in_fn, control_change_fn, set_patch_fn, SYNTH

    midi_in_fn = my_midi_in_fn
    control_change_fn = my_control_change_fn
    set_patch_fn = my_set_patch_fn
    
    SYNTH = Synth(synth, NUM_VOICES)
