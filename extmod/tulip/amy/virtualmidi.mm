/* -*- Mode: ObjC; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; fill-column: 150 -*- */

// compile with clang virtualmidi.mm  -framework CoreMIDI -framework Cocoa -lstdc++ -o vm 


#import <array>
#import <cassert>
#import <cstdio>

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreMidi/CoreMidi.h>


static CGEventSourceRef eventSource;

static void NotifyProc(const MIDINotification *message, void *refCon)
{
}


extern "C" void *mac_midi_run(void *vargp){
    @autoreleasepool {
        MIDIClientRef midi_client;
        OSStatus status = MIDIClientCreate((__bridge CFStringRef)@"VirtualAlles", NotifyProc, nullptr, &midi_client);
        if (status != noErr) {
            fprintf(stderr, "Error %d while setting up handlers\n", status);
        }
        printf("macOS Virtual MIDI running. You can optionally connect to IAC to control the mesh from this host.\n");
        eventSource = CGEventSourceCreate(kCGEventSourceStatePrivate);
        ItemCount number_sources = MIDIGetNumberOfSources();
        for (int i = 0; i < number_sources; i++) {
            MIDIEndpointRef source = MIDIGetSource(i);
            MIDIPortRef port;
            status = MIDIInputPortCreateWithProtocol(midi_client, (__bridge CFStringRef)[NSString stringWithFormat:@"VirtualAlles input %d", i], kMIDIProtocol_1_0, &port, ^(const MIDIEventList *evtlist, void *srcConnRefCon) {
                const MIDIEventPacket* packet = &evtlist->packet[0];
                for (int i = 0; i < evtlist->numPackets; i++) {
                    if(packet->wordCount == 1){
                        const unsigned char *bytes = reinterpret_cast<const unsigned char *>(&packet->words[0]);
                        if(bytes[3] == 0x20) {
                            printf("Event: %02X %02X %02X\n", bytes[2], bytes[1], bytes[0]);
                            switch ((bytes[2] & 0xF0) >> 4) {
                                case 0x9: // Note-On
                                    assert(bytes[1] <= 0x7F);
                                    printf("note on %d %d\n", (bytes[2] & 0x0F) * 128 + bytes[1], bytes[0]);
                                    break;
                                case 0x8: // Note-Off
                                    assert(bytes[1] <= 0x7F);
                                    printf("note off %d %d\n", (bytes[2] & 0x0F) * 128 + bytes[1], bytes[0]);
                                    break;
                                case 0xB: // Control Change
                                    assert(bytes[1] <= 0x7F);
                                    printf("CC %d %d\n", (bytes[2] & 0x0F), bytes[0] );
                                    break;
                            }
                            packet = MIDIEventPacketNext(packet);
                        }
                    }
                }
            });
            if (status != noErr) {
                fprintf(stderr, "Error %d while setting up port\n", status);
            }
            status = MIDIPortConnectSource(port, source, nullptr);
            if (status != noErr) {
                fprintf(stderr, "Error %d while connecting port to source\n", status);
            }
        }
        CFRunLoopRun();
    }
    return NULL;
}
