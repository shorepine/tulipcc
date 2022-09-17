import glob
import math
import os
import struct
import sys

"""
Print detailed information on the headers and structure of a WAV file.

Author:  Kristian Ovaska <kristian.ovaska@helsinki.fi>
WWW:     http://www.cs.helsinki.fi/u/hkovaska/wavdumper/
License: GNU General Public License
Version: 0.5 (2003-11-13)

See readme.html for details.
"""

NOTES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']

TEXT_CHUNKS = {
    'iarl': 'Archival location',
    'iart': 'Artist',
    'icms': 'Commissioned',
    'icmt': 'Comments',
    'icop': 'Copyright',
    'icrd': 'Creation date',
    'ieng': 'Engineer',
    'ignr': 'Genre',
    'ikey': 'Keywords',
    'inam': 'Name',
    'imed': 'Medium',
    'iprd': 'Product',
    'isbj': 'Subject',
    'isft': 'Software',
    'isrc': 'Source',
    'isrf': 'Original medium',
    'itch': 'Technician',
}

# Only reasonably-common formats here; see RFC 2361 for the rest
WAVE_FORMAT = {
    0x01: 'Uncompressed PCM',
    0x02: 'Microsoft ADPCM',
    0x03: 'IEEE Float',
    0x06: 'ITU G.711 a-law',
    0x07: 'ITU G.711 u-law',
    0x11: 'IMA ADPCM',
    0x16: 'ITU G.723 ADPCM',
    0x31: 'GSM 6.10',
    0x40: 'ITU G.721 ADPCM',
    0x50: 'MPEG',
    0x55: 'MPEG Layer 3',
}

def stripNull(string):
    return string.strip('\0')


class Wav:
    """WAV parser and printer. Only printInfo is used outside the class.
    Technically, the class is similar to LL(1) parsers. There are read_XYZ
    methods that correspond to WAV chunk types. They are defined in
    alphabetical order, with assistant methods right after the main method.
    A few methods, like read_junk, are used for more than one type of chunk
    (junk and pad, in this case).    
    """

    def __init__(self, filename):
        self.filename = filename

    def printInfo(self, outfile=None):
        """Read file and print info. Outfile is a file-like object; default
        is sys.stdout."""

        self.reset()
        if outfile:
            self.outfile = outfile

        if (not os.path.exists(self.filename)) or (not os.path.isfile(self.filename)):
            self.p('Could not open %s' % self.filename)
            return

        try:
            # We use open instead of file for backwards compability
            self.infile = open(self.filename, 'rb')
        except IOError:
            self.p('Could not open %s' % self.filename)
            return

        self.filesize = os.path.getsize(self.filename)
        self.p('File: %s (%s bytes)' % (self.filename, self.filesize))

        # First, find out if we have a valid WAV file.
        if self.filesize<12:
            self.p('Not a WAV file')
            self.infile.close()
            return
        id, = struct.unpack('4s', self.read(4))
        if id not in [b'RIFF', b'RIFX']:
            self.p('Not a WAV file')
            self.infile.close()
            return
        if id=='RIFX':
            self.p('File is in big-endian format')
            self.endian = '>'

        length, rifftype = struct.unpack(self.endian+'i 4s', self.read(8))
        if rifftype!=b'WAVE':
            self.p('Not a WAV file')
            self.infile.close()
            return
        if length!=self.filesize-8:
            self.p('Warning: incorrect RIFF header length (%s)' % length)

        # Now, parse the file.
        while self.pos()<self.filesize:
            self.read_chunk()
        if self.pos()!=self.filesize:
            self.p('Error: pos!=filesize at end (pos=%s, filesize=%s)' % (self.pos(), self.filesize))
        self.infile.close()


    def reset(self):
        self.infile = None
        self.outfile = open(os.devnull, 'w')
        self.endian = '<'     # default: little-endian, '>' is big-endian
        self.indent = 0       # indentation level
        self.fmtChunks = 0    # number of fmt chunks
        self.dataChunks = 0   # number of data chunks
        self.waveFormat = 0
        self.sampleRate = 0
        self.bytesPerSample = 0

    def read(self, bytes):
        return self.infile.read(bytes)

    def skip(self, bytes):
        self.infile.seek(bytes, 1)

    def pos(self):
        return self.infile.tell()

    def p(self, *args):
        """Print args with indentation."""
        if self.indent:
            self.outfile.write('  ' * self.indent)
        for arg in args:
            print (arg, file=self.outfile)
        print ('\n', file=self.outfile)

    def read_chunk(self):
        """Determine chunk type by reading a few bytes and launch a specialized
        read_ function. Return bytes read: this is needed by read_list."""

        id, length = struct.unpack(self.endian+'4s i', self.read(8))
        self.p('Chunk at pos %s: id = "%s", length = %s bytes' % (self.pos(), id, length))

        id = id.lower()

        methodmap = {
            b'cue ': self.read_cue,
            b'data': self.read_data,
            b'disp': self.read_disp,
            b'fact': self.read_fact,
            b'fmt ': self.read_fmt,
            b'junk': self.read_junk,
            b'labl': self.read_labl,
            b'ltxt': self.read_ltxt,
            b'list': self.read_list,
            b'note': self.read_labl,
            b'pad ': self.read_junk,
            b'plst': self.read_plst,
            b'smpl': self.read_smpl,
        }

        self.indent += 1
        if id in methodmap:
            f = methodmap[id]
            f(length)
        elif id in TEXT_CHUNKS: # short iXYZ text chunks
            s = stripNull(self.read(length))
            self.p('%s: %s' % (TEXT_CHUNKS[id], s))
        else:
            self.p('Unknown chunk')
            self.skip(length)
        self.indent -= 1

        if length%2 != 0:
            self.skip(1) # chunks are word-aligned, but the pad byte isn't counted in length
            length += 1

        return length+8

### read_ methods for each chunk. ###

    def read_cue(self, length):
        """Cue point list chunk."""
        points, = struct.unpack(self.endian+'i', self.read(4))
        count = 4

        self.p('Cue List Chunk')
        self.p('Points:', points)

        for i in range(points):
            self.indent += 1
            count += 24
            self.read_cuepoint()
            self.indent -= 1

        if length>count:
            self.skip(length-count)

    def read_cuepoint(self):
        """Cue point inside cue point list chunk."""
        id, position, fccChunk, chunkStart, blockStart, sampleOffset = \
        struct.unpack(self.endian+'2i 4s 3i', self.read(24))
        self.p('ID: %s, playlist pos: %s, sample offset: %s' % (id, position, sampleOffset))

    def read_data(self, length):
        """Sample data chunk."""
        if self.dataChunks>0:
            self.p('ERROR: too many Data chunks!')
        self.dataChunks += 1

        self.p('%s bytes sample data' % length)
        if self.waveFormat==1:
            self.samples = length/self.bytesPerSample

            self.p('%s samples' % int(self.samples))
            self.p('%.3f seconds' % (float(self.samples)/self.sampleRate))
        self.skip(length)

    def read_disp(self, length):
        """Some Microsoft(?) thing, couldn't find specs."""
        self.p('Disp Chunk')
        if length==0:
            return

        dispType, = struct.unpack(self.endian+'i', self.read(4))
        count = 4

        if dispType==1:
            s = stripNull(self.read(length-count))
            count += length-count
            self.p('Type: %s, text: %s' % (dispType, s))
        elif dispType==8:
            self.p('Type: %s, bitmap' % dispType)
        else:
            self.p('Type:', dispType)

        if length>count:
            self.skip(length-count)

    def read_fact(self, length):
        self.p('Fact Chunk')
        count = 0
        if length>=4:
            numberOfSamples, = struct.unpack(self.endian+'i', self.read(4))
            self.p('Number of samples in file:', numberOfSamples)
            count += 4
        if length>count:
            self.skip(length-count)

    def read_fmt(self, length):
        """Format chunk, basic info about the wave data."""
        if self.fmtChunks>0:
            self.p('ERROR: too many Format chunks!')
        self.fmtChunks += 1

        formatTag, self.channels, samplesPerSec, avgBytesPerSec, blockAlign = \
        struct.unpack(self.endian+'h H I I H', self.read(14))
        count = 14

        self.sampleRate = float(samplesPerSec)
        self.waveFormat = formatTag

        self.p('Format Chunk')
        if formatTag in WAVE_FORMAT:
            self.p('Data format:', WAVE_FORMAT[formatTag])
        else:
            self.p('Data format: unknown (%s)' % formatTag)
        self.p('Channels:', self.channels)
        self.p('Sample rate: %s Hz' % samplesPerSec)
        self.p('Average bytes per sec:', avgBytesPerSec)
        self.p('Block align (bytes):', blockAlign)

        if length>=16:
            # bitsPerSample wasn't part of the original WAV specs...
            bitsPerSample, = struct.unpack(self.endian+'H', self.read(2))
            count += 2
            self.p('Bits per sample:', bitsPerSample)
            self.bytesPerSample = float(self.channels)*(math.ceil(bitsPerSample/8.0))
        else:
            # A hack, but should be very rare anyway
            self.bytesPerSample = float(blockAlign)

        if length>=18:
            additionalSize, = struct.unpack(self.endian+'H', self.read(2))
            count += 2
            if additionalSize>0:
                self.p('%s bytes additional space reported' % additionalSize)

        if length>count:
            self.p('%s bytes additional skipped' % (length-count))
            self.skip(length-count)

    def read_junk(self, length):
        """Junk chunk, can be used for aligning."""
        self.p('Junk/Pad Chunk')
        self.skip(length)

    def read_labl(self, length):
        """labl or note chunk"""
        id, = struct.unpack(self.endian+'i', self.read(4))
        self.p('Label/note chunk, id = %s' % id)
        if length>4:
            s = stripNull(self.read(length-4))
            self.p('Text:', s)
        else:
            self.p('(No text)')

    def read_ltxt(self, length):
        self.p('Labeled Text Chunk')

        id, sampleLength, purporse, country, languange, dialect, codepage = \
        struct.unpack(self.endian+'3i 4h', self.read(20))

        if length>20:
            s = stripNull(self.read(length-20))
            self.p('Text:', s)
        else:
            self.p('(No text)')

    def read_list(self, length):
        """List chunk, contains other chunks."""
        listType, = struct.unpack(self.endian+'4s', self.read(4))
        count = 4
        self.p('List Chunk, id = "%s"' % listType)

        while count<length-8:
            count += self.read_chunk()

        if length>count:
            self.skip(length-count)

    def read_plst(self, length):
        """Playlist chunk."""
        segments, = struct.unpack(self.endian+'i', self.read(4))
        count = 4
        self.p('Playlist Chunk')
        self.p('Segments:', segments)

        totalLength = 0
        for i in range(segments):
            self.indent += 1
            id, segLength, repeats = struct.unpack('3i', self.read(3*4))
            count += 3*4
            self.p('ID: %s, length: %s, repeats: %s' % (id, segLength, repeats))
            totalLength += segLength*repeats
            self.indent -= 1
        self.p('Total playing time: %.1f seconds' % (float(totalLength)/self.sampleRate))

        if length>count:
            self.skip(length-count)

    def read_smpl(self, length):
        """Sampler info chunk."""
        manufacturer, product, samplePeriod, self.midiUnityNote, midiPitchFraction, smpteFormat, \
        smpteOffset, self.sampleLoops, samplerData = struct.unpack(self.endian+'9i', self.read(9*4))
        count = 9*4 # read bytes

        self.p('Sampler Chunk')
        self.p('Manufacturer: %x' % manufacturer)
        self.p('Product: %x' % product)
        noteStr = '%s-%s' % (NOTES[self.midiUnityNote % 12], self.midiUnityNote/12)
        self.p('MIDI note (60=middle-C): %s (%s) +- %.1f%%' % \
            (self.midiUnityNote, noteStr, float(midiPitchFraction)/0x80000000))

        for i in range(self.sampleLoops):
            self.p('Sample loop')
            self.indent += 1
            self.read_sampleloop(samplerData)
            count += 6*4 + samplerData
            self.indent -= 1

        if length>count:
            self.skip(length-count)

    def read_sampleloop(self, additional):
        """Sampleloop inside sampler info. additional is the length of
        vendor-specific data, which is skipped."""

        id, loopType, self.loopstart, self.loopend, fraction, playCount = struct.unpack(self.endian+'6i', self.read(6*4))

        self.p('Loop ID:', id)
        if loopType==0:
            s = 'forward'
        elif loopType==1:
            s = 'ping pong'
        elif loopType==2:
            s = 'backward'
        elif loopType>=3 and loopType<=31:
            s = 'unknown, WAV standard (%s)' % loopType
        else:
            s = 'device specific (%s)' % loopType
        self.p('Type:', s)
        self.p('Range: %s - %s' % (self.loopstart, self.loopend))
        if playCount==0:
            self.p('Repeat: infinity')
        else:
            self.p('Repeat: %s times' % playCount)

        if additional:
            self.p('%s bytes additional space skipped' % additional)
            self.skip(additional)


########################################################################

def argfiles(args, onlyFiles=False):
    """Expand arguments (globs and non-globs). Return a flat list."""
    files = []
    for arg in args:
        if ('*' in arg) or ('?' in arg):
            files.extend(glob.glob(arg))
        else:
            files.append(arg)
    if onlyFiles:
        files = filter(os.path.isfile, files)
    return files

def main(args):
    if not args:
        print('Usage: wavdump <files>')
        return

    files = argfiles(args)
    if not files:
        print('No matching files found.')
        return

    wav = Wav(files[0])
    wav.printInfo()

    for filename in files[1:]:
        print()
        wav = Wav(filename)
        wav.printInfo()

if __name__=='__main__':
    main(sys.argv[1:])
