# midi.py
# always running midi listener
# based on dan's polyvoice

#By default, at power on, Tulip will respond to MIDI note messages and assign them to different voices in AMY, with polyphony and voice stealing. 
#This will happen with no other apps running, and allows Tulip to just act as a synthesizer with no user interaction. 
#For example, by default, MIDI notes on channel 1 will map to Juno-6 patch 0, channel 2 to Juno-6 patch 1, channel 3 to DX-7 patch 0, etc.

#You can adjust which voices are sent with tulip.music_map(channel, patch_number, voice_count). 
#The channel is a MIDI channel (we use 1-16 indexing), the patch_number is an AMY patch number, and 
#voice_count is the number of voices (polyphony) you want to support for that channel and patch. 
#(A good rule of thumb is Tulip CC can support about 12 simultaneous total voices for Juno-6 and DX7, and 20-30 
#    total voices for PCM and more for other simpler oscillator patches.)

#You can also send AMY patch commands (strings) with tulip.music_map(channel, patch_string, voice_count). 
#This lets your UI apps adjust patches and send updates to AMY for the synthesizer. (You can use AMY’s record_patch 
#    functionality to get a patch string, or have your app generate one.)

#These mappings will get reset to default on boot. If you want to save them, put tulip.music_map() commands in your boot.py.


# So this
# midi in handling
# voice stealing
# setting up AMY voices on boot
# handle map from chan -> voice
# handle map from string -> voice (requires AMY changes, so do last)

import alles as amy
import tulip

# Tulip defaults, 4 note polyphony on four channels
# drum machine always on channel 9/10
PCM_PATCHES = 29
polyphony_map = {1:4, 2:4, 3:4, 4:4, 10:10}
patch_map = {1:0, 2:1, 3:128, 4:129}

# Our state
voices_for_channel = {} #1:[0,1,2,3], 2:[4,5,6,7], 3:[8,9,10,11], 4:[12,13,14,15], 10:range(110,120)}
note_for_voice = {}
voices_active = []

# Get the voice for this chan/note 
def find_voice(channel, midinote):
    if channel not in voices_for_channel:
        return None

    for v in voices_for_channel[channel]:
        # Is this voice active already but also the existing note? that's ok
        if note_for_voice.get(v,None) == midinote:
            return v

        # Is this voice free? Use it
        if v not in voices_active:
            voices_active.append(v)
            note_for_voice[v] = midinote
            return v

    # couldn't find free voice. 
    # steal the earliest voice added
    for v in voices_active:
        if v in voices_for_channel[channel]:
            # move this stolen voice to the end
            voices_active.remove(v)
            voices_active.append(v)
            note_for_voice[v] = midinote
            return v

def pitch_bend(channel, val):
    # get the active voices for this channel
    if channel not in voices_for_channel:
        return
    v_str = ""
    for v in voices_for_channel[channel]:
        if v in voices_active: v_str = v_str + "%d," % (v)
        if(len(v_str)):
            print('sending pb to voices %s val %f' % (v_str[:-1], val))
            amy.send(voices=v_str[:-1], pitch_bend=val)

def note_on(channel, midinote, vel):
    # Drum channel is special
    voice = find_voice(channel, midinote)
    if voice is not None:
        if(channel == 10):
            amy.send(osc=voice, wave=amy.PCM, patch=midinote % PCM_PATCHES, vel=vel, freq=0)
        else:
            amy.send(voices="%d" % (voice), note=midinote, vel=vel)

def note_off(channel, midinote):  
    # Get the voice number for this midinote
    for v in voices_for_channel[channel]:
        if note_for_voice.get(v,None) == midinote:
            # Remove it
            voices_active.remove(v)
            del note_for_voice[v]
            # Turn off the note
            if(channel == 10):
                amy.send(osc=v, vel=0)
            else:
                amy.send(voices='%d' % (v), vel=0)
            return

def update_channel(channel):
    import time
    vstr = ",".join([str(x) for x in voices_for_channel[channel]])
    amy.send(voices=vstr, load_patch=patch_map[channel])
    time.sleep(0.1) # AMY queue will fill if not slept 

def program_change(channel, patch):
    # update the map
    patch_map[channel] = patch
    update_channel(channel)

def midi_event_cb(x):
    """Callback that takes MIDI note on/off to create Note objects."""
    m = tulip.midi_in() 
    while m is not None and len(m) > 0:
        channel = (m[0] & 0x0F) + 1
        message = m[0] & 0xF0
        if message == 0x90:  # Note on.
            midinote = m[1]
            midivel = m[2]
            vel = midivel / 127.
            note_on(channel, midinote, vel)
        elif message == 0x80:  # Note off.
            midinote = m[1]
            note_off(channel, midinote)
        elif message == 0xc0:  # Program change
            program_change(channel, m[1])
        elif message == 0xe0:  # Pitch bend. m[2] is MSB, m[1] is LSB. 14 bits 
            pb_value = ((m[2] << 7) | (m[1])) - 8192 # -8192-8192, where 0 is nothing
            amy_value = float(pb_value)/(8192*6.0) # convert to -2 / +2 semitones
            amy.send(pitch_bend=amy_value)
    
        # Are there more events waiting?
        m = m[3:]
        if len(m) == 0:
            m = tulip.midi_in()

def setup_voices():
    #amy.reset()
    v_counter = 0
    voices_for_channel[10] = list(range(110,120))
    for channel in polyphony_map.keys():
        if channel is not 10:
            voices_for_channel[channel] = list(range(v_counter, v_counter+polyphony_map[channel]))
            v_counter = v_counter + polyphony_map[channel]
            update_channel(channel)

def defer_setup(t):
    if(tulip.seq_ticks() > tulip.seq_ppq()):
        setup_voices()
        tulip.midi_callback(midi_event_cb)
        tulip.seq_remove_callback(defer_setup)

def music_map(channel, patch_number=None, voice_count=None):
    if voice_count is not None:
        polyphony_map[channel] = voice_count
        setup_voices()
    if patch_number is not None:
        patch_map[channel] = patch_number
        update_channel(channel)

def setup():
    # we can't setup on boot right away as we need to get the bleep going and the alles setup done, so wait on a callback
    tulip.seq_add_callback(defer_setup)


