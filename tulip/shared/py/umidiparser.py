"""
NAME
    umidiparser
ff
AUTHOR
    Hermann Paul von Borries

LICENSE
    MIT, Copyright (c) Hermann Paul von Borries

INSTALLATION
    Copy umidiparser.py to your device.

DESCRIPTION
    This module allows to parse midi files.
    This module does not contain a sound synthesizer, only the capabilities to
    read and interpret a midi file.
    Example:

    import umidiparser
    Import utime
    for event in umidiplay.MidiFile("example.mid"):
        utime.sleep_us( event.delta_us )
        if event.status == umidiplay.NOTE_ON:
            ... start the note event.note on event.channel with event.velocity
        elif event.status == umidiplay.NOTE_OFF :
            ... stop the note event.note stop to sound ...
        elif event.status == umidiplay.PROGRAM_CHANGE:
            ... change midi program to event.program on event.channel ....
        else:
            print("other event", event )

"""

import time
import sys

# Change log: v1.2
#   Added CircuitPython compatibility
#   Removed use of os.path.abspath, does not exist in micropython/circuitpython
#   Renamed time functions "ticks_now_us", "ticks_diff_us" to "time_now_us", "time_diff_us"
#   Changed decode from meta event data for compatibility with all Python versions, now only
#   ascii is decoded.
#   New method MidiTrack.play, removed track_number parameter.
#   Corrected possible error if playing open file again
#   Added event.is_channel() to test for channel events
#   Allow MidiFile.play() used in async for (with asyncio.sleep instead of sleep). Requires asyncio
#   Play funcion computes event.timestamp_us for each event
# Change log: v1.3
#   For CircuitPython, it's import asyncio. Also time_now_us now returns an integer.

# Compatibility wrapper for python/micropython/circuitpython functions
_implementation = sys.implementation.name
if _implementation == "micropython":
    from micropython import const
    try:
        import uasyncio as asyncio
    except ImportError:
        import asyncio
    time_sleep_us = lambda usec: time.sleep_us( usec )
    time_now_us = lambda: time.ticks_us()
    time_diff_us = lambda x, y: time.ticks_diff( x, y )
    asyncio_sleep_ms = lambda x: asyncio.sleep_ms( x )
elif _implementation == "circuitpython":
    from micropython import const
    try:
        import asyncio
    except:
        pass
    time_sleep_us = lambda usec: time.sleep( usec/1_000_000 )
    time_now_us = lambda: (time.monotonic_ns()+500)//1_000
    time_diff_us = lambda x, y: x - y
    asyncio_sleep_ms = lambda x: asyncio.sleep_ms( x )
else:
    # Must be CPython
    import asyncio
    const = lambda x: x
    time_sleep_us = lambda usec: time.sleep( usec/1_000_000 )
    time_now_us = lambda : int(time.time()*1_000_000)
    time_diff_us = lambda x, y: x - y
    asyncio_sleep_ms = lambda x: asyncio.sleep( x/1000 )

    # Make the @micropython.native decorator do nothing
    def micropython( function ):
        return function
    micropython.native = lambda function : function

# Only utf-8 encoding available in Micropython 1.19.1 or CircuitPython 7.3.3
# Should probably be .decode("iso8859-1", errors="backslashreplace")
decode_ascii = lambda x : "".join( chr(z) for z in x )


# Constants for the MIDI channel events, first nibble
NOTE_OFF = const(0x80)
NOTE_ON = const(0x90)
POLYTOUCH = const(0xa0)
CONTROL_CHANGE = const(0xb0)
PROGRAM_CHANGE = const(0xc0)
AFTERTOUCH = const(0xd0)
PITCHWHEEL = const(0xe0)
_FIRST_CHANNEL_EVENT = const(0x80)
_LAST_CHANNEL_EVENT = const(0xef)
# Most midi channel events have 2 bytes of data, except this range that has 1 byte events
# which consists of two event types:
_FIRST_1BYTE_EVENT = const(0xc0)
_LAST_1BYTE_EVENT = const(0xdf)

# Meta messages
_META_PREFIX = const(0xff)
# Meta messages, second byte
SEQUENCE_NUMBER = const(0x00)
TEXT = const(0x01)
COPYRIGHT = const(0x02)
TRACK_NAME = const(0x03)
INSTRUMENT_NAME = const(0x04)
LYRICS = const(0x05)
MARKER = const(0x06)
CUE_MARKER = const(0x07)
PROGRAM_NAME = const(0x08)
DEVICE_NAME = const(0x09)
CHANNEL_PREFIX = const(0x20)
MIDI_PORT = const(0x21)
END_OF_TRACK = const(0x2f)
SET_TEMPO = const(0x51)
SMPTE_OFFSET = const(0x54)
TIME_SIGNATURE = const(0x58)
KEY_SIGNATURE = const(0x59)
SEQUENCER_SPECIFIC = const(0x7f)
_FIRST_META_EVENT = const(0x00)
_LAST_META_EVENT = const(0x7f)

# Sysex/escape events
SYSEX = const(0xf0)
ESCAPE = const(0xf7)

# MidiParser reuses a buffer for meta and sysex events
# This is the starting size for this buffer.
# If there are larger messages in a file, this buffer will increase automatically
# to accomodate the larger data
_INITIAL_EVENT_BUFFER_SIZE = const(20)


# Parse midi variable length number format,
# used for time deltas and meta message lengths
def _midi_number_to_int( midi_data ):
    # Converts a midi variable length number to integer. In midi files,
    # variable length numbers are used for delta times (time difference between
    #one midi event and the next) and for variable length fields in meta messages.
    data_byte = next( midi_data )
    if data_byte <= 0x7f:
        # This "if" is really only for an improvement in speed, because
        # most variable length numbers tend to be 1 byte long, so this
        # is the most probable execution path.
        return data_byte

    # The value spans more than one byte, parse until
    # a byte with most significant bit "on" is found, and gather
    # 7 bits of data for each byte found, according to Midi standard.
    value = data_byte & 0x7f
    while data_byte >= 0x80:
        data_byte = next( midi_data )
        value = (value<<7) | (data_byte & 0x7f )
    return value



def _process_events( event_iterator,
                    miditicks_per_quarter,
                    reuse_event_object ):
    # This function iterates through the provided event iterator,
    # getting one MidiEvent at a time, and processes MIDI meta set tempo
    # events to convert the time delta in MIDI ticks to time delta in microseconds,
    # rounded to the next microsecond.
    # The function also ensures that there will be a MIDI meta end of track event
    # at the end (as should be according to MIDI standard).
    # If the reuse_event_object parameter is set to False, a independent deep copy
    # of each event is returned. If the reuse_event_object is True, the same
    # object is returned over and over, to reduce CPU usage and RAM heap allocation.

    # Start with default "microseconds per quarter" according to midi standard
    tempo = 500_000

    for event in event_iterator:

        if not reuse_event_object:
            event = event.copy()

        # According to MIDI spec
        # time (in ms) = number_of_ticks * tempo / divisor * 1000
        # where tempo is expressed in microseconds per quarter note and
        # the divisor is expressed in MIDI ticks per quarter note
        # Do the math in microseconds.
        # Do not use floating point, in some microcontrollers
        # floating point is slow or lacks precision.
        event.delta_us = ( event.delta_miditicks * tempo \
                           + (miditicks_per_quarter//2) \
                          ) // miditicks_per_quarter


        # Process tempo meta event, get tempo to be used for
        # event.delta_us calculation for next events.
        status = event.status
        if status == SET_TEMPO:
            tempo = event.tempo

        # If end_of_track is seen, stop processing events
        elif status == END_OF_TRACK:
            yield event
            # Ignore events after end of track
            break

        yield event

    else:
        # Loop ended without end_of_track meta event, yield one
        # last event of type "end of track" to make caller happy.
        yield MidiEvent()._set_end_of_track()





class MidiParser:
    # This class instantiates a MidiParser, the class constructor
    # accepts a iterable with MIDI events in MIDI file format, i.e.
    # it accepts a iterable for the content of a MIDI file track.
    def __init__( self, midi_data ):
        # Initialize a parser on the midi_data iterable. The parsing
        # is then done with the parse_events method.

        # Allocate data buffers for the sake of CPU and RAM efficiency,
        # to avoid allocating new objects for each event parsed.

        # Save midi data iterator
        self._midi_data = midi_data

        # The first event cannot be a "running status event"
        self._running_status = None

        # This buffer is for meta and sysex events.
        # This buffer can potentially grow
        # because it will eventually contain the largest midi meta, sysex,
        # or escape  event in the file.
        self._buffer = bytearray(_INITIAL_EVENT_BUFFER_SIZE )

        # The most frequently used buffers are 1 and 2 bytes long
        # For CPU/RAM efficiency, preallocate these buffers.
        # These two buffers are used only for midi channel events
        self._buffer1 = memoryview(bytearray(1))
        self._buffer2 = memoryview(bytearray(2))


    def parse_events( self ):
        # This generator will parse the midi_data iterable
        # and yield MidiEvent objects until end of data (i.e. this
        # function is in itself a generator for events)

        # For CPU and RAM efficiency, the midi event is  returned
        # in the same object, that is, the MidiEvent returned is allocated
        # once, and set before yielding to the new values. It is responsibility
        # of the caller to copy the event if needed. The data buffer in the
        # event is also reused from one call to the next (see __init__).

        # Exceptions:
        # ValueError if the midi event subtype is in the range 0x7f to 0xff.
        # RuntimeError if there is a running status event without a previous
        # midi channel event.
        # RuntimeError if a system common or real time event is detected (event
        # status byte 0xf1-0xf6 and 0xf7-0xfe. These type of events are not allowed
        # in MIDI files.

        event = MidiEvent()
        midi_data = self._midi_data
        try:
            while True:
                # Parse a delta time
                delta = _midi_number_to_int( midi_data )

                # Parse a message
                event_status, data = self._parse_message(  )

                # Set the event with new data
                event._set( event_status, data, delta )

                yield event

        except StopIteration:
            # No more input data, "next" in called function got end of data,
            # stop this generator
            return


    def _parse_non_channel_events( self, event_status ):
        # Parses meta, sysex and escape events

        # Precondition: the event status byte has already been read.
        # Postcondition: the event data has been parsed and the next
        # byte in midi_data is a new MIDI event.

        midi_data = self._midi_data

        if event_status == _META_PREFIX:
            # Midi messages event status has format
            # 0xff nn (nn from 0x00-0x7f)
            # discard 0xff, keep nn as event status
            event_status = next( midi_data )

            # The second event status byte might not be in range
            # defined by standard
            if not  _FIRST_META_EVENT \
                    <= event_status \
                    <= _LAST_META_EVENT:
                raise ValueError(\
                    f"Meta midi second event status byte (0x{event_status:x}) "
                    "not in range 0x00-0x7f")

        # All non-channel events have a variable length field
        data_length = _midi_number_to_int( midi_data )

        # Data might be longer than available buffer
        if data_length >= len(self._buffer):
            # Increase buffer size to fit the data.
            self._buffer = bytearray( data_length )

        # Use a memoryview for efficiency, this avoids copying the buffer
        # and allows to return a slice of the current size efficiently.
        data = memoryview(self._buffer)[0:data_length]

        # Now copy the data from the input midi_data to the buffer
        for idx in range(data_length):
            data[idx] = next( midi_data )

        return event_status, data

    def _parse_channel_event( self, event_status, data_byte ):
        # Parse midi channel events, i.e. events with event status
        # byte 0x80 to 0xef. Procesess both regular channel events and
        # running status events.
        # Precondition: the event status byte has already been read.
        # Postcondition: the event data has been parsed and the next
        # byte in midi_data is a new MIDI event.

        # Check if this event is 1 or 2 bytes long
        if _FIRST_1BYTE_EVENT <= event_status \
            <= _LAST_1BYTE_EVENT:
            # This is a one-byte midi channel event,
            # such as program change, use 1 byte preallocated buffer
            data = self._buffer1
            data[0] = data_byte

        else:
            # This is a two-byte midi channel event, such as note on,
            # note off, use preallocated buffer
            data = self._buffer2
            data[0] = data_byte
            data[1]= next( self._midi_data )

        return event_status, data

    def _parse_message( self ):
        # Parse a MIDI  message in a MIDI file. Uses _parse_channel_event
        # or _parse_non_channel_events depending on the event status byte.
        # Precondition: the next byte in the midi_data is the
        # event status byte.
        midi_data = self._midi_data

        # Preconditions: the next byte in midi_data should now be
        # the starting byte of a midi event, i.e a
        # midi event status byte. Delta time has already been parsed

        # Get event_status byte
        event_status = next( midi_data )
        if event_status < 0x80:
            # This is a running event. It has no event status byte,
            # only data, the event status byte is the one from the last midi channel
            # event seen in the midi_data.

            # A running event at the beginning of a track is an error
            if self._running_status is None:
                raise RuntimeError("Midi running status without previous channel event")

            # Reuse the event_status as first data byte and parse the event
            return self._parse_channel_event(
                    self._running_status,
                    event_status )

        if _FIRST_CHANNEL_EVENT \
                <= event_status \
                <= _LAST_CHANNEL_EVENT:
            # Not a running event, this is a midi channel event
            # (status 0x80-0xef) followed by 1 or 2 bytes of data

            # Remember event status in case next event is a
            # running status event
            self._running_status = event_status

            return self._parse_channel_event(
                    event_status,
                    next( midi_data ) )

        if event_status in (_META_PREFIX, SYSEX, ESCAPE ):
            return self._parse_non_channel_events( event_status )

        # Neither midi channel event, nor meta, nor sysex/escape.
        # Real time and system common events are not expected in MIDI files.
        raise RuntimeError("Real time/system common event"
                f" status 0x{event_status:x}"
                " not supported in midi files")


class MidiEvent:
    """
    Represents a parsed midi event.

    """
    def __init__( self ):
        """
        Initializes MidiEvent, all instances are assigned None as value,
        this is. This method is used internally by MidiParser.

        Usually you will not need to create an instance of MidiEvent,
        MidiEvent objects are returned by iterating over the MidiFile
        object.

        """
        # MidiEvent private instance variables:
        #   self._event_status_byte
        #       The original event status byte, as detected in the midi file.
        #       The difference with self.status is that the _event_status_byte
        #       still has the channel number in the lower half in the case of
        #       a midi channel event.
        #
        #   self._status
        #       Same as self._event_status_byte but with the lower nibble
        #       cleared for midi channel events. self.status is a read only
        #       property for self._status.
        #
        #   self._data
        #       The raw data of the event. self.data is the read only
        #       property for self._data.

        self._event_status_byte = None
        self._data = None

        self._status = None
        self.delta_miditicks = None
        self.delta_us = None
        self.timestamp_us = None

    def _set( self, event_status, data, delta_miditicks ):
        # Set event information to the event status byte, data and
        # delta time (in miditicks) specified in the parameters, replacing all
        # previous information in the event (if there was any).

        # event_status: contains the event status byte as found in the file.
        # In case of midi channel events, lower nibble has the channel number.
        # For meta messages, the first byte of the meta status is 0xff,
        # so the event_status contains the second byte or "meta event type"
        # with values from 0 to 0x7f (_FIRST_META_EVENT to _LAST_META_EVENT).

        # data: a buffer with the raw data of the event.

        # delta_miditicks: the delta time (time difference with previous event)
        # in midi ticks (or pulses).

        # Store event status byte and compute event.status
        self._event_status_byte = event_status
        if _FIRST_CHANNEL_EVENT <= event_status <= _LAST_CHANNEL_EVENT:
            self._status = event_status & 0xf0
        else:
            self._status = event_status

        self._data = data
        self.delta_miditicks = delta_miditicks
        self.delta_us = None

        return self


    def _set_end_of_track( self ):
        self._set( END_OF_TRACK, b'', 0 )
        self.delta_us = 0
        return self

    def _check_property_available( self, *argv ):
        # This method is used to check availabilty of a property.
        # Check if self._status is in list of possible status values
        # and raises AttributeError if not.
        if self._status not in argv:
            raise AttributeError(
                    f"Midi event 0x{self._status:02x}"
                    " does not support attribute")


    def _get_event_name( self ):
        # This metod is used by __str___.
        # Computes the event name as a string. To keep memory
        # requirements at a minimum, instead of having a dictionary of
        # names, this method uses the global variables of thid
        # module as dictionary.

        # Make a dictionary out of the global names of this module
        # Exclude private names starting with _ and
        # exclude names that don't translate to an integer
        event_names_dict = { globals()[varname] : varname.lower() \
                             for varname in globals() \
                             if isinstance(globals()[varname], int) \
                             and varname[0:1] != "_" }

        try:
            name = event_names_dict[self._status]
        except KeyError:
            # Show meaningful information for custom event numbers
            if _FIRST_META_EVENT <= self._status <= _LAST_META_EVENT:
                name = f"meta_0x{self._status:02x}"
            else:
                name = f"midi_0x{self._status:02x}"
        return name

    def _get_property_dict( self ):
        # This is used by __str__
        # Get values for allvalid @properties for
        # this event, except the "data" property

        property_dict = {}
        for prop in dir(MidiEvent):
            if prop[0:1] != "_":
                try:
                    value = getattr( self, prop )
                    # Filter methods from the list
                    if isinstance(value,(int,str)):
                        property_dict[prop] = getattr(self, prop )
                except AttributeError:
                    pass
        return property_dict

    def __str__( self ):
        """
        Standard method to translate the event information to a string,
        """
        description = self._get_event_name()
        # Add event time attributes
        description += " delta[miditicks]=" + str( self.delta_miditicks )
        # Add data, show only a couple of bytes of data
        description += " data=" + str( bytes( self._data[0:5] ) )
        if len(self._data) > 5:
            # Show only first 5 bytes in the data field.
            description = description[0:-1] + "...'"

        # Show time in microseconds only if already computed
        if self.delta_us is not None:
            description += " delta[usec]=" + str(self.delta_us)

        # Get all @property names and their values
        for prop, value in self._get_property_dict().items():
            description += " " + prop + "=" + str(value)
        return description

    @property
    def status( self ):
        """
        Returns the event status. For midi channel events, such as note on, note off,
        program change, the lower nibble (lower 4 bits) are cleared (set to zero).
        For a meta event, this is the meta type, for example 0x2f for "end of track".
        This is the event type: note on, note off, meta text, etc.
        """
        return self._status

    @property
    def channel( self ):
        """
        Returns the channel number for the event, 0-15.

        channel property available for:  NOTE_OFF NOTE_ON
        POLYTOUCH CONTROL_CHANGE PROGRAM_CHANGE AFTERTOUCH
        CHANNEL_PREFIX
        """
        if _FIRST_CHANNEL_EVENT<= self._status <= \
             _LAST_CHANNEL_EVENT:
            # For midi event status byte 0x80 to 0xef,
            # the channel is part of the event status byte
            return self._event_status_byte & 0x0f
        if self._status == CHANNEL_PREFIX:
            return self._data[0]
        raise AttributeError


    @property
    def note( self ):
        """
        Returns the note number for the event, usually 0-127.

        note property available for:  NOTE_OFF NOTE_ON POLYTOUCH
        """
        self._check_property_available( NOTE_ON,
                                        NOTE_OFF,
                                        POLYTOUCH )
        return self._data[0]

    @property
    def velocity( self ):
        """
        Returns the velocity fot the event, usually 0-127.

        velocity property available for:  NOTE_OFF NOTE_ON
        """

        self._check_property_available( NOTE_ON, NOTE_OFF )
        return self._data[1]

    @property
    def value( self ):
        """
        Returns the the value in the event.

        value property available for:  AFTERTOUCH, CONTROL_CHANGE, POLYTOUCH
        """
        if self._status == AFTERTOUCH:
            return self._data[0]
        if self._status in ( CONTROL_CHANGE, POLYTOUCH):
            return self._data[1]
        raise AttributeError

    @property
    def pitch( self ):
        """
        Returns the pitch for a PITCHWHEEL midi channel event.

        -8192 is the lowest value possible, 0 (zero) means "no pitch bend"
        and 8191 is the highest possible value.
        """
        self._check_property_available( PITCHWHEEL )
        # lsb (0 - 127) and msb (0 - 127) together form a 14-bit number,
        # allowing fine adjustment to pitch.
        # Using hex, 00 40 is the central (no bend) setting.
        # 00 00 gives the maximum downwards bend, and 7F 7F the
        # maximum upwards bend.
        # Return 0 for no bend/central bend, -8192 to -1 for downward
        # bend and -1 to 8191 for upward bend
        return (((self._data[1]&0x7f)-0x40)<<7)| (self._data[0]&0x7f)

    @property
    def program( self ):
        """
        Returns the program number 0-127 for a PROGRAM_CHANGE event.
        """
        self._check_property_available( PROGRAM_CHANGE )
        return self._data[0]

    @property
    def control( self ):
        """
        Returns the value for the controller 0-127 for a CONTROL_CHANGE event.
        """
        self._check_property_available( CONTROL_CHANGE )
        return self._data[0]

    @property
    def number( self ):
        """
        Returns number of a SEQUENCE_NUMBER meta event.
        Values range from 0 to 2**24.
        """
        self._check_property_available(  SEQUENCE_NUMBER )
        # Meta event sequence number has a 2 byte big endian number
        return int.from_bytes(self._data[0:2], "big" )

    @property
    def text( self ):
        """
        Returns the text for a meta events.

        text property is available for:  TEXT COPYRIGHT LYRICS MARKER CUE_MARKER

        Both event.text and event.name decode the data. Non ASCII
        characters are shown for example as \xa5
        
        """
        self._check_property_available( TEXT,
                                        COPYRIGHT,
                                        LYRICS,
                                        MARKER,
                                        CUE_MARKER )

        return decode_ascii( self.data )

    @property
    def name( self ):
        """
        Returns the text for a meta events.

        name property available for:  TRACK_NAME INSTRUMENT_NAME PROGRAM_NAME DEVICE_NAME

        See text property for description of text conversion.

        The raw data can be retrieved using the data property.
        """
        self._check_property_available( TRACK_NAME,
                                        INSTRUMENT_NAME,
                                        PROGRAM_NAME,
                                        DEVICE_NAME )
        return decode_ascii( self.data )

    @property
    def port( self ):
        """
        Returns the port number  0-256 for a meta MIDI_PORT message
        """
        self._check_property_available( MIDI_PORT )
        # Meta port event
        return self._data[0]

    @property
    def tempo( self ):
        """
        Returns the tempo (0 to 2**32 microseconds per quarter beat)
        for a SET_TEMPO meta event.
        This module interprets the tempo event before returning it, so
        the following events returned will have their delta_us property
        calculated with the new tempo value.

        """
        self._check_property_available( SET_TEMPO )
        # Meta tempo event, 4 bytes big endian with tempo
        # in microseconds per quarter note or beat
        return int.from_bytes( self._data[0:3], "big")

    @property
    def key( self ):
        """
        Returns the key, as str, for a KEY_SIGNATURE meta event.
  
        For mayor keys:
        C, D, E, F, G, A, B, C#, F#, Cb, Db, Eb, Gb, Ab

        For minor keys:
        Cm, Dm, Em, Fm, Gm, Am, Bm, C#m, F#m, Cbm, Dbm, Ebm, Gbm, Abm

        If the midi message contains a value out of range, a ValueError
        is raised. The raw data can be read with the data property.
        """
        self._check_property_available( KEY_SIGNATURE )
        # Translate data of key meta messages to scale name
        # 2 data bytes: sharps/flats and mayor/minor
        # sharps/flats: 0=no flats/sharps, 1 to 7 number of sharps, -1 to -7 number of flats
        # mayor/minor: 0=mayor, 1=minor
        sharps_flats = self._data[0]
        minor_mayor = self._data[1]

        if sharps_flats > 128:
            sharps_flats -= 256
        if sharps_flats not in range(-7,8) \
                or minor_mayor not in [0,1]:
            raise ValueError(
                    "Midi file format error, key signature meta unrecogized data" )
        if minor_mayor == 0:
            scale_names = ("Cb", "Gb", "Db", "Ab", "Eb", "Bb", "F",
                    "C",
                    "G", "D", "A", "E", "B", "F#", "C#"  )
        else:
            scale_names = ( "Abm", "Ebm", "Bbm", "Fm", "Cm", "Gm", "Dm",
                "Am",
                "Em", "Bm", "F#m", "C#m", "G#m", "D#m", "A#m" )
        return scale_names[sharps_flats+7]

    # Time signature meta message
    @property
    def numerator( self ):
        """
        Returns the numerator for the TIME_SIGNATURE meta messages, 0-255.
        """
        self._check_property_available( TIME_SIGNATURE )
        return self._data[0]

    @property
    def denominator( self ):
        """
        Returns the denominator for the TIME_SIGNATURE meta messages, 0-255.
        """
        self._check_property_available( TIME_SIGNATURE )
        return 2**self._data[1]

    @property
    def clocks_per_click( self ):
        """
        Returns the clocks_per_click for the TIME_SIGNATURE meta messages, 0-255.
        """
        self._check_property_available( TIME_SIGNATURE )
        return self._data[2]

    @property
    def notated_32nd_notes_per_beat( self ):
        """
        Returns the notated_32nd_notes_per_beat for the TIME_SIGNATURE meta messages,
        0-255.
        """
        self._check_property_available( TIME_SIGNATURE )
        return self._data[3]

    @property
    def frame_rate( self ):
        """
        Returns the frame for the SMPTE_OFFSET meta messages,
        which can be 24, 25, 29.97 or 30.

        An invalid value in the MIDI file will raise a IndexError
        """
        self._check_property_available( SMPTE_OFFSET )
        return [24,25,29.97,30][(self._data[0] >> 5)]

    @property
    def hours( self ):
        """
        Returns the hour for the SMPTE_OFFSET meta message,
        usually from 0 to 23.
        """
        self._check_property_available( SMPTE_OFFSET )
        return self._data[0] & 0x1f

    @property
    def minutes( self ):
        """
        Returns the minutes for the SMPTE_OFFSET meta message,
        usually from 0 to 59.
        """
        self._check_property_available( SMPTE_OFFSET )
        return self._data[1]

    @property
    def seconds( self ):
        """
        Returns the seconds for the SMPTE_OFFSET meta message,
        usually from 0 to 59.
        """
        self._check_property_available( SMPTE_OFFSET )
        return self._data[2]

    @property
    def frames( self ):
        """
        Returns the frames for the SMPTE_OFFSET meta message,
        usually from 0 to 255.
        """
        self._check_property_available( SMPTE_OFFSET )
        return self._data[3]

    @property
    def sub_frames( self ):
        """
        Returns the sub frames for the SMPTE_OFFSET meta message,
        usually from 0 to 59.
        """
        self._check_property_available( SMPTE_OFFSET )
        return self._data[4]

    @property
    def data( self ):
        """
        Returns the raw data for the underlying message, with no transofrmations,
        as a memoryview, without the event status byte or meta prefix.
        """

        return self._data

    def copy( self ):
        """
        Returns a deep copy (a complete independent copy) of the event.
        """

        my_copy = MidiEvent()
        my_copy._event_status_byte = self._event_status_byte
        my_copy._status = self._status
        my_copy._data = bytearray( self._data )
        my_copy.delta_miditicks = self.delta_miditicks
        my_copy.delta_us = self.delta_us
        my_copy.timestamp_us = self.timestamp_us
        return my_copy

    def is_meta( self ):
        """
        Returns True if this is a Meta event, such as
        lyrics, set tempo or key signature.
        Returns False if this is a MIDI channel event,
        or a Sysex or Escape event.
        """
        return _FIRST_META_EVENT <= self._status <= _LAST_META_EVENT

    def is_channel( self ):
        """
        Returns True if this event is a channel event
        """
        return _FIRST_CHANNEL_EVENT <= self._status <= _LAST_CHANNEL_EVENT

    def to_midi( self ):
        """
        Returns the event as bytes, in a format that allows sending the
        data to a MIDI controller.

        to_midi will raise AttributeError if the event is for MIDI meta messages, these
        occur in MIDI files and are not normally sent to MIDI controllers.

        This function has not been tested with a real MIDI device.
        """
        if self.is_meta():
            raise AttributeError
        return self._event_status_byte.to_bytes( 1, "big") + self._data



class MidiTrack:
    """
    This object contains the track of a midi file. It is
    created internally by the MidiFile function for each track
    chunk found in the midi file.

    MidiTrack objects are accessible via the MidiFile.tracks list

    """
    def __init__( self,
                file_object,
                filename,
                reuse_event_object, 
                buffer_size,
                miditicks_per_quarter ):
        """
        The MidiTrack cosntructor is called internally by MidiFile,
        you don't need to create a MidiTrack.
        """

        # Parameters are:
        # file_object: This is the currently opened midi file, positioned at the start
        #   of this track's data, 
        #   just before the 4 bytes with the track or chunk length.
        # filename: the file name of file_object.
        #   file_object.name not available on CircuitPython
        self._reuse_event_object = reuse_event_object
        self._miditicks_per_quarter = miditicks_per_quarter
        self._buffer_size = buffer_size
        
        # MTrk header in file has just been processed, get chunk length
        self._track_length = int.from_bytes( file_object.read(4), "big" )

        if buffer_size <= 0:
            # Read the entire track data to RAM
            self._track_data = file_object.read( self._track_length )
        else:
            # Store filename and start position to open file when interation over track data starts
            self._filename = filename
            self._start_position = file_object.tell()
            
            # Skip rest of track chunk, fast forward to beginning of next track
            file_object.seek( self._track_length, 1 )

        self._track_parser = None
        self.event = None
        self.current_miditicks = None

    def _buffered_data_generator( self ):
        # Generator to return byte by byte from a buffered track
        # (a track entirely in memory, buffer_size=0)
        # This way seems to be rather fast:
        return ( data_byte for data_byte in self._track_data )


    def _file_data_generator( self ):
        # Generator to return byte by byte of a track with
        # buffer_size>0. Reads portions of n bytes and then returns
        # byte by byte.

        # Allocate buffer to be reused for each read
        buffer = bytearray( self._buffer_size )

        # Open file again to read the track
        with open( self._filename, "rb") as file:
            file.seek( self._start_position )
            unread_bytes = self._track_length
            while True:
                # Read a buffer of data and yield byte by byte to caller
                bytes_read = file.readinto( buffer )
                yield from memoryview(buffer)[0:bytes_read]
                # Check if end of track reached
                unread_bytes -= bytes_read
                if unread_bytes <= 0 or bytes_read == 0:
                    return


    def _get_midi_data( self ):
        # Choose method to return data
        if self._buffer_size <= 0:
            return self._buffered_data_generator
        return self._file_data_generator

    def __iter__( self ):
        """
        Iterating through a track will yield all events of that track
        of MIDI file. For example, to parse the first track in a midi file:

            for event in MidiFile("example.mid").track[0]:
                .... process event ...

        event.delta_miditicks will have the time difference with the previous
        event, in MIDI ticks (pulses).

        event.delta_us is calculated as the time difference with the previous event
        in microseconds. For this calculation, the set tempo events and
        the MIDI ticks per quarter note (also called "pulses per beat")
        of the MIDI file header are taken into consideration.

        The last event will always be a END_OF_TRACK event, if missing in the file.

        """
        # Get the parser to return event by event, process set tempo meta events,
        # calculate delta_us and ensure END_OF_TRACK present at the end.
        # This is used to parse a single track, for multitrack processing _track_parse_start
        # method is used
        return _process_events(
                MidiParser( iter(self._get_midi_data()()) ).parse_events(),
                self._miditicks_per_quarter,
                self._reuse_event_object )

    # _track_parse_start and _track_parse_next are an iterator used
    # to merge tracks. Instead of just iterationg, they also keep track of the
    # sum of midi ticks in thr track. They allow comparing tracks to know which
    # has the next event.
    def _track_parse_start( self ):
        # This is an internal method called by MidiFile for multitrack processing.
        self._track_parser = MidiParser( iter(self._get_midi_data()()) ).parse_events()

        # Get first event to get things going...
        self.event = next( self._track_parser )
        self.current_miditicks = self.event.delta_miditicks

        return self


    def _track_parse_next( self ):
        # Used internally by MidiFile object.
        # After doing a _track_parse_start, this will return the next event in track.
        self.event = next( self._track_parser )
        self.current_miditicks += self.event.delta_miditicks
        return self.event


    def __lt__( self, compare_to ):
        """
        Used internally by the min function to compare the current time in miditicks
        of the different tracks, the goal is to find the next midi event
        of all tracks (the one with the smallest time since the beginning of the track)
        """
        # Valid after _track_parse_start, in conjunction with _track_parse_next.
        return self.current_miditicks < compare_to.current_miditicks

    def _get_current_miditicks(self):
        return self.current_miditicks

    def play( self ):
        """
        Plays the track. Intended for use with format 2 MIDI files.
        Sleeps between events, yielding the events on time.
        See also MidiFile.play.
        
        """
        return MidiPlay( self )
        
class MidiFile:
    """
    Parses a MIDI file.
    """
    def __init__( self,
                  filename,
                  buffer_size=100,
                  reuse_event_object=False ):
        """
        filename
        The name of a MIDI file, usually a .mid or .rtx MIDI file.

        buffer_size=100
        The buffer size that will be allocated for each track, 0=read
        complete track to memory.
        
        reuse_event_object=False
        True will reuse the event object during parsing, using less RAM.

        Returns an iterator over the events in the MIDI file.
        """

        # Store parameters
        self._reuse_event_object = reuse_event_object
        self._buffer_size = buffer_size

        # Process file
        with open( filename, "rb" ) as file:

            # First chunk must be MThd midi header, process header and validate
            self._format_type, \
                number_of_chunks, \
                self._miditicks_per_quarter = self._get_header( file )

            self._filename = filename 

            # Get all track objects of the file.
            # Disregard the number of chunks, read the real number of tracks present.
            self.tracks = []
            for _ in range(number_of_chunks):
                track_id = file.read(4).decode( "latin-1" )
                # Only process MTrk chunks
                if track_id == "MTrk":
                    self.tracks.append( MidiTrack(     file, 
                         filename,
                         reuse_event_object,
                         buffer_size, 
                         self._miditicks_per_quarter) )
                else:
                    # Skip non-track chunk,
                    # use MidiTrack but ignore result
                    MidiTrack( file,
                          filename,
                          reuse_event_object,
                          10,
                          self._miditicks_per_quarter )


    def _get_header( self, file ):
        # Decodes the MIDI file header, returns the
        # values of the header:
        # format type (0-2), number of data chunks, MIDI ticks per quarter note

        track_id = file.read(4).decode( "latin-1" )
        if track_id != "MThd":
            # It is said that Mac midi files may have 128 extra
            # bytes prepended (i.e. a Mac BInary Header)
            # Just in case, skip bytes until 128 bytes have been ignored,
            # then read track_id again
            # I have not been able to verify this.
            file.read(128-4)
            track_id = file.read(4).decode( "latin-1" )
            if track_id != "MThd":
                raise ValueError("Midi file does not start with MThd header")
        header_len = int.from_bytes( file.read(4), "big" )

        if header_len < 6:
            raise ValueError(
                f"Midi file header MThd length ({header_len}) is smaller than 6 bytes"
                )
            # IF header is larger than 6 bytes, the extra bytes are ignored.

        header_data = file.read( header_len )

        # Format type 0: single MTrk chunk
        # Format type 1: two or more MTrk chunks to be merged
        # Format type 2: multiple MTrk chunks, each to be used separately
        format_type = int.from_bytes( header_data[0:2], "big" )

        # Get number of data chunks (track chunks) in the file
        number_of_chunks = int.from_bytes( header_data[2:4], "big" )

        # Get pulses per beat
        miditicks_per_quarter = int.from_bytes( header_data[4:6], "big" )

        if miditicks_per_quarter > 32767:
            raise ValueError("Midi SMPTE time codes not supported")

        return format_type, number_of_chunks, miditicks_per_quarter

    @property
    def format_type( self ):
        """
        Returns the MIDI format type as stored in the header of the MIDI file:
        """
        return self._format_type

    @property
    def miditicks_per_quarter( self ):
        """
        Return the midi ticks per quarter note (also called pulses per beat)
        parameter in the MIDI header of the file.
        """
        return self._miditicks_per_quarter

    @property
    def filename( self ):
        """
        Return the file name of the MIDI file, with absolute path.
        """
        return self._filename

    @property
    def buffer_size( self ):
        """
        Return the buffer_size value. 0=tracks are buffered entirely in RAM.
        A number, for example buffer_size=100 means a buffer of 100 bytes is
        allocated per track to read the track data.

        This allows to read large MIDI files efficiently on microcontrollers with small RAM.
        """
        return self._buffer_size

    @property
    def reuse_event_object( self ):
        """
        Return the value of reuse_event_object.
        True: when iterating through a track or midi file, the same event object
        is returned over and over (this is an optimization recommended for Micropython)

        False: when iterating through a track or midi file, a different event
        object is returned each time (this is the typical Python behavior).
        """
        return self._reuse_event_object

    def _track_merger( self ):
        # Merges all tracks of a multitrack format 1 file

        # Iterate through each track, set up one iterator for each track
        # For this code to work, the track interator will always yield
        # a END_OF_TRACK event at the end of the track.
        play_tracks = [ track._track_parse_start() for track in self.tracks ]

        # Current miditicks keeps the time, in MIDI ticks, since start of track
        # of the last event returned
        current_miditicks = 0

        while True:
            # From all tracks, select the track with the next event, this is
            # the one with the lowest "current MIDI ticks time".
            next_track = min( play_tracks ) # Uses the __lt__ function of track

            # Get the current event of the selected track
            event = next_track.event

            # Adjust event miditicks to time difference with last event overall,
            # replacing delta time with last event in the event's track
            track_miditicks = next_track._get_current_miditicks()
            event.delta_miditicks = track_miditicks - current_miditicks

            # If end_of_track is seen, don't continue to process this track
            if event.status == END_OF_TRACK:
                # Delete the track from the list of tracks being processed
                track_index = play_tracks.index(next_track)
                del play_tracks[track_index]

                # If all tracks have ended, stop processing file
                if len(play_tracks) == 0:
                    # Yield only the last end_of_track found
                    yield event
                    # And stop iteration
                    return

                # Don't yield end of track events (except for the last track)
                continue

            yield event

            # Update current time, this is now the time of the event
            # just returned
            current_miditicks = track_miditicks

            # Get  the next event of the selected track.
            # This has to be done after the yield, because this might
            # overwrite the yielded message if reuse_event_object=True.
            next_track._track_parse_next()


    def __iter__( self ):
        """
        To get all the events of a format type 0 or format type 1 MIDI file,
        iterate through the MidiFile object, for example:
            for event in MidiFile("example.mid"):
                print(event)

        """
        # Check if there are tracks, there should be....
        if len(self.tracks) == 0:
            # No tracks in file, simulate an empty track
            # This will yield a single END_OF_TRACK event.
            return _process_events( iter([]),
                    self._miditicks_per_quarter,
                    self._reuse_event_object )

        # Iterate over track instead of file for format 2 files
        if self._format_type == 2 and len(self.tracks) > 1:
            raise RuntimeError(
                    "It's not possible to merge tracks of a MIDI format type 2 file")

        # If there is only one track present,
        # iterate through the track only, no track merge is needed.
        # This reduces CPU usage, in some cases by up to 15%.
        if len(self.tracks) == 1:
            return iter(self.tracks[0])

        # For multitrack file type 1 files, tracks must be merged.
        # Type 0 files with many tracks (not standard) are merged too.
        return _process_events( self._track_merger(),
                    self._miditicks_per_quarter,
                    self._reuse_event_object )

    def length_us( self ):
        """
        Returns the length of the MidiFile in microseconds.
        """
        # Returns the duration of playback time of the midi file microseconds

        # Start playing time at 0, in case there are no events
        playback_time_us = 0

        # Iterate through all events
        # The complete file must be processed to compute length
        # Open another instance of the file, so that the current process is not disturbed
        for event in MidiFile( self.filename ):
            playback_time_us += event.delta_us

        # Return the last time seen, or 0 if there were no events
        return playback_time_us

    def play( self ):
        """
        Iterate through the events of a MIDI file or a track,
        sleep until the event has to take place, and
        yield the event. Playing time is measured always from the start
        of file, correcting a possible accumulation of timing errors.
        """
        return MidiPlay( self )
        
        
class MidiPlay:
    """
    Internal class used to play a MIDI file waiting after each event for the next one.
    Use: MidiPlay( instance_of_MidiFile ) or MidiPlay( instance_of_MidiTrack )
    Uses the __iter__/__next__ functions of MidiFile and MidiTrack to iterathe over the events.
    """
    def __init__( self, midi_event_source ):
        self.midi_event_source =  midi_event_source 
    
        
    def get_event_generator( self ):
        # Generator to iterate over the events and calculate the wait time
        # for each event. Wait time is corrected by adjusting with real time compared
        # to time since start of file.
        playing_started_at = time_now_us()
        midi_time = 0
        for event in self.midi_event_source:
            midi_time += event.delta_us
            now = time_now_us()
            playing_time = time_diff_us( now, playing_started_at )
            event.timestamp_us = midi_time
            yield (event, midi_time - playing_time)

    def __iter__( self ):
        self.iterator = iter(  self.get_event_generator() )
        return self
        
    def __next__( self ):
        event, wait_time = next( self.iterator )
        if wait_time > 0:
            time_sleep_us( wait_time )
        return event
    
    def __aiter__( self ):
        return self.__iter__()
        
    async def __anext__( self ):
        # asyncio version of __next__
        try:
            event, wait_time = next( self.  iterator )
        except StopIteration:
            raise StopAsyncIteration
        # If wait time <= 0, execute asyncio.sleep anyhow to yield control to other tasks
        await asyncio_sleep_ms( max(wait_time//1_000,0) )
        return event
                    
