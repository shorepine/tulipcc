import amy, sys

import pydub
import loris
import time
import numpy as np
from math import pi
from collections import deque


def list_from_py2_iterator(obj, how_many):
    # Oof, the loris object uses some form of iteration that py3 doesn't like. 
    ret = []
    it = obj.iterator()
    for i in range(how_many):
        ret.append(it.next())
    return ret

def loris_synth(filename, freq_res=150, analysis_window=100,amp_floor=-30, max_len_s = 10, noise_ratio=1, hop_time=0.04):
    # Pure loris synth for A/B testing
    audio = pydub.AudioSegment.from_file(filename)
    audio = audio[:int(max_len_s*1000.0)]
    y = np.array(audio.get_array_of_samples())
    if audio.channels == 2:
        y =y.reshape((-1, 2))
        y = y[:,1]
    y = np.float64(y) / 2**15
    analyzer = loris.Analyzer(freq_res, analysis_window)
    analyzer.setAmpFloor(amp_floor)
    analyzer.setHopTime(hop_time)
    partials = analyzer.analyze(y,44100)
    bps = 0
    for i in list_from_py2_iterator(partials, len(partials)):
        bps = bps + len(list_from_py2_iterator(i, i.numBreakpoints()))
    print("%d partials %d bps" % (len(partials), bps))
    loris.scaleNoiseRatio(partials, noise_ratio)
    return loris.synthesize(partials,44100)

def sequence(filename, max_len_s = 10, amp_floor=-30, hop_time=0.04, max_oscs=amy.OSCS, freq_res = 10, freq_drift=20, analysis_window = 100):
    # my job: take a file, analyze it, output a sequence + some metadata
    # i do voice stealing to keep maximum partials at once to max_oscs 
    # my sequence is an ordered list of partials/oscillators, a list with (ms, osc, freq, amp, bw, phase, time_delta, amp_delta, freq_delta, bw_delta)
    audio = pydub.AudioSegment.from_file(filename)
    audio = audio[:int(max_len_s*1000.0)]
    y = np.array(audio.get_array_of_samples())
    if int(audio.frame_rate) != int(amy.SAMPLE_RATE):
        print("SR mismatch, todo")
        return (None, None)
    if audio.channels == 2:
        y =y.reshape((-1, 2))
        y = y[:,1]
    y = np.float64(y) / 2**15
    metadata = {"filename":filename, "samples":y.shape[0]}

    if(filename.endswith(".wav")):
        import wavdumper # Forked version
        w = wavdumper.Wav(filename)
        w.printInfo() # TODO, call this extractInfo
        try:
            if(w.midiUnityNote>0):
                metadata["midi_note"] = w.midiUnityNote
            if(w.loopstart >= 0 and w.loopend >= 0):
                metadata["sustain_ms"] = int(((w.loopstart + ((w.loopend-w.loopstart)/2.0)) / amy.SAMPLE_RATE) * 1000.0)
        except AttributeError:
            pass # No wav metadata

    # Do the loris analyze
    analyzer = loris.Analyzer(freq_res, analysis_window)
    analyzer.setAmpFloor(amp_floor)
    analyzer.setFreqDrift(freq_drift)
    analyzer.setHopTime(hop_time)
    partials_it = analyzer.analyze(y, audio.frame_rate)
    # build the sequence
    sequence = []
    partials = list_from_py2_iterator(partials_it, partials_it.size())
    partial_count = 0
    for partial_idx, partial in enumerate(partials):
        breakpoints = list_from_py2_iterator(partial, partial.numBreakpoints())
        if(len(breakpoints)>1):
            partial_count = partial_count + 1
            for bp_idx, bp in enumerate(breakpoints):
                phase = -1
                # Last breakpoint?
                if(bp_idx == len(breakpoints)-1): phase = -2
                # First breakpoint
                if(bp_idx == 0):
                    phase = bp.phase() / (2*pi)
                    if(phase < 0): phase = phase + 1
                time_ms = int(bp.time() * 1000.0)
                sequence.append( [time_ms, partial_idx, bp.frequency(), bp.amplitude(), bp.bandwidth(), phase] )

    # Now go and order them and figure out which oscillator gets which partial
    time_ordered = sorted(sequence, key=lambda x:x[0])
    first_time = time_ordered[0][0]
    # Clear the sequence
    sequence = []
    min_q_len = max_oscs
    # Now add in a voice / osc # 
    osc_map = {}
    osc_q = deque(range(max_oscs)) 
    for i,s in enumerate(time_ordered):
        next_idx = -1
        time_delta, amp_delta, freq_delta, bw_delta = (0,0,0,0)
        if(s[5] != -2): # if not the end of a partial
            next_idx = i+1
            while(time_ordered[next_idx][1] != s[1]):
                next_idx = next_idx + 1
            n = time_ordered[next_idx]
            time_delta = n[0] - s[0]
            amp_delta = n[3]/s[3]
            freq_delta = n[2]/s[2]
            if(s[4]>0):
                bw_delta = n[4]/s[4]
            else:
                bw_delta = 0

        s.append(time_delta)
        s.append(amp_delta)
        s.append(freq_delta)
        s.append(bw_delta)

        # Start the partials at 0
        s[0] = s[0] - first_time

        if(s[5]>=0): #new partial
            if(len(osc_q)):
                osc_map[s[1]] = osc_q.popleft()
                # Replace the partial_idx with a osc offset
                s[1] = osc_map[s[1]]
                sequence.append(s)
        else:
            osc = osc_map.get(s[1], None)
            if(osc is not None):
                s[1] = osc_map[s[1]]
                sequence.append(s)
                if(s[5] == -2): # last bp
                    # Put the oscillator back
                    osc_q.appendleft(osc)
        if(len(osc_q) < min_q_len): min_q_len = len(osc_q)
    print("%d partials and %d breakpoints, max oscs used at once was %d" % (partial_count, len(sequence), max_oscs - min_q_len))
    # Fix sustain_ms
    if(metadata.get("sustain_ms", 0) > 0):
        metadata["sustain_ms"] = metadata["sustain_ms"] - first_time
    metadata["oscs_alloc"] = max_oscs-min_q_len
    return (metadata, sequence)


def play(sequence, osc_offset=0, sustain_ms = -1, sustain_len_ms = 0, time_ratio = 1, pitch_ratio = 1, amp_ratio = 1, bw_ratio = 1, round_robin=False):
    # i take a sequence and play it to AMY, just like native AMY will do from a .h file
    my_start_time = amy.millis()

    if(round_robin):
        next_client = 0
        osc_to_client_map = {}
        print("Syncing mesh....")
        clients = len(amy.sync())
        # After a sync, we don't want to immediately spam the mesh, so let's wait 2000ms
        time.sleep(2)
        print("Ready to play among %d speakers" % (clients))
    
    sustain_offset = 0
    if(sustain_ms > 0):
        if(sustain_ms > sequence[-1][0]):
            print("Moving sustain_ms from %d to %d" % (sustain_ms, sequence[-1][0]-100))
            sustain_ms = sequence[-1][0] - 100

    for i,s in enumerate(sequence):
        # Wait for the item in the sequence to be close, so I don't overflow the synthesizers' state
        while(my_start_time + (s[0] / time_ratio) > (amy.millis() - 500)):
            time.sleep(0.01)

        # Make envelope strings
        bp0 = "%d,%s,0,0" % (s[6] / time_ratio, amy.trunc(s[7]))
        bp1 = "%d,%s,0,0" % (s[6] / time_ratio, amy.trunc(s[8]))
        if(bw_ratio > 0):
            bp2 = "%d,%s,0,0" % (s[6] / time_ratio, amy.trunc(s[9]))
        else:
            bp2 = ""
        if(sustain_ms > 0 and sustain_offset == 0):
            if(s[0]/time_ratio > sustain_ms/time_ratio):
                sustain_offset = sustain_len_ms/time_ratio

        osc = s[1]+osc_offset

        partial_args = {}

        if(round_robin):
            if(osc_to_client_map.get(osc,None) is None):
                osc_to_client_map[osc] = (next_client % clients)
                next_client += 1
            partial_args["client"] = osc_to_client_map[osc]

        partial_args.update({"timestamp":my_start_time + (s[0]/time_ratio + sustain_offset),
            "osc":s[1]+osc_offset,
            "wave":amy.PARTIAL,
            "amp":s[3]*amp_ratio,
            "freq":s[2]*pitch_ratio,
            "feedback":s[4]*bw_ratio,
            "bp0":bp0, "bp1":bp1, "bp2":bp2,
            "bp0_target":amy.TARGET_AMP+amy.TARGET_LINEAR,
            "bp1_target":amy.TARGET_FREQ+amy.TARGET_LINEAR,
            "bp2_target":amy.TARGET_FEEDBACK+amy.TARGET_LINEAR})

        if(s[5]==-2): #end, add note off
            amy.send(**partial_args, vel=0)
        elif(s[5]==-1): # continue
            amy.send(**partial_args)
        else: #start, add phase and note on
            amy.send(**partial_args, vel=s[3]*amp_ratio, phase=s[5])

    return sequence[-1][0]/time_ratio

#In [6]: partials.generate_partials_header(fns,amp_floor=-40,analysis_window=40,freq_drift=5,hop_time=0.04,freq_res=5)
def generate_partials_header(filenames=None, **kwargs):
    # given a list of filenames, output a partials.h
    out = open("main/amy/partials.h", "w")
    out.write("// Automatically generated by partials.generate_partials_header()\n#ifndef __PARTIALS_H\n#define __PARTIALS_H\n#define PARTIALS_PATCHES %d\n" % (len(filenames)))
    all_partials = []
    if(filenames is None):
        import glob
        filenames = glob.glob('sounds/partial_sources/*.wav')
    for f in filenames:
        m, s = sequence(f, **kwargs)
        if(m is not None):
            all_partials.append((m ,s))
    out.write("const partial_breakpoint_map_t partial_breakpoint_map[%d] = {\n" % (len(all_partials)))
    out.write("\t// offset, length, midi_note, sustain_ms, oscs_alloc\n")
    start = 0
    for p in all_partials:
        out.write("\t{ %d, %d, %d, %d, %d }, /* %s */ \n" % (start, len(p[1]), p[0].get("midi_note", 0), p[0].get("sustain_ms", 0),  p[0]["oscs_alloc"], p[0]["filename"]))
        start = start + len(p[1])
    out.write("};\n");
    out.write("const partial_breakpoint_t partial_breakpoints[%d] = {\n" % (start))
    out.write("\t// ms_offset, osc, freq, amp, bw, phase, ms_delta, amp_delta, freq_delta, bw_delta\n")
    for p in all_partials:
        for s in p[1]:
            out.write("\t { %d, %d, %f, %f, %f, %f, %d, %f, %f, %f }, \n" % tuple(s))
    out.write("};\n")
    out.write("#endif // __PARTIALS_H\n")
    out.close()

# OK defaults here
def test(   filename="/Users/bwhitman/sounds/billboard/0157/0157.mp4", \
                    max_len_s=60, \
                    freq_res = 10, \
                    analysis_window = 100, \
                    time_ratio = 1, \
                    max_oscs = 40, \
                    bw_ratio = 0,\
                    amp_ratio = 1, \
                    pitch_ratio = 1, \
                    amp_floor = -40, \
                    hop_time = 0.04, \
                    sustain_len_ms = 0, \
                    **kwargs):
    import sounddevice as sd
    amy.stop()
    amy.live()
    m,s = sequence(filename, max_len_s = max_len_s, freq_res = freq_res, analysis_window = analysis_window, amp_floor=amp_floor, hop_time=hop_time, max_oscs=max_oscs)
    ms = play(s, sustain_ms = m.get("sustain_ms", -1), time_ratio=time_ratio, pitch_ratio=pitch_ratio, amp_ratio=amp_ratio, bw_ratio = bw_ratio, sustain_len_ms = sustain_len_ms)
    sd.play(amy.render(ms/1000.0))

