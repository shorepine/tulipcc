# music.py 
# little computer for chords / scales / keys

class Progression:
    # TODO add 6/7 and etc to these
    # http://en.wikipedia.org/wiki/Roman_numeral_analysis
    roman_major = ["I", "II", "III", "IV", "V", "VI", "VII"]
    roman_minor = [x.lower() for x in roman_major]

    def __init__(self, progression, key):
        # progression is like ["I", "vi", "IV", "V"]
        self.progression = progression
        self.key = key # Key object, like Key("A:maj")
        self.chords = []

        for p in self.progression:
            try:
                self.chords.append(Chord(self.key.notes[self.roman_major.index(p)].name()+":maj"))
            except ValueError:
                try:
                    self.chords.append(Chord(self.key.notes[self.roman_minor.index(p)].name()+":min"))
                except ValueError:
                    print ("Could not parse progression string " + p)


class Key:
    def __init__(self, key_string): # key_string like "A:maj"
        self.key_string = key_string
        self.chord = Chord(key_string+"/scale")
        self.notes = self.chord.notes

    def note_in_key(note):
        if note in self.notes: return True
        return False

class Note(object):
    """
        A note with an octave
        Create like 
            Note(56)  -- midinote
            Note("C#", 5) -- named note, octave
            Note(note, 5) -- NoteClass object, octave
            Note("C") -- no octave means octave = 4
    """
    def __init__(self, *args):
        if isinstance(args[0], int): # midinote
            (note_number, self.octave) = (args[0] % 12, int(args[0] / 12))
            self.note = NoteClass(note_number)
            assert args[0] == self.midinote() # just making sure
        else:
            if isinstance(args[0], NoteClass): # note
                self.note = args[0]
            else: # string or unicode
                self.note = NoteClass(args[0])
            if len(args) > 1:
                self.octave = args[1]
            else:
                self.octave = 4

    def midinote(self):
        """
            MIDI note number of note
        """
        return self.note.midinote(octave=self.octave)

    def frequency(self):
        """
            Base frequency in Hz of note
        """
        return self.note.frequency(octave=self.octave)


class NoteClass:
    # TODO: more precision for this
    freq = [
        16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,
        32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74,
        65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.8, 110.0, 116.5, 123.5,
        130.8, 138.6, 146.8, 155.6, 164.8, 174.6, 185.0, 196.0, 207.7, 220.0, 233.1, 246.9,
        261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3, 440.0, 466.2, 493.9,
        523.3, 554.4, 587.3, 622.3, 659.3, 698.5, 740.0, 784.0, 830.6, 880.0, 932.3, 987.8,
        1047 , 1109 , 1175 , 1245 , 1319 , 1397 , 1480 , 1568 , 1661 , 1760 , 1865 , 1976 ,
        2093 , 2217 , 2349 , 2489 , 2637 , 2794 , 2960 , 3136 , 3322 , 3520 , 3729 , 3951 ,
        4186 , 4435 , 4699 , 4978 , 5274 , 5588 , 5920 , 6272 , 6645 , 7040 , 7459 , 7902 
    ]
    notes = {
        "C":0,  "C#":1,  "Db":1,  "D":2,  "D#":3,  "Eb":3,  "E":4,  "E#":5,  "Fb":4,  "F":5,  "F#":6,  "Gb":6,  "G":7,  "G#":8,  "Ab":8,  "A":9,  "A#":10,  "Bb":10,  "B":11,  "Cb":11,  "B#":12
    }
    for n in list(notes.keys()):
        notes[n+"1"] = notes[n] + 12
        notes[n+"2"] = notes[n] + 24

    basenames = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
    names = []
    for n in basenames:
        names.append(n)
    for n in basenames:
        names.append(n+"1")
    for n in basenames:
        names.append(n+"2")

    def __init__(self, note):
        self.valid = True
        if isinstance(note, str):
            self.note = self.notes.get(note, -1)
        else:
            self.note = note

        if self.note < 0:
            self.valid = False

    def midinote(self, octave=4):
        # TODO: is this right?
        return (self.note)+(octave*12)

    def name(self):
        if self.note > -1:
            return self.names[self.note]
        else:
            return "unknown"

    def __repr__(self):
        return "%s" % (self.name())

    def frequency(self, octave=4):
        return self.freq[(octave*12) + self.note ]


class Chord:
    # interval map
    [
        root, minor_second, major_second, minor_third, major_third, perfect_fourth, diminished_fifth, augmented_fourth, perfect_fifth, 
        minor_sixth, major_sixth, minor_seventh, major_seventh, perfect_octave, minor_ninth, major_ninth, perfect_fifth_octave, major_thirteenth
    ] = [0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 13, 14, 19, 21]

    annotations_map = {
        # TODO: we completely punt on inversions and it shows
        "maj":[root, major_third, perfect_fifth], 
        "min":[root, minor_third, perfect_fifth],
        "7":[root, major_third, perfect_fifth, minor_seventh],
        "7(#9)":[root, major_third, perfect_fifth, minor_seventh, major_ninth+1],
        "7(b9)":[root, major_third, perfect_fifth, minor_seventh, major_ninth-1],
        "min7":[root, minor_third, perfect_fifth, minor_seventh], 
        "maj7":[root, major_third, perfect_fifth, major_seventh], 
        "maj13":[root, major_third, perfect_fifth, major_seventh, major_ninth, major_thirteenth], 
        "maj/5":[root, perfect_fifth], # not really
        "5":[root, perfect_fifth, perfect_fifth_octave],
        "1":[root],
        "add9":[root, major_third, perfect_fifth, major_ninth], # ?
        "maj/3":[root, major_third, perfect_fifth], # not really
        "maj/13":[root, major_third, perfect_fifth, major_thirteenth], # not really
        "sus4":[root, perfect_fourth, perfect_fifth],
        "maj(9)":[root, major_third, perfect_fifth, major_seventh, major_ninth],
        "sus4(b7,9)":[root, perfect_fourth, perfect_fifth, minor_seventh, major_ninth],
        "maj/9":[root, major_third, perfect_fifth, major_seventh, major_ninth], # not really
        "sus4(b7)":[root, perfect_fourth, perfect_fifth, minor_seventh],
        "min9":[root, minor_third, perfect_fifth, minor_seventh, major_ninth],
        "maj/b7":[root, major_third, perfect_fifth, minor_seventh], 
        "min/3":[root, minor_third, perfect_fifth], # not really
        "maj6":[root, major_third, perfect_fifth, major_sixth],
        "maj6(b7)":[root, major_third, perfect_fifth, major_sixth,major_seventh-1],
        "sus4(b7,9,13)":[root, perfect_fourth, perfect_fifth, minor_seventh, major_ninth, major_thirteenth],
        "maj/scale":[0, 2, 4, 5, 7, 9, 11],
        "min/scale":[0, 2, 3, 5, 7, 8, 10],
    }
   
    def __init__(self, chord_string):
        self.chord_string = chord_string
        self.notes = []
        self.annotation = []
        self.annotation_name = "unknown"
        self.valid = True
        self.root_note = NoteClass("none")
        self.parse_chord(chord_string)

    def __repr__(self):
        return "%s - %s %s (from %s)" % (self.root_note, self.annotation_name, self.notes, self.chord_string)


    def parse_chord(self, chord_string):
        if chord_string == "N" or chord_string == "&pause" or chord_string=="*":
            # These all mean more or less no music, TODO maybe set a play_silence flag vs. the time sig ones
            return

        root_string = chord_string[:chord_string.find(":")]
        self.root_note = NoteClass(root_string)
        if not self.root_note.valid:
            self.valid = False
            return

        annotation_string = chord_string[chord_string.find(":")+1:]
        self.annotations = self.annotations_map.get(annotation_string,[])

        if len(self.annotations):
            self.annotation_name = annotation_string
        else:
            self.valid = False

        self.notes = [NoteClass(self.root_note.note + n) for n in self.annotations]

    def names(self):
        return [n.name() for n in self.notes]

    def frequencies(self, octave=4):
        return [n.frequency(octave=octave) for n in self.notes]

    def midinotes(self, octave=4):
        return [n.midinote(octave=octave) for n in self.notes]



