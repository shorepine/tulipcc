"""Random musical patterns to audition a loaded AMYboard sketch.

A PatternPlayer runs in its own thread and sends MIDI note on/off to the board
(channel 1 by default — what the World sketches listen on) until stopped. It walks
a short diatonic chord progression and, for each chord, plays an **arpeggio phase**
followed by a **block-chord phase** — so you hear pattern, chord, pattern, chord…
Each performance picks a random root, scale, arp direction, tempo and progression.
"""

import random
import threading

SCALES = {
    "major":          [0, 2, 4, 5, 7, 9, 11],
    "minor":          [0, 2, 3, 5, 7, 8, 10],
    "dorian":         [0, 2, 3, 5, 7, 9, 10],
    "phrygian":       [0, 1, 3, 5, 7, 8, 10],
    "lydian":         [0, 2, 4, 6, 7, 9, 11],
    "mixolydian":     [0, 2, 4, 5, 7, 9, 10],
    "harmonic_minor": [0, 2, 3, 5, 7, 8, 11],
}

# Arp direction within the arpeggio phase.
STYLES = ["up", "down", "updown", "random_walk"]

# Chord progressions as 0-indexed scale degrees (I=0, ii=1, … vi=5).
PROGRESSIONS = [
    [0, 4, 5, 3],   # I  V  vi IV
    [0, 5, 3, 4],   # I  vi IV V
    [5, 3, 0, 4],   # vi IV I  V
    [0, 3, 4, 0],   # I  IV V  I
    [0, 4, 1, 4],   # I  V  ii V
    [0, 3, 0, 4],
]

NOTE_NAMES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]


def note_name(midi):
    return "%s%d" % (NOTE_NAMES[midi % 12], midi // 12 - 1)


class Pattern:
    def __init__(self, root, scale, style, bpm, octaves, velocity, progression):
        self.root = root
        self.scale = scale
        self.style = style
        self.bpm = bpm
        self.octaves = octaves
        self.velocity = velocity
        self.progression = progression

    def describe(self):
        return "%s %s · arp+chords (%s) · %d BPM" % (
            note_name(self.root), self.scale, self.style, self.bpm)

    def as_dict(self):
        return {"root": self.root, "root_name": note_name(self.root), "scale": self.scale,
                "style": self.style, "bpm": self.bpm, "octaves": self.octaves,
                "progression": self.progression}


def random_pattern():
    root = random.randint(48, 62)  # C3..D4
    scale = random.choice(list(SCALES))
    style = random.choice(STYLES)
    bpm = random.choice([72, 84, 96, 110, 120, 132])
    octaves = random.choice([1, 1, 2])
    velocity = random.randint(85, 112)
    progression = random.choice(PROGRESSIONS)
    return Pattern(root, scale, style, bpm, octaves, velocity, progression)


class PatternPlayer(threading.Thread):
    """Plays `pattern` on `link` forever (in a thread) until stop()."""

    def __init__(self, link, pattern, channel=0):
        super().__init__(daemon=True)
        self.link = link
        self.pattern = pattern
        self.channel = channel
        self._stop = threading.Event()
        self._sounding = []  # notes currently on

    def stop(self):
        self._stop.set()

    def _wait(self, seconds):
        self._stop.wait(seconds)  # wakes early on stop

    def _on(self, note, vel=None):
        self.link.note_on(note, vel or self.pattern.velocity, self.channel)
        self._sounding.append(note)

    def _off(self, note):
        self.link.note_off(note, self.channel)
        if note in self._sounding:
            self._sounding.remove(note)

    def _all_off(self):
        for n in list(self._sounding):
            self._off(n)
        self.link.all_notes_off(self.channel)

    def _diatonic_chord(self, degree, size=3):
        """Build an in-key chord by stacking scale thirds from `degree`."""
        steps = SCALES[self.pattern.scale]
        n = len(steps)
        out = []
        for k in range(size):
            idx = degree + 2 * k
            out.append(self.pattern.root + 12 * (idx // n) + steps[idx % n])
        return out

    def run(self):
        p = self.pattern
        beat = 60.0 / p.bpm
        try:
            while not self._stop.is_set():
                for degree in p.progression:
                    if self._stop.is_set():
                        break
                    chord = self._diatonic_chord(degree, size=random.choice([3, 3, 4]))
                    self._arp_phase(chord, beat)      # pattern
                    if self._stop.is_set():
                        break
                    self._chord_phase(chord, beat)    # chord
        finally:
            self._all_off()

    def _arp_phase(self, chord, beat, eighths=8):
        """One bar of eighth-note arpeggio over the chord tones."""
        p = self.pattern
        notes = list(chord) + [chord[0] + 12]  # add the octave on top
        if p.octaves >= 2:
            notes = notes + [n + 12 for n in chord]
        if p.style == "down":
            notes = list(reversed(notes))
        elif p.style == "updown":
            notes = notes + list(reversed(notes[1:-1]))
        step = beat / 2.0
        gate = step * 0.85
        for i in range(eighths):
            if self._stop.is_set():
                return
            n = random.choice(notes) if p.style == "random_walk" else notes[i % len(notes)]
            self._on(n, p.velocity + random.randint(-8, 8))
            self._wait(gate)
            self._off(n)
            self._wait(step - gate)

    def _chord_phase(self, chord, beat, beats=2):
        """Hold the block chord for a couple of beats."""
        hold = beat * beats
        for n in chord:
            self._on(n, self.pattern.velocity)
        self._wait(hold * 0.92)
        for n in chord:
            self._off(n)
        self._wait(hold * 0.08)
