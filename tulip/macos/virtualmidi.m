/* -*- Mode: ObjC; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; fill-column: 150 -*- */

// compile with clang virtualmidi.mm  -framework CoreMIDI -framework Cocoa -lstdc++ -o vm 


//#import <array>
//#import <cassert>
//#import <cstdio>

#define unichar OSX_UNICHAR
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreMidi/CoreMidi.h>
#undef unichar
#import "midi.h"

static CGEventSourceRef eventSource;
MIDIClientRef midi_client;
MIDIPortRef out_port;


static void NotifyProc(const MIDINotification *message, void *refCon)
{
}


void midi_out(uint8_t * bytes, uint16_t len) {
    if (@available(macOS 11, *))  {
        fprintf(stderr, "midi out len %d\n",len);
        // do something with out_port
        
    } else {
        fprintf(stderr, "Can only run MIDI on macOS Big Sur (11.0) or later, sorry\n");   
    }
}


void* run_midi(void*argp){
    if (@available(macOS 11, *))  {
        @autoreleasepool {
            //py_midi_callback = 0;

            OSStatus status = MIDIClientCreate((__bridge CFStringRef)@"Tulip", NotifyProc, NULL, &midi_client);
            if (status != noErr) {
                fprintf(stderr, "Error %d while setting up handlers\n", status);
            }


            status = MIDIOutputPortCreate(midi_client, (__bridge CFStringRef)[NSString stringWithFormat:@"Tulip Output"], &out_port );
            if(status != noErr) {
                fprintf(stderr, "Error %d while setting up MIDI output port\n", status);
            }

            eventSource = CGEventSourceCreate(kCGEventSourceStatePrivate);
            ItemCount number_sources = MIDIGetNumberOfSources();
            for (unsigned long i = 0; i < number_sources; i++) {
                MIDIEndpointRef source = MIDIGetSource(i);
                MIDIPortRef in_port;
                status = MIDIInputPortCreateWithProtocol(midi_client, (__bridge CFStringRef)[NSString stringWithFormat:@"Tulip Input %lu", i], kMIDIProtocol_1_0, &in_port, ^(const MIDIEventList *evtlist, void *srcConnRefCon) {
                    for (uint32_t i = 0; i < evtlist->numPackets; i++) {
                        const MIDIEventPacket* packet = &evtlist->packet[i];
                        for(uint32_t j=0 ; j < packet->wordCount; j++) {
                            const unsigned char *bytes = (unsigned char*)(&packet->words[j]);
                            if(bytes[3] == 0x20) { //  TODO, non-3 packets, then what? 
                                uint8_t data[3];
                                data[0] = bytes[2];
                                data[1] = bytes[1];
                                data[2] = bytes[0];
                                convert_midi_bytes_to_messages(data, 3);
                            } else {
                               //printf("bytes[3] was not 0x20\n");
                            }
                        }
                    }
                });
                if (status != noErr) {
                    fprintf(stderr, "Error %d while setting up MIDI input port\n", status);
                }
                status = MIDIPortConnectSource(in_port, source, NULL);
                if (status != noErr) {
                    fprintf(stderr, "Error %d while connecting MIDI input port to source\n", status);
                }
            }
            CFRunLoopRun();
        }
    } else {
        fprintf(stderr, "Can only run MIDI on macOS Big Sur (11.0) or later, sorry\n");
    }
    return NULL;
}
